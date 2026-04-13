// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_MATRIX_H
#define RAYTRACER_MATRIX_H
#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>

namespace rt {

template <std::size_t Rows, std::size_t Cols>
class Matrix {
  static_assert(Rows > 0 && Cols > 0,
                "Matrix dimensions must be greater than 0.");

  static constexpr std::size_t kRows = Rows;
  static constexpr std::size_t kCols = Cols;

  std::array<float, kRows * kCols> data_{};

 public:
  Matrix() = default;

  constexpr Matrix(std::initializer_list<float> values) {
    assert(values.size() == kRows * kCols);
    std::copy(values.begin(), values.end(), data_.begin());
  }

  [[nodiscard]] constexpr float& operator()(std::size_t const row,
                                            std::size_t const col) noexcept {
    return data_[row * Cols + col];
  }

  [[nodiscard]] constexpr float const& operator()(
      std::size_t const row, std::size_t const col) const noexcept {
    return data_[row * Cols + col];
  }

  [[nodiscard]] auto begin() noexcept { return data_.begin(); }
  [[nodiscard]] auto end() noexcept { return data_.end(); }
  [[nodiscard]] auto begin() const noexcept { return data_.begin(); }
  [[nodiscard]] auto end() const noexcept { return data_.end(); }
  [[nodiscard]] auto cbegin() const noexcept { return data_.cbegin(); }
  [[nodiscard]] auto cend() const noexcept { return data_.cend(); }

  [[nodiscard]] static constexpr Matrix Identity()
    requires(Rows == Cols)
  {
    Matrix result{};
    for (std::size_t i = 0; i < Rows; ++i) {
      result(i, i) = 1.0f;
    }
    return result;
  }
};

template <std::size_t Rows, std::size_t Cols>
Matrix<Rows, Cols>& operator+=(Matrix<Rows, Cols>& lhs,
                               Matrix<Rows, Cols> const& rhs) {
  auto dst = lhs.begin();
  auto src = rhs.cbegin();
  auto const src_end = rhs.cend();
  for (; src != src_end; ++src, ++dst) {
    *dst += *src;
  }

  return lhs;
}

template <std::size_t Rows, std::size_t Cols>
Matrix<Rows, Cols> operator+(Matrix<Rows, Cols> lhs,
                             Matrix<Rows, Cols> const& rhs) noexcept {
  lhs += rhs;
  return lhs;
}

template <std::size_t Rows, std::size_t Cols>
Matrix<Rows, Cols>& operator-=(Matrix<Rows, Cols>& lhs,
                              Matrix<Rows, Cols> const& rhs) {
  auto dst = lhs.begin();
  auto src = rhs.cbegin();
  auto const src_end = rhs.cend();
  for (; src != src_end; ++src, ++dst) {
    *dst -= *src;
  }

  return lhs;
}

template <std::size_t Rows, std::size_t Cols>
Matrix<Rows, Cols> operator-(Matrix<Rows, Cols> lhs,
                             Matrix<Rows, Cols> const& rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

// TODO: Profile matrix multiplication and potentially implement strassen
// algorithm or SSE for 4x4 & 3x3
template <std::size_t ARows, std::size_t ACols, std::size_t BCols>
Matrix<ARows, BCols> operator*(Matrix<ARows, ACols> lhs,
                               Matrix<ACols, BCols> const& rhs) noexcept {
  Matrix<ARows, BCols> matrix;

  for (std::size_t row = 0; row < ARows; ++row) {
    for (std::size_t col = 0; col < BCols; ++col) {
      float sum = 0.0f;
      for (std::size_t i = 0; i < ACols; ++i) {
        sum += lhs(row, i) * rhs(i, col);
      }
      matrix(row, col) = sum;
    }
  }

  return matrix;
}

template <std::size_t Rows, std::size_t Cols>
[[nodiscard]] float NormSquared(Matrix<Rows, Cols> const& matrix) {
  return std::transform_reduce(matrix.cbegin(), matrix.cend(), 0.0f,
                               std::plus{}, [](auto const x) { return x * x; });
}

template <std::size_t Rows, std::size_t Cols>
bool Near(Matrix<Rows, Cols> const& lhs, Matrix<Rows, Cols> const& rhs,
          float const epsilon = 1e-6f) {
  return NormSquared(rhs - lhs) <= epsilon * epsilon;
}

}  // namespace rt

#endif  // RAYTRACER_MATRIX_H

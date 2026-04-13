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
    if (values.size() != kRows * kCols) {
      throw "Matrix initializer_list has wrong size";
    }
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

  [[nodiscard]] constexpr auto begin() noexcept { return data_.begin(); }
  [[nodiscard]] constexpr auto end() noexcept { return data_.end(); }
  [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin(); }
  [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }
  [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }
  [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }

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
[[nodiscard]] float NormSquared(Matrix<Rows, Cols> const& matrix) {
  return std::transform_reduce(matrix.cbegin(), matrix.cend(), 0.0f,
                               std::plus{}, [](auto const x) { return x * x; });
}
}  // namespace rt

#endif  // RAYTRACER_MATRIX_H

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_MATRIX_H
#define RAYTRACER_MATRIX_H
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <numeric>

#include "rt/math/concepts.h"

namespace rt {

template <std::size_t Rows, std::size_t Cols>
  requires(NonZeroDimensions<Rows, Cols>)
class Matrix {
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
  [[nodiscard]] constexpr auto cbegin() const noexcept {
    return data_.cbegin();
  }
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

template <std::size_t Rows, std::size_t Cols>
  requires(SquareMatrix<Rows, Cols>)
[[nodiscard]] float Determinant(Matrix<Rows, Cols> const& matrix) {
  if constexpr (Rows == 1 && Cols == 1) {
    return matrix(0, 0);
  } else if constexpr (Rows == 2 && Cols == 2) {
    return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
  } else {
    float det = 0.0f;
    for (std::size_t col = 0; col < Cols; ++col) {
      det += matrix(0, col) * Cofactor(matrix, 0, col);
    }
    return det;
  }
}

template <std::size_t Rows, std::size_t Cols>
  requires(SubMatrixable<Rows, Cols>)
[[nodiscard]] constexpr Matrix<Rows - 1, Cols - 1> Submatrix(
    Matrix<Rows, Cols> const& matrix, std::size_t const row,
    std::size_t const col) {
  Matrix<Rows - 1, Cols - 1> submatrix;

  for (std::size_t r = 0; r < Rows - 1; ++r) {
    for (std::size_t c = 0; c < Cols - 1; ++c) {
      auto const row_idx = r >= row ? r + 1 : r;
      auto const col_idx = c >= col ? c + 1 : c;
      submatrix(r, c) = matrix(row_idx, col_idx);
    }
  }

  return submatrix;
}

template <std::size_t Rows, std::size_t Cols>
[[nodiscard]] constexpr float Minor(Matrix<Rows, Cols> const& matrix,
                          std::size_t const row, std::size_t const col) {
  return Determinant(Submatrix(matrix, row, col));
}

template <std::size_t Rows, std::size_t Cols>
[[nodiscard]] constexpr float Cofactor(Matrix<Rows, Cols> const& matrix,
                             std::size_t const row, std::size_t const col) {
  auto const minor = Minor(matrix, row, col);
  return (row + col) % 2 == 0 ? minor : -minor;
}

template<std::size_t Rows, std::size_t Cols>
[[nodiscard]] constexpr bool IsSingularMatrix(Matrix<Rows, Cols> const& matrix) {
  auto constexpr epsilon = 1e-6f;
  return std::fabs(Determinant(matrix)) < epsilon;
}

template<std::size_t Rows, std::size_t Cols>
[[nodiscard]] constexpr Matrix<Rows, Cols> Inverse(Matrix<Rows, Cols> const& matrix) {
  auto const determinant = Determinant(matrix);
  if (std::fabs(determinant) < 1e-6f) {
    throw std::runtime_error("Attempting to inverse singular matrix.");
  }

  auto inverse = Matrix<Rows, Cols>{};
  for (std::size_t row =0; row<Rows; ++row) {
    for (std::size_t col=0; col<Cols; ++col) {
      auto const c = Cofactor(matrix, row, col);
      inverse(col, row) = c / determinant;
    }
  }
  return inverse;
}
}  // namespace rt

#endif  // RAYTRACER_MATRIX_H

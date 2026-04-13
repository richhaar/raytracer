// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_TRANSFORMS_H
#define RAYTRACER_TRANSFORMS_H

#include "rt/math/matrix.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"

namespace rt {
template <std::size_t Rows, std::size_t Cols>
concept TransformMatrix = (Rows == Cols) && (Rows == 3 || Cols == 4);

namespace detail {
template <std::size_t Rows, std::size_t Cols>
  requires(TransformMatrix<Rows, Cols>)
[[nodiscard]] constexpr Vector3 LinearTransform(Matrix<Rows, Cols> const& m,
                                                float const x, float const y,
                                                float const z) {
  return {m(0, 0) * x + m(0, 1) * y + m(0, 2) * z,
          m(1, 0) * x + m(1, 1) * y + m(1, 2) * z,
          m(2, 0) * x + m(2, 1) * y + m(2, 2) * z};
}
}  // namespace detail

template <std::size_t Rows, std::size_t Cols>
  requires(TransformMatrix<Rows, Cols>)
constexpr Vector3 operator*(Matrix<Rows, Cols> const& matrix,
                            Vector3 const& vec) {
  return detail::LinearTransform(matrix, vec.x, vec.y, vec.z);
}

template <std::size_t Rows, std::size_t Cols>
  requires(TransformMatrix<Rows, Cols>)
constexpr Point3 operator*(Matrix<Rows, Cols> const& matrix,
                           Point3 const& vec) {
  auto [x, y, z] = detail::LinearTransform(matrix, vec.x, vec.y, vec.z);

  if constexpr (Rows == 3) {
    return {x, y, z};
  }

  return {x + matrix(0, 3), y + matrix(1, 3), z + matrix(2, 3)};
}

template <std::size_t Rows, std::size_t Cols>
constexpr Matrix<Cols, Rows> Transpose(Matrix<Rows, Cols> const& matrix) {
  Matrix<Cols, Rows> transposed;
  for (std::size_t col = 0; col < Rows; ++col) {
    for (std::size_t row = 0; row < Cols; ++row) {
      transposed(row, col) = matrix(col, row);
    }
  }
  return transposed;
}
}  // namespace rt

#endif  // RAYTRACER_TRANSFORMS_H

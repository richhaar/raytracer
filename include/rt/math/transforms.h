// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_TRANSFORMS_H
#define RAYTRACER_TRANSFORMS_H

#include "rt/math/concepts.h"
#include "rt/math/matrix.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"

namespace rt {

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

constexpr Matrix<4, 4> Translation(float const x, float const y,
                                   float const z) {
  return Matrix<4, 4>{1.0f, 0.0f, 0.0f, x, 0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z, 0.0f, 0.0f, 0.0f, 1.0f};
}

constexpr Matrix<4, 4> Scaling(float const x, float const y, float const z) {
  return Matrix<4, 4>{x,    0.0f, 0.0f, 0.0f, 0.0f, y,    0.0f, 0.0f,
                      0.0f, 0.0f, z,    0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}

constexpr Matrix<4, 4> RotateX(float const radians) {
  return Matrix<4, 4>{1.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      std::cos(radians),
                      -std::sin(radians),
                      0.0f,
                      0.0f,
                      std::sin(radians),
                      std::cos(radians),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};
}

constexpr Matrix<4, 4> RotateY(float const radians) {
  return Matrix<4, 4>{std::cos(radians),
                      0.0f,
                      std::sin(radians),
                      0.0f,
                      0.0f,
                      1.0f,
                      0.0f,
                      0.0f,
                      -std::sin(radians),
                      0.0f,
                      std::cos(radians),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};
}

constexpr Matrix<4, 4> RotateZ(float const radians) {
  return Matrix<4, 4>{std::cos(radians),
                      -std::sin(radians),
                      0.0f,
                      0.0f,
                      std::sin(radians),
                      std::cos(radians),
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      0.0f,
                      1.0f};
}

constexpr Matrix<4, 4> Shear(float const xy, float const xz, float const yx,
                             float const yz, float const zx, float const zy) {
  return Matrix<4, 4>{1.0f, xy, xz,   0.0f, yx,   1.0f, yz,   0.0f,
                      zx,   zy, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}
}  // namespace rt

#endif  // RAYTRACER_TRANSFORMS_H

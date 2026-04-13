// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_MATRIX_OPS_H
#define RAYTRACER_MATRIX_OPS_H

#include "rt/math/matrix.h"

namespace rt {

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
bool Near(Matrix<Rows, Cols> const& lhs, Matrix<Rows, Cols> const& rhs,
          float const epsilon = 1e-6f) {
  return NormSquared(rhs - lhs) <= epsilon * epsilon;
}


}  // namespace rt
#endif  // RAYTRACER_MATRIX_OPS_H

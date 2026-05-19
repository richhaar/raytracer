// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_CONCEPTS_H
#define RAYTRACER_CONCEPTS_H

#include <cstddef>

namespace rt {

template <std::size_t Rows, std::size_t Cols>
concept NonZeroDimensions = Rows > 0 && Cols > 0;

static_assert(NonZeroDimensions<4, 4>);
static_assert(NonZeroDimensions<3, 3>);
static_assert(!NonZeroDimensions<3, 0>);
static_assert(!NonZeroDimensions<0, 0>);

template <std::size_t Rows, std::size_t Cols>
concept SquareMatrix = Rows == Cols && NonZeroDimensions<Rows, Cols>;

static_assert(SquareMatrix<4, 4>);
static_assert(SquareMatrix<1, 1>);
static_assert(!SquareMatrix<0, 0>);

template <std::size_t Rows, std::size_t Cols>
concept TransformMatrix = SquareMatrix<Rows, Cols> && (Rows == 3 || Cols == 4);

static_assert(TransformMatrix<4, 4>);
static_assert(TransformMatrix<3, 3>);
static_assert(!TransformMatrix<2, 2>);
static_assert(!TransformMatrix<4, 3>);

template <std::size_t Rows, std::size_t Cols>
concept SubMatrixable = Rows > 1 && Cols > 1;

static_assert(SubMatrixable<3, 4>);
static_assert(SubMatrixable<2, 2>);
static_assert(!SubMatrixable<1, 1>);
static_assert(!SubMatrixable<1, 2>);
}  // namespace rt

#endif  // RAYTRACER_CONCEPTS_H

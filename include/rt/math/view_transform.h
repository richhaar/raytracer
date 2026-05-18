// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_VIEW_TRANSFORM_H
#define RAYTRACER_VIEW_TRANSFORM_H
#include "rt/math/matrix.h"
#include "rt/math/matrix_ops.h"
#include "rt/math/point3.h"
#include "rt/math/transforms.h"

namespace rt {
inline Matrix<4, 4> ViewTransform(Point3 const& from, Point3 const& to,
                                  Vector3 const& up) {
  auto const forward = Normalize(to - from);
  auto const left = Cross(forward, Normalize(up));
  auto const new_up = Cross(left, forward);

  auto const m = Matrix<4, 4>{left.x,     left.y,     left.z,     0.0f,
                              new_up.x,   new_up.y,   new_up.z,   0.0f,
                              -forward.x, -forward.y, -forward.z, 0.0f,
                              0.0f,       0.0f,       0.0f,       1.0f};

  return m * Translation(-from.x, -from.y, -from.z);
}

}  // namespace rt

#endif  // RAYTRACER_VIEW_TRANSFORM_H

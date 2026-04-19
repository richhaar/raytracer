// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
#include "rt/math/matrix.h"
#include "rt/math/transforms.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"

namespace rt {

struct Ray {
  Point3 origin;
  Vector3 direction;
};

inline Point3 Position(Ray const& ray, float const t) {
  return ray.origin + ray.direction * t;
}

inline Ray Transform(Ray const& ray, Matrix<4, 4> const& matrix) {
  return Ray{matrix * ray.origin, matrix * ray.direction};
}
}  // namespace rt

#endif  // RAYTRACER_RAY_H

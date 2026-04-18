// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H
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
}  // namespace rt

#endif  // RAYTRACER_RAY_H

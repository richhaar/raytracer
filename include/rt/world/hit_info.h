// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_HIT_COMPUTATIONS_H
#define RAYTRACER_HIT_COMPUTATIONS_H
#include "hit_record.h"
#include "ray.h"
#include "rt/math/point3.h"
#include "sphere.h"

namespace rt {
struct HitInfo {
  HitRecord hit;
  bool inside = false;
  Point3 point;
  Vector3 eye;
  Vector3 normal;
};

inline HitInfo CalculateHitInfo(HitRecord const& record, Ray const& ray) {
  auto const pos = Position(ray, record.t);
  auto const normal = record.object->NormalAt(pos);
  auto const eye = -ray.direction;
  auto const inside = Dot(normal, eye) < 0.0f;
  return {.hit = record,
          .inside = inside,
          .point = pos,
          .eye = eye,
          .normal = inside ? -normal : normal};
}
}  // namespace rt

#endif  // RAYTRACER_HIT_COMPUTATIONS_H

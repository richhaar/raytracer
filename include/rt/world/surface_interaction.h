// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_SURFACE_INTERACTION_H
#define RAYTRACER_SURFACE_INTERACTION_H
#include "intersection.h"
#include "ray.h"
#include "rt/math/point3.h"
#include "sphere.h"

namespace rt {
struct SurfaceInteraction {
  Intersection intersection;
  bool inside = false;
  Point3 point;
  Vector3 eye;
  Vector3 normal;
  Vector3 reflect;
};

inline SurfaceInteraction ComputeSurfaceInteraction(Intersection const& record, Ray const& ray) {
  auto const pos = Position(ray, record.t);
  auto const normal = record.object->NormalAt(pos);
  auto const eye = -ray.direction;
  auto const inside = Dot(normal, eye) < 0.0f;
  auto const adjusted_normal = inside ? -normal : normal;
  return {.intersection = record,
          .inside = inside,
          .point = pos,
          .eye = eye,
          .normal = adjusted_normal,
          .reflect = Reflect(ray.direction, adjusted_normal)
  };
}
}  // namespace rt

#endif  // RAYTRACER_SURFACE_INTERACTION_H

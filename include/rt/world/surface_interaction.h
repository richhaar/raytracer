// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_SURFACE_INTERACTION_H
#define RAYTRACER_SURFACE_INTERACTION_H
#include <vector>

#include "intersection.h"
#include "ray.h"
#include "rt/math/point3.h"
#include "sphere.h"

namespace rt {
// TODO: investigate needed offset for speckling, previously 2e-3f
inline constexpr float kRaySurfaceOffset = 2e-3f;  // 2e-3f

struct SurfaceInteraction {
  Intersection intersection;
  bool inside = false;
  Point3 point;
  Point3 over_point;
  Point3 under_point;
  Vector3 eye;
  Vector3 normal;
  Vector3 reflect;
  float n1;
  float n2;
};

inline std::pair<float, float> CalculateN1N2(
    Intersection const& record,
    std::vector<Intersection> const& intersections) {
  std::vector<Intersectable const*> container;

  float n1 = 1.0;
  float n2 = 1.0;

  for (auto const& intersection : intersections) {
    if (intersection == record) {
      n1 = container.empty() ? 1.0f
                             : container.back()->GetMaterial().refractive_index;
    }

    auto const it = std::ranges::find(container, intersection.object);
    if (it != container.end()) {
      container.erase(it);
    } else {
      container.push_back(intersection.object);
    }

    if (intersection == record) {
      n2 = container.empty() ? 1.0f
                             : container.back()->GetMaterial().refractive_index;
      break;
    }
  }

  return {n1, n2};
}

// TODO: refactor to index based
inline SurfaceInteraction ComputeSurfaceInteraction(
    Intersection const& record, Ray const& ray,
    std::vector<Intersection> intersections = {}) {
  if (intersections.empty()) {
    intersections.push_back(record);
  }

  auto const pos = Position(ray, record.t);
  auto const normal = record.object->NormalAt(pos);
  auto const eye = -ray.direction;
  auto const inside = Dot(normal, eye) < 0.0f;
  auto const adjusted_normal = inside ? -normal : normal;

  auto const [n1, n2] = CalculateN1N2(record, intersections);
  return {.intersection = record,
          .inside = inside,
          .point = pos,
          .over_point = pos + adjusted_normal * kRaySurfaceOffset,
          .under_point = pos - adjusted_normal * kRaySurfaceOffset,
          .eye = eye,
          .normal = adjusted_normal,
          .reflect = Reflect(ray.direction, adjusted_normal),
          .n1 = n1,
          .n2 = n2};
}
}  // namespace rt

#endif  // RAYTRACER_SURFACE_INTERACTION_H

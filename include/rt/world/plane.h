// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_PLANE_H
#define RAYTRACER_PLANE_H
#include "intersectable.h"

namespace rt {
class Plane : public Intersectable {
  // TODO:: refactor hit return types
  [[nodiscard]] std::optional<std::pair<HitRecord, HitRecord>> LocalHit(
      Ray const& ray) const override {
    if (std::abs(ray.direction.y) < 1e-6f) {
      return std::nullopt;
    }
    auto const t = -ray.origin.y / ray.direction.y;
    return std::make_pair(HitRecord{t, this}, HitRecord{t, this});
  }

  [[nodiscard]] Vector3 LocalNormalAt(Point3 const&) const override {
    return Vector3{0.0f, 1.0f, 0.0f};
  }
};
}  // namespace rt

#endif  // RAYTRACER_PLANE_H

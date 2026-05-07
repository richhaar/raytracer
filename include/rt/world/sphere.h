// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H
#include <cstdint>

#include "rt/graphics/material.h"
#include "rt/util/noncopyable.h"
#include "rt/world/intersectable.h"
#include "rt/world/ray.h"
#include "rt/world/uuid.h"

namespace rt {
class Sphere : public NonCopyable, public Intersectable {
  uint64_t uuid_;

  [[nodiscard]] std::optional<std::pair<HitRecord, HitRecord>> LocalHit(
      Ray const& ray) const override {
    auto const sphere_to_ray =
        ray.origin - Point3(0.0f, 0.0f, 0.0f);  // assume sphere is 0,0,0
    auto const a = Dot(ray.direction, ray.direction);
    auto const b = 2.0f * Dot(ray.direction, sphere_to_ray);
    auto const c = Dot(sphere_to_ray, sphere_to_ray) - 1.0f;

    auto const discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
      return {};
    }

    auto const t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
    auto const t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

    return std::make_pair(HitRecord{t1, this}, HitRecord{t2, this});
  }

  [[nodiscard]] Vector3 LocalNormalAt(Point3 const& object_point) const override {
    return object_point - Point3{0.0f, 0.0f, 0.0f};
  }

 public:
  Sphere() : uuid_(Uuid()) {}

  bool operator==(Sphere const& rhs) const { return uuid_ == rhs.uuid_; }
};
}  // namespace rt

#endif  // RAYTRACER_SPHERE_H

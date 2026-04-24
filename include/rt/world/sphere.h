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
  Matrix<4, 4> transform_{Matrix<4, 4>::Identity()};
  Material material_;

  [[nodiscard]] std::optional<std::pair<HitRecord, HitRecord>> DoHit(
      Ray const& ray) const override {
    const auto [origin, direction] = Transform(ray, Inverse(transform_));
    auto const sphere_to_ray =
        origin - Point3(0.0f, 0.0f, 0.0f);  // assume sphere is 0,0,0
    auto const a = Dot(direction, direction);
    auto const b = 2.0f * Dot(direction, sphere_to_ray);
    auto const c = Dot(sphere_to_ray, sphere_to_ray) - 1.0f;

    auto const discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
      return {};
    }

    auto const t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
    auto const t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

    return std::make_pair(HitRecord{t1, this}, HitRecord{t2, this});
  }

 public:
  Sphere() : uuid_(Uuid()) {}

  void SetTransform(Matrix<4, 4> const& matrix) { transform_ = matrix; }
  void SetMaterial(Material const& material) { material_ = material; }

  Matrix<4, 4> GetTransform() const { return transform_; }
  bool operator==(Sphere const& rhs) const { return uuid_ == rhs.uuid_; }
};

inline Vector3 NormalAt(Sphere const& s, Point3 const& point) {
  auto const inv_transform = Inverse(s.GetTransform());
  auto const object_point = inv_transform * point;
  auto const object_normal = object_point - Point3{0.0f, 0.0f, 0.0f};
  auto const world_normal = Transpose(inv_transform) * object_normal;
  return Normalize(world_normal);
}
}  // namespace rt

#endif  // RAYTRACER_SPHERE_H

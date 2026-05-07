// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_INTERSECTABLE_H
#define RAYTRACER_INTERSECTABLE_H
#include <optional>
#include <utility>

#include "hit_record.h"
#include "rt/graphics/material.h"
#include "rt/math/matrix_ops.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"
#include "rt/world/ray.h"

namespace rt {
struct Ray;

class Intersectable {
  Matrix<4, 4> transform_{Matrix<4, 4>::Identity()};
  Matrix<4, 4> inverse_transform_{Matrix<4, 4>::Identity()};
  Material material_{};

  [[nodiscard]] virtual std::optional<std::pair<HitRecord, HitRecord>> LocalHit(
      Ray const& ray) const = 0;

  [[nodiscard]] virtual Vector3 LocalNormalAt(Point3 const& point) const = 0;

 public:
  virtual ~Intersectable() = default;

  [[nodiscard]] std::optional<std::pair<HitRecord, HitRecord>> Hit(
      Ray const& ray) const {
    auto const local_ray = Transform(ray, inverse_transform_);
    return LocalHit(local_ray);
  }

  [[nodiscard]] Vector3 NormalAt(Point3 const& world_point) const {
    auto const local_point = inverse_transform_ * world_point;
    auto const local_normal = LocalNormalAt(local_point);
    auto const world_normal = Transpose(inverse_transform_) * local_normal;
    return Normalize(world_normal);
  }

  void SetTransform(Matrix<4, 4> const& transform) {
    transform_ = transform;
    inverse_transform_ = Inverse(transform);
  }

  [[nodiscard]] Matrix<4, 4> const& GetTransform() const { return transform_; }
  [[nodiscard]] Matrix<4, 4> const& GetInverseTransform() const {
    return inverse_transform_;
  }
  [[nodiscard]] Material const& GetMaterial() const { return material_; }
  void SetMaterial(Material material) { material_ = std::move(material); }
};

}  // namespace rt

#endif  // RAYTRACER_INTERSECTABLE_H

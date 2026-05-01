// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_INTERSECTABLE_H
#define RAYTRACER_INTERSECTABLE_H
#include <optional>
#include <utility>

#include "hit_record.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"

namespace rt {
struct Ray;

class Intersectable {
  [[nodiscard]] virtual std::optional<std::pair<HitRecord, HitRecord>> DoHit(
      Ray const& ray) const = 0;

  [[nodiscard]] virtual Vector3 DoNormalAt(Point3 const& point) const = 0;

 public:
  virtual ~Intersectable() = default;
  [[nodiscard]] std::optional<std::pair<HitRecord, HitRecord>> Hit(
      Ray const& ray) const {
    return DoHit(ray);
  }

  [[nodiscard]] Vector3 NormalAt(Point3 const& point) const {
    return DoNormalAt(point);
  }
};

}  // namespace rt

#endif  // RAYTRACER_INTERSECTABLE_H

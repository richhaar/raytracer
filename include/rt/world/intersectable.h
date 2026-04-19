// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_INTERSECTABLE_H
#define RAYTRACER_INTERSECTABLE_H
#include <optional>
#include <utility>

#include "hit_record.h"

namespace rt {
struct Ray;

class Intersectable {
  virtual std::optional<std::pair<HitRecord, HitRecord>> DoHit(
      Ray const& ray) const = 0;

 public:
  virtual ~Intersectable() = default;
  std::optional<std::pair<HitRecord, HitRecord>> Hit(Ray const& ray) const {
    return DoHit(ray);
  }
};

}  // namespace rt

#endif  // RAYTRACER_INTERSECTABLE_H

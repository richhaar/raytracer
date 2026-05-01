// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_WORLD_H
#define RAYTRACER_WORLD_H
#include <memory>
#include <set>
#include <vector>

#include "intersectable.h"
#include "point_light.h"

namespace rt {

struct World {
  std::vector<PointLight> lights_;
  std::vector<std::unique_ptr<Intersectable>> objects_;
};

inline auto IntersectWorld(World const& world, Ray const& ray) {
  auto const sort_by_t = [](HitRecord const& a, HitRecord const& b) {
    return a.t < b.t;
  };
  std::set<HitRecord, decltype(sort_by_t)> hits(sort_by_t);
  for (auto const& object : world.objects_) {
    if (auto const intersection = object->Hit(ray); intersection) {
      hits.insert(intersection->first);
      hits.insert(intersection->second);
    }
  }

  return hits;
}
}  // namespace rt

#endif  // RAYTRACER_WORLD_H

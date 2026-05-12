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
  std::vector<Intersection> hits;
  hits.reserve(world.objects_.size() * 2);

  for (auto const& object : world.objects_) {
    if (auto const xs = object->Hit(ray); xs) {
      hits.push_back(xs->first);
      hits.push_back(xs->second);
    }
  }

  std::ranges::sort(hits, {}, &Intersection::t);
  return hits;
}
}  // namespace rt

#endif  // RAYTRACER_WORLD_H

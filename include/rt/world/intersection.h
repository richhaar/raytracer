// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

#include <cmath>

namespace rt {
class Intersectable;

struct Intersection {
  float t = 0.0f;
  Intersectable const* object = nullptr;
};

inline bool operator==(Intersection const& a, Intersection const& b) {
  return std::abs(a.t - b.t) < 1e-6f && a.object == b.object;
}

}  // namespace rt

#endif  // RAYTRACER_INTERSECTION_H

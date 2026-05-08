// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_INTERSECTION_H
#define RAYTRACER_INTERSECTION_H

namespace rt {
class Intersectable;

struct Intersection {
  float t = 0.0f;
  Intersectable const* object = nullptr;
};

}  // namespace rt

#endif  // RAYTRACER_INTERSECTION_H

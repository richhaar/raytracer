// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_HIT_RECORD_H
#define RAYTRACER_HIT_RECORD_H

namespace rt {
class Intersectable;

struct HitRecord {
  float t;
  Intersectable const* object;
};

}  // namespace rt

#endif  // RAYTRACER_HIT_RECORD_H

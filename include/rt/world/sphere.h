// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H
#include <cstdint>

#include "rt/util/noncopyable.h"
#include "rt/world/uuid.h"

namespace rt {
class Sphere : public NonCopyable {
  uint64_t uuid_;

 public:
  Sphere() : uuid_(Uuid()) {}

  bool operator==(Sphere const& rhs) const { return uuid_ == rhs.uuid_; };
};
}  // namespace rt

#endif  // RAYTRACER_SPHERE_H

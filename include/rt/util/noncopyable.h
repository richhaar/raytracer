// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_NONCOPYABLE_H
#define RAYTRACER_NONCOPYABLE_H

namespace rt {

class NonCopyable {
 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(NonCopyable const&) = delete;
  NonCopyable& operator=(NonCopyable const&) = delete;
};
}  // namespace rt
#endif  // RAYTRACER_NONCOPYABLE_H

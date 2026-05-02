// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_RENDER_H
#define RAYTRACER_RENDER_H
#include "canvas.h"
#include "lighting.h"
#include "rt/world/camera.h"

namespace rt {

inline Canvas RenderWorld(Camera const& cam, World const& world) {
  auto img = Canvas(cam.hsize, cam.vsize);

  for (uint64_t y = 0ULL; y < cam.vsize - 1ULL; ++y) {
    for (uint64_t x = 0ULL; x < cam.hsize - 1ULL; ++x) {
      auto const ray = RayForPixel(cam, x, y);
      auto const colour = ColourAt(world, ray);
      img.Write(x, y, colour);
    }
  }
  return img;
}
}  // namespace rt

#endif  // RAYTRACER_RENDER_H

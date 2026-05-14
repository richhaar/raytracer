// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_RENDER_H
#define RAYTRACER_RENDER_H
#include <random>

#include "canvas.h"
#include "lighting.h"
#include "rt/world/camera.h"

namespace rt {


inline Canvas RenderWorld(Camera const& cam, World const& world) {
  constexpr int spp_side = 5;
  constexpr int samples_per_pixel = spp_side * spp_side;

  std::mt19937 rng(12345);
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  auto img = Canvas(cam.hsize, cam.vsize);

  for (uint64_t y = 0; y < cam.vsize; ++y) {
    for (uint64_t x = 0; x < cam.hsize; ++x) {
      ColourRGB sum = ColourRGB::Black();

      for (int sy = 0; sy < spp_side; ++sy) {
        for (int sx = 0; sx < spp_side; ++sx) {
          float u = (static_cast<float>(sx) + dist(rng)) / spp_side;
          float v = (static_cast<float>(sy) + dist(rng)) / spp_side;

          auto const ray = RayForPixel(cam, static_cast<float>(x) + u,
                                            static_cast<float>(y) + v);
          sum = sum + ColourAt(world, ray);
        }
      }

      img.Write(x, y, sum * (1.0f / samples_per_pixel));
    }
  }

  return img;
}
/*
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
}*/
}  // namespace rt

#endif  // RAYTRACER_RENDER_H

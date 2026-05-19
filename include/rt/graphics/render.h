// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_RENDER_H
#define RAYTRACER_RENDER_H
#include <random>

#include "canvas.h"
#include "lighting.h"
#include "rt/world/camera.h"

namespace rt {

enum class AntiAliasing {
  Off,
  X4,
  X9,
  X16,
};

[[nodiscard]] constexpr int32_t GridSide(AntiAliasing const& aa) noexcept {
  switch (aa) {
    default:
    case AntiAliasing::Off:
      return 1;
    case AntiAliasing::X4:
      return 2;
    case AntiAliasing::X9:
      return 3;
    case AntiAliasing::X16:
      return 4;
  }
}

[[nodiscard]] constexpr int32_t SamplesPerPixel(AntiAliasing aa) noexcept {
  const int32_t side = GridSide(aa);
  return side * side;
}

class SampleStrategy {
  virtual ColourRGB SampleImplementation(Camera const& cam, World const& world,
                                         uint64_t const x, uint64_t y) = 0;

 public:
  virtual ~SampleStrategy() = default;
  ColourRGB Sample(Camera const& cam, World const& world, uint64_t const x,
                   uint64_t y) {
    return SampleImplementation(cam, world, x, y);
  }
};

class NoAntiAliasing : public SampleStrategy {
  ColourRGB SampleImplementation(Camera const& cam, World const& world,
                                         uint64_t const x, uint64_t y) override {
    auto const ray = RayForPixel(cam, static_cast<float>(x) + 0.5f,
                                 static_cast<float>(y) + 0.5f);

    return ColourAt(world, ray);
  }
};

template <typename Rng>
ColourRGB SamplePixel(Camera const& cam, World const& world, uint64_t const x,
                      uint64_t const y, AntiAliasing const& aa, Rng& rng) {
  const int32_t side = GridSide(aa);
  const float inv_side = 1.0f / static_cast<float>(side);

  std::uniform_real_distribution dist(0.0f, 1.0f);

  auto sum = ColourRGB::Black();

  for (int32_t sy = 0; sy < side; ++sy) {
    for (int32_t sx = 0; sx < side; ++sx) {
      auto const u = (static_cast<float>(sx) + dist(rng)) * inv_side;
      auto const v = (static_cast<float>(sy) + dist(rng)) * inv_side;

      auto const ray = RayForPixel(cam, static_cast<float>(x) + u,
                                   static_cast<float>(y) + v);

      sum = sum + ColourAt(world, ray);
    }
  }

  return sum * (1.0f / static_cast<float>(SamplesPerPixel(aa)));
}

class X4AntiAliasing : public SampleStrategy {
  std::mt19937 rng_{12345};
  ColourRGB SampleImplementation(Camera const& cam, World const& world,
                                 uint64_t const x, uint64_t y) override {
    return SamplePixel(cam, world, x, y, AntiAliasing::X4, rng_);
  }
};

class X9AntiAliasing : public SampleStrategy {
  std::mt19937 rng_{12345};
  ColourRGB SampleImplementation(Camera const& cam, World const& world,
                                 uint64_t const x, uint64_t y) override {
    return SamplePixel(cam, world, x, y, AntiAliasing::X9, rng_);
  }
};

class X16AntiAliasing : public SampleStrategy {
  std::mt19937 rng_{12345};
  ColourRGB SampleImplementation(Camera const& cam, World const& world,
                                 uint64_t const x, uint64_t y) override {
    return SamplePixel(cam, world, x, y, AntiAliasing::X16, rng_);
  }
};

inline Canvas RenderWorld(Camera const& cam, World const& world,
                          std::unique_ptr<SampleStrategy> const& ss =
                              std::make_unique<NoAntiAliasing>()) {
  Canvas image(cam.hsize, cam.vsize);

  for (uint64_t y = 0; y < cam.vsize; ++y) {
    for (uint64_t x = 0; x < cam.hsize; ++x) {
      image.Write(x, y, ss->Sample(cam, world, x, y));
    }
  }

  return image;
}
}  // namespace rt

#endif  // RAYTRACER_RENDER_H

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <iostream>

#include "rt/graphics/canvas.h"
#include "rt/graphics/ppm_writer.h"
#include "rt/graphics/render.h"
#include "rt/math/view_transform.h"
#include "rt/scenes/world1.h"
#include "rt/world/camera.h"
#include "rt/world/ray.h"

int main() {
  std::size_t constexpr kWidth = 2000;
  std::size_t constexpr kHeight = 1000;
  auto const file_name = "world1.ppm";

  std::unique_ptr<rt::SampleStrategy> const aa_strategy =
      std::make_unique<rt::X9AntiAliasing>();

  auto const world = rt::CreateWorld1();

  auto const cam =
      rt::Camera::Create(kWidth, kHeight, std::numbers::pi_v<float> / 2.0f,
                         rt::ViewTransform(rt::Point3{0.0f, 0.5f, -3.0f},
                                           rt::Point3{0.0f, 0.5f, 0.0f},
                                           rt::Vector3{0.0f, 1.0f, 0.0f}));

  std::cout << "Starting render..." << std::endl;
  auto const t0 = std::chrono::steady_clock::now();
  auto const img = rt::RenderWorld(cam, world, aa_strategy);
  auto const t1 = std::chrono::steady_clock::now();
  std::cout
      << "time "
      << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
      << "ms." << std::endl;

  rt::PPMWriter::Write(img, file_name);
  return EXIT_SUCCESS;
}
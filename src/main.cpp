// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <iostream>

#include "rt/graphics/canvas.h"
#include "rt/graphics/colour_rgb.h"
#include "rt/graphics/ppm_writer.h"
#include "rt/graphics/render.h"
#include "rt/math/matrix_ops.h"
#include "rt/math/point3.h"
#include "rt/math/transforms.h"
#include "rt/math/view_transform.h"
#include "rt/world/camera.h"
#include "rt/world/point_light.h"
#include "rt/world/ray.h"
#include "rt/world/sphere.h"
#include "rt/world/world_builder.h"

int main() {
  auto builder = rt::WorldBuilder();
  auto constexpr pi = std::numbers::pi_v<float>;

  auto constexpr floor_material =
      rt::Material{.colour = rt::ColourRGB{1.0f, 0.9f, 0.9f}, .specular = 0.0f};
  auto const world =
      builder
          // Floor
          .AddSphere(rt::Scaling(10.0f, 0.01f, 10.0f), floor_material)
          // Left wall
          .AddSphere(rt::Translation(0.0f, 0.0f, 5.0f) *
                         rt::RotateY(-pi / 4.0f) * rt::RotateX(pi / 2.0f) *
                         rt::Scaling(10.0f, 0.01f, 10.0f),
                     floor_material)
          // Right wall
          .AddSphere(rt::Translation(0.0f, 0.0f, 5.0f) *
                         rt::RotateY(pi / 4.0f) * rt::RotateX(pi / 2.0f) *
                         rt::Scaling(10.0f, 0.01f, 10.0f),
                     floor_material)
          // Large green sphere
          .AddSphere(rt::Translation(-0.5f, 1.0f, 0.5f),
                     rt::Material{.colour = rt::ColourRGB{0.1f, 1.0f, 0.5f},
                                  .diffuse = 0.7f,
                                  .specular = 0.3f})
          // Smaller green sphere
          .AddSphere(rt::Translation(1.5f, 0.5f, -0.5f) *
                         rt::Scaling(0.5f, 0.5f, 0.5f),
                     rt::Material{.colour = rt::ColourRGB{0.5f, 1.0f, 0.5f},
                                  .diffuse = 0.7f,
                                  .specular = 0.3f})
          // Smallest sphere
          .AddSphere(rt::Translation(-1.5f, 0.33f, -0.75f) *
                         rt::Scaling(0.33f, 0.33f, 0.33f),
                     rt::Material{.colour = rt::ColourRGB{1.0f, 0.8f, 0.1f},
                                  .diffuse = 0.7f,
                                  .specular = 0.9f})
  .AddSphere(rt::Translation(-2.5f, 0.33f, -0.75f) *
               rt::Scaling(0.33f, 0.33f, 0.33f),
           rt::Material{.colour = rt::ColourRGB{1.0f, 0.8f, 0.1f},
                        .diffuse = 0.7f,
                        .specular = 0.9f})
          // Light source
          .AddLight(rt::PointLight{rt::ColourRGB{0.1f, 0.7f, 0.4f},
                                   rt::Point3{-10.0f, 10.0f, -10.0f}})
  .AddLight(rt::PointLight{rt::ColourRGB{0.4f, 0.1f, 0.4f},
                         rt::Point3{10.0f, 10.0f, -10.0f}})
          .Build();

  auto const cam =
      rt::Camera::Create(2000, 1000, pi / 3.0f,
                         rt::ViewTransform(rt::Point3{0.0f, 1.5f, -5.0f},
                                           rt::Point3{0.0f, 1.0f, 0.0f},
                                           rt::Vector3{0.0f, 1.0f, 0.0f}));

  std::cout << "Starting render..." << std::endl;
  auto t0 = std::chrono::steady_clock::now();
  auto const img = rt::RenderWorld(cam, world);
  auto t1 = std::chrono::steady_clock::now();
  auto const ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
  std::cout << "time " << ms.count() << "ms." << std::endl;
  rt::PPMWriter::Write(img, "default_world_shaded.ppm");
  return EXIT_SUCCESS;
}
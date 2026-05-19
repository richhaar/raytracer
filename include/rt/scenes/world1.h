// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_WORLD1_H
#define RAYTRACER_WORLD1_H

#include "rt/world/world_builder.h"

namespace rt {

inline World CreateWorld1() {
  auto builder = WorldBuilder();

  auto checker_pattern =
      std::make_shared<CheckerPattern>(ColourRGB::Black(), ColourRGB::White());
  checker_pattern->SetTransform(Scaling(0.3f, 0.3f, 0.3f));

  auto const green_sphere =
      Material{.pattern = std::make_shared<rt::SolidColour>(0.2f, 0.8f, 0.1f)};
  auto const blue_sphere =
      Material{.pattern = std::make_shared<SolidColour>(0.2f, 0.3f, 0.8f)};
  auto const red_sphere =
      Material{.pattern = std::make_shared<SolidColour>(1.0f, 0.3f, 0.1f)};
  auto const yellow_sphere =
      Material{.pattern = std::make_shared<SolidColour>(0.8f, 0.8f, 0.1f)};
  auto const purple_sphere =
      Material{.pattern = std::make_shared<SolidColour>(0.8f, 0.1f, 1.0f)};
  auto const cyan_sphere =
      Material{.pattern = std::make_shared<SolidColour>(0.0f, 0.8f, 0.9f)};
  auto const reflective_sphere = Material{.ambient = 0.0f,
                                          .diffuse = 0.05f,
                                          .shininess = 300.0f,
                                          .reflective = 0.9f};
  auto const refractive_sphere = Material{.ambient = 0.0f,
                                          .diffuse = 0.05f,
                                          .shininess = 300.0f,
                                          .reflective = 0.9f,
                                          .transparency = 0.9f,
                                          .refractive_index = 1.5f};
  auto const checkered_floor = Material{.pattern = checker_pattern};
  return builder.AddSphere(Scaling(10.0f, 10.0f, 10.0f), {})
      .AddLight(PointLight{ColourRGB::Red(), Point3(-4.0f, 3.0f, -4.0f)})
      .AddLight(PointLight{ColourRGB::Green(), Point3(4.0f, 3.0f, -4.0f)})
      .AddLight(PointLight{ColourRGB::Blue(), Point3(0.0f, 3.0f, -4.0f)})
      .AddSphere(Scaling(0.2f, 0.2f, 0.2f) * Translation(0.0f, 1.0, 0.0f),
                 green_sphere)
      .AddSphere(Scaling(0.2f, 0.2f, 0.2f) * Translation(-4.0f, 1.0, 4.0f),
                 red_sphere)
      .AddSphere(Scaling(0.2f, 0.2f, 0.2f) * Translation(-2.0f, 1.0, 5.5f),
                 blue_sphere)
      .AddSphere(Scaling(0.8f, 0.8f, 0.8f) * Translation(2.0f, 1.0, 1.0f),
                 purple_sphere)
      .AddSphere(Scaling(0.4f, 0.4f, 0.4f) * Translation(-3.0f, 1.0, -0.5f),
                 reflective_sphere)
      .AddSphere(Scaling(0.4f, 0.4f, 0.4f) * Translation(1.0f, 1.0, -2.5f),
                 refractive_sphere)
      .AddSphere(Scaling(0.6f, 0.6f, 0.6f) * Translation(0.0f, 1.0, -6.0f),
                 purple_sphere)
      .AddSphere(Scaling(0.2f, 0.2f, 0.2f) * Translation(-5.5f, 1.0, -7.0f),
                 cyan_sphere)
      .AddPlane(Matrix<4, 4>::Identity(), checkered_floor)
      .Build();
}

}  // namespace rt

#endif  // RAYTRACER_WORLD1_H

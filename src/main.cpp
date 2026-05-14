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
  std::size_t constexpr kWidth = 2000;
  std::size_t constexpr kHeight = 1000;

  auto builder = rt::WorldBuilder();
  auto constexpr pi = std::numbers::pi_v<float>;


  auto checker_pattern = std::make_shared<rt::CheckerPattern>(
      rt::ColourRGB::Black(), rt::ColourRGB::White());
  checker_pattern->SetTransform(rt::Scaling(0.3f, 0.3f, 0.3f));

  auto const green_sphere = rt::Material{
      .pattern = std::make_shared<rt::SolidColour>(0.2f, 0.8f, 0.1f)};
  auto const blue_sphere = rt::Material{
    .pattern = std::make_shared<rt::SolidColour>(0.2f, 0.3f, 0.8f)};
  auto const red_sphere = rt::Material{
    .pattern = std::make_shared<rt::SolidColour>(1.0f, 0.3f, 0.1f)};
  auto const yellow_sphere = rt::Material{
    .pattern = std::make_shared<rt::SolidColour>(0.8f, 0.8f, 0.1f)};
  auto const purple_sphere = rt::Material{
    .pattern = std::make_shared<rt::SolidColour>(0.8f, 0.1f, 1.0f)};
  auto const cyan_sphere = rt::Material{
    .pattern = std::make_shared<rt::SolidColour>(0.0f, 0.8f, 0.9f)};
  auto const reflective_sphere = rt::Material{
    .ambient = 0.0f, .diffuse = 0.05f, .shininess = 300.0f, .reflective = 0.9f};
  auto const refractive_sphere = rt::Material{
    .ambient = 0.0f, .diffuse = 0.05f, .shininess = 300.0f, .reflective = 0.9f, .transparency = 0.9f, .refractive_index = 1.5f};
  auto const checkered_floor = rt::Material{
    .pattern = checker_pattern};
  auto const world =
      builder.AddSphere(rt::Scaling(10.0f, 10.0f, 10.0f), {})
  .AddLight(rt::PointLight{rt::ColourRGB::Red(), rt::Point3(-4.0f, 3.0f, -4.0f)})
  .AddLight(rt::PointLight{rt::ColourRGB::Green(), rt::Point3(4.0f, 3.0f, -4.0f)})
  .AddLight(rt::PointLight{rt::ColourRGB::Blue(), rt::Point3(0.0f, 3.0f, -4.0f)})
          .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(0.0f, 1.0, 0.0f), green_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-4.0f, 1.0, 4.0f), red_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-2.0f, 1.0, 5.5f), blue_sphere)
  .AddSphere(rt::Scaling(0.8f, 0.8f, 0.8f) * rt::Translation(2.0f, 1.0, 1.0f), purple_sphere)
  .AddSphere(rt::Scaling(0.4f, 0.4f, 0.4f) * rt::Translation(-3.0f, 1.0, -0.5f), reflective_sphere)
  .AddSphere(rt::Scaling(0.4f, 0.4f, 0.4f) * rt::Translation(1.0f, 1.0, -2.5f), refractive_sphere)
  .AddSphere(rt::Scaling(0.6f, 0.6f, 0.6f) * rt::Translation(0.0f, 1.0, -6.0f), purple_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-5.5f, 1.0, -7.0f), cyan_sphere)
  .AddPlane(rt::Matrix<4,4>::Identity(), checkered_floor)
  .Build();

  auto const cam =
    rt::Camera::Create(kWidth, kHeight, pi / 2.0f,
                       rt::ViewTransform(rt::Point3{0.0f, 0.5f, -3.0f},
                                         rt::Point3{0.0f, 0.5f, 0.0f},
                                         rt::Vector3{0.0f, 1.0f, 0.0f}));

  std::cout << "Starting render..." << std::endl;
  auto t0 = std::chrono::steady_clock::now();
  auto const img = rt::RenderWorld(cam, world);
  auto t1 = std::chrono::steady_clock::now();
  auto const ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
  std::cout << "time " << ms.count() << "ms." << std::endl;
  rt::PPMWriter::Write(img, "world1.ppm");
  return EXIT_SUCCESS;
}
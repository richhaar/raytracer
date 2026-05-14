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
  std::size_t constexpr kWidth = 4000;
  std::size_t constexpr kHeight = 2000;

  auto builder = rt::WorldBuilder();
  auto constexpr pi = std::numbers::pi_v<float>;

  auto const ring_pattern = std::make_shared<rt::CheckerPattern>(
      rt::ColourRGB{1.0f, 0.9f, 0.9f}, rt::ColourRGB::White());
  ring_pattern->SetTransform(rt::Translation(0.0f, 0.0f, -5.0f) *
                             rt::Scaling(0.02f, 0.02f, 0.2f));

  auto const wall_pattern = std::make_shared<rt::RingPattern>(
      rt::ColourRGB{1.0f, 0.9f, 0.9f}, rt::ColourRGB::White());
  wall_pattern->SetTransform(rt::Translation(0.0f, 0.0f, -5.0f) *
                             rt::Scaling(0.8f, 0.8f, 0.8f));
  auto const wall_material =
      rt::Material{.pattern = wall_pattern,
        .ambient = 0.03f,
                   .specular = 0.6f,
                   .reflective = 0.0f};
  wall_material.pattern->SetTransform(rt::Scaling(0.06f, 0.06f, 0.6f)); //0.2

  auto checker_pattern = std::make_shared<rt::CheckerPattern>(
      rt::ColourRGB::Black(), rt::ColourRGB::White());
  checker_pattern->SetTransform(rt::Scaling(0.3f, 0.3f, 0.3f));

  auto const floor_material =
      rt::Material{.pattern = ring_pattern, .specular = 0.0f};

  auto const reflective_dome = rt::Material{.pattern = std::make_shared<rt::SolidColour>(0.53f, 0.81f, 0.92f), .ambient = 0.1f, .diffuse = 0.6f, .shininess = 300.0f, .reflective = 0.0f};
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
          //.AddLight(rt::PointLight{rt::ColourRGB::White(),rt::Point3(2.0f, 3.0f, -4.0f)})
  .AddLight(rt::PointLight{rt::ColourRGB::Red(), rt::Point3(-4.0f, 3.0f, -4.0f)})
  .AddLight(rt::PointLight{rt::ColourRGB::Green(), rt::Point3(4.0f, 3.0f, -4.0f)})
  .AddLight(rt::PointLight{rt::ColourRGB::Blue(), rt::Point3(0.0f, 3.0f, -4.0f)})
          .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(0.0f, 1.0, 0.0f), green_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-4.0f, 1.0, 4.0f), red_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-2.0f, 1.0, 5.5f), blue_sphere)
  //.AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(3.0f, 1.0, 1.0f), yellow_sphere)
  //.AddSphere(rt::Scaling(1.0f, 1.0f, 1.0f) * rt::Translation(2.0f, 1.0, 1.0f), yellow_sphere) //COOL
  .AddSphere(rt::Scaling(0.8f, 0.8f, 0.8f) * rt::Translation(2.0f, 1.0, 1.0f), purple_sphere)
  .AddSphere(rt::Scaling(0.4f, 0.4f, 0.4f) * rt::Translation(-3.0f, 1.0, -0.5f), reflective_sphere)
  //.AddSphere(rt::Scaling(0.6f, 0.6f, 0.6f) * rt::Translation(1.0f, 1.0, -2.5f), refractive_sphere)
  .AddSphere(rt::Scaling(0.4f, 0.4f, 0.4f) * rt::Translation(1.0f, 1.0, -2.5f), refractive_sphere)
  .AddSphere(rt::Scaling(0.6f, 0.6f, 0.6f) * rt::Translation(0.0f, 1.0, -6.0f), purple_sphere)
  .AddSphere(rt::Scaling(0.2f, 0.2f, 0.2f) * rt::Translation(-5.5f, 1.0, -7.0f), cyan_sphere)
  .AddPlane(rt::Matrix<4,4>::Identity(), checkered_floor)
  /*.AddPlane(rt::Translation(-3.0f, 0.0f, 0.0f) *
                         rt::RotateY(-pi / 2.3f) * rt::RotateX(pi / 2.0f), {})*/

  .Build();

  auto const world2 =
      builder
          .AddSphere(
              rt::Translation(0.0f, 1.8f, 1.5f) * rt::Scaling(1.5f, 1.5f, 1.5f),
              rt::Material{.ambient = 0.00f,
                           .diffuse = 0.02f,
                           .specular = 0.90f,
                           .shininess = 300.0f,
                           .reflective = 0.8f,
                           .transparency = 0.98f,
                           .refractive_index = 1.5f})
          .AddPlane(rt::Translation(0.0f, 0.0f, 10.0f) *
                        rt::RotateX(std::numbers::pi_v<float> / 2.0f),
                    {.pattern = checker_pattern})
          .AddPlane(rt::Matrix<4, 4>::Identity(), floor_material)
          .AddLight(rt::PointLight{rt::ColourRGB{0.9f, 0.9f, 0.9f},
                                   rt::Point3{10.0f, 4.0f, -10.0f}})
          .AddSphere(rt::Translation(-0.9f, 0.5f, -0.5f) *
                         rt::Scaling(0.5f, 0.5f, 0.5f),
                     rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                                      rt::ColourRGB{0.5f, 1.0f, 0.5f}),
                                  .ambient = 0.0f,
                                  .diffuse = 0.0f,
                                  .specular = 0.3f,
                                  .reflective = 1.0f})
          .AddSphere(
              rt::Translation(2.3f, 0.5f, 5.5f) * rt::Scaling(0.5f, 0.5f, 0.5f),
              rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                               rt::ColourRGB{0.5f, 1.0f, 0.5f}),
                           .diffuse = 0.7f,
                           .specular = 0.3f})
          /* .AddPlane(rt::Translation(-3.0f, 0.0f, 0.0f) *
                         rt::RotateY(-pi / 2.0f) * rt::RotateX(pi / 2.0f),
                     wall_material)*/
          .Build();

  builder
      // Floor
      .AddPlane(rt::Matrix<4, 4>::Identity(), floor_material)
      .AddPlane(rt::Translation(0.0f, 0.0f, -20.0f) *
                    rt::RotateX(std::numbers::pi_v<float> / 2.0f),
                wall_material)
      /* .AddPlane(rt::Translation(0.0f, 0.0f, 10.0f) *
                     rt::RotateX(std::numbers::pi_v<float> / 2.0f),
                 wall_material)*/
      /*.AddSphere(rt::Scaling(10.0f, 0.01f, 10.0f), floor_material)
       */
      // Left wall
      .AddSphere(rt::Translation(0.0f, 0.0f, 5.0f) * rt::RotateY(-pi / 4.0f) *
                     rt::RotateX(pi / 2.0f) * rt::Scaling(10.0f, 0.01f, 10.0f),
                 floor_material)
      // Right wall
      .AddSphere(rt::Translation(0.0f, 0.0f, 5.0f) * rt::RotateY(pi / 4.0f) *
                     rt::RotateX(pi / 2.0f) * rt::Scaling(10.0f, 0.01f, 10.0f),
                 floor_material)
      // Large green sphere
      .AddSphere(rt::Translation(-0.5f, 1.0f, 0.5f),
                 rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                                  rt::ColourRGB::White()),

                              // std::make_shared<rt::SolidColour>(
                              // rt::ColourRGB{0.1f, 1.0f, 0.5f}),
                              /* .diffuse = 0.7f,
                               .specular = 0.3f,*/
                              .transparency = 1.0f,
                              .refractive_index = 1.5f})
      // Smaller green sphere
      /* .AddSphere(rt::Translation(-0.5f, 1.5f, 4.5f) *
                      rt::Scaling(0.5f, 0.5f, 0.5f),
                  rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                                   rt::ColourRGB{0.5f, 1.0f, 0.5f}),
                               .diffuse = 0.7f,
                               .specular = 0.3f})*/
      // Smallest sphere
      .AddSphere(rt::Translation(-1.5f, 0.33f, -0.75f) *
                     rt::Scaling(0.33f, 0.33f, 0.33f),
                 rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                                  rt::ColourRGB{1.0f, 0.8f, 0.1f}),
                              .diffuse = 0.7f,
                              .specular = 0.9f})
      .AddSphere(rt::Translation(-2.5f, 0.33f, -0.75f) *
                     rt::Scaling(0.33f, 0.33f, 0.33f),
                 rt::Material{.pattern = std::make_shared<rt::SolidColour>(
                                  rt::ColourRGB{1.0f, 0.8f, 0.1f}),
                              .diffuse = 0.7f,
                              .specular = 0.9f})
      // Light source
      /* .AddLight(rt::PointLight{rt::ColourRGB{1.0f, 1.0f, 1.0f},
                                rt::Point3{-10.0f, 4.0f, -10.0f}})*/
      .AddLight(rt::PointLight{rt::ColourRGB{0.1f, 0.7f, 0.4f},
                               rt::Point3{-10.0f, 4.0f, -10.0f}})
      .AddLight(rt::PointLight{rt::ColourRGB{0.4f, 0.1f, 0.4f},
                               rt::Point3{10.0f, 4.0f, -10.0f}})
      .Build();

  auto const cam2 =
      rt::Camera::Create(kWidth, kHeight, pi / 3.0f,
                         rt::ViewTransform(rt::Point3{0.0f, 1.5f, -5.0f},
                                           rt::Point3{0.0f, 1.0f, 0.0f},
                                           rt::Vector3{0.0f, 1.0f, 0.0f}));
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
// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <iostream>

#include "rt/graphics/canvas.h"
#include "rt/graphics/colour_rgb.h"
#include "rt/graphics/lighting.h"
#include "rt/graphics/ppm_writer.h"
#include "rt/math/matrix_ops.h"
#include "rt/math/point3.h"
#include "rt/math/transforms.h"
#include "rt/world/point_light.h"
#include "rt/world/ray.h"
#include "rt/world/sphere.h"

int main() {
  auto constexpr width = 600;
  auto constexpr height = 600;
  auto canvas = rt::Canvas{width, height};

  auto sph = rt::Sphere{};
  auto material = rt::Material{};
  material.colour.green = 0.2f;
  sph.SetMaterial(material);
  // sph.SetTransform(rt::Shear(1, 0, 0, 0, 0, 0) *
  // rt::Scaling(0.5f, 1.0f, 1.0f));

  auto constexpr light_position = rt::Point3{-10.0f, 10.0f, -10.0f};
  auto constexpr light_colour = rt::ColourRGB::White();
  auto const light = rt::PointLight(light_colour, light_position);

  auto constexpr wall_size = 7.0f;
  auto constexpr pixel_size_x = wall_size / width;
  auto constexpr pixel_size_y = wall_size / height;

  auto constexpr red = rt::ColourRGB{1.0f, 0.0f, 0.0f};

  //-3.5 to +3.5
  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      auto const direction =
          Normalize(rt::Point3{-3.5f + pixel_size_x * x,
                               -3.5f + pixel_size_y * y, 10.0f} -
                    rt::Point3{0.0f, 0.0f, -5.0f});
      auto const ray = rt::Ray{rt::Point3{0.0f, 0.0f, -5.0f}, direction};

      if (auto const hit = sph.Hit(ray); hit.has_value()) {
        auto const point = Position(ray, hit->first.t);
        auto const eye = -ray.direction;

        auto const colour = rt::Lighting(sph.GetMaterial(), light, point, eye,
                                         sph.NormalAt(point));

        canvas.Write(x, width - y, colour);
      }
    }
  }
  rt::PPMWriter::Write(canvas, "sphere_with_lighting.ppm");

  return EXIT_SUCCESS;
}
// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_LIGHTING_H
#define RAYTRACER_LIGHTING_H
#include "colour_rgb.h"
#include "material.h"
#include "rt/world/point_light.h"
namespace rt {
struct Material;

inline ColourRGB Lighting(Material const& material, PointLight const& light,
                          Point3 const& point, Vector3 const& eye,
                          Vector3 const& normal) {
  auto const effective_colour = light.colour * material.colour;
  auto const light_vec = Normalize(light.position - point);
  auto const eye_vec = Normalize(eye);
  auto const normal_vec = Normalize(normal);

  auto const ambient = effective_colour * material.ambient;

  auto const light_dot_normal = Dot(light_vec, normal_vec);
  if (light_dot_normal <= 0.0f) {
    return ambient;
  }

  auto const diffuse =
      effective_colour * material.diffuse * light_dot_normal;

  auto const reflect_vec = Reflect(-light_vec, normal_vec);
  auto const reflect_dot_eye = Dot(reflect_vec, eye_vec);

  auto specular = ColourRGB::Black();
  if (reflect_dot_eye > 0.0f) {
    auto const factor = std::pow(reflect_dot_eye, material.shininess);
    specular = light.colour * material.specular * factor;
  }

  return ambient + diffuse + specular;
}
}  // namespace rt
#endif  // RAYTRACER_LIGHTING_H

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_LIGHTING_H
#define RAYTRACER_LIGHTING_H
#include "colour_rgb.h"
#include "material.h"
#include "rt/world/point_light.h"
#include "rt/world/surface_interaction.h"
#include "rt/world/world.h"
namespace rt {

inline constexpr float kRaySurfaceOffset = 2e-3f;
inline constexpr int32_t kMaxReflections = 5;

inline ColourRGB Lighting(Material const& material, Intersectable const& shape,
                          PointLight const& light, Point3 const& point,
                          Vector3 const& eye, Vector3 const& normal,
                          bool const in_shadow = false) {
  auto const effective_colour =
      light.colour * material.pattern->ColourAtShape(shape, point);
  auto const light_vec = Normalize(light.position - point);
  auto const eye_vec = Normalize(eye);
  auto const normal_vec = Normalize(normal);

  auto const ambient = effective_colour * material.ambient;

  if (in_shadow) {
    return ambient;
  }

  auto const light_dot_normal = Dot(light_vec, normal_vec);
  if (light_dot_normal <= 0.0f) {
    return ambient;
  }

  auto const diffuse = effective_colour * material.diffuse * light_dot_normal;

  auto const reflect_vec = Reflect(-light_vec, normal_vec);
  auto const reflect_dot_eye = Dot(reflect_vec, eye_vec);

  auto specular = ColourRGB::Black();
  if (reflect_dot_eye > 0.0f) {
    auto const factor = std::pow(reflect_dot_eye, material.shininess);
    specular = light.colour * material.specular * factor;
  }

  return ambient + diffuse + specular;
}

inline bool IsShadowed(World const& world, PointLight const& light,
                       Point3 const& point) {
  auto const vec = light.position - point;
  auto const distance = Length(vec);
  auto const direction = Normalize(vec);

  auto const ray = Ray(point, direction);

  auto const intersections = IntersectWorld(world, ray);

  return std::ranges::any_of(intersections, [&distance](auto const& hit) {
    return hit.t > 0.0f && hit.t < distance;
  });
}

inline ColourRGB ReflectedColour(World const& world,
                                 SurfaceInteraction const& surface, int32_t remaining = kMaxReflections);

inline ColourRGB ShadeHit(World const& world,
                          SurfaceInteraction const& surface, int32_t const remaining = kMaxReflections) {
  ColourRGB colour{0.0f, 0.0f, 0.0f};
  // TODO: investigate needed offset for speckling, previously 2e-3f
  auto const offset_point = surface.point + surface.normal * kRaySurfaceOffset;

  for (auto const& light : world.lights_) {
    auto const in_shade = IsShadowed(world, light, offset_point);
    colour = colour + Lighting(surface.intersection.object->GetMaterial(),
                               *surface.intersection.object, light, offset_point,
                               surface.eye, surface.normal, in_shade);
  }

  auto const reflected = ReflectedColour(world, surface, remaining);
  return colour * (1.0f - surface.intersection.object->GetMaterial().reflective) + reflected;
  return colour + reflected;
}

inline ColourRGB ColourAt(World const& world, Ray const& ray, int32_t const remaining = kMaxReflections) {
  auto const intersections = IntersectWorld(world, ray);

  auto const iter =
      std::ranges::upper_bound(intersections, 0.0f, {}, &Intersection::t);
  if (iter == intersections.cend()) {
    return ColourRGB::Black();
  }

  auto const info = ComputeSurfaceInteraction(*iter, ray);
  return ShadeHit(world, info, remaining);
}

inline ColourRGB ReflectedColour(World const& world,
                                 SurfaceInteraction const& surface, int32_t const remaining) {
  if (remaining == 0 || surface.intersection.object->GetMaterial().reflective == 0.0f) {
    return {0.0f, 0.0f, 0.0f};
  }

  auto const ray = Ray{surface.point + surface.normal * kRaySurfaceOffset, surface.reflect};
  auto const colour = ColourAt(world, ray, remaining -1 );

  return colour * surface.intersection.object->GetMaterial().reflective;

}

}  // namespace rt
#endif  // RAYTRACER_LIGHTING_H

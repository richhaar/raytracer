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
                                 SurfaceInteraction const& surface,
                                 int32_t remaining = kMaxReflections);

inline ColourRGB RefractedColour(World const& world,
                                 SurfaceInteraction const& surface,
                                 int32_t const remaining);

inline float Schlick(SurfaceInteraction const& surface);

inline ColourRGB ShadeHit(World const& world, SurfaceInteraction const& surface,
                          int32_t const remaining = kMaxReflections) {
  ColourRGB colour{0.0f, 0.0f, 0.0f};

  for (auto const& light : world.lights_) {
    auto const in_shade = IsShadowed(world, light, surface.over_point);
    colour = colour + Lighting(surface.intersection.object->GetMaterial(),
                               *surface.intersection.object, light,
                               surface.over_point, surface.eye, surface.normal,
                               in_shade);
  }

  auto const reflected = ReflectedColour(world, surface, remaining);
  auto const refraction = RefractedColour(world, surface, remaining);

  auto const& material = surface.intersection.object->GetMaterial();

  if (material.reflective > 0.0f && material.transparency > 0.0f) {
    auto const reflectance = Schlick(surface);
    return colour + reflected * reflectance + (1.0f - reflectance) * refraction;
  }

  // TODO investigate if solid reflection is nicer or not
  // return colour * (1.0f
  // -surface.intersection.object->GetMaterial().transparency) + refraction;
  // return colour * (1.0f
  // -surface.intersection.object->GetMaterial().reflective) + reflected;

  // std::cout << "colour: " << colour.red << " " << colour.green << " " <<
  // colour.blue << std::endl;
  //  std::cout << "reflected: " << reflected.red << " " << reflected.green << "
  //  " << reflected.blue << std::endl; std::cout << "refraction: " <<
  //  refraction.red << " " << refraction.green << " " << refraction.blue <<
  //  std::endl;
  return colour + reflected + refraction;
}

inline ColourRGB ColourAt(World const& world, Ray const& ray,
                          int32_t const remaining = kMaxReflections) {
  auto const intersections = IntersectWorld(world, ray);

  auto const iter =
      std::ranges::upper_bound(intersections, 0.0f, {}, &Intersection::t);
  if (iter == intersections.cend()) {
    return ColourRGB::Black();
  }

  auto const info = ComputeSurfaceInteraction(*iter, ray, intersections);
  return ShadeHit(world, info, remaining);
}

inline ColourRGB ReflectedColour(World const& world,
                                 SurfaceInteraction const& surface,
                                 int32_t const remaining) {
  if (remaining == 0 ||
      surface.intersection.object->GetMaterial().reflective == 0.0f) {
    return {0.0f, 0.0f, 0.0f};
  }

  auto const ray = Ray{surface.over_point, surface.reflect};
  auto const colour = ColourAt(world, ray, remaining - 1);

  return colour * surface.intersection.object->GetMaterial().reflective;
}

inline ColourRGB RefractedColour(World const& world,
                                 SurfaceInteraction const& surface,
                                 int32_t const remaining) {
  if (remaining == 0 ||
      surface.intersection.object->GetMaterial().transparency == 0.0f) {
    // std::cout << "no transparency" << std::endl;
    return ColourRGB::Black();
  }

  // Snell's law for checking internal reflection
  auto const n_ratio = surface.n1 / surface.n2;
  auto const cos_i = Dot(surface.eye, surface.normal);
  auto const sin2_t = n_ratio * n_ratio * (1.0f - cos_i * cos_i);

  // std::cout << n_ratio << " " << cos_i << " " << sin2_t << std::endl;

  if (sin2_t > 1.0f) {
    return ColourRGB::Black();
  }
  auto const cos_t = std::sqrt(1.0f - sin2_t);
  auto const refracted_direction =
      surface.normal * (n_ratio * cos_i - cos_t) - surface.eye * n_ratio;

  auto const refracted_ray = Ray{surface.under_point, refracted_direction};
  auto const colour = ColourAt(world, refracted_ray, remaining - 1);
  // std::cout << "colour from colour at : " << colour.red << " " <<
  // colour.green << " " << colour.blue << " with transparency" <<
  // surface.intersection.object->GetMaterial().transparency << std::endl;

  auto const mult =
      colour * surface.intersection.object->GetMaterial().transparency;
  // std::cout << "mult : " << mult.red << " " << mult.green << " " << mult.blue
  // << std::endl;

  return colour * surface.intersection.object->GetMaterial().transparency;
}

inline float Schlick(SurfaceInteraction const& surface) {
  auto cos = Dot(surface.eye, surface.normal);

  if (surface.n1 > surface.n2) {
    auto const n = surface.n1 / surface.n2;
    auto const sin2_t = n * n * (1.0f - cos * cos);
      if (sin2_t > 1.0f) {
        return 1.0f;
      }

    auto const cos_t = std::sqrt(1.0f - sin2_t);
    cos = cos_t;
  }

  auto const r0 = std::pow((surface.n1 - surface.n2) / (surface.n1 + surface.n2), 2.0f);
  return r0 + (1.0f - r0) * std::pow(1.0f - cos, 5.0f);

  return 0.0f;
}
}  // namespace rt
#endif  // RAYTRACER_LIGHTING_H

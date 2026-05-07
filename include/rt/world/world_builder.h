// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_WORLD_BUILDER_H
#define RAYTRACER_WORLD_BUILDER_H
#include <memory>

#include "rt/world/plane.h"
#include "rt/world/point_light.h"
#include "rt/world/sphere.h"
#include "rt/world/world.h"

namespace rt {
class WorldBuilder {
  World world_;

 public:
  WorldBuilder& AddLight(PointLight l) {
    world_.lights_.push_back(std::move(l));
    return *this;
  }

  WorldBuilder& AddPlane(Matrix<4, 4> const& transform,
                         Material const& material) {
    auto plane = std::make_unique<Plane>();
    plane->SetTransform(transform);
    plane->SetMaterial(material);
    world_.objects_.push_back(std::move(plane));
    return *this;
  }

  WorldBuilder& AddSphere(Matrix<4, 4> const& transform,
                          Material const& material) {
    auto sphere = std::make_unique<Sphere>();
    sphere->SetTransform(transform);
    sphere->SetMaterial(material);
    world_.objects_.push_back(std::move(sphere));
    return *this;
  }

  World Build() { return std::move(world_); }
};

inline World DefaultWorld() {
  WorldBuilder builder;
  return builder
      .AddLight(PointLight(ColourRGB::White(), Point3(-10.0f, 10.0f, -10.0f)))
      .AddSphere(Matrix<4, 4>::Identity(),
                 Material{.pattern = std::make_shared<SolidColour>(ColourRGB(0.8f, 1.0f, 0.6f)),
                          .diffuse = 0.7f,
                          .specular = 0.2f})
      .AddSphere(Scaling(0.5f, 0.5f, 0.5f), Material{})
      .Build();
}
}  // namespace rt

#endif  // RAYTRACER_WORLD_BUILDER_H

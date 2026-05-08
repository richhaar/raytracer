// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/graphics/lighting.h"
#include "rt/graphics/material.h"
#include "rt/graphics/pattern.h"
#include "rt/math/vector3.h"
#include "rt/world/point_light.h"
#include "rt/world/world_builder.h"

namespace rt {
namespace {

TEST(Lighting, EyeBetweenLightAndSurface) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 1.9f);
  ASSERT_FLOAT_EQ(g, 1.9f);
  ASSERT_FLOAT_EQ(b, 1.9f);
}

TEST(Lighting, EyeOffsetToLightBy45) {
  auto const eye =
      Vector3{0.0f, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 1.0f);
}

TEST(Lighting, LightOffsetBy45) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 0.7364f, 1e-5);
  ASSERT_NEAR(g, 0.7364f, 1e-5);
  ASSERT_NEAR(b, 0.7364f, 1e-5);
}

TEST(Lighting, EyeAndLightOnOppositeSidesOfNormalAt45) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 0.7364f, 1e-5);
  ASSERT_NEAR(g, 0.7364f, 1e-5);
  ASSERT_NEAR(b, 0.7364f, 1e-5);
}

TEST(Lighting, EyeInPathOfReflectionVector) {
  auto const eye =
      Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 1.6364f, 1e-5f);
  ASSERT_NEAR(g, 1.6364f, 1e-5f);
  ASSERT_NEAR(b, 1.6364f, 1e-5f);
}

TEST(Lighting, LightBehindSurface) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, 10.0f}};

  auto const [r, g, b] =
      Lighting(Material{}, Sphere{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 0.1f);
  ASSERT_FLOAT_EQ(g, 0.1f);
  ASSERT_FLOAT_EQ(b, 0.1f);
}

TEST(ShadeHit, ShadingIntersection) {
  auto const world = DefaultWorld();
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  ASSERT_FALSE(world.objects_.empty());
  auto const& sphere = world.objects_[0];

  auto const hit = Intersection{4.0f, sphere.get()};
  auto const info = ComputeSurfaceInteraction(hit, ray);

  auto const [r, g, b] = ShadeHit(world, info);
  ASSERT_NEAR(r, 0.38066f, 1e-5f);
  ASSERT_NEAR(g, 0.47583f, 1e-5f);
  ASSERT_NEAR(b, 0.2855f, 1e-5f);
}

TEST(ShadeHit, ShadingIntersectionFromInside) {
  WorldBuilder builder;
  auto const world =
      builder
          .AddLight(PointLight(ColourRGB::White(), Point3(0.0f, 0.25f, 0.0f)))
          .AddSphere(Matrix<4, 4>::Identity(),
                     Material{.pattern = std::make_shared<SolidColour>(
                                  0.8f, 1.0f, 0.6f),
                              .diffuse = 0.7f,
                              .specular = 0.2f})
          .AddSphere(Scaling(0.5f, 0.5f, 0.5f), Material{})
          .Build();

  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const& sphere = world.objects_[1];

  auto const hit = Intersection{0.5f, sphere.get()};
  auto const info = ComputeSurfaceInteraction(hit, ray);

  auto const [r, g, b] = ShadeHit(world, info);
  ASSERT_NEAR(r, 0.90498f, 1e-5f);
  ASSERT_NEAR(g, 0.90498f, 1e-5f);
  ASSERT_NEAR(b, 0.90498f, 1e-5f);
}

TEST(ColourAt, NoIntersectionOnRay) {
  auto const world = DefaultWorld();
  auto constexpr ray =
      Ray(Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 1.0f, 0.0f});
  auto const [r, g, b] = ColourAt(world, ray);

  ASSERT_FLOAT_EQ(r, 0.0f);
  ASSERT_FLOAT_EQ(g, 0.0f);
  ASSERT_FLOAT_EQ(b, 0.0f);
}

TEST(ColourAt, RayHitsSphere) {
  auto const world = DefaultWorld();
  auto constexpr ray =
      Ray(Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const [r, g, b] = ColourAt(world, ray);

  ASSERT_NEAR(r, 0.38066f, 1e-4f);
  ASSERT_NEAR(g, 0.47583f, 1e-4f);
  ASSERT_NEAR(b, 0.2855f, 1e-4f);
}

TEST(ColourAt, IntersectionBehindRay) {
  auto const world = DefaultWorld();

  auto constexpr ray =
      Ray(Point3{0.0f, 0.0f, 0.75f}, Vector3{0.0f, 0.0f, -1.0f});
  auto const [r, g, b] = ColourAt(world, ray);

  ASSERT_NEAR(r, 0.1f, 1e-5f);
  ASSERT_NEAR(g, 0.1f, 1e-5f);
  ASSERT_NEAR(b, 0.1f, 1e-5f);
}

TEST(IsShadowed, NoShadow) {
  auto const world = DefaultWorld();
  auto constexpr point = Point3{0.0f, 10.0f, 0.0f};
  ASSERT_FALSE(IsShadowed(world, world.lights_[0], point));
}

TEST(IsShadowed, InShadow) {
  auto const world = DefaultWorld();
  auto constexpr point = Point3{10.0f, -10.0f, 10.0f};
  ASSERT_TRUE(IsShadowed(world, world.lights_[0], point));
}

TEST(IsShadowed, PointBehindLightSource) {
  auto const world = DefaultWorld();
  auto constexpr point = Point3{-20.0f, 20.0f, -20.0f};
  ASSERT_FALSE(IsShadowed(world, world.lights_[0], point));
}

TEST(IsShadowed, PointInFrontOfLightSource) {
  auto const world = DefaultWorld();
  auto constexpr point = Point3{-2.0f, 2.0f, -2.0f};
  ASSERT_FALSE(IsShadowed(world, world.lights_[0], point));
}

TEST(ShadeHit, InShadow) {
  WorldBuilder builder;
  auto const world =
      builder
          .AddLight(PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}})
          .AddSphere(Matrix<4, 4>::Identity(), Material{})
          .AddSphere(Translation(0.0f, 0.0f, 10.0f), Material{})
          .Build();

  auto constexpr ray = Ray(Point3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 0.0f, 1.0f));
  auto const hit = Intersection(4.0f, world.objects_[1].get());
  auto const info = ComputeSurfaceInteraction(hit, ray);
  auto const [r, g, b] = ShadeHit(world, info);

  ASSERT_FLOAT_EQ(r, 0.1f);
  ASSERT_FLOAT_EQ(g, 0.1f);
  ASSERT_FLOAT_EQ(b, 0.1f);
}

TEST(Lighting, ColourWithStripePattern) {
  auto const pattern =
      std::make_shared<StripePattern>(ColourRGB::White(), ColourRGB::Black());
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}};
  auto const material = Material{
      .pattern = pattern, .ambient = 1.0f, .diffuse = 0.0f, .specular = 0.0f};

  auto const colour1 = Lighting(material, Sphere{}, light,
                                Point3{0.9f, 0.0f, 0.0f}, eye, normal);
  auto const colour2 = Lighting(material, Sphere{}, light,
                                Point3{1.1f, 0.0f, 0.0f}, eye, normal);

  ASSERT_FLOAT_EQ(colour1.red, 1.0f);
  ASSERT_FLOAT_EQ(colour1.green, 1.0f);
  ASSERT_FLOAT_EQ(colour1.blue, 1.0f);

  ASSERT_FLOAT_EQ(colour2.red, 0.0f);
  ASSERT_FLOAT_EQ(colour2.green, 0.0f);
  ASSERT_FLOAT_EQ(colour2.blue, 0.0f);
}

TEST(ReflectiveColour, NonReflectiveMaterial) {
  auto const world = DefaultWorld();
  auto const ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  ASSERT_TRUE(world.objects_.size() > 1);
  auto const& shape = world.objects_[1];
  auto const intersection = Intersection(1.0, shape.get());
  auto const c = ComputeSurfaceInteraction(intersection, ray);
  auto const [r, g, b] = ReflectedColour(world, c);
  ASSERT_FLOAT_EQ(r, 0.0f);
  ASSERT_FLOAT_EQ(g, 0.0f);
  ASSERT_FLOAT_EQ(b, 0.0f);
}

TEST(ReflectiveColour, ReflectiveMaterial) {
  auto world = DefaultWorld();
  auto plane = std::make_unique<Plane>();
  plane->SetTransform(Translation(0.0f, -1.0f, 0.0f));
  plane->SetMaterial(Material{.reflective = 0.5f});
  world.objects_.push_back(std::move(plane));

  auto const ray =
      Ray{Point3{0.0f, 0.0f, -3.0f},
          Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f}};
  ASSERT_EQ(world.objects_.size(), 3);
  auto const& shape = world.objects_[2];
  auto const intersection = Intersection(std::sqrt(2.0f), shape.get());
  auto const c = ComputeSurfaceInteraction(intersection, ray);
  auto const [r, g, b] = ReflectedColour(world, c);
  ASSERT_NEAR(r, 0.19032f, 3e-5f);
  ASSERT_NEAR(g, 0.2379f, 3e-5f);
  ASSERT_NEAR(b, 0.14274f, 3e-5f);
}

TEST(ShadeHit, ReflectiveMaterial) {
  auto world = DefaultWorld();
  auto plane = std::make_unique<Plane>();
  plane->SetTransform(Translation(0.0f, -1.0f, 0.0f));
  plane->SetMaterial(Material{.reflective = 0.5f});
  world.objects_.push_back(std::move(plane));

  auto const ray =
      Ray{Point3{0.0f, 0.0f, -3.0f},
          Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f}};
  ASSERT_EQ(world.objects_.size(), 3);
  auto const& shape = world.objects_[2];
  auto const intersection = Intersection(std::sqrt(2.0f), shape.get());
  auto const c = ComputeSurfaceInteraction(intersection, ray);
  auto const [r, g, b] = ShadeHit(world, c);
  ASSERT_NEAR(r, 0.87677f, 3e-5f);
  ASSERT_NEAR(g, 0.92436f, 3e-5f);
  ASSERT_NEAR(b, 0.82918f, 3e-5f);
}

TEST(ShadeHit, ColourAtReflectiveRecursion) {
  auto builder = WorldBuilder{};
  auto const world =
      builder.AddLight(PointLight(ColourRGB::White(), Point3{0.0f, 0.0f, 0.0f}))
          .AddPlane(Translation(0.0f, -1.0f, 0.0f),
                    Material{.reflective = 1.0f})
          .AddPlane(Translation(0.0f, 1.0f, 0.0f), Material{.reflective = 1.0f})
          .Build();

  auto const ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f}};
  ASSERT_NO_THROW(ColourAt(world, ray));
}

TEST(ReflectedColour, MaxRecusionDepth) {
  auto world = DefaultWorld();
  auto plane = std::make_unique<Plane>();
  plane->SetTransform(Translation(0.0f, -1.0f, 0.0f));
  plane->SetMaterial(Material{.reflective = 0.5f});
  world.objects_.push_back(std::move(plane));

  auto const ray =
      Ray{Point3{0.0f, 0.0f, -3.0f},
          Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f}};
  ASSERT_EQ(world.objects_.size(), 3);
  auto const& shape = world.objects_[2];
  auto const intersection = Intersection(std::sqrt(2.0f), shape.get());
  auto const c = ComputeSurfaceInteraction(intersection, ray);
  auto const [r, g, b] = ReflectedColour(world, c, 0);
  ASSERT_FLOAT_EQ(r, 0.0f);
  ASSERT_FLOAT_EQ(g, 0.0f);
  ASSERT_FLOAT_EQ(b, 0.0f);
}

}  // namespace
}  // namespace rt
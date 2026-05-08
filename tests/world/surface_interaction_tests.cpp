// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include "gtest/gtest.h"
#include "rt/world/plane.h"
#include "rt/world/surface_interaction.h"
namespace rt {
namespace {

TEST(SurfaceInteraction, ComputeSurfaceInteraction) {
  auto constexpr ray = Ray(Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};

  auto const record = Intersection{4.0f, &sphere};

  auto const [hit, inside, point, eye, normal, _] = ComputeSurfaceInteraction(record, ray);

  ASSERT_EQ(hit.object, record.object);
  ASSERT_EQ(hit.t, record.t);

  ASSERT_FALSE(inside);
  ASSERT_TRUE(Near(point, Point3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(normal, Vector3{0.0f, 0.0f, -1.0f}));
}

TEST(SurfaceInteraction, ComputeSurfaceInteractionInsideRay) {
  auto constexpr ray = Ray(Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};
  auto const record = Intersection{1.0f, &sphere};

  auto const surface = ComputeSurfaceInteraction(record, ray);

  ASSERT_TRUE(surface.inside);
  ASSERT_TRUE(Near(surface.point, Point3{0.0f, 0.0f, 1.0f}));
  ASSERT_TRUE(Near(surface.eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(surface.normal, Vector3{0.0f, 0.0f, -1.0f}));
}

TEST(SurfaceInteraction, ReflectVector) {
  auto const plane = Plane{};
  auto const ray = Ray{Point3{0.0f, 0.0f, -1.0f}, Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f}};

  auto const intersection = Intersection(std::sqrt(2.0f), &plane);
  auto const surface = ComputeSurfaceInteraction(intersection, ray);

  auto const [x,y,z] = surface.reflect;
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(z, std::sqrt(2.0f) / 2.0f);
}
}  // namespace
}  // namespace rt
// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include "gtest/gtest.h"
#include "rt/math/point3.h"
#include "rt/math/vector3.h"
#include "rt/world/intersects.h"
#include "rt/world/ray.h"

namespace rt {
namespace {

TEST(Intersects, RayIntersectsSphereAtTwoPoints) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = Intersects(sph, ray);
  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first, 4.0f);
  ASSERT_EQ(intersections->second, 6.0f);
}

TEST(Intersects, RayIntersectsSphereAtOnePoints) {
  auto constexpr ray =
      Ray{Point3{0.0f, 1.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = Intersects(sph, ray);
  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first, 5.0f);
  ASSERT_EQ(intersections->second, 5.0f);
}

TEST(Intersects, RayDoesntIntersectSphere) {
  auto constexpr ray =
      Ray{Point3{0.0f, 2.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = Intersects(sph, ray);
  ASSERT_FALSE(intersections.has_value());
}

TEST(Intersects, RayStartsWithinSphere) {
  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const sph = Sphere{};
  auto const intersections = Intersects(sph, ray);

  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first, -1.0f);
  ASSERT_EQ(intersections->second, 1.0f);
}

TEST(Intersects, RayIntersectsBehind) {
  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const sph = Sphere{};
  auto const intersections = Intersects(sph, ray);

  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first, -6.0f);
  ASSERT_EQ(intersections->second, -4.0f);
}

}  // namespace
}  // namespace rt

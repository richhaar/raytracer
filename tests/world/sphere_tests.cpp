// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/world/sphere.h"
namespace rt {
namespace {
TEST(Sphere, Uuid) { ASSERT_NE(Sphere{}, Sphere{}); }

TEST(Sphere, RayIntersectsSphereAtTwoPoints) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = sph.Hit(ray);
  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first.t, 4.0f);
  ASSERT_EQ(intersections->second.t, 6.0f);
}

TEST(Sphere, RayIntersectsSphereAtOnePoints) {
  auto constexpr ray =
      Ray{Point3{0.0f, 1.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = sph.Hit(ray);
  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first.t, 5.0f);
  ASSERT_EQ(intersections->second.t, 5.0f);
}

TEST(Sphere, RayDoesntIntersectSphere) {
  auto constexpr ray =
      Ray{Point3{0.0f, 2.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto const sph = Sphere{};
  auto const intersections = sph.Hit(ray);
  ASSERT_FALSE(intersections.has_value());
}

TEST(Sphere, RayStartsWithinSphere) {
  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const sph = Sphere{};
  auto const intersections = sph.Hit(ray);

  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first.t, -1.0f);
  ASSERT_EQ(intersections->second.t, 1.0f);
}

TEST(Sphere, RayIntersectsBehind) {
  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const sph = Sphere{};
  auto const intersections = sph.Hit(ray);

  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first.t, -6.0f);
  ASSERT_EQ(intersections->second.t, -4.0f);
}

TEST(Sphere, RayIntersectsScaledSphere) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto sph = Sphere{};
  sph.SetTransform(Scaling(2.0f, 2.0f, 2.0f));

  auto const intersections = sph.Hit(ray);

  ASSERT_TRUE(intersections.has_value());
  ASSERT_EQ(intersections->first.t, 3.0f);
  ASSERT_EQ(intersections->second.t, 7.0f);
}

TEST(Sphere, RayMissesTranslatedSphere) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto sph = Sphere{};
  sph.SetTransform(Translation(5.0f, 0.0f, 0.0f));

  auto const intersections = sph.Hit(ray);

  ASSERT_FALSE(intersections.has_value());
}
}  // namespace
}  // namespace rt
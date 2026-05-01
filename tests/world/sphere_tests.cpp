// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/math/matrix_ops.h"
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

TEST(Sphere, NormalAtX) {
  auto const s = Sphere{};
  auto const [x, y, z] = s.NormalAt(Point3{1.0f, 0.0f, 0.0f});
  ASSERT_FLOAT_EQ(x, 1.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Sphere, NormalAtY) {
  auto const s = Sphere{};
  auto const [x, y, z] = s.NormalAt(Point3{0.0f, 1.0f, 0.0f});
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, 1.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Sphere, NormalAtXYZ) {
  auto const s = Sphere{};
  auto const root = std::sqrt(3.0f) / 3.0f;
  auto const [x, y, z] = s.NormalAt(Point3{root, root, root});
  ASSERT_FLOAT_EQ(x, root);
  ASSERT_FLOAT_EQ(y, root);
  ASSERT_FLOAT_EQ(z, root);
}

TEST(Sphere, NormalAtWithTransform) {
  auto s = Sphere{};
  s.SetTransform(Translation(0.0f, 1.0f, 0.0f));
  auto const [x, y, z] = s.NormalAt(Point3{0.0f, 1.70711f, -0.70711f});
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_NEAR(y, 0.70711f, 1e-5f);
  ASSERT_NEAR(z, -0.70711f, 1e-5f);
}

TEST(Sphere, NormalAtWithScaling) {
  auto s = Sphere{};
  s.SetTransform(Scaling(1.0f, 0.5f, 1.0f) *
                 RotateZ(std::numbers::pi_v<float> / 5.0f));
  auto const [x, y, z] =
      s.NormalAt(Point3{0.0f, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f});
  ASSERT_NEAR(x, 0.0f, 1e-7f);
  ASSERT_NEAR(y, 0.97014f, 1e-5f);
  ASSERT_NEAR(z, -0.24254f, 1e-5f);
}
}  // namespace
}  // namespace rt
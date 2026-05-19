// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/world/plane.h"

namespace rt {
namespace {

TEST(Plane, NormalAtOrigin) {
  auto const p = Plane();
  auto const [x, y, z] = p.NormalAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, 1.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Plane, NormalAtPoint) {
  auto const p = Plane();
  auto const [x, y, z] = p.NormalAt(Point3{-10.0f, 0.0f, 10.0f});
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, 1.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Plane, ParallelRay) {
  auto const p = Plane();
  auto constexpr ray =
      Ray{Point3{0.0f, 10.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const intersections = p.Hit(ray);
  ASSERT_FALSE(intersections);
}

TEST(Plane, CoplanarRay) {
  auto const p = Plane();
  auto constexpr ray = Ray{Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f}};
  auto const intersections = p.Hit(ray);
  ASSERT_FALSE(intersections);
}

TEST(Plane, RayIntersectFromAbove) {
  auto const p = Plane();
  auto constexpr ray =
      Ray{Point3{0.0f, 1.0f, 0.0f}, Vector3{0.0f, -1.0f, 1.0f}};
  auto const intersections = p.Hit(ray);
  ASSERT_TRUE(intersections);
  ASSERT_FLOAT_EQ(intersections->first.t, 1.0f);
  ASSERT_EQ(&p, intersections->first.object);
}

TEST(Plane, RayIntersectFromBelow) {
  auto const p = Plane();
  auto constexpr ray =
      Ray{Point3{0.0f, -1.0f, 0.0f}, Vector3{0.0f, 1.0f, 1.0f}};
  auto const intersections = p.Hit(ray);
  ASSERT_TRUE(intersections);
  ASSERT_FLOAT_EQ(intersections->first.t, 1.0f);
  ASSERT_EQ(&p, intersections->first.object);
}

}  // namespace

}  // namespace rt
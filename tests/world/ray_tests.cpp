// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/math/point3.h"
#include "rt/world/ray.h"

namespace rt {
namespace {

TEST(Ray, Origin) {
  const auto [origin, _] =
      Ray{Point3{2.0f, 3.0f, 4.0f}, Vector3{1.0f, 0.0f, 0.0f}};
  auto const [x, y, z] = origin;

  ASSERT_FLOAT_EQ(x, 2.0f);
  ASSERT_FLOAT_EQ(y, 3.0f);
  ASSERT_FLOAT_EQ(z, 4.0f);
}

TEST(Ray, Direction) {
  auto const [_, direction] =
      Ray{Point3{2.0f, 3.0f, 4.0f}, Vector3{1.0f, 0.0f, 0.0f}};
  auto const [x, y, z] = direction;

  ASSERT_FLOAT_EQ(x, 1.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Ray, Position) {
  auto constexpr ray = Ray{Point3{2.0f, 3.0f, 4.0f}, Vector3{1.0f, 0.0f, 0.0f}};

  auto const [t0_x, t0_y, t0_z] = Position(ray, 0.0f);
  ASSERT_FLOAT_EQ(t0_x, 2.0f);
  ASSERT_FLOAT_EQ(t0_y, 3.0f);
  ASSERT_FLOAT_EQ(t0_z, 4.0f);

  auto const [t1_x, t1_y, t1_z] = Position(ray, 1.0f);
  ASSERT_FLOAT_EQ(t1_x, 3.0f);
  ASSERT_FLOAT_EQ(t1_y, 3.0f);
  ASSERT_FLOAT_EQ(t1_z, 4.0f);

  auto const [tm1_x, tm1_y, tm1_z] = Position(ray, -1.0f);
  ASSERT_FLOAT_EQ(tm1_x, 1.0f);
  ASSERT_FLOAT_EQ(tm1_y, 3.0f);
  ASSERT_FLOAT_EQ(tm1_z, 4.0f);

  auto const [t2p5_x, t2p5_y, t2p5_z] = Position(ray, 2.5f);
  ASSERT_FLOAT_EQ(t2p5_x, 4.5f);
  ASSERT_FLOAT_EQ(t2p5_y, 3.0f);
  ASSERT_FLOAT_EQ(t2p5_z, 4.0f);
}

}  // namespace
}  // namespace rt
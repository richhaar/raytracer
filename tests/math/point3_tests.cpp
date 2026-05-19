// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/point3.h"

namespace rt {
namespace {
TEST(Point3, Construction) {
  auto const [x, y, z] = Point3{1.0, 2.0, 3.0};
  ASSERT_FLOAT_EQ(x, 1.0);
  ASSERT_FLOAT_EQ(y, 2.0);
  ASSERT_FLOAT_EQ(z, 3.0);
}

TEST(Point3, AddingVectorToPoint) {
  auto const [x, y, z] = Point3{1.0, 2.0, 3.0} + Vector3{4.0, 5.0, 6.0};
  ASSERT_FLOAT_EQ(x, 5.0);
  ASSERT_FLOAT_EQ(y, 7.0);
  ASSERT_FLOAT_EQ(z, 9.0);
}

TEST(Point3, SubtractVectorFromPoint) {
  auto const [x, y, z] = Point3{1.0, 2.0, 3.0} - Vector3{4.0, 5.0, 6.0};
  ASSERT_FLOAT_EQ(x, -3.0);
  ASSERT_FLOAT_EQ(y, -3.0);
  ASSERT_FLOAT_EQ(z, -3.0);
}

TEST(Point3, Point3MinusPoint3GivesVector3) {
  testing::StaticAssertTypeEq<
      decltype(Point3{1.0, 2.0, 3.0} - Point3{4.0, 5.0, 6.0}), Vector3>();
}

}  // namespace
}  // namespace rt

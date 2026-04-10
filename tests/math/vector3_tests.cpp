// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT Licence
#include <gtest/gtest.h>

#include "rt/math/vector3.h"

namespace rt {
namespace {
TEST(Vector3, Construction) {
  auto const [x, y, z] = Vector3{1.0f, 2.0f, 3.0f};
  ASSERT_FLOAT_EQ(x, 1.0f);
  ASSERT_FLOAT_EQ(y, 2.0f);
  ASSERT_FLOAT_EQ(z, 3.0f);
}

TEST(Vector3, Vector3AddVector3) {
  auto const [x, y, z] = Vector3{1.0f, 2.0f, 3.0f} + Vector3{4.0f, 5.0f, 6.0f};
  ASSERT_FLOAT_EQ(x, 5.0f);
  ASSERT_FLOAT_EQ(y, 7.0f);
  ASSERT_FLOAT_EQ(z, 9.0f);
}

TEST(Vector3, Vector3SubtractVector3) {
  auto const [x, y, z] = Vector3{1.0f, 2.0f, 3.0f} - Vector3{4.0f, 5.0f, 6.0f};
  ASSERT_FLOAT_EQ(x, -3.0f);
  ASSERT_FLOAT_EQ(y, -3.0f);
  ASSERT_FLOAT_EQ(z, -3.0f);
}

TEST(Vector3, Negate) {
  auto const [x, y, z] = -Vector3{1.0f, 2.0f, 3.0f};
  ASSERT_FLOAT_EQ(x, -1.0f);
  ASSERT_FLOAT_EQ(y, -2.0f);
  ASSERT_FLOAT_EQ(z, -3.0f);
}

TEST(Vector3, ScaleVectorFloatLHS) {
  auto const [x, y, z] = 3.0f * Vector3{1.0f, 2.0f, 3.0f};
  ASSERT_FLOAT_EQ(x, 3.0f);
  ASSERT_FLOAT_EQ(y, 6.0f);
  ASSERT_FLOAT_EQ(z, 9.0f);
}

TEST(Vector3, ScaleVectorFloatRHS) {
  auto const [x, y, z] = Vector3{1.0f, 2.0f, 3.0f} * 2.0f;
  ASSERT_FLOAT_EQ(x, 2.0f);
  ASSERT_FLOAT_EQ(y, 4.0f);
  ASSERT_FLOAT_EQ(z, 6.0f);
}

TEST(Vector3, Length) {
  ASSERT_FLOAT_EQ(Length(Vector3{1.0f, 0.0f, 0.0f}), 1.0f);
  ASSERT_FLOAT_EQ(Length(Vector3{1.0f, 4.0f, 8.0f}), 9.0f);
  ASSERT_FLOAT_EQ(Length(Vector3{2.0f, 3.0f, 6.0f}), 7.0f);
}

TEST(Vector3, NormalizeUnitVector) {
  auto const [x, y, z] = Normalize(Vector3{1.0f, 0.0f, 0.0f});
  ASSERT_FLOAT_EQ(x, 1.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Vector3, NormalizeLargeVector) {
  auto const [x, y, z] = Normalize(Vector3{3.0f, 4.0f, 0.0f});
  EXPECT_FLOAT_EQ(x, 0.6f);
  EXPECT_FLOAT_EQ(y, 0.8f);
  EXPECT_FLOAT_EQ(z, 0.0f);
}

TEST(Vector3, NormalizeSmallVector) {
  auto const [x, y, z] = Normalize(Vector3{0.0f, 0.3f, 0.4f});
  EXPECT_FLOAT_EQ(x, 0.0f);
  EXPECT_FLOAT_EQ(y, 0.6f);
  EXPECT_FLOAT_EQ(z, 0.8f);
}

TEST(Vector3, DotProduct) {
  EXPECT_FLOAT_EQ(Dot(Vector3{1.0f, 2.0f, 3.0f}, Vector3{2.0f, 3.0f, 4.0f}),
                  20.0f);
  EXPECT_FLOAT_EQ(Dot(Vector3{1.0f, 0.0f, 0.0f}, Vector3{0.0f, 1.0f, 2.0f}),
                  0.0f);
}

TEST(Vector3, CrossProduct) {
  auto constexpr a = Vector3{1.0f, 2.0f, 3.0f};
  auto constexpr b = Vector3{2.0f, 3.0f, 4.0f};

  auto const [ab_x, ab_y, ab_z] = Cross(a, b);
  EXPECT_FLOAT_EQ(ab_x, -1.0);
  EXPECT_FLOAT_EQ(ab_y, 2.0);
  EXPECT_FLOAT_EQ(ab_z, -1.0);

  auto const [ba_x, ba_y, ba_z] = Cross(b, a);
  EXPECT_FLOAT_EQ(ba_x, 1.0);
  EXPECT_FLOAT_EQ(ba_y, -2.0);
  EXPECT_FLOAT_EQ(ba_z, 1.0);
}

TEST(Vector3, NearComparison) {
  ASSERT_TRUE(Near(Vector3{1.0f, 0.0f, 0.0f}, Vector3{0.9999999f, 0.0f, 0.0f}));
  ASSERT_FALSE(Near(Vector3{1.0f, 0.0f, 0.0f}, Vector3{1.1f, 0.0f, 0.0f}));
  ASSERT_FALSE(Near(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1e-5f}));
}

TEST(Vector3, NearComparisonCustomEpsilon) {
  ASSERT_TRUE(
      Near(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1e-5f}, 1e-5f));
  ASSERT_TRUE(Near(Vector3{0.0f, 0.0f, 0.0f}, Vector3{1.0f, 0.0f, 0.0f}, 1.0f));
  ASSERT_FALSE(
      Near(Vector3{0.0f, 0.0f, 0.0f}, Vector3{1.0f, 0.0f, 0.0f}, 0.99f));
}
}  // namespace
}  // namespace rt

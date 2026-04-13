// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/matrix.h"

namespace rt {
namespace {
TEST(Matrix, Default2x2) {
  auto constexpr m = Matrix<2, 2>{};

  EXPECT_FLOAT_EQ(m(0, 0), 0.0f);
  EXPECT_FLOAT_EQ(m(0, 1), 0.0f);
  EXPECT_FLOAT_EQ(m(1, 0), 0.0f);
  EXPECT_FLOAT_EQ(m(1, 1), 0.0f);
}

TEST(Matrix, 2x2) {
  auto constexpr m = Matrix<2, 2>{-3.0f, 5.0f, 1.0f, -2.0f};

  ASSERT_FLOAT_EQ(m(0, 0), -3.0f);
  ASSERT_FLOAT_EQ(m(0, 1), 5.0f);
  ASSERT_FLOAT_EQ(m(1, 0), 1.0f);
  ASSERT_FLOAT_EQ(m(1, 1), -2.0f);
}

TEST(Matrix, 4x4) {
  auto constexpr m =
      Matrix<4, 4>{1.0f, 2.0f,  3.0f,  4.0f,  5.5f,  6.5f,  7.5f,  8.5f,
                   9.0f, 10.0f, 11.0f, 12.0f, 13.5f, 14.5f, 15.5f, 16.5f};

  ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
  ASSERT_FLOAT_EQ(m(0, 3), 4.0f);
  ASSERT_FLOAT_EQ(m(1, 0), 5.5f);
  ASSERT_FLOAT_EQ(m(1, 2), 7.5f);

  ASSERT_FLOAT_EQ(m(2, 2), 11.0f);
  ASSERT_FLOAT_EQ(m(3, 0), 13.5f);
  ASSERT_FLOAT_EQ(m(3, 2), 15.5f);
}

TEST(Matrix, 3x3Identity) {
  auto constexpr m = Matrix<3, 3>::Identity();

  ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
  ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
  ASSERT_FLOAT_EQ(m(0, 2), 0.0f);

  ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
  ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
  ASSERT_FLOAT_EQ(m(1, 2), 0.0f);

  ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
  ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
  ASSERT_FLOAT_EQ(m(1, 2), 0.0f);
}

TEST(Matrix, NormSquared) {
  auto constexpr matrix = Matrix<2, 2>{1.0f, 2.0f, 4.0f, 2.0f};
  ASSERT_FLOAT_EQ(NormSquared(matrix), 25.0f);
}
}  // namespace
}  // namespace rt
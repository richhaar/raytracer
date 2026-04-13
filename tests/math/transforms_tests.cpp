// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/matrix_ops.h"
#include "rt/math/transforms.h"

namespace rt {
namespace {
TEST(Transforms, MatrixMultiplyPoint3) {
  auto constexpr matrix =
      Matrix<4, 4>{1.0f, 2.0f, 3.0f, 4.0f, 2.0f, 4.0f, 4.0f, 2.0f,
                   8.0f, 6.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  auto constexpr vec = Point3{1.0f, 2.0f, 3.0f};

  auto const [x, y, z] = matrix * vec;

  EXPECT_FLOAT_EQ(x, 18.0f);
  EXPECT_FLOAT_EQ(y, 24.0f);
  EXPECT_FLOAT_EQ(z, 33.0f);
}

TEST(Transforms, MatrixMultiplyVector3) {
  auto constexpr matrix =
      Matrix<4, 4>{1.0f, 2.0f, 3.0f, 4.0f, 2.0f, 4.0f, 4.0f, 2.0f,
                   8.0f, 6.0f, 4.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  auto constexpr vec = Vector3{1.0f, 2.0f, 3.0f};

  auto const [x, y, z] = matrix * vec;

  EXPECT_FLOAT_EQ(x, 14.0f);
  EXPECT_FLOAT_EQ(y, 22.0f);
  EXPECT_FLOAT_EQ(z, 32.0f);
}

TEST(Transforms, IdentityMultiplyVector3) {
  auto constexpr matrix = Matrix<3, 3>::Identity();
  auto constexpr vec = Vector3{1.0f, 2.0f, 3.0f};

  auto const [x, y, z] = matrix * vec;

  EXPECT_FLOAT_EQ(x, 1.0f);
  EXPECT_FLOAT_EQ(y, 2.0f);
  EXPECT_FLOAT_EQ(z, 3.0f);
}

TEST(Transforms, TranposeMatrix) {
  auto constexpr matrix = Matrix<2, 3>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  auto constexpr transposed = Transpose(matrix);

  EXPECT_FLOAT_EQ(transposed(0, 0), 1.0f);
  EXPECT_FLOAT_EQ(transposed(0, 1), 4.0f);
  EXPECT_FLOAT_EQ(transposed(1, 0), 2.0f);
  EXPECT_FLOAT_EQ(transposed(1, 1), 5.0f);
  EXPECT_FLOAT_EQ(transposed(2, 0), 3.0f);
  EXPECT_FLOAT_EQ(transposed(2, 1), 6.0f);
}

TEST(Transforms, TranposeMatrixSquare) {
  auto constexpr matrix =
      Matrix<4, 4>{0.0f, 9.0f, 3.0f, 0.0f, 9.0f, 8.0f, 0.0f, 8.0f,
                   1.0f, 8.0f, 5.0f, 3.0f, 0.0f, 0.0f, 5.0f, 8.0f};
  auto constexpr expected_matrix =
      Matrix<4, 4>{0.0f, 9.0f, 1.0f, 0.0f, 9.0f, 8.0f, 8.0f, 0.0f,
                   3.0f, 0.0f, 5.0f, 5.0f, 0.0f, 8.0f, 3.0f, 8.0f};

  ASSERT_TRUE(Near(Transpose(matrix), expected_matrix));
}
TEST(Transforms, TranposeIdentity) {
  ASSERT_TRUE(
      Near(Transpose(Matrix<3, 3>::Identity()), Matrix<3, 3>::Identity()));
}
}  // namespace
}  // namespace rt
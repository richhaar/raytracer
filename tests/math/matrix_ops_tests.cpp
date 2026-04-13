// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/matrix_ops.h"

namespace rt {
namespace {
TEST(MatrixOps, Addition) {
  auto constexpr matrix1 = Matrix<2, 2>{1.0f, 2.0f, 3.0f, 4.0f};
  auto constexpr matrix2 = Matrix<2, 2>{4.0f, 3.0f, 2.0f, 1.0f};

  auto const sum = matrix1 + matrix2;

  ASSERT_EQ(sum(0, 0), 5.0f);
  ASSERT_EQ(sum(0, 1), 5.0f);
  ASSERT_EQ(sum(1, 0), 5.0f);
  ASSERT_EQ(sum(1, 1), 5.0f);
}

TEST(MatrixOps, AdditionAssignment) {
  auto matrix1 = Matrix<2, 2>{1.1f, 2.2f, 3.3f, 4.4f};
  auto constexpr matrix2 = Matrix<2, 2>{1.1f, 2.2f, 3.3f, 4.4f};

  matrix1 += matrix2;
  ASSERT_EQ(matrix1(0, 0), 2.2f);
  ASSERT_EQ(matrix1(0, 1), 4.4f);
  ASSERT_EQ(matrix1(1, 0), 6.6f);
  ASSERT_EQ(matrix1(1, 1), 8.8f);
}

TEST(MatrixOps, Multiplication) {
  auto constexpr matrix1 =
      Matrix<4, 4>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
                   9.0f, 8.0f, 7.0f, 6.0f, 5.0f, 4.0,  3.0f, 2.0f};
  auto constexpr matrix2 =
      Matrix<4, 4>{-2.0f, 1.0f, 2.0f, 3.0f, 3.0f, 2.0f, 1.0f, -1.0f,
                   4.0f,  3.0f, 6.0f, 5.0f, 1.0f, 2.0f, 7.0f, 8.0f};
  auto const product = matrix1 * matrix2;

  ASSERT_FLOAT_EQ(product(0, 0), 20.0f);
  ASSERT_FLOAT_EQ(product(0, 1), 22.0f);
  ASSERT_FLOAT_EQ(product(0, 2), 50.0f);
  ASSERT_FLOAT_EQ(product(0, 3), 48.0f);

  ASSERT_FLOAT_EQ(product(1, 0), 44.0f);
  ASSERT_FLOAT_EQ(product(1, 1), 54.0f);
  ASSERT_FLOAT_EQ(product(1, 2), 114.0f);
  ASSERT_FLOAT_EQ(product(1, 3), 108.0f);

  ASSERT_FLOAT_EQ(product(2, 0), 40.0f);
  ASSERT_FLOAT_EQ(product(2, 1), 58.0f);
  ASSERT_FLOAT_EQ(product(2, 2), 110.0f);
  ASSERT_FLOAT_EQ(product(2, 3), 102.0f);

  ASSERT_FLOAT_EQ(product(3, 0), 16.0f);
  ASSERT_FLOAT_EQ(product(3, 1), 26.0f);
  ASSERT_FLOAT_EQ(product(3, 2), 46.0f);
  ASSERT_FLOAT_EQ(product(3, 3), 42.0f);
}

TEST(MatrixOps, NearOperator) {
  ASSERT_TRUE(Near(Matrix<3, 3>::Identity(), Matrix<3, 3>::Identity()));
}

TEST(MatrixOps, NearOperatorNotEqual) {
  auto matrix = Matrix<3, 3>::Identity();
  matrix(0, 0) = 1.00001f;
  ASSERT_FALSE(Near(Matrix<3, 3>::Identity(), matrix));
  ASSERT_FALSE(Near(Matrix<3, 3>::Identity(), matrix, 1e-5f));
}
}  // namespace
}  // namespace rt
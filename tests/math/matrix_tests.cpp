// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/matrix.h"
#include "rt/math/matrix_ops.h"
#include "rt/math/transforms.h"

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

TEST(Matrix, Determinant2x2) {
  auto constexpr matrix = Matrix<2, 2>{1.0f, 5.0f, -3.0f, 2.0f};
  ASSERT_FLOAT_EQ(Determinant(matrix), 17.0f);
}

TEST(Matrix, Submatrix3x3) {
  auto constexpr matrix =
      Matrix<3, 3>{1.0f, 5.0f, 0.0f, -3.0f, 2.0f, 7.0f, 0.0f, 6.0f, -3.0f};
  auto constexpr expected = Matrix<2, 2>{-3.0f, 2.0f, 0.0f, 6.0f};

  ASSERT_TRUE(Near(Submatrix(matrix, 0, 2), expected));
}

TEST(Matrix, Submatrix4x4) {
  auto constexpr matrix =
      Matrix<4, 4>{-6.0f, 1.0f, 1.0f, 6.0f, -8.0f, 5.0f, 8.0f,  6.0f,
                   -1.0f, 0.0f, 8.0f, 2.0f, -7.0f, 1.0f, -1.0f, 1.0f};
  auto constexpr expected =
      Matrix<3, 3>{-6.0f, 1.0f, 6.0f, -8.0f, 8.0f, 6.0f, -7.0f, -1.0f, 1.0f};

  ASSERT_TRUE(Near(Submatrix(matrix, 2, 1), expected));
}

TEST(Matrix, Minor) {
  auto constexpr matrix =
      Matrix<3, 3>{3.0f, 5.0f, 0.0f, 2.0f, -1.0f, -7.0f, 6.0f, -1.0f, 5.0f};
  auto const submatrix = Submatrix(matrix, 1, 0);
  ASSERT_EQ(Determinant(submatrix), 25.0f);
  ASSERT_EQ(Minor(matrix, 1, 0), 25.0f);
}

TEST(Matrix, Cofactor) {
  auto constexpr matrix =
      Matrix<3, 3>{3.0f, 5.0f, 0.0f, 2.0f, -1.0f, -7.0f, 6.0f, -1.0f, 5.0f};
  ASSERT_EQ(Minor(matrix, 0, 0), -12.0f);
  ASSERT_EQ(Cofactor(matrix, 0, 0), -12.0f);
  ASSERT_EQ(Minor(matrix, 1, 0), 25.0f);
  ASSERT_EQ(Cofactor(matrix, 1, 0), -25.0f);
}

TEST(Matrix, Determinant3x3) {
  auto constexpr matrix =
      Matrix<3, 3>{1.0f, 2.0f, 6.0f, -5.0f, 8.0f, -4.0f, 2.0f, 6.0f, 4.0f};

  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 0), 56.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 1), 12.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 2), -46.0f);
  ASSERT_FLOAT_EQ(Determinant(matrix), -196.0f);
}

TEST(Matrix, Determinant4x4) {
  auto constexpr matrix =
      Matrix<4, 4>{-2.0f, -8.0f, 3.0f,  5.0f, -3.0f, 1.0f, 7.0f, 3.0f,
                   1.0f,  2.0f,  -9.0f, 6.0f, -6.0f, 7.0f, 7.0f, -9.0f};

  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 0), 690.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 1), 447.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 2), 210.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 0, 3), 51.0f);
  ASSERT_FLOAT_EQ(Determinant(matrix), -4071.0f);
}

TEST(Matrix, CheckInvertability) {
  auto constexpr matrix =
      Matrix<4, 4>{6.0f, 4.0f,  4.0f, 4.0f,  5.0f, 5.0f, 7.0f, 6.0f,
                   4.0f, -9.0f, 3.0f, -7.0f, 9.0f, 1.0f, 7.0f, -6.0f};

  ASSERT_FLOAT_EQ(Determinant(matrix), -2120.0f);
  ASSERT_FALSE(IsSingularMatrix(matrix));
}

TEST(Matrix, CheckInvertabilityNonInvertiable) {
  auto constexpr matrix =
      Matrix<4, 4>{-4.0f, 2.0f,  -2.0f, -3.0f, 9.0f, 6.0f, 2.0f, 6.0f,
                   0.0f,  -5.0f, 1.0f,  -5.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  ASSERT_FLOAT_EQ(Determinant(matrix), 0.0f);
  ASSERT_TRUE(IsSingularMatrix(matrix));
}

TEST(Matrix, InverseMatrix) {
  auto constexpr matrix =
      Matrix<4, 4>{-5.0f, 2.0f, 6.0f,  -8.0f, 1.0f, -5.0f, 1.0f, 8.0f,
                   7.0f,  7.0f, -6.0f, -7.0f, 1.0f, -3.0f, 7.0f, 4.0f};

  auto const inverse = Inverse(matrix);

  ASSERT_FLOAT_EQ(Determinant(matrix), 532.0f);
  ASSERT_FLOAT_EQ(Cofactor(matrix, 2, 3), -160.0f);
  ASSERT_FLOAT_EQ(inverse(3, 2), -160.0f / 532.0f);

  ASSERT_FLOAT_EQ(Cofactor(matrix, 3, 2), 105.0f);
  ASSERT_FLOAT_EQ(inverse(2, 3), 105.0f / 532.0f);

  auto constexpr expected_inverse = Matrix<4, 4>{
      0.21805f,  0.45113f,  0.24060f,  -0.04511f, -0.80827f, -1.45677f,
      -0.44361f, 0.52068f,  -0.07895f, -0.22368f, -0.05263f, 0.19737f,
      -0.52256f, -0.81391f, -0.30075f, 0.30639f};

  for (std::size_t row = 0; row < 4; ++row) {
    for (std::size_t col = 0; col < 4; ++col) {
      ASSERT_NEAR(inverse(row, col), expected_inverse(row, col), 1e-5f);
    }
  }
}

TEST(Matrix, InverseMatrix2) {
  auto constexpr matrix =
      Matrix<4, 4>{8.0f,  -5.0f, 9.0f, 2.0f, 7.0f,  5.0f, 6.0f,  1.0f,
                   -6.0f, 0.0f,  9.0f, 6.0f, -3.0f, 0.0f, -9.0f, -4.0f};

  auto const inverse = Inverse(matrix);

  auto constexpr expected_inverse = Matrix<4, 4>{
      -0.15385f, -0.15385f, -0.28205f, -0.53846f, -0.07692f, 0.12308f,
      0.02564f,  0.03077f,  0.35897f,  0.35897f,  0.43590f,  0.92308f,
      -0.69231f, -0.69231f, -0.76923f, -1.92308f};

  for (std::size_t row = 0; row < 4; ++row) {
    for (std::size_t col = 0; col < 4; ++col) {
      ASSERT_NEAR(inverse(row, col), expected_inverse(row, col), 1e-5f);
    }
  }
}

TEST(Matrix, InverseMatrix3) {
  auto constexpr matrix =
      Matrix<4, 4>{9.0f,  3.0f, 0.0f, 9.0f, -5.0f, -2.0f, -6.0f, -3.0f,
                   -4.0f, 9.0f, 6.0f, 4.0f, -7.0f, 6.0f,  6.0f,  2.0f};

  auto const inverse = Inverse(matrix);

  auto constexpr expected_inverse = Matrix<4, 4>{
      -0.04074f, -0.07778f, 0.14444f,  -0.22222f, -0.07778f, 0.03333f,
      0.36667f,  -0.33333f, -0.02901f, -0.14630f, -0.10926f, 0.12963f,
      0.17778f,  0.06667f,  -0.26667f, 0.33333f};

  for (std::size_t row = 0; row < 4; ++row) {
    for (std::size_t col = 0; col < 4; ++col) {
      ASSERT_NEAR(inverse(row, col), expected_inverse(row, col), 1e-5f);
    }
  }
}

TEST(Matrix, InverseMatrixProduct) {
  auto constexpr A =
      Matrix<4, 4>{3.0f,  -9.0f, 7.0f, 3.0f, 3.0f,  -8.0f, 2.0f,  -9.0f,
                   -4.0f, 4.0f,  4.0f, 1.0f, -6.0f, 5.0f,  -1.0f, 1.0f};

  auto constexpr B =
      Matrix<4, 4>{8.0f, 2.0f, 2.0f, 2.0f, 3.0f, -1.0f, 7.0f, 0.0f,
                   7.0f, 0.0f, 5.0f, 4.0f, 6.0f, -2.0f, 0.0f, 5.0f};

  auto const C = A * B;

  auto const A2 = C * Inverse(B);

  for (std::size_t row = 0; row < 4; ++row) {
    for (std::size_t col = 0; col < 4; ++col) {
      ASSERT_NEAR(A(row, col), A2(row, col), 1e-6f);
    }
  }
}
}  // namespace
}  // namespace rt
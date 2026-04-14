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

TEST(Transforms, TranslationOnPoint3) {
  auto const transform = Translation(5, -3, 2);
  auto pos = Point3{-3.0f, 4.0f, 5.0f};
  auto const [x, y, z] = transform * pos;

  ASSERT_FLOAT_EQ(x, 2.0);
  ASSERT_FLOAT_EQ(y, 1.0);
  ASSERT_FLOAT_EQ(z, 7.0);
}

TEST(Transforms, InverseTranslationOnPoint3) {
  auto const transform = Inverse(Translation(5, -3, 2));
  auto constexpr pos = Point3{-3.0f, 4.0f, 5.0f};
  auto const [x, y, z] = transform * pos;

  ASSERT_FLOAT_EQ(x, -8.0);
  ASSERT_FLOAT_EQ(y, 7.0);
  ASSERT_FLOAT_EQ(z, 3.0);
}

TEST(Transforms, TranslationDoesntChangeVector3) {
  auto constexpr transform = Translation(5, -3, 2);
  auto constexpr pos = Vector3{-3.0f, 4.0f, 5.0f};
  auto const [x, y, z] = transform * pos;

  ASSERT_FLOAT_EQ(x, -3.0);
  ASSERT_FLOAT_EQ(y, 4.0);
  ASSERT_FLOAT_EQ(z, 5.0);
}

TEST(Transforms, ScalingPoint3) {
  auto constexpr scaling = Scaling(2.0f, 3.0f, 4.0f);
  auto constexpr point = Point3{-4.0f, 6.0f, 8.0f};
  auto const [x, y, z] = scaling * point;
  ASSERT_FLOAT_EQ(x, -8.0f);
  ASSERT_FLOAT_EQ(y, 18.0f);
  ASSERT_FLOAT_EQ(z, 32.0f);
}

TEST(Transforms, ScalingVector3) {
  auto constexpr scaling = Scaling(2.0f, 3.0f, 4.0f);
  auto constexpr vec = Vector3{-4.0f, 6.0f, 8.0f};
  auto const [x, y, z] = scaling * vec;
  ASSERT_FLOAT_EQ(x, -8.0f);
  ASSERT_FLOAT_EQ(y, 18.0f);
  ASSERT_FLOAT_EQ(z, 32.0f);
}

TEST(Transforms, InverseScalingVector3) {
  auto const scaling = Inverse(Scaling(2.0f, 3.0f, 4.0f));
  auto constexpr vec = Vector3{-4.0f, 6.0f, 8.0f};
  auto const [x, y, z] = scaling * vec;
  ASSERT_FLOAT_EQ(x, -2.0f);
  ASSERT_FLOAT_EQ(y, 2.0f);
  ASSERT_FLOAT_EQ(z, 2.0f);
}

TEST(Transforms, ReflectionViaScaling) {
  auto constexpr scaling = Scaling(-1.0f, 1.0f, 1.0f);
  auto constexpr point = Point3(2.0f, 3.0f, 4.0f);
  auto const [x, y, z] = scaling * point;
  ASSERT_FLOAT_EQ(x, -2.0f);
  ASSERT_FLOAT_EQ(y, 3.0f);
  ASSERT_FLOAT_EQ(z, 4.0f);
}

TEST(Transform, RotateXHalfQuarterTurn) {
  auto constexpr point = Point3{0.0f, 1.0f, 0.0f};
  auto const half_quarter = RotateX(std::numbers::pi_v<float> / 4.0f);
  auto const [x, y, z] = half_quarter * point;

  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(z, std::sqrt(2.0f) / 2.0f);
}

TEST(Transform, RotateXQuarterTurn) {
  auto constexpr point = Point3{0.0f, 1.0f, 0.0f};
  auto const full_quarter = RotateX(std::numbers::pi_v<float> / 2.0f);
  auto const [x, y, z] = full_quarter * point;

  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_NEAR(y, 0.0f, 1e-7f);
  ASSERT_FLOAT_EQ(z, 1.0f);
}

TEST(Transform, InverseRotateXHalfQuarter) {
  auto constexpr point = Point3{0.0f, 1.0f, 0.0f};
  auto const half_quarter = Inverse(RotateX(std::numbers::pi_v<float> / 4.0f));
  auto const [x, y, z] = half_quarter * point;

  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(z, -std::sqrt(2.0f) / 2.0f);
}

TEST(Transform, RotateYHalfQuarterTurn) {
  auto constexpr point = Point3{0.0f, 0.0f, 1.0f};
  auto const half_quarter = RotateY(std::numbers::pi_v<float> / 4.0f);
  auto const [x, y, z] = half_quarter * point;

  ASSERT_FLOAT_EQ(x, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, std::sqrt(2.0f) / 2.0f);
}

TEST(Transform, RotateYFullQuarterTurn) {
  auto constexpr point = Point3{0.0f, 0.0f, 1.0f};
  auto const full_quarter = RotateY(std::numbers::pi_v<float> / 2.0f);
  auto const [x, y, z] = full_quarter * point;

  ASSERT_FLOAT_EQ(x, 1.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_NEAR(z, 0.0f, 1e-7);
}

TEST(Transform, RotateZHalfQuarterTurn) {
  auto constexpr point = Point3{0.0f, 1.0f, 0.0f};
  auto const half_quarter = RotateZ(std::numbers::pi_v<float> / 4.0f);
  auto const [x, y, z] = half_quarter * point;

  ASSERT_FLOAT_EQ(x, -std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(y, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Transform, RotateZFullQuarterTurn) {
  auto constexpr point = Point3{0.0f, 1.0f, 0.0f};
  auto const full_quarter = RotateZ(std::numbers::pi_v<float> / 2.0f);
  auto const [x, y, z] = full_quarter * point;

  ASSERT_FLOAT_EQ(x, -1.0f);
  ASSERT_NEAR(y, 0.0f, 1e-7f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}

TEST(Shearing, ShearXOnY) {
  auto const shear = Shear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 5.0f);
  EXPECT_FLOAT_EQ(y, 3.0f);
  EXPECT_FLOAT_EQ(z, 4.0f);
}

TEST(Shearing, ShearXOnZ) {
  auto const shear = Shear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 6.0f);
  EXPECT_FLOAT_EQ(y, 3.0f);
  EXPECT_FLOAT_EQ(z, 4.0f);
}

TEST(Shearing, ShearYOnX) {
  auto constexpr shear = Shear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 2.0f);
  EXPECT_FLOAT_EQ(y, 5.0f);
  EXPECT_FLOAT_EQ(z, 4.0f);
}

TEST(Shearing, ShearYOnZ) {
  auto constexpr shear = Shear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 2.0f);
  EXPECT_FLOAT_EQ(y, 7.0f);
  EXPECT_FLOAT_EQ(z, 4.0f);
}

TEST(Shearing, ShearZOnX) {
  auto constexpr shear = Shear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 2.0f);
  EXPECT_FLOAT_EQ(y, 3.0f);
  EXPECT_FLOAT_EQ(z, 6.0f);
}

TEST(Shearing, ShearZOnY) {
  auto constexpr shear = Shear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
  auto constexpr pos = Point3{2.0f, 3.0f, 4.0f};
  auto const [x, y, z] = shear * pos;

  EXPECT_FLOAT_EQ(x, 2.0f);
  EXPECT_FLOAT_EQ(y, 3.0f);
  EXPECT_FLOAT_EQ(z, 7.0f);
}

TEST(Transforms, ChainedTransformations) {
  auto constexpr pos = Point3{1.0f, 0.0f, 1.0f};
  auto constexpr A = RotateX(std::numbers::pi_v<float> / 2.0f);
  auto constexpr B = Scaling(5.0f, 5.0f, 5.0f);
  auto constexpr C = Translation(10.0f, 5.0f, 7.0f);
  auto const [x, y, z] = C * B * A * pos;
  ASSERT_FLOAT_EQ(x, 15.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, 7.0f);
}
}  // namespace
}  // namespace rt
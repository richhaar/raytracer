// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/math/matrix_ops.h"
#include "rt/math/point3.h"
#include "rt/math/transforms.h"
#include "rt/math/vector3.h"
#include "rt/math/view_transform.h"

namespace rt {
namespace {

TEST(ViewTransform, DefaultOrientation) {
  auto constexpr from = Point3{0.0f, 0.0f, 0.0f};
  auto constexpr to = Point3{0.0f, 0.0f, -1.0f};
  auto constexpr up = Vector3{0.0f, 1.0f, 0.0f};
  ASSERT_TRUE(Near(ViewTransform(from, to, up), Matrix<4, 4>::Identity()));
}

TEST(ViewTransform, LookingInPositiveZ) {
  auto constexpr from = Point3{0.0f, 0.0f, 0.0f};
  auto constexpr to = Point3{0.0f, 0.0f, 1.0f};
  auto constexpr up = Vector3{0.0f, 1.0f, 0.0f};
  ASSERT_TRUE(Near(ViewTransform(from, to, up), Scaling(-1.0f, 1.0f, -1.0f)));
}

TEST(ViewTransform, ViewTranslatesAlongZ) {
  auto constexpr from = Point3{0.0f, 0.0f, 8.0f};
  auto constexpr to = Point3{0.0f, 0.0f, 0.0f};
  auto constexpr up = Vector3{0.0f, 1.0f, 0.0f};
  ASSERT_TRUE(
      Near(ViewTransform(from, to, up), Translation(0.0f, 0.0f, -8.0f)));
}

TEST(ViewTransform, ArbritraryViewTransform) {
  auto constexpr from = Point3{1.0f, 3.0f, 2.0f};
  auto constexpr to = Point3{4.0f, -2.0f, 8.0f};
  auto constexpr up = Vector3{1.0f, 1.0f, 0.0f};
  ASSERT_TRUE(
      Near(ViewTransform(from, to, up),
           Matrix<4, 4>{-0.50709f, 0.50709f, 0.67612f, -2.36643f, 0.76772f,
                        0.60609f, 0.12122f, -2.82843f, -0.35857f, 0.59761f,
                        -0.71714f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
           1e-5f));
}
}  // namespace
}  // namespace rt
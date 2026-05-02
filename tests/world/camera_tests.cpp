// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/math/matrix_ops.h"
#include "rt/world/camera.h"

namespace rt {
namespace {

TEST(Camera, Construction) {
  auto constexpr hsize = 160;
  auto constexpr vsize = 120;
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(hsize, vsize, fov);
  ASSERT_EQ(cam.hsize, hsize);
  ASSERT_EQ(cam.vsize, vsize);
  ASSERT_FLOAT_EQ(cam.fov, fov);
  ASSERT_TRUE(Near(cam.transform, Matrix<4, 4>::Identity()));
}

TEST(Camera, PixelSize) {
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(200, 125, fov);

  ASSERT_FLOAT_EQ(cam.pixel_size, 0.01f);
}

TEST(Camera, PixelSize2) {
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(125, 200, fov);

  ASSERT_FLOAT_EQ(cam.pixel_size, 0.01f);
}

TEST(Camera, RayForPixelCenter) {
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(201, 101, fov);

  auto const [origin, direction] = RayForPixel(cam, 100, 50);
  ASSERT_FLOAT_EQ(origin.x, 0.0f);
  ASSERT_FLOAT_EQ(origin.y, 0.0f);
  ASSERT_FLOAT_EQ(origin.z, 0.0f);

  ASSERT_NEAR(direction.x, 0.0f, 1e-7f);
  ASSERT_NEAR(direction.y, 0.0f, 1e-7f);
  ASSERT_FLOAT_EQ(direction.z, -1.0f);
}

TEST(Camera, RayForPixelCorner) {
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(201, 101, fov);

  auto const [origin, direction] = RayForPixel(cam, 0, 0);
  ASSERT_FLOAT_EQ(origin.x, 0.0f);
  ASSERT_FLOAT_EQ(origin.y, 0.0f);
  ASSERT_FLOAT_EQ(origin.z, 0.0f);

  ASSERT_NEAR(direction.x, 0.66519f, 1e-5f);
  ASSERT_NEAR(direction.y, 0.33259f, 1e-5f);
  ASSERT_NEAR(direction.z, -0.66851f, 1e-5f);
}

TEST(Camera, RayForPixelTransformedCamera) {
  auto constexpr fov = std::numbers::pi_v<float> / 2.0f;
  auto const cam = Camera::Create(201, 101, fov,
                                  RotateY(std::numbers::pi_v<float> / 4.0f) *
                                      Translation(0.0f, -2.0f, 5.0f));

  auto const [origin, direction] = RayForPixel(cam, 100, 50);
  ASSERT_FLOAT_EQ(origin.x, 0.0f);
  ASSERT_FLOAT_EQ(origin.y, 2.0f);
  ASSERT_FLOAT_EQ(origin.z, -5.0f);

  ASSERT_FLOAT_EQ(direction.x, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(direction.y, 0.0f);
  ASSERT_FLOAT_EQ(direction.z, -std::sqrt(2.0f) / 2.0f);
}
}  // namespace
}  // namespace rt

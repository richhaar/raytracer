// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/graphics/lighting.h"
#include "rt/graphics/material.h"
#include "rt/math/vector3.h"
#include "rt/world/point_light.h"

namespace rt {
namespace {

TEST(Lighting, EyeBetweenLightAndSurface) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 1.9f);
  ASSERT_FLOAT_EQ(g, 1.9f);
  ASSERT_FLOAT_EQ(b, 1.9f);
}

TEST(Lighting, EyeOffsetToLightBy45) {
  auto const eye =
      Vector3{0.0f, std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, -10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 1.0f);
}

TEST(Lighting, LightOffsetBy45) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 0.7364f, 1e-5);
  ASSERT_NEAR(g, 0.7364f, 1e-5);
  ASSERT_NEAR(b, 0.7364f, 1e-5);
}

TEST(Lighting, EyeAndLightOnOppositeSidesOfNormalAt45) {
  auto constexpr eye = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 0.7364f, 1e-5);
  ASSERT_NEAR(g, 0.7364f, 1e-5);
  ASSERT_NEAR(b, 0.7364f, 1e-5);
}

TEST(Lighting, EyeInPathOfReflectionVector) {
  auto const eye =
      Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 10.0f, -10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_NEAR(r, 1.6364f, 1e-5f);
  ASSERT_NEAR(g, 1.6364f, 1e-5f);
  ASSERT_NEAR(b, 1.6364f, 1e-5f);
}

TEST(Lighting, LightBehindSurface) {
  auto constexpr eye =
      Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr normal = Vector3{0.0f, 0.0f, -1.0f};
  auto constexpr light =
      PointLight{ColourRGB::White(), Point3{0.0f, 0.0f, 10.0f}};

  auto const [r, g, b] = Lighting(Material{}, light, Point3{}, eye, normal);

  ASSERT_FLOAT_EQ(r, 0.1f);
  ASSERT_FLOAT_EQ(g, 0.1f);
  ASSERT_FLOAT_EQ(b, 0.1f);
}
}  // namespace
}  // namespace rt
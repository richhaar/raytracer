// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/world/point_light.h"

namespace rt {
namespace {
TEST(PointLight, DefaultConstruction) {
  PointLight light;
  auto const [r, g, b] = light.colour;
  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 1.0f);

  auto const [x, y, z] = light.position;
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, 0.0f);
  ASSERT_FLOAT_EQ(z, 0.0f);
}
}  // namespace
}  // namespace rt
// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/colour_rgb.h"
#include "rt/math/point3.h"

namespace rt {
namespace {
TEST(ColourRGB, Construction) {
  auto const [r, g, b] = ColourRGB{0.6f, 0.5f, 0.2f};
  ASSERT_FLOAT_EQ(r, 0.6f);
  ASSERT_FLOAT_EQ(g, 0.5f);
  ASSERT_FLOAT_EQ(b, 0.2f);
}

TEST(ColourRGB, Addition) {
  auto const [r, g, b] =
      ColourRGB{0.6f, 0.5f, 0.2f} + ColourRGB{0.4f, 0.5f, 0.8f};
  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 1.0f);
}

TEST(ColourRGB, Subtraction) {
  auto const [r, g, b] =
      ColourRGB{0.6f, 0.5f, 0.2f} - ColourRGB{0.3f, 0.3f, 0.1f};
  ASSERT_FLOAT_EQ(r, 0.3f);
  ASSERT_FLOAT_EQ(g, 0.2f);
  ASSERT_FLOAT_EQ(b, 0.1f);
}

TEST(ColourRGB, MultiplyingByScalarLHS) {
  auto const [r, g, b] = 2.0f * ColourRGB{0.6f, 0.5f, 0.2f};
  ASSERT_FLOAT_EQ(r, 1.2f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 0.4f);
}

TEST(ColourRGB, MultiplyingByScalarRHS) {
  auto const [r, g, b] = ColourRGB{0.6f, 0.5f, 0.2f} * 2.0f;
  ASSERT_FLOAT_EQ(r, 1.2f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 0.4f);
}

TEST(ColourRGB, MultiplyingByColour) {
  auto const [r, g, b] = ColourRGB{1.0f, 1.0f, 1.0f} * ColourRGB{2.0f, 3.0f, 4.0f};
  ASSERT_FLOAT_EQ(r, 2.0f);
  ASSERT_FLOAT_EQ(g, 3.0f);
  ASSERT_FLOAT_EQ(b, 4.0f);
}

}  // namespace
}  // namespace rt

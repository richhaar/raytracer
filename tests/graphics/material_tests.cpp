// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/material.h"

namespace rt {
namespace {
TEST(Material, DefaultConstruction) {
  Material const material;
  ASSERT_TRUE(material.pattern);
  auto const [r, g, b] = material.pattern->ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.0f);
  ASSERT_FLOAT_EQ(b, 1.0f);

  ASSERT_FLOAT_EQ(material.ambient, 0.1f);
  ASSERT_FLOAT_EQ(material.diffuse, 0.9f);
  ASSERT_FLOAT_EQ(material.specular, 0.9f);
  ASSERT_FLOAT_EQ(material.shininess, 200.0f);
}
}  // namespace
}  // namespace rt
// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/pattern.h"
#include "rt/math/transforms.h"
#include "rt/world/sphere.h"

namespace rt {
namespace {
class TestPattern : public Pattern {
  [[nodiscard]] ColourRGB LocalColorAt(Point3 const& point) const override {
    return ColourRGB{point.x, point.y, point.z};
  }

 public:
  constexpr TestPattern() : Pattern() {}
};

TEST(Pattern, StripeConstantInY) {
  auto const pattern = StripePattern(ColourRGB::Black(), ColourRGB::White());

  for (int32_t i = 0; i < 10; ++i) {
    auto const [r, g, b] =
        pattern.ColourAt(Point3{0.0f, 1.0f * static_cast<float>(i), 0.0f});

    ASSERT_FLOAT_EQ(r, 0.0f);
    ASSERT_FLOAT_EQ(g, 0.0f);
    ASSERT_FLOAT_EQ(b, 0.0f);
  }
}

TEST(Pattern, StripeConstantInZ) {
  auto const pattern = StripePattern(ColourRGB::Black(), ColourRGB::White());

  for (int32_t i = 0; i < 10; ++i) {
    auto const [r, g, b] =
        pattern.ColourAt(Point3{0.0f, 0.0f, 0.0f * static_cast<float>(i)});

    ASSERT_FLOAT_EQ(r, 0.0f);
    ASSERT_FLOAT_EQ(g, 0.0f);
    ASSERT_FLOAT_EQ(b, 0.0f);
  }
}

TEST(Pattern, StripeAlternatesInX) {
  auto const pattern = StripePattern(ColourRGB::Black(), ColourRGB::White());

  for (int32_t i = 0; i < 10; ++i) {
    auto const [r, g, b] =
        pattern.ColourAt(Point3{2.0f * static_cast<float>(i), 0.0f, 0.0f});

    ASSERT_FLOAT_EQ(r, 0.0f);
    ASSERT_FLOAT_EQ(g, 0.0f);
    ASSERT_FLOAT_EQ(b, 0.0f);
  }

  for (int32_t i = -1; i < 10; ++i) {
    auto const [r, g, b] = pattern.ColourAt(
        Point3{1.0f + 2.0f * static_cast<float>(i), 0.0f, 0.0f});
    ASSERT_FLOAT_EQ(r, 1.0f);
    ASSERT_FLOAT_EQ(g, 1.0f);
    ASSERT_FLOAT_EQ(b, 1.0f);
  }
}

TEST(Pattern, SolidColour) {
  auto const pattern = SolidColour(ColourRGB{0.4f, 0.5f, 0.6f});
  auto const [r, g, b] = pattern.ColourAt(Point3{1.0f, 2.0f, 3.0f});
  ASSERT_FLOAT_EQ(r, 0.4f);
  ASSERT_FLOAT_EQ(g, 0.5f);
  ASSERT_FLOAT_EQ(b, 0.6f);
}

TEST(Pattern, ColourAtShapeObjectTransform) {
  auto const pattern = TestPattern();
  auto sphere = Sphere{};
  sphere.SetTransform(Scaling(2.0f, 2.0f, 2.0f));
  auto const [r, g, b] =
      pattern.ColourAtShape(sphere, Point3{2.0f, 3.0f, 4.0f});

  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.5f);
  ASSERT_FLOAT_EQ(b, 2.0f);
}

TEST(Pattern, ColourAtShapePatternTransform) {
  auto pattern = TestPattern();
  pattern.SetTransform(Scaling(2.0f, 2.0f, 2.0f));

  auto const [r, g, b] =
      pattern.ColourAtShape(Sphere{}, Point3{2.0f, 3.0f, 4.0f});

  ASSERT_FLOAT_EQ(r, 1.0f);
  ASSERT_FLOAT_EQ(g, 1.5f);
  ASSERT_FLOAT_EQ(b, 2.0f);
}

TEST(Pattern, ColourAtShapeBothTransforms) {
  auto pattern = TestPattern();
  pattern.SetTransform(Translation(0.5f, 1.0f, 1.5f));

  auto sphere = Sphere{};
  sphere.SetTransform(Scaling(2.0f, 2.0f, 2.0f));

  auto const [r, g, b] =
      pattern.ColourAtShape(sphere, Point3{2.5f, 3.0f, 3.5f});

  ASSERT_FLOAT_EQ(r, 0.75f);
  ASSERT_FLOAT_EQ(g, 0.5f);
  ASSERT_FLOAT_EQ(b, 0.25f);
}

TEST(Pattern, GradientStart) {
  auto const gradient = GradientPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_EQ(r, 1.0f);
  ASSERT_EQ(g, 1.0f);
  ASSERT_EQ(b, 1.0f);
}

TEST(Pattern, GradientThreeQuarters) {
  auto const gradient = GradientPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.75f, 0.0f, 0.0f});
  ASSERT_EQ(r, 0.25f);
  ASSERT_EQ(g, 0.25f);
  ASSERT_EQ(b, 0.25f);
}

TEST(Pattern, RingPattern) {
  auto const gradient = RingPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_EQ(r, 1.0f);
  ASSERT_EQ(g, 1.0f);
  ASSERT_EQ(b, 1.0f);

  auto const [r2, g2, b2] = gradient.ColourAt(Point3{1.0f, 0.0f, 0.0f});
  ASSERT_EQ(r2, 0.0f);
  ASSERT_EQ(g2, 0.0f);
  ASSERT_EQ(b2, 0.0f);

  auto const [r3, g3, b3] = gradient.ColourAt(Point3{0.0f, 0.0f, 1.0f});
  ASSERT_EQ(r3, 0.0f);
  ASSERT_EQ(g3, 0.0f);
  ASSERT_EQ(b3, 0.0f);
}

TEST(Pattern, CheckerPatternX) {
  auto const gradient = CheckerPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_EQ(r, 1.0f);
  ASSERT_EQ(g, 1.0f);
  ASSERT_EQ(b, 1.0f);

  auto const [r2, g2, b2] = gradient.ColourAt(Point3{0.99f, 0.0f, 0.0f});
  ASSERT_EQ(r2, 1.0f);
  ASSERT_EQ(g2, 1.0f);
  ASSERT_EQ(b2, 1.0f);

  auto const [r3, g3, b3] = gradient.ColourAt(Point3{1.01f, 0.0f, 0.0f});
  ASSERT_EQ(r3, 0.0f);
  ASSERT_EQ(g3, 0.0f);
  ASSERT_EQ(b3, 0.0f);
}

TEST(Pattern, CheckerPatternY) {
  auto const gradient = CheckerPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_EQ(r, 1.0f);
  ASSERT_EQ(g, 1.0f);
  ASSERT_EQ(b, 1.0f);

  auto const [r2, g2, b2] = gradient.ColourAt(Point3{0.0f, 0.99f, 0.0f});
  ASSERT_EQ(r2, 1.0f);
  ASSERT_EQ(g2, 1.0f);
  ASSERT_EQ(b2, 1.0f);

  auto const [r3, g3, b3] = gradient.ColourAt(Point3{0.0f, 1.01f, 0.0f});
  ASSERT_EQ(r3, 0.0f);
  ASSERT_EQ(g3, 0.0f);
  ASSERT_EQ(b3, 0.0f);
}

TEST(Pattern, CheckerPatternZ) {
  auto const gradient = CheckerPattern(ColourRGB::White(), ColourRGB::Black());
  auto const [r, g, b] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.0f});
  ASSERT_EQ(r, 1.0f);
  ASSERT_EQ(g, 1.0f);
  ASSERT_EQ(b, 1.0f);

  auto const [r2, g2, b2] = gradient.ColourAt(Point3{0.0f, 0.0f, 0.99f});
  ASSERT_EQ(r2, 1.0f);
  ASSERT_EQ(g2, 1.0f);
  ASSERT_EQ(b2, 1.0f);

  auto const [r3, g3, b3] = gradient.ColourAt(Point3{0.0f, 0.0f, 1.01f});
  ASSERT_EQ(r3, 0.0f);
  ASSERT_EQ(g3, 0.0f);
  ASSERT_EQ(b3, 0.0f);
}
}  // namespace
}  // namespace rt

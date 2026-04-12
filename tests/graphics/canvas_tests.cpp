// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/canvas.h"

namespace rt {
namespace {
TEST(Canvas, Construction) { auto const canvas = Canvas{10ULL, 20ULL}; }

TEST(Canvas, Read) {
  auto const canvas = Canvas{10ULL, 20ULL};
  auto const [r, g, b] = canvas.Read(9ULL, 19ULL);
  EXPECT_FLOAT_EQ(r, 0.0f);
  EXPECT_FLOAT_EQ(g, 0.0f);
  EXPECT_FLOAT_EQ(b, 0.0f);
}

TEST(Canvas, WriteAndRead) {
  auto canvas = Canvas{4ULL, 4ULL};
  canvas.Write(3ULL, 3ULL, ColourRGB{1.0f, 0.9f, 0.8f});

  auto const [r, g, b] = canvas.Read(3ULL, 3ULL);
  EXPECT_FLOAT_EQ(r, 1.0f);
  EXPECT_FLOAT_EQ(g, 0.9f);
  EXPECT_FLOAT_EQ(b, 0.8f);
}

TEST(Canvas, WidthAndHeight) {
  auto canvas = Canvas{4ULL, 5ULL};
  ASSERT_EQ(canvas.Width(), 4ULL);
  ASSERT_EQ(canvas.Height(), 5ULL);
}

TEST(Canvas, WriteOutOfBounds) {
  auto canvas = Canvas{4ULL, 4ULL};
  ASSERT_ANY_THROW(canvas.Write(4ULL, 0ULL, ColourRGB{1.0f, 0.0f, 0.0f}));
}

TEST(Canvas, ReadOutOfBounds) {
  auto const canvas = Canvas{4ULL, 4ULL};
  ASSERT_ANY_THROW((void)canvas.Read(4ULL, 0ULL));
}

TEST(Canvas, UncheckedAccessor) {
  auto canvas = Canvas{4ULL, 4ULL};
  canvas(0, 0) = ColourRGB{1.0f, 0.8f, 0.6f};
  ASSERT_FLOAT_EQ(canvas(0, 0).red, 1.0f);
  ASSERT_FLOAT_EQ(canvas(0, 0).green, 0.8f);
  ASSERT_FLOAT_EQ(canvas(0, 0).blue, 0.6f);
}

TEST(Canvas, UncheckedConstAccessor) {
  auto const canvas = Canvas{4ULL, 4ULL};
  ASSERT_FLOAT_EQ(canvas(0, 0).red, 0.0f);
}

}  // namespace
}  // namespace rt

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/colour_rgb.h"
#include "rt/graphics/ppm_writer.h"

namespace rt {
namespace {

TEST(PPMWriter, WriteCanvasFiveByThree) {
  Canvas c{5, 3};
  auto constexpr c1 = ColourRGB{1.5, 0, 0};
  auto constexpr c2 = ColourRGB{0.0f, 0.5f, 0.0f};
  auto constexpr c3 = ColourRGB{-0.5f, 0.0f, 1.0f};

  c.Write(0, 0, c1);
  c.Write(2, 1, c2);
  c.Write(4, 2, c3);

  auto const expected_string = R"(P3
5 3
255
255 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 128 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 255
)";

  std::stringstream ss;

  PPMWriter::Write(c, ss);
  ASSERT_EQ(ss.str(), expected_string);
  // PPMWriter::write(c, "image.ppm");
}

TEST(PPMWriter, WriteCanvasTenByTwo) {
  Canvas c{10, 2};

  auto constexpr colour = ColourRGB{1.0f, 0.8f, 0.6f};

  for (std::size_t y = 0; y < 2; ++y) {
    for (std::size_t x = 0; x < 10; ++x) {
      c.Write(x, y, colour);
    }
  }
  auto const expected_string = R"(P3
10 2
255
255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
153 255 204 153 255 204 153 255 204 153 255 204 153
255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
153 255 204 153 255 204 153 255 204 153 255 204 153
)";

  std::stringstream ss;

  PPMWriter::Write(c, ss);
  ASSERT_EQ(ss.str(), expected_string);
}
}  // namespace
}  // namespace rt
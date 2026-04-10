// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT Licence
#ifndef CANVAS_H
#define CANVAS_H
#include <vector>

#include "colour_rgb.h"

class Canvas {
  std::size_t width_ = 0;
  std::size_t height_ = 0;
  std::vector<ColourRGB> canvas_;

 public:
  Canvas(std::size_t const width, std::size_t const height)
      : width_(width),
        height_(height),
        canvas_(width_ * height_, ColourRGB{0.0f, 0.0f, 0.0f}) {}

  ColourRGB Read(std::size_t const x, std::size_t const y) const {
    if (x >= width_ || y >= height_) {
      throw std::out_of_range("Invalid index to Canvas::Read");
    }
    return canvas_[x + y * width_];
  }

  void Write(std::size_t const x, std::size_t const y, ColourRGB const& rgb) {
    if (x >= width_ || y >= height_) {
      throw std::out_of_range("Invalid index to Canvas::Write");
    }
    canvas_[x + y * width_] = rgb;
  }

  ColourRGB& operator()(std::size_t const x, std::size_t const y) {
    return canvas_[x + y * width_];
  }

  ColourRGB const& operator()(std::size_t const x, std::size_t const y) const {
    return canvas_[x + y * width_];
  }
};

#endif  // CANVAS_H

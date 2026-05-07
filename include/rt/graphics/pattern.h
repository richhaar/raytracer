// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_PATTERN_H
#define RAYTRACER_PATTERN_H
#include <cmath>
#include <iostream>

#include "rt/graphics/colour_rgb.h"
#include "rt/math/matrix.h"
#include "rt/math/point3.h"

namespace rt {
class Intersectable;

class Pattern {
  Matrix<4, 4> transform_{Matrix<4, 4>::Identity()};
  Matrix<4, 4> inverse_transform_{Matrix<4, 4>::Identity()};

  [[nodiscard]] virtual ColourRGB LocalColorAt(Point3 const& point) const = 0;

 public:
  virtual ~Pattern() = default;
  void SetTransform(Matrix<4, 4> const& m);

  [[nodiscard]] ColourRGB ColourAtShape(Intersectable const& shape,
                                        Point3 const& world_point) const;

  [[nodiscard]] ColourRGB ColourAt(Point3 const& pattern_point) const {
    return LocalColorAt(pattern_point);
  }

 protected:
  constexpr Pattern() = default;
};

class SolidColour : public Pattern {
  ColourRGB colour_;

  [[nodiscard]] ColourRGB LocalColorAt(Point3 const&) const override {
    return colour_;
  }

 public:
  explicit constexpr SolidColour(ColourRGB const& colour)
      : Pattern(), colour_(colour) {}
};

class StripePattern : public Pattern {
  ColourRGB colour1_;
  ColourRGB colour2_;

  [[nodiscard]] ColourRGB LocalColorAt(Point3 const& point) const override {
    return static_cast<int>(std::floor(point.x)) % 2 == 0 ? colour1_ : colour2_;
  }

 public:
  constexpr StripePattern(ColourRGB const& colour1, ColourRGB const& colour2)
      : Pattern(), colour1_(colour1), colour2_(colour2) {}
};

class GradientPattern : public Pattern {
  ColourRGB colour1_;
  ColourRGB colour2_;

  [[nodiscard]] ColourRGB LocalColorAt(Point3 const& point) const override {
    auto const distance = colour2_ - colour1_;
    auto const fraction = point.x - std::floor(point.x);
    return colour1_ + distance * fraction;
  }

 public:
  constexpr GradientPattern(ColourRGB const& colour1, ColourRGB const& colour2)
      : Pattern(), colour1_(colour1), colour2_(colour2) {}
};

class RingPattern : public Pattern {
  ColourRGB colour1_;
  ColourRGB colour2_;

  [[nodiscard]] ColourRGB LocalColorAt(Point3 const& point) const override {
    return static_cast<int>(
               std::floor(std::sqrt(point.x * point.x + point.z * point.z))) %
                       2 ==
                   0
               ? colour1_
               : colour2_;
  }

 public:
  constexpr RingPattern(ColourRGB const& colour1, ColourRGB const& colour2)
      : Pattern(), colour1_(colour1), colour2_(colour2) {}
};

class CheckerPattern : public Pattern {
  ColourRGB colour1_;
  ColourRGB colour2_;

  [[nodiscard]] ColourRGB LocalColorAt(Point3 const& point) const override {
    return static_cast<int>(std::floor(point.x) + std::floor(point.y) +
                            std::floor(point.z)) %
                       2 ==
                   0
               ? colour1_
               : colour2_;
  }

 public:
  constexpr CheckerPattern(ColourRGB const& colour1, ColourRGB const& colour2)
      : Pattern(), colour1_(colour1), colour2_(colour2) {}
};
}  // namespace rt

#endif  // RAYTRACER_PATTERN_H

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#ifndef COLOUR_RGB_H
#define COLOUR_RGB_H

namespace rt {
/**
 * @brief Represent an RGB colour
 */
struct ColourRGB {
  float red{};
  float green{};
  float blue{};

  constexpr ColourRGB() = default;
  constexpr ColourRGB(float const r, float const g, float const b)
      : red(r), green(g), blue(b) {}

  static constexpr ColourRGB White() { return {1.0f, 1.0f, 1.0f}; }
  static constexpr ColourRGB Black() { return {0.0f, 0.0f, 0.0f}; }
  static constexpr ColourRGB Red() { return {1.0f, 0.0f, 0.0f}; }
  static constexpr ColourRGB Green() { return {0.0f, 1.0f, 0.0f}; }
  static constexpr ColourRGB Blue() { return {0.0f, 0.0f, 1.0f}; }
  static constexpr ColourRGB Purple() { return {0.5f, 0.0f, 0.5f}; }
};

constexpr ColourRGB operator+(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue};
}

constexpr ColourRGB operator-(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue};
}

constexpr ColourRGB operator*(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue};
}

constexpr ColourRGB operator*(ColourRGB const& colour, float const scalar) {
  return {colour.red * scalar, colour.green * scalar, colour.blue * scalar};
}

constexpr ColourRGB operator*(float const scalar, ColourRGB const& colour) {
  return colour * scalar;
}
}  // namespace rt
#endif  // COLOUR_RGB_H

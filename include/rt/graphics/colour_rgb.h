// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#ifndef COLOUR_RGB_H
#define COLOUR_RGB_H

/**
 * @brief Represent an RGB colour
 */
struct ColourRGB {
  float red;
  float green;
  float blue;
};

inline ColourRGB operator+(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue};
}

inline ColourRGB operator-(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue};
}

inline ColourRGB operator*(ColourRGB const& lhs, ColourRGB const& rhs) {
  return {lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue};
}

inline ColourRGB operator*(ColourRGB const& colour, float const scalar) {
  return {colour.red * scalar, colour.green * scalar, colour.blue * scalar};
}

inline ColourRGB operator*(float const scalar, ColourRGB const& colour) {
  return colour * scalar;
}

#endif  // COLOUR_RGB_H

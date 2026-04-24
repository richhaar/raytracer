// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_POINT_LIGHT_H
#define RAYTRACER_POINT_LIGHT_H
#include "rt/graphics/colour_rgb.h"
#include "rt/math/point3.h"

namespace rt {
struct PointLight {
  ColourRGB colour;
  Point3 position;
};
}  // namespace rt
#endif  // RAYTRACER_POINT_LIGHT_H

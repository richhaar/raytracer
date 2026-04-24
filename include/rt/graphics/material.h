// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H
#include "colour_rgb.h"

namespace rt {
struct Material {
  ColourRGB colour = ColourRGB::White();
  float ambient = 0.1f;
  float diffuse = 0.9f;
  float specular = 0.9f;
  float shininess = 200.0f;
};
}  // namespace rt
#endif  // RAYTRACER_MATERIAL_H

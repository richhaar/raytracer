// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include "rt/graphics/pattern.h"

#include "rt/world/intersectable.h"

namespace rt {

void Pattern::SetTransform(Matrix<4, 4> const& m) {
  transform_ = m;
  inverse_transform_ = Inverse(m);
}
ColourRGB Pattern::ColourAtShape(Intersectable const& shape,
                                 Point3 const& world_point) const {
  auto const object_point = shape.GetInverseTransform() * world_point;
  auto const pattern_point = inverse_transform_ * object_point;
  return LocalColorAt(pattern_point);
}
}  // namespace rt
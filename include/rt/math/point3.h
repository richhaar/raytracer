// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef POINT3_H
#define POINT3_H

#include "rt/math/vector3.h"

namespace rt {

/**
 * @brief Represent a 3D point in space
 */
struct Point3 {
  float x;
  float y;
  float z;
};

inline Point3 operator+(Point3 const& lhs, Vector3 const& rhs) {
  return Point3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline Point3 operator-(Point3 const& lhs, Vector3 const& rhs) {
  return Point3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline Vector3 operator-(Point3 const& lhs, Point3 const& rhs) {
  return Vector3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

}  // namespace rt
#endif  // POINT3_H
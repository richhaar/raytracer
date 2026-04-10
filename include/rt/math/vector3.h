// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT Licence
#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
namespace rt {

/**
 * @brief Represent a 3-Dimensional vector
 */
struct Vector3 {
  float x;
  float y;
  float z;
};

inline Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs) {
  return Vector3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs) {
  return Vector3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline Vector3 operator-(Vector3 const& vec) {
  return Vector3{-vec.x, -vec.y, -vec.z};
}

inline Vector3 operator*(Vector3 const& lhs, float const scalar) {
  return Vector3{lhs.x * scalar, lhs.y * scalar, lhs.z * scalar};
}

inline Vector3 operator*(float const scalar, Vector3 const& rhs) {
  return rhs * scalar;
}

inline float Length(Vector3 const& vec) {
  return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline Vector3 Normalize(Vector3 const& vec) {
  auto const length = Length(vec);
  return Vector3{vec.x / length, vec.y / length, vec.z / length};
}

inline bool Near(Vector3 const& vec1, Vector3 const& vec2,
                 float const epsilon = 1e-6f) {
  return Length(vec1 - vec2) <= epsilon;
}

}  // namespace rt
#endif  // VECTOR3_H
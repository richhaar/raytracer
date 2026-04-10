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

/**
 * @brief Calculate the L2 or Euclidean norm of a vector
 * @param vec Input vector
 * @return The Euclidean distance of the vector
 */
inline float Length(Vector3 const& vec) {
  return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline Vector3 Normalize(Vector3 const& vec) {
  auto const length = Length(vec);
  return Vector3{vec.x / length, vec.y / length, vec.z / length};
}

/**
 * @brief Calculate the dot product of two vectors.
 * @param vec1 First vector
 * @param vec2 Second vector
 * @return The dot product of vec1 and vec2.
 */
inline float Dot(Vector3 const& vec1, Vector3 const& vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

/**
 * @brief Calculate the cross product of two vectors
 * @param vec1 First vector
 * @param vec2 Second vector
 * @return A third vector perpendicular to the input two vectors
 */
inline Vector3 Cross(Vector3 const& vec1, Vector3 const& vec2) {
  return Vector3{vec1.y * vec2.z - vec1.z * vec2.y,
                 vec1.z * vec2.x - vec1.x * vec2.z,
                 vec1.x * vec2.y - vec1.y * vec2.x};
}

/**
 * @brief Check if two vectors are considered nearly equal.
 * @param vec1 First vector to compare
 * @param vec2 Second vector to compare
 * @param epsilon Distance needed to be considered equal, default 1e-6f
 * @return
 */
inline bool Near(Vector3 const& vec1, Vector3 const& vec2,
                 float const epsilon = 1e-6f) {
  auto const diff = vec1 - vec2;
  return Dot(diff, diff) <= epsilon * epsilon;
}

}  // namespace rt
#endif  // VECTOR3_H
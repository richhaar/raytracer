// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H
#include <cstdint>

#include "ray.h"
#include "rt/math/matrix.h"

namespace rt {

struct Camera {
  uint64_t hsize;
  uint64_t vsize;
  float fov;
  Matrix<4, 4> transform;
  Matrix<4, 4> inv_transform;

  float half_width;
  float half_height;
  float pixel_size;

  static constexpr Camera Create(
      uint64_t const hsize, uint64_t const vsize, float const fov,
      Matrix<4, 4> const& transform = Matrix<4, 4>::Identity()) {
    auto const half_view = std::tan(fov / 2.0f);
    auto const aspect = static_cast<float>(hsize) / static_cast<float>(vsize);

    auto const half_width = aspect >= 1.0f ? half_view : half_view * aspect;
    auto const half_height = aspect >= 1.0f ? half_view / aspect : half_view;
    auto const pixel_size = (half_width * 2.0f) / static_cast<float>(hsize);

    return Camera{
        .hsize = hsize,
        .vsize = vsize,
        .fov = fov,
        .transform = transform,
        .inv_transform = Inverse(transform),
        .half_width = half_width,
        .half_height = half_height,
        .pixel_size = pixel_size,
    };
  }
};

inline Ray RayForPixel(Camera const& cam, float const x, float const y) {
  auto const x_offset = x * cam.pixel_size;
  auto const y_offset = y * cam.pixel_size;

  auto const world_x = cam.half_width - x_offset;
  auto const world_y = cam.half_height - y_offset;

  auto const inv = cam.inv_transform;
  auto const pixel = inv * Point3{world_x, world_y, -1.0f};
  auto const origin = inv * Point3{0.0f, 0.0f, 0.0f};
  auto const direction = Normalize(pixel - origin);

  return Ray{origin, direction};
}
}  // namespace rt

#endif  // RAYTRACER_CAMERA_H

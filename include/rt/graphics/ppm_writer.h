// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#ifndef RAYTRACER_PPM_WRITER_H
#define RAYTRACER_PPM_WRITER_H
#include <filesystem>
#include <iosfwd>

#include "canvas.h"

namespace rt {
class PPMWriter {
 public:
  static void Write(Canvas const& canvas, std::ostream& out);
  static void Write(Canvas const& canvas, std::filesystem::path const& path);
};
}  // namespace rt
#endif  // RAYTRACER_PPM_WRITER_H

// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include "rt/graphics/ppm_writer.h"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>

#include "rt/graphics/canvas.h"

namespace rt {

class PPMLineWriter {
  std::ostream& out_;
  std::string line_;
  std::size_t const kMaxPPMLineLength_ = 70;
  char buffer[16]{};

  void Flush() {
    if (line_.empty()) {
    }
    out_ << line_ << '\n';
    line_.clear();
  }

 public:
  explicit PPMLineWriter(std::ostream& out) : out_(out) {
    line_.reserve(kMaxPPMLineLength_);
  }

  void Add(int32_t const value) {
    const auto [ptr, ec] =
        std::to_chars(buffer, buffer + sizeof(buffer), value);
    if (ec != std::errc{}) {
      throw std::runtime_error("failed to format PPM value");
    }

    auto const text =
        std::string_view(buffer, static_cast<std::size_t>(ptr - buffer));

    if (line_.empty()) {
      line_.append(text);
      return;
    }

    if (line_.size() + 1 + text.size() > kMaxPPMLineLength_) {
      Flush();
      line_.append(text);
      return;
    }

    line_.push_back(' ');
    line_.append(text);
  }

  void EndRow() { Flush(); }
};

int32_t ColourChannelToPPM(float const colour) {
  return static_cast<int32_t>(
      std::clamp(std::lround(colour * 255.0f), 0L, 255L));
}
void PPMWriter::Write(Canvas const& canvas, std::ostream& out) {
  auto const width = canvas.Width();
  auto const height = canvas.Height();

  out << "P3\n" << width << " " << height << "\n255\n";

  PPMLineWriter writer(out);

  for (std::size_t y = 0ULL; y < height; ++y) {
    for (std::size_t x = 0ULL; x < width; ++x) {
      const auto& [red, green, blue] = canvas(x, y);
      writer.Add(ColourChannelToPPM(red));
      writer.Add(ColourChannelToPPM(green));
      writer.Add(ColourChannelToPPM(blue));
    }
    writer.EndRow();
  }
}

void PPMWriter::Write(Canvas const& canvas, std::filesystem::path const& path) {
  std::ofstream file(path, std::ios::out);
  if (!file) {
    throw std::runtime_error("failed to open output file");
  }
  Write(canvas, file);
}
}  // namespace rt
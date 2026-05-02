// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License
#include <gtest/gtest.h>

#include "rt/graphics/render.h"
#include "rt/math/view_transform.h"
#include "rt/world/camera.h"
#include "rt/world/world_builder.h"

namespace rt {
namespace {

TEST(Render, RenderWorld) {
  auto const world = DefaultWorld();

  auto constexpr from = Point3{0.0f, 0.0f, -5.0f};
  auto constexpr to = Point3{0.0f, 0.0f, 0.0f};
  auto constexpr up = Vector3{0.0f, 1.0f, 0.0f};
  auto const cam = Camera::Create(11, 11, std::numbers::pi_v<float> / 2.0f,
                                  ViewTransform(from, to, up));

  auto const img = RenderWorld(cam, world);

  auto const [r, g, b] = img.Read(5, 5);

  ASSERT_NEAR(r, 0.38066f, 1e-5f);
  ASSERT_NEAR(g, 0.47583f, 1e-5f);
  ASSERT_NEAR(b, 0.2855f, 1e-5f);
}
}  // namespace
}  // namespace rt
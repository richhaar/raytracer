// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>

#include "rt/world/world.h"
#include "rt/world/world_builder.h"

namespace rt {
namespace {

TEST(World, IntersectWorld) {
  auto const world = DefaultWorld();
  auto constexpr ray =
      Ray(Point3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 1.0f));

  auto const hits = IntersectWorld(world, ray);

  ASSERT_EQ(hits.size(), 4);
  auto iter = hits.begin();
  ASSERT_FLOAT_EQ(iter->t, 4.0f);
  ++iter;
  ASSERT_FLOAT_EQ(iter->t, 4.5f);
  ++iter;
  ASSERT_FLOAT_EQ(iter->t, 5.5f);
  ++iter;
  ASSERT_FLOAT_EQ(iter->t, 6.0f);
}
}  // namespace
}  // namespace rt
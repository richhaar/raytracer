// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include <gtest/gtest.h>
#include "rt/world/sphere.h"
namespace rt {
namespace {
TEST(Sphere, Uuid) {
  ASSERT_NE(Sphere{}, Sphere{});
}
}  // namespace
}  // namespace rt
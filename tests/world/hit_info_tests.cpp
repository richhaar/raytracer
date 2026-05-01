// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include "gtest/gtest.h"
#include "rt/world/hit_info.h"
namespace rt {
namespace {

TEST(HitInfo, CalculateHitInfo) {
  auto constexpr ray = Ray(Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};

  auto const record = HitRecord{4.0f, &sphere};

  const auto [hit, inside, point, eye, normal] = CalculateHitInfo(record, ray);

  ASSERT_EQ(hit.object, record.object);
  ASSERT_EQ(hit.t, record.t);

  ASSERT_FALSE(inside);
  ASSERT_TRUE(Near(point, Point3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(normal, Vector3{0.0f, 0.0f, -1.0f}));
}

TEST(HitInfo, CalculateHitInfoInsideRay) {
  auto constexpr ray = Ray(Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};
  auto const record = HitRecord{1.0f, &sphere};

  const auto [_, inside, point, eye, normal] = CalculateHitInfo(record, ray);

  ASSERT_TRUE(inside);
  ASSERT_TRUE(Near(point, Point3{0.0f, 0.0f, 1.0f}));
  ASSERT_TRUE(Near(eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(normal, Vector3{0.0f, 0.0f, -1.0f}));

}
}  // namespace
}  // namespace rt
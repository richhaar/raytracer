// Copyright (c) 2026 Richard Haar <rh@richhaar.com>
// Licensed under the MIT License

#include "gtest/gtest.h"
#include "rt/world/plane.h"
#include "rt/world/surface_interaction.h"
#include "rt/world/world_builder.h"
namespace rt {
namespace {

TEST(SurfaceInteraction, ComputeSurfaceInteraction) {
  auto constexpr ray =
      Ray(Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};

  auto const record = Intersection{4.0f, &sphere};

  auto const surface = ComputeSurfaceInteraction(record, ray);

  ASSERT_EQ(surface.intersection.object, record.object);
  ASSERT_EQ(surface.intersection.t, record.t);

  ASSERT_FALSE(surface.inside);
  ASSERT_TRUE(Near(surface.point, Point3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(surface.eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(surface.normal, Vector3{0.0f, 0.0f, -1.0f}));
}

TEST(SurfaceInteraction, ComputeSurfaceInteractionInsideRay) {
  auto constexpr ray = Ray(Point3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 1.0f});
  auto const sphere = Sphere{};
  auto const record = Intersection{1.0f, &sphere};

  auto const surface = ComputeSurfaceInteraction(record, ray);

  ASSERT_TRUE(surface.inside);
  ASSERT_TRUE(Near(surface.point, Point3{0.0f, 0.0f, 1.0f}));
  ASSERT_TRUE(Near(surface.eye, Vector3{0.0f, 0.0f, -1.0f}));
  ASSERT_TRUE(Near(surface.normal, Vector3{0.0f, 0.0f, -1.0f}));
}

TEST(SurfaceInteraction, ReflectVector) {
  auto const plane = Plane{};
  auto const ray =
      Ray{Point3{0.0f, 0.0f, -1.0f},
          Vector3{0.0f, -std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f}};

  auto const intersection = Intersection(std::sqrt(2.0f), &plane);
  auto const surface = ComputeSurfaceInteraction(intersection, ray);

  auto const [x, y, z] = surface.reflect;
  ASSERT_FLOAT_EQ(x, 0.0f);
  ASSERT_FLOAT_EQ(y, std::sqrt(2.0f) / 2.0f);
  ASSERT_FLOAT_EQ(z, std::sqrt(2.0f) / 2.0f);
}

TEST(SurfaceInteraction, OverPoint) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto sphere = Sphere{};
  sphere.SetTransform(Translation(0.0f, 0.0f, 1.0f));

  auto const intersection = Intersection(5.0f, &sphere);
  auto const surface = ComputeSurfaceInteraction(intersection, ray);

  EXPECT_LT(surface.over_point.z, -kRaySurfaceOffset / 2.0f);
  EXPECT_GT(surface.point.z, surface.over_point.z);
}

TEST(SurfaceInteraction, UnderPoint) {
  auto constexpr ray =
      Ray{Point3{0.0f, 0.0f, -5.0f}, Vector3{0.0f, 0.0f, 1.0f}};

  auto sphere = Sphere{};
  sphere.SetTransform(Translation(0.0f, 0.0f, 1.0f));

  auto const intersection = Intersection(5.0f, &sphere);
  auto const surface = ComputeSurfaceInteraction(intersection, ray);

  ASSERT_GT(surface.under_point.z, kRaySurfaceOffset / 2.0f);
  ASSERT_LT(surface.point.z, surface.under_point.z);
}

class SurfaceInteractionRefractionTest : public ::testing::Test {
 protected:
  World world_;
  Ray ray_;
  std::vector<Intersection> intersections_;

  void SetUp() override {
    WorldBuilder builder;
    world_ =
        builder
            .AddSphere(Scaling(2.0f, 2.0f, 2.0f),
                       Material{.transparency = 1.0f, .refractive_index = 1.5f})
            .AddSphere(Translation(0.0f, 0.0f, -0.25f),
                       Material{.transparency = 1.0f, .refractive_index = 2.0f})
            .AddSphere(Translation(0.0f, 0.0f, 0.25f),
                       Material{.transparency = 1.0f, .refractive_index = 2.5f})
            .Build();

    ray_ = Ray{Point3{0.0f, 0.0f, -4.0f}, Vector3{0.0f, 0.0f, 1.0f}};

    intersections_ = {
        {2.0f, world_.objects_[0].get()},  {2.75f, world_.objects_[1].get()},
        {3.25f, world_.objects_[2].get()}, {4.75f, world_.objects_[1].get()},
        {5.25f, world_.objects_[2].get()}, {6.0f, world_.objects_[0].get()},
    };
  }
};

TEST_F(SurfaceInteractionRefractionTest, IntersectAt2) {
  auto const intersection = Intersection(2.0f, world_.objects_[0].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 1.0f);
  EXPECT_FLOAT_EQ(surface.n2, 1.5f);
}

TEST_F(SurfaceInteractionRefractionTest, IntersectAt2point75) {
  auto const intersection = Intersection(2.75f, world_.objects_[1].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 1.5f);
  EXPECT_FLOAT_EQ(surface.n2, 2.0f);
}

TEST_F(SurfaceInteractionRefractionTest, IntersectAt3point25) {
  auto const intersection = Intersection(3.25, world_.objects_[2].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 2.0f);
  EXPECT_FLOAT_EQ(surface.n2, 2.5f);
}

TEST_F(SurfaceInteractionRefractionTest, IntersectAt4point75) {
  auto const intersection = Intersection(4.75, world_.objects_[1].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 2.5f);
  EXPECT_FLOAT_EQ(surface.n2, 2.5f);
}

TEST_F(SurfaceInteractionRefractionTest, IntersectAt5point25) {
  auto const intersection = Intersection(5.25f, world_.objects_[2].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 2.5f);
  EXPECT_FLOAT_EQ(surface.n2, 1.5f);
}

TEST_F(SurfaceInteractionRefractionTest, IntersectAt6) {
  auto const intersection = Intersection(6.0f, world_.objects_[0].get());
  auto const surface =
      ComputeSurfaceInteraction(intersection, ray_, intersections_);

  EXPECT_FLOAT_EQ(surface.n1, 1.5f);
  EXPECT_FLOAT_EQ(surface.n2, 1.0f);
}

}  // namespace
}  // namespace rt
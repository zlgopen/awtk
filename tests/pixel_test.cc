#include "base/pixel.h"
#include "base/color.h"
#include "gtest/gtest.h"

TEST(Pixel, rgb565) {
  pixel_rgb565_t p = pixel_rgb565_from_rgb(1 << 3, 2 << 2, 3 << 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_rgb565_BPP);

  rgba_t rgba = pixel_rgb565_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r << 3);
  ASSERT_EQ(rgba.g, p.g << 2);
  ASSERT_EQ(rgba.b, p.b << 3);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, rgb888) {
  pixel_rgb888_t p = pixel_rgb888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_rgb888_BPP);

  rgba_t rgba = pixel_rgb888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, bgr888) {
  pixel_bgr888_t p = pixel_bgr888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_bgr888_BPP);

  rgba_t rgba = pixel_bgr888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, rgba8888) {
  pixel_rgba8888_t p = pixel_rgba8888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_rgba8888_BPP);

  rgba_t rgba = pixel_rgba8888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, abgr8888) {
  pixel_abgr8888_t p = pixel_abgr8888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_abgr8888_BPP);

  rgba_t rgba = pixel_abgr8888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, bgra8888) {
  pixel_bgra8888_t p = pixel_bgra8888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_bgra8888_BPP);

  rgba_t rgba = pixel_bgra8888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, argb8888) {
  pixel_argb8888_t p = pixel_argb8888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), pixel_argb8888_BPP);

  rgba_t rgba = pixel_argb8888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

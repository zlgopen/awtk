#include "base/pixel.h"
#include "tkc/color.h"
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

TEST(Pixel, rgb565_dark0) {
  pixel_rgb565_t p = pixel_rgb565_from_rgb(2 << 3, 4 << 2, 6 << 3);
  pixel_rgb565_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, rgb888_dark0) {
  pixel_rgb888_t p = pixel_rgb888_from_rgb(100, 128, 168);
  pixel_rgb888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, rgba8888_dark0) {
  pixel_rgba8888_t p = pixel_rgba8888_from_rgb(100, 128, 168);
  pixel_rgba8888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, abgr8888_dark0) {
  pixel_abgr8888_t p = pixel_abgr8888_from_rgb(100, 128, 168);
  pixel_abgr8888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, bgra8888_dark0) {
  pixel_bgra8888_t p = pixel_bgra8888_from_rgb(100, 128, 168);
  pixel_bgra8888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, argb8888_dark0) {
  pixel_argb8888_t p = pixel_argb8888_from_rgb(100, 128, 168);
  pixel_argb8888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, bgr565_dark0) {
  pixel_bgr565_t p = pixel_bgr565_from_rgb(2 << 3, 4 << 2, 6 << 3);
  pixel_bgr565_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, bgr888_dark0) {
  pixel_bgr888_t p = pixel_bgr888_from_rgb(100, 128, 168);
  pixel_bgr888_blend_rgba_dark(&p, 0);

  ASSERT_EQ(p.r, 0);
  ASSERT_EQ(p.g, 0);
  ASSERT_EQ(p.b, 0);
}

TEST(Pixel, rgb565_dark255) {
  pixel_rgb565_t p = pixel_rgb565_from_rgb(20 << 3, 18 << 2, 16 << 3);
  pixel_rgb565_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 19);
  ASSERT_EQ(p.g, 17);
  ASSERT_EQ(p.b, 15);
}

TEST(Pixel, rgb888_dark255) {
  pixel_rgb888_t p = pixel_rgb888_from_rgb(100, 128, 168);
  pixel_rgb888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 99);
  ASSERT_EQ(p.g, 127);
  ASSERT_EQ(p.b, 167);
}

TEST(Pixel, rgba8888_dark255) {
  pixel_rgba8888_t p = pixel_rgba8888_from_rgb(100, 128, 168);
  pixel_rgba8888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 99);
  ASSERT_EQ(p.g, 127);
  ASSERT_EQ(p.b, 167);
}

TEST(Pixel, abgr8888_dark255) {
  pixel_abgr8888_t p = pixel_abgr8888_from_rgb(100, 128, 168);
  pixel_abgr8888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 99);
  ASSERT_EQ(p.g, 127);
  ASSERT_EQ(p.b, 167);
}

TEST(Pixel, bgra8888_dark255) {
  pixel_bgra8888_t p = pixel_bgra8888_from_rgb(100, 128, 168);
  pixel_bgra8888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 99);
  ASSERT_EQ(p.g, 127);
  ASSERT_EQ(p.b, 167);
}

TEST(Pixel, argb8888_dark255) {
  pixel_argb8888_t p = pixel_argb8888_from_rgb(100, 128, 168);
  pixel_argb8888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 99);
  ASSERT_EQ(p.g, 127);
  ASSERT_EQ(p.b, 167);
}

TEST(Pixel, bgr565_dark255) {
  pixel_bgr565_t p = pixel_bgr565_from_rgb(20 << 3, 18 << 2, 16 << 3);
  pixel_bgr565_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 19);
  ASSERT_EQ(p.g, 17);
  ASSERT_EQ(p.b, 15);
}

TEST(Pixel, bgr888_dark255) {
  pixel_bgr888_t p = pixel_bgr888_from_rgb(20, 18, 16);
  pixel_bgr888_blend_rgba_dark(&p, 255);

  ASSERT_EQ(p.r, 19);
  ASSERT_EQ(p.g, 17);
  ASSERT_EQ(p.b, 15);
}

TEST(Pixel, rgb565_dark128) {
  pixel_rgb565_t p = pixel_rgb565_from_rgb(2 << 3, 4 << 2, 6 << 3);
  pixel_rgb565_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, rgb888_dark128) {
  pixel_rgb888_t p = pixel_rgb888_from_rgb(2, 4, 6);
  pixel_rgb888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, rgba8888_dark128) {
  pixel_rgba8888_t p = pixel_rgba8888_from_rgb(2, 4, 6);
  pixel_rgba8888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, abgr8888_dark128) {
  pixel_abgr8888_t p = pixel_abgr8888_from_rgb(2, 4, 6);
  pixel_abgr8888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, bgra8888_dark128) {
  pixel_bgra8888_t p = pixel_bgra8888_from_rgb(2, 4, 6);
  pixel_bgra8888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, argb8888_dark128) {
  pixel_argb8888_t p = pixel_argb8888_from_rgb(2, 4, 6);
  pixel_argb8888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, bgr565_dark128) {
  pixel_bgr565_t p = pixel_bgr565_from_rgb(2 << 3, 4 << 2, 6 << 3);
  pixel_bgr565_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, bgr888_dark128) {
  pixel_bgr888_t p = pixel_bgr888_from_rgb(2, 4, 6);
  pixel_bgr888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
}

TEST(Pixel, rgb565_premult0) {
  color_t c = color_init(80, 88, 96, 0);
  pixel_rgb565_t p = pixel_rgb565_from_rgb(80, 88, 96);
  pixel_rgb565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 10);
  ASSERT_EQ(p.g, 22);
  ASSERT_EQ(p.b, 12);
}

TEST(Pixel, rgb888_premult0) {
  color_t c = color_init(80, 88, 96, 0);
  pixel_rgb888_t p = pixel_rgb888_from_rgb(80, 88, 96);
  pixel_rgb888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 80);
  ASSERT_EQ(p.g, 88);
  ASSERT_EQ(p.b, 96);
}

TEST(Pixel, bgr565_premult0) {
  color_t c = color_init(80, 88, 96, 0);
  pixel_bgr565_t p = pixel_bgr565_from_rgb(80, 88, 96);
  pixel_bgr565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 10);
  ASSERT_EQ(p.g, 22);
  ASSERT_EQ(p.b, 12);
}

TEST(Pixel, bgr888_premult0) {
  color_t c = color_init(80, 88, 96, 0);
  pixel_bgr888_t p = pixel_bgr888_from_rgb(80, 88, 96);
  pixel_bgr888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 80);
  ASSERT_EQ(p.g, 88);
  ASSERT_EQ(p.b, 96);
}

TEST(Pixel, rgb565_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_rgb565_t p = pixel_rgb565_from_rgb(100, 200, 160);
  pixel_rgb565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 12);
  ASSERT_EQ(p.g, 40);
  ASSERT_EQ(p.b, 13);
}

TEST(Pixel, rgb888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_rgb888_t p = pixel_rgb888_from_rgb(100, 200, 160);
  pixel_rgb888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
}

TEST(Pixel, bgr565_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_bgr565_t p = pixel_bgr565_from_rgb(100, 200, 160);
  pixel_bgr565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 12);
  ASSERT_EQ(p.g, 40);
  ASSERT_EQ(p.b, 13);
}

TEST(Pixel, bgr888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_bgr888_t p = pixel_bgr888_from_rgb(100, 200, 160);
  pixel_bgr888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
}

TEST(Pixel, rgb565_premulti255) {
  color_t c = color_init(0, 0, 0, 255);
  pixel_rgb565_t p = pixel_rgb565_from_rgb(48, 56, 72);
  pixel_rgb565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 5);
  ASSERT_EQ(p.g, 13);
  ASSERT_EQ(p.b, 8);
}

TEST(Pixel, rgb888_premulti255) {
  color_t c = color_init(0, 0, 0, 255);
  pixel_rgb888_t p = pixel_rgb888_from_rgb(48, 56, 72);
  pixel_rgb888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 47);
  ASSERT_EQ(p.g, 55);
  ASSERT_EQ(p.b, 71);
}

TEST(Pixel, bgr565_premulti255) {
  color_t c = color_init(0, 0, 0, 255);
  pixel_bgr565_t p = pixel_bgr565_from_rgb(48, 56, 72);
  pixel_bgr565_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 5);
  ASSERT_EQ(p.g, 13);
  ASSERT_EQ(p.b, 8);
}

TEST(Pixel, bgr888_premulti255) {
  color_t c = color_init(0, 0, 0, 255);
  pixel_bgr888_t p = pixel_bgr888_from_rgb(48, 56, 72);
  pixel_bgr888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 47);
  ASSERT_EQ(p.g, 55);
  ASSERT_EQ(p.b, 71);
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

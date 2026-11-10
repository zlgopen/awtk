#include "base/pixel.h"
#include "tkc/color.h"
#include "gtest/gtest.h"

TEST(Pixel, rgb565) {
  pixel_rgb565_t p = pixel_rgb565_from_rgb(1 << 3, 2 << 2, 3 << 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_rgb565_BPP);

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

TEST(Pixel, rgba8888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_rgba8888_t p = pixel_rgba8888_from_rgb(100, 200, 160);
  pixel_rgba8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, abgr8888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_abgr8888_t p = pixel_abgr8888_from_rgb(100, 200, 160);
  pixel_abgr8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, bgra8888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_bgra8888_t p = pixel_bgra8888_from_rgb(100, 200, 160);
  pixel_bgra8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, argb8888_premulti128) {
  color_t c = color_init(50, 60, 30, 128);
  pixel_argb8888_t p = pixel_argb8888_from_rgb(100, 200, 160);
  pixel_argb8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 100);
  ASSERT_EQ(p.g, 160);
  ASSERT_EQ(p.b, 110);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, rgba8888_dark_translucent) {
  /* 目标像素 alpha 小于 PIXEL_ALPHA_COMPOSITE_OPAQUE，走精确合成(归一化 + 除法) */
  pixel_rgba8888_t p = {20, 30, 40, 0x40};
  pixel_rgba8888_blend_rgba_dark(&p, 128);

  ASSERT_EQ(p.r, 12);
  ASSERT_EQ(p.g, 18);
  ASSERT_EQ(p.b, 25);
  ASSERT_EQ(p.a, 160);
}

TEST(Pixel, rgba8888_premulti_translucent) {
  /* 目标像素 alpha 小于 PIXEL_ALPHA_COMPOSITE_OPAQUE，走精确合成(归一化 + 除法) */
  color_t c = color_init(50, 60, 30, 128);
  pixel_rgba8888_t p = {20, 30, 40, 0x40};
  pixel_rgba8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 84);
  ASSERT_EQ(p.g, 102);
  ASSERT_EQ(p.b, 56);
  ASSERT_EQ(p.a, 160);
}

/* 预乘源色叠加：目标全透明且源通道饱和到 alpha 时，结果必须饱和到 255（不能回绕成 0） */
TEST(Pixel, rgba8888_premulti_saturate) {
  color_t c = color_init(255, 255, 255, 0);
  pixel_rgba8888_t p = pixel_rgba8888_from_rgba(20, 30, 40, 0x00);
  pixel_rgba8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 255);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 0xff);
}

/* 只有饱和的通道饱和到 255，其余通道保持归一化结果 */
TEST(Pixel, rgba8888_premulti_saturate_partial) {
  color_t c = color_init(128, 64, 128, 127);
  pixel_rgba8888_t p = pixel_rgba8888_from_rgba(20, 30, 40, 0x00);
  pixel_rgba8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 128);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 128);
}

/* 目标半透明、合成 alpha 饱和到 255 时同样不能回绕（out_a 由 256 饱和到 255 会把商顶到 256） */
TEST(Pixel, rgba8888_premulti_saturate_opaque_src) {
  color_t c = color_init(255, 255, 255, 0);
  pixel_rgba8888_t p = pixel_rgba8888_from_rgba(120, 60, 0, 0x40);
  pixel_rgba8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 255);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 0xff);
}

/* abgr/bgra/argb 复用同一套 rgba_blend_premulti，同样必须饱和而不是回绕 */
TEST(Pixel, abgr8888_premulti_saturate) {
  color_t c = color_init(255, 255, 255, 0);
  pixel_abgr8888_t p = pixel_abgr8888_from_rgba(20, 30, 40, 0x00);
  pixel_abgr8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 255);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, bgra8888_premulti_saturate) {
  color_t c = color_init(255, 255, 255, 0);
  pixel_bgra8888_t p = pixel_bgra8888_from_rgba(20, 30, 40, 0x00);
  pixel_bgra8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 255);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, argb8888_premulti_saturate) {
  color_t c = color_init(255, 255, 255, 0);
  pixel_argb8888_t p = pixel_argb8888_from_rgba(20, 30, 40, 0x00);
  pixel_argb8888_blend_rgba_premulti(&p, c.rgba);

  ASSERT_EQ(p.r, 255);
  ASSERT_EQ(p.g, 255);
  ASSERT_EQ(p.b, 255);
  ASSERT_EQ(p.a, 0xff);
}

TEST(Pixel, rgb888) {
  pixel_rgb888_t p = pixel_rgb888_from_rgb(1, 2, 3);
  ASSERT_EQ(p.r, 1);
  ASSERT_EQ(p.g, 2);
  ASSERT_EQ(p.b, 3);
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_rgb888_BPP);

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
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_bgr888_BPP);

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
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_rgba8888_BPP);

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
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_abgr8888_BPP);

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
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_bgra8888_BPP);

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
  ASSERT_EQ(sizeof(p), (uint32_t)pixel_argb8888_BPP);

  rgba_t rgba = pixel_argb8888_to_rgba(p);
  ASSERT_EQ(rgba.r, p.r);
  ASSERT_EQ(rgba.g, p.g);
  ASSERT_EQ(rgba.b, p.b);
  ASSERT_EQ(rgba.a, 0xff);
}

TEST(Pixel, limit_uint8) {
  ASSERT_EQ(tk_pixel_limit_uint8(0), 0);
  ASSERT_EQ(tk_pixel_limit_uint8(1), 1);
  ASSERT_EQ(tk_pixel_limit_uint8(0x7f), 0x7f);
  ASSERT_EQ(tk_pixel_limit_uint8(0xfe), 0xfe);
  ASSERT_EQ(tk_pixel_limit_uint8(0xff), 0xff);

  /* min */
  ASSERT_EQ(tk_pixel_limit_uint8(-1), 0);
  ASSERT_EQ(tk_pixel_limit_uint8(-0xff), 0);
  ASSERT_EQ(tk_pixel_limit_uint8(INT32_MIN), 0);

  /* max */
  ASSERT_EQ(tk_pixel_limit_uint8(0x100), 0xff);
  ASSERT_EQ(tk_pixel_limit_uint8(0x1234), 0xff);
  ASSERT_EQ(tk_pixel_limit_uint8(INT32_MAX), 0xff);

  /* 两个半透明像素合成时，alpha 之和可能大于 0xff，必须饱和为 0xff，不能直接截断成 0 */
  int32_t src_a = 0xf8;
  int32_t dst_a = 0xf4;
  int32_t sum = src_a + dst_a - ((src_a * dst_a) >> 8);

  ASSERT_EQ(sum, 0x100);
  ASSERT_EQ(tk_pixel_limit_uint8(sum), 0xff);
}

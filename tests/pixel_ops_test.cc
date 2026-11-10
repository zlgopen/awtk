/**
 * File:   pixel_ops_test.cc
 * Brief:  blend/pixel_ops.inc 里各合成函数的单元测试
 *
 * pixel_ops.inc 依赖每个编译单元提供的像素格式宏，这里以 bgra8888 实例化。
 */

#include "base/pixel.h"
#include "tkc/color.h"
#include "gtest/gtest.h"

#define pixel_t pixel_bgra8888_t
#define pixel_from_rgb pixel_bgra8888_from_rgb
#define pixel_from_rgba pixel_bgra8888_from_rgba
#define pixel_to_rgba pixel_bgra8888_to_rgba
#include "blend/pixel_ops.inc"

static rgba_t to_rgba(pixel_t p) {
  rgba_t rgba = pixel_bgra8888_to_rgba(p);

  return rgba;
}

static color_t to_color(rgba_t rgba) {
  color_t c;
  c.rgba = rgba;

  return c;
}

static pixel_t to_pixel(rgba_t rgba) {
  pixel_t p = pixel_bgra8888_from_rgba(rgba.r, rgba.g, rgba.b, rgba.a);

  return p;
}

static void assert_pixel(pixel_t actual, int r, int g, int b, int a) {
  rgba_t c = to_rgba(actual);

  ASSERT_EQ(c.r, r);
  ASSERT_EQ(c.g, g);
  ASSERT_EQ(c.b, b);
  ASSERT_EQ(c.a, a);
}

/* 目标不透明：走线性插值快路径 */
TEST(PixelOps, blend_rgba_opaque_dst) {
  rgba_t d = {100, 150, 200, 0xff};
  rgba_t s = {10, 20, 30, 0xff};

  assert_pixel(blend_rgba(d, s, 128), 54, 84, 114, 0xff);
}

/* 目标半透明：走精确合成（按 out_a 归一化，含除法） */
TEST(PixelOps, blend_rgba_translucent_dst) {
  rgba_t d = {100, 150, 200, 0x40};
  rgba_t s = {10, 20, 30, 0x80};

  assert_pixel(blend_rgba(d, s, 0xff), 27, 45, 62, 160);
}

/* 目标和源都全透明：out_a 为 0，返回全透明 */
TEST(PixelOps, blend_rgba_all_transparent) {
  rgba_t d = {0, 0, 0, 0};
  rgba_t s = {0, 0, 0, 0};

  assert_pixel(blend_rgba(d, s, 0xff), 0, 0, 0, 0);
}

/* 预乘源色叠加：目标不透明，直接叠加并把 alpha 置为不透明 */
TEST(PixelOps, blend_rgba_premulti_opaque_dst) {
  rgba_t d = {100, 150, 200, 0xff};
  rgba_t s = {10, 20, 30, 128};

  assert_pixel(blend_rgba_premulti(d, s, 128), 60, 95, 130, 0xff);
}

/* 预乘源色叠加：目标半透明，走精确合成 */
TEST(PixelOps, blend_rgba_premulti_translucent_dst) {
  rgba_t d = {100, 150, 200, 0x40};
  rgba_t s = {10, 20, 30, 128};

  assert_pixel(blend_rgba_premulti(d, s, 128), 19, 29, 38, 160);
}

/* 整体变暗：目标不透明 */
TEST(PixelOps, blend_rgba_dark_opaque_dst) {
  rgba_t d = {100, 150, 200, 0xff};

  assert_pixel(blend_rgba_dark(d, 128), 50, 75, 100, 0xff);
}

/* 整体变暗：目标半透明 */
TEST(PixelOps, blend_rgba_dark_translucent_dst) {
  rgba_t d = {100, 150, 200, 0x40};

  assert_pixel(blend_rgba_dark(d, 128), 19, 29, 38, 160);
}

/* blend_alpha 等价于 blend_rgba_dark：源不透明 */
TEST(PixelOps, blend_alpha_opaque) {
  rgba_t fg = {50, 60, 70, 0xff};

  assert_pixel(blend_alpha(to_color(fg), 128), 25, 30, 35, 0xff);
}

/* blend_alpha 等价于 blend_rgba_dark：源半透明 */
TEST(PixelOps, blend_alpha_translucent) {
  rgba_t fg = {50, 60, 70, 0x40};

  assert_pixel(blend_alpha(to_color(fg), 128), 9, 11, 13, 160);
}

/* 前景色覆盖背景色：整体 alpha 大于阈值，走线性插值快路径 */
TEST(PixelOps, blend_color_opaque_alpha) {
  rgba_t bg = {100, 150, 200, 0xff};
  rgba_t fg = {10, 20, 30, 0xff};

  assert_pixel(blend_color(to_color(bg), to_color(fg), 0xff), 9, 19, 29, 0xff);
}

/* 前景色覆盖背景色：整体 alpha 小于阈值，走精确合成 */
TEST(PixelOps, blend_color_translucent_alpha) {
  rgba_t bg = {100, 150, 200, 0xff};
  rgba_t fg = {10, 20, 30, 0xff};

  assert_pixel(blend_color(to_color(bg), to_color(fg), 128), 54, 84, 114, 0xff);
}

/* 前景色覆盖背景色：背景也半透明 */
TEST(PixelOps, blend_color_translucent_dst) {
  rgba_t bg = {100, 150, 200, 0x40};
  rgba_t fg = {10, 20, 30, 0x80};

  assert_pixel(blend_color(to_color(bg), to_color(fg), 0x80), 47, 74, 101, 112);
}

/* 像素合成：目标不透明 */
TEST(PixelOps, blend_pixel_opaque_dst) {
  rgba_t d = {100, 150, 200, 0xff};
  rgba_t c = {10, 20, 30, 128};

  assert_pixel(blend_pixel(to_pixel(d), to_color(c)), 54, 84, 114, 0xff);
}

/* 像素合成：目标半透明 */
TEST(PixelOps, blend_pixel_translucent_dst) {
  rgba_t d = {100, 150, 200, 0x40};
  rgba_t c = {10, 20, 30, 0x80};

  assert_pixel(blend_pixel(to_pixel(d), to_color(c)), 27, 45, 62, 160);
}

/* 合成后 alpha 为 0x100，必须饱和到 0xff（不能截断成 0） */
TEST(PixelOps, blend_rgba_out_alpha_saturate) {
  rgba_t d = {100, 150, 200, 0xf4};
  rgba_t s = {10, 20, 30, 0xf8};

  assert_pixel(blend_rgba(d, s, 0xf8), 12, 22, 33, 0xff);
}

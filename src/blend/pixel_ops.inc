/**
 * File:   pixel_ops.inc
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  pixel operations
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/pixel.h"

/**
 * 目标不透明时的快路径：前景 s 和背景 d 的线性插值 linear interpolation(lerp)，
 * 前景权重为 a/256：a 为 0 时近似取背景、为 0xff 时近似取前景，
 * 即 (d * (0xff - a) + s * a) >> 8
 */
static inline pixel_t blend_rgb_lerp(rgba_t d, rgba_t s, uint8_t a) {
  uint8_t minus_a = 0xff - a;
  uint8_t r = (d.r * minus_a + s.r * a) >> 8;
  uint8_t g = (d.g * minus_a + s.g * a) >> 8;
  uint8_t b = (d.b * minus_a + s.b * a) >> 8;
  pixel_t p = pixel_from_rgb(r, g, b);
  return p;
}

/* 按合成 alpha 归一化，out_a 为 0 时返回全透明 */
static inline pixel_t blend_rgb_div(uint32_t r, uint32_t g, uint32_t b, uint8_t out_a) {
  if (out_a > 0) {
    uint8_t r8 = (uint8_t)(r / out_a);
    uint8_t g8 = (uint8_t)(g / out_a);
    uint8_t b8 = (uint8_t)(b / out_a);
    pixel_t p = pixel_from_rgba(r8, g8, b8, out_a);
    return p;
  } else {
    pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
    return p;
  }
}

/* 前景色按 s.a 覆盖背景色（背景按 1 - s.a 保留），s.a 为前景的有效 alpha */
static inline pixel_t blend_rgba_over(rgba_t d, rgba_t s) {
  uint8_t out_a = pixel_out_alpha(s.a, d.a);
  uint8_t d_a = (d.a * (0xff - s.a)) >> 8;
  uint32_t r = d.r * d_a + s.r * s.a;
  uint32_t g = d.g * d_a + s.g * s.a;
  uint32_t b = d.b * d_a + s.b * s.a;

  return blend_rgb_div(r, g, b, out_a);
}

/****************************************************************************************/

static inline pixel_t blend_rgba(rgba_t d, rgba_t s, uint8_t a) {
  if (d.a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    return blend_rgb_lerp(d, s, a);
  }
  return blend_rgba_over(d, s);
}

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  if (a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    return blend_rgb_lerp(bg.rgba, fg.rgba, a);
  }
  fg.rgba.a = (fg.rgba.a * a) >> 8;

  return blend_rgba_over(bg.rgba, fg.rgba);
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  rgba_t d = pixel_to_rgba(pixel);
  return blend_rgba(d, c.rgba, c.rgba.a);
}

static inline pixel_t blend_rgba_dark(rgba_t d, uint8_t a) {
  if (d.a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    uint8_t r = (d.r * a) >> 8;
    uint8_t g = (d.g * a) >> 8;
    uint8_t b = (d.b * a) >> 8;
    pixel_t p = pixel_from_rgb(r, g, b);
    return p;
  } else {
    uint8_t out_a = pixel_out_alpha(a, d.a);
    uint8_t d_a = (d.a * (0xff - a)) >> 8;

    return blend_rgb_div(d.r * d_a, d.g * d_a, d.b * d_a, out_a);
  }
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  return blend_rgba_dark(fg.rgba, a);
}

static inline pixel_t blend_rgba_premulti(rgba_t d, rgba_t s, uint8_t a) {
  if (d.a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    uint8_t r = ((d.r * a) >> 8) + s.r;
    uint8_t g = ((d.g * a) >> 8) + s.g;
    uint8_t b = ((d.b * a) >> 8) + s.b;
    pixel_t p = pixel_from_rgb(r, g, b);
    return p;
  } else {
    uint8_t out_a = pixel_out_alpha(s.a, d.a);
    uint8_t d_a = (d.a * (0xff - s.a)) >> 8;
    uint32_t r = d.r * d_a + s.r;
    uint32_t g = d.g * d_a + s.g;
    uint32_t b = d.b * d_a + s.b;

    return blend_rgb_div(r, g, b, out_a);
  }
}

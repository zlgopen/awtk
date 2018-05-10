/**
 * File:   pixel_ops.inc
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  pixel operations
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  uint8_t minus_a = 0xff - a;

  uint8_t r = (bg.rgba.r * minus_a + fg.rgba.r * a) >> 8;
  uint8_t g = (bg.rgba.g * minus_a + fg.rgba.g * a) >> 8;
  uint8_t b = (bg.rgba.b * minus_a + fg.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  color_t bg = to_color(pixel);

  uint8_t r = (bg.rgba.r * minus_a + c.rgba.r * a) >> 8;
  uint8_t g = (bg.rgba.g * minus_a + c.rgba.g * a) >> 8;
  uint8_t b = (bg.rgba.b * minus_a + c.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  uint8_t r = (fg.rgba.r * a) >> 8;
  uint8_t g = (fg.rgba.g * a) >> 8;
  uint8_t b = (fg.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

#define color_to_pixel(c) rgb_to_pixel(c.rgba.r, c.rgba.g, c.rgba.b)

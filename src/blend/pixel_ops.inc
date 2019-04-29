/**
 * File:   pixel_ops.inc
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  pixel operations
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  rgba_t rgba = pixel_to_rgba(pixel);

  uint8_t r = (rgba.r * minus_a + c.rgba.r * a) >> 8;
  uint8_t g = (rgba.g * minus_a + c.rgba.g * a) >> 8;
  uint8_t b = (rgba.b * minus_a + c.rgba.b * a) >> 8;
  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  uint8_t r = (fg.rgba.r * a) >> 8;
  uint8_t g = (fg.rgba.g * a) >> 8;
  uint8_t b = (fg.rgba.b * a) >> 8;
  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

static inline pixel_t blend_rgba(rgba_t d, rgba_t s, uint8_t a) {
  uint8_t minus_a = 0xff - a;
  uint8_t r = (d.r * minus_a + s.r * a) >> 8;
  uint8_t g = (d.g * minus_a + s.g * a) >> 8;
  uint8_t b = (d.b * minus_a + s.b * a) >> 8;
  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

static inline pixel_t blend_rgba_premulti(rgba_t d, rgba_t s, uint8_t a) {
  uint8_t r = ((d.r * a) >> 8) + s.r;
  uint8_t g = ((d.g * a) >> 8) + s.g;
  uint8_t b = ((d.b * a) >> 8) + s.b;

  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

static inline pixel_t blend_rgba_dark(rgba_t d, uint8_t a) {
  uint8_t r = ((d.r * a) >> 8);
  uint8_t g = ((d.g * a) >> 8);
  uint8_t b = ((d.b * a) >> 8);

  pixel_t p = pixel_from_rgb(r, g, b);

  return p;
}

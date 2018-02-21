/**
 * File:   rgb565.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stm32_raw implemented lcd interface/
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

typedef uint16_t pixel_t;

#define rgb_to_pixel(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
static inline pixel_t to_pixel(color_t c) { return rgb_to_pixel(c.rgba.r, c.rgba.g, c.rgba.b); }

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  uint8_t minus_a = 0xff - a;
  uint16_t rr = (bg.rgba.r * minus_a + fg.rgba.r * a) / 0xff;
  uint16_t gg = (bg.rgba.g * minus_a + fg.rgba.g * a) / 0xff;
  uint16_t bb = (bg.rgba.b * minus_a + fg.rgba.b * a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  uint16_t rr = ((0xff & ((pixel >> 11) << 3)) * minus_a + c.rgba.r * a) / 0xff;
  uint16_t gg = ((0xff & ((pixel >> 5) << 2)) * minus_a + c.rgba.g * a) / 0xff;
  uint16_t bb = ((0xff & (pixel << 3)) * minus_a + c.rgba.b * a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  uint16_t rr = (fg.rgba.r + a) / 0xff;
  uint16_t gg = (fg.rgba.g + a) / 0xff;
  uint16_t bb = (fg.rgba.b + a) / 0xff;
  uint8_t r = rr & 0xff;
  uint8_t g = gg & 0xff;
  uint8_t b = bb & 0xff;

  return rgb_to_pixel(r, g, b);
}


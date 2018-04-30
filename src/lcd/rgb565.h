/**
 * File:   rgb565.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rgb565
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

#define LCD_FORMAT BITMAP_FMT_RGB565

#define rgb_to_pixel(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
static inline pixel_t to_pixel(color_t c) { return rgb_to_pixel(c.rgba.r, c.rgba.g, c.rgba.b); }

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  uint8_t minus_a = 0xff - a;
  uint8_t r = (bg.rgba.r * minus_a + fg.rgba.r * a) >> 8;
  uint8_t g = (bg.rgba.g * minus_a + fg.rgba.g * a) >> 8;
  uint8_t b = (bg.rgba.b * minus_a + fg.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

static inline color_t to_color(pixel_t pixel) {
  uint8_t r = (0xff & ((pixel >> 11) << 3));
  uint8_t g = (0xff & ((pixel >> 5) << 2));
  uint8_t b = (0xff & (pixel << 3));

  return color_init(r, g, b, 0xff);
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  uint8_t r = ((0xff & ((pixel >> 11) << 3)) * minus_a + c.rgba.r * a) >> 8;
  uint8_t g = ((0xff & ((pixel >> 5) << 2)) * minus_a + c.rgba.g * a) >> 8;
  uint8_t b = ((0xff & (pixel << 3)) * minus_a + c.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  uint8_t r = (fg.rgba.r * a) >> 8;
  uint8_t g = (fg.rgba.g * a) >> 8;
  uint8_t b = (fg.rgba.b * a) >> 8;

  return rgb_to_pixel(r, g, b);
}

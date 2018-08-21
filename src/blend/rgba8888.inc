/**
 * File:   rgba.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief: rgba
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

#include "base/pixel_pack_unpack.h"

#define rgb_to_pixel(r, g, b) rgb_to_rgba8888(r, g, b)

static inline color_t to_color(pixel_t pixel) {
  color_t c;
  c.color = 0;
  c.rgba.r = (pixel >> 24);
  c.rgba.g = (pixel >> 16);
  c.rgba.b = (pixel >> 8);
  c.rgba.a = pixel & 0xff;

  return c;
}

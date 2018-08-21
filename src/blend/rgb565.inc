/**
 * File:   rgb565.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rgb565
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

#define rgb_to_pixel(r, g, b) rgb_to_565(r, g, b)
static inline color_t to_color(pixel_t pixel) {
  color_t c;

  c.rgba.a = 0xff;
  c.rgba.r = (0xff & ((pixel >> 11) << 3));
  c.rgba.g = (0xff & ((pixel >> 5) << 2));
  c.rgba.b = (0xff & (pixel << 3));

  return c;
}

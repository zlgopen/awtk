/**
 * File:   bitmap.c
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/bitmap.h"

ret_t bitmap_destroy(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL && bitmap->destroy != NULL, RET_BAD_PARAMS);

  if (bitmap->specific_destroy != NULL) {
    bitmap->specific_destroy(bitmap);
  }

  return bitmap->destroy(bitmap);
}

#include "blend/pixel_pack_unpack.h"

ret_t bitmap_rgba_to_rgb565(bitmap_t* image, uint16_t* output) {
  uint32_t i = 0;
  uint16_t* d = output;
  uint32_t nr = image->w * image->h;
  uint32_t* s = (uint32_t*)(image->data);
  return_value_if_fail(image->format == BITMAP_FMT_RGBA, RET_BAD_PARAMS);

  for (i = 0; i < nr; i++, d++, s++) {
    color_t c;
    c.color = *s;
    *d = rgb_to_565(c.rgba.r, c.rgba.g, c.rgba.b);
  }

  return RET_OK;
}

ret_t bitmap_rgba_to_bgra(bitmap_t* image) {
  int i = 0;
  int nr = image->w * image->h;
  uint8_t* data = image->data;

  /*default format is rgba, some sys need bgra*/
  for (i = 0; i < nr; i += 4) {
    uint8_t r = data[0];
    uint8_t g = data[1];
    uint8_t b = data[2];
    uint8_t a = data[3];
    data[0] = b;
    data[1] = g;
    data[2] = r;
    data[3] = a;

    data += 4;
  }
  image->format = BITMAP_FMT_BGRA;

  return RET_OK;
}

/**
 * File:   blend.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  image blend
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
 * 2018-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "blend/blend.h"

ret_t blend_image(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t alpha) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL && dst_r != NULL,
                       RET_BAD_PARAMS);
  return_value_if_fail(dst->format == BITMAP_FMT_RGB565 || dst->format == BITMAP_FMT_RGBA,
                       RET_BAD_PARAMS);
  return_value_if_fail(src->format == BITMAP_FMT_RGB565 || src->format == BITMAP_FMT_RGBA,
                       RET_BAD_PARAMS);

  if (dst->format == BITMAP_FMT_RGB565) {
    if (src->format == BITMAP_FMT_RGB565) {
      return blend_image_565_565(dst, src, dst_r, src_r, alpha);
    } else {
      return blend_image_565_8888(dst, src, dst_r, src_r, alpha);
    }
  } else {
    if (src->format == BITMAP_FMT_RGB565) {
      return blend_image_8888_565(dst, src, dst_r, src_r, alpha);
    } else {
      return blend_image_8888_8888(dst, src, dst_r, src_r, alpha);
    }
  }
}

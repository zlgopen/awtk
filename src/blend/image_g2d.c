/**
 * File:   image_g2d.c
 * Author: AWTK Develop Team
 * Brief:  image 2d operations.
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
 * 2018-06-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/g2d.h"
#include "blend/soft_g2d.h"
#include "blend/image_g2d.h"

ret_t image_fill(bitmap_t* dst, rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_OK);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= dst->w);
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= dst->h);

#ifdef WITH_G2D
  if (g2d_fill_rect(dst, dst_r, c) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_fill_rect(dst, dst_r, c);
}

ret_t image_clear(bitmap_t* dst, rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_OK);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= dst->w);
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= dst->h);

  return soft_clear_rect(dst, dst_r, c);
}

ret_t image_copy(bitmap_t* dst, bitmap_t* src, rect_t* src_r, xy_t dx, xy_t dy) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_OK);

  assert(dx >= 0 && (dx + src_r->w) <= dst->w);
  assert(dy >= 0 && (dy + src_r->h) <= dst->h);

#ifdef WITH_G2D
  if (g2d_copy_image(dst, src, src_r, dx, dy) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_copy_image(dst, src, src_r, dx, dy);
}

ret_t image_rotate(bitmap_t* dst, bitmap_t* src, rect_t* src_r, lcd_orientation_t o) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_OK);

  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    assert(src_r->w <= dst->w);
    assert(src_r->h <= dst->h);
  } else if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
    assert(src_r->w <= dst->h);
    assert(src_r->h <= dst->w);
  }

#ifdef WITH_G2D
  if (g2d_rotate_image(dst, src, src_r, o) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_rotate_image(dst, src, src_r, o);
}

ret_t image_blend(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r,
                  uint8_t global_alpha) {
  return_value_if_fail(dst != NULL && src != NULL && dst_r != NULL && src_r != NULL,
                       RET_BAD_PARAMS);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= dst->w);
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= dst->h);

#ifdef WITH_G2D
  if (g2d_blend_image(dst, src, dst_r, src_r, global_alpha) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_blend_image(dst, src, dst_r, src_r, global_alpha);
}

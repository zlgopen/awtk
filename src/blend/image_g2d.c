/**
 * File:   image_g2d.c
 * Author: AWTK Develop Team
 * Brief:  image 2d operations.
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
 * 2018-06-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/g2d.h"
#include "blend/soft_g2d.h"
#include "blend/image_g2d.h"

ret_t image_fill(bitmap_t* fb, rect_t* dst, color_t c) {
  return_value_if_fail(fb != NULL && dst != NULL, RET_OK);

#ifdef WITH_G2D
  if (g2d_fill_rect(fb, dst, c) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_fill_rect(fb, dst, c);
}

ret_t image_copy(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy) {
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_OK);

#ifdef WITH_G2D
  if (g2d_copy_image(fb, img, src, dx, dy) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_copy_image(fb, img, src, dx, dy);
}

ret_t image_rotate(bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o) {
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_OK);

#ifdef WITH_G2D
  if (g2d_rotate_image(fb, img, src, o) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_rotate_image(fb, img, src, o);
}

ret_t image_blend(bitmap_t* fb, bitmap_t* img, rect_t* dst, rect_t* src, uint8_t global_alpha) {
  return_value_if_fail(fb != NULL && img != NULL && dst != NULL && src != NULL, RET_BAD_PARAMS);

#ifdef WITH_G2D
  if (g2d_blend_image(fb, img, dst, src, global_alpha) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_blend_image(fb, img, dst, src, global_alpha);
}

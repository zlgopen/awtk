/**
 * File:   image_g2d.h
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

#ifndef TK_IMAGE_G2D_H
#define TK_IMAGE_G2D_H

#include "base/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

ret_t image_fill(bitmap_t* fb, rect_t* dst, color_t c);
ret_t image_copy(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy);
ret_t image_rotate(bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o);
ret_t image_blend(bitmap_t* fb, bitmap_t* img, rect_t* dst, rect_t* src, uint8_t global_alpha);

END_C_DECLS

#endif /*TK_IMAGE_G2D_H*/

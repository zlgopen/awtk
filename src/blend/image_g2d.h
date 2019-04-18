/**
 * File:   image_g2d.h
 * Author: AWTK Develop Team
 * Brief:  image 2d operations.
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
 * 2018-06-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IMAGE_G2D_H
#define TK_IMAGE_G2D_H

#include "tkc/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

ret_t image_fill(bitmap_t* dst, rect_t* dst_r, color_t c);
ret_t image_clear(bitmap_t* dst, rect_t* dst_r, color_t c);
ret_t image_copy(bitmap_t* dst, bitmap_t* src, rect_t* src_r, xy_t dx, xy_t dy);
ret_t image_rotate(bitmap_t* dst, bitmap_t* src, rect_t* src_r, lcd_orientation_t o);
ret_t image_blend(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t global_alpha);

END_C_DECLS

#endif /*TK_IMAGE_G2D_H*/

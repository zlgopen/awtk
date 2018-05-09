/**
 * File:   blend.h
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

#ifndef TK_BLEND_H
#define TK_BLEND_H

#include "base/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

ret_t blend_image(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t alpha);

ret_t blend_image_8888_565(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a);
ret_t blend_image_8888_8888(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a);
ret_t blend_image_565_565(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a);
ret_t blend_image_565_8888(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t a);

END_C_DECLS

#endif /*TK_BLEND_H*/

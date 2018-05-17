/**
 * File:   g2d.h
 * Author: AWTK Develop Team
 * Brief:  hardware 2d
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-07 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_G2D_H
#define TK_G2D_H

#include "base/rect.h"
#include "base/bitmap.h"

BEGIN_C_DECLS

ret_t g2d_fill_rect(bitmap_t* fb, xy_t x, xy_t y, wh_t w, wh_t h, color_t c);
ret_t g2d_copy_image(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy);
ret_t g2d_blend_image(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy);

END_C_DECLS

#endif /*TK_G2D_H*/

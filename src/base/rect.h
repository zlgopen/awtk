/**
 * File:   rect.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  rect struct and utils functions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_RECT_H
#define LFTK_RECT_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _point_t {
    xy_t x;
    xy_t y;
}point_t;

typedef struct _rect_t {
    xy_t x;
    xy_t y;
    wh_t w;
    wh_t h;    
}rect_t;

#define rect_init(r, xx, yy, ww, hh) r.x = (xx); r.y = (yy); r.w = (ww); r.h = (hh);
#define rectp_init(r, xx, yy, ww, hh) r->x = (xx); r->y = (yy); r->w = (ww); r->h = (hh);

ret_t rect_merge(rect_t* dst_r, rect_t* r);

END_C_DECLS

#endif/*LFTK_RECT_H*/


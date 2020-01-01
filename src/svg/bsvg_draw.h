/**
 * File:   bsvg_draw.h
 * Author: AWTK Develop Team
 * Brief:  bsvg_draw
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
 * 2018-11-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BSVG_DRAW_H
#define TK_BSVG_DRAW_H

#include "svg/bsvg.h"
#include "base/vgcanvas.h"

BEGIN_C_DECLS

ret_t bsvg_draw(bsvg_t* svg, vgcanvas_t* canvas);

END_C_DECLS

#endif /*TK_BSVG_DRAW_H*/

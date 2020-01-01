/**
 * File:   bsvg_to_svg.c
 * Author: AWTK Develop Team
 * Brief:  bsvg to svg
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
 * 2018-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BSVG_TO_SVG_H
#define TK_BSVG_TO_SVG_H

#include "svg/bsvg.h"

BEGIN_C_DECLS

ret_t bsvg_to_svg(const uint32_t* data, uint32_t size, str_t* str);

/*public for test*/
ret_t bsvg_to_svg_path(str_t* str, const svg_path_t* path);
ret_t bsvg_to_svg_shape(str_t* str, const svg_shape_t* shape);

END_C_DECLS

#endif /*TK_BSVG_TO_SVG_H*/

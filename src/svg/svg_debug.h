/**
 * File:   svg_debug.h
 * Author: AWTK Develop Team
 * Brief:  svg debug functions
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
 * 2018-11-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_DEBUG_H
#define TK_SVG_DEBUG_H

#include "svg/svg_shape.h"

BEGIN_C_DECLS

ret_t svg_shape_dump(svg_shape_t* shape);
ret_t svg_path_dump(void* ctx, const svg_path_t* path);

END_C_DECLS

#endif /*TK_SVG_DEBUG_H*/

/**
 * File:   bsvg_builder.h
 * Author: AWTK Develop Team
 * Brief:  bsvg builder
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
 * 2018-11-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BSVG_BUILDER_H
#define TK_BSVG_BUILDER_H

#include "svg/svg_shape.h"
#include "svg/bsvg_common.h"

BEGIN_C_DECLS

typedef struct _bsvg_builder_t {
  wbuffer_t buff;
  bsvg_header_t* header;
  svg_shape_type_t current_shape_type;
} bsvg_builder_t;

bsvg_builder_t* bsvg_builder_init(bsvg_builder_t* svg, uint32_t* buff, uint32_t size);

ret_t bsvg_builder_add_shape(bsvg_builder_t* svg, const svg_shape_t* shape);
ret_t bsvg_builder_add_sub_path(bsvg_builder_t* svg, const svg_path_t* path);
ret_t bsvg_builder_done(bsvg_builder_t* svg);

END_C_DECLS

#endif /*TK_BSVG_BUILDER_H*/

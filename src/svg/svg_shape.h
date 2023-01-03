/**
 * File:   svg_shape.h
 * Author: AWTK Develop Team
 * Brief:  svg shape
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_SVG_SHAPE_H
#define TK_SVG_SHAPE_H

#include "svg/svg_color.h"

BEGIN_C_DECLS

typedef enum _svg_shape_type_t {
  SVG_SHAPE_NULL = 0,
  SVG_SHAPE_TEXT,
  SVG_SHAPE_PATH
} svg_shape_type_t;

#pragma pack(push, 1)
typedef struct _svg_shape_t {
  uint8_t type;
  uint8_t fill_type;
  uint8_t stroke_type;
  uint8_t reserved[1];
  float stroke_width;
} svg_shape_t;

typedef struct _svg_shape_text_t {
  svg_shape_t shape;
  float x;
  float y;
} svg_shape_text_t;
#pragma pack(pop)

uint32_t svg_shape_size(const svg_shape_t* shape);

svg_color_t* svg_shape_get_fill(const svg_shape_t* s);

svg_color_t* svg_shape_get_stroke(const svg_shape_t* s);

svg_shape_t* svg_shape_text_init(svg_shape_text_t* s, float x, float y);

char* svg_shape_text_get_text(const svg_shape_t* s);

svg_shape_t* svg_shape_path_init(svg_shape_t* s);

END_C_DECLS

#endif /*TK_SVG_SHAPE_H*/

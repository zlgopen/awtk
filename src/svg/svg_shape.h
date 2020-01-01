/**
 * File:   svg_shape.h
 * Author: AWTK Develop Team
 * Brief:  svg shape
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

#ifndef TK_SVG_SHAPE_H
#define TK_SVG_SHAPE_H

#include "svg/svg_path.h"

BEGIN_C_DECLS

typedef enum _svg_shape_type_t {
  SVG_SHAPE_NULL = 0,
  SVG_SHAPE_RECT,
  SVG_SHAPE_CIRCLE,
  SVG_SHAPE_ELLIPSE,
  SVG_SHAPE_LINE,
  SVG_SHAPE_POLYGON,
  SVG_SHAPE_POLYLINE,
  SVG_SHAPE_PATH
} svg_shape_type_t;

#pragma pack(push, 1)
typedef struct _svg_shape_t {
  uint8_t type;
  uint8_t line_cap;
  uint8_t line_join;
  uint8_t stroke_width;
  uint8_t no_fill;
  uint8_t no_stroke;
  uint16_t reserved;
  color_t fill;
  color_t stroke;
} svg_shape_t;

typedef struct _svg_shape_rect_t {
  svg_shape_t shape;
  float x;
  float y;
  float w;
  float h;
  float r;
} svg_shape_rect_t;

typedef struct _svg_shape_circle_t {
  svg_shape_t shape;
  float cx;
  float cy;
  float r;
} svg_shape_circle_t;

typedef struct _svg_shape_ellipse_t {
  svg_shape_t shape;
  float cx;
  float cy;
  float rx;
  float ry;
} svg_shape_ellipse_t;

typedef struct _svg_shape_line_t {
  svg_shape_t shape;
  float x1;
  float y1;
  float x2;
  float y2;
} svg_shape_line_t;

typedef struct _svg_shape_polyline_t {
  svg_shape_t shape;
  uint32_t nr;
  float data[1];
} svg_shape_polyline_t;

typedef struct _svg_shape_polygon_t {
  svg_shape_t shape;
  uint32_t nr;
  float data[1];
} svg_shape_polygon_t;

typedef struct _svg_shape_path_t {
  svg_shape_t shape;

  svg_path_t path[1];
} svg_shape_path_t;
#pragma pack(pop)

uint32_t svg_shape_size(const svg_shape_t* shape);

svg_shape_t* svg_shape_rect_init(svg_shape_rect_t* s, float x, float y, float w, float h, float r);
svg_shape_t* svg_shape_circle_init(svg_shape_circle_t* s, float cx, float cy, float r);
svg_shape_t* svg_shape_ellipse_init(svg_shape_ellipse_t* s, float cx, float cy, float rx, float ry);
svg_shape_t* svg_shape_line_init(svg_shape_line_t* s, float x1, float y1, float x2, float y2);
svg_shape_t* svg_shape_polygon_init(svg_shape_polygon_t* s);
svg_shape_t* svg_shape_polyline_init(svg_shape_polyline_t* s);
svg_shape_t* svg_shape_path_init(svg_shape_path_t* s);

END_C_DECLS

#endif /*TK_SVG_SHAPE_H*/

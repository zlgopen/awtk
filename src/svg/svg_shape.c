/**
 * File:   svg_shape.c
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

#include "svg/svg_shape.h"

uint32_t svg_shape_size(const svg_shape_t* shape) {
  uint32_t size = 0;
  switch (shape->type) {
    case SVG_SHAPE_RECT: {
      size = sizeof(svg_shape_rect_t);
      break;
    }
    case SVG_SHAPE_CIRCLE: {
      size = sizeof(svg_shape_circle_t);
      break;
    }
    case SVG_SHAPE_ELLIPSE: {
      size = sizeof(svg_shape_ellipse_t);
      break;
    }
    case SVG_SHAPE_LINE: {
      size = sizeof(svg_shape_line_t);
      break;
    }
    case SVG_SHAPE_POLYGON: {
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;
      size = sizeof(svg_shape_polygon_t) + s->nr * sizeof(float) - sizeof(s->data);
      break;
    }
    case SVG_SHAPE_POLYLINE: {
      svg_shape_polyline_t* s = (svg_shape_polyline_t*)shape;
      size = sizeof(svg_shape_polyline_t) + s->nr * sizeof(float) - sizeof(s->data);
      break;
    }
    case SVG_SHAPE_PATH: {
      size = sizeof(svg_shape_path_t);
      break;
    }
    case SVG_SHAPE_NULL: {
      size = sizeof(svg_shape_t);
      break;
    }
    default: {
      assert(!"not supported type");
      break;
    }
  }

  return size;
}

svg_shape_t* svg_shape_rect_init(svg_shape_rect_t* s, float x, float y, float w, float h, float r) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->x = x;
  s->y = y;
  s->w = w;
  s->h = h;
  s->r = r;
  s->shape.type = SVG_SHAPE_RECT;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_circle_init(svg_shape_circle_t* s, float cx, float cy, float r) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->r = r;
  s->cx = cx;
  s->cy = cy;
  s->shape.type = SVG_SHAPE_CIRCLE;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_ellipse_init(svg_shape_ellipse_t* s, float cx, float cy, float rx,
                                    float ry) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->rx = rx;
  s->ry = ry;
  s->cx = cx;
  s->cy = cy;
  s->shape.type = SVG_SHAPE_ELLIPSE;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_line_init(svg_shape_line_t* s, float x1, float y1, float x2, float y2) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->x1 = x1;
  s->y1 = y1;
  s->x2 = x2;
  s->y2 = y2;
  s->shape.type = SVG_SHAPE_LINE;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_polygon_init(svg_shape_polygon_t* s) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->nr = 0;
  s->shape.type = SVG_SHAPE_POLYGON;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_polyline_init(svg_shape_polyline_t* s) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->nr = 0;
  s->shape.type = SVG_SHAPE_POLYLINE;

  return (svg_shape_t*)s;
}

svg_shape_t* svg_shape_path_init(svg_shape_path_t* s) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->shape.type = SVG_SHAPE_PATH;

  return (svg_shape_t*)s;
}

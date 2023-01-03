/**
 * File:   svg_shape.c
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

#include "svg/svg_shape.h"
#include "svg/svg_color.inc"

static uint32_t _svg_shape_size(const svg_shape_t* shape) {
  uint32_t size = 0;
  switch (shape->type) {
    case SVG_SHAPE_TEXT: {
      size = sizeof(svg_shape_text_t);
      break;
    }
    case SVG_SHAPE_PATH: {
      size = sizeof(svg_shape_t);
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

uint32_t svg_shape_size(const svg_shape_t* shape) {
  return _svg_shape_size(shape) + svg_color_size(shape->fill_type) +
         svg_color_size(shape->stroke_type);
}

svg_color_t* svg_shape_get_fill(const svg_shape_t* s) {
  return_value_if_fail(s != NULL && s->fill_type != SVG_COLOR_NULL, NULL);
  uint8_t* p = (uint8_t*)s + _svg_shape_size(s);
  return (svg_color_t*)p;
}

svg_color_t* svg_shape_get_stroke(const svg_shape_t* s) {
  return_value_if_fail(s != NULL && s->stroke_type != SVG_COLOR_NULL, NULL);
  uint8_t* p = (uint8_t*)s + _svg_shape_size(s) + svg_color_size(s->fill_type);
  return (svg_color_t*)p;
}

svg_shape_t* svg_shape_text_init(svg_shape_text_t* s, float x, float y) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->shape.type = SVG_SHAPE_TEXT;
  s->x = x;
  s->y = y;

  return (svg_shape_t*)s;
}

char* svg_shape_text_get_text(const svg_shape_t* s) {
  return_value_if_fail(s != NULL && s->type == SVG_SHAPE_TEXT, NULL);

  return (char*)s + svg_shape_size(s);
}

svg_shape_t* svg_shape_path_init(svg_shape_t* s) {
  return_value_if_fail(s != NULL, NULL);

  memset(s, 0x00, sizeof(*s));

  s->type = SVG_SHAPE_PATH;

  return s;
}

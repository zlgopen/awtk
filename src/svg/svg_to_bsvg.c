/**
 * File:   svg_to_bsvg.c
 * Author: AWTK Develop Team
 * Brief:  svg to  bsvg
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
 * 2018-11-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/color_parser.h"
#include "xml/xml_parser.h"
#include "svg/svg_to_bsvg.h"
#include "svg/svg_path_parser.h"
#include "svg/bsvg_builder.h"

typedef struct _xml_builder_t {
  XmlBuilder builder;

  bsvg_builder_t bsvg;
} xml_builder_t;

static void svg_on_svg_tag(bsvg_builder_t* svg, const char** attrs) {
  uint32_t i = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "viewBox")) {
      tk_sscanf(v, "%d %d %d %d", &x, &y, &w, &h);

      svg->header->viewport.x = x;
      svg->header->viewport.y = y;
      svg->header->viewport.w = w;
      svg->header->viewport.h = h;
    } else if (tk_str_eq(k, "width")) {
      svg->header->w = tk_atoi(v);
    } else if (tk_str_eq(k, "height")) {
      svg->header->h = tk_atoi(v);
    }
    i += 2;
  }

  return;
}

static ret_t svg_on_sub_path(void* ctx, const void* data) {
  bsvg_builder_t* svg = (bsvg_builder_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;

  bsvg_builder_add_sub_path(svg, path);

  return RET_OK;
}

static void svg_init_shape(svg_shape_t* shape, const char** attrs) {
  uint32_t i = 0;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "stroke-linecap")) {
      shape->line_cap = v[0];
    } else if (tk_str_eq(k, "stroke-linejoin")) {
      shape->line_join = v[0];
    } else if (tk_str_eq(k, "stroke-width")) {
      shape->stroke_width = tk_atoi(v);
    } else if (tk_str_eq(k, "stroke")) {
      shape->stroke = color_parse_simple(v);
    } else if (tk_str_eq(k, "fill")) {
      shape->fill = color_parse_simple(v);
    }

    i += 2;
  }

  return;
}

static void svg_on_shape_path(bsvg_builder_t* svg, const char** attrs) {
  uint32_t i = 0;
  svg_shape_path_t s;
  svg_shape_path_init(&s);
  svg_init_shape((svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "d")) {
      svg_path_parse(v, svg, svg_on_sub_path);
    }
    i += 2;
  }

  return;
}

static void svg_on_shape_rect(bsvg_builder_t* svg, const char** attrs) {
  float_t x = 0;
  float_t y = 0;
  float_t w = 0;
  float_t h = 0;
  float_t r = 0;
  uint32_t i = 0;
  svg_shape_rect_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "x")) {
      x = tk_atof(v);
    } else if (tk_str_eq(k, "y")) {
      y = tk_atof(v);
    } else if (tk_str_eq(k, "w")) {
      w = tk_atof(v);
    } else if (tk_str_eq(k, "h")) {
      h = tk_atof(v);
    } else if (tk_str_eq(k, "rx")) {
      r = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_rect_init(&s, x, y, w, h, r);
  svg_init_shape((svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

static void svg_on_shape_circle(bsvg_builder_t* svg, const char** attrs) {
  float_t x = 0;
  float_t y = 0;
  float_t r = 0;
  uint32_t i = 0;
  svg_shape_circle_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "x")) {
      x = tk_atof(v);
    } else if (tk_str_eq(k, "y")) {
      y = tk_atof(v);
    } else if (tk_str_eq(k, "r")) {
      r = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_circle_init(&s, x, y, r);
  svg_init_shape((svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

static void svg_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  bsvg_builder_t* svg = &(b->bsvg);

  if (tk_str_eq(tag, "svg")) {
    svg_on_svg_tag(svg, attrs);
  } else if (tk_str_eq(tag, "path")) {
    svg_on_shape_path(svg, attrs);
  } else if (tk_str_eq(tag, "rect")) {
    svg_on_shape_rect(svg, attrs);
  } else if (tk_str_eq(tag, "circle")) {
    svg_on_shape_circle(svg, attrs);
  }

  return;
}

static void svg_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  printf("parse error: %d:%d %s\n", line, row, message);
  return;
}

static XmlBuilder* builder_init(xml_builder_t* b, uint32_t* buff, uint32_t buff_size) {
  memset(b, 0x00, sizeof(*b));

  b->builder.on_start = svg_on_start;
  b->builder.on_error = svg_on_error;
  bsvg_builder_init(&(b->bsvg), buff, buff_size);

  return &(b->builder);
}

ret_t svg_to_bsvg(const char* xml, uint32_t size, uint32_t** out, uint32_t* out_size) {
  xml_builder_t b;
  uint32_t* buff = NULL;
  uint32_t buff_size = size + 100;
  return_value_if_fail(xml != NULL && out != NULL && out_size != NULL, RET_BAD_PARAMS);

  buff = (uint32_t*)TKMEM_ALLOC(buff_size);
  return_value_if_fail(buff != NULL, RET_OOM);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b, buff, buff_size));
  xml_parser_parse(parser, xml, size);

  *out = buff;
  *out_size = b.bsvg.buff.cursor;

  return RET_OK;
}

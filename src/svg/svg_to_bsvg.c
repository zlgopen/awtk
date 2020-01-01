/**
 * File:   svg_to_bsvg.c
 * Author: AWTK Develop Team
 * Brief:  svg to  bsvg
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/color_parser.h"
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

static void svg_init_shape(bsvg_builder_t* svg, svg_shape_t* shape, const char** attrs) {
  uint32_t i = 0;
  bsvg_header_t* header = svg->header;
  shape->no_stroke = TRUE;
  shape->fill = header->fill;
  shape->stroke = header->stroke;

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
      if (tk_str_eq(v, "transparent") || tk_str_eq(v, "none")) {
        shape->no_stroke = TRUE;
      } else {
        shape->no_stroke = FALSE;
        if (tk_str_eq(v, "") || v == NULL) {
          shape->stroke = color_init(0x00, 0x00, 0x00, 0xff);
        } else {
          shape->stroke = color_parse(v);
        }
      }
    } else if (tk_str_eq(k, "fill")) {
      if (tk_str_eq(v, "transparent") || tk_str_eq(v, "none")) {
        shape->no_fill = TRUE;
      } else {
        if (tk_str_eq(v, "") || v == NULL) {
          shape->fill = color_init(0x00, 0x00, 0x00, 0xff);
        } else {
          shape->fill = color_parse(v);
        }
      }
    }

    i += 2;
  }

  return;
}

static void svg_on_shape_path(bsvg_builder_t* svg, const char** attrs) {
  uint32_t i = 0;
  svg_shape_path_t s;
  svg_shape_path_init(&s);
  svg_init_shape(svg, (svg_shape_t*)&s, attrs);
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

static void svg_on_shape_line(bsvg_builder_t* svg, const char** attrs) {
  float x1 = 0;
  float y1 = 0;
  float x2 = 0;
  float y2 = 0;
  uint32_t i = 0;
  svg_shape_line_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "x1")) {
      x1 = tk_atof(v);
    } else if (tk_str_eq(k, "y1")) {
      y1 = tk_atof(v);
    } else if (tk_str_eq(k, "x2")) {
      x2 = tk_atof(v);
    } else if (tk_str_eq(k, "y2")) {
      y2 = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_line_init(&s, x1, y1, x2, y2);
  svg_init_shape(svg, (svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

static void svg_on_shape_rect(bsvg_builder_t* svg, const char** attrs) {
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;
  float r = 0;
  uint32_t i = 0;
  svg_shape_rect_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "x")) {
      x = tk_atof(v);
    } else if (tk_str_eq(k, "y")) {
      y = tk_atof(v);
    } else if (tk_str_eq(k, "width")) {
      w = tk_atof(v);
    } else if (tk_str_eq(k, "height")) {
      h = tk_atof(v);
    } else if (tk_str_eq(k, "rx")) {
      r = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_rect_init(&s, x, y, w, h, r);
  svg_init_shape(svg, (svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

static void svg_on_shape_circle(bsvg_builder_t* svg, const char** attrs) {
  float x = 0;
  float y = 0;
  float r = 0;
  uint32_t i = 0;
  svg_shape_circle_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "cx")) {
      x = tk_atof(v);
    } else if (tk_str_eq(k, "cy")) {
      y = tk_atof(v);
    } else if (tk_str_eq(k, "r")) {
      r = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_circle_init(&s, x, y, r);
  svg_init_shape(svg, (svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

static void svg_on_shape_ellipse(bsvg_builder_t* svg, const char** attrs) {
  float x = 0;
  float y = 0;
  float rx = 0;
  float ry = 0;
  uint32_t i = 0;
  svg_shape_ellipse_t s;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "cx")) {
      x = tk_atof(v);
    } else if (tk_str_eq(k, "cy")) {
      y = tk_atof(v);
    } else if (tk_str_eq(k, "rx")) {
      rx = tk_atof(v);
    } else if (tk_str_eq(k, "ry")) {
      ry = tk_atof(v);
    }
    i += 2;
  }

  svg_shape_ellipse_init(&s, x, y, rx, ry);
  svg_init_shape(svg, (svg_shape_t*)&s, attrs);
  bsvg_builder_add_shape(svg, (svg_shape_t*)&s);

  return;
}

typedef struct _svg_number_parser_t {
  const char* p;
  const char* str;
  uint32_t max_nr;
  uint32_t nr;
  float* numbers;
} svg_number_parser_t;

static bool_t svg_number_parser_has_more(svg_number_parser_t* parser) {
  while (parser->p[0] == ' ') {
    parser->p++;
  }

  return parser->p[0];
}

static float svg_number_parser_get_number(svg_number_parser_t* parser) {
  uint32_t i = 0;
  const char* p = NULL;
  char token[TK_NUM_MAX_LEN + 1];

  p = parser->p;
  if (*p == '+' || *p == '-') {
    token[i++] = *p++;
  }
  while (*p == '.' || (*p >= '0' && *p <= '9')) {
    token[i++] = *p++;
  }
  token[i] = '\0';
  parser->p = p;

  return tk_atof(token);
}

static uint32_t svg_number_count(const char* str) {
  const char* p = str;
  uint32_t max_nr = 0;

  while (*p) {
    if (*p == ' ') {
      max_nr++;
    }
    p++;
  }

  return max_nr + 1;
}

static ret_t svg_number_parser_init(svg_number_parser_t* parser, const char* str, float* out,
                                    uint32_t max_nr) {
  parser->nr = 0;
  parser->p = str;
  parser->str = str;
  parser->max_nr = max_nr;
  parser->numbers = out;

  return RET_OK;
}

uint32_t svg_parse_numbers(const char* str, float* out, uint32_t max_nr) {
  svg_number_parser_t parser;

  return_value_if_fail(str != NULL && out != NULL, 0);
  return_value_if_fail(svg_number_parser_init(&parser, str, out, max_nr) == RET_OK, 0);

  while (svg_number_parser_has_more(&parser) && parser.nr < parser.max_nr) {
    parser.numbers[parser.nr++] = svg_number_parser_get_number(&parser);
  }

  return parser.nr;
}

static void svg_on_shape_polygon(bsvg_builder_t* svg, const char** attrs) {
  uint32_t i = 0;
  svg_shape_polygon_t* s = NULL;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "points")) {
      uint32_t max_nr = svg_number_count(v);
      s = TKMEM_ALLOC(sizeof(svg_shape_polygon_t) + sizeof(float) * max_nr);
      return_if_fail(s != NULL);

      svg_shape_polygon_init(s);
      s->nr = svg_parse_numbers(v, s->data, max_nr);

      break;
    }
    i += 2;
  }

  if (s != NULL) {
    svg_init_shape(svg, (svg_shape_t*)s, attrs);
    bsvg_builder_add_shape(svg, (svg_shape_t*)s);
    TKMEM_FREE(s);
  }

  return;
}

static void svg_on_shape_polyline(bsvg_builder_t* svg, const char** attrs) {
  uint32_t i = 0;
  svg_shape_polyline_t* s = NULL;

  while (attrs[i] != NULL) {
    const char* k = attrs[i];
    const char* v = attrs[i + 1];
    if (tk_str_eq(k, "points")) {
      uint32_t max_nr = svg_number_count(v);
      s = TKMEM_ALLOC(sizeof(svg_shape_polyline_t) + sizeof(float) * max_nr);
      return_if_fail(s != NULL);

      svg_shape_polyline_init(s);
      s->nr = svg_parse_numbers(v, s->data, max_nr);

      break;
    }
    i += 2;
  }

  if (s != NULL) {
    svg_init_shape(svg, (svg_shape_t*)s, attrs);
    bsvg_builder_add_shape(svg, (svg_shape_t*)s);
    TKMEM_FREE(s);
  }

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
  } else if (tk_str_eq(tag, "ellipse")) {
    svg_on_shape_ellipse(svg, attrs);
  } else if (tk_str_eq(tag, "line")) {
    svg_on_shape_line(svg, attrs);
  } else if (tk_str_eq(tag, "polygon")) {
    svg_on_shape_polygon(svg, attrs);
  } else if (tk_str_eq(tag, "polyline")) {
    svg_on_shape_polyline(svg, attrs);
  }

  return;
}

static void svg_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  log_debug("parse error: %d:%d %s\n", line, row, message);
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
  uint32_t buff_size = 2 * size + 100;
  return_value_if_fail(xml != NULL && out != NULL && out_size != NULL, RET_BAD_PARAMS);

  buff = (uint32_t*)TKMEM_ALLOC(buff_size);
  return_value_if_fail(buff != NULL, RET_OOM);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b, buff, buff_size));
  xml_parser_parse(parser, xml, size);
  bsvg_builder_done(&(b.bsvg));

  *out = buff;
  *out_size = b.bsvg.buff.cursor;
  xml_parser_destroy(parser);

  return RET_OK;
}

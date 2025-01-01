/**
 * File:   svg_path_parser.c
 * Author: AWTK Develop Team
 * Brief:  svg path_parser
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/mem.h"
#include "svg/svg_path.h"
#include "svg/svg_path_parser.h"
#include "svgtiny/include/svgtiny_arc.h"

typedef struct _svg_path_parser_t {
  char* path;
  void* ctx;
  tk_visit_t on_path;
} svg_path_parser_t;

static ret_t svg_path_parser_parse(svg_path_parser_t* parser) {
  char* s = parser->path;
  float first_x = 0, first_y = 0;
  float last_x = 0, last_y = 0;
  float last_cubic_x = 0, last_cubic_y = 0;
  float last_quad_x = 0, last_quad_y = 0;

  /* parse d and build path */
  s = parser->path;
  while (*s) {
    if (*s == ',') {
      *s = ' ';
    }
    s++;
  }

  s = parser->path;
  while (*s) {
    int n;
    char command[2];
    float x, y, x1, y1, x2, y2, rx, ry, rotation, large_arc, sweep;

    /* moveto (M, m) (2 arguments) */
    if (tk_sscanf(s, " %1[Mm] %f %f %n", command, &x, &y, &n) == 3) {
      svg_path_move_t path;

      do {
        if (*command == 'm') {
          x += last_x;
          y += last_y;
        }
        first_x = last_cubic_x = last_quad_x = last_x = x;
        first_y = last_cubic_y = last_quad_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_move_init(&path, x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %n", &x, &y, &n) == 2);

      /* lineto (L, l) (2 arguments) */
    } else if (tk_sscanf(s, " %1[Ll] %f %f %n", command, &x, &y, &n) == 3) {
      svg_path_line_t path;

      do {
        if (*command == 'l') {
          x += last_x;
          y += last_y;
        }
        last_cubic_x = last_quad_x = last_x = x;
        last_cubic_y = last_quad_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_line_init(&path, x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %n", &x, &y, &n) == 2);

      /* closepath (Z, z) (no arguments) */
    } else if (tk_sscanf(s, " %1[Zz] %n", command, &n) == 1) {
      svg_path_t path;

      last_x = first_x;
      last_y = first_y;
      s += n;

      memset(&path, 0, sizeof(path));
      path.type = SVG_PATH_Z;
      parser->on_path(parser->ctx, (svg_path_t*)&path);

      /* horizontal lineto (H, h) (1 argument) */
    } else if (tk_sscanf(s, " %1[Hh] %f %n", command, &x, &n) == 2) {
      svg_path_line_t path;

      do {
        if (*command == 'h') {
          x += last_x;
        }
        last_cubic_x = last_quad_x = last_x = x;
        last_cubic_y = last_quad_y = last_y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_line_init(&path, x, last_y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %n", &x, &n) == 1);

      /* vertical lineto (V, v) (1 argument) */
    } else if (tk_sscanf(s, " %1[Vv] %f %n", command, &y, &n) == 2) {
      svg_path_line_t path;

      do {
        if (*command == 'v') {
          y += last_y;
        }
        last_cubic_x = last_quad_x = last_x;
        last_cubic_y = last_quad_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_line_init(&path, last_x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %n", &y, &n) == 1);

      /* curveto (C, c) (6 arguments) */
    } else if (tk_sscanf(s, " %1[Cc] %f %f %f %f %f %f %n", command, &x1, &y1, &x2, &y2, &x, &y,
                         &n) == 7) {
      svg_path_curve_to_t path;

      do {
        if (*command == 'c') {
          x1 += last_x;
          y1 += last_y;
          x2 += last_x;
          y2 += last_y;
          x += last_x;
          y += last_y;
        }
        last_cubic_x = x2;
        last_cubic_y = y2;
        last_quad_x = last_x = x;
        last_quad_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_curve_to_init(&path, x1, y1, x2, y2, x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %f %f %f %f %n", &x1, &y1, &x2, &y2, &x, &y, &n) == 6);

      /* shorthand/smooth curveto (S, s) (4 arguments) */
    } else if (tk_sscanf(s, " %1[Ss] %f %f %f %f %n", command, &x2, &y2, &x, &y, &n) == 5) {
      svg_path_curve_to_t path;

      do {
        x1 = last_x + (last_x - last_cubic_x);
        y1 = last_y + (last_y - last_cubic_y);
        if (*command == 's') {
          x2 += last_x;
          y2 += last_y;
          x += last_x;
          y += last_y;
        }
        last_cubic_x = x2;
        last_cubic_y = y2;
        last_quad_x = last_x = x;
        last_quad_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_curve_to_init(&path, x1, y1, x2, y2, x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %f %f %n", &x2, &y2, &x, &y, &n) == 4);

      /* quadratic Bezier curveto (Q, q) (4 arguments) */
    } else if (tk_sscanf(s, " %1[Qq] %f %f %f %f %n", command, &x1, &y1, &x, &y, &n) == 5) {
      svg_path_curve_to_t path;
      float p[4];

      do {
        if (*command == 'q') {
          x1 += last_x;
          y1 += last_y;
          x += last_x;
          y += last_y;
        }
        last_quad_x = x1;
        last_quad_y = y1;
        p[0] = 1. / 3 * last_x + 2. / 3 * x1;
        p[1] = 1. / 3 * last_y + 2. / 3 * y1;
        p[2] = 2. / 3 * x1 + 1. / 3 * x;
        p[3] = 2. / 3 * y1 + 1. / 3 * y;
        last_cubic_x = last_x = x;
        last_cubic_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_curve_to_init(&path, p[0], p[1], p[2], p[3], x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %f %f %n", &x1, &y1, &x, &y, &n) == 4);

      /* shorthand/smooth quadratic Bezier curveto (T, t)
		   (2 arguments) */
    } else if (tk_sscanf(s, " %1[Tt] %f %f %n", command, &x, &y, &n) == 3) {
      svg_path_curve_to_t path;
      float p[4];

      do {
        x1 = last_x + (last_x - last_quad_x);
        y1 = last_y + (last_y - last_quad_y);
        last_quad_x = x1;
        last_quad_y = y1;
        if (*command == 't') {
          x += last_x;
          y += last_y;
        }
        p[0] = 1. / 3 * last_x + 2. / 3 * x1;
        p[1] = 1. / 3 * last_y + 2. / 3 * y1;
        p[2] = 2. / 3 * x1 + 1. / 3 * x;
        p[3] = 2. / 3 * y1 + 1. / 3 * y;
        last_cubic_x = last_x = x;
        last_cubic_y = last_y = y;
        s += n;

        memset(&path, 0, sizeof(path));
        svg_path_curve_to_init(&path, p[0], p[1], p[2], p[3], x, y);
        parser->on_path(parser->ctx, (svg_path_t*)&path);

      } while (tk_sscanf(s, "%f %f %n", &x, &y, &n) == 2);

      /* elliptical arc (A, a) (7 arguments) */
    } else if (tk_sscanf(s, " %1[Aa] %f %f %f %f %f %f %f %n", command, &rx, &ry, &rotation,
                         &large_arc, &sweep, &x, &y, &n) == 8) {
      svg_path_curve_to_t path;

      do {
        arc_info_t info;
        pointf_t cp1 = {0, 0};
        pointf_t cp2 = {0, 0};
        pointf_t end = {0, 0};
        pointf_t r = {rx, ry};
        pointf_t from = {last_x, last_y};
        pointf_t to = {x, y};
        if (*command == 'a') {
          to.x += last_x;
          to.y += last_y;
        }
        arc_info_init(&info, from, to, r, rotation, large_arc, sweep);
        while (arc_info_next(&info, &cp1, &cp2, &end)) {
          memset(&path, 0, sizeof(path));
          svg_path_curve_to_init(&path, cp1.x, cp1.y, cp2.x, cp2.y, end.x, end.y);
          parser->on_path(parser->ctx, (svg_path_t*)&path);
        }

        last_x = to.x;
        last_y = to.y;
        s += n;
      } while (tk_sscanf(s, "%f %f %f %f %f %f %f %n", &rx, &ry, &rotation, &large_arc, &sweep, &x,
                         &y, &n) == 7);

    } else {
      log_error("parse failed at \"%s\"\n", s);
      return RET_FAIL;
    }
  }

  return RET_OK;
}

static ret_t svg_path_parser_init(svg_path_parser_t* parser, const char* path, void* ctx,
                                  tk_visit_t on_path) {
  memset(parser, 0x00, sizeof(*parser));

  parser->ctx = ctx;
  parser->path = (char*)path;
  parser->on_path = on_path;

  return RET_OK;
}

ret_t svg_path_parse(const char* path, void* ctx, tk_visit_t on_path) {
  ret_t ret;
  char* p = NULL;
  svg_path_parser_t parser;
  return_value_if_fail(path != NULL && on_path != NULL, RET_BAD_PARAMS);

  p = tk_strdup(path);
  return_value_if_fail(p != NULL, RET_OOM);

  svg_path_parser_init(&parser, p, ctx, on_path);
  ret = svg_path_parser_parse(&parser);
  TKMEM_FREE(p);
  return ret;
}

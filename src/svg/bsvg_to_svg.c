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

#include "tkc/str.h"
#include "tkc/utils.h"
#include "svg/bsvg.h"
#include "svg/bsvg_to_svg.h"

typedef struct _ctx_info_t {
  str_t* str;
  const svg_shape_t* shape;
} ctx_info_t;

ret_t bsvg_to_svg_path(str_t* str, const svg_path_t* path) {
  char buff[128];

  switch (path->type) {
    case SVG_PATH_M: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " M%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_M_REL: {
      const svg_path_move_t* p = (const svg_path_move_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " m%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_L: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " L%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_L_REL: {
      const svg_path_line_t* p = (const svg_path_line_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " l%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_H: {
      const svg_path_hline_t* p = (const svg_path_hline_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " H%.1f", p->x);
      break;
    }
    case SVG_PATH_H_REL: {
      const svg_path_hline_t* p = (const svg_path_hline_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " h%.1f", p->x);
      break;
    }
    case SVG_PATH_V: {
      const svg_path_vline_t* p = (const svg_path_vline_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " V%.1f", p->y);
      break;
    }
    case SVG_PATH_V_REL: {
      const svg_path_vline_t* p = (const svg_path_vline_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " v%.1f", p->y);
      break;
    }
    case SVG_PATH_C: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " C%.1f %.1f %.1f %.1f %.1f %.1f", p->x1, p->y1, p->x2,
                  p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_C_REL: {
      const svg_path_curve_to_t* p = (const svg_path_curve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " c%.1f %.1f %.1f %.1f %.1f %.1f", p->x1, p->y1, p->x2,
                  p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_S: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " S%.1f %.1f %.1f %.1f", p->x2, p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_S_REL: {
      const svg_path_scurve_to_t* p = (const svg_path_scurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " s%.1f %.1f %.1f %.1f", p->x2, p->y2, p->x, p->y);
      break;
    }
    case SVG_PATH_Q: {
      const svg_path_qcurve_to_t* p = (const svg_path_qcurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " Q%.1f %.1f %.1f %.1f", p->x1, p->y1, p->x, p->y);
      break;
    }
    case SVG_PATH_Q_REL: {
      const svg_path_qcurve_to_t* p = (const svg_path_qcurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " q%.1f %.1f %.1f %.1f", p->x1, p->y1, p->x, p->y);
      break;
    }
    case SVG_PATH_T: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " T%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_T_REL: {
      const svg_path_tcurve_to_t* p = (const svg_path_tcurve_to_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " t%.1f %.1f", p->x, p->y);
      break;
    }
    case SVG_PATH_A: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " A%.1f %.1f %.1f %d %d %.1f %.1f", p->rx, p->ry,
                  p->rotation, p->large_arc, p->sweep, p->x, p->y);
      break;
    }
    case SVG_PATH_A_REL: {
      const svg_path_arc_t* p = (const svg_path_arc_t*)path;
      tk_snprintf(buff, sizeof(buff) - 1, " a%.1f %.1f %.1f %d %d %.1f %.1f", p->rx, p->ry,
                  p->rotation, p->large_arc, p->sweep, p->x, p->y);
      break;
    }
    case SVG_PATH_Z: {
      tk_snprintf(buff, sizeof(buff) - 1, "z");
      break;
    }
    case SVG_PATH_NULL:
    default: { break; }
  }

  return str_append(str, buff);
}

ret_t bsvg_to_svg_shape_end(str_t* str, const svg_shape_t* shape) {
  char buff[128];

  if (shape->line_cap) {
    const char* line_cap = "butt";
    if (shape->line_cap == 'r') {
      line_cap = "round";
    } else if (shape->line_cap == 's') {
      line_cap = "square";
    }

    tk_snprintf(buff, sizeof(buff) - 1, " stroke-linecap=\"%s\"", line_cap);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  if (shape->line_join) {
    const char* line_join = "miter";
    if (shape->line_join == 'r') {
      line_join = "round";
    } else if (shape->line_join == 'b') {
      line_join = "bevel";
    }

    tk_snprintf(buff, sizeof(buff) - 1, " stroke-linejoin=\"%s\"", line_join);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  if (shape->stroke_width) {
    tk_snprintf(buff, sizeof(buff) - 1, " stroke-width==\"%d\"", shape->stroke_width);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  if (shape->fill.rgba.a) {
    const rgba_t* rgba = &(shape->fill.rgba);
    tk_snprintf(buff, sizeof(buff) - 1, " fill=\"rgba(%d,%d,%d,%.2f)\"", (int)(rgba->r),
                (int)(rgba->g), (int)(rgba->b), (float)rgba->a / 255.0f);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  if (shape->stroke.rgba.a) {
    const rgba_t* rgba = &(shape->stroke.rgba);
    tk_snprintf(buff, sizeof(buff) - 1, " stroke=\"rgba(%d,%d,%d,%.2f)\"", (int)(rgba->r),
                (int)(rgba->g), (int)(rgba->b), (float)rgba->a / 255.0f);
    return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);
  }

  tk_snprintf(buff, sizeof(buff) - 1, "/>\n");
  return_value_if_fail(str_append(str, buff) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t bsvg_to_svg_shape(str_t* str, const svg_shape_t* shape) {
  char buff[128];
  switch (shape->type) {
    case SVG_SHAPE_RECT: {
      svg_shape_rect_t* s = (svg_shape_rect_t*)shape;
      tk_snprintf(
          buff, sizeof(buff) - 1,
          "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.1f\" height=\"%.1f\" rx=\"%.1f\" ry=\"%.1f\"",
          s->x, s->y, s->w, s->h, s->r, s->r);
      str_append(str, buff);
      break;
    }
    case SVG_SHAPE_CIRCLE: {
      svg_shape_circle_t* s = (svg_shape_circle_t*)shape;
      tk_snprintf(buff, sizeof(buff) - 1, "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"%.1f\"", s->cx,
                  s->cy, s->r);
      str_append(str, buff);
      break;
    }
    case SVG_SHAPE_ELLIPSE: {
      svg_shape_ellipse_t* s = (svg_shape_ellipse_t*)shape;
      tk_snprintf(buff, sizeof(buff) - 1,
                  "<ellipse cx=\"%.1f\" cy=\"%.1f\" rx=\"%.1f\" ry=\"%.1f\"", s->cx, s->cy, s->rx,
                  s->ry);
      str_append(str, buff);
      break;
    }
    case SVG_SHAPE_LINE: {
      svg_shape_line_t* s = (svg_shape_line_t*)shape;
      log_debug("line(%f %f %f %f)\n", s->x1, s->y1, s->x2, s->y2);
      tk_snprintf(buff, sizeof(buff) - 1, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"",
                  s->x1, s->y1, s->x2, s->y2);
      str_append(str, buff);
      break;
    }
    case SVG_SHAPE_POLYGON: {
      uint32_t i = 0;
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;

      str_append(str, "<polygon points=\"");
      for (i = 0; i < s->nr; i++) {
        tk_snprintf(buff, sizeof(buff) - 1, "%.1f ", s->data[i]);
        str_append(str, buff);
      }
      str_append_char(str, '\"');

      break;
    }
    case SVG_SHAPE_POLYLINE: {
      uint32_t i = 0;
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;

      str_append(str, "<polyline points=\"");
      for (i = 0; i < s->nr; i++) {
        tk_snprintf(buff, sizeof(buff) - 1, "%.1f ", s->data[i]);
        str_append(str, buff);
      }
      str_append_char(str, '\"');
      break;
    }
    case SVG_SHAPE_PATH: {
      str_append(str, "<path d=\"");
      return RET_OK;
    }
    case SVG_SHAPE_NULL: {
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_OK;
    }
  }

  return bsvg_to_svg_shape_end(str, shape);
}

static ret_t bsvg_to_svg_on_path(void* ctx, const void* data) {
  ctx_info_t* info = (ctx_info_t*)ctx;
  const svg_path_t* path = (const svg_path_t*)data;

  if (path->type == SVG_PATH_NULL) {
    str_append_char(info->str, '\"');
    bsvg_to_svg_shape_end(info->str, info->shape);
  } else {
    bsvg_to_svg_path(info->str, path);
  }

  return RET_OK;
}

static ret_t bsvg_to_svg_on_shape(void* ctx, const void* data) {
  ctx_info_t* info = (ctx_info_t*)ctx;
  const svg_shape_t* shape = (const svg_shape_t*)data;

  info->shape = shape;
  bsvg_to_svg_shape(info->str, shape);

  return RET_OK;
}

ret_t bsvg_to_svg(const uint32_t* data, uint32_t size, str_t* str) {
  bsvg_t svg;
  ctx_info_t ctx;
  return_value_if_fail(bsvg_init(&svg, data, size) != NULL, RET_BAD_PARAMS);

  ctx.str = str;
  ctx.shape = NULL;

  return bsvg_visit(&svg, &ctx, bsvg_to_svg_on_shape, bsvg_to_svg_on_path);
}

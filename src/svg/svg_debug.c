/**
 * File:   svg_debug.c
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

#include "svg/bsvg.h"
#include "svg/svg_shape.h"
#include "svg/svg_debug.h"

ret_t svg_path_dump(void* ctx, const svg_path_t* path) {
  uint32_t size = 0;
  switch (path->type) {
    case SVG_PATH_M:
    case SVG_PATH_M_REL: {
      size = sizeof(svg_path_move_t);
      break;
    }
    case SVG_PATH_L:
    case SVG_PATH_L_REL: {
      size = sizeof(svg_path_line_t);
      break;
    }
    case SVG_PATH_H:
    case SVG_PATH_H_REL: {
      size = sizeof(svg_path_hline_t);
      break;
    }
    case SVG_PATH_V:
    case SVG_PATH_V_REL: {
      size = sizeof(svg_path_vline_t);
      break;
    }
    case SVG_PATH_C:
    case SVG_PATH_C_REL: {
      size = sizeof(svg_path_curve_to_t);
      break;
    }
    case SVG_PATH_S:
    case SVG_PATH_S_REL: {
      size = sizeof(svg_path_scurve_to_t);
      break;
    }
    case SVG_PATH_Q:
    case SVG_PATH_Q_REL: {
      size = sizeof(svg_path_qcurve_to_t);
      break;
    }
    case SVG_PATH_T:
    case SVG_PATH_T_REL: {
      size = sizeof(svg_path_tcurve_to_t);
      break;
    }
    case SVG_PATH_A:
    case SVG_PATH_A_REL: {
      size = sizeof(svg_path_arc_t);
      break;
    }
    case SVG_PATH_NULL:
    case SVG_PATH_Z: {
      size = sizeof(svg_path_t);
      break;
    }
    default: {
      assert(!"not supported");
      break;
    }
  }

  return RET_OK;
}

ret_t svg_shape_dump(svg_shape_t* shape) {
  return_value_if_fail(shape != NULL, RET_BAD_PARAMS);

  log_debug("[type=%d fill=0x%08X stroke=0x%08X line_width=%d line_cap=%d line_join=%d]) ",
            (int)(shape->type), shape->fill.color, shape->stroke.color, (int)(shape->stroke_width),
            (int)(shape->line_cap), (int)(shape->line_join));

  switch (shape->type) {
    case SVG_SHAPE_RECT: {
      svg_shape_rect_t* s = (svg_shape_rect_t*)shape;
      log_debug("rect(%f %f %f %f %f);\n", s->x, s->y, s->w, s->h, s->r);
      break;
    }
    case SVG_SHAPE_CIRCLE: {
      svg_shape_circle_t* s = (svg_shape_circle_t*)shape;
      log_debug("circle(%f %f %f);\n", s->cx, s->cy, s->r);
      break;
    }
    case SVG_SHAPE_ELLIPSE: {
      svg_shape_ellipse_t* s = (svg_shape_ellipse_t*)shape;
      log_debug("ellipse(%f %f %f %f)\n", s->cx, s->cy, s->rx, s->ry);

      break;
    }
    case SVG_SHAPE_LINE: {
      svg_shape_line_t* s = (svg_shape_line_t*)shape;
      log_debug("line(%f %f %f %f)\n", s->x1, s->y1, s->x2, s->y2);
      break;
    }
    case SVG_SHAPE_POLYGON: {
      uint32_t i = 0;
      svg_shape_polygon_t* s = (svg_shape_polygon_t*)shape;

      log_debug("polygon(");
      for (i = 0; i < s->nr; i++) {
        log_debug("%f ", s->data[i]);
      }
      log_debug(")\n");

      break;
    }
    case SVG_SHAPE_POLYLINE: {
      uint32_t i = 0;
      svg_shape_polyline_t* s = (svg_shape_polyline_t*)shape;
      log_debug("polyline(");
      for (i = 0; i < s->nr; i++) {
        log_debug("%f ", s->data[i]);
      }
      log_debug(")\n");
      break;
    }
    case SVG_SHAPE_PATH: {
      svg_shape_path_t* s = (svg_shape_path_t*)shape;
      bsvg_visit_path((const uint8_t*)(s->path), NULL, (tk_visit_t)svg_path_dump);
      break;
    }
    default: {
      assert(!"not supported type");
      break;
    }
  }

  return RET_OK;
}

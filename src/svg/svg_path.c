/**
 * File:   svg_path.c
 * Author: AWTK Develop Team
 * Brief:  svg path
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
 * 2018-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "svg/svg_path.h"

svg_path_t* svg_path_move_init(svg_path_move_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_M;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_move_rel_init(svg_path_move_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_M_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_line_init(svg_path_line_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_L;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_line_rel_init(svg_path_line_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_L_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_hline_init(svg_path_hline_t* path, float x) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->path.type = SVG_PATH_H;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_hline_rel_init(svg_path_hline_t* path, float x) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->path.type = SVG_PATH_H_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_vline_init(svg_path_vline_t* path, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->y = y;
  path->path.type = SVG_PATH_V;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_vline_rel_init(svg_path_vline_t* path, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->y = y;
  path->path.type = SVG_PATH_V_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_curve_to_init(svg_path_curve_to_t* path, float x1, float y1, float x2,
                                   float y2, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->x1 = x1;
  path->y1 = y1;
  path->x2 = x2;
  path->y2 = y2;
  path->path.type = SVG_PATH_C;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_curve_to_rel_init(svg_path_curve_to_t* path, float x1, float y1, float x2,
                                       float y2, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  svg_path_curve_to_init(path, x1, y1, x2, y2, x, y);
  path->path.type = SVG_PATH_C_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_scurve_to_init(svg_path_scurve_to_t* path, float x2, float y2, float x,
                                    float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->x2 = x2;
  path->y2 = y2;
  path->path.type = SVG_PATH_S;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_scurve_to_rel_init(svg_path_scurve_to_t* path, float x2, float y2, float x,
                                        float y) {
  return_value_if_fail(path != NULL, NULL);

  svg_path_scurve_to_init(path, x2, y2, x, y);
  path->path.type = SVG_PATH_S_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_qcurve_to_init(svg_path_qcurve_to_t* path, float x1, float y1, float x,
                                    float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->x1 = x1;
  path->y1 = y1;
  path->path.type = SVG_PATH_Q;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_qcurve_to_rel_init(svg_path_qcurve_to_t* path, float x1, float y1, float x,
                                        float y) {
  return_value_if_fail(path != NULL, NULL);

  svg_path_qcurve_to_init(path, x1, y1, x, y);
  path->path.type = SVG_PATH_Q_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_tcurve_to_init(svg_path_tcurve_to_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_T;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_tcurve_to_rel_init(svg_path_tcurve_to_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  svg_path_tcurve_to_init(path, x, y);
  path->path.type = SVG_PATH_T_REL;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_arc_init(svg_path_arc_t* path, float rx, float ry, float rotation,
                              bool_t large_arc, bool_t sweep, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->rx = rx;
  path->ry = ry;
  path->sweep = sweep;
  path->rotation = rotation;
  path->large_arc = large_arc;
  path->path.type = SVG_PATH_A;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_arc_rel_init(svg_path_arc_t* path, float rx, float ry, float rotation,
                                  bool_t large_arc, bool_t sweep, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  svg_path_arc_init(path, rx, ry, rotation, large_arc, sweep, x, y);
  path->path.type = SVG_PATH_A_REL;

  return (svg_path_t*)path;
}

uint32_t svg_path_size(const svg_path_t* path) {
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

  return size;
}

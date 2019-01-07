/**
 * File:   svg_path.h
 * Author: AWTK Develop Team
 * Brief:  svg path
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_SVG_PATH_H
#define TK_SVG_PATH_H

#include "tkc/color.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

#pragma pack(push, 1)
typedef struct _svg_path_t {
  uint32_t type;
} svg_path_t;

typedef enum _svg_path_type_t {
  SVG_PATH_NULL = 0,
  SVG_PATH_M,
  SVG_PATH_M_REL,
  SVG_PATH_L,
  SVG_PATH_L_REL,
  SVG_PATH_H,
  SVG_PATH_H_REL,
  SVG_PATH_V,
  SVG_PATH_V_REL,
  SVG_PATH_C,
  SVG_PATH_C_REL,
  SVG_PATH_S,
  SVG_PATH_S_REL,
  SVG_PATH_Q,
  SVG_PATH_Q_REL,
  SVG_PATH_T,
  SVG_PATH_T_REL,
  SVG_PATH_A,
  SVG_PATH_A_REL,
  SVG_PATH_Z,
} svg_path_type_t;

typedef struct _svg_path_move_t {
  svg_path_t path;
  float_t x;
  float_t y;
} svg_path_move_t;

typedef struct _svg_path_line_t {
  svg_path_t path;
  float_t x;
  float_t y;
} svg_path_line_t;

typedef struct _svg_path_hline_t {
  svg_path_t path;
  float_t x;
} svg_path_hline_t;

typedef struct _svg_path_vline_t {
  svg_path_t path;
  float_t y;
} svg_path_vline_t;

typedef struct _svg_path_curve_to_t {
  svg_path_t path;
  float_t x1;
  float_t y1;
  float_t x2;
  float_t y2;
  float_t x;
  float_t y;
} svg_path_curve_to_t;

typedef struct _svg_path_scurve_to_t {
  svg_path_t path;
  float_t x2;
  float_t y2;
  float_t x;
  float_t y;
} svg_path_scurve_to_t;

typedef struct _svg_path_qcurve_to_t {
  svg_path_t path;
  float_t x1;
  float_t y1;
  float_t x;
  float_t y;
} svg_path_qcurve_to_t;

typedef struct _svg_path_tcurve_to_t {
  svg_path_t path;
  float_t x;
  float_t y;
} svg_path_tcurve_to_t;

typedef struct _svg_path_arc_t {
  svg_path_t path;
  float_t rx;
  float_t ry;
  float_t rotation;
  uint16_t large_arc;
  uint16_t sweep;
  float_t x;
  float_t y;
} svg_path_arc_t;
#pragma pack(pop)

uint32_t svg_path_size(const svg_path_t* path);

svg_path_t* svg_path_move_init(svg_path_move_t* path, float_t x, float_t y);
svg_path_t* svg_path_move_rel_init(svg_path_move_t* path, float_t x, float_t y);

svg_path_t* svg_path_line_init(svg_path_line_t* path, float_t x, float_t y);
svg_path_t* svg_path_line_rel_init(svg_path_line_t* path, float_t x, float_t y);

svg_path_t* svg_path_hline_init(svg_path_hline_t* path, float_t x);
svg_path_t* svg_path_hline_rel_init(svg_path_hline_t* path, float_t x);

svg_path_t* svg_path_vline_init(svg_path_vline_t* path, float_t y);
svg_path_t* svg_path_vline_rel_init(svg_path_vline_t* path, float_t y);

svg_path_t* svg_path_curve_to_init(svg_path_curve_to_t* path, float_t x1, float_t y1, float_t x2,
                                   float_t y2, float_t x, float_t y);

svg_path_t* svg_path_curve_to_rel_init(svg_path_curve_to_t* path, float_t x1, float_t y1,
                                       float_t x2, float_t y2, float_t x, float_t y);

svg_path_t* svg_path_scurve_to_init(svg_path_scurve_to_t* path, float_t x2, float_t y2, float_t x,
                                    float_t y);

svg_path_t* svg_path_scurve_to_rel_init(svg_path_scurve_to_t* path, float_t x2, float_t y2,
                                        float_t x, float_t y);

svg_path_t* svg_path_qcurve_to_init(svg_path_qcurve_to_t* path, float_t x1, float_t y1, float_t x,
                                    float_t y);

svg_path_t* svg_path_qcurve_to_rel_init(svg_path_qcurve_to_t* path, float_t x1, float_t y1,
                                        float_t x, float_t y);

svg_path_t* svg_path_tcurve_to_init(svg_path_tcurve_to_t* path, float_t x, float_t y);

svg_path_t* svg_path_tcurve_to_rel_init(svg_path_tcurve_to_t* path, float_t x, float_t y);

svg_path_t* svg_path_arc_init(svg_path_arc_t* path, float_t rx, float_t ry, float_t rotation,
                              bool_t large_arc, bool_t sweep, float_t x, float_t y);

svg_path_t* svg_path_arc_rel_init(svg_path_arc_t* path, float_t rx, float_t ry, float_t rotation,
                                  bool_t large_arc, bool_t sweep, float_t x, float_t y);

END_C_DECLS

#endif /*TK_SVG_PATH_H*/

/**
 * File:   svg_path.h
 * Author: AWTK Develop Team
 * Brief:  svg path
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
 * 2018-11-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SVG_PATH_H
#define TK_SVG_PATH_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

#pragma pack(push, 1)
typedef struct _svg_path_t {
  uint8_t type;
  uint8_t reserved[3];
} svg_path_t;

typedef enum _svg_path_type_t {
  SVG_PATH_NULL = 0,
  SVG_PATH_M,
  SVG_PATH_L,
  SVG_PATH_C,
  SVG_PATH_Z,
} svg_path_type_t;

typedef struct _svg_path_move_t {
  svg_path_t path;
  float x;
  float y;
} svg_path_move_t;

typedef struct _svg_path_line_t {
  svg_path_t path;
  float x;
  float y;
} svg_path_line_t;

typedef struct _svg_path_curve_to_t {
  svg_path_t path;
  float x1;
  float y1;
  float x2;
  float y2;
  float x;
  float y;
} svg_path_curve_to_t;
#pragma pack(pop)

uint32_t svg_path_size(const svg_path_t* path);

svg_path_t* svg_path_move_init(svg_path_move_t* path, float x, float y);

svg_path_t* svg_path_line_init(svg_path_line_t* path, float x, float y);

svg_path_t* svg_path_curve_to_init(svg_path_curve_to_t* path, float x1, float y1, float x2,
                                   float y2, float x, float y);

END_C_DECLS

#endif /*TK_SVG_PATH_H*/

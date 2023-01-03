/**
 * File:   svg_path.c
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

#include "svg/svg_path.h"

svg_path_t* svg_path_move_init(svg_path_move_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_M;

  return (svg_path_t*)path;
}

svg_path_t* svg_path_line_init(svg_path_line_t* path, float x, float y) {
  return_value_if_fail(path != NULL, NULL);

  path->x = x;
  path->y = y;
  path->path.type = SVG_PATH_L;

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

uint32_t svg_path_size(const svg_path_t* path) {
  uint32_t size = 0;
  switch (path->type) {
    case SVG_PATH_M: {
      size = sizeof(svg_path_move_t);
      break;
    }
    case SVG_PATH_L: {
      size = sizeof(svg_path_line_t);
      break;
    }
    case SVG_PATH_C: {
      size = sizeof(svg_path_curve_to_t);
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

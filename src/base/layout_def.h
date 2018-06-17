/**
 * File:   layout_def.h
 * Author: AWTK Develop Team
 * Brief:  layout type def
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYOUT_DEF_H
#define TK_LAYOUT_DEF_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef enum _x_attr_t { X_ATTR_DEFAULT = 0, X_ATTR_PERCENT, X_ATTR_CENTER, X_ATTR_RIGHT } x_attr_t;

typedef enum _y_attr_t {
  Y_ATTR_DEFAULT = 0,
  Y_ATTR_PERCENT,
  Y_ATTR_MIDDLE,
  Y_ATTR_BOTTOM
} y_attr_t;

typedef enum _w_attr_t { W_ATTR_PIXEL = 0, W_ATTR_PERCENT, W_ATTR_FILL } w_attr_t;

typedef enum _h_attr_t { H_ATTR_PIXEL = 0, H_ATTR_PERCENT, H_ATTR_FILL } h_attr_t;

typedef struct _widget_layout_t {
  uint8_t x_attr;
  uint8_t y_attr;
  uint8_t w_attr;
  uint8_t h_attr;
  xy_t x;
  xy_t y;
  wh_t w;
  wh_t h;
  bool_t inited;
} widget_layout_t;

typedef struct _children_layout_t {
  uint16_t rows;
  uint16_t cols;
  uint8_t x_margin;
  uint8_t y_margin;
  uint8_t spacing;
  uint16_t cols_is_width : 1;
  uint16_t rows_is_height : 1;
  bool_t inited;
} children_layout_t;

typedef struct _layout_params_t {
  widget_layout_t self;
  children_layout_t children;
} layout_params_t;

END_C_DECLS

#endif /*TK_LAYOUT_DEF_H*/

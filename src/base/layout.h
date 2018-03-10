/**
 * File:   layout.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  layout
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_LAYOUT_H
#define LFTK_LAYOUT_H

#include "base/widget.h"

BEGIN_C_DECLS


typedef enum _x_attr_t {
  X_ATTR_DEFAULT = 0,
  X_ATTR_PERCENT,
  X_ATTR_CENTER,
  X_ATTR_RIGHT
}x_attr_t;

typedef enum _y_attr_t {
  Y_ATTR_DEFAULT = 0,
  Y_ATTR_PERCENT,
  Y_ATTR_MIDDLE,
  Y_ATTR_BOTTOM
}y_attr_t;

typedef enum _w_attr_t {
  W_ATTR_PIXEL = 0,
  W_ATTR_PERCENT
}w_attr_t;

typedef enum _h_attr_t {
  H_ATTR_PIXEL = 0,
  H_ATTR_PERCENT
}h_attr_t;

typedef struct _widget_layout_t {
  uint8_t x_attr;
  uint8_t y_attr;
  uint8_t w_attr;
  uint8_t h_attr;
  xy_t x;
  xy_t y;
  wh_t w;
  wh_t h;
}widget_layout_t;

typedef struct _children_layout_t {
  uint8_t rows;
  uint8_t cols;
  uint8_t margin;
  uint8_t cell_h_margin;
  uint8_t cell_v_margin;
}children_layout_t;

typedef struct _layout_params_t {
  widget_layout_t self;
  children_layout_t children;
}layout_params_t;

ret_t widget_layout_calc(const widget_layout_t* layout, rect_t* r, wh_t parent_w, wh_t parent_h);
widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y, const char* w, const char* h);

ret_t widget_set_self_layout_params(widget_t* widget, const widget_layout_t* layout);
ret_t widget_set_children_layout_params(widget_t* widget, uint8_t rows, uint8_t cols, uint8_t margin, uint8_t cell_h_margin, uint8_t cell_v_margin);

ret_t widget_layout(widget_t* widget);
ret_t widget_layout_self(widget_t* widget);
ret_t widget_layout_children(widget_t* widget);

END_C_DECLS

#endif/*LFTK_LAYOUT_H*/


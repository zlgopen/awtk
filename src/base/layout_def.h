/**
 * File:   layout_def.h
 * Author: AWTK Develop Team
 * Brief:  layout type def
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYOUT_DEF_H
#define TK_LAYOUT_DEF_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef enum _x_attr_t {
  X_ATTR_DEFAULT = 0,
  X_ATTR_PERCENT,
  X_ATTR_CENTER,
  X_ATTR_RIGHT,
  X_ATTR_CENTER_PERCENT,
  X_ATTR_RIGHT_PERCENT,
  X_ATTR_UNDEF
} x_attr_t;

typedef enum _y_attr_t {
  Y_ATTR_DEFAULT = 0,
  Y_ATTR_PERCENT,
  Y_ATTR_MIDDLE,
  Y_ATTR_BOTTOM,
  Y_ATTR_MIDDLE_PERCENT,
  Y_ATTR_BOTTOM_PERCENT,
  Y_ATTR_UNDEF
} y_attr_t;

typedef enum _w_attr_t { W_ATTR_PIXEL = 0, W_ATTR_PERCENT, W_ATTR_UNDEF } w_attr_t;

typedef enum _h_attr_t { H_ATTR_PIXEL = 0, H_ATTR_PERCENT, H_ATTR_UNDEF } h_attr_t;

END_C_DECLS

#endif /*TK_LAYOUT_DEF_H*/

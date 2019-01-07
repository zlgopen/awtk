/**
 * File:   self_layouter_default.h
 * Author: AWTK Develop Team
 * Brief:  self layouter default
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SELF_LAYOUTER_DEFAULT_H
#define TK_SELF_LAYOUTER_DEFAULT_H

#include "base/self_layouter.h"

BEGIN_C_DECLS

typedef struct _self_layouter_default_t {
  self_layouter_t layouter;
  uint8_t x_attr;
  uint8_t y_attr;
  uint8_t w_attr;
  uint8_t h_attr;
  xy_t x;
  xy_t y;
  wh_t w;
  wh_t h;
} self_layouter_default_t;

self_layouter_t* self_layouter_default_create(void);

bool_t self_layouter_default_is_valid(self_layouter_t* layouter);
ret_t widget_layout_self_with_rect(self_layouter_t* layouter, widget_t* widget, rect_t* area);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_DEFAULT_H*/

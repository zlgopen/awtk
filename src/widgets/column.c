/**
 * File:   column.h
 * Author: AWTK Develop Team
 * Brief:  column
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/column.h"

TK_DECL_VTABLE(column) = {.size = sizeof(column_t),
                          .type = WIDGET_TYPE_COLUMN,
                          .parent = TK_PARENT_VTABLE(widget),
                          .create = column_create};

widget_t* column_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(column), x, y, w, h);
}

widget_t* column_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, column), NULL);

  return widget;
}

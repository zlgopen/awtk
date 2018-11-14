/**
 * File:   column.h
 * Author: AWTK Develop Team
 * Brief:  column
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/column.h"

static const widget_vtable_t s_column_vtable = {
    .size = sizeof(column_t), .type = WIDGET_TYPE_COLUMN, .create = column_create};

widget_t* column_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  column_t* column = TKMEM_ZALLOC(column_t);
  widget_t* widget = WIDGET(column);
  return_value_if_fail(column != NULL, NULL);

  return widget_init(widget, parent, &s_column_vtable, x, y, w, h);
}

widget_t* column_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_column_vtable, NULL);

  return widget;
}

/**
 * File:   row.h
 * Author: AWTK Develop Team
 * Brief:  row
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/row.h"

static const widget_vtable_t s_row_vtable = {
    .size = sizeof(row_t), .type = WIDGET_TYPE_ROW, .create = row_create};

widget_t* row_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, &s_row_vtable, x, y, w, h);
}

widget_t* row_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_row_vtable, NULL);

  return widget;
}

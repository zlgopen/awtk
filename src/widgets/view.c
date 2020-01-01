/**
 * File:   view.h
 * Author: AWTK Develop Team
 * Brief:  view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/view.h"

TK_DECL_VTABLE(view) = {.size = sizeof(view_t),
                        .type = WIDGET_TYPE_VIEW,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = view_create};

widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(view), x, y, w, h);
}

widget_t* view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, view), NULL);

  return widget;
}

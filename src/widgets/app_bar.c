/**
 * File:   app_bar.h
 * Author: AWTK Develop Team
 * Brief:  app_bar
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
#include "widgets/app_bar.h"
#include "base/widget_vtable.h"

TK_DECL_VTABLE(app_bar) = {.size = sizeof(app_bar_t),
                           .type = WIDGET_TYPE_APP_BAR,
                           .parent = TK_PARENT_VTABLE(widget),
                           .create = app_bar_create};

widget_t* app_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(app_bar), x, y, w, h);
}

widget_t* app_bar_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, app_bar), NULL);

  return widget;
}

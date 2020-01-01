/**
 * File:   tab_control.h
 * Author: AWTK Develop Team
 * Brief:  tab_control
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/tab_control.h"

static ret_t tab_control_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

TK_DECL_VTABLE(tab_control) = {.size = sizeof(tab_control_t),
                               .type = WIDGET_TYPE_TAB_CONTROL,
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = tab_control_create,
                               .on_paint_self = tab_control_on_paint_self};

widget_t* tab_control_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(tab_control), x, y, w, h);
}

widget_t* tab_control_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, tab_control), NULL);

  return widget;
}

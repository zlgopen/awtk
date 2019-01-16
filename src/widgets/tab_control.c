/**
 * File:   tab_control.h
 * Author: AWTK Develop Team
 * Brief:  tab_control
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/tab_control.h"

static ret_t tab_control_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_tab_control_vtable = {.size = sizeof(tab_control_t),
                                                     .type = WIDGET_TYPE_TAB_CONTROL,
                                                     .create = tab_control_create,
                                                     .on_paint_self = tab_control_on_paint_self};

widget_t* tab_control_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, &s_tab_control_vtable, x, y, w, h);
}

widget_t* tab_control_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_tab_control_vtable, NULL);

  return widget;
}

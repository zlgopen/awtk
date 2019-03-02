/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "widgets/window.h"

static const char* s_window_properties[] = {WIDGET_PROP_ANIM_HINT, WIDGET_PROP_OPEN_ANIM_HINT,
                                            WIDGET_PROP_CLOSE_ANIM_HINT, WIDGET_PROP_THEME, NULL};

TK_DECL_VTABLE(window) = {.type = WIDGET_TYPE_NORMAL_WINDOW,
                          .size = sizeof(window_t),
                          .is_window = TRUE,
                          .parent = TK_PARENT_VTABLE(window_base),
                          .create = window_create,
                          .clone_properties = s_window_properties,
                          .persistent_properties = s_window_properties,
                          .on_event = window_base_on_event,
                          .on_paint_self = window_base_on_paint_self,
                          .on_paint_begin = window_base_on_paint_begin,
                          .on_paint_end = window_base_on_paint_end,
                          .set_prop = window_base_set_prop,
                          .get_prop = window_base_get_prop,
                          .on_destroy = window_base_on_destroy};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(window), x, y, w, h);
}

widget_t* window_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, window), NULL);

  return widget;
}

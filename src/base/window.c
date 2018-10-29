/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/enums.h"
#include "base/window.h"

static const char* s_window_properties[] = {WIDGET_PROP_ANIM_HINT,       WIDGET_PROP_OPEN_ANIM_HINT,
                                            WIDGET_PROP_CLOSE_ANIM_HINT, WIDGET_PROP_THEME,
                                            WIDGET_PROP_SCRIPT,          NULL};

static const widget_vtable_t s_window_vtable = {.type = WIDGET_TYPE_NORMAL_WINDOW,
                                                .clone_properties = s_window_properties,
                                                .persistent_properties = s_window_properties,
                                                .on_event = window_base_on_event,
                                                .on_paint_self = window_base_on_paint_self,
                                                .on_paint_begin = window_base_on_paint_begin,
                                                .on_paint_end = window_base_on_paint_end,
                                                .set_prop = window_base_set_prop,
                                                .get_prop = window_base_get_prop,
                                                .destroy = window_base_destroy};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  window_t* win = TKMEM_ZALLOC(window_t);
  widget_t* widget = WIDGET(win);

  return window_base_init(widget, parent, &s_window_vtable, x, y, w, h);
}

widget_t* window_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_window_vtable, NULL);

  return widget;
}

/**
 * File:   system_bar.c
 * Author: AWTK Develop Team
 * Brief:  system_bar
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
 * 2018-12-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "widgets/label.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/window.h"
#include "widgets/system_bar.h"
#include "base/window_manager.h"

static const char* s_system_bar_properties[] = {WIDGET_PROP_THEME, NULL};

static ret_t system_bar_on_request_close_window(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* top_window = window_manager_get_top_main_window(widget->parent);

  if (top_window != NULL) {
    event_t e = event_init(EVT_REQUEST_CLOSE_WINDOW, top_window);
    widget_dispatch(top_window, &e);
  }

  return RET_OK;
}

static ret_t system_bar_update_title(widget_t* widget, widget_t* top_window) {
  widget_t* title = widget_lookup(widget, "title", TRUE);

  return_value_if_fail(top_window != NULL, RET_OK);

  if (title != NULL) {
    if (top_window->tr_text) {
      widget_set_tr_text(title, top_window->tr_text);
    } else if (top_window->text.str) {
      widget_set_text(title, top_window->text.str);
    } else {
      widget_set_text_utf8(title, top_window->name);
    }
  }

  return RET_OK;
}

static ret_t system_bar_update_close(widget_t* widget, widget_t* top_window) {
  widget_t* close = widget_lookup(widget, "close", TRUE);
  return_value_if_fail(top_window != NULL, RET_OK);

  if (close != NULL) {
    int32_t closable = widget_get_prop_int(top_window, WIDGET_PROP_CLOSABLE, WINDOW_CLOSABLE_NO);
    widget_set_enable(close, closable != WINDOW_CLOSABLE_NO);
  }

  return RET_OK;
}

static ret_t system_bar_on_top_window_prop_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  prop_change_event_t* evt = (prop_change_event_t*)e;
  widget_t* top_window = window_manager_get_top_main_window(widget->parent);

  if (top_window != WIDGET(e->target)) {
    return RET_OK;
  }

  if (tk_str_eq(evt->name, WIDGET_PROP_TEXT) || tk_str_eq(evt->name, WIDGET_PROP_NAME)) {
    system_bar_update_title(widget, top_window);
  }

  if (tk_str_eq(evt->name, WIDGET_PROP_CLOSABLE)) {
    system_bar_update_close(widget, top_window);
  }

  return RET_OK;
}

static ret_t system_bar_on_top_window_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  window_event_t* evt = (window_event_t*)e;
  widget_t* top_window = evt->window;

  widget_off_by_ctx(top_window, ctx);
  system_bar_update_title(widget, top_window);
  system_bar_update_close(widget, top_window);
  widget_on(top_window, EVT_PROP_CHANGED, system_bar_on_top_window_prop_changed, ctx);

  return RET_OK;
}

ret_t system_bar_on_event(widget_t* widget, event_t* e) {
  if (e->type == EVT_WINDOW_OPEN) {
    widget_t* close = widget_lookup(widget, "close", TRUE);

    if (close != NULL) {
      widget_on(close, EVT_CLICK, system_bar_on_request_close_window, widget);
    }
  }

  return window_base_on_event(widget, e);
}

static ret_t system_bar_on_destroy(widget_t* widget) {
  widget_t* wm = window_manager();

  if (wm != NULL && wm->emitter != NULL) {
    emitter_off_by_ctx(wm->emitter, widget);
  }

  return window_base_on_destroy(widget);
}

TK_DECL_VTABLE(system_bar) = {.size = sizeof(system_bar_t),
                              .type = WIDGET_TYPE_SYSTEM_BAR,
                              .is_window = TRUE,
                              .clone_properties = s_system_bar_properties,
                              .persistent_properties = s_system_bar_properties,
                              .parent = TK_PARENT_VTABLE(window_base),
                              .create = system_bar_create,
                              .on_event = system_bar_on_event,
                              .set_prop = window_base_set_prop,
                              .get_prop = window_base_get_prop,
                              .on_paint_self = window_base_on_paint_self,
                              .on_paint_begin = window_base_on_paint_begin,
                              .on_paint_end = window_base_on_paint_end,
                              .on_destroy = system_bar_on_destroy};

widget_t* system_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = window_base_create(parent, TK_REF_VTABLE(system_bar), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  widget_on(widget->parent, EVT_TOP_WINDOW_CHANGED, system_bar_on_top_window_changed, widget);

  return widget;
}

TK_DECL_VTABLE(system_bar_bottom) = {.size = sizeof(system_bar_t),
                                     .type = WIDGET_TYPE_SYSTEM_BAR_BOTTOM,
                                     .is_window = TRUE,
                                     .clone_properties = s_system_bar_properties,
                                     .persistent_properties = s_system_bar_properties,
                                     .parent = TK_PARENT_VTABLE(window_base),
                                     .create = system_bar_create,
                                     .on_event = system_bar_on_event,
                                     .set_prop = window_base_set_prop,
                                     .get_prop = window_base_get_prop,
                                     .on_paint_self = window_base_on_paint_self,
                                     .on_paint_begin = window_base_on_paint_begin,
                                     .on_paint_end = window_base_on_paint_end,
                                     .on_destroy = system_bar_on_destroy};

widget_t* system_bar_bottom_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = window_base_create(parent, TK_REF_VTABLE(system_bar_bottom), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  widget_on(widget->parent, EVT_TOP_WINDOW_CHANGED, system_bar_on_top_window_changed, widget);

  return widget;
}

widget_t* system_bar_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, system_bar), NULL);

  return widget;
}

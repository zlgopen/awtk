/**
 * File:   demo_overlay.c
 * Author: AWTK Develop Team
 * Brief:  demo_overlay
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-06-25 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "awtk.h"
#include "ext_widgets.h"

static widget_t* always_on_top_widget = NULL;
static widget_t* highlight_widget = NULL;
static widget_t* timer_open_overlay_widget = NULL;
static widget_t* window_anim_widget = NULL;

static ret_t install_one(void* ctx, const void* iter);
static void open_window(const char* name, widget_t* to_close);


static ret_t timer_open_open_overlay(const timer_info_t* timer) {
  open_window("top", NULL);
  return RET_REMOVE;
}

static ret_t on_quit(void* ctx, event_t* e) {
  widget_t* dialog = WIDGET(ctx);

  dialog_quit(dialog, 0);
  (void)e;
  return RET_OK;
}

static ret_t on_back_to_home(void* ctx, event_t* e) {
  widget_t* dialog = WIDGET(ctx);
  open_window(dialog->name, NULL);
  (void)e;
  return RET_OK;
}

static void open_window(const char* name, widget_t* to_close) {
  widget_t* win = to_close ? window_open_and_close(name, to_close) : window_open(name);
  widget_foreach(win, install_one, win);

  if (tk_str_eq(name, "top")) {
    bool_t always_on_top = widget_get_prop_bool(always_on_top_widget, WIDGET_PROP_VALUE, FALSE);
    widget_set_prop_bool(win, WIDGET_PROP_ALWAYS_ON_TOP, always_on_top);
  } else if (tk_str_eq(name, "dialog1")) {
    bool_t highlight = widget_get_prop_bool(highlight_widget, WIDGET_PROP_VALUE, FALSE);
    bool_t timer_open_overlay = widget_get_prop_bool(timer_open_overlay_widget, WIDGET_PROP_VALUE, FALSE);
    bool_t window_anim = widget_get_prop_bool(window_anim_widget, WIDGET_PROP_VALUE, FALSE);
    
    if (timer_open_overlay) {
      timer_add(timer_open_open_overlay, NULL, 1000);
    }
    if (highlight) {
      widget_set_prop_str(win, WIDGET_PROP_HIGHLIGHT, "default(alpha=100)");
    }
    if (!window_anim) {
      widget_set_prop_str(win, WIDGET_PROP_ANIM_HINT, "");
    }
    if (tk_str_eq(widget_get_type(win), WIDGET_TYPE_DIALOG)) {
      int32_t ret = dialog_modal(win);
    }
  }
}

static ret_t on_context_menu(void* ctx, event_t* e) {
  open_window("menu_point", NULL);

  return RET_OK;
}

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;
  open_window(name, NULL);

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  (void)e;
  return window_close(win);
}

static ret_t install_one(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = widget_get_window(widget);

  if (widget->name != NULL) {
    const char* name = widget->name;
    if (strstr(name, "open:") == name) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
      if (tk_str_eq(name, "open:menu_point")) {
        widget_on(widget, EVT_CONTEXT_MENU, on_context_menu, win);
      }
    } else if (tk_str_eq(name, "always_on_top")) {
      always_on_top_widget = widget;
    } else if (tk_str_eq(name, "highlight")) {
      highlight_widget = widget;
    } else if (tk_str_eq(name, "timer_open_overlay")) {
      timer_open_overlay_widget = widget;
    } else if (tk_str_eq(name, "window_anim")) {
      window_anim_widget = widget;
    } else if (tk_str_eq(name, "quit")) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_quit, win);
      }
    } else if (tk_str_eq(name, "back_to_home")) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_back_to_home, win);
      }
    } else if (tk_str_eq(name, "close")) {
      widget_on(widget, EVT_CLICK, on_close, win);
    }

  }
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("overlay_test");
  widget_foreach(win, install_one, win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"

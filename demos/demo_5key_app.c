/**
 * File:   demo_5key_app.c
 * Author: AWTK Develop Team
 * Brief:  5key demo
 *
 * Copyright (c) 2023 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-11-14 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "awtk.h"

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;

  if (tk_str_eq(name, "confirm")) {
    dialog_confirm(NULL, "Hello AWTK!\nAre you sure to close?");
  } else {
    window_open_and_close(name, NULL);
  }

  (void)e;
  return RET_OK;
}

static ret_t on_open_toast(void* ctx, event_t* e) {
  (void)e;
  dialog_toast((char*)ctx, 1000);
  return RET_OK;
}

static ret_t install_one(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget->name != NULL) {
    const char* name = widget->name;
    if (strstr(name, "open:") == name) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
    } else if (tk_str_eq(name, "slide_button")) {
      widget_on(widget, EVT_CLICK, on_open_toast, (void*)("click slide_button"));
    }
  }
  return RET_OK;
}

static void install_click_hander(widget_t* widget) {
  widget_foreach(widget, install_one, widget);
}

static widget_t* open_window(const char* name) {
  widget_t* win = window_open(name);
  install_click_hander(win);
  return win;
}

ret_t application_init() {
#ifdef WITH_STATE_ACTIVATED
  system_info_set_keyboard_type(system_info(), KEYBOARD_5KEYS);
#else
  log_error("not found WITH_STATE_ACTIVATED macro definiton !!!\r\n");
#endif

  open_window("5key_main");
  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"

static ret_t install_one(void* ctx, const void* iter);

static ret_t on_fullscreen(void* ctx, event_t* e) {
  widget_t* btn = WIDGET(e->target);
  window_t* win = WINDOW(widget_get_window(btn));

  if (win->fullscreen) {
    window_set_fullscreen(WIDGET(win), FALSE);
    widget_set_text_utf8(btn, "Fullscreen");
  } else {
    window_set_fullscreen(WIDGET(win), TRUE);
    widget_set_text_utf8(btn, "Unfullscreen");
  }

  return RET_OK;
}

static ret_t on_click_enlarge(void* ctx, event_t* e) {
  window_manager_resize(window_manager(), 840, 800);

  return RET_OK;
}

static ret_t on_click_win_close(void* ctx, event_t* e) {
  tk_quit();

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  if (widget_is_dialog(win)) {
    dialog_quit(win, DIALOG_QUIT_CANCEL);
  } else {
    window_close(win);
  }

  return RET_OK;
}

static ret_t on_ok(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  if (widget_is_dialog(win)) {
    dialog_quit(win, DIALOG_QUIT_OK);
  }
  return RET_OK;
}

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;
  widget_t* win = window_open(name);
  widget_foreach(win, install_one, win);
  if (tk_str_eq(name, "toast")) {
    dialog_toast("Hello AWTK!\nThis is a toast(1)!", 4000);
  } else if (tk_str_eq(name, "info")) {
    dialog_info("info", "hello awtk");
  } else if (widget_is_dialog(win)) {
    dialog_quit_code_t quit_code = dialog_modal(win);
    switch (quit_code) {
      case DIALOG_QUIT_OK:
        log_debug("dialog_quit : DIALOG_QUIT_OK\n");
        break;
      case DIALOG_QUIT_CANCEL:
        log_debug("dialog_quit : DIALOG_QUIT_CANCEL\n");
        break;
      default:
        log_debug("DIALOG_QUIT_NONE\n");
        break;
    }
  }
  return RET_OK;
}

static ret_t install_one(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = widget_get_window(widget);
  if (widget->name != NULL) {
    const char* name = widget->name;
    if (strstr(name, "open:") == name) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
    } else if (tk_str_eq(name, "close")) {
      widget_on(widget, EVT_CLICK, on_close, win);
    } else if (tk_str_eq(name, "ok")) {
      widget_on(widget, EVT_CLICK, on_ok, win);
    }
  }
  return RET_OK;
}

ret_t application_init(void) {
  widget_t* win = window_open("desktop");
  widget_child_on(win, "win_close", EVT_CLICK, on_click_win_close, NULL);
  widget_child_on(win, "enlarge", EVT_CLICK, on_click_enlarge, NULL);
  widget_child_on(win, "fullscreen", EVT_CLICK, on_fullscreen, win);

  widget_foreach(win, install_one, win);
  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#define LCD_WIDTH 800
#define LCD_HEGHT 600
#define APP_TYPE APP_DESKTOP

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"

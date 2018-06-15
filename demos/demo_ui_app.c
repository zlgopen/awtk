/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/label.h"
#include "base/timer.h"
#include "base/button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/utils.h"
#include "base/window.h"
#include "base/check_button.h"
#include "base/progress_bar.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "ui_loader/ui_builder_default.h"

static void install_click_hander(widget_t* widget);

static void open_window(const char* name) {
  widget_t* win = window_open(name);

  install_click_hander(win);

  if (win->type == WIDGET_DIALOG) {
    dialog_modal(win);
  }
}

static ret_t on_open_window(void* ctx, event_t* e) {
  open_window((const char*)ctx);
  (void)e;

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  widget_t* win = (widget_t*)ctx;
  (void)e;
  return window_close(win);
}

static ret_t on_quit(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 0);
  (void)e;
  return RET_OK;
}

static ret_t on_show_fps(void* ctx, event_t* e) {
  widget_t* button = WIDGETP(ctx);
  widget_t* widget = window_manager();
  window_manager_t* wm = WINDOW_MANAGER(widget);

  widget_invalidate(widget, NULL);
  window_manager_set_show_fps(widget, !wm->show_fps);
  widget_set_text(button, wm->show_fps ? L"Hide FPS" : L"Show FPS");

  return RET_OK;
}

static ret_t on_mem_test(void* ctx, event_t* e) {
  char text[32];
  uint32_t size = 100 * 1024;
  uint32_t memset_speed = 0;
  uint32_t memcpy_speed = 0;
  widget_t* win = WIDGETP(ctx);
  widget_t* label_memset = widget_lookup(win, "memset", TRUE);
  widget_t* label_cost = widget_lookup(win, "cost", TRUE);
  widget_t* label_memcpy = widget_lookup(win, "memcpy", TRUE);
  void* buff = TKMEM_ALLOC(size);
  uint32_t cost = tk_mem_speed_test(buff, size, &memcpy_speed, &memset_speed);
  TKMEM_FREE(buff);

  tk_snprintf(text, sizeof(text), "%ums", cost);
  widget_set_text_utf8(label_cost, text);

  tk_snprintf(text, sizeof(text), "memset: %uK/s", memset_speed);
  widget_set_text_utf8(label_memset, text);

  tk_snprintf(text, sizeof(text), "memcpy: %uK/s", memcpy_speed);
  widget_set_text_utf8(label_memcpy, text);

  return RET_OK;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* win = WIDGETP(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);

  (void)e;
  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* win = WIDGETP(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 90) % 100;
  progress_bar_set_value(progress_bar, value);

  (void)e;
  return RET_OK;
}

static void install_click_hander(widget_t* widget) {
  uint32_t i = 0;
  uint32_t nr = 0;
  if (widget->name.size && widget->type == WIDGET_BUTTON) {
    const char* name = widget->name.str;
    if (strstr(name, "open:") != NULL) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
    } else if (strstr(name, "memtest") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_mem_test, win);
    } else if (strstr(name, "inc") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_inc, win);
    } else if (strstr(name, "show_fps") != NULL) {
      widget_on(widget, EVT_CLICK, on_show_fps, widget);
    } else if (strstr(name, "dec") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_dec, win);
    } else if (strstr(name, "close") != NULL) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_close, win);
      }
    } else if (strstr(name, "quit") != NULL) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_quit, win);
      }
    }
  }

  nr = widget_count_children(widget);
  for (i = 0; i < nr; i++) {
    install_click_hander(widget_get_child(widget, i));
  }
}

ret_t application_init() {
  open_window("main");

  return RET_OK;
}

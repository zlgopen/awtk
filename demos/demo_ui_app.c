/**
 * File:   demo1_app.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic class of all widget
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "base/timer.h"
#include "base/button.h"
#include "base/check_button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/image_manager.h"
#include "base/label.h"
#include "base/mem.h"
#include "base/progress_bar.h"
#include "base/utils.h"
#include "base/window.h"
#include "ui_loader/ui_builder_default.h"

static ret_t on_timer(const timer_info_t* timer) {
  widget_t* progress_bar = (widget_t*)timer->ctx;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);

  return RET_REPEAT;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* progress_bar = (widget_t*)ctx;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);
  (void)e;
  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* progress_bar = (widget_t*)ctx;
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 90) % 100;
  progress_bar_set_value(progress_bar, value);
  (void)e;
  return RET_OK;
}

static ret_t on_ok(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 0);
  (void)e;
  mem_info_dump();
  return RET_OK;
}

static ret_t on_cancel(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 1);
  (void)e;
  mem_info_dump();
  return RET_OK;
}

static ret_t on_show_dialog(void* ctx, event_t* e) {
  uint32_t code = 0;
  widget_t* win = window_open("dialog1");

  widget_child_on(win, "ok", EVT_CLICK, on_ok, win);
  widget_child_on(win, "cancel", EVT_CLICK, on_cancel, win);

  code = dialog_modal(win);
  widget_destroy(win);

  mem_info_dump();
  (void)e;

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("window1");
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);
  widget_child_on(win, "inc", EVT_CLICK, on_inc, progress_bar);
  widget_child_on(win, "dec", EVT_CLICK, on_dec, progress_bar);
  widget_child_on(win, "dialog", EVT_CLICK, on_show_dialog, NULL);
  timer_add(on_timer, widget_lookup(win, "bar2", TRUE), 500);

  return RET_OK;
}

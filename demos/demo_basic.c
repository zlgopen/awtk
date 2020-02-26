/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic demo
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"

static ret_t on_timer(const timer_info_t* timer) {
  widget_t* win = WIDGET(timer->ctx);
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);

  float_t value = ((int)(PROGRESS_BAR(progress_bar)->value + 5)) % 100;
  progress_bar_set_value(progress_bar, value);

  return RET_REPEAT;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar2", TRUE);

  float_t value = ((int)(PROGRESS_BAR(progress_bar)->value + 10)) % 100;
  progress_bar_set_value(progress_bar, value);

  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar2", TRUE);

  float_t value = ((int)(PROGRESS_BAR(progress_bar)->value + 90)) % 100;
  progress_bar_set_value(progress_bar, value);

  return RET_OK;
}

static ret_t on_value_changed(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar2", TRUE);

  float_t value = widget_get_value(WIDGET(e->target));
  progress_bar_set_value(progress_bar, value);

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  dialog_confirm(NULL, "Hello AWTK!\nAre you sure to close?");

  return RET_OK;
}

ret_t application_init() {
  widget_t* system_bar = window_open("system_bar");
  widget_t* win = window_open("basic");

  widget_set_prop_str(win, WIDGET_PROP_CLICKABLE, "no");

  widget_add_timer(win, on_timer, 1000);
  widget_child_on(win, "inc_value", EVT_CLICK, on_inc, win);
  widget_child_on(win, "dec_value", EVT_CLICK, on_dec, win);
  widget_child_on(win, "close", EVT_CLICK, on_close, win);
  widget_child_on(win, "slider", EVT_VALUE_CHANGED, on_value_changed, win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

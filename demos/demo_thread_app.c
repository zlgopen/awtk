/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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

#include "base/idle.h"
#include "base/timer.h"
#include "base/enums.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/utf8.h"
#include "base/window.h"
#include "awtk_global.h"
#include "base/image_manager.h"
#include "widgets/progress_bar.h"

static bool_t s_app_quit = FALSE;

static ret_t update_progress_bar(widget_t* progress_bar) {
  static bool_t inc = TRUE;
  int value = widget_get_value(progress_bar);

  if (inc) {
    widget_set_value(progress_bar, ++value);
  } else {
    widget_set_value(progress_bar, --value);
  }

  if (value > 99) {
    inc = FALSE;
  } else if (value < 1) {
    inc = TRUE;
  }

  return RET_OK;
}

void* test_thread1(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    tk_run_in_ui_thread((tk_callback_t)update_progress_bar, args, TRUE);
    sleep_ms(30);
  }

  return NULL;
}

void* test_thread2(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    tk_run_in_ui_thread((tk_callback_t)update_progress_bar, args, FALSE);
    sleep_ms(30);
  }

  return NULL;
}

static ret_t on_idle_destroy(void* ctx) {
  idle_info_t* info = (idle_info_t*)ctx;
  log_debug("on_idle_destroy:%p\n", info);

  return RET_OK;
}

static ret_t on_idle_once(const idle_info_t* idle) {
  update_progress_bar(WIDGET(idle->ctx));

  return RET_REMOVE;
}

static ret_t on_idle(const idle_info_t* idle) {
  return update_progress_bar(WIDGET(idle->ctx));
}

void* test_thread3(void* args) {
  int nr = 500000;
  idle_queue_ex(on_idle_once, args, on_idle_destroy, NULL);

  while ((nr-- > 0) && (!s_app_quit)) {
    idle_queue(on_idle, args);
    sleep_ms(30);
  }

  return NULL;
}

static ret_t on_timer(const timer_info_t* timer) {
  return update_progress_bar(WIDGET(timer->ctx));
}

void* test_thread4(void* args) {
  int nr = 500000;
  while ((nr-- > 0) && (!s_app_quit)) {
    timer_queue(on_timer, args, 30);
    sleep_ms(30);
  }

  return NULL;
}

static tk_thread_t* thread1 = NULL;
static tk_thread_t* thread2 = NULL;
static tk_thread_t* thread3 = NULL;
static tk_thread_t* thread4 = NULL;

static ret_t on_close(void* ctx, event_t* e) {
  s_app_quit = TRUE;
  tk_quit();
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(win, 0, 0, 0, 0);
  widget_t* close = button_create(win, 0, 0, 0, 0);
  widget_t* progress_bar1 = progress_bar_create(win, 0, 0, 0, 0);
  widget_t* progress_bar2 = progress_bar_create(win, 0, 0, 0, 0);
  widget_t* progress_bar3 = progress_bar_create(win, 0, 0, 0, 0);
  widget_t* progress_bar4 = progress_bar_create(win, 0, 0, 0, 0);

  widget_set_text(label, L"Update progressbar in non GUI thread");
  widget_set_self_layout_params(label, "center", "middle:-90", "80%", "30");

  widget_set_self_layout_params(progress_bar1, "center", "middle:-60", "80%", "20");
  widget_set_self_layout_params(progress_bar2, "center", "middle:-30", "80%", "20");
  widget_set_self_layout_params(progress_bar3, "center", "middle:0", "80%", "20");
  widget_set_self_layout_params(progress_bar4, "center", "middle:30", "80%", "20");

  widget_set_text(close, L"close");
  widget_set_self_layout_params(close, "center", "bottom:30", "50%", "30");

  thread1 = tk_thread_create(test_thread1, progress_bar1);
  tk_thread_start(thread1);

  thread2 = tk_thread_create(test_thread2, progress_bar2);
  tk_thread_start(thread2);

  thread3 = tk_thread_create(test_thread3, progress_bar3);
  tk_thread_start(thread3);

  thread4 = tk_thread_create(test_thread4, progress_bar4);
  tk_thread_start(thread4);

  widget_on(close, EVT_CLICK, on_close, NULL);
  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  tk_thread_destroy(thread1);
  tk_thread_destroy(thread2);
  tk_thread_destroy(thread3);
  tk_thread_destroy(thread4);

  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

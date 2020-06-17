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
#include "base/image_manager.h"
#include "widgets/progress_bar.h"

static ret_t update_label(widget_t* label) {
  char str[64];
  static int times = 0;

  tk_snprintf(str, sizeof(str), "times:%d", times++);
  widget_set_text_utf8(label, str);

  return RET_OK;
}

static ret_t on_timer(const timer_info_t* timer) {
  return update_label(WIDGET(timer->ctx));
}

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

static ret_t on_idle(const idle_info_t* idle) {
  return update_progress_bar(WIDGET(idle->ctx));
}

void* test_idle_queue(void* args) {
  int nr = 500000;
  while (nr-- > 0) {
    idle_queue(on_idle, args);
    sleep_ms(30);
  }

  return NULL;
}

void* test_timer_queue(void* args) {
  int nr = 500000;
  while (nr-- > 0) {
    timer_queue(on_timer, args, 30);
    sleep_ms(30);
  }

  return NULL;
}

ret_t application_init() {
  tk_thread_t* thread = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(win, 10, 10, 300, 20);

  widget_set_text(label, L"Update progressbar in non GUI thread");
  progress_bar = progress_bar_create(win, 10, 80, 300, 20);

  thread = tk_thread_create(test_idle_queue, progress_bar);
  tk_thread_start(thread);

  thread = tk_thread_create(test_timer_queue, label);
  tk_thread_start(thread);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

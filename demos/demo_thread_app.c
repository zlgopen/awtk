/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "widgets/window.h"
#include "base/image_manager.h"
#include "widgets/progress_bar.h"

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

static ret_t on_timer(const timer_info_t* timer) {
  return update_progress_bar(WIDGET(timer->ctx));
}

static ret_t on_idle(const idle_info_t* idle) {
  return update_progress_bar(WIDGET(idle->ctx));
}

void* tk_thread_entry(void* args) {
  int nr = 500;
  while (nr-- > 0) {
    idle_queue(on_idle, args);
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

  thread = tk_thread_create(tk_thread_entry, progress_bar);
  tk_thread_start(thread);

  return RET_OK;
}

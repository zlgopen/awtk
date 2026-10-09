/**
 * File:   demo_mledit_partial_layout_app.c
 * Author: AWTK Develop Team
 * Brief:  mledit partial layout demo
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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
 * 2026-10-09 Yang Zewu <yangzewu@zlg.cn> created
 *
 */

#include "awtk.h"

#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define INSERT_TIMER_DURATION 100
#define PERFORMANCE_TEST_MAX_LINES 9999
#define PERFORMANCE_TEST_INSERT_LINES 100
#define DEMO_MAX_LINES 100

static uint32_t s_insert_timer_id = TK_INVALID_ID;
static widget_t* s_mledit = NULL;
static bool_t s_partial_layout = FALSE;
static bool_t s_overwrite = FALSE;
static bool_t s_insert = TRUE;
static bool_t s_performance_test = FALSE;

static ret_t on_insert_timer(const timer_info_t* timer) {
  static uint32_t sequence = 0;

  return_value_if_fail(s_mledit != NULL, RET_REMOVE);

  if (s_performance_test) {
    uint32_t i = 0;
    char text[2048] = {0};
    uint32_t length = 0;

    for (i = 0; i < PERFORMANCE_TEST_INSERT_LINES; i++) {
      length += tk_snprintf(text + length, sizeof(text) - length, "line %u\n", sequence++);
    }

    mledit_insert_text(s_mledit, s_mledit->text.size, text);
  } else {
    char text[64] = {0};

    tk_snprintf(text, sizeof(text), "line %u\n", sequence++);
    mledit_insert_text(s_mledit, s_mledit->text.size, text);
  }

  return RET_REPEAT;
}

static void demo_set_status(widget_t* label, bool_t value) {
  widget_set_text_utf8(label, value ? "开" : "关");
}

static ret_t on_partial_layout_clicked(void* ctx, event_t* e) {
  widget_t* label = WIDGET(ctx);
  (void)e;

  s_partial_layout = !s_partial_layout;
  mledit_set_partial_layout(s_mledit, s_partial_layout);
  demo_set_status(label, s_partial_layout);

  return RET_OK;
}

static ret_t on_overwrite_clicked(void* ctx, event_t* e) {
  widget_t* label = WIDGET(ctx);
  (void)e;

  s_overwrite = !s_overwrite;
  mledit_set_overwrite(s_mledit, s_overwrite);
  demo_set_status(label, s_overwrite);

  return RET_OK;
}

static ret_t on_insert_clicked(void* ctx, event_t* e) {
  widget_t* label = WIDGET(ctx);
  (void)e;

  s_insert = !s_insert;
  if (s_insert) {
    if (s_insert_timer_id == TK_INVALID_ID && s_mledit != NULL) {
      s_insert_timer_id = widget_add_timer(s_mledit, on_insert_timer, INSERT_TIMER_DURATION);
    }
  } else if (s_insert_timer_id != TK_INVALID_ID) {
    timer_remove(s_insert_timer_id);
    s_insert_timer_id = TK_INVALID_ID;
  }
  demo_set_status(label, s_insert);

  return RET_OK;
}

static ret_t on_performance_clicked(void* ctx, event_t* e) {
  widget_t* label = WIDGET(ctx);
  widget_t* insert_status = NULL;
  widget_t* win = widget_get_window(label);
  (void)e;

  s_performance_test = !s_performance_test;
  if (s_performance_test) {
    widget_t* wm = window_manager();

    mledit_set_max_lines(s_mledit, PERFORMANCE_TEST_MAX_LINES);
    window_manager_set_show_fps(wm, TRUE);
    window_manager_set_show_fps_position(wm, (wm->w - 60) / 2, 0);
  } else {
    mledit_set_max_lines(s_mledit, DEMO_MAX_LINES);
    window_manager_set_show_fps(window_manager(), FALSE);
  }

  if (!s_insert && s_mledit != NULL) {
    insert_status = widget_lookup(win, "insert_status", TRUE);
    if (insert_status != NULL) {
      demo_set_status(insert_status, TRUE);
    }
    s_insert = TRUE;
    s_insert_timer_id = widget_add_timer(s_mledit, on_insert_timer, INSERT_TIMER_DURATION);
  }

  demo_set_status(label, s_performance_test);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("mledit_partial_layout");
  widget_t* toggle_partial_layout = NULL;
  widget_t* toggle_overwrite = NULL;
  widget_t* toggle_insert = NULL;
  widget_t* toggle_performance = NULL;
  widget_t* partial_layout_status = NULL;
  widget_t* overwrite_status = NULL;
  widget_t* insert_status = NULL;
  widget_t* performance_status = NULL;

  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  s_mledit = widget_lookup(win, "mledit", TRUE);
  toggle_partial_layout = widget_lookup(win, "toggle_partial_layout", TRUE);
  toggle_overwrite = widget_lookup(win, "toggle_overwrite", TRUE);
  toggle_insert = widget_lookup(win, "toggle_insert", TRUE);
  toggle_performance = widget_lookup(win, "toggle_performance", TRUE);
  partial_layout_status = widget_lookup(win, "partial_layout_status", TRUE);
  overwrite_status = widget_lookup(win, "overwrite_status", TRUE);
  insert_status = widget_lookup(win, "insert_status", TRUE);
  performance_status = widget_lookup(win, "performance_status", TRUE);

  return_value_if_fail(s_mledit != NULL && toggle_partial_layout != NULL &&
                       toggle_overwrite != NULL && toggle_insert != NULL &&
                       toggle_performance != NULL && partial_layout_status != NULL &&
                       overwrite_status != NULL && insert_status != NULL &&
                       performance_status != NULL,
                       RET_BAD_PARAMS);

  widget_on(toggle_partial_layout, EVT_CLICK, on_partial_layout_clicked, partial_layout_status);
  widget_on(toggle_overwrite, EVT_CLICK, on_overwrite_clicked, overwrite_status);
  widget_on(toggle_insert, EVT_CLICK, on_insert_clicked, insert_status);
  widget_on(toggle_performance, EVT_CLICK, on_performance_clicked, performance_status);

  s_insert_timer_id = widget_add_timer(s_mledit, on_insert_timer, INSERT_TIMER_DURATION);

  return RET_OK;
}

ret_t application_exit() {
  s_insert_timer_id = TK_INVALID_ID;
  s_mledit = NULL;

  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"

/**
 * File:   demo2_app.c
 * Author: AWTK Develop Team
 * Brief:  demo paint
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
 * 2018-03-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/window.h"
#include "base/image_manager.h"
#include "canvas_widget/canvas_widget.h"

#include "vg_common.inc"

ret_t application_init() {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* canvas = canvas_widget_create(win, 0, 0, win->w, win->h);

  widget_on(canvas, EVT_PAINT, on_paint_vg, NULL);

  timer_add(on_timer, win, 500);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

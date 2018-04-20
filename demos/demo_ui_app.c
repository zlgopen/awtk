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
#include "common.c"

static ret_t on_show_dialog(void* ctx, event_t* e) {
  uint32_t code = 0;
  const char* name = (const char*)ctx;
  widget_t* win = window_open(name);

  widget_child_on(win, "ok", EVT_CLICK, on_ok, win);
  widget_child_on(win, "cancel", EVT_CLICK, on_cancel, win);

  code = dialog_modal(win);

  mem_info_dump();
  (void)e;
  (void)code;

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("window1");
  widget_t* bar1 = widget_lookup(win, "bar1", TRUE);
  widget_t* bar2 = widget_lookup(win, "bar2", TRUE);
  widget_child_on(win, "inc", EVT_CLICK, on_inc, bar2);
  widget_child_on(win, "dec", EVT_CLICK, on_dec, bar2);
  widget_child_on(win, "dialog", EVT_CLICK, on_show_dialog, "dialog1");
  widget_child_on(win, "dialog2", EVT_CLICK, on_show_dialog, "dialog2");
  //  timer_add(on_timer, bar1, 500);

  return RET_OK;
}

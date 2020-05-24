/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
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
#include "assets.h"

static ret_t on_click_close(void* ctx, event_t* e) {
  tk_quit();

  return RET_OK;
}

ret_t application_init(void) {
  widget_t* win = window_open("edit");
  widget_child_on(win, "close", EVT_CLICK, on_click_close, NULL);

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

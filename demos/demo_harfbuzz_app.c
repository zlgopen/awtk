/**
 * File:   demo_harfbuzz.c
 * Author: AWTK Develop Team
 * Brief:  demo_harfbuzz
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-12-18 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "awtk.h"

static ret_t on_edit_timer(const timer_info_t* timer) {
  char ch[32] = {0};
  static int32_t speed = 0;
  widget_t* edit = WIDGET(timer->ctx);

  tk_snprintf(ch, sizeof(ch), "%d", speed++);
  widget_set_text_utf8(edit, ch);

  return RET_REPEAT;
}

static ret_t on_label_timer(const timer_info_t* timer) {
  char ch[32] = {0};
  static int32_t speed = 0;
  widget_t* label = WIDGET(timer->ctx);

  tk_snprintf(ch, sizeof(ch), "%d", speed++);
  widget_set_text_utf8(label, ch);

  return RET_REPEAT;
}

static ret_t visit_init_child(void* ctx, const void* iter) {
  widget_t* win = WIDGET(ctx);
  widget_t* widget = WIDGET(iter);
  const char* name = widget->name;

  // 初始化指定名称的控件（设置属性或注册事件），请保证控件名称在窗口上唯一
  if (name != NULL && *name != '\0') {
    if (tk_str_eq(name, "label_anim")) {
      widget_add_timer(widget, on_label_timer, 500);
    } else if (tk_str_eq(name, "edit_anim")) {
      widget_add_timer(widget, on_edit_timer, 500);
    }
  }

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("harfbuzz_test");
  widget_foreach(win, visit_init_child, win);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#define LCD_WIDTH 800
#define LCD_HEIGHT 480

#include "awtk_main.inc"

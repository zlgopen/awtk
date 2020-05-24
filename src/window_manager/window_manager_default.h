/**
 * File:   window_manager.h
 * Author: AWTK Develop Team
 * Brief:  window manager
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_MANAGER_DEFAULT_H
#define TK_WINDOW_MANAGER_DEFAULT_H

#include "base/native_window.h"
#include "base/window_manager.h"

BEGIN_C_DECLS

/**
 * @class window_manager_default_t
 * @parent window_manager_t
 * 缺省窗口管理器。
 */
typedef struct _window_manager_default_t {
  window_manager_t window_manager;

  /*private*/
  bool_t animating;
  bool_t ready_animator;
  bool_t ignore_user_input;
  window_animator_t* animator;

  uint32_t fps;
  uint32_t fps_time;
  uint32_t fps_count;
  uint32_t last_paint_cost;

  widget_t* pending_close_window;
  widget_t* pending_open_window;

  char cursor[TK_NAME_LEN + 1];
  rect_t r_cursor;

  input_device_status_t input_device_status;
  uint32_t screen_saver_timer_id;
  uint32_t screen_saver_time;

  widget_t* prev_win;

  /* for window_manager_default_snap_prev_window */
  widget_t* curr_win;

  native_window_t* native_window;
  dialog_highlighter_t* dialog_highlighter;

  int32_t lcd_w;
  int32_t lcd_h;
} window_manager_default_t;

/**
 * @method window_manager_create
 * @export none
 * 创建窗口管理器。
 * @annotation ["constructor"]
 *
 * @return {window_manager_t*} 返回窗口管理器对象。
 */
widget_t* window_manager_create(void);

#define WINDOW_MANAGER_DEFAULT(widget) ((window_manager_default_t*)(widget))

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c);

END_C_DECLS

#endif /*TK_WINDOW_MANAGER_DEFAULT_H*/

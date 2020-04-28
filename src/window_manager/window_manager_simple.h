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
 * 2019-08-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_MANAGER_SIMPLE_H
#define TK_WINDOW_MANAGER_SIMPLE_H

#include "base/native_window.h"
#include "base/window_manager.h"

BEGIN_C_DECLS

/**
 * @class window_manager_simple_t
 * @parent window_manager_t
 * 简单窗口管理器(不支持窗口动画和对话框高亮策略)。
 */
typedef struct _window_manager_simple_t {
  window_manager_t window_manager;

  /*private*/
  uint32_t last_paint_cost;

  widget_t* system_bar;
  input_device_status_t input_device_status;

  widget_t* prev_win;
  native_window_t* native_window;

  int32_t lcd_w;
  int32_t lcd_h;
} window_manager_simple_t;

/**
 * @method window_manager_create
 * @export none
 * 创建窗口管理器。
 * @annotation ["constructor"]
 *
 * @return {window_manager_t*} 返回窗口管理器对象。
 */
widget_t* window_manager_create(void);

#define WINDOW_MANAGER_SIMPLE(widget) ((window_manager_simple_t*)(widget))

END_C_DECLS

#endif /*TK_WINDOW_MANAGER_SIMPLE_H*/

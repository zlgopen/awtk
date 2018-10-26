/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_WINDOW_BASE_H
#define TK_WINDOW_BASE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @enum window_stage_t
 * @annotation ["scriptable"]
 * @prefix WINDOW_STAGE_
 * 窗口的生命周期常量定义。
 */
typedef enum _window_stage_t {
  /**
   * @const WINDOW_STAGE_CREATED
   * 初始状态。
   */
  WINDOW_STAGE_CREATED = 0,
  /**
   * @const WINDOW_STAGE_OPENED
   * 窗口已经打开(窗口打开动画完成后，处于该状态，直到窗口被关闭)
   */
  WINDOW_STAGE_OPENED,
  /**
   * @const WINDOW_STAGE_CLOSED
   * 窗口已关闭。
   */
  WINDOW_STAGE_CLOSED
} window_stage_t;

/**
 * @class window_base_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 窗口。
 */
typedef struct _window_base_t {
  widget_t widget;
  /**
   * @property {char*} theme
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 主题资源的名称。
   */
  char* theme;

  /**
   * @property {char*} script
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 脚本文件名称(暂时没用)。
   */
  char* script;

  /**
   * @property {char*} open_anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 打开时的动画名称。
   */
  char* open_anim_hint;

  /**
   * @property {char*} close_anim_hint
   * @annotation ["set_prop","get_prop","readable","persitent","design"]
   * 关闭时的动画名称。
   */
  char* close_anim_hint;

  /**
   * @property {char*} stage
   * @annotation ["readable"]
   * 窗口当前处于的状态。
   */
  window_stage_t stage;

} window_base_t;

/*for sub class*/
ret_t window_base_destroy(widget_t* widget);
ret_t window_base_on_event(widget_t* widget, event_t* e);
ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c);
ret_t window_base_get_prop(widget_t* widget, const char* name, value_t* v);
ret_t window_base_set_prop(widget_t* widget, const char* name, const value_t* v);
widget_t* window_base_init(widget_t* widget, widget_t* parent, const widget_vtable_t* vt, xy_t x,
                           xy_t y, wh_t w, wh_t h);

#define WINDOW_BASE(widget) ((window_base_t*)(widget))

END_C_DECLS

#endif /*TK_WINDOW_BASE_H*/

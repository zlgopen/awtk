/**
 * File:   window_animator.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window_animator
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_ANIMATOR_H
#define TK_WINDOW_ANIMATOR_H

#include "base/widget.h"
#include "base/easing.h"

BEGIN_C_DECLS

struct _window_animator_t;
typedef struct _window_animator_t window_animator_t;

typedef ret_t (*window_animator_update_percent_t)(window_animator_t* wa);
typedef ret_t (*window_animator_draw_window_t)(window_animator_t* wa);

typedef ret_t (*window_animator_destroy_t)(window_animator_t* wa);

/**
 * @enum window_animator_type_t
 * @prefix WINDOW_ANIMATOR_
 * 窗口动画常量定义。
 */
typedef enum _window_animator_type_t {
  /**
   * @const WINDOW_ANIMATOR_NONE
   * 无动画。
   */
  WINDOW_ANIMATOR_NONE = 0,
  /**
   * @const WINDOW_ANIMATOR_CENTER_SCALE
   * 中心缩放。适用于居中的对话框。
   */
  WINDOW_ANIMATOR_CENTER_SCALE,
  /**
   * @const WINDOW_ANIMATOR_FADE,
   * 淡入淡出。适用于toast之类的提示。
   */
  WINDOW_ANIMATOR_FADE,
  /**
   * @const WINDOW_ANIMATOR_TOP_TOP_BOTTOM
   * 顶部部弹出。适用于对话框。
   */
  WINDOW_ANIMATOR_TOP_TO_BOTTOM,
  /**
   * @const WINDOW_ANIMATOR_BOTTOM_TO_TOP
   * 底部弹出。适用于对话框。
   */
  WINDOW_ANIMATOR_BOTTOM_TO_TOP,
  /**
   * @const WINDOW_ANIMATOR_HTRANSLATE
   * 水平平移。适用于窗口。
   */
  WINDOW_ANIMATOR_HTRANSLATE,
  /**
   * @const WINDOW_ANIMATOR_VTRANSLATE
   * 垂直平移。适用于窗口。
   */
  WINDOW_ANIMATOR_VTRANSLATE,

  WINDOW_ANIMATOR_NR
} window_animator_type_t;

/**
 * @class window_animator_t
 * 窗口动画。
 */
typedef struct _window_animator_t {
  window_animator_update_percent_t update_percent;
  window_animator_draw_window_t draw_prev_window;
  window_animator_draw_window_t draw_curr_window;
  window_animator_destroy_t destroy;

  uint32_t duration;
  uint32_t start_time;
  easing_func_t easing;

  widget_t* prev_win;
  widget_t* curr_win;

  bitmap_t prev_img;
  bitmap_t curr_img;

  framebuffer_object_t prev_fbo;
  framebuffer_object_t curr_fbo;

  bool_t open;
  float_t ratio;
  float_t percent;
  canvas_t* canvas;
  float_t time_percent;
} window_animator_t;

/**
 * @method window_animator_create_for_open
 * @constructor
 * 为打开窗口创建动画。
 * @param {window_animator_type_t} type 动画类型。
 * @param {canvas_t*} c canvas。
 * @param {widget_t*} prev_win 前一窗口。
 * @param {widget_t*} curr_win 当前窗口。
 *
 * @return {window_animator_t*} 窗口动画对象。
 */
window_animator_t* window_animator_create_for_open(window_animator_type_t type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win);

/**
 * @method window_animator_create_for_close
 * @constructor
 * 为关闭窗口创建动画。
 * @param {window_animator_type_t} type 动画类型。
 * @param {canvas_t*} c canvas。
 * @param {widget_t*} prev_win 前一窗口。
 * @param {widget_t*} curr_win 当前窗口。
 *
 * @return {window_animator_t*} 窗口动画对象。
 */
window_animator_t* window_animator_create_for_close(window_animator_type_t type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win);

/**
 * @method window_animator_update
 * 更新动画。
 * @param {window_animator_t*} wa 窗口动画对象。
 * @param {canvas_t*} canvas 画布对象。
 * @param {uint32_t} time_ms 当前时间(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_update(window_animator_t* wa, uint32_t time_ms);

/**
 * @method window_animator_destroy
 * 销毁窗口动画对象。
 * @param {window_animator_t*} wa 窗口动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_destroy(window_animator_t* wa);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_H*/

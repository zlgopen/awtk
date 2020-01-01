/**
 * File:   window_animator.h
 * Author: AWTK Develop Team
 * Brief:  window_animator
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WINDOW_ANIMATOR_H
#define TK_WINDOW_ANIMATOR_H

#include "tkc/easing.h"
#include "base/widget.h"
#include "base/dialog_highlighter.h"

BEGIN_C_DECLS

struct _window_animator_t;
typedef struct _window_animator_t window_animator_t;

typedef ret_t (*window_animator_init_t)(window_animator_t* wa);
typedef ret_t (*window_animator_update_percent_t)(window_animator_t* wa);
typedef ret_t (*window_animator_draw_prev_window_t)(window_animator_t* wa);
typedef ret_t (*window_animator_draw_curr_window_t)(window_animator_t* wa);

typedef struct _window_animator_vtable_t {
  const char* type;
  const char* desc;
  uint32_t size;
  bool_t overlap;
  window_animator_init_t init;
  window_animator_update_percent_t update_percent;
  window_animator_draw_prev_window_t draw_prev_window;
  window_animator_draw_curr_window_t draw_curr_window;
} window_animator_vtable_t;

typedef window_animator_t* (*window_animator_create_t)(bool_t open, object_t* args);

/**
 * @enum window_animator_type_t
 * @prefix WINDOW_ANIMATOR_
 * @type string
 * 内置窗口动画常量定义。
 */

/**
 * @const WINDOW_ANIMATOR_CENTER_SCALE
 * 中心缩放。适用于居中的对话框。
 */
#define WINDOW_ANIMATOR_CENTER_SCALE "center_scale"

/**
 * @const WINDOW_ANIMATOR_FADE,
 * 淡入淡出。适用于toast之类的提示。
 */
#define WINDOW_ANIMATOR_FADE "fade"

/**
 * @const WINDOW_ANIMATOR_POPDOWN
 * 顶部部弹出。适用于对话框。
 */
#define WINDOW_ANIMATOR_POPDOWN "popdown"

/*for compatibility, use WINDOW_ANIMATOR_POPDOWN instead*/
#define WINDOW_ANIMATOR_TOP_TO_BOTTOM "top_to_bottom"

/**
 * @const WINDOW_ANIMATOR_POPUP
 * 底部弹出。适用于对话框。
 */
#define WINDOW_ANIMATOR_POPUP "popup"
/*for compatibility, use WINDOW_ANIMATOR_POPUP instead*/
#define WINDOW_ANIMATOR_BOTTOM_TO_TOP "bottom_to_top"

/**
 * @const WINDOW_ANIMATOR_SLIDE_UP
 * 向上弹出。适用于窗口。
 */
#define WINDOW_ANIMATOR_SLIDE_UP "slide_up"

/**
 * @const WINDOW_ANIMATOR_SLIDE_DOWN
 * 向下弹出。适用于窗口。
 */
#define WINDOW_ANIMATOR_SLIDE_DOWN "slide_down"

/**
 * @const WINDOW_ANIMATOR_SLIDE_LEFT
 * 向左边弹出。适用于窗口。
 */
#define WINDOW_ANIMATOR_SLIDE_LEFT "slide_left"

/**
 * @const WINDOW_ANIMATOR_SLIDE_RIGHT
 * 向右边弹出。适用于窗口。
 */
#define WINDOW_ANIMATOR_SLIDE_RIGHT "slide_right"

/**
 * @const WINDOW_ANIMATOR_HTRANSLATE
 * 水平平移。适用于窗口。
 */
#define WINDOW_ANIMATOR_HTRANSLATE "htranslate"

/**
 * @const WINDOW_ANIMATOR_VTRANSLATE
 * 垂直平移。适用于窗口。
 */
#define WINDOW_ANIMATOR_VTRANSLATE "vtranslate"

/**
 * @class window_animator_t
 * 窗口动画。
 */
struct _window_animator_t {
  uint32_t duration;
  uint32_t start_time;
  easing_func_t easing;

  widget_t* prev_win;
  widget_t* curr_win;

  bitmap_t prev_img;
  bitmap_t curr_img;

  bool_t open;
  float_t ratio;
  float_t percent;
  canvas_t* canvas;
  float_t time_percent;

  framebuffer_object_t prev_fbo;
  framebuffer_object_t curr_fbo;
  dialog_highlighter_t* dialog_highlighter;

  const window_animator_vtable_t* vt;
};

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
 * @method window_animator_begin_frame
 * begin frame
 * @param {window_animator_t*} wa 窗口动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_begin_frame(window_animator_t* wa);

/**
 * @method window_animator_end_frame
 * end frame
 * @param {window_animator_t*} wa 窗口动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_end_frame(window_animator_t* wa);

/**
 * @method window_animator_destroy
 * 销毁窗口动画对象。
 * @param {window_animator_t*} wa 窗口动画对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_animator_destroy(window_animator_t* wa);

/**
 * @method window_animator_create
 * 创建窗口动画对象。
 *
 *>供子类构造函数用。
 * @param {bool_t} open TRUE表示打开窗口的动画，FALSE表示关闭窗口的动画。
 * @param {const window_animator_vtable_t*} vt 虚表对象。
 *
 * @return {window_animator_t*} 返回窗口动画对象。
 */
window_animator_t* window_animator_create(bool_t open, const window_animator_vtable_t* vt);

/*public for factory*/
ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                              widget_t* curr_win);

ret_t window_animator_overlap_default_draw_prev(window_animator_t* wa);

END_C_DECLS

#endif /*TK_WINDOW_ANIMATOR_H*/

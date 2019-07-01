/**
 * File:   window_manager.h
 * Author: AWTK Develop Team
 * Brief:  window manager
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_WINDOW_MANAGER_H
#define TK_WINDOW_MANAGER_H

#include "base/widget.h"
#include "base/canvas.h"
#include "base/dialog_highlighter.h"
#include "base/input_device_status.h"
#include "base/window_animator_factory.h"

BEGIN_C_DECLS

/**
 * @class window_manager_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 窗口管理器。
 */
typedef struct _window_manager_t {
  widget_t widget;

  /**
   * @property {bool_t} show_fps
   * @annotation ["readable", "scriptable"]
   * 是否显示fps。
   */
  bool_t show_fps;

  /*private*/
  rect_t dirty_rect;
  rect_t last_dirty_rect;

  bool_t animating;
  bool_t ignore_user_input;
  window_animator_t* animator;
  canvas_t* canvas;
  uint32_t last_paint_cost;

  uint32_t fps;
  uint32_t fps_time;
  uint32_t fps_count;

  widget_t* pending_close_window;
  widget_t* pending_open_window;

  char* cursor;
  rect_t r_cursor;

  widget_t* system_bar;
  input_device_status_t input_device_status;
  uint32_t screen_saver_timer_id;
  uint32_t screen_saver_time;

  dialog_highlighter_t* dialog_highlighter;
  widget_t* prev_win;
} window_manager_t;

/**
 * @method window_manager
 * 获取全局window_manager对象
 * @alias window_manager_instance
 * @annotation ["constructor", "scriptable", "cast"]
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_manager(void);

/**
 * @method window_manager_cast
 * 转换为window_manager对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget window_manager对象。
 *
 * @return {widget_t*} window_manager对象。
 */
widget_t* window_manager_cast(widget_t* widget);

/**
 * @method window_manager_set
 * 设置缺省的窗口管理器。
 * @param {window_manager_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_set(widget_t* widget);

/**
 * @method window_manager_create
 * 创建窗口管理器。
 * @annotation ["constructor"]
 *
 * @return {window_manager_t*} 返回窗口管理器对象。
 */
widget_t* window_manager_create(void);

/**
 * @method window_manager_init
 * 初始化窗口管理器。
 * @annotation ["constructor"]
 * @param {window_manager_t*} widget 窗口管理器对象。
 *
 * @return {widget_t*} 返回窗口管理器对象。
 */
widget_t* window_manager_init(window_manager_t* widget);

/**
 * @method window_manager_get_top_main_window
 * 获取最上面的主窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {widget_t*} 返回窗口对象。
 */
widget_t* window_manager_get_top_main_window(widget_t* widget);

/**
 * @method window_manager_get_top_window
 * 获取最上面的窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {widget_t*} 返回窗口对象。
 */
widget_t* window_manager_get_top_window(widget_t* widget);

/**
 * @method window_manager_resize
 * 调整窗口管理器的大小。
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_resize(widget_t* widget, wh_t w, wh_t h);

/**
 * @method window_manager_open_window
 * 打开窗口。
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {widget_t*} window 窗口对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_open_window(widget_t* widget, widget_t* window);

/**
 * @method window_manager_close_window
 * 关闭窗口。
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {widget_t*} window 窗口对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_close_window(widget_t* widget, widget_t* window);

/**
 * @method window_manager_close_window_force
 * 强制立即关闭窗口。
 *
 *> 本函数不会执行窗口动画。
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {widget_t*} window 窗口对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_close_window_force(widget_t* widget, widget_t* window);

/**
 * @method window_manager_paint
 * 绘制。
 *
 *> 仅由主循环调用。
 *
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {canvas_t*} c 画布。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_paint(widget_t* widget, canvas_t* c);

/**
 * @method window_manager_dispatch_input_event
 * 分发输入事件。
 *
 *> 一般仅由主循环调用，特殊情况也可以用来注入事件。
 *
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {event_t*} e 事件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_dispatch_input_event(widget_t* widget, event_t* e);

/**
 * @method window_manager_set_show_fps
 * 设置是否显示FPS。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {bool_t}  show_fps 是否显示FPS。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_set_show_fps(widget_t* widget, bool_t show_fps);

/**
 * @method window_manager_set_screen_saver_time
 * 设置屏保时间。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {uint32_t}  screen_saver_time 屏保时间(单位毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_set_screen_saver_time(widget_t* widget, uint32_t screen_saver_time);

/**
 * @method window_manager_set_cursor
 * 设置鼠标指针。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {const char*} cursor 图片名称(从图片管理器中加载)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_set_cursor(widget_t* widget, const char* cursor);

/**
 * @method window_manager_back
 * 请求关闭顶层窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_back(widget_t* widget);

/**
 * @method window_manager_back_to_home
 * 回到主窗口，关闭之上的全部窗口。
 *
 *> 由于dialog通常需要用户确认，顶层窗口为dialog时调用会失败。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_back_to_home(widget_t* widget);

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c);
ret_t window_manager_set_dialog_highlighter(widget_t* widget, dialog_highlighter_t* highlighter);
#define WINDOW_MANAGER(widget) ((window_manager_t*)(widget))

/*for compatible*/
#define window_manager_request_close_top_window window_manager_back

/*public for window*/
ret_t window_manager_layout_children(widget_t* widget);

/*public for window animator*/
ret_t window_manager_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate);

ret_t window_manager_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate);

END_C_DECLS

#endif /*TK_WINDOW_MANAGER_H*/

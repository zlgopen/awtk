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

#ifndef TK_WINDOW_MANAGER_H
#define TK_WINDOW_MANAGER_H

#include "base/widget.h"
#include "base/canvas.h"
#include "base/dialog_highlighter.h"
#include "base/input_device_status.h"
#include "base/window_animator_factory.h"

BEGIN_C_DECLS

typedef widget_t* (*window_manager_get_top_window_t)(widget_t* widget);
typedef widget_t* (*window_manager_get_prev_window_t)(widget_t* widget);
typedef widget_t* (*window_manager_get_top_main_window_t)(widget_t* widget);
typedef ret_t (*window_manager_open_window_t)(widget_t* widget, widget_t* window);
typedef ret_t (*window_manager_close_window_t)(widget_t* widget, widget_t* window);
typedef ret_t (*window_manager_close_window_force_t)(widget_t* widget, widget_t* window);
typedef ret_t (*window_manager_paint_t)(widget_t* widget);
typedef ret_t (*window_manager_dispatch_input_event_t)(widget_t* widget, event_t* e);
typedef ret_t (*window_manager_set_show_fps_t)(widget_t* widget, bool_t show_fps);
typedef ret_t (*window_manager_set_screen_saver_time_t)(widget_t* widget, uint32_t time);
typedef ret_t (*window_manager_set_cursor_t)(widget_t* widget, const char* cursor);
typedef ret_t (*window_manager_post_init_t)(widget_t* widget, wh_t w, wh_t h);
typedef ret_t (*window_manager_back_t)(widget_t* widget);
typedef ret_t (*window_manager_back_to_t)(widget_t* widget, const char* name);
typedef ret_t (*window_manager_get_pointer_t)(widget_t* widget, xy_t* x, xy_t* y, bool_t* pressed);
typedef ret_t (*window_manager_is_animating_t)(widget_t* widget, bool_t* playing);

typedef ret_t (*window_manager_dispatch_native_window_event_t)(widget_t* widget, event_t* e,
                                                               void* handle);

typedef ret_t (*window_manager_snap_curr_window_t)(widget_t* widget, widget_t* curr_win,
                                                   bitmap_t* img, framebuffer_object_t* fbo,
                                                   bool_t auto_rotate);
typedef ret_t (*window_manager_snap_prev_window_t)(widget_t* widget, widget_t* prev_win,
                                                   bitmap_t* img, framebuffer_object_t* fbo,
                                                   bool_t auto_rotate);
typedef dialog_highlighter_t* (*window_manager_get_dialog_highlighter_t)(widget_t* widget);
typedef ret_t (*window_manager_resize_t)(widget_t* widget, wh_t w, wh_t h);

typedef struct _window_manager_vtable_t {
  window_manager_back_t back;
  window_manager_back_to_t back_to;
  window_manager_paint_t paint;
  window_manager_post_init_t post_init;
  window_manager_set_cursor_t set_cursor;
  window_manager_open_window_t open_window;
  window_manager_close_window_t close_window;
  window_manager_set_show_fps_t set_show_fps;
  window_manager_get_top_window_t get_top_window;
  window_manager_get_prev_window_t get_prev_window;
  window_manager_close_window_force_t close_window_force;
  window_manager_get_top_main_window_t get_top_main_window;
  window_manager_dispatch_input_event_t dispatch_input_event;
  window_manager_dispatch_native_window_event_t dispatch_native_window_event;
  window_manager_set_screen_saver_time_t set_screen_saver_time;
  window_manager_get_pointer_t get_pointer;
  window_manager_is_animating_t is_animating;
  window_manager_snap_curr_window_t snap_curr_window;
  window_manager_snap_prev_window_t snap_prev_window;
  window_manager_get_dialog_highlighter_t get_dialog_highlighter;
  window_manager_resize_t resize;
} window_manager_vtable_t;

/**
 * @class window_manager_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 窗口管理器。
 */
typedef struct _window_manager_t {
  widget_t widget;

  /*private*/
  bool_t show_fps;
  bool_t ignore_input_events;
  bool_t show_waiting_pointer_cursor;
  const window_manager_vtable_t* vt;
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
 * @method window_manager_get_prev_window
 * 获取前一个的窗口。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {widget_t*} 返回窗口对象。
 */
widget_t* window_manager_get_prev_window(widget_t* widget);

/**
 * @method window_manager_get_pointer_x
 * 获取指针当前的X坐标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {xy_t} 返回指针当前的X坐标。
 */
xy_t window_manager_get_pointer_x(widget_t* widget);

/**
 * @method window_manager_get_pointer_y
 * 获取指针当前的Y坐标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {xy_t} 返回指针当前的X坐标。
 */
xy_t window_manager_get_pointer_y(widget_t* widget);

/**
 * @method window_manager_get_pointer_pressed
 * 获取指针当前是否按下。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {bool_t} 返回指针当前是否按下。
 */
bool_t window_manager_get_pointer_pressed(widget_t* widget);

/**
 * @method window_manager_is_animating
 * 获取当前窗口动画是否正在播放。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {bool_t} 返回TRUE表示正在播放，FALSE表示没有播放。
 */
bool_t window_manager_is_animating(widget_t* widget);

/**
 * @method window_manager_post_init
 * post init。
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_post_init(widget_t* widget, wh_t w, wh_t h);

/**
 * @method window_manager_open_window
 * 打开窗口。
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
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_paint(widget_t* widget);

/**
 * @method window_manager_check_and_layout
 * 检查各个窗口的layout并且把有需要的执行对应的layout。
 *
 *> 仅由主循环调用。
 *
 * @annotation ["private"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_check_and_layout(widget_t* widget);

/**
 * @method window_manager_dispatch_input_event
 * 分发输入事件。
 *
 *> 一般仅由主循环调用，特殊情况也可以用来注入事件。
 *
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
 * @param {uint32_t}  screen_saver_time 屏保时间(单位毫秒), 为0关闭屏保。
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
 * 
 * > 如果顶层窗口时模态对话框，用DIALOG\_QUIT\_NONE调用dialog\_quit。
 * 
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
 * > 如果顶层窗口时模态对话框，用DIALOG\_QUIT\_NONE调用dialog\_quit。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_back_to_home(widget_t* widget);

/**
 * @method window_manager_back_to
 * 回到指定的窗口，关闭之上的全部窗口。
 *
 * > 如果顶层窗口时模态对话框，用DIALOG\_QUIT\_NONE调用dialog\_quit。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {const char*} target 目标窗口的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_back_to(widget_t* widget, const char* target);

/**
 * @method window_manager_dispatch_native_window_event
 * 处理native window事件。
 *
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {event_t*} e 事件。
 * @param {void*} handle native window句柄。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_dispatch_native_window_event(widget_t* widget, event_t* e, void* handle);

/**
 * @method window_manager_begin_wait_pointer_cursor
 * 开始等待鼠标指针。
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {bool_t} ignore_user_input 是否忽略用户输入。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t window_manager_begin_wait_pointer_cursor(widget_t* widget, bool_t ignore_user_input);

/**
 * @method window_manager_end_wait_pointer_cursor
 * 结束等待鼠标指针。
 * @param {widget_t*} widget 窗口管理器对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。。
 */
ret_t window_manager_end_wait_pointer_cursor(widget_t* widget);

/**
 * @method window_manager_resize
 * 调整原生窗口的大小。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 窗口管理器对象。
 * @param {wh_t}   w 宽度
 * @param {wh_t}   h 高度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t window_manager_resize(widget_t* widget, wh_t w, wh_t h);

/*public for animators*/
ret_t window_manager_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate);

ret_t window_manager_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate);

dialog_highlighter_t* window_manager_get_dialog_highlighter(widget_t* widget);

widget_t* window_manager_create(void);

/*helper for sub class*/
widget_t* window_manager_init(window_manager_t* wm, const widget_vtable_t* wvt,
                              const window_manager_vtable_t* vt);

widget_t* window_manager_find_target_by_win(widget_t* widget, void* native_win);
widget_t* window_manager_find_target(widget_t* widget, void* native_win, xy_t x, xy_t y);
ret_t window_manager_on_theme_changed(widget_t* widget);
ret_t window_manager_dispatch_top_window_changed(widget_t* widget);
ret_t window_manager_dispatch_window_event(widget_t* window, event_type_t type);

#define WINDOW_MANAGER(widget) ((window_manager_t*)(widget))

END_C_DECLS

#endif /*TK_WINDOW_MANAGER_H*/

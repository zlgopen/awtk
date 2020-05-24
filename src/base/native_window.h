/**
 * File:   native_window.h
 * Author: AWTK Develop Team
 * Brief:  native window
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NATIVE_WINDOW_H
#define TK_NATIVE_WINDOW_H

#include "tkc/str.h"
#include "tkc/value.h"
#include "tkc/object.h"
#include "base/canvas.h"

BEGIN_C_DECLS

struct _native_window_t;
typedef struct _native_window_t native_window_t;

typedef struct _native_window_info_t {
  void* handle;
  int32_t x;
  int32_t y;
  uint32_t w;
  uint32_t h;
  float_t ratio;
} native_window_info_t;

typedef canvas_t* (*native_window_get_canvas_t)(native_window_t* win);
typedef ret_t (*native_window_move_t)(native_window_t* win, xy_t x, xy_t y);
typedef ret_t (*native_window_resize_t)(native_window_t* win, wh_t w, wh_t h);
typedef ret_t (*native_window_gl_make_current_t)(native_window_t* win);
typedef ret_t (*native_window_swap_buffer_t)(native_window_t* win);
typedef ret_t (*native_window_preprocess_event_t)(native_window_t* win, event_t* e);
typedef ret_t (*native_window_get_info_t)(native_window_t* win, native_window_info_t* info);
typedef ret_t (*native_window_minimize_t)(native_window_t* win);
typedef ret_t (*native_window_maximize_t)(native_window_t* win);
typedef ret_t (*native_window_restore_t)(native_window_t* win);
typedef ret_t (*native_window_center_t)(native_window_t* win);
typedef ret_t (*native_window_show_border_t)(native_window_t* win, bool_t show);
typedef ret_t (*native_window_set_fullscreen_t)(native_window_t* win, bool_t fullscreen);
typedef ret_t (*native_window_set_cursor_t)(native_window_t* win, const char* name, bitmap_t* img);

typedef struct _native_window_vtable_t {
  const char* type;
  native_window_move_t move;
  native_window_resize_t resize;
  native_window_get_info_t get_info;
  native_window_get_canvas_t get_canvas;
  native_window_swap_buffer_t swap_buffer;
  native_window_gl_make_current_t gl_make_current;
  native_window_preprocess_event_t preprocess_event;
  native_window_center_t center;
  native_window_restore_t restore;
  native_window_minimize_t minimize;
  native_window_maximize_t maximize;
  native_window_show_border_t show_border;
  native_window_set_fullscreen_t set_fullscreen;
  native_window_set_cursor_t set_cursor;
} native_window_vtable_t;

/**
 * @class native_window_t
 * @parent object_t
 * @annotation ["scriptable"]
 * 原生窗口。
 *
 */
struct _native_window_t {
  object_t object;

  void* handle;
  bool_t shared;

  rect_t rect;
  float_t ratio;

  bool_t dirty;
  rect_t dirty_rect;
  rect_t last_dirty_rect;

  const native_window_vtable_t* vt;
};

/**
 * @method native_window_move
 * 移动窗口。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {bool_t} force 无论是否shared都move。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_move(native_window_t* win, xy_t x, xy_t y, bool_t force);

/**
 * @method native_window_resize
 * 调整窗口大小。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 * @param {wh_t} w 宽。
 * @param {wh_t} h 高。
 * @param {bool_t} force 无论是否shared都resize。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_resize(native_window_t* win, wh_t w, wh_t h, bool_t force);

/**
 * @method native_window_minimize
 * 最小化窗口。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_minimize(native_window_t* win);

/**
 * @method native_window_maximize
 * 最大化窗口。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_maximize(native_window_t* win);

/**
 * @method native_window_restore
 * 恢复窗口大小。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_restore(native_window_t* win);

/**
 * @method native_window_center
 * 窗口居中。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_center(native_window_t* win);

/**
 * @method native_window_show_border
 * 是否显示边框。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 * @param {bool_t} show 是否显示。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_show_border(native_window_t* win, bool_t show);

/**
 * @method native_window_set_fullscreen
 * 是否全屏。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 * @param {bool_t} fullscreen 是否全屏。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_set_fullscreen(native_window_t* win, bool_t fullscreen);

/**
 * @method native_window_set_cursor
 * 设置鼠标光标。
 *
 * @annotation ["scriptable"]
 * @param {native_window_t*} win win对象。
 * @param {const char*}  name 鼠标光标的名称。
 * @param {bitmap_t*}  img 鼠标光标的图片。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_set_cursor(native_window_t* win, const char* name, bitmap_t* img);

/**
 * @method native_window_get_canvas
 * 获取canvas。
 *
 * @param {native_window_t*} win win对象。
 *
 * @return {canvas_t} 返回canvas对象。
 */
canvas_t* native_window_get_canvas(native_window_t* win);

/**
 * @method native_window_create
 * 创建win对象。
 * @param {const char*} params 参数。
 *
 * @return {native_window_t*} 返回win对象。
 */
native_window_t* native_window_create(widget_t* widget);

/**
 * @method native_window_invalidate
 * 请求重绘指定区域。
 * @param {native_window_t*} win win对象。
 * @param {rect_t*} r 重绘区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t native_window_invalidate(native_window_t* win, rect_t* r);

ret_t native_window_swap_buffer(native_window_t* win);
ret_t native_window_gl_make_current(native_window_t* win);
ret_t native_window_preprocess_event(native_window_t* win, event_t* e);
ret_t native_window_get_info(native_window_t* win, native_window_info_t* info);

/*public for window manager only*/
ret_t native_window_begin_frame(native_window_t* win, lcd_draw_mode_t mode);
ret_t native_window_paint(native_window_t* win, widget_t* widget);
ret_t native_window_end_frame(native_window_t* win);

rect_t native_window_calc_dirty_rect(native_window_t* win);
ret_t native_window_clear_dirty_rect(native_window_t* win);
ret_t native_window_update_last_dirty_rect(native_window_t* win);
ret_t native_window_on_resized(native_window_t* win, wh_t w, wh_t h);

#define NATIVE_WINDOW(win) ((native_window_t*)(win))

#define NATIVE_WINDOW_PROP_SIZE "size"
#define NATIVE_WINDOW_PROP_TITLE "title"
#define NATIVE_WINDOW_PROP_POSITION "position"

typedef enum _native_window_event_type_t {
  EVT_NATIVE_WINDOW_RESIZED = 0xff,
  EVT_NATIVE_WINDOW_DESTROY
} native_window_event_type_t;

END_C_DECLS

#endif /*TK_NATIVE_WINDOW_H*/

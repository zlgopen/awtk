/**
 * File:   calibration_win.h
 * Author: AWTK Develop Team
 * Brief:  touch screen calibration win
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
 * 2018-10-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CALIBRATION_WIN_H
#define TK_CALIBRATION_WIN_H

#include "base/window_base.h"

BEGIN_C_DECLS

typedef enum _point_name_t {
  PT_TOP_LEFT = 0,
  PT_TOP_RIGHT,
  PT_BOTTOM_RIGHT,
  PT_BOTTOM_LEFT,
  PT_CENTER,
  PT_MAX_NR
} point_name_t;

typedef ret_t (*calibration_win_on_done_t)(void* ctx, point_t points[PT_MAX_NR]);
typedef ret_t (*calibration_win_on_click_t)(void* ctx, uint32_t index, point_t p);

/**
 * @class calibration_win_t
 * @parent window_base_t
 * @annotation ["scriptable", "design", "widget", "window"]
 * 电阻屏校准窗口。
 *
 * calibration\_win\_t是[window\_base\_t](window_base_t.md)的子类控件，
 * window\_base\_t的函数均适用于calibration\_win\_t控件。
 *
 * 在xml中使用"calibration\_win"标签创建电阻屏校准窗口。如：
 *
 * ```xml
 * <calibration_win name="cali" w="100%" h="100%" text="Please click the center of cross">
 * </calibration_win>
 * ```
 *
 * > 更多用法请参考：
 * [window.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/calibration_win.xml)
 *
 * 在c代码中使用函数calibration\_win\_create创建窗口。如：
 *
 * ```c
 *  widget_t* win = calibration_win_create(NULL, 0, 0, 320, 480);
 * ```
 *
 * 通过calibration\_win\_set\_on\_done注册回调函数，用于保存校准数据。
 *
 */
typedef struct _calibration_win_t {
  window_base_t window;

  uint32_t x_offset;
  uint32_t y_offset;
  uint32_t cross_size;

  uint32_t cursor;
  point_t points[PT_MAX_NR];

  void* on_done_ctx;
  calibration_win_on_done_t on_done;
  void* on_click_ctx;
  calibration_win_on_click_t on_click;
} calibration_win_t;

/**
 * @method calibration_win_create
 * @annotation ["constructor"]
 * 创建calibration_win对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* calibration_win_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method calibration_win_set_on_done
 * 设置校准完成的处理函数。
 * @param {widget_t*} widget widget对象。
 * @param {calibration_win_on_done_t} on_done 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t calibration_win_set_on_done(widget_t* widget, calibration_win_on_done_t on_done, void* ctx);

/**
 * @method calibration_win_set_on_click
 * 设置校准点击事件的处理函数。
 * @param {widget_t*} widget widget对象。
 * @param {calibration_win_on_click_t} on_click 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t calibration_win_set_on_click(widget_t* widget, calibration_win_on_click_t on_click,
                                   void* ctx);
/**
 * @method calibration_win_cast
 * 转换为calibration_win对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget calibration_win对象。
 *
 * @return {widget_t*} calibration_win对象。
 */
widget_t* calibration_win_cast(widget_t* widget);

#define CALIBRATION_WIN(widget) ((calibration_win_t*)(calibration_win_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(calibration_win);

END_C_DECLS

#endif /*TK_CALIBRATION_WIN_H*/

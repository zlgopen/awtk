/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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

#ifndef TK_WINDOW_H
#define TK_WINDOW_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class window_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 窗口。
 */
typedef struct _window_t {
  window_base_t window;

} window_t;

/**
 * @method window_create
 * 创建window对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method window_cast
 * 转换为window对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget window对象。
 *
 * @return {widget_t*} window对象。
 */
widget_t* window_cast(widget_t* widget);

#define WINDOW(widget) ((window_t*)(widget))

END_C_DECLS

#endif /*TK_WINDOW_H*/

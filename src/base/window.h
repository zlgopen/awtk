/**
 * File:   window.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_WINDOW_H
#define LFTK_WINDOW_H

#include "base/widget.h"
#include "base/window_animator.h"

BEGIN_C_DECLS

/**
 * @class window_t
 * @parent widget_t
 * @scriptable
 * 窗口。
 */
typedef struct _window_t {
  widget_t widget;
  window_animator_type_t anim_hint;
} window_t;

/**
 * @method window_create
 * @constructor
 * 创建window对象
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
 * @method window_open
 * @constructor
 * 从资源文件中加载并创建window对象。本函数在ui_loader/ui_builder_default里实现。
 * @param {char*} name window的名称。
 *
 * @return {widget_t*} 对象。
 */

#define WINDOW(widget) ((window_t*)(widget))

END_C_DECLS

#endif /*LFTK_WINDOW_H*/

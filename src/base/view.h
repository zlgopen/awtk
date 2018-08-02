/**
 * File:   view.h
 * Author: AWTK Develop Team
 * Brief:  view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_H
#define TK_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class view_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 一个通用的容器控件，也可以用作自绘。
 */
typedef struct _view_t {
  widget_t widget;
} view_t;

/**
 * @method view_create
 * 创建view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

END_C_DECLS

#endif /*TK_VIEW_H*/

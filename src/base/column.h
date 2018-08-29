/**
 * File:   column.h
 * Author: AWTK Develop Team
 * Brief:  column
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLUMN_H
#define TK_COLUMN_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class column_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * column。一个简单的容器控件，垂直排列一组控件。
 * 它本身不提供布局功能(可用layout参数指定)，和view功能一致，仅提供具有语义的标签，让xml更具有可读性。
 */
typedef struct _column_t {
  widget_t widget;
} column_t;

/**
 * @method column_create
 * 创建column对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* column_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method column_cast
 * 转换为column对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget column对象。
 *
 * @return {widget_t*} column对象。
 */
widget_t* column_cast(widget_t* widget);

END_C_DECLS

#endif /*TK_COLUMN_H*/

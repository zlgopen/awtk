/**
 * File:   grid.h
 * Author: AWTK Develop Team
 * Brief:  grid
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GRID_H
#define TK_GRID_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class grid_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * grid控件。一个简单的容器控件，用于网格排列一组控件。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * grid\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于grid\_t控件。
 *
 * 在xml中使用"grid"标签创建grid。如：
 *
 * ```xml
 *  <grid x="0" y="0" w="100%" h="100%" children_layout="default(c=2,r=2,m=5,s=5)">
 *     <button name="open:basic" text="Basic"/>
 *     <button name="open:button" text="Buttons"/>
 *     <button name="open:edit" text="Edits"/>
 *     <button name="open:keyboard" text="KeyBoard"/>
 *   </grid>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 *
 */
typedef struct _grid_t {
  widget_t widget;
} grid_t;

/**
 * @method grid_create
 * 创建grid对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* grid_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method grid_cast
 * 转换为grid对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget grid对象。
 *
 * @return {widget_t*} grid对象。
 */
widget_t* grid_cast(widget_t* widget);

#define GRID(widget) ((grid_t*)(grid_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(grid);

END_C_DECLS

#endif /*TK_GRID_H*/

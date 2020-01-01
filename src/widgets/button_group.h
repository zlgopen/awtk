/**
 * File:   button_group.h
 * Author: AWTK Develop Team
 * Brief:  button_group
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUTTON_GROUP_H
#define TK_BUTTON_GROUP_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class button_group_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * Button Group控件。一个简单的容器控件，用于容纳一组按钮控件。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * button\_group\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于button\_group\_t控件。
 *
 * 在xml中使用"button\_group"标签创建button\_group。如：
 *
 * ```xml
 * <button_group x="0" y="m" w="100%" h="40" children_layout="default(c=4,r=1,s=5,m=5)">
 *   <button name="open:basic" text="Basic"/>
 *   <button name="open:button" text="Buttons"/>
 *   <button name="open:edit" text="Edits"/>
 *   <button name="open:keyboard" text="KeyBoard"/>
 * </button_group>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 */
typedef struct _button_group_t {
  widget_t widget;
} button_group_t;

/**
 * @method button_group_create
 * 创建button_group对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method button_group_cast
 * 转换为button_group对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget button_group对象。
 *
 * @return {widget_t*} button_group对象。
 */
widget_t* button_group_cast(widget_t* widget);

#define BUTTON_GROUP(widget) ((button_group_t*)(button_group_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(button_group);

END_C_DECLS

#endif /*TK_BUTTON_GROUP_H*/

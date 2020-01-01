/**
 * File:   group_box.h
 * Author: AWTK Develop Team
 * Brief:  group_box
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
 * 2018-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GROUP_BOX_H
#define TK_GROUP_BOX_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class group_box_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 分组控件。
 *
 * 单选按钮在同一个父控件中是互斥的，所以通常将相关的单选按钮放在一个group\_box中。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * group\_box\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于group\_box\_t控件。
 *
 * 在xml中使用"group\_box"标签创建group\_box。如：
 *
 * ```xml
 * <group_box x="20" y="230" w="50%" h="90" children_layout="default(r=3,c=1,ym=2,s=10)" >
 *   <radio_button name="r1" text="Book"/>
 *   <radio_button name="r2" text="Food"/>
 *   <radio_button name="r3" text="Pencil" value="true"/>
 * </group_box>
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
typedef struct _group_box_t {
  widget_t widget;
} group_box_t;

/**
 * @method group_box_create
 * 创建group_box对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* group_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method group_box_cast
 * 转换为group_box对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget group_box对象。
 *
 * @return {widget_t*} group_box对象。
 */
widget_t* group_box_cast(widget_t* widget);

#define GROUP_BOX(widget) ((group_box_t*)(group_box_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(group_box);

END_C_DECLS

#endif /*TK_GROUP_BOX_H*/

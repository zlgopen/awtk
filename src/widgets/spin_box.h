/**
 * File:   spin_box.h
 * Author: AWTK Develop Team
 * Brief:  spin_box
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SPIN_BOX_H
#define TK_SPIN_BOX_H

#include "widgets/edit.h"

BEGIN_C_DECLS

/**
 * @class spin_box_t
 * @parent edit_t
 * @annotation ["scriptable","design","widget"]
 * spinbox控件。
 *
 * 一个特殊的数值编辑器，将edit\_t和button\_t进行组合，方便编辑数值。
 *
 * 点击向上的按钮将数值增加一个step，点击向下的按钮将数值减小一个step。
 * step的值可以通过step属性进行设置。
 *
 * spin_box\_t是[edit\_t](edit_t.md)的子类控件，edit\_t的函数均适用于spin\_box\_t控件。
 *
 * 在xml中使用"spin_box"标签创建spinbox控件。如：
 *
 * ```xml
 * <spin_box w="70%" input_type="int" min="-100" max="100" step="5">
 * ```
 *
 * >
 * 更多用法请参考：[spin_box.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/spinbox.xml)
 *
 * 在c代码中使用函数spin_box\_create创建spinbox控件。如：
 *
 * ```c
 *  widget_t* spin_box = spin_box_create(win, 10, 10, 128, 30);
 *  edit_set_input_type(spin_box, type);
 * ```
 *
 * > 创建之后:
 * >
 * > 可以用edit相关函数去设置它的各种属性。
 *
 * > 完整示例请参考：[spin_box
 * demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/spin_box.c)
 *
 * 可用通过style来设置控件的显示风格，如背景和边框等。如：
 *
 * ```xml
 * <spin_box>
 * <style name="default" border_color="#a0a0a0"  text_color="black" text_align_h="left">
 *   <normal     bg_color="#f0f0f0" />
 *   <focused    bg_color="#f0f0f0" border_color="black"/>
 *   <disable    bg_color="gray" text_color="#d0d0d0" />
 *   <error      bg_color="#f0f0f0" text_color="red" />
 *   <empty      bg_color="#f0f0f0" text_color="#a0a0a0" />
 * </style>
 * </spin_box>
 * ```
 *
 * > 更多用法请参考：[theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L128)
 *
 */
typedef struct _spin_box_t {
  edit_t edit;

  /**
   * @property {bool_t} easy_touch_mode
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用易点击模式(在电容屏设备上建议启用)。
   * > 在该模式下：
   * > * 1.当高度大于font size的3倍时，inc按钮在顶部(style名为spinbox_top)，dec按钮在底部(style名为spinbox_bottom)。
   * > * 2.当高度正常时，dec按钮在左边(style名为spinbox_left)，inc按钮在右边(style名为spinbox_right)。
   * > 不在该模式下：
   * > inc按钮在右上角(style名为spinbox_up)，dec按钮在右下角(style名为spinbox_down)。
   */
  bool_t easy_touch_mode;

  /**
   * @property {int32_t} repeat
   * @annotation ["set_prop","get_prop","readable","persitent","design","fake"]
   * 重复触发EVT\_CLICK事件的时间间隔。
   *
   * 为0则不重复触发EVT\_CLICK事件。
   */

} spin_box_t;

/**
 * @method spin_box_create
 * @annotation ["constructor", "scriptable"]
 * 创建spin_box对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* spin_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method spin_box_cast
 * 转换为spin_box对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget spin_box对象。
 *
 * @return {widget_t*} spin_box对象。
 */
widget_t* spin_box_cast(widget_t* widget);

/**
 * @method spin_box_set_easy_touch_mode
 * 设置是否启用易点击模式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} easy_touch_mode 易点击模式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t spin_box_set_easy_touch_mode(widget_t* widget, bool_t easy_touch_mode);

/**
 * @method spin_set_repeat
 * 设置连击的时间间隔。
 * 备注：时间间隔越低，速度越快。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {int32_t} repeat 连击的时间间隔。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t spin_set_repeat(widget_t* widget, int32_t repeat);

#define SPIN_BOX(widget) ((spin_box_t*)(spin_box_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(spin_box);

END_C_DECLS

#endif /*TK_SPIN_BOX_H*/

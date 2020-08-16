/**
 * File:   tab_control.h
 * Author: AWTK Develop Team
 * Brief:  tab_control
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

#ifndef TK_TAB_CONTROL_H
#define TK_TAB_CONTROL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class tab_control_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 标签控件。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 *
 * 标签控件通常会包含一个pages控件和一个tab\_button\_group控件。
 *
 * ```graphviz
 *   [default_style]
 *
 *   pages_t -> tab_control_t[arrowhead = "ediamond"]
 *   tab_button_group_t -> tab_control_t[arrowhead = "ediamond"]
 *   view_t ->pages_t[arrowhead = "ediamond"]
 *   tab_button_t -> tab_button_group_t[arrowhead = "ediamond"]
 * ```
 *
 * tab\_control\_t是[widget\_t](widget_t.md)的子类控件，
 * widget\_t的函数均适用于tab\_control\_t控件。
 *
 * 在xml中使用"tab\_control"标签创建标签控件。如：
 *
 * ```xml
 * <tab_control x="0" y="0" w="100%" h="100%" >
 *   <pages x="c" y="20" w="90%" h="-60" value="1">
 *   ...
 *   </pages>
 *   <tab_button_group>
 *   ...
 *   </tab_button_group>
 * </tab_control>
 * ```
 *
 * > 更多用法请参考：
 * [tab control](https://github.com/zlgopen/awtk/blob/master/design/default/ui/)
 *
 */
typedef struct _tab_control_t {
  widget_t widget;
} tab_control_t;

/**
 * @method tab_control_create
 * 创建tab_control对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_control_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method tab_control_cast
 * 转换tab_control对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget tab_control对象。
 *
 * @return {widget_t*} tab_control对象。
 */
widget_t* tab_control_cast(widget_t* widget);

#define TAB_CONTROL(widget) ((tab_control_t*)(tab_control_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(tab_control);

END_C_DECLS

#endif /*TK_TAB_CONTROL_H*/

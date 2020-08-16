/**
 * File:   line_number.h
 * Author: AWTK Develop Team
 * Brief:  line_number
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
 * 2019-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LINE_NUMBER_H
#define TK_LINE_NUMBER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class line_number_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 *
 * 行号。多行编辑器的行号。
 *
 * line\_number\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于line\_number\_t控件。
 *
 * 在xml中使用"lin\e_number"标签创建行号控件，一般配合mledit使用。如：
 *
 * ```xml
 * <mledit x="c" y="10" h="40%" w="90%" focus="true" left_margin="36" right_margin="16"
 wrap_word="true">
 *  <line_number x="0" y="0" w="32" h="100%" value="0"/>
 *  <scroll_bar_d x="right" y="0" w="14" h="100%" value="0"/>
 * </mledit>
 * ```
 *
 * > 更多用法请参考：[mledit.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/mledit.xml)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <line_number>
 * <style name="default">
    <normal text_color="black" bg_color="#d0d0d0" text_align_h="right"/>
 * </style>
 * </line_number>
 * ```
 *
 * > 更多用法请参考：
 * [theme default](
 *https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L556)
 *
 */
typedef struct _line_number_t {
  widget_t widget;

  /*private*/
  /*由mledit设置*/
  int32_t yoffset;
  int32_t line_height;
  int32_t top_margin;
  int32_t bottom_margin;
} line_number_t;

/**
 * @method line_number_create
 * @annotation ["constructor", "scriptable"]
 * 创建line_number对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* line_number_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method line_number_set_top_margin
 * 设置顶部边距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  top_margin 顶部边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_number_set_top_margin(widget_t* widget, int32_t top_margin);

/**
 * @method line_number_set_bottom_margin
 * 设置顶部边距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  bottom_margin 顶部边距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_number_set_bottom_margin(widget_t* widget, int32_t bottom_margin);

/**
 * @method line_number_set_line_height
 * 设置行高。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  line_height 行高。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_number_set_line_height(widget_t* widget, int32_t line_height);

/**
 * @method line_number_set_yoffset
 * 设置y偏移。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  yoffset 行高。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_number_set_yoffset(widget_t* widget, int32_t yoffset);

/**
 * @method line_number_cast
 * 转换为line_number对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget line_number对象。
 *
 * @return {widget_t*} line_number对象。
 */
widget_t* line_number_cast(widget_t* widget);

#define WIDGET_TYPE_LINE_NUMBER "line_number"
#define LINE_NUMBER(widget) ((line_number_t*)(line_number_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(line_number);

END_C_DECLS

#endif /*TK_LINE_NUMBER_H*/

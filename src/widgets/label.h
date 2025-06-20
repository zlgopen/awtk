﻿/**
 * File:   label.h
 * Author: AWTK Develop Team
 * Brief:  label
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LABEL_H
#define TK_LABEL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class label_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 *
 * 文本控件。用于显示一行或多行文本。
 *
 * 文本控件不会根据文本的长度自动换行，只有文本内容包含换行符时才会换行。
 *
 * 如需自动换行请使用[rich\_text\_t](rich_text_t.md)控件。
 *
 * label\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于label\_t控件。
 *
 * 在xml中使用"label"标签创建文本控件。如：
 *
 * ```xml
 * <label style="center" text="center"/>
 * ```
 *
 * > 更多用法请参考：[label.xml](
 *https://github.com/zlgopen/awtk/blob/master/design/default/ui/label.xml)
 *
 * 在c代码中使用函数label\_create创建文本控件。如：
 *
 * ```c
 *  widget_t* label = label_create(win, 10, 10, 128, 30);
 *  widget_set_text(label, L"hello awtk!");
 * ```
 *
 * > 创建之后，需要用widget\_set\_text或widget\_set\_text\_utf8设置文本内容。
 *
 * > 完整示例请参考：[label demo](
 *https://github.com/zlgopen/awtk-c-demos/blob/master/demos/label.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 *  <style name="left">
 *   <normal text_color="red" text_align_h="left" border_color="#a0a0a0" margin="4" />
 *  </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme default](
 *https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L144)
 *
 */
typedef struct _label_t {
  widget_t widget;

  /**
   * @property {int32_t} length
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 显示字符的个数(小于0时全部显示)。
   * 主要用于动态改变显示字符的个数，来实现类似[拨号中...]的动画效果。
   * > 和换行是冲突的，换行后，该属性不生效
   */
  int32_t length;

  /**
   * @property {bool_t} line_wrap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动换行(默认FALSE)。
   */
  bool_t line_wrap;

  /**
   * @property {bool_t} word_wrap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否允许整个单词换行(默认FALSE)。
   * > 需要开启自动换行才有效果
   */
  bool_t word_wrap;

  /**
   * @property {bool_t} ellipses
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否开启缩写，开启后，当文字长度操作控件长度后，自动变为... 
   * > 和换行是冲突的，换行后，该属性不生效
   */
  bool_t ellipses;

  /**
   * @property {int32_t} max_w
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当auto_adjust_size为TRUE时，用于控制控件的最大宽度，超出该宽度后才自动换行。
   * >为0表示忽略该参数。小于0时取父控件宽度加上max_w。
   */
  int32_t max_w;
} label_t;

/**
 * @method label_create
 * @annotation ["constructor", "scriptable"]
 * 创建label对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method label_set_length
 * 设置显示字符的个数(小于0时全部显示)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  length 最大可显示字符个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_length(widget_t* widget, int32_t length);

/**
 * @method label_set_max_w
 * 设置max_w。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  max_w 最大宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_max_w(widget_t* widget, int32_t max_w);

/**
 * @method label_set_line_wrap
 * 设置是否自动换行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  line_wrap 是否自动换行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_line_wrap(widget_t* widget, bool_t line_wrap);

/**
 * @method label_set_word_wrap
 * 设置是否允许整个单词换行。(需要开启自动换行才有效果)
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  word_wrap 是否允许整个单词换行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_word_wrap(widget_t* widget, bool_t word_wrap);

/**
 * @method label_set_ellipses
 * 是否开启缩写，开启后，当文字长度操作控件长度后，自动变为... 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  ellipses 是否开启缩写。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_set_ellipses(widget_t* widget, bool_t ellipses);

/**
 * @method label_resize_to_content
 * 根据文本内容调节控件大小。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t}  min_w 最小宽度。
 * @param {uint32_t}  max_w 最大宽度。
 * @param {uint32_t}  min_h 最小高度。
 * @param {uint32_t}  max_h 最大高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t label_resize_to_content(widget_t* widget, uint32_t min_w, uint32_t max_w, uint32_t min_h,
                              uint32_t max_h);

/**
 * @method label_cast
 * 转换为label对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget label对象。
 *
 * @return {widget_t*} label对象。
 */
widget_t* label_cast(widget_t* widget);

#define LABEL(widget) ((label_t*)(label_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(label);

/*public for test*/

END_C_DECLS

#endif /*TK_LABEL_H*/

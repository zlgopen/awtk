/**
 * File:   text_selector.h
 * Author: AWTK Develop Team
 * Brief:  text_selector
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
 * 2018-09-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TEXT_SELECTOR_H
#define TK_TEXT_SELECTOR_H

#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

typedef struct _text_selector_option_t {
  int32_t value;
  struct _text_selector_option_t* next;
  wchar_t text[2];
} text_selector_option_t;

/**
 * @class text_selector_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 文本选择器控件，通常用于选择日期和时间等。
 *
 * > XXX: 目前需要先设置options和visible_nr，再设置其它参数(在XML中也需要按此顺序)。
 *
 * text\_selector\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于text\_selector\_t控件。
 *
 * 在xml中使用"text\_selector"标签创建文本选择器控件。如：
 *
 * ```xml
 * <text_selector options="red;green;blue;gold;orange" visible_nr="3" text="red"/>
 * ```
 *
 * > 更多用法请参考：[text\_selector.xml](
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/text_selector.xml)
 *
 * 在c代码中使用函数text\_selector\_create创建文本选择器控件。如：
 *
 * ```c
 * widget_t* ts = text_selector_create(win, 10, 10, 80, 150);
 * text_selector_set_options(ts, "1:red;2:green;3:blue;4:orange;5:gold");
 * text_selector_set_value(ts, 1);
 * widget_use_style(ts, "dark");
 * ```
 *
 * > 完整示例请参考：[text\_selector demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/text_selector.c)
 *
 * 可用通过style来设置控件的显示风格，如字体和背景颜色等。如：
 *
 * ```xml
 * <style name="dark" fg_color="#a0a0a0"  text_color="black" text_align_h="center">
 *   <normal     bg_color="#ffffff" mask_color="#404040" border_color="#404040"/>
 * </style>
 * ```
 *
 * > 更多用法请参考：[theme default](
 * https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L443)
 *
 */
typedef struct _text_selector_t {
  widget_t widget;

  /**
   * @property {uint32_t} visible_nr
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 可见的选项数量(只能是3或者5，缺省为5)。
   */
  uint32_t visible_nr;

  /**
   * @property {int32_t} selected_index
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前选中的选项。
   */
  int32_t selected_index;

  /**
   * @property {char*} options
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置可选项(冒号分隔值和文本，分号分隔选项，如:1:red;2:green;3:blue)。
   * 对于数值选项，也可以指定一个范围，用『-』分隔起始值、结束值和格式。
   * 如："1-7-%02d"表示1到7，格式为『02d』，格式为可选，缺省为『%d』。
   */
  char* options;

  /*private*/
  str_t text;
  int32_t ydown;
  int32_t yoffset;
  int32_t yoffset_save;
  bool_t pressed;
  velocity_t velocity;
  widget_animator_t* wa;
  text_selector_option_t* option_items;
} text_selector_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(当前项)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(当前项)改变事件。
 */

/**
 * @method text_selector_create
 * 创建text_selector对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* text_selector_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method text_selector_cast
 * 转换text_selector对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget text_selector对象。
 *
 * @return {widget_t*} text_selector对象。
 */
widget_t* text_selector_cast(widget_t* widget);

/**
 * @method text_selector_reset_options
 * 重置所有选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_reset_options(widget_t* widget);

/**
 * @method text_selector_count_options
 * 获取选项个数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 *
 * @return {int32_t} 返回选项个数。
 */
int32_t text_selector_count_options(widget_t* widget);

/**
 * @method text_selector_append_option
 * 追加一个选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {int32_t} value 值。
 * @param {char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_append_option(widget_t* widget, int32_t value, const char* text);

/**
 * @method text_selector_set_options
 * 设置选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {char*} options 选项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_options(widget_t* widget, const char* options);

/**
 * @method text_selector_set_range_options
 * 设置一系列的整数选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {int32_t} start 起始值。
 * @param {uint32_t} nr 个数。
 * @param {int32_t} step 步长。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_range_options(widget_t* widget, int32_t start, uint32_t nr, int32_t step);

/**
 * @method text_selector_get_option
 * 获取第index个选项。
 * @param {widget_t*} widget text_selector对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {text_selector_option_t*} 成功返回选项，失败返回NULL。
 */
text_selector_option_t* text_selector_get_option(widget_t* widget, uint32_t index);

/**
 * @method text_selector_get_value
 * 获取text_selector的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 *
 * @return {int32_t} 返回值。
 */
int32_t text_selector_get_value(widget_t* widget);

/**
 * @method text_selector_set_value
 * 设置text_selector的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {int32_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_value(widget_t* widget, int32_t value);

/**
 * @method text_selector_get_text
 * 获取text_selector的文本。
 * @annotation ["scriptable"]
 * @alias text_selector_get_text_value
 * @param {widget_t*} widget text_selector对象。
 *
 * @return {const char*} 返回文本。
 */
const char* text_selector_get_text(widget_t* widget);

/**
 * @method text_selector_set_text
 * 设置text_selector的文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {const char*} text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_text(widget_t* widget, const char* text);

/**
 * @method text_selector_set_selected_index
 * 设置第index个选项为当前选中的选项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {uint32_t} index 选项的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_selected_index(widget_t* widget, uint32_t index);

/**
 * @method text_selector_set_visible_nr
 * 设置可见的选项数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget text_selector对象。
 * @param {uint32_t} visible_nr 选项数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_selector_set_visible_nr(widget_t* widget, uint32_t visible_nr);

#define TEXT_SELECTOR_PROP_VISIBLE_NR "visible_nr"
#define WIDGET_TYPE_TEXT_SELECTOR "text_selector"
#define TEXT_SELECTOR(widget) ((text_selector_t*)(text_selector_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(text_selector);

/*public for test*/
ret_t text_selector_parse_options(widget_t* widget, const char* str);

END_C_DECLS

#endif /*TK_TEXT_SELECTOR_H*/

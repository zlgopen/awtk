/**
 * File:   mledit.h
 * Author: AWTK Develop Team
 * Brief:  mledit
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
 * 2019-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MLEDIT_H
#define TK_MLEDIT_H

#include "base/widget.h"
#include "base/text_edit.h"

BEGIN_C_DECLS

/**
 * @class mledit_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 多行编辑器控件。
 *
 * mledit\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于mledit\_t控件。
 *
 * 在xml中使用"mledit"标签创建多行编辑器控件。如：
 *
 * ```xml
 * <mledit x="c" y="m" w="300" h="300" />
 * ```
 *
 * > 更多用法请参考：[mledit.xml](
 * https://github.com/zlgopen/awtk/blob/master/design/default/ui/mledit.xml)
 *
 * 在c代码中使用函数mledit\_create创建多行编辑器控件。如：
 *
 * ```c
 * widget_t* tc = mledit_create(win, 10, 10, 240, 240);
 * ```
 *
 * > 完整示例请参考：[mledit demo](
 * https://github.com/zlgopen/awtk-c-demos/blob/master/demos/mledit.c)
 *
 * time\_clock一般不需要设置style。
 *
 */
typedef struct _mledit_t {
  widget_t widget;

  /**
   * @property {bool_t} readonly
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 编辑器是否为只读。
   */
  bool_t readonly;
  /**
   * @property {uint8_t} top_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 上边距。
   */
  uint8_t top_margin;
  /**
   * @property {uint8_t} bottom_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 下边距。
   */
  uint8_t bottom_margin;
  /**
   * @property {uint8_t} left_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 左边距。
   */
  uint8_t left_margin;
  /**
   * @property {uint8_t} right_margin
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 右边距。
   */
  uint8_t right_margin;
  /**
   * @property {char*} tips
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 输入提示。
   */
  char* tips;
  /**
   * @property {char*} tr_tips
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 保存用于翻译的提示信息。
   */
  char* tr_tips;
  /**
   * @property {char*} keyboard
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 自定义软键盘名称。
   */
  char* keyboard;

  /**
   * @property {bool_t} wrap_word
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动折行。
   */
  bool_t wrap_word;

  /**
   * @property {uint32_t} max_lines
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大行数。
   */
  uint32_t max_lines;

  /**
   * @property {uint32_t} scroll_line
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 鼠标一次滚动行数。
   */
  uint32_t scroll_line;

  /*private*/
  text_edit_t* model;
  uint32_t timer_id;

  wstr_t temp;
  uint64_t last_user_action_time;
} mledit_t;

/**
 * @event {event_t} EVT_VALUE_CHANGING
 * 文本正在改变事件(编辑中)。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 文本改变事件。
 */

/**
 * @method mledit_create
 * 创建mledit对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* mledit_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method mledit_set_readonly
 * 设置编辑器是否为只读。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} readonly 只读。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_readonly(widget_t* widget, bool_t readonly);

/**
 * @method mledit_set_focus
 * 设置为焦点。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} focus 是否为焦点。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_focus(widget_t* widget, bool_t focus);

/**
 * @method mledit_set_wrap_word
 * 设置编辑器是否自动折行。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} wrap_word 是否自动折行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_wrap_word(widget_t* widget, bool_t wrap_word);

/**
 * @method mledit_set_max_lines
 * 设置编辑器的最大行数。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} max_lines 最大行数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_max_lines(widget_t* widget, uint32_t max_lines);

/**
 * @method mledit_set_tips
 * 设置编辑器的输入提示。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {char*} tips 输入提示。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_tips(widget_t* widget, const char* tips);

/**
 * @method mledit_set_tr_tips
 * 获取翻译之后的文本，然后调用mledit_set_tips。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {const char*}  tr_tips 提示信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_tr_tips(widget_t* widget, const char* tr_tips);

/**
 * @method mledit_set_keyboard
 * 设置自定义软键盘名称。
 * 
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {char*} keyboard 键盘名称(相应UI资源必须存在)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_keyboard(widget_t* widget, const char* keyboard);

/**
 * @method mledit_set_cursor
 * 设置编辑器光标位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} cursor 光标位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_cursor(widget_t* widget, uint32_t cursor);

/**
 * @method mledit_set_scroll_line
 * 设置编辑器滚动速度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} scroll_line 滚动行数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_scroll_line(widget_t* widget, uint32_t scroll_line);

/**
 * @method mledit_cast
 * 转换为mledit对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget mledit对象。
 *
 * @return {widget_t*} mledit对象。
 */
widget_t* mledit_cast(widget_t* widget);

#define WIDGET_TYPE_MLEDIT "mledit"

#define MLEDIT_PROP_MAX_LINES "max_lines"
#define MLEDIT_PROP_WRAP_WORD "wrap_word"
#define MLEDIT_PROP_SCROLL_LINE "scroll_line"
#define MLEDIT(widget) ((mledit_t*)(mledit_cast(WIDGET(widget))))

/*for compatability*/
#define mledit_set_input_tips(w, t) mledit_set_tips(w, t)

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(mledit);

END_C_DECLS

#endif /*TK_MLEDIT_H*/

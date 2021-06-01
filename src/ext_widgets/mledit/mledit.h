/**
 * File:   mledit.h
 * Author: AWTK Develop Team
 * Brief:  mledit
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
   * @property {uint32_t} max_lines
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大行数。
   */
  uint32_t max_lines;

  /**
   * @property {uint32_t} max_chars
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大字符数。
   */
  uint32_t max_chars;

  /**
   * @property {bool_t} wrap_word
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否自动折行。
   */
  bool_t wrap_word;
  /**
   * @property {uint32_t} scroll_line
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 鼠标一次滚动行数。
   */
  uint32_t scroll_line;
  /**
   * @property {bool_t} readonly
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 编辑器是否为只读。
   */
  bool_t readonly;

  /**
   * @property {bool_t} cancelable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否支持撤销编辑。如果为TRUE，在失去焦点之前可以撤销所有修改(恢复获得焦点之前的内容)。
   *
   * > * 1.一般配合keyboard的"cancel"按钮使用。
   * > * 2.为TRUE时，如果内容有变化，会设置编辑器的状态为changed，所以此时编辑器需要支持changed状态的style。
   */
  bool_t cancelable;
  /**
   * @property {bool_t} open_im_when_focused
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 获得焦点时打开输入法。
   *
   * > 主要用于没有指针设备的情况，否则每次切换焦点时都打开输入法。
   */
  bool_t open_im_when_focused;
  /**
   * @property {bool_t} close_im_when_blured
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 
   * 是否在失去焦点时关闭输入法(默认是)。
   *
   */
  bool_t close_im_when_blured;
  /*private*/
  uint8_t margin;
  uint8_t top_margin;
  uint8_t left_margin;
  uint8_t right_margin;
  uint8_t bottom_margin;

  text_edit_t* model;
  uint32_t timer_id;

  wstr_t temp;
  wstr_t saved_text;
  uint64_t last_user_action_time;
} mledit_t;

/**
 * @event {value_change_event_t} EVT_VALUE_CHANGING
 * 文本正在改变事件(编辑中)。
 */

/**
 * @event {value_change_event_t} EVT_VALUE_CHANGED
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
 * @method mledit_set_cancelable
 * 设置编辑器是否为可撤销修改。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} cancelable 是否为可撤销修。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_cancelable(widget_t* widget, bool_t cancelable);

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
 * @method mledit_set_max_chars
 * 设置编辑器的最大字符数（0 为不限制字符数）。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} max_chars 最大字符数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_max_chars(widget_t* widget, uint32_t max_chars);

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
 * @method mledit_get_cursor
 * 获取编辑器光标位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {uint32_t} 返回光标位置。
 */
uint32_t mledit_get_cursor(widget_t* widget);

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
 * @method mledit_scroll_to_offset
 * 设置编辑器滚动到指定偏移位置。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} offset 偏移位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_scroll_to_offset(widget_t* widget, uint32_t offset);

/**
 * @method mledit_set_open_im_when_focused
 * 设置编辑器是否在获得焦点时打开输入法。
 *
 *> * 设置默认焦点时，打开窗口时不弹出软键盘。
 *> * 用键盘切换焦点时，编辑器获得焦点时不弹出软键盘。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} open_im_when_focused 是否在获得焦点时打开输入法。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_open_im_when_focused(widget_t* widget, bool_t open_im_when_focused);

/**
 * @method mledit_set_close_im_when_blured
 * 设置编辑器是否在失去焦点时关闭输入法。
 *
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {bool_t} close_im_when_blured 是否是否在失去焦点时关闭输入法。在失去焦点时关闭输入法。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_close_im_when_blured(widget_t* widget, bool_t close_im_when_blured);

/**
 * @method mledit_set_select
 * 选择编辑器中指定范围的文本。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 * @param {uint32_t} start 起始偏移。
 * @param {uint32_t} end 结束偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mledit_set_select(widget_t* widget, uint32_t start, uint32_t end);

/**
 * @method mledit_get_selected_text
 * 获取选中的文本。
 * 使用完后需调用 TKMEM_FREE() 进行释放文本占有内存。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget widget对象。
 *
 * @return {char*} 返回选中文本。
 */
char* mledit_get_selected_text(widget_t* widget);

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
#define MLEDIT_PROP_MAX_CHARS "max_chars"
#define MLEDIT_PROP_WRAP_WORD "wrap_word"
#define MLEDIT_PROP_SCROLL_LINE "scroll_line"
#define MLEDIT(widget) ((mledit_t*)(mledit_cast(WIDGET(widget))))

/*for compatability*/
#define mledit_set_input_tips(w, t) mledit_set_tips(w, t)

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(mledit);

END_C_DECLS

#endif /*TK_MLEDIT_H*/

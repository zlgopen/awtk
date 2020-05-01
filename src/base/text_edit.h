/**
 * File:   text_edit.h
 * Author: AWTK Develop Team
 * Brief:  text_edit
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

#ifndef TK_TEXT_EDIT_H
#define TK_TEXT_EDIT_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class text_edit_state_t
 */
typedef struct _text_edit_state_t {
  int32_t ox;
  int32_t oy;
  int32_t virtual_w;
  int32_t virtual_h;
  uint32_t rows;
  uint32_t line_height;
  point_t caret;
  uint32_t cursor;
  uint32_t max_rows;
  uint32_t select_start;
  uint32_t select_end;
  uint32_t last_line_number;

  bool_t mask;
  bool_t preedit;
  bool_t wrap_word;
  wchar_t mask_char;
  bool_t caret_visible;
  bool_t single_line;
} text_edit_state_t;

typedef ret_t (*text_edit_on_state_changed_t)(void* ctx, text_edit_state_t* state);

/**
 * @class text_edit_t
 */
typedef struct _text_edit_t {
  canvas_t* c;
  widget_t* widget;

} text_edit_t;

/**
 * @method text_edit_create
 * 创建text_edit对象
 * @param {widget_t*} widget 控件
 * @param {boo_t} single_line 是否是单行编辑器。
 *
 * @return {widget_t*} 对象。
 */
text_edit_t* text_edit_create(widget_t* widget, bool_t single_line);

/**
 * @method text_edit_set_canvas
 * 设置canvas对象。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_canvas(text_edit_t* text_edit, canvas_t* c);

/**
 * @method text_edit_click
 * 处理点击事件。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_click(text_edit_t* text_edit, xy_t x, xy_t y);

/**
 * @method text_edit_drag
 * 处理拖动事件。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_drag(text_edit_t* text_edit, xy_t x, xy_t y);

/**
 * @method text_edit_paste
 * 粘贴文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {const wchar_t*} str 文本。
 * @param {uint32_t} size 文本长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_paste(text_edit_t* text_edit, const wchar_t* str, uint32_t size);

/**
 * @method text_edit_cut
 * 剪切文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_cut(text_edit_t* text_edit);

/**
 * @method text_edit_copy
 * 拷贝文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_copy(text_edit_t* text_edit);

/**
 * @method text_edit_key_down
 * 处理按键事件。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {key_event_t*} evt event
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_key_down(text_edit_t* text_edit, key_event_t* evt);

/**
 * @method text_edit_key_up
 * 处理按键事件。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {key_event_t*} evt event
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_key_up(text_edit_t* text_edit, key_event_t* evt);

/**
 * @method text_edit_set_select
 * 选择指定范围的文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {uint32_t} start 起始偏移。
 * @param {uint32_t} end 结束偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_select(text_edit_t* text_edit, uint32_t start, uint32_t end);

/**
 * @method text_edit_select_all
 * 全选。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_select_all(text_edit_t* text_edit);

/**
 * @method text_edit_unselect
 * 取消选择。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_unselect(text_edit_t* text_edit);

/**
 * @method text_edit_set_cursor
 * 设置光标位置。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {uint32_t} cursor 光标偏移。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_cursor(text_edit_t* text_edit, uint32_t cursor);

/**
 * @method text_edit_get_cursor
 * 获取光标位置。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {uin32_t} 返回光标的位置。
 */
uint32_t text_edit_get_cursor(text_edit_t* text_edit);

/**
 * @method text_edit_get_state
 * 获取编辑器的状态。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {text_edit_state_t*} state 状态。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_get_state(text_edit_t* text_edit, text_edit_state_t* state);

/**
 * @method text_edit_set_wrap_word
 * 设置是否自动折行。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {bool_t} wrap_word 是否自动折行。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_wrap_word(text_edit_t* text_edit, bool_t wrap_word);

/**
 * @method text_edit_invert_caret_visible
 * 如果caret可见，将其设置为不可见。 如果caret不可见，将其设置为可见。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_invert_caret_visible(text_edit_t* text_edit);

/**
 * @method text_edit_set_caret_visible
 * 设置光标的看见性。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {bool_t} caret_visible 是否可见。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_caret_visible(text_edit_t* text_edit, bool_t caret_visible);

/**
 * @method text_edit_set_max_rows
 * 设置最大行数。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {uint32_t} max_rows 最大行数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_max_rows(text_edit_t* text_edit, uint32_t max_rows);

/**
 * @method text_edit_set_mask
 * 设置是否马赛克字符(用于密码)。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {bool_t} mask 是否马赛克字符。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_mask(text_edit_t* text_edit, bool_t mask);

/**
 * @method text_edit_set_tips
 * 设置提示信息。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {const char*} tips 提示信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_tips(text_edit_t* text_edit, const char* tips);

/**
 * @method text_edit_set_mask_char
 * 设置马赛克字符。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {wchar_t} mask_char 马赛克字符。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_mask_char(text_edit_t* text_edit, wchar_t mask_char);

/**
 * @method text_edit_paint
 * 绘制文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {canvas_t*} c canvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_paint(text_edit_t* text_edit, canvas_t* c);

/**
 * @method text_edit_layout
 * 重新排版。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_layout(text_edit_t* text_edit);

/**
 * @method text_edit_set_offset
 * 设置滚动偏移。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {int32_t} ox x偏移量。
 * @param {int32_t} oy y偏移量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_offset(text_edit_t* text_edit, int32_t ox, int32_t oy);

/**
 * @method text_edit_set_on_state_changed
 * 设置状态改变回调函数。
 * @param {text_edit_t*} text_edit text_edit对象。
 * @param {text_edit_on_state_changed_t} on_state_changed 回调函数。
 * @param {void*} ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_set_on_state_changed(text_edit_t* text_edit,
                                     text_edit_on_state_changed_t on_state_changed, void* ctx);

/**
 * @method text_edit_destroy
 * 销毁text_edit对象。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_destroy(text_edit_t* text_edit);

/**
 * @method text_edit_preedit_clear
 * 清除预编辑文本。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_preedit_clear(text_edit_t* text_edit);

/**
 * @method text_edit_preedit
 * 进入预编辑状态。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_preedit(text_edit_t* text_edit);

/**
 * @method text_edit_preedit_confirm
 * 提交预编辑的文本，并退出预编辑状态。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_preedit_confirm(text_edit_t* text_edit);

/**
 * @method text_edit_preedit_abort
 * 取消预编辑的文本，并退出预编辑状态。
 * @param {text_edit_t*} text_edit text_edit对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t text_edit_preedit_abort(text_edit_t* text_edit);

END_C_DECLS

#endif /*TK_TEXT_EDIT_H*/

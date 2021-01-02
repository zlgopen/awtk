/**
 * File:   line_break.h
 * Author: AWTK Develop Team
 * Brief:  line break and work break algorithm.
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LINE_BREAK_H
#define TK_LINE_BREAK_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef ret_t (*line_break_on_line_t)(void* ctx, uint32_t index, const wchar_t* str, uint32_t size);

/**
 * @enum line_break_type_t
 * @prefix LINE_BREAK_
 * 换行类型。
 */
typedef enum _line_break_type_t {
  /**
   * @const LINE_BREAK_MUST
   * 必须换行。
   */
  LINE_BREAK_MUST = 0,
  /**
   * @const LINE_BREAK_ALLOW
   * 可以换行。
   */
  LINE_BREAK_ALLOW,
  /**
   * @const LINE_BREAK_NO
   * 不可以换行。
   */
  LINE_BREAK_NO
} break_type_t,
    line_break_type_t;

/**
 * @class line_break_check
 * 换行相关函数。
 * @annotation ["fake"]
 *
 */

/**
 * @method line_break_check
 * 检查两个字符之间是否允许换行。
 *
 * @param {wchar_t} c1 前面的字符。
 * @param {wchar_t} c2 后面的字符。
 *
 * @return {line_break_type_t} 返回类型。
 */
line_break_type_t line_break_check(wchar_t c1, wchar_t c2);

/**
 * @method word_break_check
 * 检查两个字符之间是否允许单词分割。
 *
 * @param {wchar_t} c1 前面的字符。
 * @param {wchar_t} c2 后面的字符。
 *
 * @return {line_break_type_t} 返回类型。
 */
line_break_type_t word_break_check(wchar_t c1, wchar_t c2);

/**
 * @method line_break_count
 * 统计行数。
 *
 * @param {const wchar_t*} str 字符串。
 *
 * @return {uint32_t} 返回行数。
 */
uint32_t line_break_count(const wchar_t* str);

/**
 * @method line_break
 *
 * 解析字符串，对于每一行调用on_line函数。
 *
 * @param {const wchar_t*} str 字符串。
 * @param {line_break_on_line_t} on_line 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t line_break(const wchar_t* str, line_break_on_line_t on_line, void* ctx);

END_C_DECLS

#endif /*TK_LINE_BREAK_H*/

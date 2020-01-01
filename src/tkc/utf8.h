/**
 * File:   utf8.h
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UTF8_H
#define TK_UTF8_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class utf8_t
 * @annotation ["fake"]
 * wchar_t和char类型转换接口。
 *
 * 示例：
 *
 * ```c
 * const char* str = "hello";
 * const wchar_t* wstr = L"hello";
 * char res_str[128];
 * wchar_t res_wstr[128];

 * tk_utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
 * tk_utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str));
 * ```
 */

/**
 * @method tk_utf8_from_utf16
 *
 * 把ucs字符串转成UTF8字符串。
 *
 * @param {const wchar_t*} in 输入字符串。
 * @param {const char*} out 输出字符串。
 * @param {uint32_t} out_size 输出字符串的最大长度。
 *
 *
 * @return {char*} 返回UTF8字符串。
 */
char* tk_utf8_from_utf16(const wchar_t* in, char* out, uint32_t size);

/**
 * @method tk_utf8_from_utf16_ex
 *
 * 把ucs字符串转成UTF8字符串。
 *
 * @param {const wchar_t*} in 输入字符串。
 * @param {uint32_t} in_size 输入字符串的长度。
 * @param {const char*} out 输出字符串。
 * @param {uint32_t} out_size 输出字符串的最大长度。
 *
 *
 * @return {char*} 返回UTF8字符串。
 */
char* tk_utf8_from_utf16_ex(const wchar_t* in, uint32_t in_size, char* out, uint32_t out_size);

/**
 * @method tk_utf8_to_utf16
 *
 * 将char类型转换为wchar_t类型。
 *
 * @param {const char*} str str。
 * @param {const wchar_t*} out 返回结果缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 *
 * @return {wchar_t*} 值。
 */
wchar_t* tk_utf8_to_utf16(const char* str, wchar_t* out, uint32_t size);

END_C_DECLS

#endif /*TK_UTF8_H*/

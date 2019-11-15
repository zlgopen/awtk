/**
 * File:   utf8.h
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

 * utf8_to_utf16(str, res_wstr, ARRAY_SIZE(res_wstr));
 * utf8_from_utf16(wstr, res_str, ARRAY_SIZE(res_str));
 * ```
 */

/**
 * @method utf8_from_utf16
 *
 * 将wchar_t类型转换为char类型。
 *
 * @param {const wchar_t*} str str。
 * @param {const char*} out 返回结果缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 *
 * @return {char*} 值。
 */
char* utf8_from_utf16(const wchar_t* str, char* out, uint32_t size);

/**
 * @method utf8_from_utf16
 *
 * 将char类型转换为wchar_t类型。
 *
 * @param {const char*} str str。
 * @param {const wchar_t*} out 返回结果缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 *
 * @return {wchar_t*} 值。
 */
wchar_t* utf8_to_utf16(const char* str, wchar_t* out, uint32_t size);

END_C_DECLS

#endif /*TK_UTF8_H*/

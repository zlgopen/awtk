/**
 * File:   utf8.h
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
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

/**
 * @method tk_utf8_to_utf16_ex
 *
 * 将char类型转换为wchar_t类型。
 *
 * @param {const char*} str str。
 * @param {uint32_t} size 缓冲区大小。
 * @param {const wchar_t*} out 返回结果缓冲区。
 * @param {uint32_t} out_size 缓冲区大小。
 *
 * @return {wchar_t*} 值。
 */
wchar_t* tk_utf8_to_utf16_ex(const char* str, uint32_t size, wchar_t* out, uint32_t out_size);

/**
 * @method tk_utf8_dup_utf16
 *
 * 把ucs字符串转成UTF8字符串。
 * > 由调用者释放返回的指针。
 *
 * @param {const wchar_t*} in 输入字符串。
 * @param {int32_t} size 字符串长度。
 *
 * @return {char*} 返回UTF8字符串。
 */
char* tk_utf8_dup_utf16(const wchar_t* in, int32_t size);

/**
 * @method tk_utf8_get_bytes_of_leading
 * 获取第一个字节为c的字符的字节数。 
 * @param {uint8_t} c 字符的第一个字节。
 *
 * @return {uint32_t} 返回字符的字节数。
 */
uint32_t tk_utf8_get_bytes_of_leading(uint8_t c);

/**
 * @method tk_utf8_trim_invalid_char
 *
 * 如果字符串最后一个字符串是无效的，删掉该无效字符。
 *
 * > 对于用strncpy/snprintf等生成字符串时，如果目标字符串内存大小不够，
 * > 可能最后一个字符被从中间截断，导致该字符是无效的，这可能会触发assert，
 * > 本函数可以用来删除最后一个无效字符。
 *
 * @param {const char*} str 输入字符串。
 *
 * @return {char*} 返回UTF8字符串。
 */
char* tk_utf8_trim_invalid_char(char* str);

END_C_DECLS

#endif /*TK_UTF8_H*/

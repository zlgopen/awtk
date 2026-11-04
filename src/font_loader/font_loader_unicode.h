/**
 * File:   font_loader_unicode.h
 * Author: AWTK Develop Team
 * Brief:  font loader unicode helpers
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-10-30 Yang Zewu <yangzewu@zlg.cn> created
 *
 */

#ifndef TK_FONT_LOADER_UNICODE_H
#define TK_FONT_LOADER_UNICODE_H

#include "base/font.h"

BEGIN_C_DECLS

/**
 * @method font_loader_get_codepoint
 * 获取 Unicode 标量值，UTF-16 代理对会合并成一个码点。
 * @annotation ["scriptable"]
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} size 字符串码元长度。
 * @param {uint32_t*} index 输入码元位置，返回码点起始位置。
 * @param {uint32_t*} code_unit_count 返回码点占用的码元个数。
 *
 * @return {uint32_t} Unicode 标量值。
 */
uint32_t font_loader_get_codepoint(const wchar_t* str, uint32_t size, uint32_t* index,
                                   uint32_t* code_unit_count);

/**
 * @method font_loader_get_str_index_from_codepoint_index
 * 获取码点序号对应的码元位置，代理对返回高代理的位置。
 * @annotation ["scriptable"]
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} size 字符串码元长度。
 * @param {uint32_t} codepoint_index Unicode 码点序号。
 *
 * @return {uint32_t} 码元位置。
 */
uint32_t font_loader_get_str_index_from_codepoint_index(const wchar_t* str, uint32_t size,
                                                        uint32_t codepoint_index);

/**
 * @method font_loader_get_codepoint_count
 * 获取字符串中的 Unicode 码点数量。
 * @annotation ["scriptable"]
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} size 字符串码元长度。
 *
 * @return {uint32_t} Unicode码点数量。
 */
uint32_t font_loader_get_codepoint_count(const wchar_t* str, uint32_t size);

END_C_DECLS

#endif /*TK_FONT_LOADER_UNICODE_H*/

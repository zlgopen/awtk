/**
 * File:   encoding.h
 * Author: AWTK Develop Team
 * Brief:  encoding conversion
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; withto even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-08-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ENCODING_H
#define TK_ENCODING_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum encoding_name_t
 * 编码名称。
 */

typedef enum _encoding_name_t {
  /**
   * @const ENCODING_UTF8
   * UTF-8。
   */
  ENCODING_UTF8 = 0,
  /**
   * @const ENCODING_UTF16
   * UTF-16。
   */
  ENCODING_UTF16,
  /**
   * @const ENCODING_UTF32
   * UTF-32。
   */
  ENCODING_UTF32,
  /**
   * @const ENCODING_GBK
   * GBK。
   */
  ENCODING_GBK,
  /**
   * @const ENCODING_BIG
   * BIG5-2003。
   */
  ENCODING_BIG,
  /**
   * @const ENCODING_GB2312
   * GB_2312-80。
   */
  ENCODING_GB2312,
  /**
   * @const ENCODING_GB18030
   * GB18030。
   */
  ENCODING_GB18030
} encoding_name_t;

/**
 * @class encoding_t
 * @annotation ["fake"]
 * encoding conversion
 *
 * 示例：
 *
 * ```c
 * ```
 */

/**
 * @method encoding_convert
 *
 * 字符集转换。
 *
 * @annotation ["static"]
 *
 * @param {encoding_name_t} from 源字符串的编码名称。
 * @param {const char*} from_str 源字符串。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {encoding_name_t} to 目标字符串的编码名称。
 * @param {const char*} to_str目标字符串。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_convert(encoding_name_t from, const char* from_str, uint32_t from_size,
                       encoding_name_t to, char* to_str, uint32_t to_size);

/**
 * @method encoding_utf8_to_gbk
 *
 * 将utf8转换成gbk编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串(utf8)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str 目标字符串(gbk)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_utf8_to_gbk(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);

/**
 * @method encoding_gbk_to_utf8
 *
 * 将gbk转换成utf8编码。
 *
 * @annotation ["static"]
 *
 * @param {const char*} from_str 源字符串(gbk)。
 * @param {uint32_t} from_size 源字符串的长度。
 * @param {const char*} to_str目标字符串(utf8)。
 * @param {uint32_t} to_size 目标字符串的最大长度。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t encoding_gbk_to_utf8(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size);

END_C_DECLS

#endif /*TK_ENCODING_H*/

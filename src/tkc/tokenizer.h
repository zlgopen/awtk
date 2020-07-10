/**
 * File:   tokenizer.h
 * Author: AWTK Develop Team
 * Brief:  tokenizer
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is ditokenizeributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-06-17 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_TOKENIZER_H
#define TK_TOKENIZER_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class tokenizer_t
 * 从字符串中解析出一个一个的token。
 * 
 * ```c
 * tokenizer_t tokenizer;
 * tokenizer_t* t = tokenizer_init(&tokenizer, "20,111.2,22.3,333.3,44,555.5", 0xffffff, ",");
 *
 * while(tokenizer_has_more(t)) {
 *   double v = tokenizer_next_float(t, 0);
 *   log_debug("%lf\n", v);
 * }
 *
 * tokenizer_deinit(t);
 * ```
 */
typedef struct _tokenizer_t {
  /**
   * @property {char*} str
   * @annotation ["readable"]
   * 字符串。
   */
  const char* str;
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 字符串的长度。
   */
  uint32_t size;
  /**
   * @property {uint32_t} cursor
   * @annotation ["readable"]
   * 当前位置。
   */
  uint32_t cursor;

  /**
   * @property {char*} separtor
   * @annotation ["readable"]
   * 分隔字符串。
   */
  const char* separtor;

  /**
   * @property {char*} single_char_token
   * @annotation ["readable"]
   * 单字符的token。
   */
  const char* single_char_token;

  str_t token;
} tokenizer_t;

/**
 * @method tokenizer_init
 * 初始化tokenizer对象。
 * @annotation ["constructor"]
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {char*} str 要解析的字符串。
 * @param {uint32_t} size 字符串长度。
 * @param {char*} separtor 分隔字符。
 *
 * @return {tokenizer_t*} tokenizer对象本身。
 */
tokenizer_t* tokenizer_init(tokenizer_t* tokenizer, const char* str, uint32_t size,
                            const char* separtor);

/**
 * @method tokenizer_init_ex
 * 初始化tokenizer对象。
 * @annotation ["constructor"]
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {char*} str 要解析的字符串。
 * @param {uint32_t} size 字符串长度。
 * @param {char*} separtor 分隔字符。
 * @param {char*} single_char_token 单字符token。
 *
 * @return {tokenizer_t*} tokenizer对象本身。
 */
tokenizer_t* tokenizer_init_ex(tokenizer_t* tokenizer, const char* str, uint32_t size,
                               const char* separtor, const char* single_char_token);

/**
 * @method tokenizer_has_more
 * 是否还有下一个token。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 *
 * @return {bool_t} 还有下一个token返回TRUE，否则返回FALSE。
 */
bool_t tokenizer_has_more(tokenizer_t* tokenizer);

/**
 * @method tokenizer_next
 * 获取下一个token。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 *
 * @return {char*} 成功返回token，失败返回NULL。
 */
const char* tokenizer_next(tokenizer_t* tokenizer);

/**
 * @method tokenizer_next_until
 * 获取下一个token，该token直到遇到指定的char。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {const char*} str 字符集。
 *
 * @return {char*} 成功返回token，失败返回NULL。
 */
const char* tokenizer_next_until(tokenizer_t* tokenizer, const char* str);

/**
 * @method tokenizer_next_expr_until
 * 获取下一个expr，该expr直到遇到指定的char。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {const char*} str 字符集。
 *
 * @return {char*} 成功返回token，失败返回NULL。
 */
const char* tokenizer_next_expr_until(tokenizer_t* tokenizer, const char* str);

/**
 * @method tokenizer_next_int
 * 获取下一个token，并转换成int。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {int} defval 缺省值。
 *
 * @return {char*} 成功返回token的int值，失败返回缺省值。
 */
int tokenizer_next_int(tokenizer_t* tokenizer, int defval);

/**
 * @method tokenizer_next_float
 * 获取下一个token，并转换成float。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 * @param {float} defval 缺省值。
 *
 * @return {char*} 成功返回token的float值，失败返回缺省值。
 */
float tokenizer_next_float(tokenizer_t* tokenizer, float defval);

/**
 * @method tokenizer_deinit
 * 重置tokenizer。
 * @param {tokenizer_t*} tokenizer tokenizer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tokenizer_deinit(tokenizer_t* tokenizer);

END_C_DECLS

#endif /*TK_TOKENIZER_H*/

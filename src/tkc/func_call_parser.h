/**
 * File:   func_call_parser.h
 * Author: AWTK Develop Team
 * Brief:  func_call_parser
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is difunc_call_parseributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-10-20 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_FUNC_CALL_PARSER_H
#define TK_FUNC_CALL_PARSER_H

#include "tkc/object.h"
#include "tkc/tokenizer.h"

BEGIN_C_DECLS

struct _func_call_parser_t;
typedef struct _func_call_parser_t func_call_parser_t;

/*子类需要实现的虚函数*/
typedef ret_t (*func_call_parser_on_name_t)(func_call_parser_t* parser, const char* func_name);
typedef ret_t (*func_call_parser_on_param_t)(func_call_parser_t* parser, const char* name,
                                             const char* value);
typedef ret_t (*func_call_parser_on_done_t)(func_call_parser_t* parser);

/**
 * @class func_call_parser_t
 * 从字符串中解析出函数调用需要的参数。
 *
 * ```
 * func_call => func_name '(' params ')'
 * params => param ',' params
 * param => name '=' value
 * func_name => ID
 * name = ID
 * value = int | float | ID
 *```
 *
 * 如:move(x=10, y=20)
 * 如:rename(old_name=aa, new_name=bb)
 */
struct _func_call_parser_t {
  tokenizer_t tokenizer;
  func_call_parser_on_name_t on_name;
  func_call_parser_on_param_t on_param;
  func_call_parser_on_done_t on_done;
};

/**
 * @method func_call_parser_init
 * 初始化parser对象。
 * @annotation ["constructor"]
 * @param {func_call_parser_t*} parser parser对象。
 * @param {const char*} str 要解析的字符串。
 * @param {uint32_t} size 字符串长度。
 *
 * @return {func_call_parser_t*} parser对象本身。
 */
func_call_parser_t* func_call_parser_init(func_call_parser_t* parser, const char* str,
                                          uint32_t size);

/**
 * @method func_call_parser_parse
 * 开始解析。
 * @param {func_call_parser_t*} parser parser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t func_call_parser_parse(func_call_parser_t* parser);

/**
 * @method func_call_parser_deinit
 * 重置parser。
 * @param {func_call_parser_t*} parser parser对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t func_call_parser_deinit(func_call_parser_t* parser);

/**
 * @method func_call_parse
 * 解析参数，并将结果生成一个object对象。
 *
 *> 调用者负责释放返回的object对象。
 * @param {const char*} str 要解析的字符串。
 * @param {uint32_t} size 字符串长度。
 *
 * @return {object_t*} 返回object对象。
 */
object_t* func_call_parse(const char* str, uint32_t size);

END_C_DECLS

#endif /*TK_FUNC_CALL_PARSER_H*/

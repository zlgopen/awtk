/**
 * File:   ubjson_parser.h
 * Author: AWTK Develop Team
 * Brief:  ubjson parser
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UBJSON_PARSER_H
#define TK_UBJSON_PARSER_H

#include "tkc/object_default.h"
#include "ubjson/ubjson_reader.h"

BEGIN_C_DECLS

typedef ret_t (*ubjson_on_key_value_t)(void* ctx, const char* key, value_t* value);

/**
 * @class ubjson_parser_t
 * @annotation ["fake"]
 * ubjson parser
 */

/**
 * @method ubjson_parse
 * @annotation ["static"]
 *
 * 解析ubjson数据，遇到key/value时调用提供的回调函数。
 *
 * @param {void*} data 数据。
 * @param {uint32_t} size 数据的长度。
 * @param {ubjson_on_key_value_t} on_key_value 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_parse(void* data, uint32_t size, ubjson_on_key_value_t on_key_value, void* ctx);

/**
 * @method ubjson_to_object
 * @annotation ["static"]
 *
 * 解析ubjson数据，生成object对象。
 *
 * @param {void*} data 数据。
 * @param {uint32_t} size 数据的长度。
 *
 * @return {void*} 返回object表示的数据。
 */
object_t* ubjson_to_object(void* data, uint32_t size);

/**
 * @method ubjson_dump
 * @annotation ["static"]
 *
 * 将ubjson数据打印到控制台。
 *
 * @param {void*} data 数据。
 * @param {uint32_t} size 数据的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_dump(void* data, uint32_t size);

#define object_from_ubjson ubjson_to_object
END_C_DECLS

#endif /*TK_UBJSON_PARSER_H*/

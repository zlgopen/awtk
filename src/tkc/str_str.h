/**
 * File:   str_to_str.h
 * Author: AWTK Develop Team
 * Brief:  map one str to another str
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
 * 2019-01-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STR_STR_H
#define TK_STR_STR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class str_str_t
 * @annotation ["fake"]
 * 字符串-字符串类型。
 *
 * 负责把一个字符串映射成另外一个字符串。
 *
 * 示例：
 *
 * ```c
 * static const str_str_t color_values[] = {{"1", "red"}, {"2", "black"}, {"3", "blue"}, {"4", "white"}};
 * const char* value = str_str_value(color_values, "3");
 * const char* name = str_str_name(color_values, "blue", "0");
 * printf("value = %s \n", value);
 * printf("name = %s \n", name);
 * ```
 */
typedef struct _str_str_t {
  const char* name;
  const char* value;
} str_str_t;

/**
 * @method str_str_value
 * 根据name获取对应的value。
 *
 * @param {const str_str_t*} items str_str_t数组。
 * @param {const char*} name name。
 *
 * @return {const char*} 返回value。
 */
const char* str_str_value(const str_str_t* items, const char* name);

/**
 * @method str_str_name
 * 根据value获取对应的name。
 *
 * @param {const str_str_t*} items str_str_t数组。
 * @param {const char*} value value
 *
 * @return {const char*} 返回name。
 */
const char* str_str_name(const str_str_t* items, const char* value);

END_C_DECLS

#endif /*TK_STR_STR_H*/

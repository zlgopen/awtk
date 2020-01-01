/**
 * File:   int_str.h
 * Author: AWTK Develop Team
 * Brief:  map between int and str
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

#ifndef TK_INT_STR_H
#define TK_INT_STR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class int_str_t
 * @annotation ["fake"]
 * 数字-字符串类型。
 *
 * 负责把一个数字映射成一个字符串。
 *
 * 示例：
 *
 * ```c
 * static const int_str_t color_values[] = {{1, "red"}, {2, "black"}, {3, "blue"}, {4, "white"}};
 * const char* value = int_str_value(color_values, 3);
 * int32_t name = int_str_name(color_values, "blue", 0);
 * printf("value = %s \n", value);
 * printf("name = %d \n", name);
 * ```
 */
typedef struct _int_str_t {
  int32_t name;
  const char* value;
} int_str_t;

/**
 * @method int_str_value
 * 根据name获取对应的value。
 *
 * @param {const int_str_t*} items int_str_t数组。
 * @param {int32_t} name name。
 *
 * @return {const char*} 返回value。
 */
const char* int_str_value(const int_str_t* items, int32_t name);

/**
 * @method int_str_name
 * 根据value获取对应的name。
 *
 * @param {const int_str_t*} items int_str_t数组。
 * @param {const char*} value value。
 * @param {int32_t} defval 如果没有找到对应的name，则返回的defval默认值。
 *
 * @return {int32_t} 返回name。
 */
int32_t int_str_name(const int_str_t* items, const char* value, int32_t defval);

END_C_DECLS

#endif /*TK_INT_STR_H*/

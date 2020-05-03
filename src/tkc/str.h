/**
 * File:   str.h
 * Author: AWTK Develop Team
 * Brief:  string
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
 * 2018-04-30 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_STR_H
#define TK_STR_H

#include "tkc/value.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class str_t
 * 可变长度的UTF8字符串。
 *
 * 示例：
 *
 * ```c
 *  str_t s;
 *  str_init(&s, 0);
 *
 *  str_append(&s, "abc");
 *  str_append(&s, "123");
 *  log_debug("%s\n", s.str);
 *
 *  str_reset(&s);
 * ```
 *
 * > 先调str\_init进行初始化，最后调用str\_reset释放内存。
 *
 */
typedef struct _str_t {
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 长度。
   */
  uint32_t size;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 容量。
   */
  uint32_t capacity;
  /**
   * @property {char*} str
   * @annotation ["readable"]
   * 字符串。
   */
  char* str;
} str_t;

/**
 * @method str_init
 * 初始化字符串对象。
 * @annotation ["constructor"]
 * @param {str_t*} str str对象。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {str_t*} str对象本身。
 */
str_t* str_init(str_t* str, uint32_t capacity);

/**
 * @method str_extend
 * 扩展字符串到指定的容量。
 * @annotation ["constructor"]
 * @param {str_t*} str str对象。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_extend(str_t* str, uint32_t capacity);

/**
 * @method str_eq
 * 判断两个字符串是否相等。
 * @param {str_t*} str str对象。
 * @param {char*} text 待比较的字符串。
 *
 * @return {bool_t} 返回是否相等。
 */
bool_t str_eq(str_t* str, const char* text);

/**
 * @method str_set
 * 设置字符串。
 * @param {str_t*} str str对象。
 * @param {char*} text 要设置的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_set(str_t* str, const char* text);

/**
 * @method str_clear
 * 清除字符串内容。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_clear(str_t* str);

/**
 * @method str_set_with_len
 * 设置字符串。
 * @param {str_t*} str str对象。
 * @param {char*} text 要设置的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_set_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_append
 * 追加字符串。
 * @param {str_t*} str str对象。
 * @param {char*} text 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append(str_t* str, const char* text);

/**
 * @method str_append_more
 * 追加多个字符串。以NULL结束。
 *
 * 示例：
 *
 * ```c
 *  str_t s;
 *  str_init(&s, 0);
 *
 *  str_append_more(&s, "abc", "123", NULL);
 *  log_debug("%s\n", s.str);
 *
 *  str_reset(&s);
 * ```
 * @param {str_t*} str str对象。
 * @param {char*} text 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_more(str_t* str, const char* text, ...);

/**
 * @method str_append_with_len
 * 追加字符串。
 * @param {str_t*} str str对象。
 * @param {char*} text 要追加的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_insert
 * 插入子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {char*} text 要插入的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_insert(str_t* str, uint32_t offset, const char* text);

/**
 * @method str_insert_with_len
 * 插入子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {char*} text 要插入的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_insert_with_len(str_t* str, uint32_t offset, const char* text, uint32_t len);

/**
 * @method str_remove
 * 删除子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {uint32_t} len 长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_remove(str_t* str, uint32_t offset, uint32_t len);

/**
 * @method str_append_char
 * 追加一个字符。
 * @param {str_t*} str str对象。
 * @param {char} c 要追加的字符。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_char(str_t* str, char c);

/**
 * @method str_append_int
 * 追加一个整数。
 * @param {str_t*} str str对象。
 * @param {int32_t} value 要追加的整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_int(str_t* str, int32_t value);

/**
 * @method str_append_double
 * 追加一个浮点数。
 * @param {str_t*} str str对象。
 * @param {const char*} format 格式。
 * @param {double} value 要追加的浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_double(str_t* str, const char* format, double value);

/**
 * @method str_append_json_str
 * 追加一个字符串，字符串前后加英文双引号，字符串本身的双引号被转义为\"。
 * @param {str_t*} str str对象。
 * @param {const char*} json_str 待追加的字符串。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_str(str_t* str, const char* json_str);

/**
 * @method str_append_json_int_pair
 * 追加int格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {int32_t} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_int_pair(str_t* str, const char* key, int32_t value);

/**
 * @method str_append_json_str_pair
 * 追加字符串格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {const char*} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_str_pair(str_t* str, const char* key, const char* value);

/**
 * @method str_append_json_double_pair
 * 追加doube格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {double} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_double_pair(str_t* str, const char* key, double value);

/**
 * @method str_append_json_bool_pair
 * 追加bool格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {bool_t} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_bool_pair(str_t* str, const char* key, bool_t value);

/**
 * @method str_pop
 * 删除最后一个字符。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_pop(str_t* str);

/**
 * @method str_unescape
 * 对字符串进行反转义。如：把"\n"转换成'\n'。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_unescape(str_t* str);

/**
 * @method str_decode_xml_entity
 * 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {char*} text 要解码的XML文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_decode_xml_entity(str_t* str, const char* text);

/**
 * @method str_decode_xml_entity_with_len
 * 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {char*} text 要解码的XML文本。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_decode_xml_entity_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_from_int
 * 用整数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {int32_t} v 整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_int(str_t* str, int32_t v);

/**
 * @method str_from_float
 * 用浮点数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {double} v 浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_float(str_t* str, double v);

/**
 * @method str_from_value
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {value_t} v value。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_value(str_t* str, const value_t* v);

/**
 * @method str_from_wstr
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {wchar_t*} wstr Unicode字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_wstr(str_t* str, const wchar_t* wstr);

/**
 * @method str_from_wstr_with_len
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {wchar_t*} wstr Unicode字符串
 * @param {uint32_t} len Unicode字符串的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_wstr_with_len(str_t* str, const wchar_t* wstr, uint32_t len);

/**
 * @method str_to_int
 * 将字符串转成整数。
 * @param {str_t*} str str对象。
 * @param {int32_t*} v 用于返回整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_int(str_t* str, int32_t* v);

/**
 * @method str_to_float
 * 将字符串转成浮点数。
 * @param {str_t*} str str对象。
 * @param {double*} v 用于返回浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_float(str_t* str, double* v);

/**
 * @method str_end_with
 * 判断字符串是否以指定的子串结尾。
 * @param {str_t*} str str对象。
 * @param {char*} text 子字符串。
 *
 * @return {bool_t} 返回是否以指定的子串结尾。
 */
bool_t str_end_with(str_t* str, const char* text);

/**
 * @method str_start_with
 * 判断字符串是否以指定的子串开头。
 * @param {str_t*} str str对象。
 * @param {char*} text 子字符串。
 *
 * @return {bool_t} 返回是否以指定的子串开头。
 */
bool_t str_start_with(str_t* str, const char* text);

/**
 * @method str_trim
 * 去除首尾指定的字符。
 * @param {str_t*} str str对象。
 * @param {char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim(str_t* str, const char* text);

/**
 * @method str_trim_left
 * 去除首部指定的字符。
 * @param {str_t*} str str对象。
 * @param {char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim_left(str_t* str, const char* text);

/**
 * @method str_trim_right
 * 去除尾部指定的字符。
 * @param {str_t*} str str对象。
 * @param {char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim_right(str_t* str, const char* text);

/**
 * @method str_replace
 * 字符串替换。
 * @param {str_t*} str str对象。
 * @param {char*} text 待替换的子串。
 * @param {char*} new_text 将替换成的子串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_replace(str_t* str, const char* text, const char* new_text);

/**
 * @method str_to_lower
 * 将字符串转成小写。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_lower(str_t* str);

/**
 * @method str_to_upper
 * 将字符串转成大写。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_upper(str_t* str);

/**
 * @method str_expand_vars
 * 将字符串中的变量展开为obj中对应的属性值。
 *
 * 变量的格式为${xxx}：
 *
 * * xxx为变量名时，${xxx}被展开为obj的属性xxx的值。
 * * xxx为表达式时，${xxx}被展开为表达式的值，表达式中可以用变量，$为变量的前缀，如${$x+$y}。
 * * xxx为变量名时，而不存在obj的属性时，${xxx}被移出。
 *
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_expand_vars(str_t* str, const char* src, const object_t* obj);

/**
 * @method str_reset
 * 重置字符串为空。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_reset(str_t* str);

END_C_DECLS

#endif /*TK_STR_H*/

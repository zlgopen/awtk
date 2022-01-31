/**
 * File:   object_array.h
 * Author: AWTK Develop Team
 * Brief:  object array
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_ARRAY_H
#define TK_OBJECT_ARRAY_H

#include "tkc/object.h"
#include "tkc/value.h"

BEGIN_C_DECLS

/**
 * @class object_array_t
 * @parent tk_object_t
 * @annotation ["scriptable"]
 *
 * 简单的动态数组，内部存放value对象。
 *
 * 访问时属性名称为：
 *
 * * "size"/"length" 用于获取数组的长度。
 * * index 用于访问属性，-1可以用来追加新元素。
 *
 */
typedef struct _object_array_t {
  tk_object_t object;

  /**
   * @property {uint32_t} size
   * @annotation ["readable", "scriptable"]
   * 属性个数。
   *
   */
  uint32_t size;

  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 属性数组的容量。
   *
   */
  uint32_t capacity;

  /**
   * @property {value_t} props
   * @annotation ["readable"]
   * 属性数组。
   *
   */
  value_t* props;

} object_array_t;

/**
 * @method object_array_create
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_array_create(void);

/**
 * @method object_array_clone
 *
 * 克隆对象。
 *
 * @annotation ["constructor"]
 * @param {tk_object_t*} o 被克隆的对象。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_array_clone(tk_object_t* o);

/**
 * @method object_array_unref
 *
 * for script gc
 * @param {tk_object_t*} obj 对象。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]

 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_unref(tk_object_t* obj);

/**
 * @method object_array_clear_props
 *
 * 清除全部属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_clear_props(tk_object_t* obj);

/**
 * @method object_array_insert
 *
 * 在指定位置插入一个元素。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {uint32_t} index  位置。
 * @param {const value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_insert(tk_object_t* obj, uint32_t index, const value_t* v);

/**
 * @method object_array_push
 *
 * 追加一个元素。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {const value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_push(tk_object_t* obj, const value_t* v);

/**
 * @method object_array_index_of
 *
 * 查找元素出现的第一个位置。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {const value_t*} v 值。
 *
 * @return {int32_t} 如果找到返回其位置，否则返回-1。
 *
 */
int32_t object_array_index_of(tk_object_t* obj, const value_t* v);

/**
 * @method object_array_last_index_of
 *
 * 查找元素出现的最后一个位置。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {const value_t*} v 值。
 *
 * @return {int32_t} 如果找到返回其位置，否则返回-1。
 *
 */
int32_t object_array_last_index_of(tk_object_t* obj, const value_t* v);

/**
 * @method object_array_remove
 *
 * 在指定位置删除一个元素。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {uint32_t} index  位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_remove(tk_object_t* obj, uint32_t index);

/**
 * @method object_array_get_and_remove
 *
 * 在指定位置删除一个元素，并返回它。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {uint32_t} index  位置。
 * @param {value_t*} v 用于返回值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_get_and_remove(tk_object_t* obj, uint32_t index, value_t* v);

/**
 * @method object_array_pop
 *
 * 弹出最后一个元素。
 * @param {tk_object_t*} obj 对象。
 * @param {value_t*} v 返回值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_pop(tk_object_t* obj, value_t* v);

/**
 * @method object_array_shift
 *
 * 弹出第一个元素。
 * @param {tk_object_t*} obj 对象。
 * @param {value_t*} v 返回值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_shift(tk_object_t* obj, value_t* v);

/**
 * @method object_array_get
 * 获取指定序数的元素。
 * @param {tk_object_t*} obj 数组对象。
 * @param {uint32_t} i 序数。
 * @param {value_t*} v 返回的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_get(tk_object_t* obj, uint32_t i, value_t* v);

/**
 * @method object_array_set
 * 设置指定序数的元素。
 * @param {tk_object_t*} obj 数组对象。
 * @param {uint32_t} i 序数。
 * @param {const value_t*} v 元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_set(tk_object_t* obj, uint32_t i, const value_t* v);

/**
 * @method object_array_create_with_str
 * @annotation ["constructor"]
 *
 * 通过字符串构建数组。
 * @param {const char*} str 字符串
 * @param {const char*} sep 分隔符。
 * @param {value_type_t} type 类型。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* object_array_create_with_str(const char* str, const char* sep, value_type_t type);

/**
 * @method object_array_join
 * 使用分隔符把各个元素拼接起来。
 * @param {tk_object_t*} obj 数组对象。
 * @param {const char*} sep 分隔符。
 * @param {str_t*} result 生成的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_join(tk_object_t* obj, const char* sep, str_t* result);

/**
 * @method object_array_min
 * 查找最小值。
 * @param {tk_object_t*} obj 数组对象。
 * @param {value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_min(tk_object_t* obj, value_t* result);

/**
 * @method object_array_max
 * 查找最大值。
 * @param {tk_object_t*} obj 数组对象。
 * @param {value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_max(tk_object_t* obj, value_t* result);

/**
 * @method object_array_sum
 * 求和。
 * @param {tk_object_t*} obj 数组对象。
 * @param {value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_sum(tk_object_t* obj, value_t* result);

/**
 * @method object_array_avg
 * 求平均值。
 * @param {tk_object_t*} obj 数组对象。
 * @param {value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_avg(tk_object_t* obj, value_t* result);

/**
 * @method object_array_dup
 * @annotation ["constructor"]
 *
 * 复制部分或全部元素。
 * @param {tk_object_t*} obj 数组对象。
 * @param {uint32_t} start 开始的位置。 
 * @param {uint32_t} end 结束的位置(不包含)。 
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* object_array_dup(tk_object_t* obj, uint32_t start, uint32_t end);

/**
 * @method object_array_sort
 * 排序。
 * @param {tk_object_t*} obj 数组对象。
 * @param {tk_compare_t} cmp 比较函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_sort(tk_object_t* obj, tk_compare_t cmp);

/**
 * @method object_array_sort_as_int
 * 按整数排序。
 * @param {tk_object_t*} obj 数组对象。
 * @param {bool_t} ascending 升序或降序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_sort_as_int(tk_object_t* obj, bool_t ascending);

/**
 * @method object_array_sort_as_double
 * 按浮点数排序。
 * @param {tk_object_t*} obj 数组对象。
 * @param {bool_t} ascending 升序或降序。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_sort_as_double(tk_object_t* obj, bool_t ascending);

/**
 * @method object_array_sort_as_str
 * 按字符串排序。
 * @param {tk_object_t*} obj 数组对象。
 * @param {bool_t} ascending 升序或降序。
 * @param {bool_t} ignore_case 是否忽略大小写。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_sort_as_str(tk_object_t* obj, bool_t ascending, bool_t ignore_case);

/**
 * @method object_array_reverse
 * 反向。
 * @param {tk_object_t*} obj 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_array_reverse(tk_object_t* obj);

/**
 * @method object_array_cast
 * 转换为object_array对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_array对象。
 *
 * @return {object_array_t*} object_array对象。
 */
object_array_t* object_array_cast(tk_object_t* obj);

#define OBJECT_ARRAY(obj) object_array_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_ARRAY_H*/

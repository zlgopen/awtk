/**
 * File:   typed_array.h
 * Author: AWTK Develop Team
 * Brief:  dynamic typed_array.
 *
 * Copyright (c) 2021 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TYPED_ARRAY_H
#define TK_TYPED_ARRAY_H

#include "tkc/value.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

/**
 * @class typed_array_t
 * 带类型的动态数组，根据元素个数动态调整数组的容量。
 *
 * 示例：
 *
 * ```c
 * value_t v;
 * typed_array_t* typed_array = typed_array_create(VALUE_TYPE_INT, 10);
 * typed_array_push(a, value_set_int(&v, 123));
 * typed_array_push(a, value_set_int(&v, 234));
 * ...
 * typed_array_destroy(typed_array);
 * ```
 *
 */
typedef struct _typed_array_t {
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 数组中元素的个数。
   */
  uint32_t size;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 数组的容量大小。
   */
  uint32_t capacity;
  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * 数组中的元素。
   */
  uint8_t* data;
  /**
   * @property {value_type_t} type
   * @annotation ["readable"]
   * 元素的类型。
   */
  value_type_t type;
  /**
   * @property {uint32_t} element_size
   * @annotation ["readable"]
   * 单个元素的大小。
   */
  uint32_t element_size;
} typed_array_t;

/**
 * @method typed_array_create
 * @annotation ["constructor"]
 * 创建typed_array对象。
 *
 * @param {value_type_t} type 元素的类型。
 * @param {uint32_t} capacity 数组的初始容量(元素个数)。
 *
 * @return {typed_array_t*} 数组对象。
 */
typed_array_t* typed_array_create(value_type_t type, uint32_t capacity);

/**
 * @method typed_array_extend
 * 扩展typed_array到指定容量。
 *
 * @param {value_type_t} type 元素的类型。
 * @param {uint32_t} capacity 数组的容量(元素个数)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_extend(typed_array_t* typed_array, uint32_t capacity);

/**
 * @method typed_array_get
 * 获取指定序数的元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {uint32_t} index 序数。
 * @param {value_t*} v 返回的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_get(typed_array_t* typed_array, uint32_t index, value_t* v);

/**
 * @method typed_array_set
 * 设置指定序数的元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {uint32_t} index 序数。
 * @param {const value_t*} v 元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_set(typed_array_t* typed_array, uint32_t index, const value_t* v);

/**
 * @method typed_array_insert
 * 在指定序数插入元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {uint32_t} index 序数。
 * @param {const value_t*} v 元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_insert(typed_array_t* typed_array, uint32_t index, const value_t* v);

/**
 * @method typed_array_remove
 * 删除指定位置的元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {uint32_t} index 位置序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_remove(typed_array_t* typed_array, uint32_t index);

/**
 * @method typed_array_pop
 * 弹出最后一个元素。
 * @param {typed_array_t*} typed_array 数组对象。
 *
 * @return {void*} 成功返回最后一个元素，失败返回NULL。
 */
ret_t typed_array_pop(typed_array_t* typed_array, value_t* v);

/**
 * @method typed_array_tail
 * 返回最后一个元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {value_t*} v 返回的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_tail(typed_array_t* typed_array, value_t* v);

/**
 * @method typed_array_push
 * 在尾巴追加一个元素。
 * @param {typed_array_t*} typed_array 数组对象。
 * @param {const value_t*} v 待追加的元素。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_push(typed_array_t* typed_array, const value_t* v);

/**
 * @method typed_array_clear
 * 清除全部元素。
 * @param {typed_array_t*} typed_array 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_clear(typed_array_t* typed_array);

/**
 * @method typed_array_destroy
 * 销毁typed_array对象。
 * @param {typed_array_t*} typed_array 数组对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t typed_array_destroy(typed_array_t* typed_array);

END_C_DECLS

#endif /*TK_TYPED_ARRAY_H*/

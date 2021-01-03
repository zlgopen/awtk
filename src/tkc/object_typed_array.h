/**
 * File:   object_typed_array.h
 * Author: AWTK Develop Team
 * Brief:  wrap typed array to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_OBJECT_TYPED_ARRAY_H
#define TK_OBJECT_TYPED_ARRAY_H

#include "tkc/object.h"
#include "tkc/typed_array.h"

BEGIN_C_DECLS

/**
 * @class object_typed_array_t
 * @parent object_t
 *
 * 将typed_array包装成object。
 *
 */
typedef struct _object_typed_array_t {
  object_t object;

  /**
   * @property {typed_array_t*} arr
   * @annotation ["readable"]
   * typed array对象。
   *
   */
  typed_array_t* arr;
} object_typed_array_t;

/**
 * @method object_typed_array_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 * @param {value_type_t} type 元素的类型。
 * @param {uint32_t} capacity 数组的初始容量(元素个数)。
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_typed_array_create(value_type_t type, uint32_t capacity);

object_typed_array_t* object_typed_array_cast(object_t* obj);
#define OBJECT_TYPED_ARRAY(obj) object_typed_array_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_TYPED_ARRAY_H*/

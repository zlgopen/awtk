/**
 * File:   object_array.h
 * Author: AWTK Develop Team
 * Brief:  object array
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
 * @parent object_t
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
  object_t object;

  /**
   * @property {uint32_t} props_size
   * @annotation ["readable", "scriptable"]
   * 属性个数。
   *
   */
  uint32_t props_size;

  /**
   * @property {uint32_t} props_capacity
   * @annotation ["readable"]
   * 属性数组的容量。
   *
   */
  uint32_t props_capacity;

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
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_array_create(void);

/**
 * @method object_array_clone
 *
 * 克隆对象。
 *
 * @annotation ["constructor"]
 * @param {object_array_t*} o 被克隆的对象。
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_array_clone(object_array_t* o);

/**
 * @method object_array_unref
 *
 * for script gc
 * @param {object_t*} obj 对象。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]

 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_unref(object_t* obj);

/**
 * @method object_array_clear_props
 *
 * 清除全部属性。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_array_clear_props(object_t* obj);

object_array_t* object_array_cast(object_t* obj);
#define OBJECT_ARRAY(obj) object_array_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_ARRAY_H*/

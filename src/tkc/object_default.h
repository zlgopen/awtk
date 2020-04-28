/**
 * File:   object_default.h
 * Author: AWTK Develop Team
 * Brief:  default object
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
 * 2019-01-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_DEFAULT_H
#define TK_OBJECT_DEFAULT_H

#include "tkc/object.h"
#include "tkc/named_value.h"

BEGIN_C_DECLS

/**
 * @class object_default_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 对象接口的缺省实现。
 *
 * 内部使用有序数组保存所有属性，可以快速查找指定名称的属性。
 *
 */
typedef struct _object_default_t {
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
   * @property {named_value_t} props
   * @annotation ["readable"]
   * 属性数组。
   *
   */
  named_value_t* props;

} object_default_t;

/**
 * @method object_default_create
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_default_create(void);

/**
 * @method object_default_clone
 *
 * 克隆对象。
 *
 * @annotation ["constructor"]
 * @param {object_default_t*} o 被克隆的对象。
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_default_clone(object_default_t* o);

/**
 * @method object_default_unref
 *
 * for script gc
 * @param {object_t*} obj 对象。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]

 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_default_unref(object_t* obj);

/**
 * @method object_default_clear_props
 *
 * 清除全部属性。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_default_clear_props(object_t* obj);

/**
 * @method object_default_find_prop
 *
 * 查找满足条件的属性，并返回它的值。
 *
 * @param {object_t*} obj 对象。
 * @param {tk_compare_t} cmp 比较函数。
 * @param {const void*} data 要比较的数据。
 *
 * @return {value_t*} 返回属性的值。
 *
 */
value_t* object_default_find_prop(object_t* obj, tk_compare_t cmp, const void* data);

object_default_t* object_default_cast(object_t* obj);
#define OBJECT_DEFAULT(obj) object_default_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_DEFAULT_H*/

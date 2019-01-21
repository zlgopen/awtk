/**
 * File:   object_default.h
 * Author: AWTK Develop Team
 * Brief:  default object
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @annotation ["constructor", "scriptable"]
 *
 * @return {object_t*} 返回object对象。
 *
 */
object_t* object_default_create(void);

#define OBJECT_DEFAULT(obj) ((object_default_t*)(obj))

END_C_DECLS

#endif /*TK_OBJECT_DEFAULT_H*/

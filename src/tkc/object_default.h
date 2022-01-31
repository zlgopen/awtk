/**
 * File:   object_default.h
 * Author: AWTK Develop Team
 * Brief:  default object
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
 * 2019-01-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_DEFAULT_H
#define TK_OBJECT_DEFAULT_H

#include "tkc/object.h"
#include "tkc/darray.h"
#include "tkc/named_value.h"

BEGIN_C_DECLS

/**
 * @class object_default_t
 * @parent tk_object_t
 * @annotation ["scriptable"]
 *
 * 对象接口的缺省实现。
 *
 * 内部使用有序数组保存所有属性，可以快速查找指定名称的属性。
 *
 */
typedef struct _object_default_t {
  tk_object_t object;

  /*private*/
  darray_t props;
  bool_t enable_path;
} object_default_t;

/**
 * @method object_default_create
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_default_create(void);

/**
 * @method object_default_create_ex
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {bool_t} enable_path 是否支持按路径访问属性。
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_default_create_ex(bool_t enable_path);

/**
 * @method object_default_clone
 *
 * 克隆对象。
 *
 * @annotation ["constructor"]
 * @param {object_default_t*} o 被克隆的对象。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_default_clone(object_default_t* o);

/**
 * @method object_default_unref
 *
 * for script gc
 * @param {tk_object_t*} obj 对象。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]

 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_default_unref(tk_object_t* obj);

/**
 * @method object_default_clear_props
 *
 * 清除全部属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t object_default_clear_props(tk_object_t* obj);

/**
 * @method object_default_find_prop
 *
 * 查找满足条件的属性，并返回它的值。
 *
 * @param {tk_object_t*} obj 对象。
 * @param {tk_compare_t} cmp 比较函数。
 * @param {const void*} data 要比较的数据。
 *
 * @return {value_t*} 返回属性的值。
 *
 */
value_t* object_default_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* data);

/**
 * @method object_default_cast
 * 转换为object_default对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_default对象。
 *
 * @return {object_default_t*} object_default对象。
 */
object_default_t* object_default_cast(tk_object_t* obj);
#define OBJECT_DEFAULT(obj) object_default_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_DEFAULT_H*/

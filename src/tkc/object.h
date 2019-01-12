/**
 * File:   object.h
 * Author: AWTK Develop Team
 * Brief:  reference count object
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
 * 2019-01-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_H
#define TK_OBJECT_H

#include "tkc/emitter.h"

BEGIN_C_DECLS

struct _object_vtable_t;
typedef struct _object_vtable_t object_vtable_t;

typedef ret_t (*object_on_destroy_t)(object_t* obj);
typedef int32_t (*object_compare_t)(object_t* obj, object_t* other);

typedef ret_t (*object_remove_prop_t)(object_t* obj, const char* name);
typedef ret_t (*object_get_prop_t)(object_t* obj, const char* name, value_t* v);
typedef ret_t (*object_foreach_prop_t)(object_t* obj, tk_visit_t on_prop, void* ctx);
typedef ret_t (*object_set_prop_t)(object_t* obj, const char* name, const value_t* v);

struct _object_vtable_t {
  const char* type;
  const char* desc;
  uint32_t size : 28;
  uint32_t is_collection : 1;
  object_on_destroy_t on_destroy;

  object_compare_t compare;
  object_get_prop_t get_prop;
  object_set_prop_t set_prop;
  object_remove_prop_t remove_prop;
  object_foreach_prop_t foreach_prop;
};

/**
 * @class object_t
 * @parent emitter_t
 * @annotation ["scriptable"]
 * @alias object_base
 *
 * 对象接口。
 *
 */
struct _object_t {
  emitter_t emitter;

  /**
   * @property {int32_t} ref_count
   * @annotation ["readable", "scriptable"]
   * 引用计数。
   */
  int32_t ref_count;

  /**
   * @property {char*} name
   * @annotation ["readable", "scriptable"]
   * 对象的名称。
   */
  char* name;

  /*private*/
  uint32_t visiting : 1;
  const object_vtable_t* vt;
};

/**
 * @method object_unref
 * 引用计数减1。引用计数为0时，销毁对象。
 *
 * @annotation ["deconstructor", "scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_unref(object_t* obj);

/**
 * @method object_ref
 * 引用计数加1。
 *
 * @annotation ["constructor", "scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {object_t*} 返回object对象。
 */
object_t* object_ref(object_t* obj);

/**
 * @method object_create
 * 创建对象。
 *
 * > 仅供子类调用。
 *
 * @param {const object_vtable_t*} vt 虚函数表。
 *
 * @return {ret_t} 返回object对象。
 */
object_t* object_create(const object_vtable_t* vt);

/**
 * @method object_set_name
 * 设置对象的名称。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 对象的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_name(object_t* obj, const char* name);

/**
 * @method object_compare
 * 比较两个对象。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {object_t*} other 比较的object对象。
 *
 * @return {int32_t} 返回比较结果。
 */
int32_t object_compare(object_t* obj, object_t* other);

/**
 * @method object_get_prop
 * 获取指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_get_prop(object_t* obj, const char* name, value_t* v);

/**
 * @method object_remove_prop
 * 删除指定属性。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_remove_prop(object_t* obj, const char* name);

/**
 * @method object_set_prop
 * 设置指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop(object_t* obj, const char* name, const value_t* v);

/**
 * @method object_foreach_prop
 * 遍历所有属性。
 *
 * @annotation ["scriptable:sync_callback"]
 * @param {object_t*} obj object对象。
 * @param {tk_visit_t} on_prop 回调函数。
 * @param {void*} ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx);

END_C_DECLS

#endif /*TK_OBJECT_H*/

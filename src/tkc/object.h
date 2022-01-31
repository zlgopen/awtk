/**
 * File:   object.h
 * Author: AWTK Develop Team
 * Brief:  reference count object
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
 * 2019-01-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_H
#define TK_OBJECT_H

#include "tkc/str.h"
#include "tkc/emitter.h"

BEGIN_C_DECLS

struct _tk_object_vtable_t;
typedef struct _tk_object_vtable_t object_vtable_t;

typedef ret_t (*tk_object_on_destroy_t)(tk_object_t* obj);
typedef int32_t (*tk_object_compare_t)(tk_object_t* obj, tk_object_t* other);

typedef ret_t (*tk_object_remove_prop_t)(tk_object_t* obj, const char* name);
typedef ret_t (*tk_object_get_prop_t)(tk_object_t* obj, const char* name, value_t* v);
typedef ret_t (*tk_object_foreach_prop_t)(tk_object_t* obj, tk_visit_t on_prop, void* ctx);
typedef ret_t (*tk_object_set_prop_t)(tk_object_t* obj, const char* name, const value_t* v);
typedef bool_t (*tk_object_can_exec_t)(tk_object_t* obj, const char* name, const char* args);
typedef ret_t (*tk_object_exec_t)(tk_object_t* obj, const char* name, const char* args);
typedef tk_object_t* (*tk_object_clone_t)(tk_object_t* obj);

struct _tk_object_vtable_t {
  const char* type;
  const char* desc;
  uint32_t size : 28;
  uint32_t is_collection : 1;
  tk_object_on_destroy_t on_destroy;

  tk_object_compare_t compare;
  tk_object_get_prop_t get_prop;
  tk_object_set_prop_t set_prop;
  tk_object_remove_prop_t remove_prop;
  tk_object_foreach_prop_t foreach_prop;
  tk_object_can_exec_t can_exec;
  tk_object_exec_t exec;
  tk_object_clone_t clone;
};

/**
 * @class tk_object_t
 * @order -9
 * @parent emitter_t
 * @annotation ["scriptable"]
 *
 * 对象接口。
 *
 */
struct _tk_object_t {
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
 * @method tk_object_unref
 * 引用计数减1。引用计数为0时，销毁对象。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_unref(tk_object_t* obj);

/**
 * @method tk_object_ref
 * 引用计数加1。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* tk_object_ref(tk_object_t* obj);

/**
 * @method tk_object_clone
 * clone对象。
 *
 * @annotation ["constructor"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* tk_object_clone(tk_object_t* obj);

/**
 * @method tk_object_create
 * 创建对象。
 *
 * > 仅供子类调用。
 *
 * @param {const object_vtable_t*} vt 虚函数表。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* tk_object_create(const object_vtable_t* vt);

/**
 * @method tk_object_create_ex
 * 创建对象。
 *
 * > 仅供子类调用。
 *
 * @param {const object_vtable_t*} vt 虚函数表。
 * @param {uint32_t} extra_data_size 额外的内存大小。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* tk_object_create_ex(const object_vtable_t* vt, uint32_t extra_data_size);

/**
 * @method tk_object_get_type
 * 获取对象的类型名称。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {const char*} 返回对象的类型名称。
 */
const char* tk_object_get_type(tk_object_t* obj);

/**
 * @method tk_object_get_desc
 * 获取对象的描述信息。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {const char*} 返回对象的描述信息。
 */
const char* tk_object_get_desc(tk_object_t* obj);

/**
 * @method tk_object_get_size
 * 获取对象占用内存的大小。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {uint32_t} 返回对象占用内存的大小。
 */
uint32_t tk_object_get_size(tk_object_t* obj);

/**
 * @method tk_object_is_collection
 * 判断对象是否是集合。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {bool_t} 返回TRUE表示是集合，否则不是。
 */
bool_t tk_object_is_collection(tk_object_t* obj);

/**
 * @method tk_object_set_name
 * 设置对象的名称。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 对象的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_name(tk_object_t* obj, const char* name);

/**
 * @method tk_object_compare
 * 比较两个对象。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {tk_object_t*} other 比较的object对象。
 *
 * @return {int} 返回比较结果。
 */
int tk_object_compare(tk_object_t* obj, tk_object_t* other);

/**
 * @method tk_object_get_prop
 * 获取指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_get_prop(tk_object_t* obj, const char* name, value_t* v);

/**
 * @method tk_object_get_prop_str
 * 获取指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {const char*} 返回指定属性的字符串类型的值。
 */
const char* tk_object_get_prop_str(tk_object_t* obj, const char* name);

/**
 * @method tk_object_get_prop_pointer
 * 获取指定属性的指针类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {void*} 返回指定属性的指针类型的值。
 */
void* tk_object_get_prop_pointer(tk_object_t* obj, const char* name);

/**
 * @method tk_object_get_prop_object
 * 获取指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {tk_object_t*} 返回指定属性的object类型的值。
 */
tk_object_t* tk_object_get_prop_object(tk_object_t* obj, const char* name);

/**
 * @method tk_object_get_prop_int
 * 获取指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回指定属性的整数类型的值。
 */
int32_t tk_object_get_prop_int(tk_object_t* obj, const char* name, int32_t defval);

/**
 * @method tk_object_get_prop_bool
 * 获取指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回指定属性的bool类型的值。
 */
bool_t tk_object_get_prop_bool(tk_object_t* obj, const char* name, bool_t defval);

/**
 * @method tk_object_get_prop_float
 * 获取指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {float_t} defval 缺省值。
 *
 * @return {float_t} 返回指定属性的浮点数类型的值。
 */
float_t tk_object_get_prop_float(tk_object_t* obj, const char* name, float_t defval);

/**
 * @method tk_object_get_prop_double
 * 获取指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {double} defval 缺省值。
 *
 * @return {double} 返回指定属性的浮点数类型的值。
 */
double tk_object_get_prop_double(tk_object_t* obj, const char* name, double defval);

/**
 * @method tk_object_remove_prop
 * 删除指定属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_remove_prop(tk_object_t* obj, const char* name);

/**
 * @method tk_object_set_prop
 * 设置指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {value_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop(tk_object_t* obj, const char* name, const value_t* value);

/**
 * @method tk_object_set_prop_str
 * 设置指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {const char*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_str(tk_object_t* obj, const char* name, const char* value);

/**
 * @method tk_object_set_prop_pointer
 * 设置指定属性的指针类型的值。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {void*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_pointer(tk_object_t* obj, const char* name, void* value);

/**
 * @method tk_object_set_prop_object
 * 设置指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {tk_object_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_object(tk_object_t* obj, const char* name, tk_object_t* value);

/**
 * @method tk_object_set_prop_int
 * 设置指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int(tk_object_t* obj, const char* name, int32_t value);

/**
 * @method tk_object_set_prop_bool
 * 设置指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_bool(tk_object_t* obj, const char* name, bool_t value);

/**
 * @method tk_object_set_prop_float
 * 设置指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {float_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_float(tk_object_t* obj, const char* name, float_t value);

/**
 * @method tk_object_set_prop_double
 * 设置指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {double} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_double(tk_object_t* obj, const char* name, double value);

/**
 * @method tk_object_copy_prop
 * 拷贝指定的属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 目标对象。
 * @param {tk_object_t*} src 源对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_copy_prop(tk_object_t* obj, tk_object_t* src, const char* name);

/**
 * @method tk_object_foreach_prop
 * 遍历所有属性。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {tk_visit_t} on_prop 回调函数(data参数为named_value_t类型)。
 * @param {void*} ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx);

/**
 * @method tk_object_has_prop
 * 检查是否存在指定的属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t tk_object_has_prop(tk_object_t* obj, const char* name);

/**
 * @method tk_object_eval
 * 计算一个表达式，表达式中引用的变量从prop中获取。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} expr 表达式。
 * @param {value_t*} v 返回计算结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_eval(tk_object_t* obj, const char* expr, value_t* v);

/**
 * @method tk_object_can_exec
 * 检查是否可以执行指定的命令。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 命令的名称。
 * @param {const char*} args 命令的参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t tk_object_can_exec(tk_object_t* obj, const char* name, const char* args);

/**
 * @method tk_object_exec
 * 执行指定的命令。
 *
 * @alias execute
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 命令的名称。
 * @param {const char*} args 命令的参数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_exec(tk_object_t* obj, const char* name, const char* args);

/**
 * @method tk_object_notify_changed
 * 触发EVT_PROPS_CHANGED事件。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_notify_changed(tk_object_t* obj);

/**
 * @method tk_object_has_prop_by_path
 * 检查是否存在指定的属性。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path，各级之间用.分隔。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t tk_object_has_prop_by_path(tk_object_t* obj, const char* path);

/**
 * @method tk_object_get_prop_by_path
 * 获取指定path属性的值。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path，各级之间用.分隔。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_get_prop_by_path(tk_object_t* obj, const char* path, value_t* v);

/**
 * @method tk_object_get_prop_str_by_path
 * 获取指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {const char*} 返回指定属性的字符串类型的值。
 */
const char* tk_object_get_prop_str_by_path(tk_object_t* obj, const char* path);

/**
 * @method tk_object_get_prop_pointer_by_path
 * 获取指定属性的指针类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {void*} 返回指定属性的指针类型的值。
 */
void* tk_object_get_prop_pointer_by_path(tk_object_t* obj, const char* path);

/**
 * @method tk_object_get_prop_object_by_path
 * 获取指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {tk_object_t*} 返回指定属性的object类型的值。
 */
tk_object_t* tk_object_get_prop_object_by_path(tk_object_t* obj, const char* path);

/**
 * @method tk_object_get_prop_int_by_path
 * 获取指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回指定属性的整数类型的值。
 */
int32_t tk_object_get_prop_int_by_path(tk_object_t* obj, const char* path, int32_t defval);

/**
 * @method tk_object_get_prop_bool_by_path
 * 获取指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回指定属性的bool类型的值。
 */
bool_t tk_object_get_prop_bool_by_path(tk_object_t* obj, const char* path, bool_t defval);

/**
 * @method tk_object_get_prop_float_by_path
 * 获取指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {float_t} defval 缺省值。
 e
 * @return {float_t} 返回指定属性的浮点数类型的值。
 */
float_t tk_object_get_prop_float_by_path(tk_object_t* obj, const char* path, float_t defval);

/**
 * @method tk_object_set_prop_by_path
 * 设置指定属性的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {value_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_by_path(tk_object_t* obj, const char* path, const value_t* value);

/**
 * @method tk_object_set_prop_str_by_path
 * 设置指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {const char*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_str_by_path(tk_object_t* obj, const char* path, const char* value);

/**
 * @method tk_object_set_prop_pointer_by_path
 * 设置指定属性的指针类型的值。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {void*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_pointer_by_path(tk_object_t* obj, const char* path, void* value);

/**
 * @method tk_object_set_prop_object_by_path
 * 设置指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {tk_object_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_object_by_path(tk_object_t* obj, const char* path, tk_object_t* value);

/**
 * @method tk_object_set_prop_int_by_path
 * 设置指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {int32_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int_by_path(tk_object_t* obj, const char* path, int32_t value);

/**
 * @method tk_object_set_prop_bool_by_path
 * 设置指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {bool_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_bool_by_path(tk_object_t* obj, const char* path, bool_t value);

/**
 * @method tk_object_set_prop_float_by_path
 * 设置指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {float_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_float_by_path(tk_object_t* obj, const char* path, float_t value);

/**
 * @method tk_object_can_exec_by_path
 * 检查是否可以执行指定的命令。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 命令的path。
 * @param {const char*} args 命令的参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t tk_object_can_exec_by_path(tk_object_t* obj, const char* path, const char* args);

/**
 * @method tk_object_exec_by_path
 * 执行指定的命令。
 *
 * @alias execute_by_path
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 命令的path。
 * @param {const char*} args 命令的参数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_exec_by_path(tk_object_t* obj, const char* path, const char* args);

/**
 * @method tk_object_get_prop_int8
 * 获取指定属性的int8类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int8_t} defval 缺省值。
 *
 * @return {int8_t} 返回指定属性的int8类型的值。
 */
int8_t tk_object_get_prop_int8(tk_object_t* obj, const char* name, int8_t defval);
/**
 * @method tk_object_set_prop_int8
 * 设置指定属性的int8类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int8_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int8(tk_object_t* obj, const char* name, int8_t value);

/**
 * @method tk_object_get_prop_uint8
 * 获取指定属性的uint8类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint8_t} defval 缺省值。
 *
 * @return {uint8_t} 返回指定属性的uint8类型的值。
 */
uint8_t tk_object_get_prop_uint8(tk_object_t* obj, const char* name, uint8_t defval);
/**
 * @method tk_object_set_prop_uint8
 * 设置指定属性的uint8类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint8_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_uint8(tk_object_t* obj, const char* name, uint8_t value);

/**
 * @method tk_object_get_prop_int16
 * 获取指定属性的int16类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int16_t} defval 缺省值。
 *
 * @return {int16_t} 返回指定属性的int16类型的值。
 */
int16_t tk_object_get_prop_int16(tk_object_t* obj, const char* name, int16_t defval);
/**
 * @method tk_object_set_prop_int16
 * 设置指定属性的int16类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int16_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int16(tk_object_t* obj, const char* name, int16_t value);

/**
 * @method tk_object_get_prop_uint16
 * 获取指定属性的uint16类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint16_t} defval 缺省值。
 *
 * @return {uint16_t} 返回指定属性的uint16类型的值。
 */
uint16_t tk_object_get_prop_uint16(tk_object_t* obj, const char* name, uint16_t defval);
/**
 * @method tk_object_set_prop_uint16
 * 设置指定属性的uint16类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint16_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_uint16(tk_object_t* obj, const char* name, uint16_t value);

/**
 * @method tk_object_get_prop_int32
 * 获取指定属性的int32类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回指定属性的int32类型的值。
 */
int32_t tk_object_get_prop_int32(tk_object_t* obj, const char* name, int32_t defval);
/**
 * @method tk_object_set_prop_int32
 * 设置指定属性的int32类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int32(tk_object_t* obj, const char* name, int32_t value);

/**
 * @method tk_object_get_prop_uint32
 * 获取指定属性的uint32类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint32_t} defval 缺省值。
 *
 * @return {uint32_t} 返回指定属性的uint32类型的值。
 */
uint32_t tk_object_get_prop_uint32(tk_object_t* obj, const char* name, uint32_t defval);
/**
 * @method tk_object_set_prop_uint32
 * 设置指定属性的uint32类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint32_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_uint32(tk_object_t* obj, const char* name, uint32_t value);

/**
 * @method tk_object_get_prop_int64
 * 获取指定属性的int64类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int64_t} defval 缺省值。
 *
 * @return {int64_t} 返回指定属性的int64类型的值。
 */
int64_t tk_object_get_prop_int64(tk_object_t* obj, const char* name, int64_t defval);
/**
 * @method tk_object_set_prop_int64
 * 设置指定属性的int64类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int64_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_int64(tk_object_t* obj, const char* name, int64_t value);

/**
 * @method tk_object_get_prop_uint64
 * 获取指定属性的uint64类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint64_t} defval 缺省值。
 *
 * @return {uint64_t} 返回指定属性的uint64类型的值。
 */
uint64_t tk_object_get_prop_uint64(tk_object_t* obj, const char* name, uint64_t defval);
/**
 * @method tk_object_set_prop_uint64
 * 设置指定属性的uint64类型的值。
 *
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {uint64_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_object_set_prop_uint64(tk_object_t* obj, const char* name, uint64_t value);

/**
 * @method tk_object_get_child_object
 * 获取下一级子对象。
 * > 属性路径之间使用符号"."分割，例如：name.sub_name。
 *
 * @param {tk_object_t*} obj object对象。
 * @param {const char*} path 属性路径。
 * @param {const char**} next_path 返回下一级属性路径。
 *
 * @return {tk_object_t*} 返回下一级子对象，如果找不到则返回 NULL。
 */
tk_object_t* tk_object_get_child_object(tk_object_t* obj, const char* path, const char** next_path);

#define TK_OBJECT(obj) ((tk_object_t*)(obj))

#define TK_OBJECT_REF(obj) tk_object_ref((tk_object_t*)(obj))

#define TK_OBJECT_UNREF(obj)              \
  if ((obj) != NULL) {                    \
    tk_object_unref((tk_object_t*)(obj)); \
    (obj) = NULL;                         \
  }

/**
 * @enum object_cmd_t
 * @annotation ["scriptable", "string"]
 * @prefix TK_OBJECT_CMD_
 * 对象常见命令定义
 */

/**
 * @const TK_OBJECT_CMD_SAVE
 * 保存命令
 */
#define TK_OBJECT_CMD_SAVE "save"

/**
 * @const TK_OBJECT_CMD_RELOAD
 * 重新加载命令
 */
#define TK_OBJECT_CMD_RELOAD "reload"

/**
 * @const TK_OBJECT_CMD_MOVE_UP
 * 和前一个属性交换位置
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_MOVE_UP "move_up"

/**
 * @const TK_OBJECT_CMD_MOVE_DOWN
 * 和后一个属性交换位置
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_MOVE_DOWN "move_down"

/**
 * @const TK_OBJECT_CMD_REMOVE
 * 删除属性。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_REMOVE "remove"

/**
 * @const TK_OBJECT_CMD_REMOVE_CHECKED
 * 删除勾选的属性。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_REMOVE_CHECKED "remove_checked"

/**
 * @const TK_OBJECT_CMD_CLEAR
 * 清除全部属性。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_CLEAR "clear"

/**
 * @const TK_OBJECT_CMD_ADD
 * 增加子项。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_ADD "add"

/**
 * @const TK_OBJECT_CMD_DETAIL
 * 显示对象详细信息。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_DETAIL "detail"

/**
 * @const TK_OBJECT_CMD_EDIT
 * 编辑子项。
 *>参数为属性的名称或路径。
 */
#define TK_OBJECT_CMD_EDIT "edit"

/**
 * @enum object_prop_t
 * @annotation ["scriptable", "string"]
 * @prefix OBJECT_PROP_
 * 对象常见属性定义
 */

/**
 * @const TK_OBJECT_PROP_SIZE
 * 属性的个数。
 */
#define TK_OBJECT_PROP_SIZE "#size"

/**
 * @const TK_OBJECT_PROP_CHECKED
 * 属性是否勾选。
 */
#define TK_OBJECT_PROP_CHECKED "checked"

#include "tkc/object_compat.h"

END_C_DECLS

#endif /*TK_OBJECT_H*/

/**
 * File:   object.h
 * Author: AWTK Develop Team
 * Brief:  reference count object
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
typedef bool_t (*object_can_exec_t)(object_t* obj, const char* name, const char* args);
typedef ret_t (*object_exec_t)(object_t* obj, const char* name, const char* args);

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
  object_can_exec_t can_exec;
  object_exec_t exec;
};

/**
 * @class object_t
 * @order -9
 * @parent emitter_t
 * @annotation ["scriptable"]
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
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {object_t*} obj object对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_unref(object_t* obj);

/**
 * @method object_ref
 * 引用计数加1。
 *
 * @annotation ["constructor", "scriptable", "gc"]
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
 * @method object_get_type
 * 获取对象的类型名称。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {const char*} 返回对象的类型名称。
 */
const char* object_get_type(object_t* obj);

/**
 * @method object_get_desc
 * 获取对象的描述信息。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {const char*} 返回对象的描述信息。
 */
const char* object_get_desc(object_t* obj);

/**
 * @method object_get_size
 * 获取对象占用内存的大小。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {uint32_t} 返回对象占用内存的大小。
 */
uint32_t object_get_size(object_t* obj);

/**
 * @method object_is_collection
 * 判断对象是否是集合。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {bool_t} 返回TRUE表示是集合，否则不是。
 */
bool_t object_is_collection(object_t* obj);

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
 * @return {int} 返回比较结果。
 */
int object_compare(object_t* obj, object_t* other);

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
 * @method object_get_prop_str
 * 获取指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {const char*} 返回指定属性的字符串类型的值。
 */
const char* object_get_prop_str(object_t* obj, const char* name);

/**
 * @method object_get_prop_pointer
 * 获取指定属性的指针类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {void*} 返回指定属性的指针类型的值。
 */
void* object_get_prop_pointer(object_t* obj, const char* name);

/**
 * @method object_get_prop_object
 * 获取指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {object_t*} 返回指定属性的object类型的值。
 */
object_t* object_get_prop_object(object_t* obj, const char* name);

/**
 * @method object_get_prop_int
 * 获取指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回指定属性的整数类型的值。
 */
int32_t object_get_prop_int(object_t* obj, const char* name, int32_t defval);

/**
 * @method object_get_prop_bool
 * 获取指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回指定属性的bool类型的值。
 */
bool_t object_get_prop_bool(object_t* obj, const char* name, bool_t defval);

/**
 * @method object_get_prop_float
 * 获取指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {float_t} defval 缺省值。
 *
 * @return {float_t} 返回指定属性的浮点数类型的值。
 */
float_t object_get_prop_float(object_t* obj, const char* name, float_t defval);

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
 * @param {value_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop(object_t* obj, const char* name, const value_t* value);

/**
 * @method object_set_prop_str
 * 设置指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {const char*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_str(object_t* obj, const char* name, const char* value);

/**
 * @method object_set_prop_pointer
 * 设置指定属性的指针类型的值。
 *
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {void*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_pointer(object_t* obj, const char* name, void* value);

/**
 * @method object_set_prop_object
 * 设置指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {object_t*} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_object(object_t* obj, const char* name, object_t* value);

/**
 * @method object_set_prop_int
 * 设置指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {int32_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_int(object_t* obj, const char* name, int32_t value);

/**
 * @method object_set_prop_bool
 * 设置指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {bool_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_bool(object_t* obj, const char* name, bool_t value);

/**
 * @method object_set_prop_float
 * 设置指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 * @param {float_t} value 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_set_prop_float(object_t* obj, const char* name, float_t value);

/**
 * @method object_copy_prop
 * 拷贝指定的属性。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj 目标对象。
 * @param {object_t*} src 源对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_copy_prop(object_t* obj, object_t* src, const char* name);

/**
 * @method object_foreach_prop
 * 遍历所有属性。
 *
 * @param {object_t*} obj object对象。
 * @param {tk_visit_t} on_prop 回调函数。
 * @param {void*} ctx 回调函数上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx);

/**
 * @method object_has_prop
 * 检查是否存在指定的属性。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 属性的名称。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t object_has_prop(object_t* obj, const char* name);

/**
 * @method object_eval
 * 计算一个表达式，表达式中引用的变量从prop中获取。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} expr 表达式。
 * @param {value_t*} v 返回计算结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_eval(object_t* obj, const char* expr, value_t* v);

/**
 * @method object_can_exec
 * 检查是否可以执行指定的命令。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 命令的名称。
 * @param {const char*} args 命令的参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t object_can_exec(object_t* obj, const char* name, const char* args);

/**
 * @method object_exec
 * 执行指定的命令。
 *
 * @alias execute
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} name 命令的名称。
 * @param {const char*} args 命令的参数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_exec(object_t* obj, const char* name, const char* args);

/**
 * @method object_notify_changed
 * 触发EVT_PROPS_CHANGED事件。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_notify_changed(object_t* obj);

/**
 * @method object_get_prop_by_path
 * 获取指定path属性的值。
 *
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path，各级之间用.分隔。
 * @param {value_t*} v 返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_get_prop_by_path(object_t* obj, const char* path, value_t* v);

/**
 * @method object_get_prop_str_by_path
 * 获取指定属性的字符串类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {const char*} 返回指定属性的字符串类型的值。
 */
const char* object_get_prop_str_by_path(object_t* obj, const char* path);

/**
 * @method object_get_prop_pointer_by_path
 * 获取指定属性的指针类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {void*} 返回指定属性的指针类型的值。
 */
void* object_get_prop_pointer_by_path(object_t* obj, const char* path);

/**
 * @method object_get_prop_object_by_path
 * 获取指定属性的object类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 *
 * @return {object_t*} 返回指定属性的object类型的值。
 */
object_t* object_get_prop_object_by_path(object_t* obj, const char* path);

/**
 * @method object_get_prop_int_by_path
 * 获取指定属性的整数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {int32_t} defval 缺省值。
 *
 * @return {int32_t} 返回指定属性的整数类型的值。
 */
int32_t object_get_prop_int_by_path(object_t* obj, const char* path, int32_t defval);

/**
 * @method object_get_prop_bool_by_path
 * 获取指定属性的bool类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {bool_t} defval 缺省值。
 *
 * @return {bool_t} 返回指定属性的bool类型的值。
 */
bool_t object_get_prop_bool_by_path(object_t* obj, const char* path, bool_t defval);

/**
 * @method object_get_prop_float_by_path
 * 获取指定属性的浮点数类型的值。
 *
 * @annotation ["scriptable"]
 * @param {object_t*} obj object对象。
 * @param {const char*} path 属性的path。
 * @param {float_t} defval 缺省值。
 *
 * @return {float_t} 返回指定属性的浮点数类型的值。
 */
float_t object_get_prop_float_by_path(object_t* obj, const char* path, float_t defval);

#define OBJECT(obj) ((object_t*)(obj))

#define OBJECT_REF(obj) object_ref((object_t*)(obj))

#define OBJECT_UNREF(obj)           \
  if ((obj) != NULL) {              \
    object_unref((object_t*)(obj)); \
    (obj) = NULL;                   \
  }

/**
 * @enum object_cmd_t
 * @annotation ["scriptable", "string"]
 * @prefix OBJECT_CMD_
 * 对象常见命令定义
 */

/**
 * @const OBJECT_CMD_SAVE
 * 保存命令
 */
#define OBJECT_CMD_SAVE "save"

/**
 * @const OBJECT_CMD_RELOAD
 * 重新加载命令
 */
#define OBJECT_CMD_RELOAD "reload"

/**
 * @const OBJECT_CMD_MOVE_UP
 * 和前一个属性交换位置
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_MOVE_UP "move_up"

/**
 * @const OBJECT_CMD_MOVE_DOWN
 * 和后一个属性交换位置
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_MOVE_DOWN "move_down"

/**
 * @const OBJECT_CMD_REMOVE
 * 删除属性。
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_REMOVE "remove"

/**
 * @const OBJECT_CMD_REMOVE_CHECKED
 * 删除勾选的属性。
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_REMOVE_CHECKED "remove_checked"

/**
 * @const OBJECT_CMD_CLEAR
 * 清除全部属性。
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_CLEAR "clear"

/**
 * @const OBJECT_CMD_ADD
 * 增加子项。
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_ADD "add"

/**
 * @const OBJECT_CMD_EDIT
 * 编辑子项。
 *>参数为属性的名称或路径。
 */
#define OBJECT_CMD_EDIT "edit"

/**
 * @enum object_prop_t
 * @annotation ["scriptable", "string"]
 * @prefix OBJECT_PROP_
 * 对象常见属性定义
 */

/**
 * @const OBJECT_PROP_SIZE
 * 属性的个数。
 */
#define OBJECT_PROP_SIZE "#size"

/**
 * @const OBJECT_PROP_CHECKED
 * 属性是否勾选。
 */
#define OBJECT_PROP_CHECKED "checked"

END_C_DECLS

#endif /*TK_OBJECT_H*/

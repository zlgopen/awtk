/**
 * File:   object_hash.h
 * Author: AWTK Develop Team
 * Brief:  用散列值查询属性的object
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-08-19 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_HASH_H
#define TK_OBJECT_HASH_H

#include "tkc/object.h"
#include "tkc/darray.h"

BEGIN_C_DECLS

/**
 * @class object_hash_t
 * @parent tk_object_t
 * @annotation ["scriptable"]
 *
 * 对象接口的散列值查询属性的object实现。
 *
 * 通用当作 map 数据结构使用，内部用有序数组保存所有属性，因此可以快速查找指定名称的属性。
 *
 * 示例
 *
 *```c
 * // 创建默认对象
 * tk_object_t *obj = object_hash_create();
 *
 * // 设置属性
 * tk_object_set_prop_str(obj, "name", "awplc");
 * tk_object_set_prop_int(obj, "age", 18);
 * tk_object_set_prop_double(obj, "weight", 60.5);
 *
 * // 获取属性
 * ENSURE(tk_str_eq(tk_object_get_prop_str(obj, "name"), "awplc"));
 * ENSURE(tk_object_get_prop_int(obj, "age", 0) == 18);
 * ENSURE(tk_object_get_prop_double(obj, "weight", 0) == 60.5);
 *
 * // 遍历属性
 * tk_object_foreach_prop(obj, visit_obj, NULL);
 *
 * // 释放对象
 * TK_OBJECT_UNREF(obj); 
 *```        
 */
typedef struct _object_hash_t {
  tk_object_t object;

  /*private*/
  darray_t props;
  bool_t enable_path : 1;

  /*设置属性值不改变属性的类型*/
  bool_t keep_prop_type : 1;

  /*保持属性间的顺序*/
  bool_t keep_props_order : 1;
} object_hash_t;

/**
 * @method object_hash_create
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_hash_create(void);

/**
 * @method object_hash_create_ex
 *
 * 创建对象。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {bool_t} enable_path 是否支持按路径访问属性。
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_hash_create_ex(bool_t enable_path);

/**
 * @method object_hash_set_keep_prop_type
 * 设置属性值时不改变属性的类型。
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {bool_t} keep_prop_type 不改变属性的类型。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_hash_set_keep_prop_type(tk_object_t* obj, bool_t keep_prop_type);

/**
 * @method object_hash_set_keep_props_order
 * 设置是否保持属性间的顺序。
 * @annotation ["scriptable"]
 * @param {tk_object_t*} obj 对象。
 * @param {bool_t} keep_props_order 保持属性间的顺序。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_hash_set_keep_props_order(tk_object_t* obj, bool_t keep_props_order);

/**
 * @method object_hash_cast
 * 转换为object_hash对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_hash对象。
 *
 * @return {object_hash_t*} object_hash对象。
 */
object_hash_t* object_hash_cast(tk_object_t* obj);

#define OBJECT_HASH(obj) object_hash_cast(obj)

#define OBJECT_HASH_TYPE "object_hash"

END_C_DECLS

#endif /*TK_OBJECT_HASH_H*/

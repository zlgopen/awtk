/**
 * File:   object_overload.h
 * Author: AWTK Develop Team
 * Brief:  对象方法重载器。
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-12-31 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_OVERLOAD_H
#define TK_OBJECT_OVERLOAD_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class object_overload_t
 * @parent tk_object_t
 *
 * 对象方法重载器。
 *
 * 用于重载对象的方法实现，当overload_vt中有对应的函数实现时，使用overload_vt中的实现，
 * 否则使用原始对象(base_obj)的实现。
 *
 */
typedef struct _object_overload_t {
  tk_object_t object;
  /**
   * @property {tk_object_t*} base_obj
   * @annotation ["readable"]
   * 原始对象。
   */
  tk_object_t* base_obj;
  /**
   * @property {const object_vtable_t*} overload_vt
   * 重载的虚函数表。
   */
  const object_vtable_t* overload_vt;
} object_overload_t;

/**
 * @method object_overload_create
 *
 * 创建对象方法重载器。
 *
 * @annotation ["constructor", "gc"]
 *
 * @param {tk_object_t*} base_obj 原始对象。
 * @param {const object_vtable_t*} overload_vt 重载的虚函数表。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_overload_create(tk_object_t* base_obj, const object_vtable_t* overload_vt);

/**
 * @method object_overload_cast
 *
 * 转换为object_overload对象。
 *
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_overload对象。
 *
 * @return {object_overload_t*} object_overload对象。
 */
object_overload_t* object_overload_cast(tk_object_t* obj);

#define OBJECT_OVERLOAD(obj) object_overload_cast(obj)

#define OBJECT_OVERLOAD_TYPE "object_overload"

END_C_DECLS

#endif /*TK_OBJECT_OVERLOAD_H*/

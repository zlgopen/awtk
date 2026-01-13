/**
 * File:   object_override.h
 * Author: AWTK Develop Team
 * Brief:  对象方法重写器。
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

#ifndef TK_OBJECT_OVERRIDE_H
#define TK_OBJECT_OVERRIDE_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class object_override_t
 * @parent tk_object_t
 *
 * 对象方法重写器。
 *
 * 用于重写对象的方法实现，当override_vt中有对应的函数实现时，使用override_vt中的实现，
 * 否则使用原始对象(base_obj)的实现。
 *
 */
typedef struct _object_override_t {
  tk_object_t object;
  /**
   * @property {tk_object_t*} base_obj
   * @annotation ["readable"]
   * 原始对象。
   */
  tk_object_t* base_obj;
  /**
   * @property {const object_vtable_t*} override_vt
   * 重写的虚函数表。
   */
  const object_vtable_t* override_vt;
} object_override_t;

/**
 * @method object_override_create
 *
 * 创建对象方法重写器。
 *
 * @annotation ["constructor", "gc"]
 *
 * @param {tk_object_t*} base_obj 原始对象。
 * @param {const object_vtable_t*} override_vt 重写的虚函数表。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_override_create(tk_object_t* base_obj, const object_vtable_t* override_vt);

/**
 * @method object_override_cast
 *
 * 转换为object_override对象。
 *
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_override对象。
 *
 * @return {object_override_t*} object_override对象。
 */
object_override_t* object_override_cast(tk_object_t* obj);

#define OBJECT_OVERRIDE(obj) object_override_cast(obj)

#define OBJECT_OVERRIDE_TYPE "object_override"

END_C_DECLS

#endif /*TK_OBJECT_OVERRIDE_H*/

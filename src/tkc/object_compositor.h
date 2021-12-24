/**
 * File:   object_compositor.h
 * Author: AWTK Develop Team
 * Brief:  compositor two objects into one
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_COMPOSITOR_H
#define TK_OBJECT_COMPOSITOR_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class object_compositor_t
 * @parent tk_object_t
 *
 * 将两个对象包装为一个对象，优先访问第一个对象，如果访问失败则访问第二个对象。
 *
 */
typedef struct _object_compositor_t {
  tk_object_t object;

  /*private*/
  tk_object_t* obj1;
  tk_object_t* obj2;
} object_compositor_t;

/**
 * @method object_compositor_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 *
 * @param {tk_object_t*} obj1 对象1。
 * @param {tk_object_t*} obj2 对象2。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_compositor_create(tk_object_t* obj1, tk_object_t* obj2);

/**
 * @method object_compositor_cast
 * 转换为object_compositor对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_compositor对象。
 *
 * @return {object_compositor_t*} object_compositor对象。
 */
object_compositor_t* object_compositor_cast(tk_object_t* obj);
#define OBJECT_COMPOSITOR(obj) object_compositor_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_COMPOSITOR_H*/

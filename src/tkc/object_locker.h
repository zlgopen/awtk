/**
 * File:   object_locker.h
 * Author: AWTK Develop Team
 * Brief:  decorate object with lock
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_LOCKER_H
#define TK_OBJECT_LOCKER_H

#include "tkc/object.h"
#include "tkc/mutex_nest.h"

BEGIN_C_DECLS

/**
 * @class object_locker_t
 * @parent tk_object_t
 *
 * 对现有的object对象进行装饰，添加访问互斥功能。
 *
 */
typedef struct _object_locker_t {
  tk_object_t object;

  /*private*/
  tk_mutex_nest_t* mutex;
  tk_object_t* obj;
} object_locker_t;

/**
 * @method object_locker_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 *
 * @param {tk_object_t*} obj 待装饰的对象。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_locker_create(tk_object_t* obj);

/**
 * @method object_locker_cast
 * 转换为object_locker对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_locker对象。
 *
 * @return {object_locker_t*} object_locker对象。
 */
object_locker_t* object_locker_cast(tk_object_t* obj);
#define OBJECT_LOCKER(obj) object_locker_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_LOCKER_H*/

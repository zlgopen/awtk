/**
 * File:   object_app_conf.h
 * Author: AWTK Develop Team
 * Brief:  object app conf
 *
 * Copyright (c) 2020 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-09-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_APP_CONF_H
#define TK_OBJECT_APP_CONF_H

#include "tkc/object.h"
#include "tkc/mutex_nest.h"

BEGIN_C_DECLS

/**
 * @class object_app_conf_t
 * @parent tk_object_t
 *
 * * 将用户配置和默认配置包装起来。
 *   * 优先读取用户配置。
 *   * 默认配置只读，用户配置可读写。
 *
 */
typedef struct _object_app_conf_t {
  tk_object_t object;

  /*private*/
  tk_object_t* user_obj;
  tk_object_t* default_obj;
} object_app_conf_t;

/**
 * @method object_app_conf_create
 *
 * 创建对象。
 *
 * @annotation ["constructor"]
 *
 * @param {tk_object_t*} user_obj 用户配置。
 * @param {tk_object_t*} default_obj 默认配置。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_app_conf_create(tk_object_t* user_obj, tk_object_t* default_obj);

/**
 * @method object_app_conf_cast
 * 转换为object_app_conf对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_app_conf对象。
 *
 * @return {object_app_conf_t*} object_app_conf对象。
 */
object_app_conf_t* object_app_conf_cast(tk_object_t* obj);

#define OBJECT_APP_CONF(obj) object_app_conf_cast(obj)

#define OBJECT_APP_CONF_TYPE "object_app_conf"

END_C_DECLS

#endif /*TK_OBJECT_APP_CONF_H*/

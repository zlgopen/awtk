/**
 * File:   foo_service.h
 * Author: AWTK Develop Team
 * Brief:  foo service
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
 * 2025-04-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FOO_SERVICE_H
#define TK_FOO_SERVICE_H

#include "tkc/buffer.h"
#include "tkc/darray.h"
#include "tkc/iostream.h"
#include "tkc/event.h"
#include "service/service.h"
#include "foo_comm_types_def.h"

BEGIN_C_DECLS

struct _foo_service_t;
typedef struct _foo_service_t foo_service_t;

/**
 * @class foo_service_args_t
 * foo服务端启动参数。 
*/
typedef struct _foo_service_args_t {
  /**
   * @property {tk_service_auth_t} auth
   * @annotation ["readable"]
   * 登陆认证函数。
   */
  tk_service_auth_t auth;

  /**
   * @property {tk_service_logout_t} logout
   * @annotation ["readable"]
   * 登出函数。
   */
  tk_service_logout_t logout;

} foo_service_args_t;

/**
 * @class foo_service_t
 * foo服务端。 
*/
typedef struct _foo_service_t {
  tk_service_t service;

  /*private*/
  bool_t is_login;
  tk_service_auth_t auth;
  tk_service_logout_t logout;
  bool_t dispatching;

  str_t str;
  int value;
} foo_service_t;

/**
 * @method foo_service_create
 * 创建foo服务端。
 *
 * @param {tk_iostream_t*} io IO流(由service释放)。
 * @param {void*} args 参数。
 *
 * @return {tk_service_t*} 返回service对象。
 */
tk_service_t* foo_service_create(tk_iostream_t* io, void* args);

END_C_DECLS

#endif /*TK_FOO_SERVICE_H*/

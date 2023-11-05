/**
 * File:   remote_ui_service.h
 * Author: AWTK Develop Team
 * Brief:  remote ui service
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_REMOTE_UI_SERVICE_H
#define TK_REMOTE_UI_SERVICE_H

#include "tkc/buffer.h"
#include "tkc/darray.h"
#include "tkc/iostream.h"
#include "ubjson/ubjson_writer.h"
#include "service/service.h"
#include "remote_ui/shared/remote_ui_types_def.h"

BEGIN_C_DECLS

/**
 * @class remote_ui_service_args_t
 * remote ui服务端启动参数。 
*/
typedef struct _remote_ui_service_args_t {
  /**
   * @property {tk_service_auth_t} auth
   * @annotation ["readable"]
   * 登陆认证函数。
   */
  tk_service_auth_t auth;
} remote_ui_service_args_t;

/**
 * @class remote_ui_service_t
 * remote ui服务端。 
*/
typedef struct _remote_ui_service_t {
  tk_service_t service;

  /**
   * @property {tk_iostream_t*} io
   * @annotation ["readable"]
   * IO stream。
   */
  tk_iostream_t* io;

  /*private*/
  wbuffer_t wb;
  ubjson_writer_t writer;
  tk_object_t* event_handlers;
  bool_t is_login;
  tk_service_auth_t auth; 
} remote_ui_service_t;

/**
 * @method remote_ui_service_create
 * 创建remote ui客户端。
 *
 * @param {tk_iostream_t*} io IO流(由service释放)。
 * @param {void*} args 参数。
 *
 * @return {tk_service_t*} 返回service对象。
 */
tk_service_t* remote_ui_service_create(tk_iostream_t* io, void* args);

END_C_DECLS

#endif /*TK_REMOTE_UI_SERVICE_H*/

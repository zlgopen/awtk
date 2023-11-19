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

typedef tk_object_t* (*remote_ui_service_find_target_t)(tk_service_t* service,
                                                          const char* target);
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

  /**
   * @property {remote_ui_service_find_target_t} find_target
   * @annotation ["readable"]
   * 查找target的函数。
   */
  remote_ui_service_find_target_t find_target;
} remote_ui_service_args_t;

/**
 * @class remote_ui_service_t
 * remote ui服务端。 
*/
typedef struct _remote_ui_service_t {
  tk_service_t service;

  /*private*/
  bool_t is_login;
  ubjson_writer_t writer;
  tk_object_t* event_handlers;
  
  tk_service_auth_t auth;
  remote_ui_service_find_target_t find_target;
  bool_t dispatching;
} remote_ui_service_t;

/**
 * @method remote_ui_service_create
 * 创建remote ui服务端。
 *
 * @param {tk_iostream_t*} io IO流(由service释放)。
 * @param {void*} args 参数。
 *
 * @return {tk_service_t*} 返回service对象。
 */
tk_service_t* remote_ui_service_create(tk_iostream_t* io, void* args);

/**
 * @method remote_ui_service_start_with_uart
 * 启动基于串口的Remote UI服务。
 * > 在很多RTOS上，没有select，所以通过timer定时检查串口是否有数据。
 * 
 * @param {tk_iostream_t*} io IO流(由service释放)。
 * @param {void*} args 参数。
 *
 * @return {tk_service_t*} 返回service对象。
 */
tk_service_t* remote_ui_service_start_with_uart(tk_iostream_t* io, void* args);

END_C_DECLS

#endif /*TK_REMOTE_UI_SERVICE_H*/

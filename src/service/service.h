/**
 * File:   service.h
 * Author: AWTK Develop Team
 * Brief:  service interface
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
 * 2023-11-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SERVICE_H
#define TK_SERVICE_H

#include "tkc/iostream.h"
#include "tkc/event_source_manager.h"

BEGIN_C_DECLS

struct _tk_service_t;
typedef struct _tk_service_t tk_service_t;

typedef tk_service_t* (*tk_service_create_t)(tk_iostream_t* io, void* args);
typedef ret_t (*tk_service_dispatch_t)(tk_service_t* service);
typedef ret_t (*tk_service_destroy_t)(tk_service_t* service);

typedef ret_t (*tk_service_auth_t)(tk_service_t* service, const char* username,
                                   const char* password);

/**
 * @class tk_service_t
 * 服务接口。
 */
struct _tk_service_t {
  tk_service_dispatch_t dispatch;
  tk_service_destroy_t destroy;
  tk_iostream_t* io;
};

/**
 * @method tk_service_dispatch
 * 处理服务器请求。
 *
 * @param {tk_service_t*} service 服务对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_dispatch(tk_service_t* service);

/**
 * @method tk_service_destroy
 * 销毁服务对象。
 *
 * @param {tk_service_t*} service 服务对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_destroy(tk_service_t* service);

/**
 * @method tk_service_start
 * 启动服务。
 *
 * @param {event_source_manager_t*} esm 事件源管理器。
 * @param {const char*} url 服务地址。
 * @param {tk_service_create_t} create 创建服务对象的函数。
 * @param {void*} args 参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_start(event_source_manager_t* esm, const char* url, tk_service_create_t create,
                           void* args);

END_C_DECLS

#endif /*TK_SERVICE_H*/

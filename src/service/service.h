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

#include "tkc/buffer.h"
#include "tkc/iostream.h"
#include "service/msg_header.h"
#include "tkc/event_source_manager.h"

BEGIN_C_DECLS

struct _tk_service_t;
typedef struct _tk_service_t tk_service_t;

typedef tk_service_t* (*tk_service_create_t)(tk_iostream_t* io, void* args);
typedef ret_t (*tk_service_dispatch_t)(tk_service_t* service);
typedef ret_t (*tk_service_destroy_t)(tk_service_t* service);

typedef ret_t (*tk_service_auth_t)(tk_service_t* service, const char* username,
                                   const char* password);
typedef ret_t (*tk_service_logout_t)(tk_service_t* service);

/**
 * @class tk_service_t
 * 服务接口。
 */
struct _tk_service_t {
  /**
   * @property {wbuffer_t} wb
   * 用于接收/发送数据打包。
   */
  wbuffer_t wb;
  /**
   * @property {tk_iostream_t*} io
   * IO对象。
   */
  tk_iostream_t* io;

  /*private*/
  uint32_t retry_times;
  tk_service_dispatch_t dispatch;
  tk_service_destroy_t destroy;
};

/**
 * @method tk_service_init
 * 初始化服务对象(仅供子类使用)。
 * @param {tk_service_t*} service 服务对象。
 * @param {tk_iostream_t*} io IO对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_init(tk_service_t* service, tk_iostream_t* io);

/**
 * @method tk_service_dispatch
 * 处理服务器请求。
 * > 返回非RET_OK，停止服务器，并销毁service对象。
 * @param {tk_service_t*} service 服务对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_dispatch(tk_service_t* service);

/**
 * @method tk_service_destroy
 * 销毁服务对象。
 * > 服务负责销毁IO对象。
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
 * @param {void*} args 参数(对于TCP服务，该参数必须持续有效，使用全局或静态变量)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_start(event_source_manager_t* esm, const char* url, tk_service_create_t create,
                       void* args);

/**
 * @method tk_service_read_req
 * 服务端读取请求。
 * @param {tk_service_t*} service service对象。
 * @param {tk_msg_header_t*} header 返回消息头。
 * @param {wbuffer_t*} wb 返回对其的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_read_req(tk_service_t* service, tk_msg_header_t* header, wbuffer_t* wb);

/**
 * @method tk_service_send_resp
 * 服务端发送响应。
 * @param {tk_service_t*} service service对象。
 * @param {uint32_t} type 消息类型。
 * @param {uint32_t} data_type 数据类型。
 * @param {uint32_t} resp_code 响应码。
 * @param {wbuffer_t*} wb 要发送的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_send_resp(tk_service_t* service, uint32_t type, uint32_t data_type, uint32_t resp_code,
                           wbuffer_t* wb);

/**
 * @method tk_service_upload_file
 * 处理上传文件。
 * @param {tk_service_t*} service service对象。
 * @param {const char*} filename 文件名。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_upload_file(tk_service_t* service, const char* filename);

/**
 * @method tk_service_download_file
 * 处理下载文件。
 * @param {tk_service_t*} service service对象。
 * @param {const char*} filename 文件名。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_download_file(tk_service_t* service, const char* filename);

/**
 * @method tk_service_set_retry_times
 * 设置重试次数。
 * @param {tk_service_t*} service service对象。
 * @param {uint32_t} retry_times 重试次数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_service_set_retry_times(tk_service_t* service, uint32_t retry_times);

#define TK_SERVICE(obj) ((obj) != NULL ? &((obj)->service) : NULL)

END_C_DECLS

#endif /*TK_SERVICE_H*/

/**
 * File:   client
 * Author: AWTK Develop Team
 * Brief:  client interface
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_CLIENT_H
#define TK_CLIENT_H

#include "tkc/buffer.h"
#include "tkc/iostream.h"
#include "service/msg_header.h"

BEGIN_C_DECLS

struct _tk_client_t;
typedef struct _tk_client_t tk_client_t;

typedef ret_t (*tk_client_on_notify_t)(tk_client_t* client, tk_msg_header_t* header, wbuffer_t* wb);

/**
 * @class tk_client_t
 * 客户端接口。
 */
struct _tk_client_t {
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
  uint32_t wtimeout_ms;
  uint32_t rtimeout_ms;
  tk_client_on_notify_t on_notify;
};

/**
 * @method tk_client_init
 * 初始化(仅供子类使用)。
 * @param {tk_client_t*} client 服务对象。
 * @param {tk_iostream_t*} io io对象。
 * @param {tk_client_on_notify_t} on_notify 通知回调。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_init(tk_client_t* client, tk_iostream_t* io, tk_client_on_notify_t on_notify);

/**
 * @method tk_client_deinit
 * 释放资源(仅供子类使用)。
 * @param {tk_client_t*} client 服务对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_deinit(tk_client_t* client);

/**
 * @method tk_client_send_req
 * 客户端发送请求。
 * @param {tk_client_t*} client client对象。
 * @param {uint32_t} type 消息类型。
 * @param {uint32_t} data_type 数据类型。
 * @param {wbuffer_t*} wb 要发送的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_send_req(tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb);

/**
 * @method tk_client_read_resp
 * 客户端读取响应。
 * @param {tk_client_t*} client client对象。
 * @param {tk_msg_header_t*} header 返回消息头。
 * @param {wbuffer_t*} wb 返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_read_resp(tk_client_t* client, tk_msg_header_t* header, wbuffer_t* wb);

/**
 * @method tk_client_read_notify
 * 客户端读取通知。
 * @param {tk_client_t*} client client对象。
 * @param {uint32_t} timeout 超时时间(毫秒)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_read_notify(tk_client_t* client, uint32_t timeout);

/**
 * @method tk_client_request
 * 客户端发送请求，并读取响应。
 * @param {tk_client_t*} client client对象。
 * @param {uint32_t} type 消息类型。
 * @param {uint32_t} data_type 数据类型。
 * @param {wbuffer_t*} wb 要发送的数据/返回读取的数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_request(tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb);

/**
 * @method tk_client_download_file
 * 客户端下载文件。
 * @param {tk_client_t*} client client对象。
 * @param {const char*} remote_file 远程文件。
 * @param {const char*} local_file 本地文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_download_file(tk_client_t* client, const char* remote_file, const char* local_file);

/**
 * @method tk_client_upload_file
 * 客户端上传文件。
 * @param {tk_client_t*} client client对象。
 * @param {const char*} remote_file 远程文件。
 * @param {const char*} local_file 本地文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_upload_file(tk_client_t* client, const char* remote_file, const char* local_file);

/**
 * @method tk_client_set_retry_times
 * 设置重试次数。
 * @param {tk_client_t*} client client对象。
 * @param {uint32_t} retry_times 重试次数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_set_retry_times(tk_client_t* client, uint32_t retry_times);

/**
 * @method tk_client_set_timeout_ms
 * 设置超时时间。
 * @param {tk_client_t*} client client对象。
 * @param {uint32_t} wtimeout_ms 读数据超时时间。
 * @param {uint32_t} rtimeout_ms 写数据超时时间。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_client_set_timeout_ms(tk_client_t* client, uint32_t wtimeout_ms, uint32_t rtimeout_ms);

#define TK_CLIENT(obj) ((obj) != NULL ? &((obj)->client) : NULL)

END_C_DECLS

#endif /*TK_CLIENT_H*/

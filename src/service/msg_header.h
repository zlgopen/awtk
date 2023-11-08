/**
 * File:   msg_header.h
 * Author: AWTK Develop Team
 * Brief:  msg header
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

#ifndef TK_MSG_HEADER_H
#define TK_MSG_HEADER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum tk_msg_data_type_t
 * @prefix MSG_DATA_TYPE_
 * 数据类型。
*/
typedef enum _tk_msg_data_type_t {
  /**
   * @const MSG_DATA_TYPE_NONE
   * 无效数据类型。
   */
  MSG_DATA_TYPE_NONE = 0,
  /**
   * @const MSG_DATA_TYPE_UBJSON
   * JSON数据类型。
   */
  MSG_DATA_TYPE_UBJSON,
  /**
   * @const MSG_DATA_TYPE_STRING
   * 字符串数据类型。
   */
  MSG_DATA_TYPE_STRING,
  /**
   * @const MSG_DATA_TYPE_BINARY
   * 二进制数据类型。
   */
  MSG_DATA_TYPE_BINARY
} tk_msg_data_type_t;

/**
 * @enum tk_msg_code_t
 * @prefix MSG_
 * 消息类型。
 * 
 */
typedef enum _tk_msg_code_t {
  /**
   * @const MSG_NONE
   * 无效消息。
  */
  MSG_NONE = 0,
  /**
   * @const MSG_REQ_CONFIRM
   * 数据包确认。
   */
  MSG_REQ_CONFIRM,
  /**
   * @const MSG_REQ_LOGIN
   * 登录请求。
   */
  MSG_REQ_LOGIN,
  /**
   * @const MSG_REQ_LOGOUT
   * 登出请求。
  */
  MSG_REQ_LOGOUT,
  /**
   * @const MSG_REQ_UPLOAD_FILE_BEGIN
   * 上传文件请求开始。
   */
  MSG_REQ_UPLOAD_FILE_BEGIN,
  /**
   * @const MSG_REQ_UPLOAD_FILE_DATA
   * 上传文件请求数据。
   */
  MSG_REQ_UPLOAD_FILE_DATA,
  /**
   * @const MSG_REQ_UPLOAD_FILE_END
   * 上传文件请求结束。
   */
  MSG_REQ_UPLOAD_FILE_END,
  /**
   * @const MSG_REQ_DOWNLOAD_FILE_BEGIN
   * 下载文件请求。
   */
  MSG_REQ_DOWNLOAD_FILE_BEGIN,

  /**
   * @const MSG_RESP_LOGIN
   * 登录响应。
   */
  MSG_RESP_LOGIN,
  /**
   * @const MSG_RESP_LOGOUT
   * 登出响应。
   */
  MSG_RESP_LOGOUT,
  /**
   * @const MSG_RESP_UPLOAD_FILE_BEGIN
   * 上传文件开始响应。
   */
  MSG_RESP_UPLOAD_FILE_BEGIN,
  /**
   * @const MSG_RESP_UPLOAD_FILE_DATA
   * 上传文件数据响应。
   */
  MSG_RESP_UPLOAD_FILE_DATA,
  /**
   * @const MSG_RESP_UPLOAD_FILE_END
   * 上传文件结束响应。
   */
  MSG_RESP_UPLOAD_FILE_END,
  /**
   * @const MSG_RESP_DOWNLOAD_FILE_BEGIN
   * 下载文件开始响应。
   */
  MSG_RESP_DOWNLOAD_FILE_BEGIN,
  /**
   * @const MSG_RESP_DOWNLOAD_FILE_DATA
   * 下载文件数据响应。
   */
  MSG_RESP_DOWNLOAD_FILE_DATA,
  /**
   * @const MSG_RESP_DOWNLOAD_FILE_END
   * 下载文件数据响应。
   */
  MSG_RESP_DOWNLOAD_FILE_END,
  /**
   * @const MSG_USER_START
   * 用户扩展消息起始值。
   */
  MSG_USER_START = 100
} tk_msg_code_t;

/**
 * @class tk_msg_header_t
 * 消息头。
*/
typedef struct _tk_msg_header_t {
  /**
   * @property {uint32_t} size
   * 消息体的大小。
   */
  uint32_t size;
  /**
   * @property {uint16_t} type
   * 消息类型。
   */
  uint16_t type;
  /**
   * @property {uint8_t} data_type
   * 数据类型。
   */
  uint8_t data_type;
  /**
   * @property {uint8_t} resp_code
   * 响应码(仅适用于resp)。
   */
  uint8_t resp_code;
} tk_msg_header_t;

#define TK_MAX_RETRY_TIMES 3

END_C_DECLS

#endif /*TK_MSG_HEADER_H*/

/**
 * File:   foo_comm_types_def.h
 * Author: AWTK Develop Team
 * Brief:  foo client/service common types def
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

#ifndef TK_FOO_COMMON_TYPES_DEF_H
#define TK_FOO_COMMON_TYPES_DEF_H

#include "service/msg_header.h"

BEGIN_C_DECLS

/**
 * @enum foo_msg_code_t
 * @prefix FOO_MSG_CODE_
 * 消息类型。
 * 
 */
typedef enum _foo_msg_code_t {
  /**
   * @const FOO_MSG_CODE_SET_INT
   * 设置INT数据 
   */
  FOO_MSG_CODE_SET_INT = MSG_USER_START,
  /**
   * @const FOO_MSG_CODE_SET_STRING
   * 设置STRING数据 
   */
  FOO_MSG_CODE_SET_STRING,
  /**
   * @const FOO_MSG_CODE_GET_INT
   * 设置INT数据 
   */
  FOO_MSG_CODE_GET_INT,
  /**
   * @const FOO_MSG_CODE_GET_STRING
   * 设置STRING数据 
   */
  FOO_MSG_CODE_GET_STRING,
  /**
   * @const FOO_MSG_CODE_QUIT
   * 退出。
   */
  FOO_MSG_CODE_QUIT,
  /**
   * @const FOO_MSG_CODE_GET_RETRY_TIMES
   * 获取重试次数。
   */
  FOO_MSG_CODE_GET_RETRY_TIMES,
} foo_msg_code_t;

#define FOO_KEY_USERNAME "username"
#define FOO_KEY_PASSWORD "password"

#ifndef FOO_URL
#define FOO_URL "tcp://localhost:2233"
#endif /*FOO_URL*/

#define FOO_VERSION 0x01u
#define FOO_RETRY_TIMES 0

END_C_DECLS

#endif /*TK_FOO_COMMON_TYPES_DEF_H*/


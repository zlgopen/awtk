/**
 * File:   debugger_client_tcp.h
 * Author: AWTK Develop Team
 * Brief:  debugger client_tcp
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_CLIENT_TCP_H
#define TK_DEBUGGER_CLIENT_TCP_H

#include "tkc/iostream.h"
#include "debugger/debugger.h"
#include "debugger/debugger_client.h"
#include "debugger/debugger_const.h"
#include "debugger/debugger_global.h"
#include "debugger/debugger_message.h"

BEGIN_C_DECLS

/**
 * @class debugger_client_tcp_t
 * @annotation ["fake"]
 * 调试器TCP客户端。
 *
 */

/**
 * @method debugger_client_tcp_create
 * 创建调试器TCP客户端对象。
 * @param {const char*} host 目标主机。
 * @param {uint32_t} port 目标端口。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
debugger_t* debugger_client_tcp_create(const char* host, uint32_t port);

END_C_DECLS

#endif /*TK_DEBUGGER_CLIENT_TCP_H*/

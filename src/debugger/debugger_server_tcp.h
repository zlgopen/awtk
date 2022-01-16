/**
 * File:   debugger_server_tcp.h
 * Author: AWTK Develop Team
 * Brief:  debugger server_tcp
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

#ifndef TK_DEBUGGER_SERVER_TCP_H
#define TK_DEBUGGER_SERVER_TCP_H

#include "tkc/iostream.h"
#include "debugger/debugger.h"
#include "debugger/debugger_server.h"

BEGIN_C_DECLS

/**
 * @class debugger_server_tcp_t
 * 调试器TCP服务端。
 *
 */

/**
 * @method debugger_server_tcp_init
 * 初始化调试器服务。
 * @param {uint32_t} port 监听端口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_tcp_init(uint32_t port);

/**
 * @method debugger_server_tcp_start
 * 启动调试器服务。
 * > 接收客户端请求，并启动服务。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_tcp_start(void);

/**
 * @method debugger_server_tcp_deinit
 * 停止调试器服务。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_tcp_deinit(void);

END_C_DECLS

#endif /*TK_DEBUGGER_SERVER_TCP_H*/

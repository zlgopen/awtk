/**
 * File:   debugger_server.h
 * Author: AWTK Develop Team
 * Brief:  debugger server
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

#include "tkc/socket_helper.h"
#include "streams/inet/iostream_tcp.h"
#include "debugger/debugger_client_tcp.h"

debugger_t* debugger_client_tcp_create(const char* host, uint32_t port) {
  int32_t sock = 0;
  tk_iostream_t* io = NULL;
  debugger_t* debugger = NULL;

  return_value_if_fail(host != NULL, NULL);
  sock = tk_tcp_connect(host, port);
  return_value_if_fail(sock >= 0, NULL);

  io = tk_iostream_tcp_create(sock);
  if (io != NULL) {
    debugger = debugger_client_create(io);
    TK_OBJECT_UNREF(io);
  } else {
    socket_close(sock);
  }

  return debugger;
}

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
#include "debugger/debugger_server_tcp.h"

static int s_server_sock = -1;

ret_t debugger_server_tcp_init(uint32_t port) {
  int sock = -1;
  return_value_if_fail(!debugger_server_is_running(), RET_BAD_PARAMS);

  sock = tk_tcp_listen(port);
  return_value_if_fail(sock >= 0, RET_BAD_PARAMS);

  s_server_sock = sock;

  return RET_OK;
}

ret_t debugger_server_tcp_start(void) {
  int sock = -1;
  return_value_if_fail(s_server_sock >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!debugger_server_is_running(), RET_BAD_PARAMS);

  sock = tk_tcp_accept(s_server_sock);
  if (sock >= 0) {
    tk_iostream_t* io = tk_iostream_tcp_create(sock);
    if (io != NULL) {
      debugger_server_start(io);
    } else {
      log_warn("tk_iostream_tcp_create failed\n");
    }
  } else {
    log_warn("tk_tcp_accept failed\n");
  }

  return RET_OK;
}

ret_t debugger_server_tcp_deinit(void) {
  return_value_if_fail(s_server_sock >= 0, RET_BAD_PARAMS);
  socket_close(s_server_sock);
  s_server_sock = -1;
  debugger_server_stop();

  return RET_OK;
}

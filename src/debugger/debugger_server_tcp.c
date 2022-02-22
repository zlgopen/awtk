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

#include "tkc/thread.h"
#include "tkc/socket_helper.h"
#include "streams/inet/iostream_tcp.h"
#include "debugger/debugger_server_tcp.h"

static int s_server_sock = -1;

bool_t debugger_server_tcp_is_inited(void) {
  return s_server_sock >= 0;
}

ret_t debugger_server_tcp_init(uint32_t port) {
  int sock = -1;
  return_value_if_fail(s_server_sock < 0, RET_BAD_PARAMS);
  return_value_if_fail(!debugger_server_is_running(), RET_BAD_PARAMS);

  sock = tk_tcp_listen(port);
  log_debug("debugger server listen at: %u\n", port);
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

static tk_thread_t* s_accept_thread = NULL;

static void* accept_thread(void* arg) {
  while (s_server_sock >= 0) {
    debugger_server_tcp_start();
    debugger_server_wait();
    debugger_server_stop();
    log_debug("client disconnected\n");
  }

  s_accept_thread = NULL;
  return NULL;
}

ret_t debugger_server_tcp_start_async(void) {
  return_value_if_fail(s_accept_thread == NULL, RET_BUSY);

  s_accept_thread = tk_thread_create(accept_thread, NULL);
  return_value_if_fail(s_accept_thread != NULL, RET_OOM);

  tk_thread_start(s_accept_thread);

  return RET_OK;
}

ret_t debugger_server_tcp_deinit(void) {
  return_value_if_fail(s_server_sock >= 0, RET_BAD_PARAMS);
  socket_close(s_server_sock);
  s_server_sock = -1;

  if (s_accept_thread != NULL) {
    tk_thread_destroy(s_accept_thread);
    s_accept_thread = NULL;
  }

  debugger_server_stop();

  return RET_OK;
}

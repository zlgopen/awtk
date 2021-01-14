/**
 * File:   fscript_iostream_inet.c
 * Author: AWTK Develop Team
 * Brief:  iostream_inet functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-06 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "tkc/socket_helper.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/inet/iostream_udp.h"

static ret_t func_iostream_tcp_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int port = 0;
  int sock = 0;
  const char* host = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  host = value_str(args->args);
  port = value_int(args->args + 1);
  return_value_if_fail(host != NULL && port > 0, RET_BAD_PARAMS);
  sock = tcp_connect(host, port);
  return_value_if_fail(sock >= 0, RET_BAD_PARAMS);

  value_set_object(result, OBJECT(tk_iostream_tcp_create(sock)));
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_iostream_udp_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int port = 0;
  const char* host = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  host = value_str(args->args);
  port = value_int(args->args + 1);
  return_value_if_fail(host != NULL && port > 0, RET_BAD_PARAMS);

  value_set_object(result, OBJECT(tk_iostream_udp_create_client(host, port)));
  result->free_handle = TRUE;

  return RET_OK;
}

ret_t fscript_iostream_inet_register(void) {
  ENSURE(fscript_register_func("iostream_tcp_create", func_iostream_tcp_create) == RET_OK);
  ENSURE(fscript_register_func("iostream_udp_create", func_iostream_udp_create) == RET_OK);
  return RET_OK;
}

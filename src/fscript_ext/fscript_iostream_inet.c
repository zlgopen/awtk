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
  sock = tk_tcp_connect(host, port);
  return_value_if_fail(sock >= 0, RET_BAD_PARAMS);

  value_set_object(result, TK_OBJECT(tk_iostream_tcp_create(sock)));
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

  value_set_object(result, TK_OBJECT(tk_iostream_udp_create_client(host, port)));
  result->free_handle = TRUE;

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_iostream_inet)
FACTORY_TABLE_ENTRY("iostream_tcp_create", func_iostream_tcp_create)
FACTORY_TABLE_ENTRY("iostream_udp_create", func_iostream_udp_create)
FACTORY_TABLE_END()

ret_t fscript_iostream_inet_register(void) {
  return fscript_register_funcs(s_ext_iostream_inet);
}

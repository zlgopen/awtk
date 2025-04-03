/**
 * File:   foo_client.c
 * Author: AWTK Develop Team
 * Brief:  foo client
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
 * 2023-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

#include "foo_client.h"

static ret_t foo_client_request_simple(foo_client_t* foo, uint32_t code) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);

  return tk_client_request(&(foo->client), code, MSG_DATA_TYPE_NONE, wb);
}

static ret_t foo_client_on_notify(foo_client_t* foo, tk_msg_header_t* header, wbuffer_t* wb) {
  return_value_if_fail(foo != NULL && header != NULL && wb != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

foo_client_t* foo_client_create(tk_iostream_t* io) {
  foo_client_t* foo = NULL;
  return_value_if_fail(io != NULL, NULL);

  foo = (foo_client_t*)TKMEM_ZALLOC(foo_client_t);
  return_value_if_fail(foo != NULL, NULL);

  tk_client_init(&(foo->client), io, (tk_client_on_notify_t)foo_client_on_notify);

  return foo;
}

ret_t foo_client_login(foo_client_t* foo, const char* username, const char* password) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(username != NULL, RET_BAD_PARAMS);
  return_value_if_fail(password != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, username);
  wbuffer_write_string(wb, password);

  return tk_client_request(&(foo->client), MSG_CODE_LOGIN, MSG_DATA_TYPE_BINARY, wb);
}

ret_t foo_client_logout(foo_client_t* foo) {
  return foo_client_request_simple(foo, MSG_CODE_LOGOUT);
}

ret_t foo_client_set_int(foo_client_t* foo, int value) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_int32(wb, value);

  return tk_client_request(&(foo->client), FOO_MSG_CODE_SET_INT, MSG_DATA_TYPE_BINARY, wb);
}

ret_t foo_client_set_string(foo_client_t* foo, const char* value) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, value);

  return tk_client_request(&(foo->client), FOO_MSG_CODE_SET_STRING, MSG_DATA_TYPE_BINARY, wb);
}

ret_t foo_client_get_int(foo_client_t* foo, int* value) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);

  ret = tk_client_request(&(foo->client), FOO_MSG_CODE_GET_INT, MSG_DATA_TYPE_BINARY, wb);
  return_value_if_fail(ret == RET_OK, RET_BAD_PARAMS);

  memcpy(value, wb->data, sizeof(*value));

  return ret;
}

ret_t foo_client_get_version(foo_client_t* foo, uint32_t* version) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(version != NULL, RET_BAD_PARAMS);
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);

  ret = tk_client_request(&(foo->client), MSG_CODE_GET_VERSION, MSG_DATA_TYPE_BINARY, wb);
  return_value_if_fail(ret == RET_OK, RET_BAD_PARAMS);

  memcpy(version, wb->data, sizeof(*version));

  return ret;
}

ret_t foo_client_get_retry_times(foo_client_t* foo, uint32_t* retry_times) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(retry_times != NULL, RET_BAD_PARAMS);
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);

  ret = tk_client_request(&(foo->client), FOO_MSG_CODE_GET_RETRY_TIMES, MSG_DATA_TYPE_BINARY, wb);
  return_value_if_fail(ret == RET_OK, RET_BAD_PARAMS);

  memcpy(retry_times, wb->data, sizeof(*retry_times));

  return ret;
}

ret_t foo_client_get_string(foo_client_t* foo, str_t* str) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(foo != NULL && foo->client.io != NULL, RET_BAD_PARAMS);

  wb = &(foo->client.wb);
  wbuffer_rewind(wb);

  ret = tk_client_request(&(foo->client), FOO_MSG_CODE_GET_STRING, MSG_DATA_TYPE_BINARY, wb);
  return_value_if_fail(ret == RET_OK, RET_BAD_PARAMS);

  str_set(str, (char*)wb->data);

  return ret;
}

ret_t foo_client_quit(foo_client_t* foo) {
  return foo_client_request_simple(foo, FOO_MSG_CODE_QUIT);
}

ret_t foo_client_destroy(foo_client_t* foo) {
  return_value_if_fail(foo != NULL, RET_BAD_PARAMS);

  tk_client_deinit(&(foo->client));
  TKMEM_FREE(foo);

  return RET_OK;
}

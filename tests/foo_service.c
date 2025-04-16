/**
 * File:   foo_service.c
 * Author: AWTK Develop Team
 * Brief:  foo service
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/crc.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "foo_service.h"

static ret_t foo_service_dispatch(foo_service_t* foo);
static ret_t foo_service_destroy(foo_service_t* foo);

tk_service_t* foo_service_create(tk_iostream_t* io, void* args) {
  foo_service_t* foo = NULL;
  foo_service_args_t* service_args = (foo_service_args_t*)args;

  return_value_if_fail(io != NULL, NULL);

  foo = (foo_service_t*)TKMEM_ZALLOC(foo_service_t);
  return_value_if_fail(foo != NULL, NULL);

  tk_service_init(&(foo->service), io);
  foo->service.dispatch = (tk_service_dispatch_t)foo_service_dispatch;
  foo->service.destroy = (tk_service_destroy_t)foo_service_destroy;

  if (service_args != NULL) {
    if (service_args->auth != NULL) {
      foo->auth = service_args->auth;
    }

    if (service_args->logout != NULL) {
      foo->logout = service_args->logout;
    }
  }

  str_init(&foo->str, 100);

  return (tk_service_t*)foo;
}

static ret_t foo_service_login(foo_service_t* foo, const char* username, const char* password) {
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);
  if (foo->auth != NULL) {
    if (foo->auth((tk_service_t*)foo, username, password) == RET_OK) {
      foo->is_login = TRUE;
      return RET_OK;
    } else {
      return RET_FAIL;
    }
  } else {
    return RET_OK;
  }
}

static ret_t foo_service_logout(foo_service_t* foo) {
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);

  if (foo->logout != NULL) {
    foo->logout(&(foo->service));
  }

  foo->is_login = FALSE;

  return RET_OK;
}

static ret_t foo_service_set_string(foo_service_t* foo, const char* value) {
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  str_set(&foo->str, value);

  return RET_OK;
}

static ret_t foo_service_set_int(foo_service_t* foo, int value) {
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);

  foo->value = value;

  return RET_OK;
}

static ret_t foo_service_dispatch_impl(foo_service_t* foo, tk_msg_header_t* req, wbuffer_t* wb) {
  rbuffer_t rb;
  tk_msg_header_t resp;
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(req != NULL && wb != NULL, RET_BAD_PARAMS);

  memset(&resp, 0x00, sizeof(resp));
  rbuffer_init(&rb, wb->data, wb->cursor);

  resp.type = req->type;
  switch (req->type) {
    case MSG_CODE_LOGIN: {
      const char* username = NULL;
      const char* password = NULL;

      rbuffer_read_string(&rb, &username);
      rbuffer_read_string(&rb, &password);
      resp.resp_code = foo_service_login(foo, username, password);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_CODE_LOGOUT: {
      resp.resp_code = foo_service_logout(foo);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case FOO_MSG_CODE_SET_STRING: {
      const char* str = (const char*)(wb->data);
      resp.resp_code = foo_service_set_string(foo, str);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case FOO_MSG_CODE_SET_INT: {
      int* value = (int*)(wb->data);
      resp.resp_code = foo_service_set_int(foo, *value);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case FOO_MSG_CODE_GET_INT: {
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      wbuffer_write_int32(wb, foo->value);
      break;
    }
    case FOO_MSG_CODE_GET_STRING: {
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      wbuffer_write_string(wb, foo->str.str);
      break;
    }
    case MSG_CODE_GET_VERSION: {
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      wbuffer_write_uint32(wb, FOO_VERSION);
      break;
    }
    case FOO_MSG_CODE_GET_RETRY_TIMES: {
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      wbuffer_write_uint32(wb, foo->service.retry_times);
      break;
    }
    case FOO_MSG_CODE_QUIT: {
      resp.resp_code = RET_NOT_IMPL;
      tk_service_send_resp(&(foo->service), resp.type, resp.data_type, resp.resp_code, wb);
      return RET_QUIT;
    }
    default: {
      resp.resp_code = RET_NOT_IMPL;
      break;
    }
  }

  return tk_service_send_resp(&(foo->service), resp.type, resp.data_type, resp.resp_code, wb);
}

static ret_t foo_service_dispatch(foo_service_t* foo) {
  ret_t ret = RET_OK;
  tk_msg_header_t header;
  return_value_if_fail(foo != NULL && foo->service.io != NULL, RET_BAD_PARAMS);

  memset(&header, 0x00, sizeof(header));
  ret = tk_service_read_req(&(foo->service), &header, &(foo->service.wb));
  return_value_if_fail(ret == RET_OK, ret);

  foo->dispatching = TRUE;
  ret = foo_service_dispatch_impl(foo, &header, &(foo->service.wb));
  foo->dispatching = FALSE;

  return ret;
}

static ret_t foo_service_destroy(foo_service_t* foo) {
  return_value_if_fail(foo != NULL, RET_BAD_PARAMS);

  str_reset(&foo->str);
  memset(foo, 0x00, sizeof(*foo));
  TKMEM_FREE(foo);

  return RET_OK;
}

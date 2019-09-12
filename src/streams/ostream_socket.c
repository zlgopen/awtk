/**
 * File:   ostream_socket.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on socket
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "streams/socket_helper.h"
#include "streams/ostream_socket.h"

static int32_t tk_ostream_socket_write(tk_ostream_t* stream, const uint8_t* buff,
                                       uint32_t max_size) {
  int32_t ret = 0;
  tk_ostream_socket_t* ostream_socket = TK_OSTREAM_SOCKET(stream);

  ret = send(ostream_socket->sock, buff, max_size, 0);
  if (ret <= 0) {
    ostream_socket->is_broken = TRUE;
  }

  return ret;
}

static ret_t tk_ostream_socket_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_socket_t* ostream_socket = TK_OSTREAM_SOCKET(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, ostream_socket->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = ostream_socket->sock >= 0 && ostream_socket->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_ostream_socket_vtable = {.type = "tk_ostream_socket",
                                                           .desc = "tk_ostream_socket",
                                                           .size = sizeof(tk_ostream_socket_t),
                                                           .get_prop = tk_ostream_socket_get_prop};

tk_ostream_t* tk_ostream_socket_create(int sock) {
  object_t* obj = NULL;
  tk_ostream_socket_t* ostream_socket = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_ostream_socket_vtable);
  ostream_socket = TK_OSTREAM_SOCKET(obj);
  return_value_if_fail(ostream_socket != NULL, NULL);

  ostream_socket->sock = sock;
  TK_OSTREAM(obj)->write = tk_ostream_socket_write;

  return TK_OSTREAM(obj);
}

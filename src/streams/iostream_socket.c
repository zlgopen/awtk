/**
 * File:   iostream_socket.c
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
#include "streams/istream_socket.h"
#include "streams/ostream_socket.h"
#include "streams/iostream_socket.h"

static ret_t tk_iostream_socket_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_iostream_socket_t* iostream_socket = TK_IOSTREAM_SOCKET(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, iostream_socket->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok1 =
        object_get_prop_bool(OBJECT(iostream_socket->istream), TK_STREAM_PROP_IS_OK, TRUE);
    bool_t is_ok2 =
        object_get_prop_bool(OBJECT(iostream_socket->ostream), TK_STREAM_PROP_IS_OK, TRUE);

    value_set_bool(v, is_ok1 && is_ok2);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_socket_on_destroy(object_t* obj) {
  tk_iostream_socket_t* iostream_socket = TK_IOSTREAM_SOCKET(obj);

  socket_close(iostream_socket->sock);
  object_unref(OBJECT(iostream_socket->istream));
  object_unref(OBJECT(iostream_socket->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_socket_vtable = {
    .type = "tk_iostream_socket",
    .desc = "tk_iostream_socket",
    .size = sizeof(tk_iostream_socket_t),
    .get_prop = tk_iostream_socket_get_prop,
    .on_destroy = tk_iostream_socket_on_destroy};

static tk_istream_t* tk_iostream_socket_get_istream(tk_iostream_t* stream) {
  tk_iostream_socket_t* iostream_socket = TK_IOSTREAM_SOCKET(stream);

  return iostream_socket->istream;
}

static tk_ostream_t* tk_iostream_socket_get_ostream(tk_iostream_t* stream) {
  tk_iostream_socket_t* iostream_socket = TK_IOSTREAM_SOCKET(stream);

  return iostream_socket->ostream;
}

tk_iostream_t* tk_iostream_socket_create(int sock) {
  object_t* obj = NULL;
  tk_iostream_socket_t* iostream_socket = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_iostream_socket_vtable);
  iostream_socket = TK_IOSTREAM_SOCKET(obj);
  if (iostream_socket == NULL) {
    socket_close(sock);
    return_value_if_fail(iostream_socket != NULL, NULL);
  }

  iostream_socket->sock = sock;
  iostream_socket->istream = tk_istream_socket_create(sock);
  iostream_socket->ostream = tk_ostream_socket_create(sock);
  TK_IOSTREAM(obj)->get_istream = tk_iostream_socket_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_socket_get_ostream;

  return TK_IOSTREAM(obj);
}

/**
 * File:   iostream_tcp.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on socket
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/socket_helper.h"
#include "streams/inet/istream_tcp.h"
#include "streams/inet/ostream_tcp.h"
#include "streams/inet/iostream_tcp.h"

static ret_t tk_iostream_tcp_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_iostream_tcp_t* iostream_tcp = TK_IOSTREAM_TCP(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, iostream_tcp->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok1 = object_get_prop_bool(OBJECT(iostream_tcp->istream), TK_STREAM_PROP_IS_OK, TRUE);
    bool_t is_ok2 = object_get_prop_bool(OBJECT(iostream_tcp->ostream), TK_STREAM_PROP_IS_OK, TRUE);

    value_set_bool(v, is_ok1 && is_ok2);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_tcp_on_destroy(object_t* obj) {
  tk_iostream_tcp_t* iostream_tcp = TK_IOSTREAM_TCP(obj);

  socket_close(iostream_tcp->sock);
  object_unref(OBJECT(iostream_tcp->istream));
  object_unref(OBJECT(iostream_tcp->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_tcp_vtable = {.type = "tk_iostream_tcp",
                                                         .desc = "tk_iostream_tcp",
                                                         .size = sizeof(tk_iostream_tcp_t),
                                                         .get_prop = tk_iostream_tcp_get_prop,
                                                         .on_destroy = tk_iostream_tcp_on_destroy};

static tk_istream_t* tk_iostream_tcp_get_istream(tk_iostream_t* stream) {
  tk_iostream_tcp_t* iostream_tcp = TK_IOSTREAM_TCP(stream);

  return iostream_tcp->istream;
}

static tk_ostream_t* tk_iostream_tcp_get_ostream(tk_iostream_t* stream) {
  tk_iostream_tcp_t* iostream_tcp = TK_IOSTREAM_TCP(stream);

  return iostream_tcp->ostream;
}

tk_iostream_t* tk_iostream_tcp_create(int sock) {
  object_t* obj = NULL;
  tk_iostream_tcp_t* iostream_tcp = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_iostream_tcp_vtable);
  iostream_tcp = TK_IOSTREAM_TCP(obj);
  if (iostream_tcp == NULL) {
    socket_close(sock);
    return_value_if_fail(iostream_tcp != NULL, NULL);
  }

  iostream_tcp->sock = sock;
  iostream_tcp->istream = tk_istream_tcp_create(sock);
  iostream_tcp->ostream = tk_ostream_tcp_create(sock);
  TK_IOSTREAM(obj)->get_istream = tk_iostream_tcp_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_tcp_get_ostream;

  return TK_IOSTREAM(obj);
}

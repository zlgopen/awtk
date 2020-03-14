/**
 * File:   istream_tcp.h
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

static int32_t tk_istream_tcp_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  int32_t ret = 0;
  tk_istream_tcp_t* istream_tcp = TK_ISTREAM_TCP(stream);

  ret = recv(istream_tcp->sock, buff, max_size, 0);
  if (ret <= 0) {
    if (errno != EAGAIN) {
      perror("recv");
      istream_tcp->is_broken = TRUE;
    }
  }

  return ret;
}

static ret_t tk_istream_tcp_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_tcp_t* istream_tcp = TK_ISTREAM_TCP(stream);

  return socket_wait_for_data(istream_tcp->sock, timeout_ms);
}

static ret_t tk_istream_tcp_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_istream_tcp_t* istream_tcp = TK_ISTREAM_TCP(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, istream_tcp->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = istream_tcp->sock >= 0 && istream_tcp->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_istream_tcp_vtable = {.type = "tk_istream_tcp",
                                                        .desc = "tk_istream_tcp",
                                                        .size = sizeof(tk_istream_tcp_t),
                                                        .get_prop = tk_istream_tcp_get_prop};

tk_istream_t* tk_istream_tcp_create(int sock) {
  object_t* obj = NULL;
  tk_istream_tcp_t* istream_tcp = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_istream_tcp_vtable);
  istream_tcp = TK_ISTREAM_TCP(obj);
  return_value_if_fail(istream_tcp != NULL, NULL);

  istream_tcp->sock = sock;
  TK_ISTREAM(obj)->read = tk_istream_tcp_read;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_tcp_wait_for_data;

  return TK_ISTREAM(obj);
}

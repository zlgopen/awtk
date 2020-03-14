/**
 * File:   istream_udp.h
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
#include "streams/inet/istream_udp.h"

static int32_t tk_istream_udp_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  int32_t ret = 0;
  tk_istream_udp_t* istream_udp = TK_ISTREAM_UDP(stream);
  socklen_t addr_size = sizeof(istream_udp->addr);

  ret = recvfrom(istream_udp->sock, buff, max_size, 0, (struct sockaddr*)&(istream_udp->addr),
                 &addr_size);

  if (ret <= 0) {
    if (errno != EAGAIN && errno != 0) {
      perror("recvfrom");
      istream_udp->is_broken = TRUE;
    }
  }

  return ret;
}

static ret_t tk_istream_udp_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_udp_t* istream_udp = TK_ISTREAM_UDP(stream);

  return socket_wait_for_data(istream_udp->sock, timeout_ms);
}

static ret_t tk_istream_udp_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_istream_udp_t* istream_udp = TK_ISTREAM_UDP(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, istream_udp->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = istream_udp->sock >= 0 && istream_udp->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_istream_udp_vtable = {.type = "tk_istream_udp",
                                                        .desc = "tk_istream_udp",
                                                        .size = sizeof(tk_istream_udp_t),
                                                        .get_prop = tk_istream_udp_get_prop};

tk_istream_t* tk_istream_udp_create(int sock) {
  object_t* obj = NULL;
  tk_istream_udp_t* istream_udp = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_istream_udp_vtable);
  istream_udp = TK_ISTREAM_UDP(obj);
  return_value_if_fail(istream_udp != NULL, NULL);

  istream_udp->sock = sock;
  TK_ISTREAM(obj)->read = tk_istream_udp_read;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_udp_wait_for_data;

  return TK_ISTREAM(obj);
}

ret_t tk_istream_udp_set_target_with_addr(tk_istream_t* stream, struct sockaddr_in addr) {
  tk_istream_udp_t* istream_udp = TK_ISTREAM_UDP(stream);
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);

  memcpy(&(istream_udp->addr), &addr, sizeof(addr));

  return RET_OK;
}

ret_t tk_istream_udp_set_target_with_host(tk_istream_t* stream, const char* host, int port) {
  struct sockaddr_in addr_in;
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(socket_resolve(host, port, &addr_in) != NULL, RET_BAD_PARAMS);

  return tk_istream_udp_set_target_with_addr(stream, addr_in);
}

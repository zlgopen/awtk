/**
 * File:   ostream_udp.h
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
 * 2019-09-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/socket_helper.h"
#include "streams/inet/ostream_udp.h"

static int32_t tk_ostream_udp_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  int32_t ret = 0;
  tk_ostream_udp_t* ostream_udp = TK_OSTREAM_UDP(stream);
  struct sockaddr* addr = (struct sockaddr*)&(ostream_udp->addr);

  ret = sendto(ostream_udp->sock, buff, max_size, 0, addr, sizeof(ostream_udp->addr));

  if (ret <= 0) {
    if (errno != EAGAIN && errno != 0) {
      perror("send to");
      ostream_udp->is_broken = TRUE;
    }
  }

  return ret;
}

static ret_t tk_ostream_udp_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_udp_t* ostream_udp = TK_OSTREAM_UDP(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, ostream_udp->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = ostream_udp->sock >= 0 && ostream_udp->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_ostream_udp_vtable = {.type = "tk_ostream_udp",
                                                        .desc = "tk_ostream_udp",
                                                        .size = sizeof(tk_ostream_udp_t),
                                                        .get_prop = tk_ostream_udp_get_prop};

tk_ostream_t* tk_ostream_udp_create(int sock) {
  object_t* obj = NULL;
  tk_ostream_udp_t* ostream_udp = NULL;
  return_value_if_fail(sock >= 0, NULL);

  obj = object_create(&s_tk_ostream_udp_vtable);
  ostream_udp = TK_OSTREAM_UDP(obj);
  return_value_if_fail(ostream_udp != NULL, NULL);

  ostream_udp->sock = sock;
  TK_OSTREAM(obj)->write = tk_ostream_udp_write;

  return TK_OSTREAM(obj);
}

ret_t tk_ostream_udp_set_target_with_addr(tk_ostream_t* stream, struct sockaddr_in addr) {
  tk_ostream_udp_t* ostream_udp = TK_OSTREAM_UDP(stream);
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);

  memcpy(&(ostream_udp->addr), &addr, sizeof(addr));

  return RET_OK;
}

ret_t tk_ostream_udp_set_target_with_host(tk_ostream_t* stream, const char* host, int port) {
  struct sockaddr_in addr_in;
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(socket_resolve(host, port, &addr_in) != NULL, RET_BAD_PARAMS);

  return tk_ostream_udp_set_target_with_addr(stream, addr_in);
}

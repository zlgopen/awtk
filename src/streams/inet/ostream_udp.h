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

#ifndef TK_OSTREAM_UDP_H
#define TK_OSTREAM_UDP_H

#include "tkc/fs.h"
#include "tkc/ostream.h"

BEGIN_C_DECLS

struct _tk_ostream_udp_t;
typedef struct _tk_ostream_udp_t tk_ostream_udp_t;

/**
 * @class tk_ostream_udp_t
 * @parent tk_ostream_t
 *
 * 基于UDP实现的输出流。
 *
 */
struct _tk_ostream_udp_t {
  tk_ostream_t ostream;

  int sock;
  bool_t is_broken;
  struct sockaddr_in addr;
};

/**
 * @method tk_ostream_udp_create
 *
 * 创建ostream对象。
 *
 * @param {int} sock socket.
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_udp_create(int sock);

/**
 * @method tk_ostream_udp_set_target_with_host
 *
 * 设置目标。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {const char*} host 主机或IP。
 * @param {int} port 端口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_udp_set_target_with_host(tk_ostream_t* stream, const char* host, int port);

/**
 * @method tk_ostream_udp_set_target_with_addr
 *
 * 设置目标。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {struct sockaddr_in} addr 地址。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_udp_set_target_with_addr(tk_ostream_t* stream, struct sockaddr_in addr);

#define TK_OSTREAM_UDP(obj) ((tk_ostream_udp_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_UDP_H*/

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

#ifndef TK_ISTREAM_UDP_H
#define TK_ISTREAM_UDP_H

#include "tkc/fs.h"
#include "tkc/istream.h"

BEGIN_C_DECLS

struct _tk_istream_udp_t;
typedef struct _tk_istream_udp_t tk_istream_udp_t;

/**
 * @class tk_istream_udp_t
 * @parent tk_istream_t
 *
 * 基于UDP实现的输入流。
 *
 */
struct _tk_istream_udp_t {
  tk_istream_t istream;

  int sock;
  bool_t is_broken;
  struct sockaddr_in addr;
};

/**
 * @method tk_istream_udp_create
 *
 * 创建istream对象。
 *
 * @param {int} sock socket。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_udp_create(int sock);

/**
 * @method tk_istream_udp_set_target_with_host
 *
 * 设置目标。
 *
 * @param {tk_istream_t*} stream istream对象。
 * @param {const char*} host 主机或IP。
 * @param {int} port 端口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_istream_udp_set_target_with_host(tk_istream_t* stream, const char* host, int port);

/**
 * @method tk_istream_udp_set_target_with_addr
 *
 * 设置目标。
 *
 * @param {tk_istream_t*} stream istream对象。
 * @param {struct sockaddr_in} addr 地址。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_istream_udp_set_target_with_addr(tk_istream_t* stream, struct sockaddr_in addr);

#define TK_ISTREAM_UDP(obj) ((tk_istream_udp_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_UDP_H*/

/**
 * File:   iostream_udp.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on socket
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_UDP_H
#define TK_IOSTREAM_UDP_H

#include "tkc/iostream.h"
#include "tkc/socket_helper.h"

BEGIN_C_DECLS

struct _tk_iostream_udp_t;
typedef struct _tk_iostream_udp_t tk_iostream_udp_t;

/**
 * @class tk_iostream_udp_t
 * @parent tk_iostream_t
 *
 * 基于UDP实现的输入输出流。
 *
 */
struct _tk_iostream_udp_t {
  tk_iostream_t iostream;

  /*private*/
  int sock;
  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_udp_create_client
 *
 * 创建iostream对象(客户端)。
 *
 * @param {const char*} host 服务器地址。
 * @param {int} port 服务器端口。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_udp_create_client(const char* host, int port);

/**
 * @method tk_iostream_udp_create_client_ex
 *
 * 创建iostream对象(客户端)。
 *
 * @param {const char*} host 服务器地址。
 * @param {int} port 服务器端口。
 * @param {const char*} local_ip 本地IP地址(为NULL时自动绑定)。
 * @param {int} local_port 本地端口。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_udp_create_client_ex(const char* host, int port, const char* local_ip,
                                                int local_port);

/**
 * @method tk_iostream_udp_create
 *
 * 创建iostream对象(客户端)。
 *
 * @param {int} sock socket
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_udp_create(int sock);

#define TK_IOSTREAM_UDP(obj) ((tk_iostream_udp_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_UDP_H*/

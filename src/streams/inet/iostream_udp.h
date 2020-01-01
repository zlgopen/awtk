/**
 * File:   iostream_udp.h
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
 * 2019-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_UDP_H
#define TK_IOSTREAM_UDP_H

#include "tkc/fs.h"
#include "tkc/iostream.h"

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
 * @param {int} port 端口。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_udp_create_client(const char* host, int port);

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

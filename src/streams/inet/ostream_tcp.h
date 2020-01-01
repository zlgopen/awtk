/**
 * File:   ostream_tcp.h
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

#ifndef TK_OSTREAM_TCP_H
#define TK_OSTREAM_TCP_H

#include "tkc/fs.h"
#include "tkc/ostream.h"

BEGIN_C_DECLS

struct _tk_ostream_tcp_t;
typedef struct _tk_ostream_tcp_t tk_ostream_tcp_t;

/**
 * @class tk_ostream_tcp_t
 * @parent tk_ostream_t
 *
 * 基于TCP实现的输出流。
 *
 */
struct _tk_ostream_tcp_t {
  tk_ostream_t ostream;

  int sock;
  bool_t is_broken;
};

/**
 * @method tk_ostream_tcp_create
 *
 * 创建ostream对象。
 *
 * @param {int} sock socket.
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_tcp_create(int sock);

#define TK_OSTREAM_TCP(obj) ((tk_ostream_tcp_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_TCP_H*/

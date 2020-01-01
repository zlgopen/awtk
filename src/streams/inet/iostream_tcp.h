/**
 * File:   iostream_tcp.h
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

#ifndef TK_IOSTREAM_TCP_H
#define TK_IOSTREAM_TCP_H

#include "tkc/fs.h"
#include "tkc/iostream.h"

BEGIN_C_DECLS

struct _tk_iostream_tcp_t;
typedef struct _tk_iostream_tcp_t tk_iostream_tcp_t;

/**
 * @class tk_iostream_tcp_t
 * @parent tk_iostream_t
 *
 * 基于TCP实现的输入输出流。
 *
 */
struct _tk_iostream_tcp_t {
  tk_iostream_t iostream;

  int sock;
  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_tcp_create
 *
 * 创建iostream对象。
 *
 * @param {int} sock socket。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_tcp_create(int sock);

#define TK_IOSTREAM_TCP(obj) ((tk_iostream_tcp_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_TCP_H*/

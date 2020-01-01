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

#ifndef TK_ISTREAM_TCP_H
#define TK_ISTREAM_TCP_H

#include "tkc/fs.h"
#include "tkc/istream.h"

BEGIN_C_DECLS

struct _tk_istream_tcp_t;
typedef struct _tk_istream_tcp_t tk_istream_tcp_t;

/**
 * @class tk_istream_tcp_t
 * @parent tk_istream_t
 *
 * 基于TCP实现的输入流。
 *
 */
struct _tk_istream_tcp_t {
  tk_istream_t istream;

  int sock;
  bool_t is_broken;
};

/**
 * @method tk_istream_tcp_create
 *
 * 创建istream对象。
 *
 * @param {int} sock socket。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_tcp_create(int sock);

#define TK_ISTREAM_TCP(obj) ((tk_istream_tcp_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_TCP_H*/

/**
 * File:   socket_pair.h
 * Author: AWTK Develop Team
 * Brief:  socket pair
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
 * 2019-09-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SOCKET_PAIR_H
#define TK_SOCKET_PAIR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class socketpair_t
 * @annotation ["fake"]
 * socketpair
 */

/**
 * @method tk_socketpair
 * 生成两个socket句柄。两者可以模拟客户端和服务器通信。
 *
 * @param {int*} socks 由于返回生成的socket句柄。
 *
 * @return {int} 小于0表示失败。
 */
int tk_socketpair(int socks[2]);

END_C_DECLS

#endif /*TK_SOCKET_PAIR_H*/

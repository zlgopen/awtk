/**
 * File:   socket_helper.h
 * Author: AWTK Develop Team
 * Brief:  socket helper functions
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

#include "tkc/types_def.h"

#ifdef WITH_SOCKET

#ifndef TK_SOCKET_HELPER_H
#define TK_SOCKET_HELPER_H

BEGIN_C_DECLS

/**
 * @class socket 
 * @annotation ["fake"]
 */

/**
 * @method socket_init
 * @annotation ["static"]
 * tcp/udp 平台初始化。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_init();

/**
 * @method socket_deinit
 * @annotation ["static"]
 * tcp/udp 平台~初始化。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_deinit();

/**
 * @method socket_close
 * @annotation ["static"]
 * 关闭socket句柄。
 *
 * @param {int} sock socket句柄。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_close(int sock);

/**
 * @method socket_bind
 * @annotation ["static"]
 * 绑定到指定端口。
 *
 * @param {int} sock socket句柄。
 * @param {int} port 端口号。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_bind(int sock, int port);

/**
 * @method socket_wait_for_data
 * @annotation ["static"]
 * 等待数据。
 *
 * @param {int} sock socket句柄。
 * @param {uint32_t} timeout_ms 等待时间(ms)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_wait_for_data(int sock, uint32_t timeout_ms);

/**
 * @method socket_set_blocking
 * @annotation ["static"]
 * 设置为阻塞或非阻塞模式。
 * 
 * @param {int} sock socket句柄。
 * @param {bool_t} blocking 是否阻塞。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t socket_set_blocking(int sock, bool_t blocking);

/**
 * @method socket_resolve
 * @annotation ["static"]
 * 解析主机名，并初始化addr对象。
 * 
 * @param {int} sock socket句柄。
 * @param {bool_t} blocking 
 * @param {struct sockaddr_in*} addr 地址对象。 
 *
 * @return {struct sockaddr*} 返回地址对象。 
 */
struct sockaddr* socket_resolve(const char* host, int port, struct sockaddr_in* addr);

/**
 * @class udp_t
 * @annotation ["fake"]
 */

/**
 * @method tcp_listen
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tcp_listen(int port);

/**
 * @method tcp_accept
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} sock socket句柄。
 *
 * @return {int} 返回sock句柄。
 */
int tcp_accept(int sock);

/**
 * @method tcp_connect
 * @annotation ["static"]
 * 连接到指定服务器。
 * @param {const char*} host 主机名或IP地址。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tcp_connect(const char* host, int port);

/**
 * @class udp_t
 * @annotation ["fake"]
 */

/**
 * @method udp_listen
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int udp_listen(int port);

/**
 * @method udp_connect
 * @annotation ["static"]
 * 连接到指定服务器。
 * 
 * @param {const char*} host 主机名或IP地址。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int udp_connect(const char* host, int port);

END_C_DECLS

#endif /*TK_SOCKET_HELPER_H*/

#endif /*WITH_SOCKET*/

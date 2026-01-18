/**
 * File:   socket_helper.h
 * Author: AWTK Develop Team
 * Brief:  socket helper functions
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/darray.h"

#ifdef WITH_SOCKET

#ifndef TK_SOCKET_HELPER_H
#define TK_SOCKET_HELPER_H

BEGIN_C_DECLS

/**
 * @class tk_socket_t
 * @annotation ["fake"]
 */

/**
 * @method tk_socket_init
 * @annotation ["static"]
 * tcp/udp 平台初始化。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_init(void);

/**
 * @method tk_socket_deinit
 * @annotation ["static"]
 * tcp/udp 平台~初始化。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_deinit(void);

/**
 * @method tk_socket_close
 * @annotation ["static"]
 * 关闭socket句柄。
 *
 * @param {int} sock socket句柄。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_close(int sock);

/**
 * @method tk_socket_get_ips_by_ifname
 * @annotation ["static"]
 * 根据网卡名字获取该网卡下的 ip 列表。
 * 例子：
 *   darray_t ips;
 *   darray_init(&ips, 10, default_destroy, NULL);
 *   ret_t ret = tk_socket_get_ips_by_ifname(L"以太网 3", &ips);
 *   for (size_t i = 0; i < ips.size; i++) {
 *     printf("%s \r\n", (char*)darray_get(&ips, i));
 *   }
 *   darray_deinit(&ips);
 *
 * @param {const wchar_t*} ifname 网卡名字。
 * @param {darray_t*} ips ip 列表。（数组中的数据都是字符串，需要用户调用释放函数，用户创建 darray_t 对象的时候可以直接配置 default_destroy 释放函数）
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_get_ips_by_ifname(const wchar_t* ifname, darray_t* ips);

/**
 * @method tk_socket_send
 * @annotation ["static"]
 * 
 * 发送数据。
 *
 * @param {int} sock socket句柄。
 * @param {const void*} buffer 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 * @param {int} flags 标志。
 * 
 * @return {int32_t} 返回实际发送的字节数。
 */
int32_t tk_socket_send(int sock, const void* buffer, uint32_t size, int flags);

/**
 * @method tk_socket_sendto
 * @annotation ["static"]
 * 
 * 发送数据到指定地址。
 *
 * @param {int} sock socket句柄。
 * @param {const void*} buffer 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 * @param {int} flags 标志。
 * @param {const struct sockaddr*}  dest_addr 目标地址。
 * @param {uint32_t} dest_len 目标地址长度。
 *
 * @return {int32_t} 返回实际发送的字节数。
 */
int32_t tk_socket_sendto(int sock, const void* buffer, uint32_t size, int flags,
                         const struct sockaddr* dest_addr, uint32_t dest_len);

/**
 * @method tk_socket_recv
 * @annotation ["static"]
 * 
 * 接收数据。
 *
 * @param {int} sock socket句柄。
 * @param {void*} buffer 用于返回数据的缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 * @param {int} flags 标志。
 * 
 * @return {int32_t} 返回实际接收的字节数。
 */
int32_t tk_socket_recv(int sock, void* buffer, uint32_t size, int flags);

/**
 * @method tk_socket_recvfrom
 * @annotation ["static"]
 * 从指定地址接收数据。
 *
 * @param {int} sock socket句柄。
 * @param {void*} buffer 用于返回数据的缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 * @param {int} flags 标志。
 * @param {struct sockaddr*}  dest_addr 目标地址。
 * @param {uint32_t*} dest_len 目标地址长度。
 * 
 * @return {int32_t} 返回实际接收的字节数。
 */
int32_t tk_socket_recvfrom(int sock, void* buffer, uint32_t size, int flags,
                           struct sockaddr* dest_addr, uint32_t* dest_len);

/**
 * @method tk_socket_bind_ex
 * @annotation ["static"]
 * 绑定到指定IP和端口。
 *
 * @param {int} sock socket句柄。
 * @param {const char*} ip IP地址(可以为NULL)。
 * @param {int} port 端口号。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_bind_ex(int sock, const char* ip, int port);

/**
 * @method tk_socket_bind
 * @annotation ["static"]
 * 绑定到指定端口。
 *
 * @param {int} sock socket句柄。
 * @param {int} port 端口号。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_bind(int sock, int port);

/**
 * @method tk_socket_last_io_has_error
 * @annotation ["static"]
 * 判断最后一次IO操作是否正常。
 *
 * @return {bool_t} 返回TRUE表示有错误，否则表示正常。
 */
bool_t tk_socket_last_io_has_error(void);

/**
 * @method tk_socket_wait_for_data
 * @annotation ["static"]
 * 等待数据。
 *
 * @param {int} sock socket句柄。
 * @param {uint32_t} timeout_ms 等待时间(毫秒)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_wait_for_data(int sock, uint32_t timeout_ms);

/**
 * @method tk_socket_set_blocking
 * @annotation ["static"]
 * 设置为阻塞或非阻塞模式。
 * 
 * @param {int} sock socket句柄。
 * @param {bool_t} blocking 是否阻塞。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_socket_set_blocking(int sock, bool_t blocking);

/**
 * @method tk_socket_resolve
 * @annotation ["static"]
 * 解析主机名，并初始化addr对象。
 * 
 * @param {const char*} host 主机名。
 * @param {int} port 端口号。
 * @param {struct sockaddr_in*} addr 地址对象。 
 *
 * @return {struct sockaddr*} 返回地址对象。 
 */
struct sockaddr* tk_socket_resolve(const char* host, int port, struct sockaddr_in* addr);

/**
 * @method tk_socket_get_client_ip
 * @annotation ["static"]
 * 获取客户端IP。
 * 
 * @param {int} sockfd socket句柄。
 *
 * @return {uint32_t} 返回客户端IP。 
 */
uint32_t tk_socket_get_client_ip(int sockfd);

/**
 * @method tk_socket_get_self_ip
 * @annotation ["static"]
 * 获取本地IP。
 * 
 * @param {int} sockfd socket句柄。
 *
 * @return {uint32_t} 返回本地IP。 
 */
uint32_t tk_socket_get_self_ip(int sockfd);

/**
 * @method tk_socket_get_client_ip_str
 * @annotation ["static"]
 * 获取客户端IP。
 * 
 * @param {int} sockfd socket句柄。
 * @param {char*} ip 存放IP地址的内存。
 * @param {int} len 存放IP地址的内存的长度。 
 *
 * @return {const char*} 返回IP地址。 
 */
const char* tk_socket_get_client_ip_str(int sockfd, char* ip, int len);

/**
 * @method tk_socket_get_self_ip_str
 * @annotation ["static"]
 * 获取本地IP。
 * 
 * @param {int} sockfd socket句柄。
 * @param {char*} ip 存放IP地址的内存。
 * @param {int} len 存放IP地址的内存的长度。 
 *
 * @return {const char*} 返回IP地址。 
 */
const char* tk_socket_get_self_ip_str(int sockfd, char* ip, int len);

/**
 * @class tcp_t
 * @annotation ["fake"]
 */

/**
 * @method tk_tcp_listen_ex
 * @annotation ["static"]
 * 监听指定ip 和端口，成功返回sock句柄。
 * @param {const char*} ip ip。（这里的 ip 是指绑定在网卡中的 ip，可以通过 tk_socket_get_ips_by_ifname 获取，如果设置为 NULL，则绑定到 0.0.0.0 中）
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tk_tcp_listen_ex(const char* ip, int port);

/**
 * @method tk_tcp_listen
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tk_tcp_listen(int port);

/**
 * @method tk_socket_get_port
 * @annotation ["static"]
 * 获取当前 socket 的监听指定端口。
 * @param {int} sock socket句柄。
 *
 * @return {int} 成功返回端口，失败返回 -1。
 */
int tk_socket_get_port(int sock);

/**
 * @method tk_tcp_accept
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} sock socket句柄。
 *
 * @return {int} 返回sock句柄。
 */
int tk_tcp_accept(int sock);

/**
 * @method tk_tcp_connect
 * @annotation ["static"]
 * 连接到指定服务器。
 * @param {const char*} host 主机名或IP地址。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tk_tcp_connect(const char* host, int port);

/**
 * @method tk_tcp_is_port_in_use
 * @annotation ["static"]
 * 判断指定端口是否被占用。
 * @param {int} port 端口号。
 *
 * @return {bool_t} 返回TRUE表示被占用，否则表示未被占用。
 */
bool_t tk_tcp_is_port_in_use(int port);

/**
 * @method tk_tcp_connect_ex
 * @annotation ["static"]
 * 连接到指定服务器。
 * @param {const char*} host 主机名或IP地址。
 * @param {int} port 端口号。
 * @param {int} timeout 连接超时（毫秒）设置，设置为0代表按系统默认超时。
 * @param {void*} opts 保留参数设置，当前只接受NULL值。
 *
 * @return {int} 返回sock句柄。
 */
int tk_tcp_connect_ex(const char* host, int port, int timeout, void* opts);

/**
 * @class udp_t
 * @annotation ["fake"]
 */

/**
 * @method tk_udp_listen
 * @annotation ["static"]
 * 监听指定端口，成功返回sock句柄。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tk_udp_listen(int port);

/**
 * @method tk_udp_connect
 * @annotation ["static"]
 * 连接到指定服务器。
 * 
 * @param {const char*} host 主机名或IP地址。
 * @param {int} port 端口号。
 *
 * @return {int} 返回sock句柄。
 */
int tk_udp_connect(const char* host, int port);

/**
 * @method tk_udp_socket
 * @annotation ["static"]
 * 创建UDP socket。
 * 
 * @return {int} 返回sock句柄。
 */
int tk_udp_socket(void);

END_C_DECLS

#include "tkc/socket_helper_compat.h"

#endif /*TK_SOCKET_HELPER_H*/
#else
/*提供两个空函数*/
ret_t tk_socket_init(void);
ret_t tk_socket_deinit(void);
#endif /*WITH_SOCKET*/

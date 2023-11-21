/**
 * File:   socket_helper.h
 * Author: AWTK Develop Team
 * Brief:  socket helper functions
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
 * 2019-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/socket_helper.h"

#ifdef WITH_SOCKET

#if defined(LINUX) || defined(MACOS)
#include <signal.h>
static ret_t tk_ignore_sig_pipe(void) {
  signal(SIGPIPE, SIG_IGN);
  return RET_OK;
}
#else
#define tk_ignore_sig_pipe()
#endif/*LINUX*/

#ifdef WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
ret_t tk_socket_init() {
  int iResult;
  WSADATA wsaData;
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    log_debug("WSAStartup failed: %d\n", iResult);
    return RET_FAIL;
  }
  tk_ignore_sig_pipe();

  return RET_OK;
}

ret_t tk_socket_deinit() {
  WSACleanup();
  return RET_OK;
}

ret_t tk_socket_close(int sock) {
  closesocket(sock);

  return RET_OK;
}
#else

ret_t tk_socket_init() {
  tk_ignore_sig_pipe();
  return RET_OK;
}
ret_t tk_socket_deinit() {
  return RET_OK;
}

ret_t tk_socket_close(int sock) {
#ifdef AWORKS
  closesocket(sock);
#else
  close(sock);
#endif
  return RET_OK;
}
#endif /*WIN32*/

ret_t tk_socket_bind_ex(int sock, const char* ip, int port) {
  struct sockaddr_in s;
  return_value_if_fail(sock >= 0 && port > 0, RET_BAD_PARAMS);

  memset(&s, 0, sizeof(s));

  if (ip != NULL) {
    tk_socket_resolve(ip, port, &s);
  } else {
    s.sin_family = AF_INET;
    s.sin_port = htons(port);
    s.sin_addr.s_addr = htonl(INADDR_ANY);
  }

  if (bind(sock, (struct sockaddr*)&s, sizeof(s)) < 0) {
    log_debug("bind error\n");
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t tk_socket_bind(int sock, int port) {
  return tk_socket_bind_ex(sock, NULL, port);
}

int tk_tcp_listen(int port) {
  int sock;
  int on = 1;

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket error\n");
    return -1;
  }

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
    log_debug("setsockopt error\n");
    return -1;
  }

  return_value_if_fail(tk_socket_bind(sock, port) == RET_OK, -1);

  if (listen(sock, 5) < 0) {
    log_debug("listen error\n");
    return -1;
  }

  return (sock);
}

int tk_udp_listen(int port) {
  int sock;
  int on = 1;
  struct sockaddr_in s;

  if ((sock = (int)socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket error\n");
    return -1;
  }

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
    log_debug("setsockopt error\n");
    return -1;
  }

  memset(&s, 0, sizeof(s));
  s.sin_family = AF_INET;
  s.sin_port = htons(port);
  s.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr*)&s, sizeof(s)) < 0) {
    log_debug("bind error\n");
    return -1;
  }

  return (sock);
}

#define h_addr h_addr_list[0]

struct sockaddr* tk_socket_resolve(const char* host, int port, struct sockaddr_in* addr) {
  struct hostent* h = NULL;
  return_value_if_fail(host != NULL && addr != NULL, NULL);

  h = gethostbyname(host);
  return_value_if_fail(h != NULL, NULL);

  memset(addr, 0x00, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  return_value_if_fail(h->h_addrtype == AF_INET, NULL);

  memcpy(&(addr->sin_addr.s_addr), h->h_addr, h->h_length);

  return (struct sockaddr*)addr;
}

int tk_tcp_connect(const char* host, int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  struct sockaddr* addr = tk_socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    log_debug("socket error\n");
    return -1;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    log_debug("connect error\n");
    tk_socket_close(sock);
    return -1;
  }

  return sock;
}

int tk_udp_connect(const char* host, int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  struct sockaddr* addr = tk_socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    log_debug("socket error\n");
    return -1;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    log_debug("connect error\n");
    tk_socket_close(sock);
    return -1;
  }

  return sock;
}

int tk_tcp_accept(int sock) {
  int so;
  struct sockaddr_in s;
  socklen_t namelen;

  namelen = sizeof(s);
  memset((char*)&s, 0, namelen);

  if ((so = (int)accept(sock, (struct sockaddr*)&s, &namelen)) < 0) {
    return -1;
  }

  return so;
}

ret_t tk_socket_set_blocking(int sock, bool_t blocking) {
  return_value_if_fail(sock >= 0, RET_BAD_PARAMS);

#ifdef _WIN32
  unsigned long mode = blocking ? 0 : 1;
  return (ioctlsocket(sock, FIONBIO, &mode) == 0) ? RET_OK : RET_FAIL;
#else
  int flags = fcntl(sock, F_GETFL, 0);
  if (flags == -1) return RET_FAIL;
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  return (fcntl(sock, F_SETFL, flags) == 0) ? RET_OK : RET_FAIL;
#endif
}

ret_t tk_socket_wait_for_data(int sock, uint32_t timeout_ms) {
  fd_set fdsr;
  int ret = 0;
  struct timeval tv = {0, 0};

  FD_ZERO(&fdsr);
  FD_SET(sock, &fdsr);

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  FD_SET(sock, &fdsr);
  ret = select(sock + 1, &fdsr, NULL, NULL, &tv);

  return ret > 0 && FD_ISSET(sock, &fdsr) ? RET_OK : RET_TIMEOUT;
}

bool_t tk_socket_last_io_has_error(void) {
#ifndef WIN32
  int eno = errno;
  return eno != EAGAIN && eno != 0;
#else
  int eno = GetLastError();
  return eno != 1237 && eno != 0;
#endif /*WIN32*/
}

uint32_t tk_socket_get_client_ip(int sockfd) {
  struct sockaddr_in addr;
  socklen_t addrLen = sizeof(addr);

  if (getpeername(sockfd, (struct sockaddr*)&addr, &addrLen) == 0) {
    return addr.sin_addr.s_addr;
  }

  return 0;
}

uint32_t tk_socket_get_self_ip(int sockfd) {
  struct sockaddr_in addr;
  socklen_t addrLen = sizeof(addr);

  if (getsockname(sockfd, (struct sockaddr*)&addr, &addrLen) == 0) {
    return addr.sin_addr.s_addr;
  }

  return 0;
}

const char* tk_socket_get_client_ip_str(int sockfd, char* ip, int len) {
  struct sockaddr_in addr;
  socklen_t addrLen = sizeof(addr);
  return_value_if_fail(ip != NULL && len > 0, NULL);

  if (getpeername(sockfd, (struct sockaddr*)&addr, &addrLen) == 0) {
    return inet_ntop(AF_INET, &(addr.sin_addr), ip, len);
  }

  return NULL;
}

const char* tk_socket_get_self_ip_str(int sockfd, char* ip, int len) {
  struct sockaddr_in addr;
  socklen_t addrLen = sizeof(addr);
  return_value_if_fail(ip != NULL && len > 0, NULL);

  if (getsockname(sockfd, (struct sockaddr*)&addr, &addrLen) == 0) {
    return inet_ntop(AF_INET, &(addr.sin_addr), ip, len);
  }

  return NULL;
}

int tk_udp_socket(void) {
  return (int)socket(AF_INET, SOCK_DGRAM, 0);
}

int32_t tk_socket_send(int sock, const void* buffer, uint32_t size, int flags) {
  return send(sock, buffer, size, flags);
}

int32_t tk_socket_sendto(int sock, const void* buffer, uint32_t size, int flags,
                         const struct sockaddr* dest_addr, uint32_t dest_len) {
  return_value_if_fail(buffer != NULL, 0);
  return_value_if_fail(dest_addr != NULL, 0);
  return sendto(sock, buffer, size, flags, dest_addr, (socklen_t)dest_len);
}

int32_t tk_socket_recv(int sock, void* buffer, uint32_t size, int flags) {
  return recv(sock, buffer, size, flags);
}

int32_t tk_socket_recvfrom(int sock, void* buffer, uint32_t size, int flags,
                           struct sockaddr* dest_addr, uint32_t* dest_len) {
  return_value_if_fail(buffer != NULL, 0);
  return_value_if_fail(dest_addr != NULL && dest_len != NULL, 0);

  socklen_t len = *dest_len;
  int32_t ret = recvfrom(sock, buffer, size, flags, dest_addr, &len);
  *dest_len = len;

  return ret;
}

#else
ret_t tk_socket_init(void) {
  return RET_OK;
}

ret_t tk_socket_deinit(void) {
  return RET_OK;
}
#endif /*WITH_SOCKET*/

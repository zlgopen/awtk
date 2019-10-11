/**
 * File:   socket_helper.h
 * Author: AWTK Develop Team
 * Brief:  socket helper functions
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "streams/inet/socket_helper.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32")
void socket_init() {
  int iResult;
  WSADATA wsaData;
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    log_debug("WSAStartup failed: %d\n", iResult);
  }
}

void socket_deinit() {
  WSACleanup();
}

void msleep(int ms) {
  Sleep(ms);
}

void socket_close(int sock) {
  closesocket(sock);
}
#else

void socket_init() {
}
void socket_deinit() {
}
void socket_close(int sock) {
  close(sock);
}
#endif /**/

ret_t socket_bind(int sock, int port) {
  struct sockaddr_in s;
  return_value_if_fail(sock >= 0 && port > 0, RET_BAD_PARAMS);

  memset(&s, 0, sizeof(s));
  s.sin_family = AF_INET;
  s.sin_port = htons(port);
  s.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sock, (struct sockaddr*)&s, sizeof(s)) < 0) {
    log_debug("bind error\n");
    return RET_FAIL;
  }

  return RET_OK;
}

int tcp_listen(int port) {
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

  return_value_if_fail(socket_bind(sock, port) == RET_OK, -1);

  if (listen(sock, 1) < 0) {
    log_debug("listen error\n");
    return -1;
  }

  return (sock);
}

int udp_listen(int port) {
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

struct sockaddr* socket_resolve(const char* host, int port, struct sockaddr_in* addr) {
  struct hostent* h = NULL;
  return_value_if_fail(host != NULL && addr != NULL, NULL);

  h = gethostbyname(host);
  return_value_if_fail(h != NULL, NULL);

  memset(addr, 0x00, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  memcpy(&(addr->sin_addr.s_addr), h->h_addr, h->h_length);

  return (struct sockaddr*)addr;
}

int tcp_connect(const char* host, int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  struct sockaddr* addr = socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    log_debug("socket error\n");
    return -1;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    log_debug("connect error\n");
    socket_close(sock);
    return -1;
  }

  return sock;
}

int udp_connect(const char* host, int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  struct sockaddr* addr = socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    log_debug("socket error\n");
    return -1;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    log_debug("connect error\n");
    socket_close(sock);
    return -1;
  }

  return sock;
}

int tcp_accept(int sock) {
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

/** Returns TRUE on success, or FALSE if there was an error */
bool_t socket_set_blocking(int sock, bool_t blocking) {
  if (sock < 0) return FALSE;

#ifdef _WIN32
  unsigned long mode = blocking ? 0 : 1;
  return (ioctlsocket(sock, FIONBIO, &mode) == 0) ? TRUE : FALSE;
#else
  int flags = fcntl(sock, F_GETFL, 0);
  if (flags == -1) return FALSE;
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  return (fcntl(sock, F_SETFL, flags) == 0) ? TRUE : FALSE;
#endif
}

ret_t socket_wait_for_data(int sock, uint32_t timeout_ms) {
  fd_set fdsr;
  int ret = 0;
  struct timeval tv = {0, 0};

  FD_ZERO(&fdsr);
  FD_SET(sock, &fdsr);

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  FD_SET(sock, &fdsr);
  ret = select(sock + 1, &fdsr, NULL, NULL, &tv);

  return ret > 0 ? RET_OK : RET_TIMEOUT;
}

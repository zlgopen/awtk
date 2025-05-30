/**
 * File:   socket_helper.h
 * Author: AWTK Develop Team
 * Brief:  socket helper functions
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
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
#endif /*LINUX*/

#ifdef WIN32
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "iphlpapi")
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

ret_t tk_socket_get_ips_by_ifname(const wchar_t* ifname, darray_t* ips) {
  ULONG ret = 0;
  ULONG size = 0;
  ret_t push_ret = RET_FAIL;
  PIP_ADAPTER_ADDRESSES pAddr = NULL;
  return_value_if_fail(ifname != NULL && ips != NULL, RET_BAD_PARAMS);
  ret = GetAdaptersAddresses(AF_INET, 0, NULL, NULL, &size);
  
  if (ret == ERROR_BUFFER_OVERFLOW) {
    pAddr = (PIP_ADAPTER_ADDRESSES)malloc(size);
    ret = GetAdaptersAddresses(AF_INET, 0, NULL, pAddr, &size);
  }
  
  while (pAddr && ret == NO_ERROR) {
    if (wcscmp(pAddr->FriendlyName, ifname) == 0) {
      PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAddr->FirstUnicastAddress;
       while (pUnicast) {
        char ip[16] = {0};
        size_t len = sizeof(ip);
        struct sockaddr_in* sa = (struct sockaddr_in*)pUnicast->Address.lpSockaddr;
        inet_ntop(AF_INET, &(sa->sin_addr), ip, len);
        push_ret = darray_push(ips, tk_strdup(ip));
        break_if_fail(push_ret == RET_OK);
        pUnicast = pUnicast->Next;
      }
    }
    pAddr = pAddr->Next;
  }
  
  free(pAddr);
  return push_ret;
}
#else
#if defined(LINUX) || defined(MACOS)
#include <ifaddrs.h>
ret_t tk_socket_get_ips_by_ifname(const wchar_t* ifname, darray_t* ips) {
  ret_t ret = RET_FAIL;
  const char* name = NULL;
  struct ifaddrs *ifaddr, *ifa;
  return_value_if_fail(ifname != NULL && ips != NULL, RET_BAD_PARAMS);

  name = tk_utf8_dup_wstr(ifname);
  return_value_if_fail(name != NULL, RET_OOM);

  if (getifaddrs(&ifaddr) == -1) {
    TKMEM_FREE(name);
    return RET_FAIL;
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr && strcmp(ifa->ifa_name, name) == 0) {
      if (ifa->ifa_addr->sa_family == AF_INET) {
        struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sa->sin_addr, ip, sizeof(ip));
        ret = darray_push(ips, tk_strdup(ip));
        break_if_fail(ret == RET_OK);
      }
    }
  }

  freeifaddrs(ifaddr);
  TKMEM_FREE(name);
  return ret;
}
#endif

ret_t tk_socket_init() {
  tk_ignore_sig_pipe();
  return RET_OK;
}
ret_t tk_socket_deinit() {
  return RET_OK;
}

ret_t tk_socket_close(int sock) {
#if defined(AWORKS) || defined(WITH_LWIP)
  closesocket(sock);
#else
  close(sock);
#endif
  return RET_OK;
}
#endif /*WIN32*/

ret_t tk_socket_bind_ex(int sock, const char* ip, int port) {
  struct sockaddr_in s;
  return_value_if_fail(sock >= 0 && port >= 0, RET_BAD_PARAMS);

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

int tk_socket_get_port(int sock) {
  struct sockaddr_in serv_addr;
  socklen_t serv_addr_len = sizeof(serv_addr);

  if (getsockname(sock, (struct sockaddr*)&serv_addr, &serv_addr_len) >= 0) {
    return ntohs(serv_addr.sin_port);
  }
  return -1;
}

int tk_tcp_listen_ex(const char* ip, int port) {
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

  return_value_if_fail(tk_socket_bind_ex(sock, ip, port) == RET_OK, -1);

  if (listen(sock, 5) < 0) {
    log_debug("listen error\n");
    return -1;
  }

  if (port == 0) {
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);

    if (getsockname(sock, (struct sockaddr*)&serv_addr, &serv_addr_len) >= 0) {
      log_debug("port is 0, system auto allocted port is %d\n", ntohs(serv_addr.sin_port));
    }
  }

  return (sock);
}

int tk_tcp_listen(int port) {
  return tk_tcp_listen_ex(NULL, port);
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

bool_t tk_tcp_is_port_in_use(int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  const char* host = "0.0.0.0";
  struct sockaddr* addr = tk_socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    return FALSE;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    tk_socket_close(sock);
    return FALSE;
  } else {
    tk_socket_close(sock);
    return TRUE;
  }
}

int tk_tcp_connect(const char* host, int port) {
  int sock = 0;
  struct sockaddr_in s_in;
  struct sockaddr* addr = tk_socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    log_debug("socket for %s %d failed\n", host, port);
    return -1;
  }

  if (connect(sock, addr, sizeof(s_in)) < 0) {
    log_debug("connect %s %d failed\n", host, port);
    tk_socket_close(sock);
    return -1;
  }

  return sock;
}

int tk_tcp_connect_ex(const char* host, int port, int timeout, void* opts) {
  int ret = 0;
  int sock = 0;
  int sockfd = -1;
  int error = 0, wsaError = 0;
  socklen_t len;
  fd_set rset, wset;
  struct sockaddr_in s_in;
  struct timeval tv = {0, 0};
  struct sockaddr* addr = tk_socket_resolve(host, port, &s_in);
  return_value_if_fail(addr != NULL, -1);
  return_value_if_fail(opts == NULL, -1);

  if (timeout <= 0) {
    return tk_tcp_connect(host, port);
  }

  if ((sock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    log_debug("socket error\n");
    return -1;
  }

  tk_socket_set_blocking(sock, FALSE);

  ret = connect(sock, addr, sizeof(s_in));

#ifdef _WIN32
  if (ret < 0) {
    wsaError = WSAGetLastError();
  }
  if (wsaError != WSAEWOULDBLOCK && wsaError != WSAEINPROGRESS) {
#else
  (void)wsaError;
  if (ret < 0 && errno != EINPROGRESS) {
#endif
    log_debug("connect error\n");
    goto done;
  } else if (ret == 0) {
    sockfd = sock;
    goto done;
  }

  FD_ZERO(&rset);
  FD_ZERO(&wset);
  FD_SET(sock, &rset);
  FD_SET(sock, &wset);
  tv.tv_sec = timeout / 1000;
  tv.tv_usec = (timeout % 1000) * 1000;

  ret = select(sock + 1, &rset, &wset, NULL, timeout == 0xFFFFFFFF ? NULL : &tv);
  if (ret <= 0) {
    /* Timeout or fail */
    goto done;
  }
  if (FD_ISSET(sock, &rset) || FD_ISSET(sock, &wset)) {
    len = sizeof(error);
    ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)&error, &len);
    if (ret != 0 || error != 0) {
#ifndef _WIN32
      errno = ECONNREFUSED;
#endif
      goto done;
    } else {
      sockfd = sock;
    }
  } else {
    goto done;
  }
done:
  tk_socket_set_blocking(sock, TRUE);
  if (sockfd == -1) {
    tk_socket_close(sock);
  }
  return sockfd;
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

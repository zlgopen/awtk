
#ifdef WIN32
#include "windows.h"
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

void socket_init() {
}
void socket_deinit() {
}
void socket_close(int sock) {
  close(sock);
}
#endif /**/

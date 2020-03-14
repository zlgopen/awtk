#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "streams/inet/iostream_udp.h"
#include "tkc/socket_helper.h"

void do_recv(int port) {
  int32_t n = 10;
  int32_t ret = 0;
  char buff[1024];
  socklen_t len = 0;
  struct sockaddr_in addr;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  socket_bind(sock, port);

  while (n > 0) {
    memset(buff, 0x00, sizeof(buff));
    memset(&addr, 0x00, sizeof(addr));

    len = sizeof(addr);
    ret = recvfrom(sock, (uint8_t*)buff, sizeof(buff), 0, (struct sockaddr*)&addr, &len);
    log_debug("read ret=%d %s\n", ret, buff);

    if (ret > 0) {
      ret = sendto(sock, (uint8_t*)buff, ret, 0, (struct sockaddr*)&addr, len);
      log_debug("write %d: %s\n", ret, buff);
    }

    n--;
    log_debug("remain %d times\n", n);
  }

  socket_close(sock);

  return;
}

int main(int argc, char* argv[]) {
  int port = 0;

  if (argc != 2) {
    printf("Usage: %s port\n", argv[0]);
    return 0;
  }

  socket_init();
  platform_prepare();
  TK_ENABLE_CONSOLE();

  port = tk_atoi(argv[1]);
  do_recv(port);

  socket_deinit();

  return 0;
}

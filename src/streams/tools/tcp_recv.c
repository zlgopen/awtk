#include "tkc/platform.h"
#include "streams/inet/iostream_tcp.h"

int main(int argc, char* argv[]) {
  int32_t ret = 0;
  int32_t lsock = 0;
  tk_iostream_t* io = NULL;
  const char* response = "200 OK\n";

  tk_socket_init();
  platform_prepare();
  lsock = tk_tcp_listen(8080);
  return_value_if_fail(lsock > 0, 0);

  log_debug("listen at 8080...\n");
  while (1) {
    int32_t sock = 0;
    sock = tk_tcp_accept(lsock);
    if (sock < 0) {
      break;
    }
    log_debug("%d client come in...\n", sock);
    io = tk_iostream_tcp_create(sock);
    if (io != NULL) {
      char buff[1024] = {0};
      tk_istream_t* in = tk_iostream_get_istream(io);
      tk_ostream_t* out = tk_iostream_get_ostream(io);

      ret = tk_istream_read(in, buff, sizeof(buff));
      log_debug("recv %d %s\n", ret, buff);

      ret = tk_ostream_write(out, response, strlen(response));
      log_debug("send %d %s\n", ret, response);

      TK_OBJECT_UNREF(io);
    }
  }
  tk_socket_close(lsock);

  return 0;
}

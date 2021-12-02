#include "tkc/platform.h"
#include "streams/inet/iostream_tcp.h"

int main(int argc, char* argv[]) {
  int32_t ret = 0;
  int32_t sock = 0;
  tk_iostream_t* io = NULL;
  const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\n";

  tk_socket_init();
  platform_prepare();
  sock = tk_tcp_connect("localhost", 8080);
  return_value_if_fail(sock > 0, 0);

  io = tk_iostream_tcp_create(sock);
  if (io != NULL) {
    char buff[1024] = {0};
    tk_istream_t* in = tk_iostream_get_istream(io);
    tk_ostream_t* out = tk_iostream_get_ostream(io);

    ret = tk_ostream_write(out, request, strlen(request));
    log_debug("send %d %s\n", ret, request);

    ret = tk_istream_read(in, buff, sizeof(buff));
    log_debug("recv %d %s\n", ret, buff);

    TK_OBJECT_UNREF(io);
  }

  return 0;
}

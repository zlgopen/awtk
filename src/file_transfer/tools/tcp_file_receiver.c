#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "file_transfer/file_receiver.h"
#include "streams/inet/iostream_tcp.h"
#include "tkc/socket_helper.h"
#include "streams/file/ostream_file.h"

#include "receiver.inc"

int main(int argc, char* argv[]) {
  int sock = 0;
  int port = 0;
  const char* host = NULL;
  const char* filename = NULL;

  if (argc != 4) {
    printf("Usage: %s host port filename\n", argv[0]);
    return 0;
  }

  socket_init();
  platform_prepare();
  TK_ENABLE_CONSOLE();

  host = argv[1];
  port = tk_atoi(argv[2]);
  filename = argv[3];

  sock = tcp_connect(host, port);

  if (sock > 0) {
    log_debug("connected %d \n", sock);
    tk_iostream_t* io = tk_iostream_tcp_create(sock);
    tk_ostream_t* out = tk_ostream_file_create(filename);
    do_receive(io, out, filename);
    OBJECT_UNREF(io);
    OBJECT_UNREF(out);
  }

  socket_deinit();

  return 0;
}

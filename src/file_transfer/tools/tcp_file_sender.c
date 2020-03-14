#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "file_transfer/file_sender.h"
#include "streams/inet/iostream_tcp.h"
#include "tkc/socket_helper.h"

#include "sender.inc"

int main(int argc, char* argv[]) {
  int sock = 0;
  int port = 0;
  const char* filename = NULL;

  if (argc != 3) {
    printf("Usage: %s port filename\n", argv[0]);
    return 0;
  }

  socket_init();
  platform_prepare();
  TK_ENABLE_CONSOLE();

  port = tk_atoi(argv[1]);
  filename = argv[2];

  return_value_if_fail(file_exist(filename), 0);

  sock = tcp_listen(port);
  return_value_if_fail(sock > 0, 0);

  log_debug("listen port=%d sock=%d ...\n", port, sock);
  do {
    int client = tcp_accept(sock);
    log_debug("connected %d ...\n", client);
    tk_iostream_t* io = tk_iostream_tcp_create(client);
    do_send(io, filename);
    OBJECT_UNREF(io);
  } while (1);

  socket_close(sock);
  socket_deinit();

  return 0;
}

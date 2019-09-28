#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "streams/iostream_udp.h"
#include "streams/socket_helper.h"

void do_recv(tk_iostream_t* iostream) {
  int32_t n = 10;
  int32_t ret = 0;
  char buff[1024];
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  while (n > 0) {
    memset(buff, 0x00, sizeof(buff));
    ret = tk_istream_read(istream, (uint8_t*)buff, sizeof(buff));
    log_debug("read ret=%d %s\n", ret, buff);

    if (ret > 0) {
      ret = tk_ostream_write(ostream, (uint8_t*)buff, ret);
      log_debug("write %d: %s\n", ret, buff);
    }

    n--;
    log_debug("remain %d times\n", n);
  }

  object_unref(OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  int port = 0;

  if (argc != 2) {
    printf("Usage: %s port\n", argv[0]);
    return 0;
  }

  socket_init();
  TK_ENABLE_CONSOLE();

  port = tk_atoi(argv[1]);
  do_recv(tk_iostream_udp_create_server(port));

  socket_deinit();

  return 0;
}

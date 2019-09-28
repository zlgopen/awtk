#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/platform.h"
#include "streams/iostream_udp.h"
#include "streams/socket_helper.h"

void do_send(tk_iostream_t* iostream, const char* msg) {
  char buff[128];
  uint32_t size = strlen(msg) + 1;
  int32_t ret = 0;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  ret = tk_ostream_write(ostream, (uint8_t*)&size, sizeof(size));
  ret = tk_ostream_write(ostream, (uint8_t*)msg, size);
  log_debug("send ret=%d %s\n", ret, msg);

  ret = tk_istream_read(istream, (uint8_t*)buff, sizeof(buff));
  while (ret == 0) {
    ret = tk_istream_read(istream, (uint8_t*)buff, sizeof(buff));
    sleep_ms(100);
  }
  tk_istream_flush(istream);

  if (ret >= 0) {
    buff[ret] = '\0';
    log_debug("read: %d %s\n", ret, buff);
  } else {
    log_debug("read failed\n");
  }

  sleep_ms(2000);
  object_unref(OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  int port = 0;

  if (argc != 3) {
    printf("Usage: %s port msg\n", argv[0]);
    return 0;
  }

  socket_init();
  port = tk_atoi(argv[1]);
  do_send(tk_iostream_udp_create("localhost", port), argv[2]);
  socket_deinit();
  
  return 0;
}

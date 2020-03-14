#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/platform.h"
#include "streams/serial/iostream_serial.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

void do_echo(tk_iostream_t* iostream) {
  char buff[128];
  int32_t ret = 0;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  while (ret == 0) {
    ret = tk_istream_read(istream, (uint8_t*)buff, sizeof(buff));
    sleep_ms(1000);
  }
  tk_istream_flush(istream);

  if (ret >= 0) {
    buff[ret] = '\0';
    log_debug("read: %s\n", buff);
    ret = tk_ostream_write(ostream, (uint8_t*)buff, ret);
    log_debug("write: %d %s\n", ret, buff);
    sleep_ms(2000);
  } else {
    log_debug("read failed\n");
  }

  object_unref(OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s port\n", argv[0]);
    return 0;
  }

  TK_ENABLE_CONSOLE();

  socket_init();
  platform_prepare();
  do_echo(tk_iostream_serial_create(argv[1]));
  socket_deinit();

  return 0;
}

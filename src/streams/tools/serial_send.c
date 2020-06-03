#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/platform.h"
#include "streams/serial/iostream_serial.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

void do_send(tk_iostream_t* iostream, const char* msg) {
  char buff[128];
  int32_t ret = 0;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  ret = tk_ostream_write(ostream, (uint8_t*)msg, strlen(msg));
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
  tk_iostream_t* serial = NULL;

  if (argc != 3) {
    printf("Usage: %s port msg\n", argv[0]);
    return 0;
  }

  TK_ENABLE_CONSOLE();

  socket_init();
  platform_prepare();

  serial = tk_iostream_serial_create(argv[1]);

  tk_iostream_serial_config(serial, 115200, eightbits, parity_none, stopbits_one, flowcontrol_none);

  do_send(serial, argv[2]);

  socket_deinit();

  return 0;
}

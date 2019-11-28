#include "tkc/utils.h"
#include "tkc/platform.h"
#include "streams/serial/iostream_serial.h"
#include "streams/serial/serial_helper.h"

void do_send(tk_iostream_t* iostream, const char* msg) {
  char buff[128];
  int32_t ret = 0;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  tk_snprintf(buff, sizeof(buff), "%s\r\n", msg);

  ret = tk_ostream_write(ostream, (uint8_t*)buff, strlen(buff));
  log_debug("send ret=%d %s\n", ret, buff);

  ret = tk_istream_read_line(istream, (uint8_t*)buff, sizeof(buff), 5000);
  while (ret > 0) {
    ret = tk_istream_read_line(istream, (uint8_t*)buff, sizeof(buff), 5000);
    sleep_ms(100);
  }
  tk_istream_flush(istream);

  if (ret >= 0) {
    buff[ret] = '\0';
    log_debug("read: %d %s\n", ret, buff);
  } else {
    log_debug("read failed\n");
  }

  object_unref(OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage: %s port msg\n", argv[0]);
    return 0;
  }

  TK_ENABLE_CONSOLE();
  do_send(tk_iostream_serial_create(argv[1]), argv[2]);

  return 0;
}

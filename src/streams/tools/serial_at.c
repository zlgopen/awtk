#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/socket_helper.h"
#include "streams/serial/iostream_serial.h"
#include "streams/serial/serial_helper.h"

#define TEST_READ_LEN 1

void do_send(tk_iostream_t* iostream, const char* msg) {
  char buff[128];
  int32_t ret = 0;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  tk_snprintf(buff, sizeof(buff), "%s\r\n", msg);

  ret = tk_ostream_write(ostream, (uint8_t*)buff, strlen(buff));
  log_debug("send ret=%d %s\n", ret, buff);
#ifdef TEST_READ_LEN
  log_debug("---------test tk_istream_read_len ---------------- \r\n");
#else
  log_debug("---------test tk_istream_read_line ---------------- \r\n");
#endif
  for (uint32_t i = 0; i < 100000; i++) {
    memset(buff, 0x0, sizeof(buff));
#ifdef TEST_READ_LEN
    ret = tk_istream_read_len(istream, (uint8_t*)buff, sizeof(buff), 5000);
#else
    ret = tk_istream_read_line(istream, (uint8_t*)buff, sizeof(buff), 5000);
#endif
    if (ret > 0) {
      log_debug("read data :%s--- \r\n", buff);
    } else {
      log_debug("read failed : %d \r\n", ret);
    }
  }

  tk_istream_flush(istream);

  if (ret >= 0) {
    buff[ret] = '\0';
    log_debug("read: %d %s\n", ret, buff);
  } else {
    log_debug("read failed\n");
  }

  tk_object_unref(TK_OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  tk_iostream_t* serial = NULL;
  if (argc != 3) {
    printf("Usage: %s port msg\n", argv[0]);
    return 0;
  }
  TK_ENABLE_CONSOLE();
  tk_socket_init();
  platform_prepare();

  serial = tk_iostream_serial_create(argv[1]);
  tk_iostream_serial_config(serial, 115200, eightbits, parity_none, stopbits_one, flowcontrol_none);
  do_send(serial, argv[2]);

  return 0;
}

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/platform.h"
#include "streams/serial/iostream_serial.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

void do_echo(tk_iostream_t* iostream) {
  int32_t ret = 0;
  uint32_t size = 0;
  char* buff = NULL;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  while (ret == 0) {
    ret = tk_istream_read(istream, (uint8_t*)&size, sizeof(size));
    log_debug("size=%u ret=%d\n", size, ret);
    sleep_ms(1000);
  }
  return_if_fail(ret == sizeof(size));

  buff = TKMEM_ALLOC(size + 1);
  memset(buff, 0x00, size + 1);
  log_debug("size=%u\n", size);
  ret = tk_istream_read_len(istream, (uint8_t*)buff, size, 600 * 1000);
  log_debug("(%u==%d):%s", size, ret, buff);

  if (ret > 0) {
    tk_ostream_write_len(ostream, &ret, sizeof(ret), 10000);
    tk_ostream_write_len(ostream, buff, ret, 600 * 1000);
  }

  TKMEM_FREE(buff);
  sleep_ms(10000);
  tk_object_unref(TK_OBJECT(iostream));

  return;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s port\n", argv[0]);
    return 0;
  }

  TK_ENABLE_CONSOLE();

  tk_socket_init();
  platform_prepare();
  do_echo(tk_iostream_serial_create(argv[1]));
  tk_socket_deinit();

  return 0;
}

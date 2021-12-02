#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/platform.h"
#include "streams/serial/iostream_serial.h"
#include "tkc/socket_helper.h"
#include "streams/serial/serial_helper.h"

void do_send(tk_iostream_t* iostream, const char* filename) {
  int32_t ret = 0;
  uint32_t size = 0;
  uint8_t* data = NULL;
  tk_istream_t* istream = tk_iostream_get_istream(iostream);
  tk_ostream_t* ostream = tk_iostream_get_ostream(iostream);

  data = file_read(filename, &size);
  return_if_fail(data != NULL);

  ret = tk_ostream_write_len(ostream, (uint8_t*)&size, sizeof(size), 1000);
  ret = tk_ostream_write_len(ostream, (uint8_t*)data, size, 600 * 1000);
  log_debug("send size=%u ret=%d \n", size, ret);

  memset(data, 0x00, size);
  ret = tk_istream_read_len(istream, (uint8_t*)&size, sizeof(size), 1000);
  return_if_fail(ret == sizeof(size));
  log_debug("size=%u\n", size);
  ret = tk_istream_read_len(istream, (uint8_t*)data, size, 600 * 1000);

  if (ret >= 0) {
    data[ret] = '\0';
    log_debug("read: %s\n%d\n", data, ret);
  } else {
    log_debug("read failed\n");
  }

  TKMEM_FREE(data);
  TK_OBJECT_UNREF(iostream);

  return;
}

int main(int argc, char* argv[]) {
  tk_iostream_t* serial = NULL;

  if (argc != 3) {
    printf("Usage: %s port filename\n", argv[0]);
    return 0;
  }

  TK_ENABLE_CONSOLE();

  tk_socket_init();
  platform_prepare();

  serial = tk_iostream_serial_create(argv[1]);

  tk_iostream_serial_config(serial, 115200, eightbits, parity_none, stopbits_one, flowcontrol_none);

  do_send(serial, argv[2]);

  tk_socket_deinit();

  return 0;
}

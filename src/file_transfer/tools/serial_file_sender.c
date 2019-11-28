#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "file_transfer/file_sender.h"
#include "streams/serial/iostream_serial.h"

#include "sender.inc"

int main(int argc, char* argv[]) {
  const char* port = NULL;
  const char* filename = NULL;

  if (argc != 3) {
    printf("Usage: %s port filename\n", argv[0]);
    return 0;
  }

  platform_prepare();
  TK_ENABLE_CONSOLE();

  port = argv[1];
  filename = argv[2];
  return_value_if_fail(file_exist(filename), 0);

  do {
    tk_iostream_t* io = tk_iostream_serial_create(port);
    do_send(io, filename);
    OBJECT_UNREF(io);
  } while (1);

  return 0;
}

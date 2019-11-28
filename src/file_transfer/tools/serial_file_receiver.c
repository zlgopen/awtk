#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "file_transfer/file_receiver.h"
#include "streams/serial/iostream_serial.h"
#include "streams/file/ostream_file.h"

#include "receiver.inc"

int main(int argc, char* argv[]) {
  const char* port = NULL;
  const char* filename = NULL;
  tk_iostream_t* io = NULL;
  tk_ostream_t* target = NULL;

  if (argc != 3) {
    printf("Usage: %s port filename\n", argv[0]);
    return 0;
  }

  platform_prepare();
  TK_ENABLE_CONSOLE();

  port = argv[1];
  filename = argv[2];

  io = tk_iostream_serial_create(port);
  target = tk_ostream_file_create(filename);
  do_receive(io, target, filename);
  OBJECT_UNREF(io);
  OBJECT_UNREF(target);

  return 0;
}

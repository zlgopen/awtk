#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/buffer.h"
#include "ubjson/ubjson_parser.h"

int main(int argc, char* argv[]) {
  void* data = NULL;
  uint32_t size = 0;

  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    return 0;
  }

  data = file_read(argv[1], &size);
  return_value_if_fail(data != NULL, 0);

  ubjson_dump(data, size);

  TKMEM_FREE(data);

  return 0;
}

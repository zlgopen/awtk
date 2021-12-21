#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/platform.h"
#include "font_gen/preprocess_text.inc"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage:%s input_file output_file\n", argv[0]);
  } else {
    uint32_t size = 0;
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    void* data = file_read(input_file, &size);

    if (data == NULL) {
      printf("file %s not found\n", input_file);
    } else {
      str_t str;
      str_init(&str, 100000);

      if (font_gen_expand_text(data, &str) != NULL) {
        file_write(output_file, str.str, str.size);
      }

      str_reset(&str);
      TKMEM_FREE(data);
    }
  }
  return 0;
}

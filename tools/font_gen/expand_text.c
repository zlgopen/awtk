#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/platform.h"
#include "font_gen/preprocess_text.inc"

int wmain(int argc, wchar_t* argv[]) {
  if (argc != 3) {
    printf("Usage:%s input_file output_file\n", argv[0]);
  } else {
    uint32_t size = 0;
    str_t str_input_file = {0};
    str_t str_output_file = {0};
    const char* input_file = NULL;
    const char* output_file = NULL;

    str_init(&str_input_file, 0);
    str_init(&str_output_file, 0);
    str_from_wstr(&str_input_file, argv[1]);
    str_from_wstr(&str_output_file, argv[2]);
    input_file = str_input_file.str;
    output_file = str_output_file.str;

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

#include "common/main.inc"
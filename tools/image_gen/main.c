/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "image_gen.h"
#include "common/utils.h"
#include "base/image_manager.h"
#include "image_loader/image_loader_stb.h"

int main(int argc, char** argv) {
  bitmap_t image;
  uint32_t size = 0;
  uint8_t* buff = NULL;
  bool_t require_bgra = FALSE;
  bool_t enable_bgr565 = FALSE;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 3) {
    printf("Usage: %s in_filename out_filename (bgra|bgr565)\n", argv[0]);

    return 0;
  }

  if (argc >= 4) {
    const char* options = argv[3];

    if (strstr(options, "bgra") != NULL) {
      require_bgra = TRUE;
    }

    if (strstr(options, "bgr565") != NULL) {
      enable_bgr565 = TRUE;
    }
  }

  in_filename = argv[1];
  out_filename = argv[2];

  exit_if_need_not_update(in_filename, out_filename);

  buff = (uint8_t*)read_file(in_filename, &size);
  if (buff != NULL) {
    if (stb_load_image(0, buff, size, &image, require_bgra, enable_bgr565) == RET_OK) {
      if (image_gen(&image, out_filename) == RET_OK) {
        printf("done\n");
      } else {
        printf("gen %s failed\n", out_filename);
      }
    } else {
      printf("load %s failed\n", in_filename);
    }
    TKMEM_FREE(buff);
  }

  return 0;
}

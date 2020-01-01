/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

int wmain(int argc, wchar_t* argv[]) {
  bitmap_t image;
  uint32_t size = 0;
  uint8_t* buff = NULL;
  bool_t mono = FALSE;
  bool_t require_bgra = FALSE;
  bool_t enable_bgr565 = FALSE;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename (bgra|bgr565|mono)\n", argv[0]);

    return 0;
  }

  if (argc >= 4) {
    str_t options = {0};
    str_from_wstr(&options, argv[3]);

    if (strstr(options.str, "mono") != NULL) {
      mono = TRUE;
    }

    if (strstr(options.str, "bgra") != NULL) {
      require_bgra = TRUE;
    }

    if (strstr(options.str, "bgr565") != NULL) {
      enable_bgr565 = TRUE;
    }

    str_reset(&options);
  }

  str_t in_file;
  str_t out_file;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  exit_if_need_not_update(in_filename, out_filename);

  uint32_t ret = 0;
  buff = (uint8_t*)read_file(in_filename, &size);
  if (buff != NULL) {
    do {
      ret = stb_load_image(0, buff, size, &image, require_bgra, enable_bgr565);
      if (ret != RET_OK) {
        printf("load %s failed\n", in_filename);
        break;
      }
      ret = image_gen(&image, out_filename, mono);
      if (ret != RET_OK) {
        printf("gen %s failed\n", out_filename);
        break;
      } else {
        printf("done\n");
        break;
      }
    } while (FALSE);
    TKMEM_FREE(buff);
  }

  str_reset(&in_file);
  str_reset(&out_file);

  return ret;
}

#include "common/main.inc"

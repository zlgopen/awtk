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
#include "common/utils.h"
#include "font_gen.h"
#include "font_loader/font_loader_bitmap.h"
#include "font_loader/font_loader_truetype.h"

int main(int argc, char** argv) {
  uint32_t size = 0;
  font_t* font = NULL;
  char* str_buff = NULL;
  uint8_t* ttf_buff = NULL;
  uint32_t font_size = 20;
  const char* ttf_filename = NULL;
  const char* str_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc != 5) {
    printf("Usage: %s ttf_filename str_filename out_filename font_size\n", argv[0]);

    return 0;
  }

  ttf_filename = argv[1];
  str_filename = argv[2];
  out_filename = argv[3];
  font_size = atoi(argv[4]);

  exit_if_need_not_update(ttf_filename, out_filename);
  exit_if_need_not_update(str_filename, out_filename);

  ttf_buff = (uint8_t*)read_file(ttf_filename, &size);
  return_value_if_fail(ttf_buff != NULL, 0);

  font = font_truetype_create("default", ttf_buff, size);

  str_buff = read_file(str_filename, &size);
  return_value_if_fail(str_buff != NULL, 0);

  if (font != NULL) {
    font_gen(font, (uint16_t)font_size, str_buff, out_filename);
  }

  TKMEM_FREE(ttf_buff);
  TKMEM_FREE(str_buff);

  printf("done\n");

  return 0;
}

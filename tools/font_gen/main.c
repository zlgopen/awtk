/**
 * File:   main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "base/mem.h"
#include "common/utils.h"
#include "font/font_bitmap.h"
#include "font/font_stb.h"
#include "font_gen.h"

int main(int argc, char** argv) {
  uint32_t size = 0;
  font_t* font = NULL;
  char* str_buff = NULL;
  uint8_t* ttf_buff = NULL;
  uint32_t font_size = 20;
  const char* ttf_filename = NULL;
  const char* str_filename = NULL;
  const char* output_filename = NULL;

  static uint32_t s_heap_mem[1024 * 1024];
  mem_init(s_heap_mem, sizeof(s_heap_mem));

  if (argc != 5) {
    printf("Usage: ttf_filename str_filename output_filename font_size\n");

    return 0;
  }

  ttf_filename = argv[1];
  str_filename = argv[2];
  output_filename = argv[3];
  font_size = atoi(argv[4]);

  ttf_buff = (uint8_t*)read_file(ttf_filename, &size);
  return_value_if_fail(ttf_buff != NULL, 0);

  str_buff = read_file(str_filename, &size);
  return_value_if_fail(str_buff != NULL, 0);

  font = font_stb_create("default", font_size, ttf_buff);
  if (font != NULL) {
    font_gen(font, str_buff, output_filename);
  }

  MEM_FREE(ttf_buff);
  MEM_FREE(str_buff);

  printf("done\n");

  return 0;
}

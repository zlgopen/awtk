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
#include "image_gen.h"
#include "common/utils.h"
#include "base/image_manager.h"
#include "image_loader/image_loader_stb.h"

int main(int argc, char** argv) {
  bitmap_t image;
  uint32_t size = 0;
  uint8_t* buff = NULL;
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  image_loader_t* loader = NULL;

  static uint32_t s_heap_mem[1024 * 1024];
  mem_init(s_heap_mem, sizeof(s_heap_mem));

  if (argc != 3) {
    printf("Usage: in_filename out_filename\n");

    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  loader = image_loader_stb();
  buff = (uint8_t*)read_file(in_filename, &size);
  if(buff != NULL) {
    if (image_loader_load(loader, buff, size, &image) == RET_OK) {
      if (image_gen(&image, out_filename) == RET_OK) {
        printf("done\n");
      } else {
        printf("gen %s failed\n", out_filename);
      }
    } else {
      printf("load %s failed\n", in_filename);
    }
    free(buff);
  }

  return 0;
}

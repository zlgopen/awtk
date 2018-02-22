/**
 * File:   main.cc
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  generate theme date from xml
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */

#include "base/mem.h"
#include "xml_theme_gen.h"

int main(int argc, char* argv[]) {
  static uint32_t s_heap_mem[1024 * 1024];
  mem_init(s_heap_mem, sizeof(s_heap_mem));

  if (argc != 3) {
    printf("Usage: %s input output\n", argv[0]);
    return 0;
  }

  xml_gen(argv[1], argv[2]);

  return 0;
}

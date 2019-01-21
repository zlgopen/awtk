/**
 * File:   main.cc
 * Author: AWTK Develop Team
 * Brief:  generate theme date from xml
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */

#include "tkc/mem.h"
#include "common/utils.h"
#include "xml_theme_gen.h"

int main(int argc, char* argv[]) {
  bool_t output_bin = argc > 3;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024)

  if (argc < 3) {
    printf("Usage: %s input output [bin]\n", argv[0]);
    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  exit_if_need_not_update(in_filename, out_filename);
  xml_gen(in_filename, out_filename, output_bin);

  return 0;
}

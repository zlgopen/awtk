/**
 * File:   main.cc
 * Author: AWTK Develop Team
 * Brief:  generate str binary data from xml
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "xml_str_gen.h"
#include "tkc/str.h"

int wmain(int argc, wchar_t* argv[]) {
  bool_t output_bin = argc > 3;

  TKMEM_INIT(4 * 1024 * 1024)

  if (argc < 3) {
    printf("Usage: %S input outputidr [bin]\n", argv[0]);
    return 0;
  }

  const char* in_filename = NULL;
  const char* out_filename = NULL;

  str_t in_file;
  str_t out_file;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  xml_to_str_gen(in_filename, out_filename, output_bin);

  str_reset(&in_file);
  str_reset(&out_file);

  return 0;
}

#include "common/main.inc"

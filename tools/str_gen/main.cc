/**
 * File:   main.cc
 * Author: AWTK Develop Team
 * Brief:  generate str binary data from xml
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "xml_str_gen.h"

int main(int argc, char* argv[]) {
  bool_t output_bin = argc > 3;

  TKMEM_INIT(4 * 1024 * 1024)

  if (argc < 3) {
    printf("Usage: %s input outputidr [bin]\n", argv[0]);
    return 0;
  }

  xml_to_str_gen(argv[1], argv[2], output_bin);

  return 0;
}

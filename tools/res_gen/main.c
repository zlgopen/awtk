/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  res to c source
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
 * 2018-04-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "common/utils.h"
#include "base/assets_manager.h"

int main(int argc, char** argv) {
  uint32_t size = 0;
  uint8_t* input_buff = NULL;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc != 3) {
    printf("Usage: %s in_filename out_filename\n", argv[0]);
    return 0;
  }

  in_filename = argv[1];
  out_filename = argv[2];

  exit_if_need_not_update(in_filename, out_filename);

  input_buff = (uint8_t*)read_file(in_filename, &size);
  return_value_if_fail(input_buff != NULL, 0);
  if (end_with(in_filename, ".ttf")) {
    output_res_c_source(out_filename, ASSET_TYPE_FONT, ASSET_TYPE_FONT_TTF, input_buff, size);
  } else if (end_with(in_filename, ".png")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, input_buff, size);
  } else if (end_with(in_filename, ".jpg")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_JPG, input_buff, size);
  } else if (end_with(in_filename, ".gif")) {
    output_res_c_source(out_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_GIF, input_buff, size);
  } else {
    output_res_c_source(out_filename, ASSET_TYPE_DATA, 0, input_buff, size);
  }

  TKMEM_FREE(input_buff);

  printf("done\n");

  return 0;
}

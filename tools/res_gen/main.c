/**
 * File:   main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  res to c source
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
 * 2018-04-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "common/utils.h"
#include "base/resource_manager.h"

static uint32_t s_heap_mem[4 * 1024 * 1024];
int main(int argc, char** argv) {
  uint32_t size = 0;
  uint8_t* input_buff = NULL;
  const char* input_filename = NULL;
  const char* output_filename = NULL;

  mem_init(s_heap_mem, sizeof(s_heap_mem));

  if (argc != 3) {
    printf("Usage: %s input_filename output_filename\n", argv[0]);
    return 0;
  }

  input_filename = argv[1];
  output_filename = argv[2];

  input_buff = (uint8_t*)read_file(input_filename, &size);
  return_value_if_fail(input_buff != NULL, 0);
  if(end_with(input_filename, ".ttf")) {
    output_res_c_source(output_filename, RESOURCE_TYPE_FONT, RESOURCE_TYPE_FONT_TTF, input_buff, size); 
  } else if(end_with(input_filename, ".png")) {
    output_res_c_source(output_filename, RESOURCE_TYPE_IMAGE, RESOURCE_TYPE_IMAGE_PNG, input_buff, size); 
  } else if(end_with(input_filename, ".jpg")) {
    output_res_c_source(output_filename, RESOURCE_TYPE_IMAGE, RESOURCE_TYPE_IMAGE_JPG, input_buff, size); 
  } else {
    printf("Not supported file type!\n");
  }

  MEM_FREE(input_buff);

  printf("done\n");

  return 0;
}

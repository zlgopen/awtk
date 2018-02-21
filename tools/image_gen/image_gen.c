/**
 * File:   image_gen.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  bitmap image generator
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "image_gen/image_gen.h"
#include "base/mem.h"
#include "common/utils.h"
#include "image_loader/image_loader_bitmap.h"
#include "image_loader/image_loader_stb.h"

#define MAX_BUFF_SIZE 4 * 1024 * 1024

ret_t image_gen(bitmap_t* image, const char* output_filename) {
  uint32_t size = 0;
  char name[NAME_LEN + 1];
  uint8_t* buff = (uint8_t*)MEM_ALLOC(MAX_BUFF_SIZE);
  filename_to_name(output_filename, name, sizeof(name));

  size = image_gen_buff(image, name, buff, MAX_BUFF_SIZE);
  output_c_source(output_filename, "", name, buff, size);
  MEM_FREE(buff);

  return RET_OK;
}

uint32_t image_gen_buff(bitmap_t* image, const char* name, uint8_t* output_buff,
                        uint32_t buff_size) {
  size_t size = 0;
  bitmap_header_t* header = (bitmap_header_t*)output_buff;
  return_value_if_fail(image != NULL && name != NULL && output_buff != NULL, 0);
  size = sizeof(uint32_t) * image->w * image->h;
  return_value_if_fail((size + sizeof(bitmap_header_t)) < buff_size, RET_BAD_PARAMS);

  header->magic = IMAGE_BITMAP_MAGIC;
  header->w = image->w;
  header->h = image->h;
  header->format = image->format;
  strncpy(header->name, name, sizeof(header->name));
  header->name[sizeof(header->name) - 1] = '\0';
  memcpy(header->data, image->data, size);

  return size + sizeof(bitmap_header_t);
}

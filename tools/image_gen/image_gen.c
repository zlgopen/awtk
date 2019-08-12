/**
 * File:   image_gen.c
 * Author: AWTK Develop Team
 * Brief:  bitmap image generator
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "common/utils.h"
#include "image_gen/image_gen.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"
#include "image_loader/image_loader_stb.h"

#define MAX_BUFF_SIZE 2 * 1024 * 1024

ret_t image_gen(bitmap_t* image, const char* output_filename, bool_t mono) {
  uint32_t size = 0;
  uint8_t* buff = (uint8_t*)TKMEM_ALLOC(MAX_BUFF_SIZE);
  return_value_if_fail(buff != NULL, RET_FAIL);

  size = image_gen_buff(image, buff, MAX_BUFF_SIZE, mono);
  output_res_c_source(output_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_RAW, buff, size);
  TKMEM_FREE(buff);

  return RET_OK;
}

uint32_t image_gen_buff(bitmap_t* image, uint8_t* output_buff, uint32_t buff_size, bool_t mono) {
  size_t size = 0;
  bitmap_header_t* header = (bitmap_header_t*)output_buff;
  return_value_if_fail(image != NULL && output_buff != NULL, 0);

  header->w = image->w;
  header->h = image->h;
  header->flags = image->flags;
  if (!mono) {
    size = bitmap_get_line_length(image) * image->h;
    ENSURE((size + sizeof(bitmap_header_t)) < buff_size);

    header->format = image->format;
    memcpy(header->data, image->data, size);
  } else {
    bitmap_t b;
    bitmap_init_from_rgba(&b, image->w, image->h, BITMAP_FMT_MONO, image->data, 4);
    header->format = b.format;
    size = bitmap_get_line_length(&b) * b.h;
    ENSURE((size + sizeof(bitmap_header_t)) < buff_size);

    memcpy(header->data, b.data, size);

    bitmap_mono_dump(b.data, b.w, b.h);

    bitmap_destroy(&b);
  }

  return size + sizeof(bitmap_header_t);
}

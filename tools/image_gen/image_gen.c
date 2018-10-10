/**
 * File:   image_gen.c
 * Author: AWTK Develop Team
 * Brief:  bitmap image generator
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "common/utils.h"
#include "image_gen/image_gen.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"
#include "image_loader/image_loader_stb.h"

#define MAX_BUFF_SIZE 2 * 1024 * 1024

ret_t image_gen(bitmap_t* image, const char* output_filename) {
  uint32_t size = 0;
  uint8_t* buff = (uint8_t*)TKMEM_ALLOC(MAX_BUFF_SIZE);
  return_value_if_fail(buff != NULL, RET_FAIL);

  size = image_gen_buff(image, buff, MAX_BUFF_SIZE);
  output_res_c_source(output_filename, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_RAW, buff, size);
  TKMEM_FREE(buff);

  return RET_OK;
}

#include "base/utils.h"

uint32_t image_gen_buff(bitmap_t* image, uint8_t* output_buff, uint32_t buff_size) {
  size_t size = 0;
  bitmap_header_t* header = (bitmap_header_t*)output_buff;
  return_value_if_fail(image != NULL && output_buff != NULL, 0);

  size = sizeof(uint32_t) * image->w * image->h;
  return_value_if_fail((size + sizeof(bitmap_header_t)) < buff_size, RET_BAD_PARAMS);

  header->w = image->w;
  header->h = image->h;
  header->flags = image->flags;
  header->format = image->format;

  if (image->flags & BITMAP_FLAG_OPAQUE) {
    if (image->format == BITMAP_FMT_RGBA8888) {
      if (bitmap_rgba_to_rgb565(image, (uint16_t*)(header->data)) == RET_OK) {
        header->format = BITMAP_FMT_RGB565;
        size = sizeof(uint16_t) * image->w * image->h;
      } else {
        assert(!"bitmap_rgba_to_rgb565 fail.");
      }
    } else if (image->format == BITMAP_FMT_RGB565) {
      size = sizeof(uint16_t) * image->w * image->h;
      memcpy(header->data, image->data, size);
    } else {
      assert(!"not supported");
    }
  } else {
    size = sizeof(uint32_t) * image->w * image->h;
    memcpy(header->data, image->data, size);
  }

  return size + sizeof(bitmap_header_t);
}

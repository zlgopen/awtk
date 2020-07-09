/**
 * File:   image_gen.c
 * Author: AWTK Develop Team
 * Brief:  bitmap image generator
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

ret_t image_gen(bitmap_t* image, const char* output_filename, const char* theme, bool_t mono) {
  uint32_t size = 0;
  wbuffer_t wbuffer;
  wbuffer_init_extendable(&wbuffer);
  ret_t ret = RET_OK;
  size = image_gen_buff(image, &wbuffer, mono);
  if (size) {
    output_res_c_source(output_filename, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_RAW,
                        wbuffer.data, size);
  } else {
    ret = RET_FAIL;
  }
  wbuffer_deinit(&wbuffer);
  return ret;
}

uint32_t image_gen_buff(bitmap_t* image, wbuffer_t* wbuffer, bool_t mono) {
  size_t size = 0;
  uint8_t* image_data = NULL;
  return_value_if_fail(image != NULL, 0);
  uint32_t header_size = sizeof(bitmap_header_t) - 4;
  bitmap_header_t* header = (bitmap_header_t*)TKMEM_ALLOC(header_size);
  memset(header, 0, header_size);
  wbuffer_write_binary(wbuffer, header, header_size);

  header->w = image->w;
  header->h = image->h;
  header->flags = image->flags;
  image_data = bitmap_lock_buffer_for_read(image);
  if (!mono) {
    size = bitmap_get_line_length(image) * image->h;
    header->format = image->format;
    wbuffer_write_binary(wbuffer, image_data, size);
  } else {
    bitmap_t b;
    uint8_t* bdata = NULL;
    bitmap_init_from_rgba(&b, image->w, image->h, BITMAP_FMT_MONO, image_data, 4);
    header->format = b.format;
    size = bitmap_get_line_length(&b) * b.h;

    bdata = bitmap_lock_buffer_for_read(&b);
    wbuffer_write_binary(wbuffer, bdata, size);

    bitmap_mono_dump(bdata, b.w, b.h);
    bitmap_unlock_buffer(&b);
    bitmap_destroy(&b);
  }
  bitmap_unlock_buffer(image);
  int32_t cursor = wbuffer->cursor;
  wbuffer->cursor = 0;
  wbuffer_write_binary(wbuffer, header, header_size);
  wbuffer->cursor = cursor;
  TKMEM_FREE(header);

  return size + sizeof(bitmap_header_t);
}

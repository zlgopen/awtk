/**
 * File:   graphic_buffer_jzgpu.c
 * Author: AWTK Develop Team
 * Brief:  graphic_buffer jzgpu
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-10-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/bitmap.h"
#include "base/graphic_buffer.h"

#ifdef WITH_JZGPU
/**
 * @class graphic_buffer_jzgpu_t
 * graphic_buffer jzgpu
 */
typedef struct _graphic_buffer_jzgpu_t {
  graphic_buffer_t graphic_buffer;
  uint8_t* data;
  void* handle;
} graphic_buffer_jzgpu_t;

#define GRAPHIC_BUFFER_JZGPU(buffer) ((graphic_buffer_jzgpu_t*)(buffer))

static uint8_t* graphic_buffer_jzgpu_lock_for_read(graphic_buffer_t* buffer) {
  graphic_buffer_jzgpu_t* b = GRAPHIC_BUFFER_JZGPU(buffer);
  return_value_if_fail(b != NULL, NULL);

  return b->data;
}

static uint8_t* graphic_buffer_jzgpu_lock_for_write(graphic_buffer_t* buffer) {
  graphic_buffer_jzgpu_t* b = GRAPHIC_BUFFER_JZGPU(buffer);
  return_value_if_fail(b != NULL, NULL);

  return b->data;
}

static ret_t graphic_buffer_jzgpu_unlock(graphic_buffer_t* buffer) {
  return RET_OK;
}

static ret_t graphic_buffer_jzgpu_destroy(graphic_buffer_t* buffer) {
  graphic_buffer_jzgpu_t* b = GRAPHIC_BUFFER_JZGPU(buffer);
  return_value_if_fail(b != NULL, RET_BAD_PARAMS);

  /*TODO*/
  TKMEM_FREE(b);

  return RET_OK;
}

static const graphic_buffer_vtable_t s_graphic_buffer_jzgpu_vtable = {
    .lock_for_read = graphic_buffer_jzgpu_lock_for_read,
    .lock_for_write = graphic_buffer_jzgpu_lock_for_write,
    .unlock = graphic_buffer_jzgpu_unlock,
    .destroy = graphic_buffer_jzgpu_destroy};

static graphic_buffer_t* graphic_buffer_jzgpu_create(uint32_t w, uint32_t h, bitmap_format_t format,
                                                     uint32_t line_length) {
  uint32_t size = 0;
  uint8_t* data = NULL;
  graphic_buffer_jzgpu_t* buffer = NULL;
  uint32_t min_line_length = bitmap_get_bpp_of_format(format) * w;
  return_value_if_fail(w > 0 && h > 0, NULL);

  buffer = TKMEM_ZALLOC(graphic_buffer_jzgpu_t);
  return_value_if_fail(buffer != NULL, NULL);
  buffer->graphic_buffer.vt = &s_graphic_buffer_default_vtable;

  /*TODO*/
  return GRAPHIC_BUFFER(buffer);
}

/*public functions*/
graphic_buffer_t* graphic_buffer_create_with_data(const uint8_t* data, uint32_t w, uint32_t h,
                                                  bitmap_format_t format) {
  graphic_buffer_jzgpu_t* buffer = NULL;
  return_value_if_fail(data != NULL, NULL);

  buffer = TKMEM_ZALLOC(graphic_buffer_jzgpu_t);
  return_value_if_fail(buffer != NULL, NULL);

  buffer->data = (uint8_t*)data;
  buffer->graphic_buffer.vt = &s_graphic_buffer_jzgpu_vtable;

  return GRAPHIC_BUFFER(buffer);
}

ret_t graphic_buffer_create_for_bitmap(bitmap_t* bitmap) {
  uint32_t line_length = bitmap_get_line_length(bitmap);
  return_value_if_fail(bitmap != NULL && bitmap->buffer == NULL, RET_BAD_PARAMS);

  bitmap->buffer = graphic_buffer_jzgpu_create(bitmap->w, bitmap->h,
                                               (bitmap_format_t)(bitmap->format), line_length);
  bitmap->should_free_data = bitmap->buffer != NULL;

  return bitmap->buffer != NULL ? RET_OK : RET_OOM;
}
#endif /*WITH_JZGPU*/

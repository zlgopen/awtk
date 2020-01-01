/**
 * File:   compressor_miniz.c
 * Author: AWTK Develop Team
 * Brief:  compressor base on miniz
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed data the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-10-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "miniz/miniz.h"
#include "compressors/compressor_miniz.h"

static ret_t compressor_miniz_compress(compressor_t* compressor, const void* data, uint32_t size,
                                       wbuffer_t* out) {
  int level = 0;
  mz_ulong out_size = 0;
  return_value_if_fail(wbuffer_extend_capacity(out, 512 + size * 2) == RET_OK, RET_OOM);

  out_size = out->capacity;
  level = compressor->options == COMPRESSOR_SPEED_FIRST ? 3 : 6;

  if (mz_compress2((uint8_t*)(out->data), &out_size, (const uint8_t*)data, size, level) == MZ_OK) {
    out->cursor = out_size;

    return RET_OK;
  }
  out->cursor = 0;

  return RET_FAIL;
}

static ret_t compressor_miniz_uncompress(compressor_t* compressor, const void* data, uint32_t size,
                                         wbuffer_t* out) {
  mz_ulong out_size = 0;
  return_value_if_fail(wbuffer_extend_capacity(out, size * 5) == RET_OK, RET_OOM);

  out_size = out->capacity;

  if (mz_uncompress((uint8_t*)(out->data), &out_size, (const uint8_t*)data, size) == MZ_OK) {
    out->cursor = out_size;

    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t compressor_miniz_destroy(compressor_t* compressor) {
  TKMEM_FREE(compressor);

  return RET_OK;
}

compressor_t* compressor_miniz_create(compressor_options_t options) {
  compressor_t* compressor = TKMEM_ZALLOC(compressor_t);
  return_value_if_fail(compressor != NULL, NULL);

  compressor->type = "miniz";
  compressor->options = options;
  compressor->compress = compressor_miniz_compress;
  compressor->uncompress = compressor_miniz_uncompress;
  compressor->destroy = compressor_miniz_destroy;

  return compressor;
}

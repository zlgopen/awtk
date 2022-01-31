/**
 * File:   data_writer.c
 * Author: AWTK Develop Team
 * Brief:  data_writer
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License wbuffer for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/data_writer_wbuffer.h"

typedef struct _data_writer_wbuffer_t {
  data_writer_t data_writer;

  wbuffer_t* wbuffer;
} data_writer_wbuffer_t;

static int32_t data_writer_wbuffer_write(data_writer_t* writer, uint64_t offset, const void* data,
                                         uint32_t size) {
  data_writer_wbuffer_t* wbuffer = (data_writer_wbuffer_t*)writer;
  return_value_if_fail(wbuffer_extend_capacity(wbuffer->wbuffer, offset + size) == RET_OK, 0);

  wbuffer->wbuffer->cursor = offset;

  return wbuffer_write_binary(wbuffer->wbuffer, data, size) == RET_OK ? size : 0;
}

static ret_t data_writer_wbuffer_destroy(data_writer_t* writer) {
  TKMEM_FREE(writer);

  return RET_OK;
}

static const data_writer_vtable_t s_data_writer_wbuffer_vtable = {
    .write = data_writer_wbuffer_write,
    .destroy = data_writer_wbuffer_destroy,
};

data_writer_t* data_writer_wbuffer_create(const char* wbuffername) {
  int32_t nr = 0;
  data_writer_wbuffer_t* wbuffer = NULL;
  return_value_if_fail(wbuffername != NULL, NULL);
  wbuffer = TKMEM_ZALLOC(data_writer_wbuffer_t);
  return_value_if_fail(wbuffer != NULL, NULL);

  nr = tk_sscanf(wbuffername, "%p", &wbuffer->wbuffer);
  wbuffer->data_writer.vt = &s_data_writer_wbuffer_vtable;

  if (wbuffer->wbuffer == NULL && nr == 1) {
    TKMEM_FREE(wbuffer);
  }

  return (data_writer_t*)wbuffer;
}

const char* data_writer_wbuffer_build_url(wbuffer_t* buffer, char url[MAX_PATH + 1]) {
  return_value_if_fail(buffer != NULL, NULL);
  tk_snprintf(url, MAX_PATH, "wbuffer://%p", buffer);

  return url;
}

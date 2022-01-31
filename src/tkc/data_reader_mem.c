/**
 * File:   data_reader.c
 * Author: AWTK Develop Team
 * Brief:  data_reader
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License mem for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/data_reader_mem.h"

typedef struct _data_reader_mem_t {
  data_reader_t data_reader;

  uint32_t size;
  const uint8_t* data;
} data_reader_mem_t;

static int32_t data_reader_mem_read(data_reader_t* reader, uint64_t offset, void* data,
                                    uint32_t size) {
  uint32_t available_size = 0;
  data_reader_mem_t* mem = (data_reader_mem_t*)reader;
  return_value_if_fail(mem->data != NULL, 0);

  if (offset >= mem->size) {
    return 0;
  }

  available_size = mem->size - offset;
  size = tk_min(available_size, size);
  memcpy(data, mem->data + offset, size);

  return size;
}

static uint64_t data_reader_mem_get_size(data_reader_t* reader) {
  data_reader_mem_t* mem = (data_reader_mem_t*)reader;

  return mem->size;
}

static ret_t data_reader_mem_destroy(data_reader_t* reader) {
  TKMEM_FREE(reader);

  return RET_OK;
}

static const data_reader_vtable_t s_data_reader_mem_vtable = {
    .read = data_reader_mem_read,
    .get_size = data_reader_mem_get_size,
    .destroy = data_reader_mem_destroy,
};

data_reader_t* data_reader_mem_create(const char* memname) {
  int32_t nr = 0;
  uint32_t size = 0;
  const uint8_t* data = NULL;
  data_reader_mem_t* mem = NULL;
  return_value_if_fail(memname != NULL, NULL);
  mem = TKMEM_ZALLOC(data_reader_mem_t);
  return_value_if_fail(mem != NULL, NULL);
  nr = tk_sscanf(memname, "%p:%u", &data, &size);

  if (nr == 2 && data != NULL) {
    mem->size = size;
    mem->data = data;
    mem->data_reader.vt = &s_data_reader_mem_vtable;
  } else {
    TKMEM_FREE(mem);
  }

  return (data_reader_t*)mem;
}

const char* data_reader_mem_build_url(const void* buffer, uint32_t size, char url[MAX_PATH + 1]) {
  return_value_if_fail(buffer != NULL, NULL);
  tk_snprintf(url, MAX_PATH, "mem://%p:%u", buffer, size);

  return url;
}

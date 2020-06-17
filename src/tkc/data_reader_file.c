/**
 * File:   data_reader.c
 * Author: AWTK Develop Team
 * Brief:  data_reader
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/fs.h"
#include "tkc/data_reader_file.h"

typedef struct _data_reader_file_t {
  data_reader_t data_reader;

  fs_file_t* file;
  uint64_t size;
} data_reader_file_t;

static int32_t data_reader_file_read(data_reader_t* reader, uint64_t offset, void* data,
                                     uint32_t size) {
  data_reader_file_t* file = (data_reader_file_t*)reader;
  return_value_if_fail(fs_file_seek(file->file, offset) == RET_OK, 0);

  return fs_file_read(file->file, data, size);
}

static uint64_t data_reader_file_get_size(data_reader_t* reader) {
  data_reader_file_t* file = (data_reader_file_t*)reader;

  return file->size;
}

static ret_t data_reader_file_destroy(data_reader_t* reader) {
  data_reader_file_t* file = (data_reader_file_t*)reader;

  fs_file_close(file->file);
  TKMEM_FREE(file);

  return RET_OK;
}

static const data_reader_vtable_t s_data_reader_file_vtable = {
    .read = data_reader_file_read,
    .get_size = data_reader_file_get_size,
    .destroy = data_reader_file_destroy,
};

data_reader_t* data_reader_file_create(const char* filename) {
  fs_stat_info_t st;
  data_reader_file_t* file = NULL;
  return_value_if_fail(filename != NULL, NULL);
  file = TKMEM_ZALLOC(data_reader_file_t);
  return_value_if_fail(file != NULL, NULL);

  if (fs_stat(os_fs(), filename, &st) == RET_OK) {
    file->size = st.size;
    file->file = fs_open_file(os_fs(), filename, "rb");
    file->data_reader.vt = &s_data_reader_file_vtable;
  }

  if (file->file == NULL) {
    TKMEM_FREE(file);
  }

  return (data_reader_t*)file;
}

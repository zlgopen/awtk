/**
 * File:   data_writer.c
 * Author: AWTK Develop Team
 * Brief:  data_writer
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
 * 2020-03-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/fs.h"
#include "tkc/data_writer_file.h"

typedef struct _data_writer_file_t {
  data_writer_t data_writer;

  fs_file_t* file;
} data_writer_file_t;

static int32_t data_writer_file_write(data_writer_t* writer, uint64_t offset, const void* data,
                                      uint32_t size) {
  data_writer_file_t* file = (data_writer_file_t*)writer;
  return_value_if_fail(fs_file_seek(file->file, offset) == RET_OK, 0);

  return fs_file_write(file->file, data, size);
}

static ret_t data_writer_file_destroy(data_writer_t* writer) {
  data_writer_file_t* file = (data_writer_file_t*)writer;

  fs_file_close(file->file);
  TKMEM_FREE(file);

  return RET_OK;
}

static const data_writer_vtable_t s_data_writer_file_vtable = {
    .write = data_writer_file_write,
    .destroy = data_writer_file_destroy,
};

data_writer_t* data_writer_file_create(const char* filename) {
  data_writer_file_t* file = NULL;
  return_value_if_fail(filename != NULL, NULL);
  file = TKMEM_ZALLOC(data_writer_file_t);
  return_value_if_fail(file != NULL, NULL);

  file->file = fs_open_file(os_fs(), filename, "wb+");
  file->data_writer.vt = &s_data_writer_file_vtable;

  if (file->file == NULL) {
    TKMEM_FREE(file);
  }

  return (data_writer_t*)file;
}

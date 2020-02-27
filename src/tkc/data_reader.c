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

#include "tkc/data_reader.h"

int32_t data_reader_read(data_reader_t* reader, uint64_t offset, void* data, uint32_t size) {
  return_value_if_fail(reader != NULL && reader->vt != NULL && reader->vt->read != NULL, -1);
  return_value_if_fail(data != NULL && size > 0, -1);

  return reader->vt->read(reader, offset, data, size);
}

uint64_t data_reader_get_size(data_reader_t* reader) {
  return_value_if_fail(reader != NULL && reader->vt != NULL && reader->vt->get_size != NULL, 0);

  return reader->vt->get_size(reader);
}

ret_t data_reader_destroy(data_reader_t* reader) {
  return_value_if_fail(reader != NULL && reader->vt != NULL && reader->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return reader->vt->destroy(reader);
}

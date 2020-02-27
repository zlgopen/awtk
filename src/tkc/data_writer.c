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
 * 2020-02-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/data_writer.h"

int32_t data_writer_write(data_writer_t* writer, uint64_t offset, const void* data, uint32_t size) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->write != NULL, -1);
  return_value_if_fail(data != NULL && size > 0, -1);

  return writer->vt->write(writer, offset, data, size);
}

ret_t data_writer_destroy(data_writer_t* writer) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return writer->vt->destroy(writer);
}

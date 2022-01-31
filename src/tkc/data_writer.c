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
#include "tkc/data_writer_factory.h"

int32_t data_writer_write(data_writer_t* writer, uint64_t offset, const void* data, uint32_t size) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->write != NULL, -1);
  return_value_if_fail(data != NULL && size > 0, -1);

  return writer->vt->write(writer, offset, data, size);
}

ret_t data_writer_truncate(data_writer_t* writer, uint64_t size) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->truncate != NULL,
                       RET_BAD_PARAMS);

  return writer->vt->truncate(writer, size);
}

ret_t data_writer_flush(data_writer_t* writer) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->flush != NULL,
                       RET_BAD_PARAMS);

  return writer->vt->flush(writer);
}

ret_t data_writer_destroy(data_writer_t* writer) {
  return_value_if_fail(writer != NULL && writer->vt != NULL && writer->vt->destroy != NULL,
                       RET_BAD_PARAMS);

  return writer->vt->destroy(writer);
}

ret_t data_writer_clear(const char* url) {
  ret_t ret = RET_OK;
  data_writer_t* writer = data_writer_factory_create_writer(data_writer_factory(), url);
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);

  ret = data_writer_truncate(writer, 0);
  data_writer_destroy(writer);

  return ret;
}

int32_t data_writer_write_all(const char* url, const void* data, uint32_t size) {
  int32_t s = 0;
  data_writer_t* writer = data_writer_factory_create_writer(data_writer_factory(), url);
  return_value_if_fail(writer != NULL, -1);

  s = data_writer_write(writer, 0, data, size);
  data_writer_destroy(writer);

  return s;
}

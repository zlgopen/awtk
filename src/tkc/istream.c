/**
 * File:   tk_istream.h
 * Author: AWTK Develop Team
 * Brief:  input stream interface
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/istream.h"

int32_t tk_istream_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  return stream->read(stream, buff, max_size);
}

ret_t tk_istream_seek(tk_istream_t* stream, uint32_t offset) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->seek != NULL, RET_NOT_IMPL);

  return stream->seek(stream, offset);
}

int32_t tk_istream_read_len(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  int32_t offset = 0;
  int32_t read_bytes = 0;
  int32_t remain_bytes = max_size;
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  do {
    read_bytes = tk_istream_read(stream, buff + offset, remain_bytes);

    if (read_bytes <= 0) {
      break;
    }

    offset += read_bytes;
    remain_bytes -= read_bytes;
  } while (remain_bytes > 0);

  return offset;
}

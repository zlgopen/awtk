/**
 * File:   tk_ostream.h
 * Author: AWTK Develop Team
 * Brief:  output stream interface
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

#include "tkc/ostream.h"

int32_t tk_ostream_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  return_value_if_fail(stream != NULL && stream->write != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  return stream->write(stream, buff, max_size);
}

ret_t tk_ostream_seek(tk_ostream_t* stream, uint32_t offset) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->seek != NULL, RET_NOT_IMPL);

  return stream->seek(stream, offset);
}

int32_t tk_ostream_write_len(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  int32_t offset = 0;
  int32_t write_bytes = 0;
  int32_t remain_bytes = max_size;
  return_value_if_fail(stream != NULL && stream->write != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  do {
    write_bytes = tk_ostream_write(stream, buff + offset, remain_bytes);

    if (write_bytes <= 0) {
      break;
    }

    offset += write_bytes;
    remain_bytes -= write_bytes;
  } while (remain_bytes > 0);

  return offset;
}

ret_t tk_ostream_flush(tk_ostream_t* stream) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);

  if (stream->flush != NULL) {
    return stream->flush(stream);
  }

  return RET_OK;
}

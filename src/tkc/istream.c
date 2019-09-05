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

#include "tkc/time_now.h"
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

int32_t tk_istream_read_len(tk_istream_t* stream, uint8_t* buff, uint32_t max_size, uint32_t timeout_ms) {
  uint32_t start = 0;
  uint32_t end = 0;
  int32_t offset = 0;
  int32_t read_bytes = 0;
  int32_t remain_bytes = max_size;
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  start = time_now_ms(); 
  end = start + timeout_ms;

  do {
    read_bytes = tk_istream_read(stream, buff + offset, remain_bytes);

    if (read_bytes <= 0) {
      break;
    }

    offset += read_bytes;
    remain_bytes -= read_bytes;
    if(time_now_ms() > end) {
      log_debug("read timeout\n");
      break;
    }
  } while (remain_bytes > 0);

  return offset;
}

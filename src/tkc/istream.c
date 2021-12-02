/**
 * File:   tk_istream.h
 * Author: AWTK Develop Team
 * Brief:  input stream interface
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/istream.h"

int32_t tk_istream_read(tk_istream_t* stream, void* buff, uint32_t max_size) {
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  return stream->read(stream, (uint8_t*)buff, max_size);
}

ret_t tk_istream_seek(tk_istream_t* stream, uint32_t offset) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->seek != NULL, RET_NOT_IMPL);

  return stream->seek(stream, offset);
}

int32_t tk_istream_tell(tk_istream_t* stream) {
  return_value_if_fail(stream != NULL, -1);
  return_value_if_fail(stream->tell != NULL, -1);

  return stream->tell(stream);
}

bool_t tk_istream_eos(tk_istream_t* stream) {
  return_value_if_fail(stream != NULL, TRUE);
  return_value_if_fail(stream->eos != NULL, TRUE);

  return stream->eos(stream);
}

ret_t tk_istream_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->wait_for_data != NULL, RET_NOT_IMPL);

  return stream->wait_for_data(stream, timeout_ms);
}

ret_t tk_istream_flush(tk_istream_t* stream) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);

  if (stream->flush != NULL) {
    return stream->flush(stream);
  }

  return RET_OK;
}

int32_t tk_istream_read_len(tk_istream_t* stream, void* buff, uint32_t max_size,
                            uint32_t timeout_ms) {
  uint32_t now = 0;
  uint32_t end = 0;
  int32_t offset = 0;
  ret_t ret = RET_OK;
  int32_t read_bytes = 0;
  int32_t remain_bytes = max_size;
  uint8_t* p = (uint8_t*)buff;
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  now = time_now_ms();
  end = now + timeout_ms;

  do {
    errno = 0;
    ret = tk_istream_wait_for_data(stream, 20);

    if (ret == RET_TIMEOUT) {
      now = time_now_ms();
      if (now > end) {
        break;
      } else {
        continue;
      }
    } else if (ret != RET_OK) {
      break;
    }

    read_bytes = tk_istream_read(stream, p + offset, remain_bytes);
    if (read_bytes <= 0) {
      if (!tk_object_get_prop_bool(TK_OBJECT(stream), TK_STREAM_PROP_IS_OK, TRUE)) {
        log_debug("stream is broken\n");
        break;
      } else {
        now = time_now_ms();
        if (now > end) {
          log_debug("timeout\n");
          break;
        } else {
          log_debug("not get data\n");
          continue;
        }
      }
    }

    offset += read_bytes;
    remain_bytes -= read_bytes;

    if (remain_bytes == 0) {
      break;
    }

    if (tk_object_get_prop_bool(TK_OBJECT(stream), TK_STREAM_PROP_IS_EOS, FALSE)) {
      log_debug("stream is end\n");
      break;
    }

    now = time_now_ms();
    if (now > end) {
      break;
    }

    log_debug("read: %d/%u\n", offset, max_size);
  } while (remain_bytes > 0);

  return offset;
}

int32_t tk_istream_read_line(tk_istream_t* stream, void* buff, uint32_t max_size,
                             uint32_t timeout_ms) {
  uint32_t start = 0;
  uint32_t end = 0;
  int32_t offset = 0;
  int32_t read_bytes = 0;
  int32_t remain_bytes = max_size;
  uint8_t* p = (uint8_t*)buff;
  return_value_if_fail(stream != NULL && stream->read != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  start = time_now_ms();
  end = start + timeout_ms;

  do {
    read_bytes = tk_istream_read(stream, p + offset, 1);

    if (read_bytes < 0) {
      break;
    }

    offset += read_bytes;
    remain_bytes -= read_bytes;
    if (time_now_ms() > end) {
      break;
    }

    if (p[offset] == '\n') {
      break;
    }
  } while (remain_bytes > 0);

  return offset;
}

ret_t tk_istream_read_line_str(tk_istream_t* stream, str_t* str) {
  char line[64];
  int32_t size = 0;
  bool_t got_end_line = FALSE;
  return_value_if_fail(stream != NULL && str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->tell != NULL && stream->seek != NULL && stream->eos != NULL,
                       RET_BAD_PARAMS);

  str_set(str, "");
  if (tk_istream_eos(stream)) {
    return RET_DONE;
  }

  while (!tk_istream_eos(stream) && got_end_line == FALSE) {
    size = tk_istream_read(stream, line, sizeof(line) - 1);
    if (size >= 0) {
      int32_t i = 0;
      for (i = 0; i < size; i++) {
        if (line[i] == '\r' || line[i] == '\n') {
          got_end_line = TRUE;
          break;
        }
      }
      ENSURE(str_append_with_len(str, line, i) == RET_OK);

      /*not found end line*/
      if (i == size) {
        continue;
      }

      if (line[i] == '\r') {
        i++;
        if (line[i] == '\n') {
          i++;
        }
      } else if (line[i] == '\n') {
        i++;
      }

      if (i < size) {
        int32_t pos = tk_istream_tell(stream);
        pos = pos - (size - i);
        ENSURE(tk_istream_seek(stream, pos) == RET_OK);
      }
    } else {
      break;
    }
  }

  return RET_OK;
}

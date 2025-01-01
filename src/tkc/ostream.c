/**
 * File:   tk_ostream.h
 * Author: AWTK Develop Team
 * Brief:  output stream interface
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/utils.h"
#include "tkc/platform.h"
#include "tkc/time_now.h"
#include "tkc/ostream.h"

int32_t tk_ostream_write(tk_ostream_t* stream, const void* buff, uint32_t max_size) {
  return_value_if_fail(stream != NULL && stream->write != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  return stream->write(stream, (const uint8_t*)buff, max_size);
}

ret_t tk_ostream_seek(tk_ostream_t* stream, uint32_t offset) {
  return_value_if_fail(stream != NULL, RET_BAD_PARAMS);
  return_value_if_fail(stream->seek != NULL, RET_NOT_IMPL);

  return stream->seek(stream, offset);
}

int32_t tk_ostream_write_len(tk_ostream_t* stream, const void* buff, uint32_t max_size,
                             uint32_t timeout_ms) {
  uint32_t now = 0;
  uint32_t end = 0;
  int32_t offset = 0;
  int32_t write_bytes = 0;
  int32_t remain_bytes = max_size;
  const uint8_t* p = (const uint8_t*)buff;
  return_value_if_fail(stream != NULL && stream->write != NULL, -1);
  return_value_if_fail(buff != NULL, 0);

  now = time_now_ms();
  end = now + timeout_ms;
  do {
    errno = 0;
    write_bytes = tk_ostream_write(stream, p + offset, remain_bytes);

    if (write_bytes <= 0) {
      if (errno != EAGAIN) {
        break;
      } else {
        sleep_ms(20);
        log_debug("write: %d/%d\n", offset, max_size);
        log_debug("write:again, sleep 20ms\n");
        continue;
      }
    }

    offset += write_bytes;
    remain_bytes -= write_bytes;

    if (remain_bytes == 0) {
      break;
    }

    now = time_now_ms();
    if (now > end) {
      log_debug("write timeout\n");
      break;
    }
    log_debug("write: %d/%d\n", offset, max_size);
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

ret_t tk_ostream_write_byte(tk_ostream_t* stream, uint8_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_uint8(tk_ostream_t* stream, uint8_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_uint16(tk_ostream_t* stream, uint16_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_uint32(tk_ostream_t* stream, uint32_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_uint64(tk_ostream_t* stream, uint64_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_int8(tk_ostream_t* stream, int8_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_int16(tk_ostream_t* stream, int16_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_int32(tk_ostream_t* stream, int32_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_int64(tk_ostream_t* stream, int64_t value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_float(tk_ostream_t* stream, float value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

ret_t tk_ostream_write_double(tk_ostream_t* stream, double value) {
  int32_t ret = tk_ostream_write_len(stream, &value, sizeof(value), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(value) ? RET_OK : RET_FAIL;
}

int32_t tk_ostream_tell(tk_ostream_t* stream) {
  return_value_if_fail(stream != NULL, -1);
  return_value_if_fail(stream->tell != NULL, -1);

  return stream->tell(stream);
}

ret_t tk_ostream_write_str(tk_ostream_t* stream, const char* str) {
  int32_t len = tk_strlen(str);
  return tk_ostream_write(stream, str, len) == len ? RET_OK : RET_IO;
}

ret_t tk_ostream_printf(tk_ostream_t* stream, const char* format, ...) {
  va_list va;
  char buff[1024] = {0};

  va_start(va, format);
  tk_vsnprintf(buff, sizeof(buff) - 1, format, va);
  va_end(va);

  return tk_ostream_write_str(stream, buff);
}

ret_t tk_ostream_unref(tk_ostream_t* stream) {
  return tk_object_unref(TK_OBJECT(stream));
}

/**
 * File:   ubjson_reader.c
 * Author: AWTK Develop Team
 * Brief:  ubjson reader
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/endian.h"
#include "ubjson/ubjson_reader.h"

ubjson_reader_t* ubjson_reader_init(ubjson_reader_t* reader, ubjson_read_callback_t read,
                                    void* ctx) {
  return_value_if_fail(reader != NULL && read != NULL, NULL);

  reader->read = read;
  reader->ctx = ctx;
  str_init(&(reader->str), 64);

  return reader;
}

ret_t ubjson_reader_read_data(ubjson_reader_t* reader, void* data, uint32_t size) {
  return reader->read(reader->ctx, data, size);
}

ret_t ubjson_reader_reset(ubjson_reader_t* reader) {
  return_value_if_fail(reader != NULL && reader->read != NULL, RET_BAD_PARAMS);

  str_reset(&(reader->str));

  return RET_OK;
}

static bool_t is_binary(const char* str, uint32_t len) {
  uint32_t i = 0;
  for (i = 0; i < len; i++) {
    if (str[i] == '\0') {
      return TRUE;
    }
  }

  return FALSE;
}

ret_t ubjson_reader_read(ubjson_reader_t* reader, value_t* v) {
  ret_t ret = RET_OK;
  uint8_t marker = 0;
  return_value_if_fail(reader != NULL && reader->read != NULL && v != NULL, RET_BAD_PARAMS);
  ret = ubjson_reader_read_data(reader, &marker, 1);

  if (ret != RET_OK) {
    return RET_DONE;
  }

  switch (marker) {
    case UBJSON_MARKER_NULL: {
      value_set_pointer(v, NULL);
      break;
    }
    case UBJSON_MARKER_NOOP: {
      value_set_pointer(v, NULL);
      break;
    }
    case UBJSON_MARKER_TRUE: {
      value_set_bool(v, TRUE);
      break;
    }
    case UBJSON_MARKER_FALSE: {
      value_set_bool(v, FALSE);
      break;
    }
    case UBJSON_MARKER_INT8: {
      int8_t value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_int8(v, value);
      break;
    }
    case UBJSON_MARKER_UINT8: {
      uint8_t value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_uint8(v, value);
      break;
    }
    case UBJSON_MARKER_INT16: {
      int16_t value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_int16(v, int16_from_big_endian(value));
      break;
    }
    case UBJSON_MARKER_INT32: {
      int32_t value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_int32(v, int32_from_big_endian(value));
      break;
    }
    case UBJSON_MARKER_INT64: {
      int64_t value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_int64(v, int64_from_big_endian(value));
      break;
    }
    case UBJSON_MARKER_FLOAT32: {
      float value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_float32(v, float_from_big_endian(value));
      break;
    }
    case UBJSON_MARKER_FLOAT64: {
      double value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_double(v, double_from_big_endian(value));
      break;
    }
    case UBJSON_MARKER_CHAR: {
      char value = 0;
      return_value_if_fail(ubjson_reader_read_data(reader, &value, sizeof(value)) == RET_OK,
                           RET_FAIL);
      value_set_int8(v, value);
      break;
    }
    case UBJSON_MARKER_STRING: {
      int len = 0;
      value_t vlen;
      str_t* str = &(reader->str);

      value_set_int(&vlen, 0);
      return_value_if_fail(ubjson_reader_read(reader, &vlen) == RET_OK, RET_FAIL);

      len = value_int(&vlen);
      return_value_if_fail(str_extend(str, len + 1) == RET_OK, RET_OOM);
      return_value_if_fail(ubjson_reader_read_data(reader, str->str, len) == RET_OK, RET_FAIL);

      if (is_binary(str->str, len)) {
        str->str[len] = '\0';
        value_set_binary_data(v, str->str, len);
      } else {
        str->str[len] = '\0';
        value_set_str(v, str->str);
      }

      break;
    }
    case UBJSON_MARKER_ARRAY_BEGIN: {
      value_set_token(v, UBJSON_MARKER_ARRAY_BEGIN);
      break;
    }
    case UBJSON_MARKER_ARRAY_END: {
      value_set_token(v, UBJSON_MARKER_ARRAY_END);
      break;
    }
    case UBJSON_MARKER_OBJECT_BEGIN: {
      value_set_token(v, UBJSON_MARKER_OBJECT_BEGIN);
      break;
    }
    case UBJSON_MARKER_OBJECT_END: {
      value_set_token(v, UBJSON_MARKER_OBJECT_END);
      break;
    }
    default: {
      ret = RET_FAIL;
      break;
    }
  }

  return ret;
}

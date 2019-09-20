/**
 * File:   ubjson_writer.c
 * Author: AWTK Develop Team
 * Brief:  ubjson writer
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/endian.h"
#include "ubjson/ubjson_writer.h"

ubjson_writer_t* ubjson_writer_init(ubjson_writer_t* writer, ubjson_write_callback_t write,
                                    void* ctx) {
  return_value_if_fail(writer != NULL && write != NULL, NULL);

  writer->write = write;
  writer->ctx = ctx;

  return writer;
}

static ret_t ubjson_writer_write_data(ubjson_writer_t* writer, const void* data, uint32_t size) {
  return writer->write(writer->ctx, data, size);
}

static ret_t ubjson_writer_write_marker(ubjson_writer_t* writer, uint8_t marker) {
  return ubjson_writer_write_data(writer, &marker, 1);
}

ret_t ubjson_writer_write_null(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_NULL) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_noop(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_NOOP) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_true(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_TRUE) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_false(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_FALSE) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_int8(ubjson_writer_t* writer, int8_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_INT8) == RET_OK, RET_OOM);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_uint8(ubjson_writer_t* writer, uint8_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_UINT8) == RET_OK, RET_OOM);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_int16(ubjson_writer_t* writer, int16_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_INT16) == RET_OK, RET_OOM);
  value = int16_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_int32(ubjson_writer_t* writer, int32_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_INT32) == RET_OK, RET_OOM);
  value = int32_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_int64(ubjson_writer_t* writer, int64_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_INT64) == RET_OK, RET_OOM);
  value = int64_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_float32(ubjson_writer_t* writer, float value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_FLOAT32) == RET_OK,
                       RET_OOM);
  value = float_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_float64(ubjson_writer_t* writer, double value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_FLOAT64) == RET_OK,
                       RET_OOM);
  value = double_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_char(ubjson_writer_t* writer, char value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_CHAR) == RET_OK, RET_OOM);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_key(ubjson_writer_t* writer, const char* value) {
  uint32_t len = 0;
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);

  len = strlen(value);
  if (len < 128) {
    ubjson_writer_write_int8(writer, (int8_t)len);
  } else if (len < 0xefff) {
    ubjson_writer_write_int16(writer, (int16_t)len);
  } else if (len < 0xefffffff) {
    ubjson_writer_write_int32(writer, (int32_t)len);
  } else {
    return RET_BAD_PARAMS;
  }

  return_value_if_fail(ubjson_writer_write_data(writer, value, len) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_kv_bool(ubjson_writer_t* writer, const char* key, bool_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  if(value) {
    return ubjson_writer_write_true(writer);
  } else {
    return ubjson_writer_write_false(writer);
  }
}

ret_t ubjson_writer_write_kv_int(ubjson_writer_t* writer, const char* key, int32_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int32(writer, value);
}

ret_t ubjson_writer_write_kv_str(ubjson_writer_t* writer, const char* key, const char* value) {
  return_value_if_fail(writer != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_str(writer, value);
}

ret_t ubjson_writer_write_str(ubjson_writer_t* writer, const char* value) {
  uint32_t len = 0;
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_STRING) == RET_OK, RET_OOM);

  return ubjson_writer_write_key(writer, value);
}

ret_t ubjson_writer_write_array_begin(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_ARRAY_BEGIN) == RET_OK,
                       RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_array_end(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_ARRAY_END) == RET_OK,
                       RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_object_begin(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_OBJECT_BEGIN) == RET_OK,
                       RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_object_end(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_OBJECT_END) == RET_OK,
                       RET_OOM);

  return RET_OK;
}

/**
 * File:   ubjson_writer.c
 * Author: AWTK Develop Team
 * Brief:  ubjson writer
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

#include "tkc/value.h"
#include "tkc/object.h"
#include "tkc/endian.h"
#include "tkc/named_value.h"
#include "ubjson/ubjson_writer.h"

ubjson_writer_t* ubjson_writer_init(ubjson_writer_t* writer, ubjson_write_callback_t write,
                                    void* ctx) {
  return_value_if_fail(writer != NULL && write != NULL, NULL);

  writer->write = write;
  writer->ctx = ctx;

  return writer;
}

ret_t ubjson_writer_write_data(ubjson_writer_t* writer, const void* data, uint32_t size) {
  return writer->write(writer->ctx, data, size);
}

static ret_t ubjson_writer_write_marker(ubjson_writer_t* writer, uint8_t marker) {
  return ubjson_writer_write_data(writer, &marker, 1);
}

static ret_t ubjson_writer_write_key_len(ubjson_writer_t* writer, const char* value, uint32_t len) {
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);

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

ret_t ubjson_writer_write_key(ubjson_writer_t* writer, const char* value) {
  value = value != NULL ? value : "";

  return ubjson_writer_write_key_len(writer, value, strlen(value));
}

ret_t ubjson_writer_write_str_len(ubjson_writer_t* writer, const char* value, uint32_t len) {
  value = value != NULL ? value : "";
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_STRING) == RET_OK, RET_OOM);

  return ubjson_writer_write_key_len(writer, value, len);
}

ret_t ubjson_writer_write_str(ubjson_writer_t* writer, const char* value) {
  value = value != NULL ? value : "";

  return ubjson_writer_write_str_len(writer, value, strlen(value));
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

ret_t ubjson_writer_write_int(ubjson_writer_t* writer, int32_t value) {
  if (value < 128) {
    return ubjson_writer_write_int8(writer, (int8_t)value);
  } else if (value < 30000) {
    return ubjson_writer_write_int16(writer, (int16_t)value);
  } else {
    return ubjson_writer_write_int32(writer, value);
  }
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

ret_t ubjson_writer_write_kv_bool(ubjson_writer_t* writer, const char* key, bool_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  if (value) {
    return ubjson_writer_write_true(writer);
  } else {
    return ubjson_writer_write_false(writer);
  }
}

ret_t ubjson_writer_write_kv_int(ubjson_writer_t* writer, const char* key, int32_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int(writer, value);
}

ret_t ubjson_writer_write_kv_float(ubjson_writer_t* writer, const char* key, float value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_float32(writer, value);
}

ret_t ubjson_writer_write_kv_double(ubjson_writer_t* writer, const char* key, double value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_float64(writer, value);
}

ret_t ubjson_writer_write_kv_int64(ubjson_writer_t* writer, const char* key, int64_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int64(writer, value);
}

ret_t ubjson_writer_write_kv_object_begin(ubjson_writer_t* writer, const char* key) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_object_begin(writer);
}

ret_t ubjson_writer_write_kv_str(ubjson_writer_t* writer, const char* key, const char* value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_str(writer, value);
}

ret_t ubjson_writer_write_kv_str_len(ubjson_writer_t* writer, const char* key, const char* value,
                                     uint32_t len) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_str_len(writer, value, len);
}

static ret_t on_prop_write_ubjson(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  ubjson_writer_t* writer = (ubjson_writer_t*)ctx;

  return ubjson_writer_write_kv_value(writer, nv->name, &(nv->value));
}

ret_t ubjson_writer_write_object(ubjson_writer_t* writer, object_t* obj) {
  return_value_if_fail(ubjson_writer_write_object_begin(writer) == RET_OK, RET_OOM);
  return_value_if_fail(object_foreach_prop(obj, on_prop_write_ubjson, writer) == RET_OK, RET_OOM);

  return ubjson_writer_write_object_end(writer);
}

ret_t ubjson_writer_write_kv_object(ubjson_writer_t* writer, const char* key, object_t* value) {
  return_value_if_fail(writer != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_object(writer, value);
}

ret_t ubjson_writer_write_kv_value(ubjson_writer_t* writer, const char* key, value_t* value) {
  return_value_if_fail(writer != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  if (value->type == VALUE_TYPE_BOOL) {
    if (value_bool(value)) {
      return ubjson_writer_write_true(writer);
    } else {
      return ubjson_writer_write_false(writer);
    }
  } else if (value->type == VALUE_TYPE_STRING) {
    return ubjson_writer_write_str(writer, value_str(value));
  } else if (value->type == VALUE_TYPE_BINARY) {
    binary_data_t* data = value_binary_data(value);
    return ubjson_writer_write_str_len(writer, data->data, data->size);
  } else if (value->type == VALUE_TYPE_OBJECT) {
    return ubjson_writer_write_object(writer, value_object(value));
  } else {
    return ubjson_writer_write_int(writer, value_int(value));
  }
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

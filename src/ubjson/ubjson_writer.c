/**
 * File:   ubjson_writer.c
 * Author: AWTK Develop Team
 * Brief:  ubjson writer
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
#include "tkc/mem.h"
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

static ret_t ubjson_writer_write_length(ubjson_writer_t* writer, uint32_t len) {
  ret_t ret = RET_BAD_PARAMS;
  if (len <= INT8_MAX) {
    ret = ubjson_writer_write_int8(writer, (int8_t)len);
  } else if (len <= INT16_MAX) {
    ret = ubjson_writer_write_int16(writer, (int16_t)len);
  } else if (len <= INT_MAX) {
    ret = ubjson_writer_write_int32(writer, (int32_t)len);
  }

  return ret;
}

static ret_t ubjson_writer_write_key_len(ubjson_writer_t* writer, const char* value, uint32_t len) {
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);

  return_value_if_fail(ubjson_writer_write_length(writer, len) == RET_OK, RET_OOM);

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

ret_t ubjson_writer_write_wstr_len(ubjson_writer_t* writer, const wchar_t* value, uint32_t len) {
  str_t str;
  ret_t ret = RET_OK;
  value = value != NULL ? value : L"";
  return_value_if_fail(writer != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_WSTRING) == RET_OK,
                       RET_OOM);

  str_init(&str, 0);
  str_from_wstr_with_len(&str, value, len);
  ret = ubjson_writer_write_key_len(writer, str.str, str.size);
  str_reset(&str);

  return ret;
}

ret_t ubjson_writer_write_wstr(ubjson_writer_t* writer, const wchar_t* value) {
  value = value != NULL ? value : L"";

  return ubjson_writer_write_wstr_len(writer, value, wcslen(value));
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

ret_t ubjson_writer_write_uint16(ubjson_writer_t* writer, uint16_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_UINT16) == RET_OK, RET_OOM);
  value = uint16_to_big_endian(value);
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

ret_t ubjson_writer_write_uint32(ubjson_writer_t* writer, uint32_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_UINT32) == RET_OK, RET_OOM);
  value = uint32_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_int(ubjson_writer_t* writer, int32_t value) {
  if (INT8_MIN <= value && value <= INT8_MAX) {
    return ubjson_writer_write_int8(writer, (int8_t)value);
  } else if (0 <= value && value <= UINT8_MAX) {
    return ubjson_writer_write_uint8(writer, (uint8_t)value);
  } else if (INT16_MIN <= value && value <= INT16_MAX) {
    return ubjson_writer_write_int16(writer, (int16_t)value);
  } else if (0 <= value && value <= UINT16_MAX) {
    return ubjson_writer_write_uint16(writer, (uint16_t)value);
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

ret_t ubjson_writer_write_uint64(ubjson_writer_t* writer, uint64_t value) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_UINT64) == RET_OK, RET_OOM);
  value = uint64_to_big_endian(value);
  return_value_if_fail(ubjson_writer_write_data(writer, &value, sizeof(value)) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_pointer(ubjson_writer_t* writer, void* value) {
  uint64_t adrr;
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_POINTER) == RET_OK,
                       RET_OOM);
  adrr = uint64_to_big_endian(tk_pointer_to_long(value));
  return_value_if_fail(ubjson_writer_write_data(writer, &adrr, sizeof(adrr)) == RET_OK, RET_OOM);

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

ret_t ubjson_writer_write_kv_int8(ubjson_writer_t* writer, const char* key, int8_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int8(writer, value);
}

ret_t ubjson_writer_write_kv_int16(ubjson_writer_t* writer, const char* key, int16_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int16(writer, value);
}

ret_t ubjson_writer_write_kv_int32(ubjson_writer_t* writer, const char* key, int32_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_int32(writer, value);
}

ret_t ubjson_writer_write_kv_uint8(ubjson_writer_t* writer, const char* key, uint8_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_uint8(writer, value);
}

ret_t ubjson_writer_write_kv_uint16(ubjson_writer_t* writer, const char* key, uint16_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_uint16(writer, value);
}

ret_t ubjson_writer_write_kv_uint32(ubjson_writer_t* writer, const char* key, uint32_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_uint32(writer, value);
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

ret_t ubjson_writer_write_kv_uint64(ubjson_writer_t* writer, const char* key, uint64_t value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_uint64(writer, value);
}

ret_t ubjson_writer_write_kv_pointer(ubjson_writer_t* writer, const char* key, void* value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_pointer(writer, value);
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

ret_t ubjson_writer_write_kv_wstr(ubjson_writer_t* writer, const char* key, const wchar_t* value) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_wstr(writer, value);
}

ret_t ubjson_writer_write_kv_wstr_len(ubjson_writer_t* writer, const char* key,
                                      const wchar_t* value, uint32_t len) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_wstr_len(writer, value, len);
}

static ret_t on_prop_write_ubjson(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  ubjson_writer_t* writer = (ubjson_writer_t*)ctx;

  return ubjson_writer_write_kv_value(writer, nv->name, &(nv->value));
}

ret_t ubjson_writer_write_object(ubjson_writer_t* writer, tk_object_t* obj) {
  return_value_if_fail(ubjson_writer_write_object_begin(writer) == RET_OK, RET_OOM);
  return_value_if_fail(tk_object_foreach_prop(obj, on_prop_write_ubjson, writer) != RET_OOM,
                       RET_OOM);

  return ubjson_writer_write_object_end(writer);
}

ret_t ubjson_writer_write_kv_object(ubjson_writer_t* writer, const char* key, tk_object_t* value) {
  return_value_if_fail(writer != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_object(writer, value);
}

ret_t ubjson_writer_write_kv_value(ubjson_writer_t* writer, const char* key, const value_t* value) {
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
  } else if (value->type == VALUE_TYPE_WSTRING) {
    return ubjson_writer_write_wstr(writer, value_wstr(value));
  } else if (value->type == VALUE_TYPE_BINARY) {
    binary_data_t* data = value_binary_data(value);
    return ubjson_writer_write_str_len(writer, data->data, data->size);
  } else if (value->type == VALUE_TYPE_OBJECT) {
    return ubjson_writer_write_object(writer, value_object(value));
  } else if (value->type == VALUE_TYPE_FLOAT || value->type == VALUE_TYPE_DOUBLE) {
    return ubjson_writer_write_float64(writer, value_double(value));
  } else if (value->type == VALUE_TYPE_FLOAT32) {
    return ubjson_writer_write_float32(writer, value_float32(value));
  } else if (value->type == VALUE_TYPE_INT8) {
    return ubjson_writer_write_int8(writer, value_int8(value));
  } else if (value->type == VALUE_TYPE_UINT8) {
    return ubjson_writer_write_uint8(writer, value_uint8(value));
  } else if (value->type == VALUE_TYPE_INT16) {
    return ubjson_writer_write_int16(writer, value_int16(value));
  } else if (value->type == VALUE_TYPE_UINT16) {
    return ubjson_writer_write_uint16(writer, value_uint16(value));
  } else if (value->type == VALUE_TYPE_INT32) {
    return ubjson_writer_write_int32(writer, value_int32(value));
  } else if (value->type == VALUE_TYPE_UINT32) {
    return ubjson_writer_write_uint32(writer, value_uint32(value));
  } else if (value->type == VALUE_TYPE_INT64) {
    return ubjson_writer_write_int64(writer, value_int64(value));
  } else if (value->type == VALUE_TYPE_UINT64) {
    return ubjson_writer_write_uint64(writer, value_uint64(value));
  } else if (value->type == VALUE_TYPE_POINTER) {
    return ubjson_writer_write_pointer(writer, value_pointer(value));
  } else {
    char str[64];
    log_debug("not supported type\n");
    return ubjson_writer_write_str(writer, value_str_ex(value, str, sizeof(str) - 1));
  }
}

ret_t ubjson_writer_write_array_begin(ubjson_writer_t* writer) {
  return_value_if_fail(writer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_ARRAY_BEGIN) == RET_OK,
                       RET_OOM);

  return RET_OK;
}

ret_t ubjson_writer_write_kv_array_begin(ubjson_writer_t* writer, const char* key) {
  return_value_if_fail(writer != NULL && key != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ubjson_writer_write_key(writer, key) == RET_OK, RET_OOM);

  return ubjson_writer_write_array_begin(writer);
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

static ret_t ubjson_writer_write_optimized_array(ubjson_writer_t* writer, char type, uint32_t count,
                                                 void* array) {
  ret_t ret;
  return_value_if_fail(writer != NULL && array != NULL && count > 0, RET_BAD_PARAMS);

  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_ARRAY_BEGIN) == RET_OK,
                       RET_OOM);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_CONTAINER_TYPE) == RET_OK,
                       RET_OOM);
  return_value_if_fail(ubjson_writer_write_marker(writer, type) == RET_OK, RET_OOM);
  return_value_if_fail(ubjson_writer_write_marker(writer, UBJSON_MARKER_CONTAINER_COUNT) == RET_OK,
                       RET_OOM);

  return_value_if_fail(ubjson_writer_write_length(writer, count) == RET_OK, RET_OOM);

  if (type == UBJSON_MARKER_INT8 || type == UBJSON_MARKER_UINT8) {
    ret = ubjson_writer_write_data(writer, array, count);
    return_value_if_fail(ret == RET_OK, RET_OOM);

  } else if (type == UBJSON_MARKER_INT16) {
    int16_t* p = (int16_t*)array;

    for (uint32_t i = 0; i < count; i++) {
      int16_t val = p[i];
      val = int16_to_big_endian(val);
      ret = ubjson_writer_write_data(writer, &val, sizeof(val));
      return_value_if_fail(ret == RET_OK, RET_OOM);
    }

  } else if (type == UBJSON_MARKER_INT32) {
    int32_t* p = (int32_t*)array;

    for (uint32_t i = 0; i < count; i++) {
      int32_t val = p[i];
      val = int32_to_big_endian(val);
      ret = ubjson_writer_write_data(writer, &val, sizeof(val));
      return_value_if_fail(ret == RET_OK, RET_OOM);
    }

  } else if (type == UBJSON_MARKER_INT64) {
    int64_t* p = (int64_t*)array;

    for (uint32_t i = 0; i < count; i++) {
      int64_t val = p[i];
      val = int64_to_big_endian(val);
      ret = ubjson_writer_write_data(writer, &val, sizeof(val));
      return_value_if_fail(ret == RET_OK, RET_OOM);
    }

  } else if (type == UBJSON_MARKER_FLOAT32) {
    float* p = (float*)array;

    for (uint32_t i = 0; i < count; i++) {
      float val = p[i];
      val = float_to_big_endian(val);
      ret = ubjson_writer_write_data(writer, &val, sizeof(val));
      return_value_if_fail(ret == RET_OK, RET_OOM);
    }
  } else if (type == UBJSON_MARKER_FLOAT64) {
    double* p = (double*)array;

    for (uint32_t i = 0; i < count; i++) {
      double val = p[i];
      val = double_to_big_endian(val);
      ret = ubjson_writer_write_data(writer, &val, sizeof(val));
      return_value_if_fail(ret == RET_OK, RET_OOM);
    }
  }

  /* 优化数组，没有结束标记 */

  return RET_OK;
}

ret_t ubjson_writer_write_array_uint8(ubjson_writer_t* writer, uint8_t* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_UINT8, count, data);
}

ret_t ubjson_writer_write_array_int8(ubjson_writer_t* writer, int8_t* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_INT8, count, data);
}

ret_t ubjson_writer_write_array_int16(ubjson_writer_t* writer, int16_t* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_INT16, count, data);
}

ret_t ubjson_writer_write_array_int32(ubjson_writer_t* writer, int32_t* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_INT32, count, data);
}

ret_t ubjson_writer_write_array_int64(ubjson_writer_t* writer, int64_t* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_INT64, count, data);
}
ret_t ubjson_writer_write_array_float32(ubjson_writer_t* writer, float* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_FLOAT32, count, data);
}

ret_t ubjson_writer_write_array_float64(ubjson_writer_t* writer, double* data, uint32_t count) {
  return ubjson_writer_write_optimized_array(writer, UBJSON_MARKER_FLOAT64, count, data);
}
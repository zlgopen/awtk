﻿/**
 * File:   buffer.c
 * Author: AWTK Develop Team
 * Brief:  buffer
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/buffer.h"

wbuffer_t* wbuffer_create(void* data, uint32_t capacity) {
  wbuffer_t* buffer = NULL;
  return_value_if_fail(data != NULL && capacity > 0, NULL);
  buffer = TKMEM_ZALLOC(wbuffer_t);

  return wbuffer_init(buffer, data, capacity);
}

wbuffer_t* wbuffer_create_extendable(void) {
  wbuffer_t* buffer = TKMEM_ZALLOC(wbuffer_t);

  return wbuffer_init_extendable(buffer);
}

ret_t wbuffer_destroy(wbuffer_t* wbuffer) {
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);
  wbuffer_deinit(wbuffer);
  TKMEM_FREE(wbuffer);

  return RET_OK;
}

rbuffer_t* rbuffer_create(const void* data, uint32_t capacity) {
  rbuffer_t* buffer = NULL;
  return_value_if_fail(data != NULL && capacity > 0, NULL);
  buffer = TKMEM_ZALLOC(rbuffer_t);

  return rbuffer_init(buffer, data, capacity);
}

ret_t rbuffer_deinit(rbuffer_t* rbuffer) {
  return RET_OK;
}

ret_t rbuffer_destroy(rbuffer_t* rbuffer) {
  return_value_if_fail(rbuffer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rbuffer);

  return RET_OK;
}

wbuffer_t* wbuffer_init(wbuffer_t* wbuffer, void* data, uint32_t capacity) {
  return_value_if_fail(wbuffer != NULL && data != NULL, NULL);

  wbuffer->data = (uint8_t*)data;
  wbuffer->cursor = 0;
  wbuffer->extendable = FALSE;
  wbuffer->capacity = capacity;

  return wbuffer;
}

ret_t wbuffer_rewind(wbuffer_t* wbuffer) {
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);

  wbuffer->cursor = 0;
  return RET_OK;
}

ret_t wbuffer_extend_capacity(wbuffer_t* wbuffer, uint32_t capacity) {
  uint8_t* data = NULL;
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);

  if (capacity <= wbuffer->capacity) {
    return RET_OK;
  }

  return_value_if_fail(wbuffer->extendable, RET_FAIL);

  capacity = tk_max(capacity, wbuffer->capacity * 1.5);
  data = (uint8_t*)TKMEM_REALLOC(wbuffer->data, capacity);
  if (data != NULL) {
    wbuffer->data = data;
    wbuffer->capacity = capacity;
    memset(data + wbuffer->cursor, 0x00, capacity - wbuffer->cursor);

    return RET_OK;
  }

  return RET_OOM;
}

static ret_t wbuffer_extend_delta(wbuffer_t* wbuffer, uint32_t delta) {
  uint32_t capacity = 0;
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);

  capacity = wbuffer->cursor + delta + 1;

  return wbuffer_extend_capacity(wbuffer, capacity);
}

ret_t wbuffer_deinit(wbuffer_t* wbuffer) {
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);

  if (wbuffer->extendable && wbuffer->data != NULL) {
    TKMEM_FREE(wbuffer->data);
  }
  memset(wbuffer, 0x00, sizeof(wbuffer_t));

  return RET_OK;
}

wbuffer_t* wbuffer_init_extendable(wbuffer_t* wbuffer) {
  return_value_if_fail(wbuffer != NULL, NULL);

  wbuffer->data = 0;
  wbuffer->cursor = 0;
  wbuffer->capacity = 0;
  wbuffer->extendable = TRUE;

  return wbuffer;
}

wbuffer_t* wbuffer_clone(const wbuffer_t* wbuffer) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(wbuffer != NULL, NULL);
  wb = wbuffer_create_extendable();
  return_value_if_fail(wb!= NULL, NULL);
  memcpy(wb, wbuffer, sizeof(wbuffer_t));
  wb->data = (uint8_t*)TKMEM_ALLOC(wbuffer->capacity);
  goto_error_if_fail(wb->data!= NULL);

  memcpy(wb->data, wbuffer->data, wbuffer->capacity);
  return wb;
error:
  wbuffer_destroy(wb);
  return NULL;
}

ret_t wbuffer_skip(wbuffer_t* wbuffer, int32_t delta) {
  int32_t cursor = 0;
  return_value_if_fail(wbuffer != NULL, RET_BAD_PARAMS);
  return_value_if_fail((wbuffer->extendable || wbuffer->data != NULL), RET_BAD_PARAMS);

  cursor = wbuffer->cursor + delta;
  return_value_if_fail(cursor >= 0, RET_BAD_PARAMS);
  return_value_if_fail(wbuffer_extend_capacity(wbuffer, cursor) == RET_OK, RET_BAD_PARAMS);

  wbuffer->cursor = cursor;

  return RET_OK;
}

ret_t wbuffer_write_uint8(wbuffer_t* wbuffer, uint8_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  wbuffer->data[wbuffer->cursor++] = value;

  return RET_OK;
}

ret_t wbuffer_write_uint16(wbuffer_t* wbuffer, uint16_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_uint32(wbuffer_t* wbuffer, uint32_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_uint64(wbuffer_t* wbuffer, uint64_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_int8(wbuffer_t* wbuffer, int8_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  wbuffer->data[wbuffer->cursor++] = value;

  return RET_OK;
}

ret_t wbuffer_write_int16(wbuffer_t* wbuffer, int16_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_int32(wbuffer_t* wbuffer, int32_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_int64(wbuffer_t* wbuffer, int64_t value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_float(wbuffer_t* wbuffer, float value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_double(wbuffer_t* wbuffer, double value) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, sizeof(value)) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, &value, sizeof(value));
  wbuffer->cursor += sizeof(value);

  return RET_OK;
}

ret_t wbuffer_write_binary(wbuffer_t* wbuffer, const void* data, uint32_t size) {
  return_value_if_fail(wbuffer_extend_delta(wbuffer, size) == RET_OK, RET_BAD_PARAMS);

  memcpy(wbuffer->data + wbuffer->cursor, data, size);
  wbuffer->cursor += size;

  return RET_OK;
}

ret_t wbuffer_modify_binary(wbuffer_t* wb, uint32_t offset, const void* data, uint32_t dsize) {
  return_value_if_fail(wb && offset + dsize <= wb->cursor, RET_BAD_PARAMS);
  memcpy(wb->data + offset, data, dsize);
  return RET_OK;
}

ret_t wbuffer_insert_binary(wbuffer_t* wb, uint32_t offset, const void* data, uint32_t dsize) {
  return_value_if_fail(wbuffer_extend_delta(wb, dsize) == RET_OK, RET_BAD_PARAMS);

  memmove(wb->data + offset + dsize, wb->data + offset, wb->cursor - offset);
  memcpy(wb->data + offset, data, dsize);
  wb->cursor += dsize;
  return RET_OK;
}

ret_t wbuffer_write_value(wbuffer_t* wbuffer, const value_t* v) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(wbuffer != NULL && v != NULL, RET_BAD_PARAMS);

  if (VALUE_TYPE_WSTRING == v->type) {
    ret = wbuffer_write_uint8(wbuffer, VALUE_TYPE_STRING);
  } else {
    ret = wbuffer_write_uint8(wbuffer, v->type);
  }
  return_value_if_fail(ret == RET_OK, ret);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      ret = wbuffer_write_uint8(wbuffer, value_bool(v));
      break;
    }
    case VALUE_TYPE_UINT8: {
      ret = wbuffer_write_uint8(wbuffer, value_uint8(v));
      break;
    }
    case VALUE_TYPE_INT8: {
      ret = wbuffer_write_int8(wbuffer, value_int8(v));
      break;
    }
    case VALUE_TYPE_UINT16: {
      ret = wbuffer_write_uint16(wbuffer, value_uint16(v));
      break;
    }
    case VALUE_TYPE_INT16: {
      ret = wbuffer_write_int16(wbuffer, value_int16(v));
      break;
    }
    case VALUE_TYPE_UINT32: {
      ret = wbuffer_write_uint32(wbuffer, value_uint32(v));
      break;
    }
    case VALUE_TYPE_INT32: {
      ret = wbuffer_write_int32(wbuffer, value_int32(v));
      break;
    }
    case VALUE_TYPE_UINT64: {
      ret = wbuffer_write_uint64(wbuffer, value_uint16(v));
      break;
    }
    case VALUE_TYPE_INT64: {
      ret = wbuffer_write_int64(wbuffer, value_int64(v));
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      ret = wbuffer_write_float(wbuffer, value_float32(v));
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      ret = wbuffer_write_double(wbuffer, value_double(v));
      break;
    }
    case VALUE_TYPE_STRING: {
      ret = wbuffer_write_string(wbuffer, value_str(v));
      break;
    }
    case VALUE_TYPE_WSTRING: {
      char* str = tk_utf8_dup_wstr(value_wstr(v));
      ret = wbuffer_write_string(wbuffer, str);
      TKMEM_FREE(str);
      break;
    }
    default: {
      wbuffer_skip(wbuffer, -1);
      ret = RET_NOT_IMPL;
    }
  }

  return ret;
}

bool_t wbuffer_has_room(wbuffer_t* wbuffer, uint32_t size) {
  return_value_if_fail(wbuffer != NULL && wbuffer->data != NULL, FALSE);

  return (wbuffer->cursor + size) <= wbuffer->capacity;
}

ret_t wbuffer_write_string(wbuffer_t* wbuffer, const char* data) {
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  return wbuffer_write_binary(wbuffer, data, strlen(data) + 1);
}

rbuffer_t* rbuffer_init(rbuffer_t* rbuffer, const void* data, uint32_t capacity) {
  return_value_if_fail(rbuffer != NULL && data != NULL, NULL);

  rbuffer->data = (const uint8_t*)data;
  rbuffer->cursor = 0;
  rbuffer->capacity = capacity;

  return rbuffer;
}

bool_t rbuffer_has_more(rbuffer_t* rbuffer) {
  return_value_if_fail(rbuffer != NULL, FALSE);

  return rbuffer->cursor < rbuffer->capacity;
}

ret_t rbuffer_rewind(rbuffer_t* rbuffer) {
  return_value_if_fail(rbuffer != NULL, RET_BAD_PARAMS);

  rbuffer->cursor = 0;
  return RET_OK;
}

ret_t rbuffer_skip(rbuffer_t* rbuffer, int32_t offset) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(((int32_t)(rbuffer->cursor) + offset) >= 0, RET_BAD_PARAMS);
  return_value_if_fail(((int32_t)(rbuffer->cursor) + offset) <= rbuffer->capacity, RET_BAD_PARAMS);

  rbuffer->cursor += offset;

  return RET_OK;
}

ret_t rbuffer_read_uint8(rbuffer_t* rbuffer, uint8_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  *value = rbuffer->data[rbuffer->cursor++];

  return RET_OK;
}

ret_t rbuffer_read_uint16(rbuffer_t* rbuffer, uint16_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_uint32(rbuffer_t* rbuffer, uint32_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_uint64(rbuffer_t* rbuffer, uint64_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_int8(rbuffer_t* rbuffer, int8_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  *value = rbuffer->data[rbuffer->cursor++];

  return RET_OK;
}

ret_t rbuffer_read_int16(rbuffer_t* rbuffer, int16_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_int32(rbuffer_t* rbuffer, int32_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_int64(rbuffer_t* rbuffer, int64_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_float(rbuffer_t* rbuffer, float* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_double(rbuffer_t* rbuffer, double* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));
  rbuffer->cursor += sizeof(*value);

  return RET_OK;
}

ret_t rbuffer_read_binary(rbuffer_t* rbuffer, void* data, uint32_t size) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && data != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + size) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(data, rbuffer->data + rbuffer->cursor, size);
  rbuffer->cursor += size;

  return RET_OK;
}

ret_t rbuffer_read_value(rbuffer_t* rbuffer, value_t* v) {
  ret_t ret = RET_FAIL;
  uint8_t type = 0;
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && v != NULL, RET_BAD_PARAMS);
  ret = rbuffer_read_uint8(rbuffer, &type);
  return_value_if_fail(ret == RET_OK, ret);

  switch (type) {
    case VALUE_TYPE_BOOL: {
      uint8_t value = 0;
      ret = rbuffer_read_uint8(rbuffer, &value);
      value_set_bool(v, value);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t value = 0;
      ret = rbuffer_read_uint8(rbuffer, &value);
      value_set_uint8(v, value);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t value = 0;
      ret = rbuffer_read_int8(rbuffer, &value);
      value_set_int8(v, value);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t value = 0;
      ret = rbuffer_read_uint16(rbuffer, &value);
      value_set_uint16(v, value);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t value = 0;
      ret = rbuffer_read_int16(rbuffer, &value);
      value_set_int16(v, value);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t value = 0;
      ret = rbuffer_read_uint32(rbuffer, &value);
      value_set_uint32(v, value);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t value = 0;
      ret = rbuffer_read_int32(rbuffer, &value);
      value_set_int32(v, value);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t value = 0;
      ret = rbuffer_read_uint64(rbuffer, &value);
      value_set_uint64(v, value);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t value = 0;
      ret = rbuffer_read_int64(rbuffer, &value);
      value_set_int64(v, value);
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      float value = 0;
      ret = rbuffer_read_float(rbuffer, &value);
      value_set_float32(v, value);
      break;
    }
    case VALUE_TYPE_FLOAT: {
      double value = 0;
      ret = rbuffer_read_double(rbuffer, &value);
      value_set_float(v, value);
      break;
    }
    case VALUE_TYPE_DOUBLE: {
      double value = 0;
      ret = rbuffer_read_double(rbuffer, &value);
      value_set_double(v, value);
      break;
    }
    case VALUE_TYPE_STRING: {
      const char* value = NULL;
      ret = rbuffer_read_string(rbuffer, &value);
      value_set_str(v, value);
      break;
    }
    default: {
      rbuffer_skip(rbuffer, -1);
      ret = RET_NOT_IMPL;
    }
  }

  return ret;
}
ret_t rbuffer_read_string(rbuffer_t* rbuffer, const char** str) {
  uint32_t len = 0;
  uint32_t max_count = 0;
  const char* start = NULL;
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && str != NULL, RET_BAD_PARAMS);

  start = (const char*)rbuffer->data + rbuffer->cursor;
  max_count = rbuffer->capacity - rbuffer->cursor;
  len = tk_strnlen(start, max_count);
  if (rbuffer->cursor + len == rbuffer->capacity) {
    *str = NULL;
    return RET_FAIL;
  }

  *str = (const char*)start;
  rbuffer->cursor += len + 1;

  return RET_OK;
}

ret_t rbuffer_peek_uint8(rbuffer_t* rbuffer, uint8_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  *value = rbuffer->data[rbuffer->cursor];

  return RET_OK;
}

ret_t rbuffer_peek_uint16(rbuffer_t* rbuffer, uint16_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));

  return RET_OK;
}

ret_t rbuffer_peek_uint32(rbuffer_t* rbuffer, uint32_t* value) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail((rbuffer->cursor + sizeof(*value)) <= rbuffer->capacity, RET_BAD_PARAMS);

  memcpy(value, rbuffer->data + rbuffer->cursor, sizeof(*value));

  return RET_OK;
}

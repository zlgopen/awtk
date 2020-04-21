/**
 * File:   buffer.c
 * Author: AWTK Develop Team
 * Brief:  buffer
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/buffer.h"

wbuffer_t* wbuffer_init(wbuffer_t* wbuffer, uint8_t* data, uint32_t capacity) {
  return_value_if_fail(wbuffer != NULL && data != NULL, NULL);

  wbuffer->data = data;
  wbuffer->cursor = 0;
  wbuffer->extendable = FALSE;
  wbuffer->capacity = capacity;

  return wbuffer;
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

  if (wbuffer->extendable) {
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

ret_t wbuffer_write_float(wbuffer_t* wbuffer, float_t value) {
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

bool_t wbuffer_has_room(wbuffer_t* wbuffer, uint32_t size) {
  return_value_if_fail(wbuffer != NULL && wbuffer->data != NULL, FALSE);

  return (wbuffer->cursor + size) <= wbuffer->capacity;
}

ret_t wbuffer_write_string(wbuffer_t* wbuffer, const char* data) {
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  return wbuffer_write_binary(wbuffer, data, strlen(data) + 1);
}

rbuffer_t* rbuffer_init(rbuffer_t* rbuffer, const uint8_t* data, uint32_t capacity) {
  return_value_if_fail(rbuffer != NULL && data != NULL, NULL);

  rbuffer->data = data;
  rbuffer->cursor = 0;
  rbuffer->capacity = capacity;

  return rbuffer;
}

bool_t rbuffer_has_more(rbuffer_t* rbuffer) {
  return_value_if_fail(rbuffer != NULL, FALSE);

  return rbuffer->cursor < rbuffer->capacity;
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

ret_t rbuffer_read_float(rbuffer_t* rbuffer, float_t* value) {
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

ret_t rbuffer_read_string(rbuffer_t* rbuffer, const char** str) {
  return_value_if_fail(rbuffer != NULL && rbuffer->data != NULL && str != NULL, RET_BAD_PARAMS);

  *str = (const char*)rbuffer->data + rbuffer->cursor;
  rbuffer->cursor += strlen(*str) + 1;

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

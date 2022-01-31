/**
 * File:   typed_array.c
 * Author: AWTK Develop Team
 * Brief:  dynamic typed_array.
 *
 * Copyright (c) 2021 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/typed_array.h"

static bool_t typed_array_is_type_supported(value_type_t type) {
  switch (type) {
    case VALUE_TYPE_INT8:
    case VALUE_TYPE_UINT8:
    case VALUE_TYPE_INT16:
    case VALUE_TYPE_UINT16:
    case VALUE_TYPE_INT32:
    case VALUE_TYPE_UINT32:
    case VALUE_TYPE_INT64:
    case VALUE_TYPE_UINT64:
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_FLOAT32:
    case VALUE_TYPE_DOUBLE: {
      return TRUE;
    }
    default: {
      return FALSE;
    }
  }
}

typed_array_t* typed_array_create(value_type_t type, uint32_t capacity) {
  typed_array_t* typed_array = NULL;
  uint32_t element_size = value_type_size(type);
  return_value_if_fail(typed_array_is_type_supported(type), NULL);
  typed_array = TKMEM_ZALLOC(typed_array_t);
  return_value_if_fail(typed_array != NULL, NULL);

  typed_array->type = type;
  typed_array->element_size = element_size;
  if (capacity > 0) {
    if (typed_array_extend(typed_array, capacity) != RET_OK) {
      TKMEM_FREE(typed_array);
      return NULL;
    }
  }

  return typed_array;
}

ret_t typed_array_get(typed_array_t* typed_array, uint32_t index, value_t* v) {
  uint8_t* p = NULL;
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(typed_array->data != NULL && index < typed_array->size, RET_BAD_PARAMS);
  p = typed_array->data + index * typed_array->element_size;

  switch (typed_array->type) {
    case VALUE_TYPE_INT8: {
      value_set_int8(v, *(int8_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(v, *(uint8_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(v, *(int16_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(v, *(uint16_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(v, *(int32_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(v, *(uint32_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(v, *(int64_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(v, *(uint64_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_FLOAT: {
      value_set_float(v, *(float_t*)p);
      return RET_OK;
    }
    case VALUE_TYPE_FLOAT32: {
      value_set_float32(v, *(float*)p);
      return RET_OK;
    }
    case VALUE_TYPE_DOUBLE: {
      value_set_double(v, *(double*)p);
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_NOT_IMPL;
    }
  }
}

ret_t typed_array_set(typed_array_t* typed_array, uint32_t index, const value_t* v) {
  uint8_t* p = NULL;
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(typed_array->data != NULL && index < typed_array->size, RET_BAD_PARAMS);
  p = typed_array->data + index * typed_array->element_size;

  switch (typed_array->type) {
    case VALUE_TYPE_INT8: {
      *(int8_t*)p = value_int8(v);
      return RET_OK;
    }
    case VALUE_TYPE_UINT8: {
      *(uint8_t*)p = value_uint8(v);
      return RET_OK;
    }
    case VALUE_TYPE_INT16: {
      *(int16_t*)p = value_int16(v);
      return RET_OK;
    }
    case VALUE_TYPE_UINT16: {
      *(uint16_t*)p = value_uint16(v);
      return RET_OK;
    }
    case VALUE_TYPE_INT32: {
      *(int32_t*)p = value_int32(v);
      return RET_OK;
    }
    case VALUE_TYPE_UINT32: {
      *(uint32_t*)p = value_uint32(v);
      return RET_OK;
    }
    case VALUE_TYPE_INT64: {
      *(int64_t*)p = value_int64(v);
      return RET_OK;
    }
    case VALUE_TYPE_UINT64: {
      *(uint64_t*)p = value_uint64(v);
      return RET_OK;
    }
    case VALUE_TYPE_FLOAT: {
      *(float_t*)p = value_float(v);
      return RET_OK;
    }
    case VALUE_TYPE_FLOAT32: {
      *(float*)p = value_float32(v);
      return RET_OK;
    }
    case VALUE_TYPE_DOUBLE: {
      *(double*)p = value_double(v);
      return RET_OK;
    }
    default: {
      assert(!"not supported type");
      return RET_NOT_IMPL;
    }
  }
}

ret_t typed_array_extend(typed_array_t* typed_array, uint32_t capacity) {
  return_value_if_fail(typed_array != NULL, RET_BAD_PARAMS);
  if (capacity > typed_array->capacity) {
    uint8_t* p = NULL;
    uint32_t mem_size = (capacity + 1) * typed_array->element_size;
    uint32_t data_size = typed_array->element_size * typed_array->size;
    uint8_t* data = (uint8_t*)TKMEM_REALLOC(typed_array->data, mem_size);
    return_value_if_fail(data != NULL, RET_OOM);
    ENSURE(((uintptr_t)data % 8) == 0);

    typed_array->data = data;
    typed_array->capacity = capacity;
    p = data + data_size;
    memset(p, 0x00, mem_size - data_size);
  }

  return RET_OK;
}

static ret_t typed_array_extend_delta(typed_array_t* typed_array, uint32_t delta) {
  return_value_if_fail(typed_array != NULL, RET_BAD_PARAMS);

  if ((typed_array->size + delta) > typed_array->capacity) {
    uint32_t capacity = (typed_array->size + delta) + 1.2F;
    return typed_array_extend(typed_array, capacity);
  }

  return RET_OK;
}

ret_t typed_array_insert(typed_array_t* typed_array, uint32_t index, const value_t* v) {
  uint8_t* p = NULL;
  uint8_t* s = NULL;
  uint8_t* d = NULL;
  uint32_t element_size = 0;
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);
  index = tk_min(index, typed_array->size);
  return_value_if_fail(typed_array_extend_delta(typed_array, 1) == RET_OK, RET_OOM);

  element_size = typed_array->element_size;
  p = typed_array->data + index * element_size;
  d = typed_array->data + element_size * typed_array->size;
  s = d - element_size;

  while (s >= p) {
    memcpy(d, s, element_size);
    s -= element_size;
    d -= element_size;
  }
  typed_array->size++;

  return typed_array_set(typed_array, index, v);
}

ret_t typed_array_remove(typed_array_t* typed_array, uint32_t index) {
  uint8_t* p = NULL;
  uint8_t* s = NULL;
  uint8_t* d = NULL;
  uint32_t element_size = 0;
  return_value_if_fail(typed_array != NULL, RET_BAD_PARAMS);
  return_value_if_fail(typed_array->data != NULL && index < typed_array->size, RET_BAD_PARAMS);

  element_size = typed_array->element_size;
  d = typed_array->data + index * element_size;
  s = d + element_size;
  p = typed_array->data + element_size * typed_array->size;

  while (s < p) {
    memcpy(d, s, element_size);
    s += element_size;
    d += element_size;
  }
  typed_array->size--;

  return RET_OK;
}

ret_t typed_array_pop(typed_array_t* typed_array, value_t* v) {
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(typed_array->size > 0, RET_BAD_PARAMS);

  typed_array_get(typed_array, typed_array->size - 1, v);
  typed_array->size--;

  return RET_OK;
}

ret_t typed_array_tail(typed_array_t* typed_array, value_t* v) {
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(typed_array->size > 0, RET_BAD_PARAMS);

  return typed_array_get(typed_array, typed_array->size - 1, v);
}

ret_t typed_array_push(typed_array_t* typed_array, const value_t* v) {
  return_value_if_fail(typed_array != NULL && v != NULL, RET_BAD_PARAMS);

  return typed_array_insert(typed_array, typed_array->size, v);
}

ret_t typed_array_clear(typed_array_t* typed_array) {
  return_value_if_fail(typed_array != NULL, RET_BAD_PARAMS);
  typed_array->size = 0;

  return RET_OK;
}

ret_t typed_array_destroy(typed_array_t* typed_array) {
  return_value_if_fail(typed_array != NULL, RET_BAD_PARAMS);
  typed_array_clear(typed_array);
  TKMEM_FREE(typed_array->data);
  memset(typed_array, 0x00, sizeof(typed_array_t));
  TKMEM_FREE(typed_array);

  return RET_OK;
}

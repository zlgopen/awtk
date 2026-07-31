/**
 * File:   object_fifo_default.c
 * Author: AWTK Develop Team
 * Brief:  object_fifo 的默认实现
 *
 * Copyright (c) 2024 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-22 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "object_fifo_default.h"

static void* object_fifo_default_get(object_t* obj, uint32_t index) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL && fifo->buffer != NULL, NULL);

  return (void*)(fifo->buffer + fifo->unit_size * index);
}

static ret_t object_fifo_default_set(object_t* obj, uint32_t index, const void* data, uint32_t nr) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  uint8_t* start = (uint8_t*)(data);
  uint8_t* elem = (uint8_t*)(fifo->buffer + fifo->unit_size * index);

  if (nr > fifo->capacity) {
    if (start != NULL) {
      start += (nr - fifo->capacity) * fifo->unit_size;
    }
    nr = fifo->capacity;
  }

  if (index + nr <= fifo->capacity) {
    if (start != NULL) {
      memcpy(elem, start, fifo->unit_size * nr);
    } else {
      memset(elem, 0x00, fifo->unit_size * nr);
    }
  } else {
    uint32_t part = fifo->capacity - index;
    if (start != NULL) {
      memcpy(elem, start, fifo->unit_size * part);
      memcpy(fifo->buffer, start + fifo->unit_size * part, fifo->unit_size * (nr - part));
    } else {
      memset(elem, 0x00, fifo->unit_size * part);
      memset(fifo->buffer, 0x00, fifo->unit_size * (nr - part));
    }
  }

  return RET_OK;
}

static object_t* object_fifo_default_part_clone(object_t* obj, uint32_t index, uint32_t nr) {
  object_t* clone = NULL;
  object_fifo_default_t* fifo_clone = NULL;
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL && fifo->buffer != NULL, NULL);

  clone = object_fifo_default_create(nr, fifo->unit_size);
  fifo_clone = OBJECT_FIFO_DEFAULT(clone);

  if (fifo_clone != NULL) {
    uint8_t* start = (uint8_t*)(fifo->buffer + fifo->unit_size * index);

    if (index + nr <= fifo->capacity) {
      memcpy(fifo_clone->buffer, start, fifo->unit_size * nr);
    } else {
      uint32_t part = fifo->capacity - index;
      memcpy(fifo_clone->buffer, start, fifo->unit_size * part);
      memcpy(fifo_clone->buffer, fifo_clone->buffer, fifo->unit_size * (nr - part));
    }

    fifo_clone->size = nr;
    fifo_clone->cursor = nr - 1;
  }

  return clone;
}

static int object_fifo_default_compare(object_t* obj, const void* a, const void* b) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  return memcmp(a, b, fifo->unit_size);
}

static ret_t object_fifo_default_set_capacity(object_t* obj, uint32_t capacity) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL && fifo->buffer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(fifo->buffer);

  fifo->buffer = TKMEM_ZALLOCN(uint8_t, (fifo->unit_size * capacity));
  return_value_if_fail(fifo->buffer != NULL, RET_OOM);

  fifo->size = 0;
  fifo->cursor = 0;
  fifo->capacity = capacity;

  return RET_OK;
}

static ret_t object_fifo_default_on_destroy(object_t* obj) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(fifo->buffer);

  return RET_OK;
}

static ret_t object_fifo_default_get_prop(object_t* obj, const char* name, value_t* v) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, OBJECT_FIFO_PROP_CAPACITY)) {
    value_set_uint32(v, fifo->capacity);
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_SIZE)) {
    value_set_uint32(v, fifo->size);
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_CURSOR)) {
    value_set_uint32(v, fifo->cursor);
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_UNIT_SIZE)) {
    value_set_uint32(v, fifo->unit_size);
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_BUFFER)) {
    value_set_pointer(v, fifo->buffer);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t object_fifo_default_set_prop(object_t* obj, const char* name, const value_t* v) {
  object_fifo_default_t* fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(fifo != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, OBJECT_FIFO_PROP_CAPACITY)) {
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_SIZE)) {
    fifo->size = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, OBJECT_FIFO_PROP_CURSOR)) {
    fifo->cursor = value_uint32(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_object_vtable = {.type = "object_fifo_default",
                                                .desc = "object_fifo_default",
                                                .size = sizeof(object_fifo_default_t),
                                                .is_collection = FALSE,
                                                .on_destroy = object_fifo_default_on_destroy,
                                                .get_prop = object_fifo_default_get_prop,
                                                .set_prop = object_fifo_default_set_prop};

static const object_fifo_vtable_t s_object_fifo_vtable = {
    .part_clone = object_fifo_default_part_clone,
    .get = object_fifo_default_get,
    .set = object_fifo_default_set,
    .compare = object_fifo_default_compare,
    .set_capacity = object_fifo_default_set_capacity,
};

object_t* object_fifo_default_create(uint32_t capacity, uint32_t unit_size) {
  object_t* obj = NULL;
  object_fifo_t* object_fifo = NULL;
  object_fifo_default_t* fifo = NULL;
  return_value_if_fail(capacity > 0 && unit_size > 0, NULL);

  obj = object_create(&s_object_vtable);
  object_fifo = OBJECT_FIFO(obj);
  fifo = OBJECT_FIFO_DEFAULT(obj);
  return_value_if_fail(obj != NULL && object_fifo != NULL && fifo != NULL, NULL);

  object_fifo->vt = &s_object_fifo_vtable;

  fifo->buffer = TKMEM_ZALLOCN(uint8_t, (unit_size * capacity));
  if (fifo->buffer == NULL) {
    OBJECT_UNREF(obj);
    return NULL;
  }

  fifo->capacity = capacity;
  fifo->unit_size = unit_size;
  fifo->cursor = 0;
  fifo->size = 0;

  return obj;
}
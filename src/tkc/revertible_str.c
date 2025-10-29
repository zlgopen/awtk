/**
 * File:   revertible_str.c
 * Author: AWTK Develop Team
 * Brief:  revertible_str
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
 * 2025-9-02 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/revertible_str.h"

static ret_t revertible_str_extend(revertible_str_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (capacity <= str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    uint32_t* offsets = TKMEM_REALLOCT(uint32_t, str->offsets, capacity);
    return_value_if_fail(offsets != NULL, RET_FAIL);

    str->offsets = offsets;
    str->capacity = capacity;
  }
  return RET_OK;
}

static ret_t revertible_str_add_offset(revertible_str_t* str, uint32_t offset) {
  if (str->size + 1 > str->capacity) {
    return_value_if_fail(revertible_str_extend(str, str->capacity * 2) == RET_OK, RET_OOM);
  }
  str->offsets[str->size++] = offset;
  return RET_OK;
}

revertible_str_t* revertible_str_init(revertible_str_t* str, uint32_t capacity,
                                      uint32_t offset_capacity) {
  return_value_if_fail(str != NULL, NULL);
  return_value_if_fail(str_init(&str->base, capacity) != NULL, NULL);
  str->offsets = TKMEM_ZALLOCN(uint32_t, offset_capacity);
  if (str->offsets != NULL) {
    str->size = 0;
    str->capacity = offset_capacity;
    return str;
  } else {
    str_reset(&str->base);
    return NULL;
  }
}

ret_t revertible_str_deinit(revertible_str_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  if (str->offsets != NULL) {
    TKMEM_FREE(str->offsets);
    str->offsets = NULL;
  }
  str->size = 0;
  str->capacity = 0;
  return str_reset(&str->base);
}

revertible_str_t* revertible_str_create(uint32_t capacity, uint32_t offset_capacity) {
  revertible_str_t* str = TKMEM_ZALLOC(revertible_str_t);
  return_value_if_fail(str != NULL, NULL);
  if (revertible_str_init(str, capacity, offset_capacity) != NULL) {
    return str;
  } else {
    TKMEM_FREE(str);
    return NULL;
  }
}

ret_t revertible_str_destroy(revertible_str_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  revertible_str_deinit(str);
  TKMEM_FREE(str);
  return RET_OK;
}

ret_t revertible_str_revert(revertible_str_t* str) {
  uint32_t offset = 0;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  if (str->size > 0) {
    offset = str->offsets[str->size - 1];
    str->size--;

    str->base.str[offset] = '\0';
    str->base.size = offset;
  }
  return RET_OK;
}

ret_t revertible_str_append(revertible_str_t* str, const char* text) {
  ret_t ret = RET_OK;
  uint32_t offset = 0;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  offset = str->base.size;
  ret = str_append(&str->base, text);
  if (ret == RET_OK) {
    ret = revertible_str_add_offset(str, offset);
  }
  return ret;
}

ret_t revertible_str_append_format(revertible_str_t* str, uint32_t size, const char* format, ...) {
  va_list va;
  ret_t ret = RET_OK;
  uint32_t offset = 0;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  offset = str->base.size;
  va_start(va, format);
  str_append_vformat(&str->base, size, format, va);
  va_end(va);
  if (ret == RET_OK) {
    ret = revertible_str_add_offset(str, offset);
  }
  return ret;
}

const char* revertible_str_get_str(const revertible_str_t* str) {
  return_value_if_fail(str != NULL, NULL);
  return str->base.str;
}

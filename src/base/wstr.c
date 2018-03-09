/**
 * File:   wstr.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  width char
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/wstr.h"
#include "base/mem.h"

static ret_t wstr_extend(wstr_t* str, uint16_t capacity) {
  if (capacity < str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    wchar_t* s = MEM_REALLOC(wchar_t, str->str, capacity + 1);
    return_value_if_fail(s != NULL, RET_FAIL);

    s[capacity] = 0;
    str->str = s;
    str->capacity = capacity;
  }

  return RET_OK;
}

wstr_t* wstr_init(wstr_t* str, uint16_t capacity) {
  return_value_if_fail(str != NULL, NULL);

  memset(str, 0x00, sizeof(wstr_t));

  return wstr_extend(str, capacity) == RET_OK ? str : NULL;
}

ret_t wstr_set(wstr_t* str, const wchar_t* text) {
  uint16_t size = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  size = wcslen(text);
  return_value_if_fail(wstr_extend(str, size) == RET_OK, RET_BAD_PARAMS);

  wcscpy(str->str, text);
  str->size = size;

  return RET_OK;
}

ret_t wstr_set_utf8(wstr_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

ret_t wstr_get_utf8(wstr_t* str, char* text, uint16_t size) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

ret_t wstr_reset(wstr_t* str) {
  return_value_if_fail(str != NULL, RET_OK);
  MEM_FREE(str->str);

  return RET_OK;
}

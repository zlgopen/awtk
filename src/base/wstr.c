/**
 * File:   wstr.c
 * Author: AWTK Develop Team
 * Brief:  width char
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "base/utf8.h"
#include "base/wstr.h"
#include "base/utils.h"

static ret_t wstr_extend(wstr_t* str, uint16_t capacity) {
  if (capacity < str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    wchar_t* s = TKMEM_REALLOC(wchar_t, str->str, capacity + 1);
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
  return_value_if_fail(wstr_extend(str, size + 1) == RET_OK, RET_BAD_PARAMS);

  wcscpy(str->str, text);
  str->size = size;

  return RET_OK;
}

ret_t wstr_set_utf8(wstr_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, strlen(text) + 2) == RET_OK, RET_OOM);

  utf8_to_utf16(text, str->str, str->capacity - 1);
  str->size = wcslen(str->str);

  return RET_OK;
}

ret_t wstr_get_utf8(wstr_t* str, char* text, uint16_t size) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  utf8_from_utf16(str->str, text, size);

  return RET_OK;
}

ret_t wstr_remove(wstr_t* str, uint16_t offset, uint16_t nr) {
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && offset < str->size && nr > 0, RET_BAD_PARAMS);

  if ((offset + nr) > str->size) {
    nr = str->size - offset;
  }

  if (nr > 0) {
    p = str->str;
    memmove(p + offset, p + offset + nr, nr * sizeof(wchar_t));
    str->size -= nr;
    str->str[str->size] = 0;
  }

  return RET_OK;
}

ret_t wstr_insert(wstr_t* str, uint16_t offset, const wchar_t* text, uint16_t nr) {
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && text != NULL && offset <= str->size && nr > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, str->size + nr + 1) == RET_OK, RET_BAD_PARAMS);

  p = str->str;
  if (str->size > offset) {
    uint16_t i = 0;
    uint16_t s = str->size - 1;
    uint16_t d = str->size + nr - 1;
    uint16_t move_nr = str->size - offset;

    for (i = 0; i < move_nr; i++, d--, s--) {
      p[d] = p[s];
    }
  }
  memcpy(p + offset, text, nr * sizeof(wchar_t));
  str->size += nr;
  str->str[str->size] = 0;

  return RET_OK;
}

ret_t wstr_push(wstr_t* str, const wchar_t c) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, str->size + 1) == RET_OK, RET_BAD_PARAMS);
  str->str[str->size++] = c;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t wstr_pop(wstr_t* str) {
  return_value_if_fail(str != NULL && str->size > 0, RET_BAD_PARAMS);
  str->size--;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t wstr_from_int(wstr_t* str, int32_t v) {
  char buff[32];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return wstr_set_utf8(str, ftk_itoa(buff, sizeof(buff), v));
}

ret_t wstr_from_float(wstr_t* str, float v) {
  char buff[32];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return wstr_set_utf8(str, ftk_ftoa(buff, sizeof(buff), v));
}

ret_t wstr_from_value(wstr_t* str, const value_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_STRING) {
    return wstr_set_utf8(str, value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    return wstr_set(str, value_wstr(v));
  } else if (v->type == VALUE_TYPE_FLOAT) {
    return wstr_from_float(str, value_float(v));
  } else {
    return wstr_from_int(str, value_int(v));
  }
}

ret_t wstr_to_int(wstr_t* str, int32_t* v) {
  char buff[32];
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  wstr_get_utf8(str, buff, sizeof(buff));
  *v = ftk_atoi(buff);

  return RET_OK;
}

ret_t wstr_to_float(wstr_t* str, float* v) {
  char buff[32];
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  wstr_get_utf8(str, buff, sizeof(buff));
  *v = ftk_atof(buff);

  return RET_OK;
}

ret_t wstr_reset(wstr_t* str) {
  return_value_if_fail(str != NULL, RET_OK);
  TKMEM_FREE(str->str);

  return RET_OK;
}

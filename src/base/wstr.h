/**
 * File:   wstr.h
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


#ifndef WSTR_H
#define WSTR_H

#include "base/value.h"
#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _wstr_t {
  uint16_t size;
  uint16_t capacity;
  wchar_t* str;
} wstr_t;

wstr_t* wstr_init(wstr_t* str, uint16_t capacity);

ret_t wstr_set(wstr_t* str, const wchar_t* text);
ret_t wstr_set_utf8(wstr_t* str, const char* text);
ret_t wstr_get_utf8(wstr_t* str, char* text, uint16_t size);

ret_t wstr_remove(wstr_t* str, uint16_t offset, uint16_t nr);
ret_t wstr_insert(wstr_t* str, uint16_t offset, const wchar_t* text, uint16_t nr);
ret_t wstr_push(wstr_t* str, const wchar_t c);
ret_t wstr_pop(wstr_t* str);

ret_t wstr_from_int(wstr_t* str, int32_t v);
ret_t wstr_from_float(wstr_t* str, float v);
ret_t wstr_from_value(wstr_t* str, value_t* v);

ret_t wstr_to_int(wstr_t* str, int32_t* v);
ret_t wstr_to_float(wstr_t* str, float* v);

ret_t wstr_reset(wstr_t* str);

END_C_DECLS

#endif

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

#include "base/types_def.h"

#ifndef WSTR_H
#define WSTR_H 

BEGIN_C_DECLS

typedef struct _wstr_t {
  uint16_t size;
  uint16_t capacity;
  wchar_t* str;
}wstr_t;

wstr_t* wstr_init(wstr_t* str, uint16_t capacity);

ret_t wstr_set(wstr_t* str, const wchar_t* text);
ret_t wstr_set_utf8(wstr_t* str, const char* text);
ret_t wstr_get_utf8(wstr_t* str, char* text, uint16_t size);

ret_t wstr_reset(wstr_t* str);

END_C_DECLS

#endif


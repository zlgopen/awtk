/**
 * File:   wchar32.h
 * Author: AWTK Develop Team
 * Brief:  32bits wide char
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-12-19 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_WCHAR32_H
#define TK_WCHAR32_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

#ifdef WIN32
typedef uint32_t wchar32_t;
#else 
typedef wchar_t wchar32_t;
#endif

wchar32_t* wcs32_init_with_wcs(wchar32_t* s1, const wchar_t* s2);

wchar32_t* wcp32cpy(wchar32_t* s1, wchar32_t* s2);

wchar32_t* wcp32ncpy(wchar32_t* s1, wchar32_t* s2, size_t n);

int wcs32casecmp(const wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32cat(wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32chr(const wchar32_t* s, wchar32_t c);

int wcs32cmp(const wchar32_t* s1, const wchar32_t* s2);

size_t wcs32cspn(const wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32dup(const wchar32_t* s);

size_t wcs32lcat(wchar32_t* s1, const wchar32_t* s2, size_t n);

size_t wcs32lcpy(wchar32_t* s1, const wchar32_t* s2, size_t n);

size_t wcs32len(const wchar32_t* s);

int wcs32ncasecmp(const wchar32_t* s1, const wchar32_t* s2, size_t n);

wchar32_t* wcs32cpy(wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32ncat(wchar32_t* s1, const wchar32_t* s2, size_t n);

int wcs32ncmp(const wchar32_t* s1, const wchar32_t* s2, size_t n);

wchar32_t* wcs32ncpy(wchar32_t* s1, const wchar32_t* s2, size_t n);

size_t wcs32nlen(const wchar32_t* s, size_t maxlen);

wchar32_t* wcs32pbrk(const wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32rchr(const wchar32_t* s, wchar32_t c);

size_t wcs32spn(const wchar32_t* s1, const wchar32_t* s2);

wchar32_t* wcs32str(const wchar32_t* s1, const wchar32_t* s2);

wchar32_t* w32memchr(const wchar32_t* s, wchar32_t c, size_t n);

int w32memcmp(const wchar32_t* s1, const wchar32_t* s2, size_t n);

wchar32_t* w32memcpy(wchar32_t* s1, const wchar32_t* s2, size_t n);

wchar32_t* w32memmove(wchar32_t* s1, const wchar32_t* s2, size_t n);
wchar32_t* w32memset(wchar32_t* s, wchar32_t c, size_t n);

#ifdef WITH_WCSXXX
wchar_t* wcpcpy(wchar_t* s1, wchar_t* s2);
wchar_t* wcpncpy(wchar_t* s1, wchar_t* s2, size_t n);
int wcscasecmp(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcscat(wchar_t* s1, const wchar_t* s2);
wchar_t* wcschr(const wchar_t* s, wchar_t c);
int wcscmp(const wchar_t* s1, const wchar_t* s2);
size_t wcscspn(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcsdup(const wchar_t* s);
size_t wcslcat(wchar_t* s1, const wchar_t* s2, size_t n);
size_t wcslcpy(wchar_t* s1, const wchar_t* s2, size_t n);
size_t wcslen(const wchar_t* s);
int wcsncasecmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wcscpy(wchar_t* s1, const wchar_t* s2);
wchar_t* wcsncat(wchar_t* s1, const wchar_t* s2, size_t n);
int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wcsncpy(wchar_t* s1, const wchar_t* s2, size_t n);
size_t wcsnlen(const wchar_t* s, size_t maxlen);
wchar_t* wcspbrk(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcsrchr(const wchar_t* s, wchar_t c);
size_t wcsspn(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcsstr(const wchar_t* s1, const wchar_t* s2);
wchar_t* wmemchr(const wchar_t* s, wchar_t c, size_t n);
int wmemcmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wmemcpy(wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wmemmove(wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wmemset(wchar_t* s, wchar_t c, size_t n);
#endif /*WITH_WCSXXX*/

END_C_DECLS

#endif /*TK_WCHAR32_H*/

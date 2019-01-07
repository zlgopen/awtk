/**
 * File:   utf8.h
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UTF8_H
#define TK_UTF8_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

char* utf8_from_utf16(const wchar_t* str, char* out, uint32_t size);
wchar_t* utf8_to_utf16(const char* str, wchar_t* out, uint32_t size);

END_C_DECLS

#endif /*TK_UTF8_H*/

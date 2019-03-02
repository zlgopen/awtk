/**
 * File:   str_to_str.h
 * Author: AWTK Develop Team
 * Brief:  map one str to another str
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
 * 2019-01-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STR_STR_H
#define TK_STR_STR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _str_str_t {
  const char* name;
  const char* value;
} str_str_t;

const char* str_str_value(const str_str_t* items, const char* name);

const char* str_str_name(const str_str_t* items, const char* value);

END_C_DECLS

#endif /*TK_STR_STR_H*/

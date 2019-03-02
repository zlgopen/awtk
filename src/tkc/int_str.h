/**
 * File:   int_str.h
 * Author: AWTK Develop Team
 * Brief:  map between int and str
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

#ifndef TK_INT_STR_H
#define TK_INT_STR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _int_str_t {
  int32_t name;
  const char* value;
} int_str_t;

const char* int_str_value(const int_str_t* items, int32_t name);

int32_t int_str_name(const int_str_t* items, const char* value, int32_t defval);

END_C_DECLS

#endif /*TK_INT_STR_H*/

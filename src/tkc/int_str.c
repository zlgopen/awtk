/**
 * File:   int_str.c
 * Author: AWTK Develop Team
 * Brief:  map between int and str
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/int_str.h"

const char* int_str_value(const int_str_t* items, int32_t name) {
  const int_str_t* iter = items;
  return_value_if_fail(items != NULL, NULL);

  while (iter->value != NULL) {
    if (iter->name == name) {
      return iter->value;
    }

    iter++;
  }

  return NULL;
}

int32_t int_str_name(const int_str_t* items, const char* value, int32_t defval) {
  const int_str_t* iter = items;
  return_value_if_fail(items != NULL && value != NULL, defval);

  while (iter->value != NULL) {
    if (tk_str_eq(iter->value, value)) {
      return iter->name;
    }

    iter++;
  }

  return defval;
}

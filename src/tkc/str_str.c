/**
 * File:   str_to_str.c
 * Author: AWTK Develop Team
 * Brief:  map one str to another str
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

#include "tkc/str_str.h"

const char* str_str_value(const str_str_t* items, const char* name) {
  const str_str_t* iter = items;
  return_value_if_fail(items != NULL && name != NULL, NULL);

  while (iter->name != NULL) {
    if (tk_str_eq(iter->name, name)) {
      return iter->value;
    }

    iter++;
  }

  return NULL;
}

const char* str_str_name(const str_str_t* items, const char* value) {
  const str_str_t* iter = items;
  return_value_if_fail(items != NULL && value != NULL, NULL);

  while (iter->value != NULL) {
    if (tk_str_eq(iter->value, value)) {
      return iter->name;
    }

    iter++;
  }

  return NULL;
}

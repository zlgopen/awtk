/**
 * File:   enums.h
 * Author: AWTK Develop Team
 * Brief:  enumerations
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ENUMS_H
#define TK_ENUMS_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _key_type_value_t {
  const char* name;
  uint32_t type;
  uint32_t value;
} key_type_value_t;

enum { TYPE_INT = 0, TYPE_COLOR, TYPE_STRING, TYPE_WSTRING, TYPE_BOOL };

const key_type_value_t* input_type_find(const char* name);
const key_type_value_t* window_closable_type_find(const char* name);
const key_type_value_t* align_v_type_find(const char* name);
const key_type_value_t* align_h_type_find(const char* name);
const key_type_value_t* asset_type_find(const char* name);
const key_type_value_t* asset_type_find_by_value(uint32_t value);

const key_type_value_t* image_draw_type_find(const char* name);
const key_type_value_t* image_draw_type_find_by_value(uint32_t value);

const key_type_value_t* find_item(const key_type_value_t* items, uint32_t nr, const char* name);
const key_type_value_t* find_item_by_value(const key_type_value_t* items, uint32_t nr,
                                           uint32_t value);
const key_type_value_t* easing_type_find(const char* name);
const key_type_value_t* easing_type_find_by_value(uint32_t value);

const key_type_value_t* keys_type_find(const char* name);
const key_type_value_t* keys_type_find_by_value(uint32_t value);
END_C_DECLS

#endif /*TK_ENUMS_H*/

/**
 * File:   theme_data.h
 * Author: AWTK Develop Team
 * Brief:  theme data
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_THEME_DATA_H
#define TK_THEME_DATA_H

#include "tkc/color.h"
#include "base/gradient.h"

BEGIN_C_DECLS

#define THEME_MAGIC 0xFAFBFCFD

#pragma pack(push, 1)

typedef struct _theme_header_t {
  uint32_t magic;
  uint32_t version;
  uint32_t nr;
} theme_header_t;

typedef struct _theme_item_t {
  uint32_t offset;
  char state[TK_NAME_LEN + 1];
  char name[TK_NAME_LEN + 1];
  char widget_type[TK_NAME_LEN + 1];
} theme_item_t;

typedef struct _style_name_value_header_t {
  uint32_t type : 8;
  uint32_t name_size : 8;
  uint32_t value_size : 16;
} style_name_value_header_t;

typedef struct _style_name_value_t {
  uint32_t type : 8;
  uint32_t name_size : 8;
  uint32_t value_size : 16;
  const char name[4];
} style_name_value_t;

#pragma pack(pop)

ret_t style_data_get_value(const uint8_t* s, const char* name, value_t* v);
int32_t style_data_get_int(const uint8_t* s, const char* name, int32_t defval);
uint32_t style_data_get_uint(const uint8_t* s, const char* name, uint32_t defval);
color_t style_data_get_color(const uint8_t* s, const char* name, color_t defval);
gradient_t* style_data_get_gradient(const uint8_t* s, const char* name, gradient_t* gradient);
const char* style_data_get_str(const uint8_t* s, const char* name, const char* defval);

#define STYLE_NAME_SIZE_MAX 255
#define STYLE_VALUE_SIZE_MAX 1023

END_C_DECLS

#endif /*TK_THEME_DATA_H*/

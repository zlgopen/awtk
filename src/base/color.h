/**
 * File:   color.h
 * Author: AWTK Develop Team
 * Brief:  color structs.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_H
#define TK_COLOR_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _rgba_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} rgba_t;

typedef union _color_t {
  rgba_t rgba;
  uint32_t color;
} color_t;

typedef uint8_t gray_t;

const char* color_hex_str(color_t c, char str[8]);
color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

END_C_DECLS

#endif /*TK_COLOR_H*/

/**
 * File:   color.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  color structs.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_COLOR_H
#define LFTK_COLOR_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _rgba_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
}rgba_t;

typedef struct _color_t {
  union {
    rgba_t rgba;
    uint32_t color;
  };
}color_t;

typedef uint8_t  gray_t;

color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

END_C_DECLS

#endif/*LFTK_COLOR_H*/

/**
 * File:   matrix.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  matrix struct and utils functions.
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
 * 2018-03-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_MATRIX_H
#define LFTK_MATRIX_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _matrix_t {
  float a0;
  float a1;
  float a2;
  float a3;
  float a4;
  float a5;
} matrix_t;

matrix_t* matrix_init(matrix_t* m);

matrix_t* matrix_identity(matrix_t* m);
matrix_t* matrix_translate(matrix_t* m, xy_t x, xy_t y);
matrix_t* matrix_scale(matrix_t* m, float sx, float sy);
matrix_t* matrix_rotate(matrix_t* m, float rad);
matrix_t* matrix_transform_debug(matrix_t* m, xy_t x, xy_t y, xy_t* out_x, xy_t* out_y);

#define matrix_transform(_m, _x, _y, _ox, _oy) \
  tempx = _m.a0 * _x + _m.a2 * _y + _m.a4;\
  tempy = _m.a1 * _x + _m.a3 * _y + _m.a5;\
  _ox = tempx > 0 ? tempx + 0.5 : tempx - 0.5;\
  _oy = tempy > 0 ? tempy + 0.5 : tempy - 0.5;

END_C_DECLS

#endif /*LFTK_MATRIX_H*/


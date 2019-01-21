/**
 * File:   matrix.h
 * Author: AWTK Develop Team
 * Brief:  matrix struct and utils functions.
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
 * 2018-03-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MATRIX_H
#define TK_MATRIX_H

#include "tkc/types_def.h"

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
matrix_t* matrix_invert(matrix_t* m);
matrix_t* matrix_set(matrix_t* m, float a0, float a1, float a2, float a3, float a4, float a5);
matrix_t* matrix_multiply(matrix_t* m, matrix_t* b);

matrix_t* matrix_translate(matrix_t* m, xy_t x, xy_t y);
matrix_t* matrix_scale(matrix_t* m, float sx, float sy);
matrix_t* matrix_rotate(matrix_t* m, float rad);
matrix_t* matrix_transform_point(matrix_t* m, xy_t x, xy_t y, xy_t* out_x, xy_t* out_y);

END_C_DECLS

#endif /*TK_MATRIX_H*/

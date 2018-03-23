/**
 * File:   matrix.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  matrix
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; withm even the implied warranty of
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

#include <math.h>
#include "base/matrix.h"

matrix_t* matrix_init(matrix_t* m) {
  return_value_if_fail(m != NULL, NULL);

  return matrix_identity(m);
}

matrix_t* matrix_identity(matrix_t* m) {
  return_value_if_fail(m != NULL, NULL);

  m->a0 = 1;
  m->a1 = 0;
  m->a2 = 0;
  m->a3 = 1;
  m->a4 = 0;
  m->a5 = 0;

  return m;
}

matrix_t* matrix_translate(matrix_t* m, xy_t x, xy_t y) {
  float a0 = m->a0, a1 = m->a1, a2 = m->a2, a3 = m->a3, a4 = m->a4, a5 = m->a5;

  m->a0 = a0;
  m->a1 = a1;
  m->a2 = a2;
  m->a3 = a3;
  m->a4 = a0 * x + a2 * y + a4;
  m->a5 = a1 * x + a3 * y + a5;

  return m;
}

matrix_t* matrix_scale(matrix_t* m, float sx, float sy) {
  float a0 = m->a0, a1 = m->a1, a2 = m->a2, a3 = m->a3, a4 = m->a4, a5 = m->a5;
  m->a0 = a0 * sx;
  m->a1 = a1 * sx;
  m->a2 = a2 * sy;
  m->a3 = a3 * sy;
  m->a4 = a4;
  m->a5 = a5;

  return m;
}

matrix_t* matrix_rotate(matrix_t* m, float rad) {
  float a0 = m->a0, a1 = m->a1, a2 = m->a2, a3 = m->a3, a4 = m->a4, a5 = m->a5, s = sin(rad),
        c = cos(rad);

  m->a0 = a0 * c + a2 * s;
  m->a1 = a1 * c + a3 * s;
  m->a2 = a0 * -s + a2 * c;
  m->a3 = a1 * -s + a3 * c;
  m->a4 = a4;
  m->a5 = a5;

  return m;
}

matrix_t* matrix_transform_debug(matrix_t* m, xy_t x, xy_t y, xy_t* ox, xy_t* oy) {
  float x1 = m->a0 * x + m->a2 * y + m->a4;
  float y1 = m->a1 * x + m->a3 * y + m->a5;

  *ox = x1 > 0 ? x1 + 0.5 : x1 - 0.5;
  *oy = y1 > 0 ? y1 + 0.5 : y1 - 0.5;

  return m;
}

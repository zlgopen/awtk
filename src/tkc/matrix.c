/**
 * File:   matrix.c
 * Author: AWTK Develop Team
 * Brief:  matrix
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-03-23 Li XianJing <xianjimli@hotmail.com> created. adapted
 * from:https://github.com/toji/gl-matrix/tree/master/src/gl-matrix
 *
 */

#include <math.h>
#include "tkc/matrix.h"

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

matrix_t* matrix_invert(matrix_t* m) {
  float aa = m->a0, ab = m->a1, ac = m->a2, ad = m->a3;
  float atx = m->a4, aty = m->a5;
  float det = aa * ad - ab * ac;

  if (!det) {
    return NULL;
  }

  det = 1.0f / det;

  m->a0 = ad * det;
  m->a1 = -ab * det;
  m->a2 = -ac * det;
  m->a3 = aa * det;
  m->a4 = (ac * aty - ad * atx) * det;
  m->a5 = (ab * atx - aa * aty) * det;

  return m;
}

matrix_t* matrix_set(matrix_t* m, float a0, float a1, float a2, float a3, float a4, float a5) {
  m->a0 = a0;
  m->a1 = a1;
  m->a2 = a2;
  m->a3 = a3;
  m->a4 = a4;
  m->a5 = a5;

  return m;
}

matrix_t* matrix_multiply(matrix_t* m, matrix_t* b) {
  float a0 = m->a0, a1 = m->a1, a2 = m->a2, a3 = m->a3, a4 = m->a4, a5 = m->a5;
  float b0 = b->a0, b1 = b->a1, b2 = b->a2, b3 = b->a3, b4 = b->a4, b5 = b->a5;

  m->a0 = a0 * b0 + a2 * b1;
  m->a1 = a1 * b0 + a3 * b1;
  m->a2 = a0 * b2 + a2 * b3;
  m->a3 = a1 * b2 + a3 * b3;
  m->a4 = a0 * b4 + a2 * b5 + a4;
  m->a5 = a1 * b4 + a3 * b5 + a5;

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

matrix_t* matrix_transform_point(matrix_t* m, xy_t x, xy_t y, xy_t* ox, xy_t* oy) {
  float x1 = m->a0 * x + m->a2 * y + m->a4;
  float y1 = m->a1 * x + m->a3 * y + m->a5;

  *ox = x1 > 0 ? x1 + 0.5f : x1 - 0.5f;
  *oy = y1 > 0 ? y1 + 0.5f : y1 - 0.5f;

  return m;
}

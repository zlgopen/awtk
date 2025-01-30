/**
 * File:   matrix.c
 * Author: AWTK Develop Team
 * Brief:  matrix
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  /**
   * ⌈1 0 0⌉
   * |0 1 0|
   * ⌊0 0 1⌋
   */
  return matrix_set(m, 1, 0, 0, 1, 0, 0);
}

static inline double matrix_compute_determinant(matrix_t* m) {
  /**
   *    ⌈a c a4⌉   a * det⌈d a5⌉ - c * det⌈b a5⌉ + a4 * det⌈b d⌉
   * det|b d a5| =        ⌊0  1⌋          ⌊0  1⌋           ⌊0 0⌋
   *    ⌊0  0 1⌋
   *             = a *(d*1-a5*0) - c *(b*1-a5*0) + a4 *(b*0-d*0)
   *             = a * d - c * b
   */
  double a = m->a0;
  double b = m->a1;
  double c = m->a2;
  double d = m->a3;

  return a * d - b * c;
}

static inline bool_t matrix_is_invertible_ex(matrix_t* m, float* det) {
  return_value_if_fail(m != NULL, FALSE);

  double det_d = matrix_compute_determinant(m);

  if (det != NULL) {
    *det = det_d;
  }

  return TK_ISFINITE(det_d) && det_d != 0.;
}

bool_t matrix_is_invertible(matrix_t* m) {
  return matrix_is_invertible_ex(m, NULL);
}

matrix_t* matrix_invert(matrix_t* m) {
  return_value_if_fail(m != NULL, NULL);

  float aa = m->a0, ab = m->a1, ac = m->a2, ad = m->a3, atx = m->a4, aty = m->a5;
  float det = 0;
  if (!matrix_is_invertible_ex(m, &det)) {
    return NULL;
  }

  /**
   * Cij  = (-1)^(i+j) * Mij
   * A*   = [Cij]T
   * eg:
   * A   = ⌈a11 a12 a13⌉
   *       |a21 a22 a23|
   *       ⌊a31 a32 a33⌋
   * M11 = det⌈a22 a23⌉ = a22 * a33 - a23 * a32
   *          ⌊a32 a33⌋
   * 
   * ⌈aa ac atx⌉*  ⌈(-1)^(1+1)*(ad  *1- aty*0) (-1)^(2+1)*(ab  *1- aty*0) (-1)^(3+1)*(ab *0- ad*0)⌉T
   * |ab ad aty| = |(-1)^(1+2)*(ac *1- atx*0)  (-1)^(2+2)*(aa *1- atx*0) (-1)^(3+2)*(aa *0- ac*0)|
   * ⌊ 0  0   1⌋   ⌊(-1)^(1+3)*(ac*aty-atx*ad) (-1)^(2+3)*(aa*aty-atx*ab) (-1)^(3+3)*(aa*ad-ac*ab)⌋
   *
   *               ⌈           ad               -ab             0⌉T
   *             = |          -ac                aa             0|
   *               ⌊ac*aty-atx*ad  -(aa*aty-atx*ab)   aa*ad-ac*ab⌋
   * 
   *               ⌈ ad -ac    ac*aty-atx*ad⌉
   *             = |-ab  aa -(aa*aty-atx*ab)|
   *               ⌊ 0   0       aa*ad-ac*ab⌋
   * 
   * ∵ A-1 = A* / det(A)
   * ∴ ⌈aa ac atx⌉-1  ⌈ ad/det -ac/det   (ac*aty-atx*ad)/det⌉
   *   |ab ad aty|  = |-ab/det  aa/det  -(aa*aty-atx*ab)/det|
   *   ⌊ 0  0   1⌋    ⌊      0       0     (aa*ad-ac*ab)/det⌋
   *
   * ∵ det = aa*ad-ac*ab
   * ∴ ⌈a0 a2 a4⌉     ⌈ ad/det -ac/det   (ac*aty-atx*ad)/det⌉
   *   |a1 a3 a5|   = |-ab/det  aa/det  -(aa*aty-atx*ab)/det|
   *   ⌊0  0   1⌋     ⌊      0      0                      1⌋
   */
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
  return_value_if_fail(m != NULL, NULL);

  m->a0 = a0;
  m->a1 = a1;
  m->a2 = a2;
  m->a3 = a3;
  m->a4 = a4;
  m->a5 = a5;

  return m;
}

matrix_t* matrix_multiply(matrix_t* m, matrix_t* b) {
  return_value_if_fail(m != NULL && b != NULL, NULL);

  /**
   * ⌈a0 a2 a4⌉ ⌈b0 b2 b4⌉    ⌈a0*b0+a2*b1+a4*0 a0*b2+a2*b3+a4*0 a0*b4+a2*b5+a4*1⌉   ⌈a0*b0+a2*b1 a0*b2+a2*b3 a0*b4+a2*b5+a4⌉
   * |a1 a3 a5| |b1 b3 b5| = |a1*b0+a3*b1+a5*0 a1*b2+a3*b3+a5*0 a1*b4+a3*b5+a5*1| = |a1*b0+a3*b1 a1*b2+a3*b3 a1*b4+a3*b5+a5|
   * ⌊ 0  0  1⌋ ⌊ 0  0  1⌋    ⌊ 0*b0+ 0*b1+ 1*0  0*b2+ 0*b3+ 1*0  0*b4+ 0*b5+ 1*1⌋   ⌊          0           0              1⌋
   */
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
  return_value_if_fail(m != NULL, NULL);

  /**
   * ⌈1 0 x⌉
   * |0 1 y|
   * ⌊0 0 1⌋
   */
  matrix_t tmp = {1, 0, 0, 1, x, y};

  matrix_multiply(&tmp, m);
  *m = tmp;

  return m;
}

matrix_t* matrix_scale(matrix_t* m, float sx, float sy) {
  return_value_if_fail(m != NULL, NULL);

  /**
   * ⌈Sx  0 0⌉
   * | 0 Sy 0|
   * ⌊ 0  0 1⌋
   */
  matrix_t tmp = {sx, 0, 0, sy, 0, 0};

  matrix_multiply(&tmp, m);
  *m = tmp;

  return m;
}

matrix_t* matrix_rotate(matrix_t* m, float rad) {
  return_value_if_fail(m != NULL, NULL);

  /**
   * ⌈cos -sin 0⌉
   * |sin  cos 0|
   * ⌊  0   0  1⌋
   */
  float s = sin(rad), c = cos(rad);
  matrix_t tmp = {c, s, -s, c, 0, 0};

  matrix_multiply(&tmp, m);
  *m = tmp;

  return m;
}

matrix_t* matrix_transform_point(matrix_t* m, xy_t x, xy_t y, xy_t* ox, xy_t* oy) {
  return_value_if_fail(m != NULL, NULL);

  float x1 = 0, y1 = 0;

  matrix_transform_pointf(m, x, y, &x1, &y1);

  *ox = tk_roundi(x1);
  *oy = tk_roundi(y1);

  return m;
}

matrix_t* matrix_transform_pointf(matrix_t* m, float x, float y, float* ox, float* oy) {
  return_value_if_fail(m != NULL, NULL);

  /**
   * 齐次坐标点(x, y, w=1)
   *
   * ⌈a0 a2 a4⌉ ⌈x⌉    ⌈a0*x+a2*y+a4*1⌉   ⌈a0*x+a2*y+a4⌉
   * |a1 a3 a5| |y| = |a1*x+a3*y+a5*1| = |a1*x+a3*y+a5|
   * ⌊ 0  0  1⌋ ⌊1⌋    ⌊ 0*x+ 0*y+ 1*1⌋   ⌊           1⌋
   */
  *ox = m->a0 * x + m->a2 * y + m->a4;
  *oy = m->a1 * x + m->a3 * y + m->a5;

  return m;
}

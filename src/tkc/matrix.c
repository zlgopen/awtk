/**
 * File:   matrix.c
 * Author: AWTK Develop Team
 * Brief:  matrix
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  /**
   * ⌈1 0 0⌉
   * |0 1 0|
   * ⌊0 0 1⌋
   */
  m->a0 = 1;
  m->a1 = 0;
  m->a2 = 0;
  m->a3 = 1;
  m->a4 = 0;
  m->a5 = 0;

  return m;
}

matrix_t* matrix_invert(matrix_t* m) {
  /**
   *    ⌈aa ac atx⌉   aa * det⌈ad aty⌉ - ac * det⌈ab aty⌉ + atx * det⌈ab ad⌉
   * det|ab ad aty| =         ⌊0    1⌋           ⌊0    1⌋            ⌊0   0⌋
   *    ⌊ 0  0   1⌋
   *                = aa *(ad*1-aty*0) - ac *(ab*1-aty*0) + atx *(ab*0-ad*0)
   *                = aa * ad - ac * ab
   */
  float aa = m->a0, ab = m->a1, ac = m->a2, ad = m->a3;
  float atx = m->a4, aty = m->a5;
  float det = aa * ad - ab * ac;

  if (!det) {
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
   *   |ab ad aty|  = |-ab/det  aa/det -(aa*aty-atx*ab)/det|
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
  m->a0 = a0;
  m->a1 = a1;
  m->a2 = a2;
  m->a3 = a3;
  m->a4 = a4;
  m->a5 = a5;

  return m;
}

matrix_t* matrix_multiply(matrix_t* m, matrix_t* b) {
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
  /**
   * ⌈a0 a2 a4⌉ ⌈1 0 x⌉    ⌈a0*1+a2*0+a4*0 a0*0+a2*1+a4*0 a0*x+a2*y+a4*1⌉   ⌈a0 a2 a0*x+a2*y+a4⌉
   * |a1 a3 a5| |0 1 y| = |a1*1+a3*0+a5*0 a1*0+a3*1+a5*0 a1*x+a3*y+a5*1| = |a1 a3 a1*x+a3*y+a5|
   * ⌊ 0  0  1⌋ ⌊0 0 1⌋    ⌊ 0*1+ 0*0+ 1*0  0*0+ 0*1+ 1*0  0*x+ 0*y+ 1*1⌋   ⌊ 0  0            1⌋
   */
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
  /**
   * ⌈a0 a2 a4⌉ ⌈Sx  0 0⌉    ⌈a0*Sx+a2*0+a4*0 a0*0+a2*Sy+a4*0 a0*0+a2*0+a4*1⌉   ⌈a0*Sx a2*Sy a4⌉
   * |a1 a3 a5| | 0 Sy 0| = |a1*Sx+a3*0+a5*0 a1*0+a3*Sy+a5*0 a1*0+a3*0+a5*1| = |a1*Sx a3*Sy a5|
   * ⌊ 0  0  1⌋ ⌊ 0  0 1⌋    ⌊ 0*Sx+ 0*0+ 1*0  0*0+ 0*Sy+ 1*0  0*0+ 0*0+ 1*1⌋   ⌊    0     0  1⌋
   */
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
  /**
   * ⌈a0 a2 a4⌉ ⌈cos -sin 0⌉    ⌈a0*cos+a2*sin+a4*0 a0*-sin+a2*cos+a4*0 a0*0+a2*0+a4*1⌉   ⌈a0*cos+a2*sin a0*-sin+a2*cos a4⌉
   * |a1 a3 a5| |sin  cos 0| = |a1*cos+a3*sin+a5*0 a1*-sin+a3*cos+a5*0 a1*0+a3*0+a5*1| = |a1*cos+a3*sin a1*-sin+a3*cos a5|
   * ⌊ 0  0  1⌋ ⌊  0   0  1⌋    ⌊   0*cos+0*sin+1*0    0*-sin+0*cos+1*0    0*0+0*0+1*1⌋   ⌊            0              0  1⌋
   */
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

matrix_t* matrix_transform_pointf(matrix_t* m, float x, float y, float* ox, float* oy) {
  /**
   * 齐次坐标点(x, y, w=1)
   *
   * ⌈a0 a2 a4⌉ ⌈x⌉    ⌈a0*x+a2*y+a4*1⌉   ⌈a0*x+a2*y+a4⌉
   * |a1 a3 a5| |y| = |a1*x+a3*y+a5*1| = |a1*x+a3*y+a5|
   * ⌊ 0  0  1⌋ ⌊1⌋    ⌊ 0*x+ 0*y+ 1*1⌋   ⌊           1⌋
   */
  float x1 = m->a0 * x + m->a2 * y + m->a4;
  float y1 = m->a1 * x + m->a3 * y + m->a5;
  *ox = x1;
  *oy = y1;

  return m;
}

static double matrix_compute_determinant(matrix_t* m) {
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

bool_t matrix_is_invertible(matrix_t* m) {
  double det = matrix_compute_determinant(m);

  return TK_ISFINITE(det) && det != 0.;
}

/**
 * File:   matrix.h
 * Author: AWTK Develop Team
 * Brief:  matrix struct and utils functions.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

/**
 * @class matrix_t
 * 2D矩阵对象。
 */
typedef struct _matrix_t {
  float a0;
  float a1;
  float a2;
  float a3;
  float a4;
  float a5;
} matrix_t;

/**
 * @method matrix_init
 * 初始化矩阵对象。
 *
 * @param {matrix_t*} m 矩阵对象。
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_init(matrix_t* m);

/**
 * @method matrix_identity
 * 重置矩阵对象为单位矩阵。
 *
 * @param {matrix_t*} m 矩阵对象。
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_identity(matrix_t* m);

/**
 * @method matrix_invert
 * 求矩阵的逆。
 *
 * @param {matrix_t*} m 矩阵对象。
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_invert(matrix_t* m);

/**
 * @method matrix_set
 * 设置矩阵的参数。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {float} a0 a0
 * @param {float} a1 a1
 * @param {float} a2 a2
 * @param {float} a3 a3
 * @param {float} a4 a4
 * @param {float} a5 a5
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_set(matrix_t* m, float a0, float a1, float a2, float a3, float a4, float a5);

/**
 * @method matrix_multiply
 * 矩阵相乘。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {matrix_t*} b 矩阵对象。
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_multiply(matrix_t* m, matrix_t* b);

/**
 * @method matrix_translate
 * 平移。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {float} x x
 * @param {float} y y
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_translate(matrix_t* m, xy_t x, xy_t y);

/**
 * @method matrix_scale
 * 缩放。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {float} sx sx
 * @param {float} sy sy
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_scale(matrix_t* m, float sx, float sy);

/**
 * @method matrix_rotate
 * 旋转。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {float} rad 角度。
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_rotate(matrix_t* m, float rad);

/**
 * @method matrix_transform_point
 * 对点进行变换。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {xy_t} x x
 * @param {xy_t} y y
 * @param {xy_t*} out_x out_x
 * @param {xy_t*} out_y out_y
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_transform_point(matrix_t* m, xy_t x, xy_t y, xy_t* out_x, xy_t* out_y);

/**
 * @method matrix_transform_pointf
 * 对点进行变换。
 *
 * @param {matrix_t*} m 矩阵对象。
 * @param {float} x x
 * @param {float} y y
 * @param {float*} out_x out_x
 * @param {float*} out_y out_y
 *
 * @return {matrix_t*} 矩阵对象。
 */
matrix_t* matrix_transform_pointf(matrix_t* m, float x, float y, float* ox, float* oy);

/**
 * @method matrix_is_invertible
 * 判断matrix是否可逆。
 *
 * @param {matrix_t*} m 矩阵对象。
 *
 * @return {bool_t} 返回TRUE表示可逆，否则表示不可逆。
 */
bool_t matrix_is_invertible(matrix_t* m);

END_C_DECLS

#endif /*TK_MATRIX_H*/

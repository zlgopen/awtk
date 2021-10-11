
/**
 * File: vg_gradient.h   
 * Author: AWTK Develop Team
 * Brief:  vector graphic gradient
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VG_GRADIENT_H
#define TK_VG_GRADIENT_H

#include "tkc/rect.h"
#include "tkc/color.h"
#include "base/gradient.h"

BEGIN_C_DECLS

struct _vg_gradient_t;
typedef struct _vg_gradient_t vg_gradient_t;

/**
 * @class vg_gradient_radial_info_t
 * 放射渐变。
 */
typedef struct _vg_gradient_radial_info_t {
  /**
   * @property {float} x0
   * @annotation ["readable"]
   * 起始圆的x坐标。
   */
  float x0;
  /**
   * @property {float} y0
   * @annotation ["readable"]
   * 起始圆的y坐标。
   */
  float y0;
  /**
   * @property {float} r0
   * @annotation ["readable"]
   * 起始圆的半径。
   */
  float r0;

  /**
   * @property {float} x1
   * @annotation ["readable"]
   * 结束圆的x坐标。
   */
  float x1;
  /**
   * @property {float} y1
   * @annotation ["readable"]
   * 结束圆的y坐标。
   */
  float y1;
  /**
   * @property {float} r1
   * @annotation ["readable"]
   * 结束圆的半径。
   */
  float r1;
} vg_gradient_radial_info_t;

/**
 * @class vg_gradient_linear_info_t
 * 线性渐变。
 */
typedef struct _vg_gradient_linear_info_t {
  /**
   * @property {float} sx
   * @annotation ["readable"]
   * 起始点x坐标。
   */
  float sx;
  /**
   * @property {float} sy
   * @annotation ["readable"]
   * 起始点y坐标。
   */
  float sy;
  /**
   * @property {float} ex
   * @annotation ["readable"]
   * 结束点x坐标。
   */
  float ex;
  /**
   * @property {float} ey
   * @annotation ["readable"]
   * 结束点y坐标。
   */
  float ey;
} vg_gradient_linear_info_t;

/**
 * @class vg_gradient_t
 * 渐变。
 */
struct _vg_gradient_t {
  gradient_t gradient;

  /**
   * @property {vg_gradient_info_t} info
   * @annotation ["readable"]
   * 信息。
   */
  union vg_gradient_info_t {
    vg_gradient_linear_info_t linear;
    vg_gradient_radial_info_t radial;
  } info;
};

/**
 * @method vg_gradient_create_radial
 * 创建放射渐变。
 * @param {float} x0 起始圆的x坐标。
 * @param {float} y0 起始圆的y坐标。
 * @param {float} r0 起始圆的半径。
 * @param {float} x1 结束圆的x坐标。
 * @param {float} y1 结束圆的y坐标。
 * @param {float} r1 结束圆的半径。
 *
 * @return {vg_gradient_t*} 返回gradient对象。
 */
vg_gradient_t* vg_gradient_create_radial(float x0, float y0, float r0, float x1, float y1,
                                         float r1);

/**
 * @method vg_gradient_init_radial
 * 初始化放射渐变。
 * @param {vg_gradient_t*} gradient gradient对象。
 * @param {float} x0 起始圆的x坐标。
 * @param {float} y0 起始圆的y坐标。
 * @param {float} r0 起始圆的半径。
 * @param {float} x1 结束圆的x坐标。
 * @param {float} y1 结束圆的y坐标。
 * @param {float} r1 结束圆的半径。
 *
 * @return {vg_gradient_t*} 返回gradient对象。
 */
vg_gradient_t* vg_gradient_init_radial(vg_gradient_t* gradient, float x0, float y0, float r0,
                                       float x1, float y1, float r1);

/**
 * @method vg_gradient_init_with_gradient
 * 初始化矢量画布的渐变对象。
 * @param {vg_gradient_t*} gradient gradient对象。
 * @param {const rect_t*} rect 矩形区域。
 * @param {const gradient_t*} g 渐变对象。
 *
 * @return {vg_gradient_t*} 返回gradient对象。
 */
vg_gradient_t* vg_gradient_init_with_gradient(vg_gradient_t* gradient, const rect_t* rect,
                                              const gradient_t* g);

/**
 * @method vg_gradient_create_linear
 * 创建线性渐变。
 *
 * @param {float} sx 起始点x坐标。
 * @param {float} sy 起始点y坐标。
 * @param {float} ex 结束点x坐标。
 * @param {float} ey 结束点y坐标。
 *
 * @return {vg_gradient_t*} 返回gradient对象。
 */
vg_gradient_t* vg_gradient_create_linear(float sx, float sy, float ex, float ey);

/**
 * @method vg_gradient_init_linear
 * 初始化线性。
 * @param {vg_gradient_t*} gradient gradient对象。
 * @param {float} sx 起始点x坐标。
 * @param {float} sy 起始点y坐标。
 * @param {float} ex 结束点x坐标。
 * @param {float} ey 结束点y坐标。
 *
 * @return {vg_gradient_t*} 返回gradient对象。
 */
vg_gradient_t* vg_gradient_init_linear(vg_gradient_t* gradient, float sx, float sy, float ex,
                                       float ey);

/**
 * @method vg_gradient_destroy
 * 销毁gradient对象。
 *
 * @param {vg_gradient_t*} gradient gradient对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t vg_gradient_destroy(vg_gradient_t* gradient);

#define vg_gradient_add_stop(gradient, color, stop) \
  gradient_add_stop((gradient_t*)(gradient), color, stop)
#define vg_gradient_get_stop(gradient, index) gradient_get_stop((gradient_t*)(gradient), index)
#define vg_gradient_get_first_color(gradient) gradient_get_first_color((gradient_t*)(gradient))
#define vg_gradient_get_last_color(gradient) gradient_get_last_color((gradient_t*)(gradient))

END_C_DECLS

#endif /*TK_VG_GRADIENT_H*/

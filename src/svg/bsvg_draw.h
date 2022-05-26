/**
 * File:   bsvg_draw.h
 * Author: AWTK Develop Team
 * Brief:  bsvg_draw
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
 * 2018-11-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BSVG_DRAW_H
#define TK_BSVG_DRAW_H

#include "svg/bsvg.h"
#include "base/vgcanvas.h"

BEGIN_C_DECLS

typedef struct _bsvg_draw_ctx_t {
  float x;
  float y;
  bsvg_t* bsvg;
  vgcanvas_t* canvas;
  const svg_shape_t* shape;

  /*for S/S_REL*/
  float last_x2;
  float last_y2;
  uint8_t last_type;

  /*for T/T_REL*/
  float last_x1;
  float last_y1;
} bsvg_draw_ctx_t;

/**
 * @class bsvg_t
 * @annotation ["fake"]
 */

/**
 * @method bsvg_draw
 *
 * 绘制bsvg。
 *
 * @param {bsvg_t*} svg SVG对象。
 * @param {vgcanvas_t*} canvas vgcanvas对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bsvg_draw(bsvg_t* svg, vgcanvas_t* canvas);

/**
 * @method bsvg_draw_path
 *
 * 绘制bsvg路径。
 *
 * @param {bsvg_draw_ctx_t*} ctx 绘制上下文。
 * @param {svg_path_t*} path path对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t bsvg_draw_path(bsvg_draw_ctx_t* ctx, const svg_path_t* path);

END_C_DECLS

#endif /*TK_BSVG_DRAW_H*/

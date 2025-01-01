/**
 * File:   vgcanvas_nanovg_gl.h
 * Author: AWTK Develop Team
 * Brief:  vgcanvas_nanovg_gl 
 *
 * Copyright (c) 2024 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#ifndef TK_VG_NANOVG_GL_H
#define TK_VG_NANOVG_GL_H

#include "base/vgcanvas.h"

BEGIN_C_DECLS

/**
 * @class vgcanvas_nanovg_gl_t
 * @annotation ["fake"]
 * 主要用于导出vgcanvas_nanovg_glx函数。
 */

/**
 * @method vgcanvas_create_gl2
 * 创建一个基于nanovg的vgcanvas对象。
 * @annotation ["static"]
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @param {uint32_t} stride 跨度。
 * @param {bitmap_format_t} format 格式。
 * @param {void*} win 窗口句柄。
 *
 * @return {vgcanvas_t*} 返回vgcanvas对象。
 */
vgcanvas_t* vgcanvas_create_gl2(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                                void* win);

/**
 * @method vgcanvas_create_gl3
 * 创建一个基于nanovg的vgcanvas对象。
 * @annotation ["static"]
 * @param {uint32_t} w 宽度。
 * @param {uint32_t} h 高度。
 * @param {uint32_t} stride 跨度。
 * @param {bitmap_format_t} format 格式。
 * @param {void*} win 窗口句柄。
 *
 * @return {vgcanvas_t*} 返回vgcanvas对象。
 */
vgcanvas_t* vgcanvas_create_gl3(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                                void* win);

END_C_DECLS

#endif /*TK_VG_NANOVG_GL_H*/

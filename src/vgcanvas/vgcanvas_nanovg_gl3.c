/**
 * File:   vgcanvas_nanovg_gl.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-gl
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#undef WITH_NANOVG_GL2
#undef WITH_NANOVG_GLES2
#undef WITH_NANOVG_GLES3

#define WITH_NANOVG_GL3 1
#define VGCANVAS_CREATE_GL vgcanvas_create_gl3

#if !defined(WITH_GPU)
#include "base/vgcanvas.h"
vgcanvas_t* VGCANVAS_CREATE_GL(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                               void* win) {
  return NULL;
}
#else
#include "vgcanvas_nanovg_gl_common.inc"
#endif /*WIN32*/

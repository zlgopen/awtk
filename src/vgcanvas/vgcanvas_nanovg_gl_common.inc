/**
 * File:   vgcanvas_nanovg_gl.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-gl
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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


#include "base/system_info.h"
#include "base/vgcanvas_asset_manager.h"

#if defined(WITH_NANOVG_GL3)
#define NANOVG_GL3_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GL2)
#define NANOVG_GL2_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES2)
#define NANOVG_GLES2_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES3)
#define NANOVG_GLES3_IMPLEMENTATION
#endif

#ifdef WITH_NANOVG_PLUS_GPU
#include "nanovg_plus.h"
#else
#include "nanovg.h"
#endif

#include "tkc/utf8.h"
#include "tkc/mem.h"
#include "base/opengl.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"
#include "base/native_window.h"

#ifdef WITH_NANOVG_PLUS_GPU
#include "nanovg_plus_gl.h"
#define vgcanvas_nanovg_init vgcanvas_nanovg_plus_init
#else
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"
#endif

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
#ifdef WITH_NANOVG_PLUS_GPU
  nvgp_context_t* vg;
#else
  NVGcontext* vg;
#endif
  uint32_t text_align_v;
  uint32_t text_align_h;

  native_window_t* window;
} vgcanvas_nanovg_t;

#ifdef WITH_NANOVG_PLUS_GPU
#include "vgcanvas_nanovg_plus.h"
#else
#include "texture.inc"
#include "vgcanvas_nanovg_gl.inc"
#include "vgcanvas_asset_manager_nanovg.inc"
#endif

vgcanvas_t* VGCANVAS_CREATE_GL(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* win) {
  int32_t flags = 0;
  native_window_info_t info;
  native_window_t* window = NATIVE_WINDOW(win);
  return_value_if_fail(native_window_get_info(win, &info) == RET_OK, NULL);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->window = window;
  nanovg->base.ratio = info.ratio;

#if defined(WITH_NANOVG_PLUS_GPU)
#if !defined(WITH_ANTIALIAS) || defined(WITH_OPENGL_HW_ANTIALIAS)
  flags = NVGP_GL_FLAG_STENCIL_STROKES;
  if (!window->supported_opengl_antialias_hw) {
    flags |= NVGP_GL_FLAG_ANTIALIAS;
  }
#else
  flags = NVGP_GL_FLAG_ANTIALIAS | NVGP_GL_FLAG_STENCIL_STROKES;
#endif
  nanovg->base.vt = vgcanvas_nanovg_plus_get_vt();
#else
#if !defined(WITH_ANTIALIAS) || defined(WITH_OPENGL_HW_ANTIALIAS)
  flags = NVG_STENCIL_STROKES;
  if (!window->supported_opengl_antialias_hw) {
    flags |= NVG_ANTIALIAS;
  }
#else
  flags = NVG_ANTIALIAS | NVG_STENCIL_STROKES;
#endif
  nanovg->base.vt = &vt;
#endif

  vgcanvas_nanovg_init((vgcanvas_t*)nanovg);

  opengl_init();

#if defined(WITH_NANOVG_PLUS_GPU)
  nanovg->vg = nvgp_create(NVGP_MODE_GPU, w, h, flags);
#elif defined(WITH_NANOVG_GL2)
  nanovg->vg = nvgCreateGL2(flags);
#elif defined(WITH_NANOVG_GL3)
  nanovg->vg = nvgCreateGL3(flags);
#elif defined(WITH_NANOVG_GLES2)
  nanovg->vg = nvgCreateGLES2(flags);
#elif defined(WITH_NANOVG_GLES3)
  nanovg->vg = nvgCreateGLES3(flags);
#endif

  if (nanovg->vg == NULL) {
    assert(!"OpenGL is not supported!");
  }

#if defined(WITH_NANOVG_PLUS_GPU)
  vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), &(nanovg->base),
                                vgcanvas_asset_manager_nanovg_plus_bitmap_destroy,
                                vgcanvas_asset_manager_nanovg_plus_font_destroy);
#else
  vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), &(nanovg->base),
                                vgcanvas_asset_manager_nanovg_bitmap_destroy,
                                vgcanvas_asset_manager_nanovg_font_destroy);
#endif

  return &(nanovg->base);
}

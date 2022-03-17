/**
 * File:   vgcanvas_nanovg_gl.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-gl
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
 * 2018-04-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"

#if defined(WITH_NANOVG_GL3)
#define NANOVG_GL3_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES2)
#define NANOVG_GLES2_IMPLEMENTATION
#endif

#if defined(WITH_NANOVG_GLES3)
#define NANOVG_GLES3_IMPLEMENTATION
#endif

#ifdef WITHOUT_GLAD
#include <SDL.h>

#ifdef IOS
#include <OpenGLES/gltypes.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#endif /*IOS*/

#else
#include "glad/glad.h"
#endif /*WITHOUT_GLAD*/

#include "nanovg.h"
#include "tkc/utf8.h"
#include "tkc/mem.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"
#include "base/native_window.h"

#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;

  native_window_t* window;
} vgcanvas_nanovg_t;

typedef struct _vgcanvas_nanovg_gl_texture_t {
  int32_t image_id;
  framebuffer_object_t* fbo;
} vgcanvas_nanovg_gl_texture_t;

#include "texture.inc"
#include "vgcanvas_nanovg_gl.inc"

static ret_t vgcanvas_asset_manager_nanovg_font_destroy(void* vg, const char* font_name,
                                                        void* specific) {
  int32_t id = tk_pointer_to_int(specific);
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  if (canvas != NULL && canvas->vg != NULL && id >= 0) {
    nvgDeleteFontByName(canvas->vg, font_name);
  }
  return RET_OK;
}

static ret_t vgcanvas_asset_manager_nanovg_bitmap_destroy(void* vg, void* specific) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  vgcanvas_nanovg_gl_texture_t* texture = (vgcanvas_nanovg_gl_texture_t*)specific;
  if (canvas != NULL && canvas->vg != NULL && texture != NULL) {
    if (texture->fbo != NULL) {
      vgcanvas_destroy_fbo((vgcanvas_t*)canvas, texture->fbo);
      TKMEM_FREE(texture->fbo);
    } else {
      nvgDeleteImage(canvas->vg, texture->image_id);
    }
  }
  if (texture) {
    TKMEM_FREE(texture);
  }
  return RET_OK;
}

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* win) {
  native_window_info_t info;
  native_window_t* window = NATIVE_WINDOW(win);
  return_value_if_fail(native_window_get_info(win, &info) == RET_OK, NULL);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->window = window;
  nanovg->base.ratio = info.ratio;

  vgcanvas_nanovg_init((vgcanvas_t*)nanovg);

#if defined(WITH_NANOVG_GL3)
  nanovg->vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES2)
  nanovg->vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif defined(WITH_NANOVG_GLES3)
  nanovg->vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif

  if (nanovg->vg == NULL) {
    assert(!"OpenGL is not supported!");
  }

  vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), &(nanovg->base),
                                vgcanvas_asset_manager_nanovg_bitmap_destroy,
                                vgcanvas_asset_manager_nanovg_font_destroy);
  return &(nanovg->base);
}

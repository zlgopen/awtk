/**
 * File:   vgcanvas_nanovg_bgfx.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-bgfx
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "nanovg.h"
#include "tkc/mem.h"
#include "base/vgcanvas.h"
#include "bgfx/nanovg_bgfx_ex.h"
#include "base/image_manager.h"

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;
  SDL_Window* sdl_window;
} vgcanvas_nanovg_t;

#include "texture.inc"
#include "vgcanvas_nanovg_bgfx.inc"
#include "vgcanvas_nanovg.inc"

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* sdl_window) {
  int ww = 0;
  int wh = 0;
  int fw = 0;
  int fh = 0;
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  (void)format;
  (void)stride;
  SDL_GetWindowSize((SDL_Window*)sdl_window, &ww, &wh);
  SDL_GL_GetDrawableSize((SDL_Window*)sdl_window, &fw, &fh);

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->base.ratio = (float)fw / (float)ww;
  nanovg->sdl_window = (SDL_Window*)sdl_window;
  nanovg->vg = nvgCreateBGFX(1, 0, fw, fh, nanovg->sdl_window);

  if (nanovg->vg == NULL) {
    assert(!"BGFX is not supported!");
  }

  log_debug("use BGFX backend\n");

  return &(nanovg->base);
}

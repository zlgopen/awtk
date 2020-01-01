/**
 * File:   vgcanvas_nanovg_bgfx.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-bgfx
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/native_window.h"

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;
  native_window_t* window;
} vgcanvas_nanovg_t;

#include "texture.inc"
#include "vgcanvas_nanovg_bgfx.inc"
#include "vgcanvas_nanovg.inc"

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* win) {
  native_window_info_t info;
  native_window_t* window = NATIVE_WINDOW(win);
  return_value_if_fail(native_window_get_info(win, &info) == RET_OK, NULL);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->window = window;
  nanovg->base.ratio = info.ratio;

  nanovg->vg = nvgCreateBGFX(1, 0, fw, fh, info->handle);

  if (nanovg->vg == NULL) {
    assert(!"BGFX is not supported!");
  }

  log_debug("use BGFX backend\n");

  return &(nanovg->base);
}

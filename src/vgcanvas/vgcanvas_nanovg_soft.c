/**
 * File:   vgcanvas_nanovg_soft.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on nanovg-soft
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
#include "tkc/utf8.h"
#include "tkc/mem.h"
#include "base/vgcanvas.h"
#include "base/image_manager.h"

#ifdef WITH_NANOVG_AGGE
#include "agge/nanovg_agge.h"
#elif defined(WITH_NANOVG_AGG)
#include "agg/nanovg_agg.h"
#endif /*WITH_NANOVG_AGGE|WITH_NANOVG_AGG*/

typedef struct _vgcanvas_nanovg_t {
  vgcanvas_t base;

  int font_id;
  NVGcontext* vg;
  uint32_t text_align_v;
  uint32_t text_align_h;
} vgcanvas_nanovg_t;

#include "vgcanvas_nanovg_soft.inc"
#include "vgcanvas_nanovg.inc"

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* data) {
  enum NVGtexture f = bitmap_format_to_nanovg(format);
  vgcanvas_nanovg_t* nanovg = (vgcanvas_nanovg_t*)TKMEM_ZALLOC(vgcanvas_nanovg_t);
  return_value_if_fail(nanovg != NULL, NULL);

  nanovg->base.w = w;
  nanovg->base.h = h;
  nanovg->base.vt = &vt;
  nanovg->base.ratio = 1;
  nanovg->base.format = format;
  nanovg->base.stride = stride;
  nanovg->base.buff = (uint32_t*)data;

#if defined(WITH_NANOVG_AGG)
  nanovg->vg = nvgCreateAGG(w, h, stride, f, (uint8_t*)data);
#elif defined(WITH_NANOVG_AGGE)
  nanovg->vg = nvgCreateAGGE(w, h, stride, f, (uint8_t*)data);
#else
  assert(!"not support backend");
#endif

  return &(nanovg->base);
}

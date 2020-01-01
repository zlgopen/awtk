/**
 * File:   vgcanvas.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on agg
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
 * 2018-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/vgcanvas.h"

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* data) {
  (void)w;
  (void)h;
  (void)format;
  (void)data;
  return NULL;
}

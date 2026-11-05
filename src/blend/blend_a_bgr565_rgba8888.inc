/**
 * File:   blend_a_bgr565_rgba8888.inc
 * Author: AWTK Develop Team
 * Brief:  blend a(rgba8888) on bgr565
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-04-18 Li XianJing <xianjimli@hotmail.com> created
 * 2026-11-03 从 blend_image_bgr565_rgba8888.c 中抽离出来
 */

/* 手工维护，gen.sh不会覆盖本文件 */

#include "tkc/types_def.h"
#include "base/pixel.h"
#include "blend_a_bgr565_common.inc"

static inline void blend_a_bgr565_rgba8888(uint8_t* dst, uint8_t* src, uint8_t alpha,
                                           bool_t premulti_alpha) {
  pixel_rgba8888_t s = *(pixel_rgba8888_t*)src;
  rgba_t rgba = pixel_rgba8888_to_rgba(s);

  blend_a_bgr565_common(dst, rgba, alpha, premulti_alpha);
}

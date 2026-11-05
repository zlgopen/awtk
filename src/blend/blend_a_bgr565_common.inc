/**
 * File:   blend_a_bgr565_common.inc
 * Author: AWTK Develop Team
 * Brief:  blend a(rgba) on bgr565
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
 * 2026-11-05 从 blend_a_bgr565_bgra8888.inc 和 blend_a_bgr565_rgba8888.inc 中抽离出公共实现
 */

/* 手工维护，gen.sh不会覆盖本文件 */

#include "tkc/types_def.h"
#include "base/pixel.h"
#include "base/pixel_pack_unpack.h"

static inline void blend_a_bgr565_common(uint8_t* dst, rgba_t rgba, uint8_t alpha,
                                         bool_t premulti_alpha) {
  if (alpha <= PIXEL_ALPHA_OPAQUE) {
    rgba.a = (rgba.a * alpha) >> 8;
  }

  if (rgba.a > PIXEL_ALPHA_OPAQUE) {
    *(uint16_t*)dst = rgb_to_bgr565(rgba.r, rgba.g, rgba.b);
  } else if (rgba.a > PIXEL_ALPHA_TRANSPARENT) {
    if (premulti_alpha) {
      if (alpha <= PIXEL_ALPHA_OPAQUE) {
        rgba.r = (rgba.r * alpha) >> 8;
        rgba.g = (rgba.g * alpha) >> 8;
        rgba.b = (rgba.b * alpha) >> 8;
      }
      rgba.a = 0xff - rgba.a; /* rgba.a 此时是背景的权重 */
      pixel_bgr565_blend_rgba_premulti(dst, rgba);
    } else {
      pixel_bgr565_blend_rgba(dst, rgba);
    }
  }
}

/**
 * File:   blend_a_bgr565_bgra8888.inc
 * Author: AWTK Develop Team
 * Brief:  blend a(bgra8888) on bgr565
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
 * 2026-11-03 从 blend_image_bgr565_bgra8888.c 中抽离出来
 */

/**
 * 本文件由手工维护(gen.sh不会覆盖它)
 * 存在本文件时，gen.sh生成的 blend_image_bgr565_bgra8888.c 会包含它
 * 并且定义 blend_a 指向本文件中的实现
 */

#include "tkc/types_def.h"
#include "base/pixel.h"

static inline void blend_a_bgr565_bgra8888(uint8_t* dst, uint8_t* src, uint8_t alpha,
                                           bool_t premulti_alpha) {
  uint32_t color = *(uint32_t*)src;
  uint8_t sa = color >> 24;
  uint8_t sr = color >> 16;
  uint8_t sg = color >> 8;
  uint8_t sb = color & 0xff;
  uint8_t a = alpha > 0xf8 ? sa : ((sa * alpha) >> 8);

  if (a > 0xf8) {
    *(uint16_t*)dst = ((sr >> 3) << 11) | ((sg >> 2) << 5) | (sb >> 3);
  } else if (a > 8) {
    rgba_t rgba = {.a = a, .r = sr, .g = sg, .b = sb};
    if (premulti_alpha) {
      rgba.a = 0xff - a;
      if (alpha <= 0xf8) {
        rgba.r = (sr * alpha) >> 8;
        rgba.g = (sg * alpha) >> 8;
        rgba.b = (sb * alpha) >> 8;
      }
      pixel_bgr565_blend_rgba_premulti(dst, rgba);
    } else {
      pixel_bgr565_blend_rgba(dst, rgba);
    }
  }
}

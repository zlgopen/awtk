/**
 * File:   blend_a_bgr565_bgr565.inc
 * Author: AWTK Develop Team
 * Brief:  blend a(bgr565) on bgr565
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
 * 2026-11-03 从 blend_image_bgr565_bgr565.c 中抽离出来
 */

/* 手工维护，gen.sh不会覆盖本文件 */

#include "tkc/types_def.h"
#include "base/pixel.h"
#include "base/pixel_pack_unpack.h"

static inline void blend_a_bgr565_bgr565(uint8_t* dst, uint8_t* src, uint8_t a,
                                         bool_t premulti_alpha) {
  if (a > PIXEL_ALPHA_OPAQUE) {
    *(uint16_t*)dst = *(uint16_t*)src;
  } else if (a > PIXEL_ALPHA_TRANSPARENT) {
    uint8_t minus_a = 0xff - a;
    uint16_t sc = *(uint16_t*)src;
    uint16_t dc = *(uint16_t*)dst;
    *(uint16_t*)dst = rgb_sum_to_bgr565(bgr565_to_r(sc) * a + bgr565_to_r(dc) * minus_a,
                                        bgr565_to_g(sc) * a + bgr565_to_g(dc) * minus_a,
                                        bgr565_to_b(sc) * a + bgr565_to_b(dc) * minus_a);
  }
}

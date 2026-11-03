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

/**
 * 本文件由手工维护(gen.sh不会覆盖它)
 * 存在本文件时，gen.sh生成的 blend_image_bgr565_bgr565.c 会包含它
 * 并且定义 blend_a 指向本文件中的实现
 */

#include "tkc/types_def.h"

static inline void blend_a_bgr565_bgr565(uint8_t* dst, uint8_t* src, uint8_t a,
                                         bool_t premulti_alpha) {
  if (a > 0xf8) {
    *(uint16_t*)dst = *(uint16_t*)src;
  } else if (a > 8) {
    uint8_t minus_a = 0xff - a;
    uint16_t sc = *(uint16_t*)src;
    uint16_t dc = *(uint16_t*)dst;

    uint8_t r = (((sc >> 11) << 3) * a + ((dc >> 11) << 3) * minus_a) >> 11;
    uint8_t g = ((((sc >> 5) << 2) & 0xff) * a + ((((dc >> 5) << 2) & 0xff) * minus_a)) >> 10;
    uint8_t b = ((((sc & 0x1f) << 3)) * a + ((((dc & 0x1f) << 3) & 0xff) * minus_a)) >> 11;
    *(uint16_t*)dst = (r << 11) | (g << 5) | b;
  }
}

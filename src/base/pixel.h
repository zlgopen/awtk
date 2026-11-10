/**
 * File:   pixel.h
 * Author: AWTK Develop Team
 * Brief:  pixel format definitions
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
 * 2018-10-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PIXEL_H
#define TK_PIXEL_H

#include "tkc/color.h"
#include "base/pixel_pack_unpack.h"

/**
 * @prefix PIXEL_ALPHA_
 * alpha 快速通道阈值
 */
enum {
  /**
   * @const PIXEL_ALPHA_SRC_OPAQUE
   * 源(或整体) alpha 大于它，视为完全不透明，可直接覆盖目标，不做混合。
   */
  PIXEL_ALPHA_SRC_OPAQUE = 0xf8,
  /**
   * @const PIXEL_ALPHA_COMPOSITE_OPAQUE
   * 参与合成的 alpha 大于它，合成结果 alpha 视为 255，可跳过归一化(除法)
   */
  PIXEL_ALPHA_COMPOSITE_OPAQUE = 0xf4,
  /**
   * @const PIXEL_ALPHA_TRANSPARENT
   * alpha 小于等于它，视为全透明，可直接跳过
   */
  PIXEL_ALPHA_TRANSPARENT = 0x08,
};

static inline uint8_t tk_pixel_limit_uint8(int32_t tmp) {
  return (uint8_t)tk_clamp(tmp, 0, 0xff);
}

/* 前景叠到背景上之后的不透明度：sa + da * (1 - sa/256)，即 sa + da - ((sa*da) >> 8) */
static inline uint8_t pixel_out_alpha(uint8_t sa, uint8_t da) {
  int32_t alpha = sa + da - ((sa * da) >> 8);
  return tk_pixel_limit_uint8(alpha);
}

/**
 * 按 rgba 逻辑通道做像素运算，供各 8888 格式复用
 * （字节顺序由 pixel_xxx8888_to_rgba / _from_rgba 处理）
 */

/* 整体收缩 alpha（变暗） */
static inline rgba_t rgba_blend_dark(rgba_t c, uint8_t a) {
  if (c.a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    c.r = (c.r * a) >> 8;
    c.g = (c.g * a) >> 8;
    c.b = (c.b * a) >> 8;
    c.a = 0xff;
  } else {
    uint8_t out_a = pixel_out_alpha(0xff - a, c.a);
    if (out_a > 0) {
      c.r = (c.r * out_a) >> 8;
      c.g = (c.g * out_a) >> 8;
      c.b = (c.b * out_a) >> 8;
    }
    c.a = out_a;
  }

  return c;
}

/* 预乘源色叠加到背景上 */
static inline rgba_t rgba_blend_premulti(rgba_t c, rgba_t s, uint8_t a) {
  if (c.a > PIXEL_ALPHA_COMPOSITE_OPAQUE) {
    c.r = ((c.r * a) >> 8) + s.r;
    c.g = ((c.g * a) >> 8) + s.g;
    c.b = ((c.b * a) >> 8) + s.b;
    c.a = 0xff;
  } else {
    uint8_t out_a = pixel_out_alpha(0xff - a, c.a);
    if (out_a > 0) {
      uint8_t d_a = (c.a * a) >> 8;
      int32_t r = (c.r * d_a + (s.r << 8)) / out_a;
      int32_t g = (c.g * d_a + (s.g << 8)) / out_a;
      int32_t b = (c.b * d_a + (s.b << 8)) / out_a;
      c.r = tk_pixel_limit_uint8(r);
      c.g = tk_pixel_limit_uint8(g);
      c.b = tk_pixel_limit_uint8(b);
    }
    c.a = out_a;
  }

  return c;
}

#pragma pack(push, 1)
typedef struct _pixel_rgb565_t {
  uint16_t r : 5;
  uint16_t g : 6;
  uint16_t b : 5;
} pixel_rgb565_t;

#define pixel_rgb565_BPP 2
#define pixel_rgb565_a(p) 0xff
#define pixel_rgb565_format BITMAP_FMT_RGB565
#define pixel_rgb565_to_rgba(p) \
  {(uint8_t)((p).r << 3), (uint8_t)((p).g << 2), (uint8_t)((p).b << 3), 0xff}

#define pixel_rgb565_from_rgb(r, g, b) {(r) >> 3, (g) >> 2, (b) >> 3}

#if WITH_LCD_CLEAR_ALPHA
#define pixel_rgb565_from_rgba(r, g, b, a) {((r) * (a)) >> 11, ((g) * (a)) >> 10, ((b) * (a)) >> 11}
#else
#define pixel_rgb565_from_rgba(r, g, b, a) pixel_rgb565_from_rgb(r, g, b)
#endif

static inline void pixel_rgb565_blend_rgba_dark(void* p, uint8_t a) {
  uint16_t v = *(uint16_t*)p;
  *(uint16_t*)p = rgb_sum_to_rgb565(rgb565_to_r(v) * a, rgb565_to_g(v) * a, rgb565_to_b(v) * a);
}

static inline void pixel_rgb565_blend_rgba_premulti(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint16_t v = *(uint16_t*)p;
  *(uint16_t*)p =
      rgb_sum_to_rgb565(rgb565_to_r(v) * a + (rgba.r << 8), rgb565_to_g(v) * a + (rgba.g << 8),
                        rgb565_to_b(v) * a + (rgba.b << 8));
}

typedef struct _pixel_bgr565_t {
  uint16_t b : 5;
  uint16_t g : 6;
  uint16_t r : 5;
} pixel_bgr565_t;

#define pixel_bgr565_BPP 2
#define pixel_bgr565_a(p) 0xff
#define pixel_bgr565_format BITMAP_FMT_BGR565
#define pixel_bgr565_to_rgba(p) {(p).r << 3, (p).g << 2, (p).b << 3, 0xff}

#define pixel_bgr565_from_rgb(r, g, b) {(b) >> 3, (g) >> 2, (r) >> 3}

#if WITH_LCD_CLEAR_ALPHA
#define pixel_bgr565_from_rgba(r, g, b, a) {((b) * (a)) >> 11, ((g) * (a)) >> 10, ((r) * (a)) >> 11}
#else
#define pixel_bgr565_from_rgba(r, g, b, a) pixel_bgr565_from_rgb(r, g, b)
#endif

static inline void pixel_bgr565_blend_rgba_dark(void* p, uint8_t a) {
  uint16_t v = *(uint16_t*)p;
  *(uint16_t*)p = rgb_sum_to_bgr565(bgr565_to_r(v) * a, bgr565_to_g(v) * a, bgr565_to_b(v) * a);
}

static inline void pixel_bgr565_blend_rgba(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t minus_a = 0xff - a;
  uint16_t v = *(uint16_t*)p;
  *(uint16_t*)p = rgb_sum_to_bgr565(bgr565_to_r(v) * minus_a + (rgba.r * a),
                                    bgr565_to_g(v) * minus_a + (rgba.g * a),
                                    bgr565_to_b(v) * minus_a + (rgba.b * a));
}

static inline void pixel_bgr565_blend_rgba_premulti(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint16_t v = *(uint16_t*)p;
  *(uint16_t*)p =
      rgb_sum_to_bgr565(bgr565_to_r(v) * a + (rgba.r << 8), bgr565_to_g(v) * a + (rgba.g << 8),
                        bgr565_to_b(v) * a + (rgba.b << 8));
}

typedef struct _pixel_rgb888_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pixel_rgb888_t;

#define pixel_rgb888_BPP 3
#define pixel_rgb888_a(p) 0xff
#define pixel_rgb888_format BITMAP_FMT_RGB888
#define pixel_rgb888_to_rgba(p) {(p).r, (p).g, (p).b, 0xff}

#define pixel_rgb888_from_rgb(r, g, b) {r, g, b}

#if WITH_LCD_CLEAR_ALPHA
#define pixel_rgb888_from_rgba(r, g, b, a) {((r) * (a)) >> 8, ((g) * (a)) >> 8, ((b) * (a)) >> 8}
#else
#define pixel_rgb888_from_rgba(r, g, b, a) pixel_rgb888_from_rgb(r, g, b)
#endif

static inline void pixel_rgb888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  p[0] = (p[0] * a) >> 8;
  p[1] = (p[1] * a) >> 8;
  p[2] = (p[2] * a) >> 8;
}

static inline void pixel_rgb888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  p[0] = ((p[0] * a) >> 8) + rgba.r;
  p[1] = ((p[1] * a) >> 8) + rgba.g;
  p[2] = ((p[2] * a) >> 8) + rgba.b;
}

typedef struct _pixel_bgr888_t {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} pixel_bgr888_t;

#define pixel_bgr888_BPP 3
#define pixel_bgr888_a(p) 0xff
#define pixel_bgr888_format BITMAP_FMT_BGR888
#define pixel_bgr888_to_rgba(p) {(p).r, (p).g, (p).b, 0xff}

#define pixel_bgr888_from_rgb(r, g, b) {b, g, r}
#if WITH_LCD_CLEAR_ALPHA
#define pixel_bgr888_from_rgba(r, g, b, a) {((b) * (a)) >> 8, ((g) * (a)) >> 8, ((r) * (a)) >> 8}
#else
#define pixel_bgr888_from_rgba(r, g, b, a) pixel_bgr888_from_rgb(r, g, b)
#endif

static inline void pixel_bgr888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  p[0] = (p[0] * a) >> 8;
  p[1] = (p[1] * a) >> 8;
  p[2] = (p[2] * a) >> 8;
}

static inline void pixel_bgr888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  p[0] = ((p[0] * a) >> 8) + rgba.b;
  p[1] = ((p[1] * a) >> 8) + rgba.g;
  p[2] = ((p[2] * a) >> 8) + rgba.r;
}

typedef struct _pixel_rgba8888_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} pixel_rgba8888_t;

#define pixel_rgba8888_BPP 4
#define pixel_rgba8888_a(p) (p).a
#define pixel_rgba8888_format BITMAP_FMT_RGBA8888
#define pixel_rgba8888_to_rgba(p) {(p).r, (p).g, (p).b, (p).a}
#define pixel_rgba8888_from_rgba(r, g, b, a) {r, g, b, a}
#define pixel_rgba8888_from_rgb(r, g, b) {r, g, b, 0xff}
static inline void pixel_rgba8888_blend_rgba_dark(void* pixel, uint8_t a) {
  pixel_rgba8888_t* p = (pixel_rgba8888_t*)pixel;
  rgba_t c = pixel_rgba8888_to_rgba(*p);
  c = rgba_blend_dark(c, a);
  {
    pixel_rgba8888_t q = pixel_rgba8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

static inline void pixel_rgba8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  pixel_rgba8888_t* p = (pixel_rgba8888_t*)pixel;
  rgba_t c = pixel_rgba8888_to_rgba(*p);
  c = rgba_blend_premulti(c, rgba, rgba.a);
  {
    pixel_rgba8888_t q = pixel_rgba8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

typedef struct _pixel_abgr8888_t {
  uint8_t a;
  uint8_t b;
  uint8_t g;
  uint8_t r;
} pixel_abgr8888_t;

#define pixel_abgr8888_BPP 4
#define pixel_abgr8888_a(p) (p).a
#define pixel_abgr8888_format BITMAP_FMT_ABGR8888
#define pixel_abgr8888_to_rgba(p) {(p).r, (p).g, (p).b, (p).a}
#define pixel_abgr8888_from_rgb(r, g, b) {0xff, b, g, r}
#define pixel_abgr8888_from_rgba(r, g, b, a) {a, b, g, r}
static inline void pixel_abgr8888_blend_rgba_dark(void* pixel, uint8_t a) {
  pixel_abgr8888_t* p = (pixel_abgr8888_t*)pixel;
  rgba_t c = pixel_abgr8888_to_rgba(*p);
  c = rgba_blend_dark(c, a);
  {
    pixel_abgr8888_t q = pixel_abgr8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

static inline void pixel_abgr8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  pixel_abgr8888_t* p = (pixel_abgr8888_t*)pixel;
  rgba_t c = pixel_abgr8888_to_rgba(*p);
  c = rgba_blend_premulti(c, rgba, rgba.a);
  {
    pixel_abgr8888_t q = pixel_abgr8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

typedef struct _pixel_bgra8888_t {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} pixel_bgra8888_t;

#define pixel_bgra8888_BPP 4
#define pixel_bgra8888_a(p) (p).a
#define pixel_bgra8888_format BITMAP_FMT_BGRA8888
#define pixel_bgra8888_to_rgba(p) {(p).r, (p).g, (p).b, (p).a}
#define pixel_bgra8888_from_rgba(r, g, b, a) {b, g, r, a}
#define pixel_bgra8888_from_rgb(r, g, b) {b, g, r, 0xff}
static inline void pixel_bgra8888_blend_rgba_dark(void* pixel, uint8_t a) {
  pixel_bgra8888_t* p = (pixel_bgra8888_t*)pixel;
  rgba_t c = pixel_bgra8888_to_rgba(*p);
  c = rgba_blend_dark(c, a);
  {
    pixel_bgra8888_t q = pixel_bgra8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

static inline void pixel_bgra8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  pixel_bgra8888_t* p = (pixel_bgra8888_t*)pixel;
  rgba_t c = pixel_bgra8888_to_rgba(*p);
  c = rgba_blend_premulti(c, rgba, rgba.a);
  {
    pixel_bgra8888_t q = pixel_bgra8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

typedef struct _pixel_argb8888_t {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pixel_argb8888_t;

#define pixel_argb8888_BPP 4
#define pixel_argb8888_a(p) (p).a
#define pixel_argb8888_format BITMAP_FMT_ARGB8888
#define pixel_argb8888_to_rgba(p) {(p).r, (p).g, (p).b, (p).a}
#define pixel_argb8888_from_rgba(r, g, b, a) {a, r, g, b}
#define pixel_argb8888_from_rgb(r, g, b) {0xff, r, g, b}
static inline void pixel_argb8888_blend_rgba_dark(void* pixel, uint8_t a) {
  pixel_argb8888_t* p = (pixel_argb8888_t*)pixel;
  rgba_t c = pixel_argb8888_to_rgba(*p);
  c = rgba_blend_dark(c, a);
  {
    pixel_argb8888_t q = pixel_argb8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

static inline void pixel_argb8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  pixel_argb8888_t* p = (pixel_argb8888_t*)pixel;
  rgba_t c = pixel_argb8888_to_rgba(*p);
  c = rgba_blend_premulti(c, rgba, rgba.a);
  {
    pixel_argb8888_t q = pixel_argb8888_from_rgba(c.r, c.g, c.b, c.a);
    *p = q;
  }
}

typedef uint8_t pixel_gray_t;

#define pixel_gray_BPP 1
#define pixel_gray_a(p) 0xff
#define pixel_gray_format BITMAP_FMT_GRAY
#define pixel_gray_to_rgba(p) {(p) * 0.2989, (p) * 0.5870, (p) * 0.1140, 0xff}
#define pixel_gray_from_rgba(r, g, b, a) rgb_to_gray(r, g, b)
#define pixel_gray_from_rgb(r, g, b) rgb_to_gray(r, g, b)

#define color_to_mono(c) (((c).rgba.r))
#define color_from_mono(p) color_init(p, 0, 0, 0xff)

#pragma pack(pop)

#define color_to_pixel(c) pixel_from_rgb((c).rgba.r, (c).rgba.g, (c).rgba.b)

#endif /*TK_PIXEL_H*/

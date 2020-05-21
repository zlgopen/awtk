/**
 * File:   pixel.h
 * Author: AWTK Develop Team
 * Brief:  pixel format definitions
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
 * 2018-10-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PIXEL_H
#define TK_PIXEL_H

#include "tkc/color.h"

static inline uint8_t tk_pixel_limit_uint8(int32_t tmp) {
  if (tmp > 0xff) {
    tmp = 0xff;
  } else if (tmp < 0) {
    tmp = 0;
  }
  return (uint8_t)tmp;
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
  { p.r << 3, p.g << 2, p.b << 3, 0xff }

#define pixel_rgb565_from_rgb(r, g, b) \
  { r >> 3, g >> 2, b >> 3 }

#if WITH_LCD_CLEAR_ALPHA
#define pixel_rgb565_from_rgba(r, g, b, a) \
  { (r * a) >> 11, (g * a) >> 10, (b * a) >> 11 }
#else
#define pixel_rgb565_from_rgba(r, g, b, a) pixel_rgb565_from_rgb(r, g, b)
#endif

static inline void pixel_rgb565_blend_rgba_dark(void* p, uint8_t a) {
  uint16_t v = *(uint16_t*)p;

  *(uint16_t*)p = (((((v & 0xf800) >> 8) * a) >> 11) << 11) |
                  (((((v & 0x7e0) >> 3) * a) >> 10) << 5) | ((((v & 0x1f) << 3) * a) >> 11);
}

static inline void pixel_rgb565_blend_rgba_premulti(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint16_t v = *(uint16_t*)p;

  *(uint16_t*)p = (((((v & 0xf800) >> 8) * a + (rgba.b << 8)) >> 11) << 11) |
                  (((((v & 0x7e0) >> 3) * a + (rgba.g << 8)) >> 10) << 5) |
                  ((((v & 0x1f) << 3) * a + (rgba.r << 8)) >> 11);
}

typedef struct _pixel_bgr565_t {
  uint16_t b : 5;
  uint16_t g : 6;
  uint16_t r : 5;
} pixel_bgr565_t;

#define pixel_bgr565_BPP 2
#define pixel_bgr565_a(p) 0xff
#define pixel_bgr565_format BITMAP_FMT_BGR565
#define pixel_bgr565_to_rgba(p) \
  { p.r << 3, p.g << 2, p.b << 3, 0xff }

#define pixel_bgr565_from_rgb(r, g, b) \
  { b >> 3, g >> 2, r >> 3 }

#if WITH_LCD_CLEAR_ALPHA
#define pixel_bgr565_from_rgba(r, g, b, a) \
  { (b * a) >> 11, (g * a) >> 10, (r * a) >> 11 }
#else
#define pixel_bgr565_from_rgba(r, g, b, a) pixel_bgr565_from_rgb(r, g, b)
#endif

static inline void pixel_bgr565_blend_rgba_dark(void* p, uint8_t a) {
  uint16_t v = *(uint16_t*)p;

  *(uint16_t*)p = (((((v & 0xf800) >> 8) * a) >> 11) << 11) |
                  (((((v & 0x7e0) >> 3) * a) >> 10) << 5) | ((((v & 0x1f) << 3) * a) >> 11);
}

static inline void pixel_bgr565_blend_rgba(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t minus_a = 0xff - a;
  uint16_t v = *(uint16_t*)p;

  *(uint16_t*)p = (((((v & 0xf800) >> 8) * minus_a + (rgba.r * a)) >> 11) << 11) |
                  (((((v & 0x7e0) >> 3) * minus_a + (rgba.g * a)) >> 10) << 5) |
                  ((((v & 0x1f) << 3) * minus_a + (rgba.b * a)) >> 11);
}

static inline void pixel_bgr565_blend_rgba_premulti(void* p, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint16_t v = *(uint16_t*)p;

  *(uint16_t*)p = (((((v & 0xf800) >> 8) * a + (rgba.r << 8)) >> 11) << 11) |
                  (((((v & 0x7e0) >> 3) * a + (rgba.g << 8)) >> 10) << 5) |
                  ((((v & 0x1f) << 3) * a + (rgba.b << 8)) >> 11);
}

typedef struct _pixel_rgb888_t {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pixel_rgb888_t;

#define pixel_rgb888_BPP 3
#define pixel_rgb888_a(p) 0xff
#define pixel_rgb888_format BITMAP_FMT_RGB888
#define pixel_rgb888_to_rgba(p) \
  { p.r, p.g, p.b, 0xff }

#define pixel_rgb888_from_rgb(r, g, b) \
  { r, g, b }

#if WITH_LCD_CLEAR_ALPHA
#define pixel_rgb888_from_rgba(r, g, b, a) \
  { (r * a) >> 8, (g * a) >> 8, (b * a) >> 8 }
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
#define pixel_bgr888_to_rgba(p) \
  { p.r, p.g, p.b, 0xff }

#define pixel_bgr888_from_rgb(r, g, b) \
  { b, g, r }
#if WITH_LCD_CLEAR_ALPHA
#define pixel_bgr888_from_rgba(r, g, b, a) \
  { (b * a) >> 8, (g * a) >> 8, (r * a) >> 8 }
#else
#define pixel_bgr888_from_rgba(r, g, b, a) pixel_bgr888_from_rgb(r, g, b);
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
#define pixel_rgba8888_a(p) p.a
#define pixel_rgba8888_format BITMAP_FMT_RGBA8888
#define pixel_rgba8888_to_rgba(p) \
  { p.r, p.g, p.b, p.a }
#define pixel_rgba8888_from_rgba(r, g, b, a) \
  { r, g, b, a }
#define pixel_rgba8888_from_rgb(r, g, b) \
  { r, g, b, 0xff }
static inline void pixel_rgba8888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  if (p[3] > 0xf4) {
    p[0] = (p[0] * a) >> 8;
    p[1] = (p[1] * a) >> 8;
    p[2] = (p[2] * a) >> 8;
    p[3] = 0xff;
  } else {
    uint8_t out_a = (p[3] * a) >> 8;

    p[0] = (p[0] * out_a) >> 8;
    p[1] = (p[1] * out_a) >> 8;
    p[2] = (p[2] * out_a) >> 8;
    p[3] = out_a;
  }
}

static inline void pixel_rgba8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  if (p[3] > 0xf4) {
    p[0] = ((p[0] * a) >> 8) + rgba.r;
    p[1] = ((p[1] * a) >> 8) + rgba.g;
    p[2] = ((p[2] * a) >> 8) + rgba.b;
    p[3] = 0xff;
  } else {
    uint8_t s_a = 0xff - a;
    uint8_t out_a = tk_pixel_limit_uint8(p[3] + s_a - ((p[3] * s_a) >> 8));
    if (out_a > 0) {
      uint8_t d_a = (p[3] * (0xff - rgba.a)) >> 8;
      p[0] = (p[0] * d_a + (rgba.r << 8)) / out_a;
      p[1] = (p[1] * d_a + (rgba.g << 8)) / out_a;
      p[2] = (p[2] * d_a + (rgba.b << 8)) / out_a;
    }
    p[3] = out_a;
  }
}

typedef struct _pixel_abgr8888_t {
  uint8_t a;
  uint8_t b;
  uint8_t g;
  uint8_t r;
} pixel_abgr8888_t;

#define pixel_abgr8888_BPP 4
#define pixel_abgr8888_a(p) p.a
#define pixel_abgr8888_format BITMAP_FMT_ABGR8888
#define pixel_abgr8888_to_rgba(p) \
  { p.r, p.g, p.b, p.a }
#define pixel_abgr8888_from_rgb(r, g, b) \
  { 0xff, b, g, r }
#define pixel_abgr8888_from_rgba(r, g, b, a) \
  { a, b, g, r }
static inline void pixel_abgr8888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  if (p[0] > 0xf4) {
    p[0] = 0xff;
    p[1] = (p[1] * a) >> 8;
    p[2] = (p[2] * a) >> 8;
    p[3] = (p[3] * a) >> 8;
  } else {
    uint8_t out_a = (p[0] * a) >> 8;
    p[0] = out_a;
    p[1] = (p[1] * out_a) >> 8;
    p[2] = (p[2] * out_a) >> 8;
    p[3] = (p[3] * out_a) >> 8;
  }
}

static inline void pixel_abgr8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  if (p[0] > 0xf4) {
    p[0] = 0xff;
    p[1] = ((p[1] * a) >> 8) + rgba.b;
    p[2] = ((p[2] * a) >> 8) + rgba.g;
    p[3] = ((p[3] * a) >> 8) + rgba.r;
  } else {
    uint8_t s_a = 0xff - a;
    uint8_t out_a = tk_pixel_limit_uint8(p[0] + s_a - ((p[0] * s_a) >> 8));
    if (out_a > 0) {
      uint8_t d_a = (p[0] * (0xff - rgba.a)) >> 8;
      p[1] = (p[1] * d_a + (rgba.b << 8)) / out_a;
      p[2] = (p[2] * d_a + (rgba.g << 8)) / out_a;
      p[3] = (p[3] * d_a + (rgba.r << 8)) / out_a;
    }
    p[0] = out_a;
  }
}

typedef struct _pixel_bgra8888_t {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} pixel_bgra8888_t;

#define pixel_bgra8888_BPP 4
#define pixel_bgra8888_a(p) p.a
#define pixel_bgra8888_format BITMAP_FMT_BGRA8888
#define pixel_bgra8888_to_rgba(p) \
  { p.r, p.g, p.b, p.a }
#define pixel_bgra8888_from_rgba(r, g, b, a) \
  { b, g, r, a }
#define pixel_bgra8888_from_rgb(r, g, b) \
  { b, g, r, 0xff }
static inline void pixel_bgra8888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  if (p[3] > 0xf4) {
    p[0] = (p[0] * a) >> 8;
    p[1] = (p[1] * a) >> 8;
    p[2] = (p[2] * a) >> 8;
    p[3] = 0xff;
  } else {
    uint8_t out_a = (p[3] * a) >> 8;
    p[0] = (p[0] * out_a) >> 8;
    p[1] = (p[1] * out_a) >> 8;
    p[2] = (p[2] * out_a) >> 8;
    p[3] = out_a;
  }
}

static inline void pixel_bgra8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  if (p[3] > 0xf4) {
    p[0] = ((p[0] * a) >> 8) + rgba.b;
    p[1] = ((p[1] * a) >> 8) + rgba.g;
    p[2] = ((p[2] * a) >> 8) + rgba.r;
    p[3] = 0xff;
  } else {
    uint8_t s_a = 0xff - a;
    uint8_t out_a = tk_pixel_limit_uint8(p[3] + s_a - ((p[3] * s_a) >> 8));
    if (out_a > 0) {
      uint8_t d_a = (p[3] * (0xff - rgba.a)) >> 8;
      p[0] = (p[0] * d_a + (rgba.b << 8)) / out_a;
      p[1] = (p[1] * d_a + (rgba.g << 8)) / out_a;
      p[2] = (p[2] * d_a + (rgba.r << 8)) / out_a;
    }
    p[3] = out_a;
  }
}

typedef struct _pixel_argb8888_t {
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pixel_argb8888_t;

#define pixel_argb8888_BPP 4
#define pixel_argb8888_a(p) p.a
#define pixel_argb8888_format BITMAP_FMT_ARGB8888
#define pixel_argb8888_to_rgba(p) \
  { p.r, p.g, p.b, p.a }
#define pixel_argb8888_from_rgba(r, g, b, a) \
  { a, r, g, b }
#define pixel_argb8888_from_rgb(r, g, b) \
  { 0xff, r, g, b }
static inline void pixel_argb8888_blend_rgba_dark(void* pixel, uint8_t a) {
  uint8_t* p = (uint8_t*)pixel;

  if (p[0] > 0xf4) {
    p[0] = 0xff;
    p[1] = (p[1] * a) >> 8;
    p[2] = (p[2] * a) >> 8;
    p[3] = (p[3] * a) >> 8;
  } else {
    uint8_t out_a = (p[0] * a) >> 8;
    p[0] = out_a;
    p[1] = (p[1] * out_a) >> 8;
    p[2] = (p[2] * out_a) >> 8;
    p[3] = (p[3] * out_a) >> 8;
  }
}

static inline void pixel_argb8888_blend_rgba_premulti(void* pixel, rgba_t rgba) {
  uint8_t a = rgba.a;
  uint8_t* p = (uint8_t*)pixel;

  if (p[0] > 0xf4) {
    p[0] = 0xff;
    p[1] = ((p[1] * a) >> 8) + rgba.r;
    p[2] = ((p[2] * a) >> 8) + rgba.g;
    p[3] = ((p[3] * a) >> 8) + rgba.b;
  } else {
    uint8_t s_a = 0xff - a;
    uint8_t out_a = tk_pixel_limit_uint8(p[0] + s_a - ((p[0] * s_a) >> 8));
    if (out_a > 0) {
      uint8_t d_a = (p[0] * (0xff - rgba.a)) >> 8;
      p[1] = (p[1] * d_a + (rgba.r << 8)) / out_a;
      p[2] = (p[2] * d_a + (rgba.g << 8)) / out_a;
      p[3] = (p[3] * d_a + (rgba.b << 8)) / out_a;
    }
    p[0] = out_a;
  }
}

typedef uint8_t pixel_gray_t;

#define pixel_gray_BPP 1
#define pixel_gray_a(p) 0xff
#define pixel_gray_format BITMAP_FMT_GRAY
#define pixel_gray_to_rgba(p) \
  { p * 0.2989, p * 0.5870, p * 0.1140, 0xff }
#define pixel_gray_from_rgba(r, g, b, a) (((r)*30 + (g)*59 + (b)*11) / 100)
#define pixel_gray_from_rgb(r, g, b) (((r)*30 + (g)*59 + (b)*11) / 100)

#define color_to_mono(c) (((c).rgba.r))
#define color_from_mono(p) color_init(p, 0, 0, 0xff)

#pragma pack(pop)

#define color_to_pixel(c) pixel_from_rgb(c.rgba.r, c.rgba.g, c.rgba.b)

#endif /*TK_PIXEL_H*/

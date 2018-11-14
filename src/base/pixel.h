/**
 * File:   pixel.h
 * Author: AWTK Develop Team
 * Brief:  pixel format definitions
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/types_def.h"

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
#define pixel_rgba8888_from_rgb(r, g, b) \
  { r, g, b, 0xff }

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
#define pixel_bgra8888_from_rgb(r, g, b) \
  { b, g, r, 0xff }

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
#define pixel_argb8888_from_rgb(r, g, b) \
  { 0xff, r, g, b }

#pragma pack(pop)

#define color_to_pixel(c) pixel_from_rgb(c.rgba.r, c.rgba.g, c.rgba.b)

#endif /*TK_PIXEL_H*/

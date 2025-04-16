/**
 * File:   nanovg_plus_type.c
 * Author: AWTK Develop Team
 * Brief:  nanovg plus base type.
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-10-27 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef __NANOVG_PLUS_TYPE_H__
#define __NANOVG_PLUS_TYPE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define NVGP_PI 3.14159265358979323846264338327f
#define NVGP_KAPPA90 0.5522847493f              // Length proportional to radius of a cubic bezier handle for 90deg arcs.


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif

typedef union _nvgp_color_t {
  uint32_t color;
  struct {
      uint8_t r;
      uint8_t g; 
      uint8_t b;
      uint8_t a;
  } rgba;
} nvgp_color_t;

typedef enum _nvgp_error_t {
  NVGP_OK = 0,
  NVGP_BAD_PARAMS,
  NVGP_FAIL,
  NVGP_OOM,
} nvgp_error_t;

#define nvgp_sqrtf(a) sqrtf(a)
#define nvgp_modf(a, b) fmodf(a, b)
#define nvgp_sinf(a) sinf(a)
#define nvgp_cosf(a) cosf(a)
#define nvgp_tanf(a) tanf(a)
#define nvgp_acosf(a) acosf(a)
#define nvgp_atan2f(a, b) atan2f(a, b)

#define nvgp_abs(a) ((a) < (0) ? (-(a)) : (a))
#define nvgp_min(a, b) ((a) < (b) ? (a) : (b))
#define nvgp_max(a, b) ((a) > (b) ? (a) : (b))

#define nvgp_signf(a) ((a) >= 0.0f ? 1.0f : -1.0f)
#define nvgp_cross(dx0, dy0, dx1, dy1) (dx1) * (dy0) - (dx0) * (dy1)
#define nvgp_clamp(a, mn, mx) (a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a))
static int nvgp_fequalf(float a, float b) { float t = a - b; return t > -1e-6 && t < 1e-6; }

#define nvgp_get_arrary_size(arr) (sizeof(arr) / sizeof(0[(arr)]))

#ifndef NULL
#ifdef __cplusplus
#define NULL            0
#else
#define NULL   ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE         0u
#endif

#ifndef TRUE
#define TRUE          1u
#endif

#ifndef nvgp_bool_t
typedef uint8_t nvgp_bool_t;
#endif

#ifndef NVGP_MALLOC
#define NVGP_MALLOC(size) malloc(size)
#endif

#ifndef NVGP_CALLOC
#define NVGP_CALLOC(nmemb, size) calloc(nmemb, size)
#endif

#ifndef NVGP_REALLOC
#define NVGP_REALLOC(p, size) realloc(p, size)
#endif

#ifndef NVGP_FREE
#define NVGP_FREE(p) free(p); p = NULL;
#endif

#ifndef NVGP_MEMSET
#define NVGP_MEMSET(ptr, data, n) memset(ptr, data, n)
#endif

#ifndef NVGP_MEMCPY
#define NVGP_MEMCPY(dst, src, n) memcpy(dst, src, n)
#endif

#ifndef NVGP_MEMCMP
#define NVGP_MEMCMP(dst, src, n) memcmp(dst, src, n)
#endif

#define NVGP_ZALLOC(type) (type*)NVGP_CALLOC(1, sizeof(type))
#define NVGP_ZALLOCN(type, n) (type*)NVGP_CALLOC(n, sizeof(type))
#define NVGP_REALLOCT(type, p, n) (type*)NVGP_REALLOC(p, (n) * sizeof(type))

#ifndef NVGP_PRINTF
#if defined(_MSC_VER)
#define NVGP_PRINTF(format, ...) printf(format, __VA_ARGS__);
#elif defined(__GNUC__)
#define NVGP_PRINTF(format, args...) printf(format, ##args)
#else
#define NVGP_PRINTF(format, ...)
#endif
#endif

inline static nvgp_color_t nvgp_color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  nvgp_color_t c;
  c.rgba.r = r;
  c.rgba.g = g;
  c.rgba.b = b;
  c.rgba.a = a;
  return c;
}

#ifdef __cplusplus
}
#endif
#endif

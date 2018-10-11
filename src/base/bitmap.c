/**
 * File:   bitmap.c
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/pixel.h"
#include "base/bitmap.h"

bitmap_t* bitmap_create(void) {
  bitmap_t* bitmap = TKMEM_ZALLOC(bitmap_t);
  return_value_if_fail(bitmap != NULL, NULL);

  bitmap->should_free_handle = TRUE;

  return bitmap;
}

ret_t bitmap_destroy(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap->specific_destroy != NULL) {
    bitmap->specific_destroy(bitmap);
  }

  if (bitmap->destroy != NULL) {
    bitmap->destroy(bitmap);
  }

  if (bitmap->should_free_data) {
    TKMEM_FREE(bitmap->data);
  }

  if (bitmap->should_free_handle) {
    memset(bitmap, 0x00, sizeof(bitmap_t));
    TKMEM_FREE(bitmap);
  }

  return RET_OK;
}

#include "base/pixel_pack_unpack.h"

ret_t bitmap_rgba_to_bgr565(bitmap_t* image, uint16_t* output) {
  uint32_t i = 0;
  uint16_t* d = output;
  uint32_t nr = image->w * image->h;
  uint32_t* s = (uint32_t*)(image->data);
  return_value_if_fail(image->format == BITMAP_FMT_RGBA8888, RET_BAD_PARAMS);

  for (i = 0; i < nr; i++, d++, s++) {
    color_t c;
    c.color = *s;
    *d = rgb_to_bgr565(c.rgba.r, c.rgba.g, c.rgba.b);
  }

  return RET_OK;
}

ret_t bitmap_rgba_to_bgra(bitmap_t* image) {
  int i = 0;
  int nr = image->w * image->h;
  uint8_t* data = (uint8_t*)(image->data);

  /*default format is rgba, some sys need bgra*/
  for (i = 0; i < nr; i++) {
    uint8_t r = data[0];
    uint8_t g = data[1];
    uint8_t b = data[2];
    uint8_t a = data[3];
    data[0] = b;
    data[1] = g;
    data[2] = r;
    data[3] = a;

    data += 4;
  }
  image->format = BITMAP_FMT_BGRA8888;

  return RET_OK;
}

uint32_t bitmap_get_bpp(bitmap_t* bmp) {
  return_value_if_fail(bmp != NULL, 0);

  switch (bmp->format) {
    case BITMAP_FMT_RGBA8888:
    case BITMAP_FMT_ABGR8888:
    case BITMAP_FMT_BGRA8888:
    case BITMAP_FMT_ARGB8888:
      return 4;
    case BITMAP_FMT_RGB565:
    case BITMAP_FMT_BGR565:
      return 2;
    case BITMAP_FMT_RGB888:
    case BITMAP_FMT_BGR888:
      return 3;
    default:
      break;
  }

  return 0;
}

bitmap_t* bitmap_create_ex(uint32_t w, uint32_t h, bitmap_format_t format) {
  uint32_t bpp = 0;
  uint8_t* data = NULL;
  bitmap_t* bitmap = TKMEM_ZALLOC(bitmap_t);
  return_value_if_fail(bitmap != NULL, NULL);

  bitmap->w = w;
  bitmap->h = h;
  bitmap->format = format;
  bpp = bitmap_get_bpp(bitmap);
  bitmap->should_free_data = TRUE;
  bitmap->should_free_handle = TRUE;
  data = (uint8_t*)TKMEM_ALLOC(bpp * w * h);

  if (data != NULL) {
    memset(data, 0x00, bpp * w * h);
    bitmap->data = data;
  }

  return bitmap;
}

ret_t bitmap_get_pixel(bitmap_t* bitmap, uint32_t x, uint32_t y, rgba_t* rgba) {
  const uint8_t* data = NULL;
  uint32_t bpp = bitmap_get_bpp(bitmap);
  return_value_if_fail(bitmap != NULL && bitmap->data != NULL && rgba != NULL, RET_BAD_PARAMS);
  return_value_if_fail(x < bitmap->w && y < bitmap->h, RET_BAD_PARAMS);

  data = bitmap->data + (bitmap->w * y + x) * bpp;

  switch (bitmap->format) {
    case BITMAP_FMT_RGBA8888: {
      pixel_rgba8888_t* p = (pixel_rgba8888_t*)data;
      rgba_t t = pixel_rgba8888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_ABGR8888: {
      pixel_abgr8888_t* p = (pixel_abgr8888_t*)data;
      rgba_t t = pixel_abgr8888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_BGRA8888: {
      pixel_bgra8888_t* p = (pixel_bgra8888_t*)data;
      rgba_t t = pixel_bgra8888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_ARGB8888: {
      pixel_argb8888_t* p = (pixel_argb8888_t*)data;
      rgba_t t = pixel_argb8888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_RGB565: {
      pixel_rgb565_t* p = (pixel_rgb565_t*)data;
      rgba_t t = pixel_rgb565_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_BGR565: {
      pixel_bgr565_t* p = (pixel_bgr565_t*)data;
      rgba_t t = pixel_bgr565_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_RGB888: {
      pixel_rgb888_t* p = (pixel_rgb888_t*)data;
      rgba_t t = pixel_rgb888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    case BITMAP_FMT_BGR888: {
      pixel_bgr888_t* p = (pixel_bgr888_t*)data;
      rgba_t t = pixel_bgr888_to_rgba((*p));
      *rgba = t;

      return RET_OK;
    }
    default:
      break;
  }

  return RET_NOT_IMPL;
}

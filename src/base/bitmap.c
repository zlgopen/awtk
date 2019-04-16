/**
 * File:   bitmap.c
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
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
    if (bitmap->data_free_ptr != NULL) {
      TKMEM_FREE(bitmap->data_free_ptr);
    } else {
      TKMEM_FREE(bitmap->data);
    }

    if (bitmap->gif_delays) {
      TKMEM_FREE(bitmap->gif_delays);
      bitmap->gif_delays = NULL;
    }
  }

  if (bitmap->should_free_handle) {
    memset(bitmap, 0x00, sizeof(bitmap_t));
    TKMEM_FREE(bitmap);
  }

  return RET_OK;
}

#include "base/pixel_pack_unpack.h"

uint32_t bitmap_get_bpp_of_format(bitmap_format_t format) {
  switch (format) {
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

uint32_t bitmap_get_bpp(bitmap_t* bmp) {
  return_value_if_fail(bmp != NULL, 0);

  return bitmap_get_bpp_of_format((bitmap_format_t)(bmp->format));
}

ret_t bitmap_alloc_data(bitmap_t* bitmap) {
  uint32_t size = 0;
  uint8_t* data = NULL;
  return_value_if_fail(bitmap != NULL && bitmap->w > 0 && bitmap->h > 0, RET_BAD_PARAMS);

  size = bitmap_get_line_length(bitmap) * bitmap->h;
  size = TK_ROUND_TO(size, BITMAP_ALIGN_SIZE) + BITMAP_ALIGN_SIZE;

  data = (uint8_t*)TKMEM_ALLOC(size);
  if (data != NULL) {
    memset(data, 0x00, size);
    bitmap->data_free_ptr = data;
    bitmap->should_free_data = TRUE;
    while (((uint32_t)(data - (uint8_t*)NULL)) % BITMAP_ALIGN_SIZE) {
      data++;
    }
    bitmap->data = data;

    return RET_OK;
  } else {
    return RET_OOM;
  }
}

bitmap_t* bitmap_create_ex(uint32_t w, uint32_t h, uint32_t line_length, bitmap_format_t format) {
  bitmap_t* bitmap = TKMEM_ZALLOC(bitmap_t);
  return_value_if_fail(bitmap != NULL, NULL);

  bitmap->w = w;
  bitmap->h = h;
  bitmap->format = format;
  bitmap->should_free_handle = TRUE;
  bitmap_set_line_length(bitmap, line_length);

  bitmap_alloc_data(bitmap);
  if (bitmap->data == NULL) {
    TKMEM_FREE(bitmap);
    bitmap = NULL;
  }

  return bitmap;
}

ret_t bitmap_get_pixel(bitmap_t* bitmap, uint32_t x, uint32_t y, rgba_t* rgba) {
  const uint8_t* data = NULL;
  uint32_t bpp = bitmap_get_bpp(bitmap);
  return_value_if_fail(bitmap != NULL && bitmap->data != NULL && rgba != NULL, RET_BAD_PARAMS);
  return_value_if_fail(x < bitmap->w && y < bitmap->h, RET_BAD_PARAMS);

  data = bitmap->data + bitmap_get_line_length(bitmap) * y + x * bpp;

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

ret_t bitmap_init_rgba8888(bitmap_t* b, uint32_t w, uint32_t h, const uint8_t* data,
                           uint32_t comp) {
  uint32_t i = 0;

  if (comp == 4) {
    uint32_t size = w * h * 4;
    memcpy((uint8_t*)(b->data), data, size);
  } else {
    uint32_t n = w * h;
    const uint8_t* s = data;
    uint8_t* d = (uint8_t*)(b->data);

    for (i = 0; i < n; i++) {
      *d++ = *s++;
      *d++ = *s++;
      *d++ = *s++;
      *d++ = 0xff;
    }
  }

  return RET_OK;
}

ret_t bitmap_init_bgra8888(bitmap_t* b, uint32_t w, uint32_t h, const uint8_t* data,
                           uint32_t comp) {
  uint32_t i = 0;
  uint32_t n = w * h;
  const uint8_t* s = data;
  uint8_t* d = (uint8_t*)(b->data);

  /*bgra=rgba*/
  for (i = 0; i < n; i++) {
    d[0] = s[2];
    d[1] = s[1];
    d[2] = s[0];
    d[3] = (comp == 3) ? 0xff : s[3];

    d += 4;
    s += comp;
  }

  return RET_OK;
}

ret_t bitmap_init_bgr565(bitmap_t* b, uint32_t w, uint32_t h, const uint8_t* data, uint32_t comp) {
  uint32_t i = 0;
  uint32_t n = w * h;
  const uint8_t* s = data;
  uint16_t* d = (uint16_t*)(b->data);

  for (i = 0; i < n; i++) {
    uint8_t r = s[0];
    uint8_t g = s[1];
    uint8_t b = s[2];

    *d++ = rgb_to_bgr565(r, g, b);

    s += comp;
  }

  return RET_OK;
}

bool_t rgba_data_is_opaque(const uint8_t* data, uint32_t w, uint32_t h, uint8_t comp) {
  if (comp == 4) {
    uint32_t i = 0;
    uint32_t n = w * h;
    const uint8_t* s = data;

    for (i = 0; i < n; i++) {
      if (s[3] != 0xff) {
        return FALSE;
      }
      s += 4;
    }
  }

  return TRUE;
}

ret_t bitmap_init_from_rgba(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format,
                            const uint8_t* data, uint32_t comp) {
  return_value_if_fail(bitmap != NULL && data != NULL && (comp == 3 || comp == 4), RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap_t));

  bitmap->w = w;
  bitmap->h = h;
  bitmap->format = format;
  bitmap_set_line_length(bitmap, 0);
  bitmap->flags = BITMAP_FLAG_IMMUTABLE;
  return_value_if_fail(bitmap_alloc_data(bitmap) == RET_OK, RET_OOM);

  if (rgba_data_is_opaque(data, w, h, comp)) {
    bitmap->flags |= BITMAP_FLAG_OPAQUE;
  }

  if (format == BITMAP_FMT_BGRA8888) {
    return bitmap_init_bgra8888(bitmap, w, h, data, comp);
  } else if (format == BITMAP_FMT_RGBA8888) {
    return bitmap_init_rgba8888(bitmap, w, h, data, comp);
  } else if (format == BITMAP_FMT_BGR565) {
    return bitmap_init_bgr565(bitmap, w, h, data, comp);
  } else {
    return RET_NOT_IMPL;
  }
}

ret_t bitmap_init(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format, uint8_t* data) {
  uint32_t bpp = bitmap_get_bpp_of_format(format);
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap_t));

  bitmap->w = w;
  bitmap->h = h;
  bitmap->data = data;
  bitmap->format = format;
  bitmap_set_line_length(bitmap, 0);
  if (bpp < 4) {
    bitmap->flags = BITMAP_FLAG_OPAQUE;
  }

  if (data == NULL) {
    bitmap_alloc_data(bitmap);
  }

  return bitmap->data != NULL ? RET_OK : RET_OOM;
}

ret_t bitmap_set_line_length(bitmap_t* bitmap, uint32_t line_length) {
  uint32_t bpp = bitmap_get_bpp(bitmap);
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  bitmap->line_length = tk_max(bitmap->w * bpp, line_length);

  return RET_OK;
}

uint32_t bitmap_get_line_length(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, 0);

  assert(bitmap->line_length > bitmap->w);

  return bitmap->line_length;
}

#if defined(WITH_SDL) || defined(LINUX)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(p, s)

#include "stb/stb_image_write.h"

bool_t bitmap_save_png(bitmap_t* bitmap, const char* filename) {
  color_t c;
  uint32_t x = 0;
  uint32_t y = 0;
  bitmap_t* t = NULL;
  uint32_t* p = NULL;
  return_value_if_fail(bitmap != NULL && filename != NULL, FALSE);

  t = bitmap_create_ex(bitmap->w, bitmap->h, 0, BITMAP_FMT_RGBA8888);
  return_value_if_fail(t != NULL, FALSE);

  p = (uint32_t*)(t->data);
  for (y = 0; y < bitmap->h; y++) {
    for (x = 0; x < bitmap->w; x++) {
      bitmap_get_pixel(bitmap, x, y, &(c.rgba));
      *p++ = c.color;
    }
  }

  stbi_write_png(filename, t->w, t->h, 4, t->data, t->w * 4);
  bitmap_destroy(t);

  return TRUE;
}

#endif /*defined(WITH_SDL) || defined(LINUX)*/

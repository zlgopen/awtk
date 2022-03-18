/**
 * File:   bitmap.c
 * Author: AWTK Develop Team
 * Brief:  bitmap interface
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/utils.h"
#include "base/pixel.h"
#include "base/bitmap.h"
#include "base/graphic_buffer.h"

bitmap_t* bitmap_create(void) {
  bitmap_t* bitmap = TKMEM_ZALLOC(bitmap_t);
  return_value_if_fail(bitmap != NULL, NULL);

  bitmap->should_free_handle = TRUE;

  return bitmap;
}

ret_t bitmap_destroy_with_self(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);
  bitmap->should_free_handle = TRUE;

  return bitmap_destroy(bitmap);
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
    if (bitmap->buffer != NULL) {
      graphic_buffer_destroy(bitmap->buffer);
    }

    if (bitmap->gif_delays != NULL) {
      TKMEM_FREE(bitmap->gif_delays);
      bitmap->gif_delays = NULL;
    }

    TKMEM_FREE(bitmap->data_free_ptr);
  }

  if (bitmap->should_free_handle) {
    memset(bitmap, 0x00, sizeof(bitmap_t));
    TKMEM_FREE(bitmap);
  } else {
    memset(bitmap, 0x00, sizeof(bitmap_t));
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
    case BITMAP_FMT_GRAY:
      return 1;
    default:
      break;
  }

  return 0;
}

uint32_t bitmap_get_bpp(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, 0);

  return bitmap_get_bpp_of_format((bitmap_format_t)(bitmap->format));
}

ret_t bitmap_alloc_data(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL && bitmap->w > 0 && bitmap->h > 0, RET_BAD_PARAMS);

  return GRAPHIC_BUFFER_CREATE_FOR_BITMAP(bitmap);
}

bool_t bitmap_flag_is_lcd_orientation(bitmap_t* bitmap) {
  return bitmap->flags & BITMAP_FLAG_LCD_ORIENTATION;
}

#ifdef AWTK_WEB
#include <emscripten.h>
static ret_t bitmap_web_destroy(bitmap_t* bitmap) {
  int32_t id = tk_pointer_to_int(bitmap->specific);
  EM_ASM_INT({ return VGCanvas.destroyMutableImage($0); }, id);

  return RET_OK;
}

#endif /*AWTK_WEB*/

bitmap_t* bitmap_create_ex(uint32_t w, uint32_t h, uint32_t line_length, bitmap_format_t format) {
  bitmap_t* bitmap = TKMEM_ZALLOC(bitmap_t);
  uint32_t bpp = bitmap_get_bpp_of_format(format);

  return_value_if_fail(bitmap != NULL, NULL);

  bitmap->w = w;
  bitmap->h = h;
  bitmap->format = format;
  bitmap->should_free_handle = TRUE;

  if (bpp < 4) {
    bitmap->flags = BITMAP_FLAG_OPAQUE;
  }

  bitmap_set_line_length(bitmap, line_length);

  bitmap_alloc_data(bitmap);
  if (bitmap->buffer == NULL) {
    TKMEM_FREE(bitmap);
    bitmap = NULL;
  } else {
#ifdef AWTK_WEB
    uint8_t* data = bitmap_lock_buffer_for_write(bitmap);
    return_value_if_fail(format == BITMAP_FMT_RGBA8888, NULL);
    int32_t id = EM_ASM_INT({ return VGCanvas.createMutableImage($0, $1, $2, $3, $4); }, data, w, h,
                            line_length, format);
    bitmap->specific = tk_pointer_from_int(id);
    bitmap->specific_destroy = bitmap_web_destroy;
    bitmap_unlock_buffer(bitmap);
#endif /*AWTK_WEB*/
  }

  return bitmap;
}

ret_t bitmap_get_pixel(bitmap_t* bitmap, uint32_t x, uint32_t y, rgba_t* rgba) {
  ret_t ret = RET_OK;
  const uint8_t* data = NULL;
  uint8_t* bitmap_data = NULL;
  uint32_t w = bitmap_get_physical_width(bitmap);
  uint32_t h = bitmap_get_physical_height(bitmap);
  uint32_t bpp = bitmap_get_bpp(bitmap);
  return_value_if_fail(bitmap != NULL && bitmap->buffer != NULL && rgba != NULL, RET_BAD_PARAMS);
  return_value_if_fail(x < w && y < h, RET_BAD_PARAMS);

  bitmap_data = bitmap_lock_buffer_for_read(bitmap);
  data = bitmap_data + bitmap_get_physical_line_length(bitmap) * y + x * bpp;

  switch (bitmap->format) {
    case BITMAP_FMT_MONO: {
      bool_t pixel = bitmap_mono_get_pixel(bitmap_data, w, h, x, y);
      color_t c = color_from_mono(pixel);
      *rgba = c.rgba;
      break;
    }
    case BITMAP_FMT_RGBA8888: {
      pixel_rgba8888_t* p = (pixel_rgba8888_t*)data;
      rgba_t t = pixel_rgba8888_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_ABGR8888: {
      pixel_abgr8888_t* p = (pixel_abgr8888_t*)data;
      rgba_t t = pixel_abgr8888_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      pixel_bgra8888_t* p = (pixel_bgra8888_t*)data;
      rgba_t t = pixel_bgra8888_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_ARGB8888: {
      pixel_argb8888_t* p = (pixel_argb8888_t*)data;
      rgba_t t = pixel_argb8888_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_RGB565: {
      pixel_rgb565_t* p = (pixel_rgb565_t*)data;
      rgba_t t = pixel_rgb565_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_BGR565: {
      pixel_bgr565_t* p = (pixel_bgr565_t*)data;
      rgba_t t = pixel_bgr565_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_RGB888: {
      pixel_rgb888_t* p = (pixel_rgb888_t*)data;
      rgba_t t = pixel_rgb888_to_rgba((*p));
      *rgba = t;
      break;
    }
    case BITMAP_FMT_BGR888: {
      pixel_bgr888_t* p = (pixel_bgr888_t*)data;
      rgba_t t = pixel_bgr888_to_rgba((*p));
      *rgba = t;
      break;
    }
    default: {
      ret = RET_NOT_IMPL;
      break;
    }
  }
  bitmap_unlock_buffer(bitmap);

  return ret;
}

static ret_t bitmap_init_data_by_rotate(bitmap_t* bitmap, uint32_t w, uint32_t h,
                                        lcd_orientation_t o) {
  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    bitmap->w = w;
    bitmap->h = h;
  } else {
    bitmap->w = h;
    bitmap->h = w;
  }
  bitmap_set_line_length(bitmap, 0);
  return_value_if_fail(bitmap_alloc_data(bitmap) == RET_OK, RET_OOM);
  if (o != LCD_ORIENTATION_0) {
    bitmap->orientation = o;
    bitmap->flags |= BITMAP_FLAG_LCD_ORIENTATION;
  }
  bitmap->w = w;
  bitmap->h = h;
  bitmap_set_line_length(bitmap, 0);
  return RET_OK;
}

typedef ret_t (*bitmap_init_fill_color_t)(const uint8_t* s, uint8_t* d, uint32_t comp);
static ret_t bitmap_init_impl_by_rotate(bitmap_t* bitmap, uint32_t w, uint32_t h,
                                        const uint8_t* data, uint32_t comp, lcd_orientation_t o,
                                        bitmap_init_fill_color_t fill_color) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t bpp = 0;
  uint8_t* d = NULL;
  uint8_t* bdata = NULL;
  const uint8_t* s = data;
  uint32_t line_length = 0;

  ret_t ret = bitmap_init_data_by_rotate(bitmap, w, h, o);
  return_value_if_fail(ret == RET_OK, ret);

  bpp = bitmap_get_bpp(bitmap);
  line_length = bitmap_get_physical_line_length(bitmap);
  bdata = bitmap_lock_buffer_for_write(bitmap);

  switch (o) {
    case LCD_ORIENTATION_0: {
      for (j = 0; j < h; j++) {
        d = bdata + j * line_length;
        for (i = 0; i < w; i++) {
          fill_color(s, d, comp);
          d += bpp;
          s += comp;
        }
      }
      break;
    }
    case LCD_ORIENTATION_90: {
      bdata = bdata + (w - 1) * line_length;
      for (i = 0; i < h; i++) {
        uint8_t* d = bdata;
        for (j = 0; j < w; j++) {
          fill_color(s, d, comp);
          d -= line_length;
          s += comp;
        }
        bdata += bpp;
      }
      break;
    }
    case LCD_ORIENTATION_180: {
      uint8_t* d = bdata + h * line_length - bpp;
      for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
          fill_color(s, d, comp);
          d -= bpp;
          s += comp;
        }
      }
      break;
    }
    case LCD_ORIENTATION_270: {
      bdata = bdata + line_length - bpp;
      for (i = 0; i < h; i++) {
        uint8_t* d = bdata;
        for (j = 0; j < w; j++) {
          fill_color(s, d, comp);
          d += line_length;
          s += comp;
        }
        bdata -= bpp;
      }
      break;
    }
    default:
      break;
  }

  bitmap_unlock_buffer(bitmap);
  return RET_OK;
}

static ret_t bitmap_init_rgba8888_fill_color(const uint8_t* s, uint8_t* d, uint32_t comp) {
  d[0] = s[0];
  d[1] = s[1];
  d[2] = s[2];
  d[3] = (comp == 3) ? 0xff : s[3];
  return RET_OK;
}

ret_t bitmap_init_rgba8888(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data,
                           uint32_t comp, lcd_orientation_t o) {
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  if (comp == 4 && o == LCD_ORIENTATION_0) {
    uint32_t i = 0;
    uint8_t* bdata = NULL;
    bitmap->w = w;
    bitmap->h = h;
    bitmap_set_line_length(bitmap, 0);
    return_value_if_fail(bitmap_alloc_data(bitmap) == RET_OK, RET_OOM);

    bdata = bitmap_lock_buffer_for_write(bitmap);
    return_value_if_fail(bdata != NULL, RET_BAD_PARAMS);
    for (i = 0; i < h; i++) {
      memcpy((uint8_t*)(bdata) + i * bitmap->line_length, data + i * w * 4, w * 4);
    }
    bitmap_unlock_buffer(bitmap);
    return RET_OK;
  } else {
    return bitmap_init_impl_by_rotate(bitmap, w, h, data, comp, o, bitmap_init_rgba8888_fill_color);
  }
}

static ret_t bitmap_init_bgra8888_fill_color(const uint8_t* s, uint8_t* d, uint32_t comp) {
  d[0] = s[2];
  d[1] = s[1];
  d[2] = s[0];
  d[3] = (comp == 3) ? 0xff : s[3];
  return RET_OK;
}

ret_t bitmap_init_bgra8888(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data,
                           uint32_t comp, lcd_orientation_t o) {
  return bitmap_init_impl_by_rotate(bitmap, w, h, data, comp, o, bitmap_init_bgra8888_fill_color);
}

static ret_t bitmap_init_rgb565_fill_color(const uint8_t* s, uint8_t* d, uint32_t comp) {
  uint8_t r = s[0];
  uint8_t g = s[1];
  uint8_t b = s[2];
  *(uint16_t*)d = rgb_to_rgb565(r, g, b);
  (void)comp;
  return RET_OK;
}

ret_t bitmap_init_rgb565(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data,
                         uint32_t comp, lcd_orientation_t o) {
  return bitmap_init_impl_by_rotate(bitmap, w, h, data, comp, o, bitmap_init_rgb565_fill_color);
}

static ret_t bitmap_init_bgr565_fill_color(const uint8_t* s, uint8_t* d, uint32_t comp) {
  uint8_t r = s[0];
  uint8_t g = s[1];
  uint8_t b = s[2];
  *(uint16_t*)d = rgb_to_bgr565(r, g, b);
  (void)comp;
  return RET_OK;
}

ret_t bitmap_init_bgr565(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data,
                         uint32_t comp, lcd_orientation_t o) {
  return bitmap_init_impl_by_rotate(bitmap, w, h, data, comp, o, bitmap_init_bgr565_fill_color);
}

ret_t bitmap_init_mono(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data,
                       uint32_t comp) {
  uint32_t i = 0;
  uint32_t j = 0;
  bool_t pixel = FALSE;
  const uint8_t* s = data;
  uint8_t* bdata = bitmap_lock_buffer_for_write(bitmap);
  uint8_t* d = (uint8_t*)(bdata);

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      uint8_t r = s[0];
      uint8_t g = s[1];
      uint8_t b = s[2];

      if (comp == 4) {
        uint8_t a = s[3];
        r = (r * a) >> 8;
        g = (g * a) >> 8;
        b = (b * a) >> 8;
      }

      pixel = rgb_to_gray(r, g, b) > 10;
      bitmap_mono_set_pixel(d, w, h, i, j, pixel);

      s += comp;
    }
  }
  bitmap_unlock_buffer(bitmap);

  return RET_OK;
}

static ret_t bitmap_init_gray_fill_color(const uint8_t* s, uint8_t* d, uint32_t comp) {
  uint8_t r = s[0];
  uint8_t g = s[1];
  uint8_t b = s[2];

  if (comp == 4) {
    uint8_t a = s[3];
    r = (r * a) >> 8;
    g = (g * a) >> 8;
    b = (b * a) >> 8;
  }

  *d = rgb_to_gray(r, g, b);
  return RET_OK;
}

ret_t bitmap_init_gray(bitmap_t* bitmap, uint32_t w, uint32_t h, const uint8_t* data, uint32_t comp,
                       lcd_orientation_t o) {
  return bitmap_init_impl_by_rotate(bitmap, w, h, data, comp, o, bitmap_init_gray_fill_color);
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

ret_t bitmap_init_from_bgra(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format,
                            const uint8_t* data, uint32_t comp, lcd_orientation_t o) {
  ret_t ret = RET_OK;
  return_value_if_fail(bitmap != NULL && data != NULL && (comp == 3 || comp == 4), RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap_t));

  bitmap->format = format;
  bitmap->flags = BITMAP_FLAG_IMMUTABLE;

  if (rgba_data_is_opaque(data, w, h, comp)) {
    bitmap->flags |= BITMAP_FLAG_OPAQUE;
  }

  if (format == BITMAP_FMT_BGRA8888) {
    ret = bitmap_init_rgba8888(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_RGBA8888) {
    ret = bitmap_init_bgra8888(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_BGR565) {
    ret = bitmap_init_rgb565(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_RGB565) {
    ret = bitmap_init_bgr565(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_MONO) {
    ret = bitmap_init_mono(bitmap, w, h, data, comp);
  } else if (format == BITMAP_FMT_GRAY) {
    ret = bitmap_init_gray(bitmap, w, h, data, comp, o);
  } else {
    return RET_NOT_IMPL;
  }
  return ret;
}

ret_t bitmap_init_from_rgba(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format,
                            const uint8_t* data, uint32_t comp, lcd_orientation_t o) {
  ret_t ret = RET_OK;
  return_value_if_fail(bitmap != NULL && data != NULL && (comp == 3 || comp == 4), RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap_t));

  bitmap->format = format;
  bitmap->flags = BITMAP_FLAG_IMMUTABLE;

  if (rgba_data_is_opaque(data, w, h, comp)) {
    bitmap->flags |= BITMAP_FLAG_OPAQUE;
  }

  if (format == BITMAP_FMT_BGRA8888) {
    ret = bitmap_init_bgra8888(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_RGBA8888) {
    ret = bitmap_init_rgba8888(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_BGR565) {
    ret = bitmap_init_bgr565(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_RGB565) {
    ret = bitmap_init_rgb565(bitmap, w, h, data, comp, o);
  } else if (format == BITMAP_FMT_MONO) {
    ret = bitmap_init_mono(bitmap, w, h, data, comp);
  } else if (format == BITMAP_FMT_GRAY) {
    ret = bitmap_init_gray(bitmap, w, h, data, comp, o);
  } else {
    return RET_NOT_IMPL;
  }
  return ret;
}

ret_t bitmap_init(bitmap_t* bitmap, uint32_t w, uint32_t h, bitmap_format_t format, uint8_t* data) {
  uint32_t bpp = bitmap_get_bpp_of_format(format);
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap_t));

  bitmap->w = w;
  bitmap->h = h;
  bitmap->format = format;
  bitmap_set_line_length(bitmap, 0);
  if (bpp < 4) {
    bitmap->flags = BITMAP_FLAG_OPAQUE;
  }

  if (data == NULL) {
    bitmap_alloc_data(bitmap);
  } else {
    bitmap->buffer = GRAPHIC_BUFFER_CREATE_WITH_DATA(data, w, h, format);
    bitmap->should_free_data = TRUE;
  }

  return bitmap->buffer != NULL ? RET_OK : RET_OOM;
}

ret_t bitmap_set_line_length(bitmap_t* bitmap, uint32_t line_length) {
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap->format == BITMAP_FMT_MONO) {
    bitmap->line_length = TK_BITMAP_MONO_LINE_LENGTH(bitmap->w);
  } else {
    uint32_t bpp = bitmap_get_bpp(bitmap);
    bitmap->line_length = tk_max(bitmap->w * bpp, line_length);
  }

  return RET_OK;
}

uint32_t bitmap_get_line_length(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, 0);

  if (bitmap->line_length == 0) {
    bitmap_set_line_length(bitmap, 0);
  }

  return bitmap->line_length;
}

uint32_t bitmap_get_physical_line_length(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, 0);

  if (bitmap_flag_is_lcd_orientation(bitmap)) {
    uint32_t ret = graphic_buffer_get_physical_line_length(bitmap->buffer);
    if (ret != 0) {
      return ret;
    }
  }

  return bitmap_get_line_length(bitmap);
}

uint32_t bitmap_get_physical_width(bitmap_t* bitmap) {
  uint32_t ret = 0;
  return_value_if_fail(bitmap != NULL, 0);

  if (bitmap_flag_is_lcd_orientation(bitmap)) {
    ret = graphic_buffer_get_physical_width(bitmap->buffer);
    if (ret != 0) {
      return ret;
    }
  }
  return bitmap->w;
}

uint32_t bitmap_get_physical_height(bitmap_t* bitmap) {
  uint32_t ret = 0;
  return_value_if_fail(bitmap != NULL, 0);

  if (bitmap_flag_is_lcd_orientation(bitmap)) {
    ret = graphic_buffer_get_physical_height(bitmap->buffer);
    if (ret != 0) {
      return ret;
    }
  }
  return bitmap->h;
}

ret_t rgba_data_premulti_alpha(const uint8_t* data, uint8_t a_index, uint32_t w, uint32_t h) {
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t n = w * h;
  uint8_t* s = (uint8_t*)data;
  return_value_if_fail(data != NULL && a_index < 4, RET_BAD_PARAMS);

  for (i = 0; i < n; i++) {
    uint8_t a = s[a_index];
    for (k = 0; k < 4; k++) {
      if (k != a_index) {
        s[k] = (s[k] * a) >> 8;
      }
    }
    s += 4;
  }

  return RET_OK;
}

ret_t bitmap_premulti_alpha(bitmap_t* bitmap) {
  ret_t ret = RET_FAIL;
  uint8_t* bitmap_data = NULL;
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap->flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return RET_OK;
  }

  bitmap_data = bitmap_lock_buffer_for_write(bitmap);
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  switch (bitmap->format) {
    case BITMAP_FMT_RGBA8888:
    case BITMAP_FMT_BGRA8888: {
      ret = rgba_data_premulti_alpha(bitmap_data, 3, bitmap->w, bitmap->h);
      break;
    }
    case BITMAP_FMT_ABGR8888:
    case BITMAP_FMT_ARGB8888: {
      ret = rgba_data_premulti_alpha(bitmap_data, 0, bitmap->w, bitmap->h);
      break;
    }
  }

  if (ret == RET_OK) {
    bitmap->flags = bitmap->flags | BITMAP_FLAG_PREMULTI_ALPHA;
  }
  bitmap_unlock_buffer(bitmap);

  return ret;
}

bitmap_t* bitmap_clone(bitmap_t* bitmap) {
  bitmap_t* b = NULL;
  return_value_if_fail(bitmap != NULL && bitmap->buffer != NULL, NULL);

  b = bitmap_create_ex(bitmap->w, bitmap->h, bitmap->line_length,
                       (bitmap_format_t)(bitmap->format));
  return_value_if_fail(b != NULL, NULL);

  if (b->buffer != NULL) {
    uint8_t* s = bitmap_lock_buffer_for_read(bitmap);
    uint8_t* d = bitmap_lock_buffer_for_write(b);
    if (s != NULL && d != NULL) {
      memcpy((char*)(d), s, b->line_length * b->h);
    }
    bitmap_unlock_buffer(bitmap);
    bitmap_unlock_buffer(b);
    b->name = bitmap->name;
  }

  return b;
}

#if defined(WITH_STB_IMAGE)
#define STB_IMAGE_STATIC 1
#define STBI_WRITE_NO_STDIO 1
#define STB_IMAGE_WRITE_STATIC 1
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(p, s)

#include "tkc/fs.h"
#include "stb/stb_image_write.h"

bitmap_t* bitmap_rgba8888_from_bitmap(bitmap_t* bitmap) {
  color_t c;
  uint32_t x = 0;
  uint32_t y = 0;
  bitmap_t* t = NULL;
  uint32_t* p = NULL;
  uint8_t* tdata = NULL;
  uint32_t w = bitmap_get_physical_width(bitmap);
  uint32_t h = bitmap_get_physical_height(bitmap);
  t = bitmap_create_ex(w, h, 0, BITMAP_FMT_RGBA8888);
  return_value_if_fail(t != NULL, FALSE);

  tdata = (uint8_t*)bitmap_lock_buffer_for_write(t);
  for (y = 0; y < h; y++) {
    p = (uint32_t*)(tdata + (y * t->line_length));
    for (x = 0; x < w; x++) {
      bitmap_get_pixel(bitmap, x, y, &(c.rgba));
      *p++ = c.color;
    }
  }
  bitmap_unlock_buffer(t);

  return t;
}

static bool_t bitmap_rgba8888_save_png(bitmap_t* bitmap, const char* filename) {
  int32_t len = 0;
  bitmap_t* t = bitmap;
  uint8_t* tdata = NULL;
  unsigned char* png_data = NULL;
  tdata = bitmap_lock_buffer_for_write(t);
  png_data = stbi_write_png_to_mem(tdata, t->w * 4, t->w, t->h, 4, &len);
  bitmap_unlock_buffer(t);

  if (png_data == NULL) {
    return_value_if_fail(png_data != NULL, FALSE);
  } else {
    fs_t* fs = os_fs();
    fs_file_t* png_file = fs_open_file(fs, filename, "wb");
    if (png_file != NULL) {
      fs_file_write(png_file, png_data, len);
    } else {
      ENSURE(!" do not open file, do not save png file !");
    }
    STBIW_FREE(png_data);

    return_value_if_fail(fs_file_close(png_file) == RET_OK, FALSE);
  }

  return TRUE;
}

bool_t bitmap_save_png(bitmap_t* bitmap, const char* filename) {
  return_value_if_fail(bitmap != NULL && filename != NULL, FALSE);

  if (bitmap->format != BITMAP_FMT_RGBA8888 || bitmap->line_length != bitmap->w * 4) {
    bitmap_t* t = bitmap_rgba8888_from_bitmap(bitmap);
    bitmap_rgba8888_save_png(t, filename);
    bitmap_destroy(t);
  } else {
    bitmap_rgba8888_save_png(bitmap, filename);
  }

  return TRUE;
}

#else

bool_t bitmap_save_png(bitmap_t* bitmap, const char* filename) {
  return FALSE;
}

#endif /*defined(WITH_STB_IMAGE)*/

/*helper*/
#define BIT_OFFSET(xx) (7 - ((xx) % 8))

ret_t bitmap_mono_set_pixel(uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y,
                            bool_t pixel) {
  uint32_t offset = y * TK_BITMAP_MONO_LINE_LENGTH(w) + (x >> 3);
  uint8_t* data = buff + offset;
  uint32_t offset_bit = BIT_OFFSET(x);

  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(x < w && y < h, RET_BAD_PARAMS);

  if (pixel) {
    *data |= (1 << offset_bit);
  } else {
    *data &= ~(1 << offset_bit);
  }

  return RET_OK;
}

bool_t bitmap_mono_get_pixel(const uint8_t* buff, uint32_t w, uint32_t h, uint32_t x, uint32_t y) {
  uint32_t offset = y * TK_BITMAP_MONO_LINE_LENGTH(w) + (x >> 3);
  const uint8_t* data = buff + offset;
  uint32_t offset_bit = BIT_OFFSET(x);
  return_value_if_fail(buff != NULL, FALSE);
  return_value_if_fail(x < w && y < h, FALSE);

  return (*data >> offset_bit) & 0x1;
}

uint8_t* bitmap_mono_create_data(uint32_t w, uint32_t h) {
  uint8_t* buff = NULL;
  uint32_t size = TK_BITMAP_MONO_LINE_LENGTH(w) * h;
  return_value_if_fail(w > 0 && h > 0, NULL);

  buff = TKMEM_ALLOC(size);
  return_value_if_fail(buff != NULL, NULL);

  memset(buff, 0x00, size);

  return buff;
}

ret_t bitmap_mono_dump(const uint8_t* buff, uint32_t w, uint32_t h) {
  uint32_t j = 0;
  uint32_t i = 0;
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      bool_t pixel = bitmap_mono_get_pixel(buff, w, h, i, j);
      if (pixel) {
        log_debug("O");
      } else {
        log_debug(" ");
      }
    }
    log_debug("\n");
  }
  return RET_OK;
}

uint8_t* bitmap_lock_buffer_for_read(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, NULL);

  if (bitmap->buffer != NULL) {
    if (!graphic_buffer_is_valid_for(bitmap->buffer, bitmap)) {
      assert(!" graphic_buffer is not valid ");
      return NULL;
    }
    return graphic_buffer_lock_for_read(bitmap->buffer);
  } else {
    return NULL;
  }
}

uint8_t* bitmap_lock_buffer_for_write(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, NULL);

  if (bitmap->buffer != NULL) {
    if (!graphic_buffer_is_valid_for(bitmap->buffer, bitmap)) {
      assert(!" graphic_buffer is not valid ");
      return NULL;
    }
    return graphic_buffer_lock_for_write(bitmap->buffer);
  } else {
    return NULL;
  }
}

ret_t bitmap_unlock_buffer(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, RET_BAD_PARAMS);

  if (bitmap->buffer != NULL) {
    assert(graphic_buffer_is_valid_for(bitmap->buffer, bitmap));
    return graphic_buffer_unlock(bitmap->buffer);
  } else {
    return RET_FAIL;
  }
}

ret_t bitmap_transform(bitmap_t* bitmap, bitmap_transform_t transform, void* ctx) {
  uint32_t x = 0;
  uint32_t y = 0;
  ret_t ret = RET_OK;
  const uint8_t* data = NULL;
  uint8_t* bitmap_data = NULL;
  uint32_t bpp = bitmap_get_bpp(bitmap);
  return_value_if_fail(transform != NULL, RET_BAD_PARAMS);
  return_value_if_fail(bitmap != NULL && bitmap->buffer != NULL, RET_BAD_PARAMS);

  bitmap_data = bitmap_lock_buffer_for_write(bitmap);
  for (y = 0; y < bitmap->h; y++) {
    data = bitmap_data + bitmap_get_physical_line_length(bitmap) * y;
    for (x = 0; x < bitmap->w; x++) {
      switch (bitmap->format) {
        case BITMAP_FMT_RGBA8888: {
          pixel_rgba8888_t* p = (pixel_rgba8888_t*)data;
          rgba_t pixel = pixel_rgba8888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_rgba8888_t result = pixel_rgba8888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_ABGR8888: {
          pixel_abgr8888_t* p = (pixel_abgr8888_t*)data;
          rgba_t pixel = pixel_abgr8888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_abgr8888_t result = pixel_abgr8888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_BGRA8888: {
          pixel_bgra8888_t* p = (pixel_bgra8888_t*)data;
          rgba_t pixel = pixel_bgra8888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_bgra8888_t result = pixel_bgra8888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_ARGB8888: {
          pixel_argb8888_t* p = (pixel_argb8888_t*)data;
          rgba_t pixel = pixel_argb8888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_argb8888_t result = pixel_argb8888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_RGB565: {
          pixel_rgb565_t* p = (pixel_rgb565_t*)data;
          rgba_t pixel = pixel_rgb565_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_rgb565_t result = pixel_rgb565_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_BGR565: {
          pixel_bgr565_t* p = (pixel_bgr565_t*)data;
          rgba_t pixel = pixel_bgr565_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_bgr565_t result = pixel_bgr565_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_RGB888: {
          pixel_rgb888_t* p = (pixel_rgb888_t*)data;
          rgba_t pixel = pixel_rgb888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_rgb888_t result = pixel_rgb888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        case BITMAP_FMT_BGR888: {
          pixel_bgr888_t* p = (pixel_bgr888_t*)data;
          rgba_t pixel = pixel_bgr888_to_rgba((*p));
          if (transform(ctx, bitmap, x, y, &pixel) == RET_OK) {
            pixel_bgr888_t result = pixel_bgr888_from_rgba(pixel.r, pixel.g, pixel.b, pixel.a);
            *p = result;
          }
          break;
        }
        default: {
          ret = RET_NOT_IMPL;
          break;
        }
      }
      data += bpp;
    }
  }
  bitmap_unlock_buffer(bitmap);
  bitmap->flags |= BITMAP_FLAG_CHANGED;

  return ret;
}

uint32_t bitmap_get_mem_size(bitmap_t* bitmap) {
  return_value_if_fail(bitmap != NULL, 0);

  return bitmap->w * bitmap->h * bitmap_get_bpp(bitmap);
}

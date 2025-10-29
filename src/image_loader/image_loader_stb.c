/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(p, s)

#include "tkc/mem.h"
#include "base/bitmap.h"
#include "stb/stb_image.h"
#include "base/system_info.h"
#include "image_loader/image_loader_stb.h"

static uint8_t* convert_2_to_4(uint8_t* src, uint32_t w, uint32_t h) {
  uint32_t i = 0;
  uint8_t* s = src;
  uint8_t* d = NULL;
  uint8_t* data = NULL;
  uint32_t size = w * h;
  return_value_if_fail(src != NULL, NULL);

  data = TKMEM_ALLOC(size * 4);
  return_value_if_fail(data != NULL, NULL);

  d = data;
  for (i = 0; i < size; i++) {
    d[0] = s[0];
    d[1] = s[0];
    d[2] = s[0];
    d[3] = s[1];

    d += 4;
    s += 2;
  }

  return data;
}

static uint8_t* convert_1_to_4(uint8_t* src, uint32_t w, uint32_t h) {
  uint32_t i = 0;
  uint8_t* s = src;
  uint8_t* d = NULL;
  uint8_t* data = NULL;
  uint32_t size = w * h;
  return_value_if_fail(src != NULL, NULL);

  data = TKMEM_ALLOC(size * 4);
  return_value_if_fail(data != NULL, NULL);

  d = data;
  for (i = 0; i < size; i++) {
    d[0] = s[0];
    d[1] = s[0];
    d[2] = s[0];
    d[3] = 0xFF;

    d += 4;
    s += 1;
  }

  return data;
}

ret_t stb_load_gif_next_frame(bitmap_t* bitmap, void** gif_context, void** gif_msg,
                              void** gif_cache, int* gif_delays, const uint8_t* buff,
                              uint32_t buff_size, int* layers, bool_t* is_end) {
  int comp;
  int stride;
  int req_comp = 0;
  stbi_uc* u = 0;
  stbi_uc* out = 0;
  stbi__gif* g = 0;
  stbi__context* s = 0;
  stbi_uc* two_back = 0;
  uint8_t* data = NULL;
  ret_t ret = RET_FAIL;
  lcd_orientation_t o = LCD_ORIENTATION_0;
  bitmap_format_t opaque_bitmap_format = BITMAP_FMT_RGBA8888;
  return_value_if_fail(bitmap != NULL && buff != NULL, RET_FAIL);

#if !defined(WITH_VGCANVAS_CAIRO) && defined(WITH_FAST_LCD_PORTRAIT)
  if (system_info()->flags & SYSTEM_INFO_FLAG_FAST_LCD_PORTRAIT) {
    o = system_info()->lcd_orientation;
  }
#endif

#ifdef WITHOUT_FAST_LCD_PORTRAIT_FOR_IMAGE
  o = LCD_ORIENTATION_0;
#endif

#ifdef WITH_BITMAP_BGR565
  opaque_bitmap_format = BITMAP_FMT_BGR565;
#elif defined(WITH_BITMAP_RGB565)
  opaque_bitmap_format = BITMAP_FMT_RGB565;
#elif defined(WITH_BITMAP_BGR888)
  opaque_bitmap_format = BITMAP_FMT_BGR888;
#elif defined(WITH_BITMAP_RGB888)
  opaque_bitmap_format = BITMAP_FMT_RGB888;
#endif /*WITH_BITMAP_RGB565*/

#ifdef WITH_LCD_MONO
  opaque_bitmap_format = BITMAP_FMT_MONO;
#endif

  if (*gif_context == NULL) {
    *gif_context = TKMEM_ALLOC(sizeof(stbi__context));
    if (*gif_context == NULL) goto oom;
    memset(*gif_context, 0, sizeof(stbi__context));
    stbi__start_mem(*gif_context, buff, buff_size);
  } else if (((stbi__context*)*gif_context)->img_buffer == NULL) {
    stbi__start_mem(*gif_context, buff, buff_size);
  }
  if (*gif_msg == NULL) {
    *gif_msg = TKMEM_ALLOC(sizeof(stbi__gif));
    if (*gif_msg == NULL) goto oom;
    memset(*gif_msg, 0, sizeof(stbi__gif));
  }
  out = (stbi_uc*)*gif_cache;
  g = (stbi__gif*)*gif_msg;
  s = (stbi__context*)*gif_context;
  if (*is_end) {
    *layers = 0;
    stbi__start_mem(*gif_context, buff, buff_size);
    if (g->out) STBI_FREE(g->out);
    if (g->history) STBI_FREE(g->history);
    if (g->background) STBI_FREE(g->background);
    memset(*gif_msg, 0, sizeof(stbi__gif));
  }

  if ((bitmap->is_gif && !*is_end) || stbi__gif_test(s)) {
    bitmap_deinit(bitmap);
    *is_end = FALSE;

    return_value_if_fail(s != NULL && g != NULL, RET_FAIL);

    if (*layers >= 2) {
      two_back = out;
    }
    u = stbi__gif_load_next(s, g, &comp, req_comp, two_back);
    if (u == 0) goto oom;
    if (u == (stbi_uc*)s) u = 0;

    if (u) {
      ++(*layers);
      stride = g->w * g->h * 4;
      if (out == NULL) {
        out = (stbi_uc*)stbi__malloc(2 * stride);
        if (out == NULL) goto oom;
        *gif_cache = out;
      }

      memcpy(out, out + stride, stride);
      memcpy(out + stride, u, stride);

      *gif_delays = g->delay;

      comp = 4;
      data = out + stride;
    } else {
      comp = 4;
      data = out + g->w * g->h * 4;
      *is_end = TRUE;
    }
    if (opaque_bitmap_format == BITMAP_FMT_MONO) {
      ret = bitmap_init_from_rgba(bitmap, g->w, g->h, BITMAP_FMT_MONO, data, comp, o);
    } else {
      if (opaque_bitmap_format == BITMAP_FMT_BGR565 &&
          rgba_data_is_opaque(data, g->w, g->h, comp)) {
        ret = bitmap_init_from_rgba(bitmap, g->w, g->h, BITMAP_FMT_BGR565, data, comp, o);
      } else if (opaque_bitmap_format == BITMAP_FMT_RGB565 &&
                 rgba_data_is_opaque(data, g->w, g->h, comp)) {
        ret = bitmap_init_from_rgba(bitmap, g->w, g->h, BITMAP_FMT_RGB565, data, comp, o);
      } else if (opaque_bitmap_format == BITMAP_FMT_BGRA8888) {
        ret = bitmap_init_from_rgba(bitmap, g->w, g->h, BITMAP_FMT_BGRA8888, data, comp, o);
      } else {
        ret = bitmap_init_from_rgba(bitmap, g->w, g->h, BITMAP_FMT_RGBA8888, data, comp, o);
      }
    }
    if (ret == RET_OK) {
      bitmap->is_gif = TRUE;
      bitmap->gif_frame_h = g->h;
      bitmap->gif_frames_nr = *layers;
    }
  }

  return ret;
oom:
  if (out) STBI_FREE(out);
  if (g->out) STBI_FREE(g->out);
  if (g->history) STBI_FREE(g->history);
  if (g->background) STBI_FREE(g->background);
  if (g) TKMEM_FREE(g);
  if (s) TKMEM_FREE(s);
  *gif_msg = NULL;
  *gif_context = NULL;
  *gif_cache = NULL;

  return ret;
}

ret_t stb_gif_frame_create(void** gif_context, void** gif_msg) {
  *gif_context = TKMEM_ALLOC(sizeof(stbi__context));
  if (*gif_context == NULL) {
    return RET_OOM;
  }
  memset(*gif_context, 0, sizeof(stbi__context));
  *gif_msg = TKMEM_ALLOC(sizeof(stbi__gif));
  if (*gif_msg == NULL) {
    return RET_OOM;
  }
  memset(*gif_msg, 0, sizeof(stbi__gif));
  return RET_OK;
}

ret_t stb_gif_frame_reset(void** gif_context, void** gif_msg, void** gif_cache,
                          bool_t is_reset_gif_cache) {
  if (*gif_msg) {
    stbi__gif* g = (stbi__gif*)*gif_msg;
    if (g->out) STBI_FREE(g->out);
    if (g->history) STBI_FREE(g->history);
    if (g->background) STBI_FREE(g->background);
    memset(g, 0, sizeof(stbi__gif));
  }
  if (*gif_context) {
    memset(*gif_context, 0, sizeof(stbi__context));
  }
  if (*gif_cache && is_reset_gif_cache) {
    stbi_image_free((uint8_t*)(*gif_cache));
    *gif_cache = NULL;
  }
  return RET_OK;
}

ret_t stb_gif_frame_free(void** gif_context, void** gif_msg, void** gif_cache) {
  if (*gif_msg) {
    stbi__gif* g = (stbi__gif*)*gif_msg;
    if (g->out) STBI_FREE(g->out);
    if (g->history) STBI_FREE(g->history);
    if (g->background) STBI_FREE(g->background);
    TKMEM_FREE(g);
    *gif_msg = NULL;
  }
  if (*gif_context) {
    TKMEM_FREE(*gif_context);
    *gif_context = NULL;
  }
  if (*gif_cache) {
    stbi_image_free((uint8_t*)(*gif_cache));
    *gif_cache = NULL;
  }
  return RET_OK;
}

ret_t stb_load_image(int32_t subtype, const uint8_t* buff, uint32_t buff_size, bitmap_t* image,
                     bitmap_format_t transparent_bitmap_format,
                     bitmap_format_t opaque_bitmap_format, lcd_orientation_t o) {
  int w = 0;
  int h = 0;
  int n = 0;
  ret_t ret = RET_FAIL;

  if (subtype != ASSET_TYPE_IMAGE_GIF) {
    uint8_t* data = NULL;
    int out_channel_order;
    uint8_t* stb_data =
        stbi_load_from_memory_ex(buff, buff_size, &w, &h, &n, &out_channel_order, 0);
    return_value_if_fail(stb_data != NULL, RET_FAIL);

    if (n == 2) {
      n = 4;
      data = convert_2_to_4(stb_data, w, h);
    } else if (n == 1) {
      n = 4;
      data = convert_1_to_4(stb_data, w, h);
    } else {
      data = stb_data;
    }
    if (opaque_bitmap_format == BITMAP_FMT_MONO) {
      if (out_channel_order == STBI_ORDER_RGB) {
        ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_MONO, data, n, o);
      } else {
        ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_MONO, data, n, o);
      }
    } else {
      if (out_channel_order == STBI_ORDER_RGB) {
        if (opaque_bitmap_format == BITMAP_FMT_BGR565 && rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGR565, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_RGB565 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGB565, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_BGR888 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGR888, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_RGB888 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGB888, data, n, o);
        } else if (transparent_bitmap_format == BITMAP_FMT_BGRA8888) {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGRA8888, data, n, o);
        } else {
          ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGBA8888, data, n, o);
        }
      } else {
        if (opaque_bitmap_format == BITMAP_FMT_BGR565 && rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_BGR565, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_RGB565 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_RGB565, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_BGR888 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_BGR888, data, n, o);
        } else if (opaque_bitmap_format == BITMAP_FMT_RGB888 &&
                   rgba_data_is_opaque(data, w, h, n)) {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_RGB888, data, n, o);
        } else if (transparent_bitmap_format == BITMAP_FMT_BGRA8888) {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_BGRA8888, data, n, o);
        } else {
          ret = bitmap_init_from_bgra(image, w, h, BITMAP_FMT_RGBA8888, data, n, o);
        }
      }
    }
    stbi_image_free((uint8_t*)(stb_data));
    if (stb_data != data) {
      TKMEM_FREE(data);
    }
  } else {
    int z = 0;
    int total_h = 0;
    int* delays = NULL;
    uint8_t* data = stbi_load_gif_from_memory(buff, buff_size, &delays, &w, &h, &z, &n, 0);

    return_value_if_fail(data != NULL, RET_FAIL);

    total_h = h * z;
    if (opaque_bitmap_format == BITMAP_FMT_MONO) {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_MONO, data, n, o);
    } else {
      if (opaque_bitmap_format == BITMAP_FMT_BGR565 && rgba_data_is_opaque(data, w, total_h, n)) {
        ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGR565, data, n, o);
      } else if (opaque_bitmap_format == BITMAP_FMT_RGB565 && rgba_data_is_opaque(data, w, h, n)) {
        ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_RGB565, data, n, o);
      } else if (opaque_bitmap_format == BITMAP_FMT_BGRA8888) {
        ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGRA8888, data, n, o);
      } else {
        ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_RGBA8888, data, n, o);
      }
    }

    image->is_gif = TRUE;
    image->gif_frame_h = h;
    image->gif_frames_nr = z;
    image->gif_delays = delays;

    stbi_image_free((uint8_t*)(data));
  }

  return ret;
}

static ret_t image_loader_stb_load(image_loader_t* l, const asset_info_t* asset, bitmap_t* image) {
  ret_t ret = RET_OK;
  system_info_t* info = system_info();
  lcd_orientation_t o = LCD_ORIENTATION_0;
  bitmap_format_t opaque_bitmap_format = BITMAP_FMT_RGBA8888;
  bitmap_format_t transparent_bitmap_format = BITMAP_FMT_RGBA8888;
  return_value_if_fail(l != NULL && image != NULL && info != NULL, RET_BAD_PARAMS);

  if (asset->subtype != ASSET_TYPE_IMAGE_JPG && asset->subtype != ASSET_TYPE_IMAGE_PNG &&
      asset->subtype != ASSET_TYPE_IMAGE_GIF && asset->subtype != ASSET_TYPE_IMAGE_BMP) {
    return RET_NOT_IMPL;
  }

#if !defined(WITH_VGCANVAS_CAIRO) && defined(WITH_FAST_LCD_PORTRAIT)
  if (system_info()->flags & SYSTEM_INFO_FLAG_FAST_LCD_PORTRAIT) {
    o = info->lcd_orientation;
  }
#endif

#ifdef WITHOUT_FAST_LCD_PORTRAIT_FOR_IMAGE
  o = LCD_ORIENTATION_0;
#endif

#ifdef WITH_BITMAP_BGR565
  opaque_bitmap_format = BITMAP_FMT_BGR565;
#elif defined(WITH_BITMAP_RGB565)
  opaque_bitmap_format = BITMAP_FMT_RGB565;
#elif defined(WITH_BITMAP_BGR888)
  opaque_bitmap_format = BITMAP_FMT_BGR888;
#elif defined(WITH_BITMAP_RGB888)
  opaque_bitmap_format = BITMAP_FMT_RGB888;
#endif /*WITH_BITMAP_RGB565*/

#ifdef WITH_BITMAP_BGRA
  transparent_bitmap_format = BITMAP_FMT_BGRA8888;
#endif /*WITH_BITMAP_BGRA*/

#ifdef WITH_LCD_MONO
  opaque_bitmap_format = BITMAP_FMT_MONO;
#endif

  ret = stb_load_image(asset->subtype, asset->data, asset->size, image, transparent_bitmap_format,
                       opaque_bitmap_format, o);

#ifdef WITH_BITMAP_PREMULTI_ALPHA
  if (ret == RET_OK) {
    ret = bitmap_premulti_alpha(image);
  }
#endif /*WITH_BITMAP_RGB565*/
  return ret;
}

static const image_loader_t stb_loader = {.load = image_loader_stb_load};

image_loader_t* image_loader_stb() {
  return (image_loader_t*)&stb_loader;
}

/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#if !defined(WITH_GPU) && !defined(WITH_VGCANVAS_CAIRO) && defined(WITH_FAST_LCD_PORTRAIT)
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

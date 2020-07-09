/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(p, s)

#include "tkc/mem.h"

#include "stb/stb_image.h"
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
                     bool_t require_bgra, bool_t enable_bgr565, bool_t enable_rgb565) {
  int w = 0;
  int h = 0;
  int n = 0;
  ret_t ret = RET_FAIL;

  if (subtype != ASSET_TYPE_IMAGE_GIF) {
    uint8_t* data = NULL;
    uint8_t* stb_data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 0);
    return_value_if_fail(stb_data != NULL, RET_FAIL);

    if (n == 2) {
      n = 4;
      data = convert_2_to_4(stb_data, w, h);
    } else {
      data = stb_data;
    }
#ifdef WITH_LCD_MONO
    ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_MONO, data, n);
#else
    if (enable_bgr565 && rgba_data_is_opaque(data, w, h, n)) {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGR565, data, n);
    } else if (enable_rgb565 && rgba_data_is_opaque(data, w, h, n)) {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGB565, data, n);
    } else if (require_bgra) {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGRA8888, data, n);
    } else {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGBA8888, data, n);
    }
#endif /*WITH_LCD_MONO*/
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
#ifdef WITH_LCD_MONO
    ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_MONO, data, n);
#else
    if (enable_bgr565 && rgba_data_is_opaque(data, w, total_h, n)) {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGR565, data, n);
    } else if (require_bgra) {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGRA8888, data, n);
    } else {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_RGBA8888, data, n);
    }
#endif /*WITH_LCD_MONO*/

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
  bool_t require_bgra = FALSE;
  bool_t enable_bgr565 = FALSE;
  bool_t enable_rgb565 = FALSE;
  return_value_if_fail(l != NULL && image != NULL, RET_BAD_PARAMS);

  if (asset->subtype != ASSET_TYPE_IMAGE_JPG && asset->subtype != ASSET_TYPE_IMAGE_PNG &&
      asset->subtype != ASSET_TYPE_IMAGE_GIF && asset->subtype != ASSET_TYPE_IMAGE_BMP) {
    return RET_NOT_IMPL;
  }

#ifdef WITH_BITMAP_BGR565
  enable_bgr565 = TRUE;
#endif /*WITH_BITMAP_BGR565*/

#ifdef WITH_BITMAP_RGB565
  enable_rgb565 = TRUE;
#endif /*WITH_BITMAP_RGB565*/

#ifdef WITH_BITMAP_BGRA
  require_bgra = TRUE;
#endif /*WITH_BITMAP_BGRA*/

  ret = stb_load_image(asset->subtype, asset->data, asset->size, image, require_bgra, enable_bgr565,
                       enable_rgb565);

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

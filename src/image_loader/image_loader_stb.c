/**
 * File:   image_loader.h
 * Author: AWTK Develop Team
 * Brief:  stb image loader
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

ret_t stb_load_image(int32_t subtype, const uint8_t* buff, uint32_t buff_size, bitmap_t* image,
                     bool_t require_bgra, bool_t enable_bgr565) {
  int w = 0;
  int h = 0;
  int n = 0;
  ret_t ret = RET_FAIL;

  if (subtype != ASSET_TYPE_IMAGE_GIF) {
    uint8_t* data = stbi_load_from_memory(buff, buff_size, &w, &h, &n, 0);
    return_value_if_fail(data != NULL, RET_FAIL);

    if (enable_bgr565 && rgba_data_is_opaque(data, w, h, n)) {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGR565, data, n);
    } else if (require_bgra) {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_BGRA8888, data, n);
    } else {
      ret = bitmap_init_from_rgba(image, w, h, BITMAP_FMT_RGBA8888, data, n);
    }

    stbi_image_free((uint8_t*)(data));
  } else {
    int z = 0;
    int total_h = 0;
    int* delays = NULL;
    uint8_t* data = stbi_load_gif_from_memory(buff, buff_size, &delays, &w, &h, &z, &n, 0);

    return_value_if_fail(data != NULL, RET_FAIL);

    total_h = h * z;
    if (enable_bgr565 && rgba_data_is_opaque(data, w, total_h, n)) {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGR565, data, n);
    } else if (require_bgra) {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_BGRA8888, data, n);
    } else {
      ret = bitmap_init_from_rgba(image, w, total_h, BITMAP_FMT_RGBA8888, data, n);
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
  bool_t require_bgra = FALSE;
  bool_t enable_bgr565 = FALSE;
  return_value_if_fail(l != NULL && image != NULL, RET_BAD_PARAMS);

#ifdef WITH_BITMAP_BGR565
  enable_bgr565 = TRUE;
#endif /*WITH_BITMAP_BGR565*/

#ifdef WITH_BITMAP_BGRA
  require_bgra = TRUE;
#endif /*WITH_BITMAP_BGRA*/

  return stb_load_image(asset->subtype, asset->data, asset->size, image, require_bgra,
                        enable_bgr565);
}

static const image_loader_t stb_loader = {.load = image_loader_stb_load};

image_loader_t* image_loader_stb() {
  return (image_loader_t*)&stb_loader;
}

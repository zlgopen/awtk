/**
 * File:   soft_g2d.c
 * Author: AWTK Develop Team
 * Brief:  soft g2d
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
 * 2018-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/utils.h"
#include "base/pixel.h"
#include "blend/soft_g2d.h"
#include "base/pixel_pack_unpack.h"

#include "blend_image_bgr565_bgr565.h"
#include "blend_image_bgr565_bgra8888.h"
#include "blend_image_bgr565_rgba8888.h"
#include "blend_image_bgr888_bgr565.h"
#include "blend_image_bgr888_bgra8888.h"
#include "blend_image_bgr888_rgba8888.h"
#include "blend_image_bgra8888_bgr565.h"
#include "blend_image_bgra8888_bgra8888.h"
#include "blend_image_bgra8888_rgba8888.h"
#include "blend_image_rgb565_bgr565.h"
#include "blend_image_rgb565_bgra8888.h"
#include "blend_image_rgb565_rgba8888.h"
#include "blend_image_rgba8888_bgr565.h"
#include "blend_image_rgba8888_bgra8888.h"
#include "blend_image_rgba8888_rgba8888.h"
#include "fill_image_bgr565.h"
#include "fill_image_bgr888.h"
#include "fill_image_bgra8888.h"
#include "fill_image_rgb565.h"
#include "fill_image_rgba8888.h"
#include "rotate_image_bgr565.h"
#include "rotate_image_bgr888.h"
#include "rotate_image_bgra8888.h"
#include "rotate_image_rgb565.h"
#include "rotate_image_rgba8888.h"

ret_t soft_copy_image(bitmap_t* fb, bitmap_t* img, rect_t* src, xy_t dx, xy_t dy) {
  uint32_t i = 0;
  uint32_t size = 0;
  uint8_t* src_p = NULL;
  uint8_t* dst_p = NULL;
  uint32_t bpp = bitmap_get_bpp(fb);
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == img->format, RET_BAD_PARAMS);

  src_p = (uint8_t*)(img->data) + (src->y * img->w + src->x) * bpp;
  dst_p = (uint8_t*)(fb->data) + (dy * fb->w + dx) * bpp;

  if (fb->w == img->w && fb->h == img->h && src->w == img->w && src->x == 0) {
    size = (img->w * src->h);
    tk_pixel_copy(dst_p, src_p, size, bpp);

    return RET_OK;
  } else {
    size = src->w;
    for (i = 0; i < src->h; i++) {
      tk_pixel_copy(dst_p, src_p, size, bpp);
      dst_p += bpp * fb->w;
      src_p += bpp * img->w;
    }
  }

  return RET_OK;
}

ret_t soft_fill_rect(bitmap_t* fb, rect_t* dst, color_t c) {
  return_value_if_fail(fb != NULL && dst != NULL, RET_BAD_PARAMS);

  switch (fb->format) {
    case BITMAP_FMT_RGB565: {
      return fill_rgb565_rect(fb, dst, c);
    }
    case BITMAP_FMT_BGR565: {
      return fill_bgr565_rect(fb, dst, c);
    }
    case BITMAP_FMT_BGR888: {
      return fill_bgr888_rect(fb, dst, c);
    }
    case BITMAP_FMT_BGRA8888: {
      return fill_bgra8888_rect(fb, dst, c);
    }
    case BITMAP_FMT_RGBA8888: {
      return fill_rgba8888_rect(fb, dst, c);
    }
    default:
      break;
  }

  assert(!"not supported format");

  return RET_NOT_IMPL;
}

ret_t soft_rotate_image(bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o) {
  return_value_if_fail(o == LCD_ORIENTATION_90, RET_NOT_IMPL);
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fb->format == img->format, RET_BAD_PARAMS);
  return_value_if_fail(fb->w == img->h && fb->h == img->w, RET_BAD_PARAMS);

  switch (fb->format) {
    case BITMAP_FMT_RGB565: {
      return rotate_rgb565_image(fb, img, src, o);
    }
    case BITMAP_FMT_BGR565: {
      return rotate_bgr565_image(fb, img, src, o);
    }
    case BITMAP_FMT_BGR888: {
      return rotate_bgr888_image(fb, img, src, o);
    }
    case BITMAP_FMT_BGRA8888: {
      return rotate_bgra8888_image(fb, img, src, o);
    }
    case BITMAP_FMT_RGBA8888: {
      return rotate_rgba8888_image(fb, img, src, o);
    }
    default:
      break;
  }

  assert(!"not supported format");

  return RET_NOT_IMPL;
}

ret_t soft_blend_image(bitmap_t* fb, bitmap_t* img, rect_t* dst, rect_t* src, uint8_t alpha) {
  return_value_if_fail(fb != NULL && img != NULL && src != NULL && dst != NULL, RET_BAD_PARAMS);

  switch (fb->format) {
    case BITMAP_FMT_BGR565: {
      switch (img->format) {
        case BITMAP_FMT_BGR565: {
          if (dst->w == src->w && dst->h == src->h && alpha > 0xf8) {
            soft_copy_image(fb, img, src, dst->x, dst->y);
          } else {
            return blend_image_bgr565_bgr565(fb, img, dst, src, alpha);
          }
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgr565_rgba8888(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgr565_bgra8888(fb, img, dst, src, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_BGR888: {
      switch (img->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_bgr888_bgr565(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgr888_rgba8888(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgr888_bgra8888(fb, img, dst, src, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      switch (img->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_bgra8888_bgr565(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgra8888_rgba8888(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgra8888_bgra8888(fb, img, dst, src, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_RGBA8888: {
      switch (img->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_rgba8888_bgr565(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_rgba8888_rgba8888(fb, img, dst, src, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_rgba8888_bgra8888(fb, img, dst, src, alpha);
        }
        default:
          break;
      }
      break;
    }
    default:
      break;
  }

  return RET_NOT_IMPL;
}

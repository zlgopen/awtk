/**
 * File:   soft_g2d.c
 * Author: AWTK Develop Team
 * Brief:  software implemented image operations
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
 * 2018-05-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/pixel.h"
#include "blend/soft_g2d.h"
#include "base/pixel_pack_unpack.h"

#include "blend_image_bgr565_bgr565.h"
#include "blend_image_bgr565_rgb565.h"
#include "blend_image_bgr565_bgra8888.h"
#include "blend_image_bgr565_rgba8888.h"

#include "blend_image_rgb565_bgr565.h"
#include "blend_image_rgb565_rgb565.h"
#include "blend_image_rgb565_bgra8888.h"
#include "blend_image_rgb565_rgba8888.h"

#include "blend_image_bgr888_bgr565.h"
#include "blend_image_bgr888_rgb565.h"
#include "blend_image_bgr888_bgr888.h"
#include "blend_image_bgr888_bgra8888.h"
#include "blend_image_bgr888_rgba8888.h"

#include "blend_image_rgb888_bgr565.h"
#include "blend_image_rgb888_rgb565.h"
#include "blend_image_rgb888_rgb888.h"
#include "blend_image_rgb888_bgra8888.h"
#include "blend_image_rgb888_rgba8888.h"

#include "blend_image_bgra8888_bgr565.h"
#include "blend_image_bgra8888_rgb565.h"
#include "blend_image_bgra8888_bgra8888.h"
#include "blend_image_bgra8888_rgba8888.h"

#include "blend_image_rgba8888_bgr565.h"
#include "blend_image_rgba8888_rgb565.h"
#include "blend_image_rgba8888_bgra8888.h"
#include "blend_image_rgba8888_rgba8888.h"

#include "fill_image_rgb565.h"
#include "fill_image_bgr565.h"
#include "fill_image_bgr888.h"
#include "fill_image_rgb888.h"
#include "fill_image_bgra8888.h"
#include "fill_image_rgba8888.h"
#include "fill_image_argb8888.h"
#include "fill_image_abgr8888.h"

#include "rotate_image_bgr565.h"
#include "rotate_image_rgb565.h"
#include "rotate_image_bgr888.h"
#include "rotate_image_rgb888.h"
#include "rotate_image_bgra8888.h"
#include "rotate_image_rgba8888.h"

ret_t soft_copy_image(bitmap_t* dst, bitmap_t* src, rect_t* src_r, xy_t dx, xy_t dy) {
  uint8_t* src_p = NULL;
  uint8_t* dst_p = NULL;
  uint8_t* src_data = NULL;
  uint8_t* dst_data = NULL;
  uint32_t bpp = bitmap_get_bpp(dst);
  uint32_t dst_line_length = bitmap_get_line_length(dst);
  uint32_t src_line_length = bitmap_get_line_length(src);
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(dst->format == src->format, RET_BAD_PARAMS);

  src_data = bitmap_lock_buffer_for_read(src);
  dst_data = bitmap_lock_buffer_for_write(dst);
  return_value_if_fail(src_data != NULL && dst_data != NULL, RET_BAD_PARAMS);

  src_p = (uint8_t*)(src_data) + src_r->y * src_line_length + src_r->x * bpp;
  dst_p = (uint8_t*)(dst_data) + dy * dst_line_length + dx * bpp;
  if ((dst->w * bpp == dst_line_length) && (src->w * bpp == src_line_length) && dst->w == src->w &&
      dst->h == src->h && src_r->w == src->w && src_r->x == 0) {
    uint32_t size = (src_r->w * src_r->h);
    tk_pixel_copy(dst_p, src_p, size, bpp);

  } else {
    uint32_t i = 0;
    uint32_t size = src_r->w;

    for (i = 0; i < src_r->h; i++) {
      tk_pixel_copy(dst_p, src_p, size, bpp);
      dst_p += dst_line_length;
      src_p += src_line_length;
    }
  }
  bitmap_unlock_buffer(src);
  bitmap_unlock_buffer(dst);

  return RET_OK;
}

ret_t soft_clear_rect(bitmap_t* dst, rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_BAD_PARAMS);

  switch (dst->format) {
    case BITMAP_FMT_BGR565: {
      return clear_bgr565_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_RGBA8888: {
      return clear_rgba8888_rect(dst, dst_r, c);
    }
#ifndef LCD_BGR565_LITE
    case BITMAP_FMT_RGB565: {
      return clear_rgb565_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_BGR888: {
      return clear_bgr888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_RGB888: {
      return clear_rgb888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_BGRA8888: {
      return clear_bgra8888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_ARGB8888: {
      return clear_argb8888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_ABGR8888: {
      return clear_abgr8888_rect(dst, dst_r, c);
    }
#endif /*LCD_BGR565_LITE*/
    default:
      break;
  }

  assert(!"not supported format");

  return RET_NOT_IMPL;
}

ret_t soft_fill_rect(bitmap_t* dst, rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_BAD_PARAMS);

  switch (dst->format) {
    case BITMAP_FMT_BGR565: {
      return fill_bgr565_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_RGBA8888: {
      return fill_rgba8888_rect(dst, dst_r, c);
    }

#ifndef LCD_BGR565_LITE
    case BITMAP_FMT_RGB565: {
      return fill_rgb565_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_BGR888: {
      return fill_bgr888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_RGB888: {
      return fill_rgb888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_BGRA8888: {
      return fill_bgra8888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_ARGB8888: {
      return fill_argb8888_rect(dst, dst_r, c);
    }
    case BITMAP_FMT_ABGR8888: {
      return fill_abgr8888_rect(dst, dst_r, c);
    }
#endif /*LCD_BGR565_LITE*/
    default:
      break;
  }

  assert(!"not supported format");

  return RET_NOT_IMPL;
}

ret_t soft_rotate_image(bitmap_t* dst, bitmap_t* src, rect_t* src_r, lcd_orientation_t o) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(dst->format == src->format, RET_BAD_PARAMS);

  switch (dst->format) {
    case BITMAP_FMT_BGR565: {
      return rotate_bgr565_image(dst, src, src_r, o);
    }
    case BITMAP_FMT_RGBA8888: {
      return rotate_rgba8888_image(dst, src, src_r, o);
    }
#ifndef LCD_BGR565_LITE
    case BITMAP_FMT_RGB565: {
      return rotate_rgb565_image(dst, src, src_r, o);
    }
    case BITMAP_FMT_BGR888: {
      return rotate_bgr888_image(dst, src, src_r, o);
    }
    case BITMAP_FMT_RGB888: {
      return rotate_rgb888_image(dst, src, src_r, o);
    }
    case BITMAP_FMT_BGRA8888: {
      return rotate_bgra8888_image(dst, src, src_r, o);
    }
#endif /*LCD_BGR565_LITE*/
    default:
      break;
  }

  assert(!"not supported format");

  return RET_NOT_IMPL;
}

ret_t soft_blend_image(bitmap_t* dst, bitmap_t* src, rect_t* dst_r, rect_t* src_r, uint8_t alpha) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL && dst_r != NULL,
                       RET_BAD_PARAMS);

  switch (dst->format) {
    case BITMAP_FMT_BGR565: {
      switch (src->format) {
        case BITMAP_FMT_BGR565: {
          if (dst_r->w == src_r->w && dst_r->h == src_r->h && alpha > 0xf8) {
            return soft_copy_image(dst, src, src_r, dst_r->x, dst_r->y);
          } else {
            return blend_image_bgr565_bgr565(dst, src, dst_r, src_r, alpha);
          }
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgr565_rgba8888(dst, src, dst_r, src_r, alpha);
        }
#ifndef LCD_BGR565_LITE
        case BITMAP_FMT_RGB565: {
          return blend_image_bgr565_rgb565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgr565_bgra8888(dst, src, dst_r, src_r, alpha);
        }
#endif /*LCD_BGR565_LITE*/
        default:
          break;
      }
      break;
    }
#ifndef LCD_BGR565_LITE
    case BITMAP_FMT_RGB565: {
      switch (src->format) {
        case BITMAP_FMT_RGB565: {
          if (dst_r->w == src_r->w && dst_r->h == src_r->h && alpha > 0xf8) {
            return soft_copy_image(dst, src, src_r, dst_r->x, dst_r->y);
          } else {
            return blend_image_rgb565_rgb565(dst, src, dst_r, src_r, alpha);
          }
        }
        case BITMAP_FMT_BGR565: {
          return blend_image_rgb565_bgr565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_rgb565_rgba8888(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_rgb565_bgra8888(dst, src, dst_r, src_r, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_BGR888: {
      switch (src->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_bgr888_bgr565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGB565: {
          return blend_image_bgr888_rgb565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGR888: {
          if (dst_r->w == src_r->w && dst_r->h == src_r->h && alpha > 0xf8) {
            return soft_copy_image(dst, src, src_r, dst_r->x, dst_r->y);
          } else {
            return blend_image_bgr888_bgr888(dst, src, dst_r, src_r, alpha);
          }
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgr888_rgba8888(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgr888_bgra8888(dst, src, dst_r, src_r, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_RGB888: {
      switch (src->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_rgb888_bgr565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGB565: {
          return blend_image_rgb888_rgb565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGB888: {
          if (dst_r->w == src_r->w && dst_r->h == src_r->h && alpha > 0xf8) {
            return soft_copy_image(dst, src, src_r, dst_r->x, dst_r->y);
          } else {
            return blend_image_rgb888_rgb888(dst, src, dst_r, src_r, alpha);
          }
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_rgb888_rgba8888(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_rgb888_bgra8888(dst, src, dst_r, src_r, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      switch (src->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_bgra8888_bgr565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGB565: {
          return blend_image_bgra8888_rgb565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_bgra8888_rgba8888(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_bgra8888_bgra8888(dst, src, dst_r, src_r, alpha);
        }
        default:
          break;
      }
      break;
    }
    case BITMAP_FMT_RGBA8888: {
      switch (src->format) {
        case BITMAP_FMT_BGR565: {
          return blend_image_rgba8888_bgr565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGB565: {
          return blend_image_rgba8888_rgb565(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_RGBA8888: {
          return blend_image_rgba8888_rgba8888(dst, src, dst_r, src_r, alpha);
        }
        case BITMAP_FMT_BGRA8888: {
          return blend_image_rgba8888_bgra8888(dst, src, dst_r, src_r, alpha);
        }
        default:
          break;
      }
      break;
    }
#endif /*LCD_BGR565_LITE*/
    default:
      break;
  }

  return RET_NOT_IMPL;
}

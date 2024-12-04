
/**
 * File:   pixman_g2d.c
 * Author: AWTK Develop Team
 * Brief:  g2d base on pixman.
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2024-12-05 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifdef WITH_PIXMAN_G2D

#include "pixman/config.h"
#include "pixman/pixman-private.h"
#include "pixman/pixman.h"
#undef return_if_fail

#include "base/g2d.h"
#include "blend/soft_g2d.h"
#include "blend/image_g2d.h"

static pixman_format_code_t pixman_format_code_from_bitmap_fmt(bitmap_format_t format) {
  switch (format) {
    case BITMAP_FMT_BGR565:
      return PIXMAN_r5g6b5;
    case BITMAP_FMT_RGB565:
      return PIXMAN_b5g6r5;
    case BITMAP_FMT_BGRA8888:
      return PIXMAN_a8r8g8b8;
    case BITMAP_FMT_RGBA8888:
      return PIXMAN_a8b8g8r8;
    case BITMAP_FMT_ARGB8888:
      return PIXMAN_b8g8r8a8;
    case BITMAP_FMT_ABGR8888:
      return PIXMAN_r8g8b8a8;
    case BITMAP_FMT_RGB888:
      return PIXMAN_b8g8r8;
    case BITMAP_FMT_BGR888:
      return PIXMAN_r8g8b8;
    case BITMAP_FMT_GRAY:
      return PIXMAN_a8;
    case BITMAP_FMT_MONO:
      return PIXMAN_a1;
    default:
      assert(0);
      return PIXMAN_a8b8g8r8;
  }
}

static pixman_image_t* s_share_pixman_image_dst = NULL;
static pixman_image_t* s_share_pixman_image_src = NULL;

static pixman_image_t* pixman_image_get_dst(bitmap_t* fb) {
  pixman_format_code_t format = pixman_format_code_from_bitmap_fmt(fb->format);
  uint8_t* data = bitmap_lock_buffer_for_write(fb);
  int line_length = bitmap_get_line_length(fb);
  return_value_if_fail(data != NULL, NULL);

  if (s_share_pixman_image_dst == NULL) {
    s_share_pixman_image_dst =
        pixman_image_create_bits(format, fb->w, fb->h, (uint32_t*)data, line_length);
  } else {
    _pixman_bits_image_init(s_share_pixman_image_dst, format, fb->w, fb->h, (uint32_t*)data,
                            line_length / sizeof(uint32_t), FALSE);
  }

  return s_share_pixman_image_dst;
}

static pixman_image_t* pixman_image_get_src(bitmap_t* fb) {
  pixman_format_code_t format = pixman_format_code_from_bitmap_fmt(fb->format);
  uint8_t* data = bitmap_lock_buffer_for_read(fb);
  int line_length = bitmap_get_line_length(fb);
  return_value_if_fail(data != NULL, NULL);

  if (s_share_pixman_image_src == NULL) {
    s_share_pixman_image_src =
        pixman_image_create_bits(format, fb->w, fb->h, (uint32_t*)data, line_length);
  } else {
    _pixman_bits_image_init(s_share_pixman_image_src, format, fb->w, fb->h, (uint32_t*)data,
                            line_length / sizeof(uint32_t), FALSE);
  }

  return s_share_pixman_image_src;
}

static pixman_color_t pixman_color_from_color(color_t c) {
  pixman_color_t color;

  color.red = c.rgba.r << 8;
  color.green = c.rgba.g << 8;
  color.blue = c.rgba.b << 8;
  color.alpha = c.rgba.a << 8;

  return color;
}

static pixman_rectangle16_t pixman_rectangle16_from_rect(const rect_t* r) {
  pixman_rectangle16_t rect = {0, 0, 0, 0};
  return_value_if_fail(r != NULL, rect);

  rect.x = r->x;
  rect.y = r->y;
  rect.width = r->w;
  rect.height = r->h;

  return rect;
}

ret_t g2d_fill_rect(bitmap_t* fb, const rect_t* dst, color_t c) {
  pixman_image_t* img = pixman_image_get_dst(fb);
  pixman_color_t fill_color = pixman_color_from_color(c);
  pixman_rectangle16_t rect = pixman_rectangle16_from_rect(dst);
  return_value_if_fail(fb != NULL && dst != NULL, RET_OK);

  if (c.rgba.a == 0xff) {
    pixman_image_fill_rectangles(PIXMAN_OP_SRC, img, &fill_color, 1, &rect);
  } else {
    pixman_image_fill_rectangles(PIXMAN_OP_OVER, img, &fill_color, 1, &rect);
  }
  bitmap_unlock_buffer(fb);

  return RET_OK;
}

ret_t g2d_copy_image(bitmap_t* fb, bitmap_t* img, const rect_t* src, xy_t x, xy_t y) {
  pixman_image_t* dst_img = pixman_image_get_dst(fb);
  pixman_image_t* src_img = pixman_image_get_src(img);
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_OK);

  pixman_image_composite(PIXMAN_OP_SRC, src_img, NULL, dst_img, src->x, src->y, 0, 0, x, y, src->w,
                         src->h);

  bitmap_unlock_buffer(fb);
  bitmap_unlock_buffer(img);

  return RET_OK;
}

ret_t g2d_rotate_image(bitmap_t* fb, bitmap_t* img, const rect_t* src, lcd_orientation_t o) {
  return RET_NOT_IMPL;
}

ret_t g2d_blend_image(bitmap_t* fb, bitmap_t* img, const rect_t* dst, const rect_t* src,
                      uint8_t global_alpha) {
  pixman_image_t* dst_img = pixman_image_get_dst(fb);
  pixman_image_t* src_img = pixman_image_get_src(img);
  return_value_if_fail(fb != NULL && img != NULL && src != NULL, RET_OK);

  pixman_image_composite(PIXMAN_OP_OVER, src_img, NULL, dst_img, src->x, src->y, 0, 0, dst->x,
                         dst->y, src->w, src->h);

  bitmap_unlock_buffer(fb);
  bitmap_unlock_buffer(img);

  return RET_OK;
}

ret_t tk_g2d_init(void) {
  return RET_OK;
}

ret_t tk_g2d_deinit(void) {
  if (s_share_pixman_image_dst != NULL) {
    pixman_image_unref(s_share_pixman_image_dst);
    s_share_pixman_image_dst = NULL;
  }

  if (s_share_pixman_image_src != NULL) {
    pixman_image_unref(s_share_pixman_image_src);
    s_share_pixman_image_src = NULL;
  }
  return RET_OK;
}
#endif /*WITH_PIXMAN_G2D*/

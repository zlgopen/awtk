/**
 * File:   image_g2d.c
 * Author: AWTK Develop Team
 * Brief:  image 2d operations.
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/g2d.h"
#include "blend/soft_g2d.h"
#include "blend/image_g2d.h"

#ifndef AWTK_WEB
#if defined(WITH_G2D) && !defined(WITHOUT_DEFAULT_G2D)

ret_t TK_WEAK g2d_fill_rect(bitmap_t* fb, const rect_t* dst, color_t c) {
  (void)fb;
  (void)dst;
  (void)c;
  return RET_NOT_IMPL;
}

ret_t TK_WEAK g2d_copy_image(bitmap_t* fb, bitmap_t* img, const rect_t* src, xy_t x, xy_t y) {
  (void)fb;
  (void)img;
  (void)x;
  (void)y;
  return RET_NOT_IMPL;
}

ret_t TK_WEAK g2d_rotate_image(bitmap_t* fb, bitmap_t* img, const rect_t* src,
                               lcd_orientation_t o) {
  (void)fb;
  (void)img;
  (void)src;
  (void)o;
  return RET_NOT_IMPL;
}

ret_t TK_WEAK g2d_blend_image(bitmap_t* fb, bitmap_t* img, const rect_t* dst, const rect_t* src,
                              uint8_t global_alpha) {
  (void)fb;
  (void)img;
  (void)dst;
  (void)src;
  (void)global_alpha;
  return RET_NOT_IMPL;
}

#ifdef WITH_FAST_LCD_PORTRAIT
ret_t TK_WEAK g2d_rotate_image_ex(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx,
                                  xy_t dy, lcd_orientation_t o) {
  (void)dst;
  (void)src;
  (void)src_r;
  (void)dx;
  (void)dy;
  (void)o;
  return RET_NOT_IMPL;
}

ret_t TK_WEAK g2d_blend_image_rotate(bitmap_t* dst, bitmap_t* src, const rectf_t* dst_r,
                                     const rectf_t* src_r, uint8_t alpha, lcd_orientation_t o) {
  (void)dst;
  (void)src;
  (void)dst_r;
  (void)src_r;
  (void)alpha;
  (void)o;
  return RET_NOT_IMPL;
}
#endif

#endif

ret_t image_fill(bitmap_t* dst, const rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_OK);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= bitmap_get_physical_width(dst));
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= bitmap_get_physical_height(dst));

#ifdef WITH_G2D
  if (g2d_fill_rect(dst, dst_r, c) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_fill_rect(dst, dst_r, c);
}

ret_t image_clear(bitmap_t* dst, const rect_t* dst_r, color_t c) {
  return_value_if_fail(dst != NULL && dst_r != NULL, RET_OK);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= bitmap_get_physical_width(dst));
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= bitmap_get_physical_height(dst));

  return soft_clear_rect(dst, dst_r, c);
}

ret_t image_copy(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_OK);

  assert(dx >= 0 && (dx + src_r->w) <= bitmap_get_physical_width(dst));
  assert(dy >= 0 && (dy + src_r->h) <= bitmap_get_physical_height(dst));

#ifdef WITH_G2D
  if (g2d_copy_image(dst, src, src_r, dx, dy) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_copy_image(dst, src, src_r, dx, dy);
}

ret_t image_rotate(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, lcd_orientation_t o) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_OK);

  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    assert(src_r->w <= bitmap_get_physical_width(dst));
    assert(src_r->h <= bitmap_get_physical_height(dst));
  } else if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
    assert(src_r->w <= bitmap_get_physical_height(dst));
    assert(src_r->h <= bitmap_get_physical_width(dst));
  }

#ifdef WITH_G2D
  if (g2d_rotate_image(dst, src, src_r, o) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_rotate_image(dst, src, src_r, o);
}

ret_t image_blend(bitmap_t* dst, bitmap_t* src, const rectf_t* dst_r, const rectf_t* src_r,
                  uint8_t global_alpha) {
  return_value_if_fail(dst != NULL && src != NULL && dst_r != NULL && src_r != NULL,
                       RET_BAD_PARAMS);

  assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= bitmap_get_physical_width(dst));
  assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= bitmap_get_physical_height(dst));

#ifdef WITH_G2D
  /* 
   * 由于裁剪区的作用下，只重绘原来已经缩放的图片的一小部分区域
   * 由于传入的是整型矩形，所以用出现精度丢失的问题
   * 会导致原来缩放的图片和重绘的小区域对不上的问题，所以禁止硬件的 g2d 做缩放效果。
   */
  if (src_r->w == dst_r->w && src_r->h == dst_r->h) {
    rect_t tmp_src_r = rect_from_rectf(src_r);
    rect_t tmp_dst_r = rect_from_rectf(dst_r);
    if (g2d_blend_image(dst, src, (const rect_t*)&tmp_dst_r, (const rect_t*)&tmp_src_r,
                        global_alpha) == RET_OK) {
      return RET_OK;
    }
  }
#endif /*WITH_G2D*/

  return soft_blend_image(dst, src, dst_r, src_r, global_alpha);
}

#ifdef WITH_FAST_LCD_PORTRAIT
ret_t image_rotate_ex(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy,
                      lcd_orientation_t o) {
  return_value_if_fail(dst != NULL && src != NULL && src_r != NULL, RET_OK);

  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    assert(src_r->w <= bitmap_get_physical_width(dst));
    assert(src_r->h <= bitmap_get_physical_height(dst));
  } else if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
    assert(src_r->w <= bitmap_get_physical_height(dst));
    assert(src_r->h <= bitmap_get_physical_width(dst));
  }

#ifdef WITH_G2D
  if (g2d_rotate_image_ex(dst, src, src_r, dx, dy, o) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_rotate_image_ex(dst, src, src_r, dx, dy, o);
}

ret_t image_rotate_blend(bitmap_t* dst, bitmap_t* src, const rectf_t* dst_r, const rectf_t* src_r,
                         uint8_t global_alpha, lcd_orientation_t o) {
  return_value_if_fail(dst != NULL && src != NULL && dst_r != NULL && src_r != NULL,
                       RET_BAD_PARAMS);
  if (o == LCD_ORIENTATION_90 || o == LCD_ORIENTATION_270) {
    assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= bitmap_get_physical_height(dst));
    assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= bitmap_get_physical_width(dst));
  } else {
    assert(dst_r->x >= 0 && (dst_r->x + dst_r->w) <= bitmap_get_physical_width(dst));
    assert(dst_r->y >= 0 && (dst_r->y + dst_r->h) <= bitmap_get_physical_height(dst));
  }

#ifdef WITH_G2D
  if (src_r->w == dst_r->w && src_r->h == dst_r->h) {
    if (g2d_blend_image_rotate(dst, src, dst_r, src_r, global_alpha, o) == RET_OK) {
      return RET_OK;
    }
  }
#endif /*WITH_G2D*/

  return soft_blend_image_rotate(dst, src, dst_r, src_r, global_alpha, o);
}
#else

ret_t image_rotate_ex(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy,
                      lcd_orientation_t o) {
  (void)dst;
  (void)src;
  (void)src_r;
  (void)dx;
  (void)dy;
  (void)o;
  return RET_NOT_IMPL;
}

ret_t image_rotate_blend(bitmap_t* dst, bitmap_t* src, const rectf_t* dst_r, const rectf_t* src_r,
                         uint8_t global_alpha, lcd_orientation_t o) {
  (void)dst;
  (void)src;
  (void)dst_r;
  (void)src_r;
  (void)global_alpha;
  (void)o;
  return RET_NOT_IMPL;
}
#endif

#if defined(WITH_G2D)
#include "tkc/time_now.h"

ret_t image_g2d_benchmark(void) {
  int w = 1024;
  int h = 1024;
  int i = 0;
  int n = 10;
  rect_t r = rect_init(0, 0, w, h);
  color_t c = color_init(0x80, 0x80, 0x80, 0xff);
  bitmap_t* img_rgba8888 = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_RGBA8888);
  bitmap_t* img_rgba8888_src = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_RGBA8888);
  bitmap_t* img_rgb565 = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_BGR565);
  bitmap_t* img_rgb565_src = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_BGR565);
  uint64_t start = time_now_us();
  uint32_t cost = 0; 
  for (i = 0; i < n; i++) {
    soft_fill_rect(img_rgba8888, &r, c);
    soft_fill_rect(img_rgb565, &r, c);
  }
  cost = time_now_us() - start;
  log_debug("soft_fill_rect cost=%u\n", cost);
  
  start = time_now_us();
  for (i = 0; i < n; i++) {
    g2d_fill_rect(img_rgba8888, &r, c);
    g2d_fill_rect(img_rgb565, &r, c);
  }
  cost = time_now_us() - start;
  log_debug("g2d_fill_rect cost=%u\n", cost);
  
  start = time_now_us();
  for (i = 0; i < n; i++) {
    soft_copy_image(img_rgb565, img_rgb565_src, &r, 0, 0);
  }
  cost = time_now_us() - start;
  log_debug("soft_copy_image cost=%u\n", cost);
  
  start = time_now_us();
  for (i = 0; i < n; i++) {
    g2d_copy_image(img_rgb565, img_rgb565_src, &r, 0, 0);
  }
  cost = time_now_us() - start;
  log_debug("g2d_copy_image cost=%u\n", cost);
  
  start = time_now_us();
  for (i = 0; i < n; i++) {
    rectf_t rf = {0, 0, w, h};
    soft_blend_image(img_rgba8888, img_rgba8888_src, &rf, &rf, 0xff);
  }
  cost = time_now_us() - start;
  log_debug("soft_blend_image cost=%u\n", cost);
  
  start = time_now_us();
  for (i = 0; i < n; i++) {
    g2d_blend_image(img_rgba8888, img_rgba8888_src, &r, &r, 0xff);
  }
  cost = time_now_us() - start;
  log_debug("g2d_blend_image cost=%u\n", cost);
   
  bitmap_destroy(img_rgba8888);
  bitmap_destroy(img_rgba8888_src);
  bitmap_destroy(img_rgb565);
  bitmap_destroy(img_rgb565_src);

  return RET_OK;
}

#endif/*WITH_G2D*/

#endif/*ndef AWTK_WEB*/

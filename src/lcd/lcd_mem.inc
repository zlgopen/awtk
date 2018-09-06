/**
 * File:   lcd_mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  mem implemented lcd interface/
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "lcd/lcd_mem.h"
#include "base/vgcanvas.h"
#include "blend/image_g2d.h"
#include "base/system_info.h"

static pixel_t* lcd_mem_init_drawing_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* fbuff = mem->offline_fb;

  if (lcd->draw_mode == LCD_DRAW_ANIMATION && mem->online_fb && !lcd_is_swappable(lcd)) {
    fbuff = mem->online_fb;
  }

  if (fb != NULL) {
    fb->w = lcd->w;
    fb->h = lcd->h;
    fb->data = fbuff;
    fb->format = LCD_FORMAT;
  }

  return (pixel_t*)fbuff;
}

static bitmap_t* lcd_mem_init_online_fb(lcd_t* lcd, bitmap_t* fb, lcd_orientation_t o) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  fb->flags = 0;
  fb->format = mem->format;
  fb->data = mem->online_fb;
  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    fb->w = lcd->w;
    fb->h = lcd->h;
  } else {
    fb->h = lcd->w;
    fb->w = lcd->h;
  }

  return fb;
}

static bitmap_t* lcd_mem_init_offline_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  fb->flags = 0;
  fb->w = lcd->w;
  fb->h = lcd->h;
  fb->data = mem->offline_fb;
  fb->format = mem->format;

  return fb;
}

static ret_t lcd_mem_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  return RET_OK;
}

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  color_t c = lcd->stroke_color;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  pixel_t* p = lcd_mem_init_drawing_fb(lcd, NULL) + y * lcd->w + x;

  if (a >= TK_OPACITY_ALPHA) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < w; i++) {
      p[i] = pixel;
    }
  } else if (a >= TK_TRANSPARENT_ALPHA) {
    c.rgba.a = a;
    for (i = 0; i < w; i++) {
      p[i] = blend_pixel(p[i], c);
    }
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  wh_t w = lcd->w;
  color_t c = lcd->stroke_color;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  pixel_t* p = lcd_mem_init_drawing_fb(lcd, NULL) + y * lcd->w + x;

  if (a >= TK_OPACITY_ALPHA) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < h; i++) {
      *p = pixel;
      p += w;
    }
  } else if (a >= TK_TRANSPARENT_ALPHA) {
    c.rgba.a = a;
    for (i = 0; i < h; i++) {
      *p = blend_pixel(*p, c);
      p += w;
    }
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  wh_t w = lcd->w;
  color_t c = lcd->stroke_color;
  pixel_t pixel = color_to_pixel(c);
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  pixel_t* fbuff = lcd_mem_init_drawing_fb(lcd, NULL);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    pixel_t* p = fbuff + point->y * w + point->x;
    if (a >= TK_OPACITY_ALPHA) {
      *p = pixel;
    } else if (a >= TK_TRANSPARENT_ALPHA) {
      *p = blend_pixel(*p, c);
    }
  }

  return RET_OK;
}

static color_t lcd_mem_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  pixel_t* fbuff = lcd_mem_init_drawing_fb(lcd, NULL);
  pixel_t p = fbuff[y * lcd->w + x];

  return to_color(p);
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t r;
  bitmap_t fb;
  color_t c = lcd->fill_color;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;

  if (a >= TK_OPACITY_ALPHA) {
    r = rect_init(x, y, w, h);
    lcd_mem_init_drawing_fb(lcd, &fb);

    return image_fill(&fb, &r, lcd->fill_color);
  } else if (a >= TK_TRANSPARENT_ALPHA) {
    uint32_t i = 0;
    color_t stroke_color = lcd->stroke_color;
    lcd->stroke_color = c;
    for (i = 0; i < h; i++) {
      lcd_draw_hline(lcd, x, y + i, w);
    }
    lcd->stroke_color = stroke_color;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t w = lcd->w;
  color_t color = lcd->text_color;
  uint8_t global_alpha = lcd->global_alpha;
  pixel_t* fbuff = lcd_mem_init_drawing_fb(lcd, NULL);
  pixel_t* dst_p = fbuff + y * w + x;
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;
  pixel_t pixel = color_to_pixel(color);

  for (j = 0; j < sh; j++) {
    pixel_t* d = dst_p;
    const uint8_t* s = src_p;
    for (i = 0; i < sw; i++, d++, s++) {
      uint8_t a = global_alpha > TK_OPACITY_ALPHA ? *s : ((*s * global_alpha) >> 8);

      if (a >= TK_OPACITY_ALPHA) {
        *d = pixel;
      } else if (a >= TK_TRANSPARENT_ALPHA) {
        color.rgba.a = a;
        *d = blend_pixel(*d, color);
      }
    }
    src_p += glyph->w;
    dst_p += w;
  }

  return RET_OK;
}

#if defined(WITH_PXIMAN)
#include "pixman.h"

static pixman_image_t* to_pixman_image(const uint8_t* data, bitmap_format_t format, int32_t w,
                                       int32_t h) {
  int32_t bpp = 0;
  pixman_format_code_t f = PIXMAN_b8g8r8a8;

  switch (format) {
    case BITMAP_FMT_RGBA: {
      bpp = 4;
      f = PIXMAN_r8g8b8a8;
      break;
    }
    case BITMAP_FMT_BGRA: {
      bpp = 4;
      f = PIXMAN_a8r8g8b8;
      break;
    }
    case BITMAP_FMT_RGB565: {
      bpp = 2;
      f = PIXMAN_r5g6b5;
      break;
    }
    default: { return NULL; }
  }

  return pixman_image_create_bits_no_clear(f, w, h, (uint32_t*)data, w * bpp);
}
#endif /*WITH_PXIMAN*/

static ret_t lcd_mem_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
#if defined(WITH_VGCANVAS)
  matrix_t* m = &(info->matrix);
  rect_t* s = &(info->src);
  rect_t* d = &(info->dst);
  rect_t* clip = &(info->clip);
  bitmap_t* img = info->img;
  vgcanvas_t* canvas = lcd_get_vgcanvas(lcd);

  if (canvas != NULL) {
    vgcanvas_save(canvas);
    vgcanvas_set_transform(canvas, m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
    vgcanvas_draw_image(canvas, info->img, s->x, s->y, s->w, s->h, d->x, d->y, d->w, d->h);
    vgcanvas_restore(canvas);

    return RET_OK;
  }
#elif defined(WITH_PXIMAN)
/*
  matrix_t* m = &(info->matrix);
  rect_t* s = &(info->src);
  rect_t* d = &(info->dst);
  rect_t* clip = &(info->clip);
  bitmap_t* img = info->img;

  bitmap_t fb;
  pixman_transform_t t;
  lcd_mem_init_drawing_fb(lcd, &fb);
  pixman_image_t* d_img = to_pixman_image(fb.data, fb.format, fb.w, fb.h);
  pixman_image_t* s_img = to_pixman_image(img->data, img->format, img->w, img->h);
  pixman_transform_init_identity(&t);
  pixman_image_set_transform(s_img, &t);
  pixman_image_composite(PIXMAN_OP_SRC, s_img, NULL, d_img, s->x, s->y, 0, 0, d->x, d->y, d->w,
  d->h);

  pixman_image_unref(d_img);
  pixman_image_unref(s_img);
*/
#else
  (void)lcd;
  (void)info;

  return RET_NOT_IMPL;
#endif /*WITH_PIXMAN*/
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  bitmap_t fb;
  ret_t ret = RET_OK;
  system_info_t* info = system_info();
  lcd_orientation_t o = info->lcd_orientation;
  bool_t is_opaque = (img->flags & BITMAP_FLAG_OPAQUE || img->format == BITMAP_FMT_RGB565);

  lcd_mem_init_drawing_fb(lcd, &fb);

  if (img->format == fb.format && is_opaque && src->w == dst->w && src->h == dst->h &&
      lcd->global_alpha >= TK_OPACITY_ALPHA) {
    rect_t r;
    xy_t dx = 0;
    xy_t dy = 0;

    if (o == LCD_ORIENTATION_90 && lcd->draw_mode == LCD_DRAW_ANIMATION) {
      fb.w = lcd->h;
      fb.h = lcd->w;

      r.w = src->h;
      r.h = src->w;
      r.x = src->y;
      r.y = img->h - src->x - src->w;

      dx = dst->y;
      dy = fb.h - dst->x - dst->w;
    } else {
      r = *src;
      dx = dst->x;
      dy = dst->y;
    }

    ret = image_copy(&fb, img, &r, dx, dy);
  } else {
    ret = image_blend(&fb, img, dst, src, lcd->global_alpha);
  }

  return ret;
}

static vgcanvas_t* lcd_mem_get_vgcanvas(lcd_t* lcd) {
  bitmap_t fb;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  lcd_mem_init_drawing_fb(lcd, &fb);

  if (mem->vgcanvas == NULL) {
    mem->vgcanvas = vgcanvas_create(fb.w, fb.h, (bitmap_format_t)(fb.format), (uint32_t*)(fb.data));
  }

  return mem->vgcanvas;
}

static ret_t snapshot_destroy(bitmap_t* img) {
  TKMEM_FREE(img->data);

  return RET_OK;
}

static ret_t lcd_mem_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  bitmap_t fb;
  uint32_t size = 0;
  void* data = NULL;
  lcd_mem_init_drawing_fb(lcd, &fb);
  system_info_t* info = system_info();
  lcd_orientation_t o = info->lcd_orientation;

  memset(img, 0x00, sizeof(bitmap_t));
  img->format = fb.format;
  img->flags = BITMAP_FLAG_OPAQUE;

  if (img->format == BITMAP_FMT_RGBA || img->format == BITMAP_FMT_BGRA) {
    size = lcd->w * lcd->h * 4;
  } else if (img->format == BITMAP_FMT_RGB565) {
    size = lcd->w * lcd->h * 2;
  } else {
    return_value_if_fail(size > 0, RET_FAIL);
  }

  data = TKMEM_ALLOC(size);
  return_value_if_fail(data != NULL, RET_FAIL);

  img->data = (uint8_t*)data;
  img->destroy = snapshot_destroy;

  if (auto_rotate && o == LCD_ORIENTATION_90) {
    rect_t r;
    img->w = fb.h;
    img->h = fb.w;
    r = rect_init(0, 0, fb.w, fb.h);

    image_rotate(img, &fb, &r, o);
  } else {
    img->w = fb.w;
    img->h = fb.h;

    memcpy((void*)(img->data), (void*)(fb.data), size);
  }

  return RET_OK;
}

static ret_t lcd_mem_flush(lcd_t* lcd) {
  bitmap_t online_fb;
  bitmap_t offline_fb;
  rect_t* r = &(lcd->dirty_rect);
  system_info_t* info = system_info();
  lcd_orientation_t o = info->lcd_orientation;

  assert(o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_90);
  return_value_if_fail(o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_90, RET_NOT_IMPL);

  lcd_mem_init_drawing_fb(lcd, &offline_fb);
  lcd_mem_init_online_fb(lcd, &online_fb, o);

  if (o == LCD_ORIENTATION_0) {
    return image_copy(&online_fb, &offline_fb, r, r->x, r->y);
  } else {
    return image_rotate(&online_fb, &offline_fb, r, o);
  }
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) {
  if (lcd->draw_mode == LCD_DRAW_OFFLINE) {
    return RET_OK;
  }

  /*FIXME: Handle ROTATE*/

  if (lcd_is_swappable(lcd)) {
    return lcd_swap(lcd);
  } else {
    if (lcd->draw_mode == LCD_DRAW_ANIMATION) {
      return RET_OK;
    }
    return lcd_mem_flush(lcd);
  }
}

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  TKMEM_FREE(lcd);

  return RET_OK;
}

static ret_t lcd_mem_destroy_offline_fb(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  TKMEM_FREE(mem->offline_fb);
  TKMEM_FREE(lcd);

  return RET_OK;
}

static lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  lcd_t* base = NULL;
  system_info_t* info = system_info();
  lcd_mem_t* lcd = TKMEM_ZALLOC(lcd_mem_t);

  return_value_if_fail(lcd != NULL, NULL);

  if (alloc) {
    lcd->offline_fb = (uint8_t*)TKMEM_ALLOC(w * h * sizeof(pixel_t));
    return_value_if_fail(lcd->offline_fb != NULL, NULL);
  }

  base = &(lcd->base);
  base->begin_frame = lcd_mem_begin_frame;
  base->draw_vline = lcd_mem_draw_vline;
  base->draw_hline = lcd_mem_draw_hline;
  base->fill_rect = lcd_mem_fill_rect;
  base->draw_image = lcd_mem_draw_image;
  base->draw_image_matrix = lcd_mem_draw_image_matrix;
  base->draw_glyph = lcd_mem_draw_glyph;
  base->draw_points = lcd_mem_draw_points;
  base->get_point_color = lcd_mem_get_point_color;
  base->get_vgcanvas = lcd_mem_get_vgcanvas;
  base->take_snapshot = lcd_mem_take_snapshot;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = alloc ? lcd_mem_destroy_offline_fb : lcd_mem_destroy;

  base->w = w;
  base->h = h;
  base->ratio = 1;
  base->type = LCD_FRAMEBUFFER;
  base->global_alpha = 0xff;
  lcd->format = LCD_FORMAT;

  info->lcd_w = base->w;
  info->lcd_h = base->h;
  info->lcd_type = base->type;
  info->device_pixel_ratio = 1;

  return base;
}

static lcd_t* lcd_mem_create_double_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->offline_fb = offline_fb;
  mem->online_fb = online_fb;

  return lcd;
}

static lcd_t* lcd_mem_create_three_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb,
                                      uint8_t* next_fb) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->offline_fb = offline_fb;
  mem->online_fb = online_fb;
  mem->next_fb = next_fb;

  return lcd;
}

static lcd_t* lcd_mem_create_single_fb(wh_t w, wh_t h, uint8_t* fbuff) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->offline_fb = fbuff;
  mem->online_fb = NULL;

  return lcd;
}

/**
 * File:   lcd_mem.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  mem implemented lcd interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/darray.h"
#include "lcd/lcd_mem.h"
#include "base/vgcanvas.h"
#include "blend/image_g2d.h"
#include "base/system_info.h"

static uint32_t lcd_mem_get_line_length(lcd_mem_t* mem) {
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);
  return tk_max(mem->base.w * bpp, mem->line_length);
}

static pixel_t* lcd_mem_init_drawing_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* fbuff = mem->offline_fb;

  if (fb != NULL) {
    memset(fb, 0x00, sizeof(bitmap_t));

    fb->w = lcd->w;
    fb->h = lcd->h;
    fb->buffer = mem->offline_gb;
    fb->format = mem->format;
    graphic_buffer_attach(mem->offline_gb, mem->offline_fb, fb->w, fb->h);
    bitmap_set_line_length(fb, lcd_mem_get_line_length(mem));
  }

  return (pixel_t*)fbuff;
}

static bitmap_t* lcd_mem_init_online_fb(lcd_t* lcd, bitmap_t* fb, lcd_orientation_t o) {
  uint32_t w = 0;
  uint32_t h = 0;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);

  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    w = lcd->w;
    h = lcd->h;
  } else {
    h = lcd->w;
    w = lcd->h;
  }

  memset(fb, 0x00, sizeof(bitmap_t));
  fb->w = w;
  fb->h = h;
  fb->buffer = mem->online_gb;
  fb->format = mem->format;
  graphic_buffer_attach(mem->online_gb, mem->online_fb, w, h);
  bitmap_set_line_length(fb, tk_max(fb->w * bpp, mem->online_line_length));

  return fb;
}

static ret_t lcd_mem_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
#if WITH_LCD_CLEAR_ALPHA
  rect_t r;
  bitmap_t fb;
  
  if (lcd->draw_mode == LCD_DRAW_OFFLINE || dirty_rect == NULL) {
    r = rect_init(0, 0, lcd->w, lcd->h);
  } else {
    r = rect_init(dirty_rect->x, dirty_rect->y, dirty_rect->w, dirty_rect->h);
  }

  lcd_mem_init_drawing_fb(lcd, &fb);
  return image_clear(&fb, &r, color_init(0x0, 0x0, 0x0, 0x0));
#else
  return RET_OK;
#endif
}

static ret_t lcd_mem_fill_rect_with_color(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h, color_t c) {
  bitmap_t fb;
  rect_t r = rect_init(x, y, w, h);
  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  lcd_mem_init_drawing_fb(lcd, &fb);
  return image_fill(&fb, &r, c);
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  return lcd_mem_fill_rect_with_color(lcd, x, y, w, h, lcd->fill_color);
}

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  return lcd_mem_fill_rect_with_color(lcd, x, y, w, 1, lcd->stroke_color);
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  color_t c = lcd->stroke_color;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  uint32_t line_length = lcd_mem_get_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);
  pixel_t* p = (pixel_t*)(fbuff + y * line_length) + x;

  if (a >= TK_OPACITY_ALPHA) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < h; i++) {
      *p = pixel;
      p = (pixel_t*)(((char*)p) + line_length);
    }
  } else if (a >= TK_TRANSPARENT_ALPHA) {
    c.rgba.a = a;
    for (i = 0; i < h; i++) {
      *p = blend_pixel(*p, c);
      p = (pixel_t*)(((char*)p) + line_length);
    }
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  color_t c = lcd->stroke_color;
  pixel_t pixel = color_to_pixel(c);
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  uint32_t line_length = lcd_mem_get_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    pixel_t* p = (pixel_t*)(fbuff + point->y * line_length) + point->x;

    if (a >= TK_OPACITY_ALPHA) {
      *p = pixel;
    } else if (a >= TK_TRANSPARENT_ALPHA) {
      *p = blend_pixel(*p, c);
    }
  }

  return RET_OK;
}

static color_t lcd_mem_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  color_t c;
  uint32_t line_length = lcd_mem_get_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);

  pixel_t p = *((pixel_t*)(fbuff + y * line_length) + x);
  rgba_t rgba = pixel_to_rgba(p);
  c.rgba = rgba;

  return c;
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
  uint8_t color_alpha = color.rgba.a;
  uint8_t global_alpha = lcd->global_alpha;
  uint32_t line_length = lcd_mem_get_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;
  pixel_t pixel = color_to_pixel(color);

  for (j = 0; j < sh; j++) {
    pixel_t* dst_p = (pixel_t*)(fbuff + (y + j) * line_length) + x;
    const uint8_t* s = src_p;
    pixel_t* d = dst_p;

    for (i = 0; i < sw; i++, d++, s++) {
      uint8_t a = global_alpha > TK_OPACITY_ALPHA ? *s : ((*s * global_alpha) >> 8);
      a = (a * color_alpha) >> 8;
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

static ret_t lcd_mem_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  matrix_t* m = &(info->matrix);
  rect_t* s = &(info->src);
  rect_t* d = &(info->dst);
  vgcanvas_t* canvas = lcd_get_vgcanvas(lcd);

  if (canvas != NULL) {
    rect_t r = info->clip;
    vgcanvas_save(canvas);
    vgcanvas_clip_rect(canvas, r.x, r.y, r.w, r.h);
    vgcanvas_set_transform(canvas, m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
    vgcanvas_draw_image(canvas, info->img, s->x, s->y, s->w, s->h, d->x, d->y, d->w, d->h);
    vgcanvas_restore(canvas);

    return RET_OK;
  }

  return RET_NOT_IMPL;
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  bitmap_t fb;
  ret_t ret = RET_OK;
  bool_t is_opaque = (img->flags & BITMAP_FLAG_OPAQUE || img->format == BITMAP_FMT_RGB565);

  lcd_mem_init_drawing_fb(lcd, &fb);

  if (img->format == fb.format && is_opaque && src->w == dst->w && src->h == dst->h &&
      lcd->global_alpha >= TK_OPACITY_ALPHA) {
    rect_t r = *src;
    xy_t dx = dst->x;
    xy_t dy = dst->y;
    ret = image_copy(&fb, img, &r, dx, dy);
  } else {
    ret = image_blend(&fb, img, dst, src, lcd->global_alpha);
  }

  return ret;
}

static vgcanvas_t* lcd_mem_get_vgcanvas(lcd_t* lcd) {
  uint32_t w = lcd->w;
  uint32_t h = lcd->h;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  bitmap_format_t format = mem->format;
  uint32_t line_length = lcd_mem_get_line_length(mem);
  uint32_t* fbdata = (uint32_t*)lcd_mem_init_drawing_fb(lcd, NULL);

  if (mem->vgcanvas == NULL) {
    mem->vgcanvas = vgcanvas_create(w, h, line_length, format, fbdata);
  } else {
    vgcanvas_reinit(mem->vgcanvas, w, h, line_length, format, fbdata);
  }

  return mem->vgcanvas;
}

static ret_t lcd_mem_take_snapshot(lcd_t* lcd, bitmap_t* img, bool_t auto_rotate) {
  bitmap_t fb;
  lcd_mem_init_drawing_fb(lcd, &fb);

  rect_t r = rect_init(0, 0, fb.w, fb.h);
  return_value_if_fail(bitmap_init(img, fb.w, fb.h, (bitmap_format_t)(fb.format), NULL) == RET_OK,
                       RET_OOM);

  img->flags = BITMAP_FLAG_OPAQUE;
  return image_copy(img, &fb, &r, 0, 0);
}

static ret_t lcd_mem_flush(lcd_t* lcd) {
  bitmap_t online_fb;
  bitmap_t offline_fb;
  rect_t* r = &(lcd->dirty_rect);
  system_info_t* info = system_info();
  lcd_orientation_t o = info->lcd_orientation;

  lcd_mem_init_drawing_fb(lcd, &offline_fb);
  lcd_mem_init_online_fb(lcd, &online_fb, o);

  if (o == LCD_ORIENTATION_0) {
    return image_copy(&online_fb, &offline_fb, r, r->x, r->y);
  } else {
    return image_rotate(&online_fb, &offline_fb, r, o);
  }
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) {
  if (lcd->draw_mode != LCD_DRAW_OFFLINE) {
    if (lcd_is_swappable(lcd)) {
      lcd_swap(lcd);
    } else {
      lcd_flush(lcd);
    }

    lcd_sync(lcd);
  }

  return RET_OK;
}

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  return_value_if_fail(lcd->begin_frame != NULL, RET_BAD_PARAMS);

  if (mem->vgcanvas != NULL) {
    vgcanvas_destroy(mem->vgcanvas);
    mem->vgcanvas = NULL;
  }

  if (mem->own_offline_fb) {
    TKMEM_FREE(mem->offline_fb);
  }

  graphic_buffer_destroy(mem->online_gb);
  graphic_buffer_destroy(mem->offline_gb);

  memset(lcd, 0x00, sizeof(lcd_mem_t));
  TKMEM_FREE(lcd);

  return RET_OK;
}

static ret_t lcd_mem_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);

  lcd->w = w;
  lcd->h = h;
  mem->line_length = tk_max(mem->base.w * bpp, line_length);

  return RET_OK;
}

static bitmap_format_t lcd_mem_get_desired_bitmap_format(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  return mem->format;
}

static lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  lcd_t* base = NULL;
  system_info_t* info = system_info();
  lcd_mem_t* lcd = TKMEM_ZALLOC(lcd_mem_t);
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);

  return_value_if_fail(lcd != NULL, NULL);

  if (alloc) {
    lcd->offline_fb = (uint8_t*)TKMEM_ALLOC(w * h * sizeof(pixel_t));
    return_value_if_fail(lcd->offline_fb != NULL, NULL);
  }

  lcd->own_offline_fb = alloc;

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
  base->get_desired_bitmap_format = lcd_mem_get_desired_bitmap_format;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = lcd_mem_destroy;
  base->resize = lcd_mem_resize;
  base->flush = lcd_mem_flush;

  base->w = w;
  base->h = h;
  base->ratio = 1;
  base->global_alpha = 0xff;
  base->type = LCD_FRAMEBUFFER;
  base->support_dirty_rect = TRUE;

  lcd->format = LCD_FORMAT;
  lcd->line_length = w * bpp;
  lcd->online_line_length = lcd->line_length;
  lcd->online_gb = graphic_buffer_create_with_data(NULL, w, h, LCD_FORMAT);
  lcd->offline_gb = graphic_buffer_create_with_data(NULL, w, h, LCD_FORMAT);

  system_info_set_lcd_w(info, base->w);
  system_info_set_lcd_h(info, base->h);
  system_info_set_lcd_type(info, base->type);
  system_info_set_device_pixel_ratio(info, 1);

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
  lcd->support_dirty_rect = FALSE;

  return lcd;
}

static lcd_t* lcd_mem_create_single_fb(wh_t w, wh_t h, uint8_t* fbuff) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->offline_fb = fbuff;
  mem->online_fb = NULL;
  lcd->flush = NULL;

  return lcd;
}

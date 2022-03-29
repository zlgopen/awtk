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
#include "base/lcd_orientation_helper.h"

#ifdef WITH_FAST_LCD_PORTRAIT
static wh_t lcd_mem_get_physical_width(lcd_t* lcd) {
  system_info_t* info = system_info();
  if (info != NULL) {
    if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
      return lcd->h;
    }
  }
  return lcd->w;
}

static wh_t lcd_mem_get_physical_height(lcd_t* lcd) {
  system_info_t* info = system_info();
  if (info != NULL) {
    if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
      return lcd->w;
    }
  }
  return lcd->h;
}
#endif

static uint32_t lcd_get_physical_line_length(lcd_mem_t* mem) {
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);
  return tk_max(lcd_get_physical_width((lcd_t*)mem) * bpp, mem->line_length);
}

static pixel_t* lcd_mem_init_drawing_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* fbuff = mem->offline_fb;

  if (fb != NULL) {
    memset(fb, 0x00, sizeof(bitmap_t));

    fb->format = mem->format;
    fb->buffer = mem->offline_gb;
    fb->w = lcd_get_physical_width(lcd);
    fb->h = lcd_get_physical_height(lcd);
    graphic_buffer_attach(mem->offline_gb, mem->offline_fb, fb->w, fb->h);
    bitmap_set_line_length(fb, lcd_get_physical_line_length(mem));
  }

  return (pixel_t*)fbuff;
}

static bitmap_t* lcd_mem_init_online_fb(lcd_t* lcd, bitmap_t* fb, lcd_orientation_t o) {
  uint32_t w = 0;
  uint32_t h = 0;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);
#ifdef WITH_FAST_LCD_PORTRAIT
  w = lcd_get_physical_width(lcd);
  h = lcd_get_physical_height(lcd);
#else
  if (o == LCD_ORIENTATION_0 || o == LCD_ORIENTATION_180) {
    w = lcd_get_width(lcd);
    h = lcd_get_height(lcd);
  } else {
    h = lcd_get_width(lcd);
    w = lcd_get_height(lcd);
  } 
#endif

  memset(fb, 0x00, sizeof(bitmap_t));
  fb->w = w;
  fb->h = h;
  fb->buffer = mem->online_gb;
  fb->format = mem->format;
  graphic_buffer_attach(mem->online_gb, mem->online_fb, w, h);
  bitmap_set_line_length(fb, tk_max(fb->w * bpp, mem->online_line_length));

  return fb;
}

static ret_t lcd_mem_begin_frame(lcd_t* lcd, const dirty_rects_t* dirty_rects) {
  bitmap_t fb;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  vgcanvas_t* vg = lcd_get_vgcanvas(lcd);

  if (dirty_rects == NULL) {
    rect_t r = rect_init(0, 0, lcd_get_width(lcd), lcd_get_height(lcd));
    lcd_fb_dirty_rects_update_all_fb_dirty_rect(&(mem->fb_dirty_rects_list), &r);
  } else {
    lcd_fb_dirty_rects_update_all_fb_dirty_rects(&(mem->fb_dirty_rects_list), dirty_rects);
  }

  if (vg) {
    vgcanvas_begin_frame(vg, dirty_rects);
  }

#if WITH_LCD_CLEAR_ALPHA
  lcd_mem_init_drawing_fb(lcd, &fb);

  if (lcd->draw_mode == LCD_DRAW_OFFLINE || dirty_rects == NULL) {
    rect_t r = rect_init(0, 0, lcd_get_physical_width(lcd), lcd_get_physical_height(lcd));
    return image_clear(&fb, &r, color_init(0x0, 0x0, 0x0, 0x0));
  } else {
    if (dirty_rects->disable_multiple) {
      return image_clear(&fb, (const rect_t*)&(dirty_rects->max), color_init(0x0, 0x0, 0x0, 0x0));
    } else {
      int32_t i = 0;
      for (i = 0; i < dirty_rects->nr; i++) {
        const rect_t* iter = dirty_rects->rects + i;
        image_clear(&fb, iter, color_init(0x0, 0x0, 0x0, 0x0));
      }
      return RET_OK;
    }
  }
#else
  (void)fb;
  return RET_OK;
#endif
}

static ret_t lcd_mem_fill_rect_with_color(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h, color_t c) {
  bitmap_t fb;

#ifdef WITH_FAST_LCD_PORTRAIT
  rect_t r = rect_init(x, y, w, h);
  system_info_t* info = system_info();
  rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(&r, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));
#else
  rect_t rr = rect_init(x, y, w, h);
#endif

  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  lcd_mem_init_drawing_fb(lcd, &fb);
  return image_fill(&fb, &rr, c);
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  return lcd_mem_fill_rect_with_color(lcd, x, y, w, h, lcd->fill_color);
}

static ret_t lcd_mem_clear_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  bitmap_t fb;
  color_t c = lcd->fill_color;
#ifdef WITH_FAST_LCD_PORTRAIT
  rect_t r = rect_init(x, y, w, h);
  system_info_t* info = system_info();
  rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(&r, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));
#else
  rect_t rr = rect_init(x, y, w, h);
#endif
  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  lcd_mem_init_drawing_fb(lcd, &fb);
  return image_clear(&fb, &rr, c);
}

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  return lcd_mem_fill_rect_with_color(lcd, x, y, w, 1, lcd->stroke_color);
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  pixel_t* p = NULL;
  int32_t offset = 0;
  color_t c = lcd->stroke_color;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  uint32_t line_length = lcd_get_physical_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);

#ifdef WITH_FAST_LCD_PORTRAIT
  system_info_t* info = system_info();
  lcd_orientation_point_rotate_by_anticlockwise(&x, &y, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));

  switch (info->lcd_orientation)
  {
  case LCD_ORIENTATION_90:
    offset = sizeof(pixel_t);
    break;
  case LCD_ORIENTATION_180:
    offset = -line_length;
    break;
  case LCD_ORIENTATION_270:
    offset = -sizeof(pixel_t);
    break;
  default:
    offset = line_length;
    break;
  }
#else
  offset = line_length;
#endif

  p = (pixel_t*)(fbuff + y * line_length) + x;

  if (a >= TK_OPACITY_ALPHA) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < h; i++) {
      *p = pixel;
      p = (pixel_t*)(((char*)p) + offset);
    }
  } else if (a >= TK_TRANSPARENT_ALPHA) {
    c.rgba.a = a;
    for (i = 0; i < h; i++) {
      *p = blend_pixel(*p, c);
      p = (pixel_t*)(((char*)p) + offset);
    }
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  color_t c = lcd->stroke_color;
  pixel_t pixel = color_to_pixel(c);
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  uint32_t line_length = lcd_get_physical_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    xy_t x = point->x;
    xy_t y = point->y;
#ifdef WITH_FAST_LCD_PORTRAIT
    lcd_orientation_point_rotate_by_anticlockwise(&x, &y, system_info()->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));
#endif
    pixel_t* p = (pixel_t*)(fbuff + y * line_length) + x;

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
  uint32_t line_length = lcd_get_physical_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);
#ifdef WITH_FAST_LCD_PORTRAIT
  lcd_orientation_point_rotate_by_anticlockwise(&x, &y, system_info()->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));
#endif
  {
    pixel_t p = *((pixel_t*)(fbuff + y * line_length) + x);
    rgba_t rgba = pixel_to_rgba(p);
    c.rgba = rgba;
  }

  return c;
}

static ret_t lcd_mem_draw_glyph4(lcd_t* lcd, glyph_t* glyph, const rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  uint8_t glyph_a = 0;
  wh_t d_offset = sizeof(pixel_t);
  pixel_t* dst_p = NULL;
  uint32_t pitch = glyph->pitch;
  color_t color = lcd->text_color;
  uint8_t global_alpha = lcd->global_alpha;
  uint8_t color_alpha = (color.rgba.a * global_alpha) >> 8;
  uint32_t line_length = lcd_get_physical_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);
  const uint8_t* src_p = glyph->data + pitch * sy + sx / 2;
  wh_t dst_offset = line_length;
  pixel_t pixel = color_to_pixel(color);


#ifdef WITH_FAST_LCD_PORTRAIT
  system_info_t* info = system_info();
  lcd_orientation_point_rotate_by_anticlockwise(&x, &y, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));

  switch (info->lcd_orientation)
  {
  case LCD_ORIENTATION_90:
    d_offset = -line_length;
    dst_offset = sizeof(pixel_t);
    break;
  case LCD_ORIENTATION_180:
    dst_offset = -line_length;
    d_offset = -sizeof(pixel_t);
    break;
  case LCD_ORIENTATION_270:
    d_offset = line_length;
    dst_offset = -sizeof(pixel_t);
    break;
  default:
    break;
  }
#endif

  dst_p = (pixel_t*)(fbuff + (y + j) * line_length) + x;
  for (j = 0; j < sh; j++) {
    bool_t even = (sx % 2) == 0;
    const uint8_t* s = src_p;
    pixel_t* d = dst_p;

    for (i = 0; i < sw; i++) {
      uint8_t a = 0;
      if (even) {
        glyph_a = ((*s & 0x0f) << 4);
      } else {
        glyph_a = (*s & 0xf0);
        s++;
      }
      even = !even;

      a = (glyph_a * color_alpha) >> 8;
      if (a >= TK_OPACITY_ALPHA) {
        *d = pixel;
      } else if (a >= TK_TRANSPARENT_ALPHA) {
        color.rgba.a = a;
        *d = blend_pixel(*d, color);
      }
      d = (pixel_t*)(((uint8_t*)d) + d_offset);
    }
    src_p += pitch;
    dst_p = (pixel_t*)(((uint8_t*)dst_p) + dst_offset);
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_glyph8(lcd_t* lcd, glyph_t* glyph, const rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t d_offset = sizeof(pixel_t);
  pixel_t* dst_p = NULL;
  uint32_t glyph_w = glyph->w;
  color_t color = lcd->text_color;
  uint8_t global_alpha = lcd->global_alpha;
  uint8_t color_alpha = (color.rgba.a * global_alpha) >> 8;
  uint32_t line_length = lcd_get_physical_line_length((lcd_mem_t*)lcd);
  uint8_t* fbuff = (uint8_t*)lcd_mem_init_drawing_fb(lcd, NULL);
  const uint8_t* src_p = glyph->data + glyph->w * src->y + src->x;
  wh_t dst_offset = line_length;
  pixel_t pixel = color_to_pixel(color);

#ifdef WITH_FAST_LCD_PORTRAIT
  system_info_t* info = system_info();
  lcd_orientation_point_rotate_by_anticlockwise(&x, &y, info->lcd_orientation, lcd_get_width(lcd), lcd_get_height(lcd));

  switch (info->lcd_orientation)
  {
  case LCD_ORIENTATION_90:
    d_offset = -line_length;
    dst_offset = sizeof(pixel_t);
    break;
  case LCD_ORIENTATION_180:
    dst_offset = -line_length;
    d_offset = -sizeof(pixel_t);
    break;
  case LCD_ORIENTATION_270:
    d_offset = line_length;
    dst_offset = -sizeof(pixel_t);
    break;
  default:
    break;
  }
#endif

  dst_p = (pixel_t*)(fbuff + (y + j) * line_length) + x;

  for (j = 0; j < src->h; j++) {
    const uint8_t* s = src_p;
    pixel_t* d = dst_p;

    for (i = 0; i < src->w; i++, s++) {
      uint8_t a = (*s * color_alpha) >> 8;
      if (a >= TK_OPACITY_ALPHA) {
        *d = pixel;
      } else if (a >= TK_TRANSPARENT_ALPHA) {
        color.rgba.a = a;
        *d = blend_pixel(*d, color);
      }
      d = (pixel_t*)(((uint8_t*)d) + d_offset);
    }
    src_p += glyph_w;
    dst_p = (pixel_t*)(((uint8_t*)dst_p) + dst_offset);
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_glyph(lcd_t* lcd, glyph_t* glyph, const rect_t* src, xy_t x, xy_t y) {
  if (glyph->format == GLYPH_FMT_ALPHA) {
    return lcd_mem_draw_glyph8(lcd, glyph, src, x, y);
  } else if (glyph->format == GLYPH_FMT_ALPHA4) {
    return lcd_mem_draw_glyph4(lcd, glyph, src, x, y);
  } else {
    return RET_FAIL;
  }
}

static ret_t lcd_mem_draw_image_matrix(lcd_t* lcd, draw_image_info_t* info) {
  matrix_t* m = &(info->matrix);
  const rect_t* s = &(info->src);
  const rect_t* d = &(info->dst);
  system_info_t* s_info = system_info();
  vgcanvas_t* canvas = lcd_get_vgcanvas(lcd);

  assert(!((info->img->flags & BITMAP_FLAG_LCD_ORIENTATION) && (info->img->orientation != LCD_ORIENTATION_0)) || 
          info->img->orientation == s_info->lcd_orientation);

  if (canvas != NULL) {
    rect_t r = info->clip;
    vgcanvas_save(canvas);
    vgcanvas_clip_rect(canvas, r.x, r.y, r.w, r.h);
    if (s_info->lcd_orientation != LCD_ORIENTATION_0) {
      vgcanvas_transform(canvas, m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
    } else {
      vgcanvas_set_transform(canvas, m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
    }
    vgcanvas_draw_image(canvas, info->img, s->x, s->y, s->w, s->h, d->x, d->y, d->w, d->h);
    vgcanvas_restore(canvas);

    return RET_OK;
  }

  return RET_NOT_IMPL;
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, const rectf_t* src, const rectf_t* dst) {
  bitmap_t fb;
  rectf_t r_dst, r_src;
  ret_t ret = RET_OK;
  system_info_t* info = system_info();
  lcd_orientation_t o = info->lcd_orientation;
  bool_t is_opaque = (img->flags & BITMAP_FLAG_OPAQUE || img->format == BITMAP_FMT_RGB565);
  bool_t is_lcd_orientation = (img->flags & BITMAP_FLAG_LCD_ORIENTATION) && (img->orientation != LCD_ORIENTATION_0);

  lcd_mem_init_drawing_fb(lcd, &fb);

  if (is_lcd_orientation) {
    assert(img->orientation == o);
    r_src = lcd_orientation_rectf_rotate_by_anticlockwise(src, o, img->w, img->h);
    r_dst = lcd_orientation_rectf_rotate_by_anticlockwise(dst, o, lcd_get_width(lcd), lcd_get_height(lcd));
  } else {
    r_src = *src;
    r_dst = *dst;
  }

  if (img->format == fb.format && is_opaque && src->w == dst->w && src->h == dst->h &&
      lcd->global_alpha >= TK_OPACITY_ALPHA) {
    xy_t dx = (xy_t)(r_dst.x);
    xy_t dy = (xy_t)(r_dst.y);
    rect_t r = rect_from_rectf(&r_src);
#ifndef WITH_FAST_LCD_PORTRAIT
    ret = image_copy(&fb, img, &r, dx, dy);
#else
    if (o == LCD_ORIENTATION_0 || is_lcd_orientation) {
      ret = image_copy(&fb, img, &r, dx, dy);
    } else {
      ret = image_rotate_ex(&fb, img, &r, dx, dy, o);
    }
#endif
  } else {
#ifndef WITH_FAST_LCD_PORTRAIT
    ret = image_blend(&fb, img, &r_dst, &r_src, lcd->global_alpha);
#else
    if (o == LCD_ORIENTATION_0 || is_lcd_orientation) {
      ret = image_blend(&fb, img, &r_dst, &r_src, lcd->global_alpha);
    } else {
      ret = image_rotate_blend(&fb, img, &r_dst, &r_src, lcd->global_alpha, o);
    }
#endif
  }
  return ret;
}

static vgcanvas_t* lcd_mem_get_vgcanvas(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
#ifndef WITH_GPU
  bitmap_format_t format = mem->format;
  uint32_t w = lcd_get_physical_width(lcd);
  uint32_t h = lcd_get_physical_height(lcd);
  uint32_t line_length = lcd_get_physical_line_length(mem);
  uint32_t* fbdata = (uint32_t*)lcd_mem_init_drawing_fb(lcd, NULL);

  if (mem->vgcanvas == NULL) {
    mem->vgcanvas = vgcanvas_create(w, h, line_length, format, fbdata);
    vgcanvas_clip_rect(mem->vgcanvas, 0, 0, w, h);
  } else {
    vgcanvas_reinit(mem->vgcanvas, w, h, line_length, format, fbdata);
  }
#endif

  return mem->vgcanvas;
}

static ret_t lcd_mem_get_dirty_rect(lcd_t* lcd, rect_t* r) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* offline_fb = lcd_mem_get_offline_fb(mem);
  const dirty_rects_t* dirty_rects =
      lcd_fb_dirty_rects_get_dirty_rects_by_fb(&(mem->fb_dirty_rects_list), offline_fb);
  if (dirty_rects != NULL) {
    r->x = dirty_rects->max.x;
    r->y = dirty_rects->max.y;
    r->w = dirty_rects->max.w;
    r->h = dirty_rects->max.h;
  } else {
    r->x = 0;
    r->y = 0;
    r->w = lcd_get_physical_width(lcd);
    r->h = lcd_get_physical_height(lcd);
  }
  return RET_OK;
}

static const dirty_rects_t* lcd_mem_get_dirty_rects(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* offline_fb = lcd_mem_get_offline_fb(mem);
  return lcd_fb_dirty_rects_get_dirty_rects_by_fb(&(mem->fb_dirty_rects_list), offline_fb);
}

static ret_t lcd_mem_flush(lcd_t* lcd) {
  bitmap_t online_fb;
  bitmap_t offline_fb;
  const dirty_rects_t* dirty_rects;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  system_info_t* info = system_info();
  uint8_t* fb = lcd_mem_get_offline_fb(mem);
  lcd_orientation_t o = info->lcd_orientation;

  lcd_mem_init_drawing_fb(lcd, &offline_fb);
  lcd_mem_init_online_fb(lcd, &online_fb, o);

  if (mem->wait_vbi != NULL) {
    mem->wait_vbi(mem->wait_vbi_ctx);
  }

  dirty_rects = lcd_fb_dirty_rects_get_dirty_rects_by_fb(&(mem->fb_dirty_rects_list), fb);
  if (dirty_rects != NULL && dirty_rects->nr > 0) {
    if (dirty_rects->disable_multiple) {
      const rect_t* dr = (const rect_t*)&(dirty_rects->max);
#ifdef WITH_FAST_LCD_PORTRAIT
      rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, lcd_get_width(lcd), lcd_get_height(lcd));
      image_copy(&online_fb, &offline_fb, &rr, rr.x, rr.y);
#else
      if (o == LCD_ORIENTATION_0) {
        image_copy(&online_fb, &offline_fb, dr, dr->x, dr->y);
      } else {
        image_rotate(&online_fb, &offline_fb, dr, o);
      }
#endif
    } else {
      uint32_t i = 0;
      for (i = 0; i < dirty_rects->nr; i++) {
        const rect_t* dr = (const rect_t*)dirty_rects->rects + i;
#ifdef WITH_FAST_LCD_PORTRAIT
        rect_t rr = lcd_orientation_rect_rotate_by_anticlockwise(dr, o, lcd_get_width(lcd), lcd_get_height(lcd));
        image_copy(&online_fb, &offline_fb, &rr, rr.x, rr.y);
#else
        if (o == LCD_ORIENTATION_0) {
          image_copy(&online_fb, &offline_fb, dr, dr->x, dr->y);
        } else {
          image_rotate(&online_fb, &offline_fb, dr, o);
        }
#endif
      }
    }
  }
  return RET_OK;
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  uint8_t* offline_fb = lcd_mem_get_offline_fb(mem);
  vgcanvas_t* vg = lcd_get_vgcanvas(lcd);
  if (vg) {
    vgcanvas_end_frame(vg);
  }

  if (lcd->draw_mode != LCD_DRAW_OFFLINE) {
    if (lcd_is_swappable(lcd)) {
      lcd_swap(lcd);
    } else {
      lcd_flush(lcd);
    }

    lcd_sync(lcd);
  }

  lcd_fb_dirty_rects_reset_dirty_rects_by_fb(&(mem->fb_dirty_rects_list), offline_fb);
  return RET_OK;
}

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  return_value_if_fail(lcd->begin_frame != NULL, RET_BAD_PARAMS);

  return_value_if_fail(lcd_mem_deinit(mem) == RET_OK, RET_BAD_PARAMS);

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
  if (mem->vgcanvas != NULL) {
    vgcanvas_clip_rect(mem->vgcanvas, 0, 0, w, h);
  }
  lcd_fb_dirty_rects_reinit(&(mem->fb_dirty_rects_list), w, h);

  return RET_OK;
}

static ret_t lcd_mem_set_orientation(lcd_t* lcd, lcd_orientation_t old_orientation,
                                     lcd_orientation_t new_orientation) {
#ifdef WITH_FAST_LCD_PORTRAIT
  if (tk_is_swap_size_by_orientation(old_orientation, new_orientation)) {
    wh_t w = lcd->w;
    lcd->w = lcd->h;
    lcd->h = w;
  }
#else
  if (tk_is_swap_size_by_orientation(old_orientation, new_orientation)) {
    return lcd_mem_resize(lcd, lcd->h, lcd->w, 0);
  }
#endif
  return RET_OK;
}

static bitmap_format_t lcd_mem_get_desired_bitmap_format(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  return mem->format;
}

static ret_t lcd_mem_set_line_length_impl(lcd_t* lcd, uint32_t line_length) {
  lcd_mem_t* mem = ((lcd_mem_t*)lcd);
  return_value_if_fail(mem != NULL, RET_BAD_PARAMS);
  mem->line_length = line_length;
  mem->online_line_length = line_length;
  return RET_OK;
}

static ret_t lcd_mem_set_vgcanvas(lcd_t* lcd, vgcanvas_t* vgcanvas) {
  lcd_mem_t* mem = ((lcd_mem_t*)lcd);
  return_value_if_fail(mem != NULL, RET_BAD_PARAMS);
  mem->vgcanvas = vgcanvas;
  return RET_OK;
}

static lcd_t* lcd_mem_init(lcd_mem_t* lcd, wh_t w, wh_t h, bool_t alloc) {
  lcd_t* base = NULL;
  system_info_t* info = system_info();
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);

  return_value_if_fail(lcd != NULL, NULL);
  base = &(lcd->base);

  lcd_fb_dirty_rects_init(&(lcd->fb_dirty_rects_list), w, h);

  if (alloc) {
    uint8_t* offline_fb = (uint8_t*)TKMEM_ALLOC(w * h * sizeof(pixel_t));
    ret_t ret = lcd_mem_set_offline_fb(lcd, offline_fb);
    if (ret != RET_OK && offline_fb != NULL) {
      TKMEM_FREE(offline_fb);
    }
    return_value_if_fail(lcd->offline_fb != NULL && ret == RET_OK, NULL);
  }

  lcd->own_offline_fb = alloc;

  base->begin_frame = lcd_mem_begin_frame;
  base->draw_vline = lcd_mem_draw_vline;
  base->draw_hline = lcd_mem_draw_hline;
  base->fill_rect = lcd_mem_fill_rect;
  base->clear_rect = lcd_mem_clear_rect;
  base->draw_image = lcd_mem_draw_image;
  base->draw_image_matrix = lcd_mem_draw_image_matrix;
  base->draw_glyph = lcd_mem_draw_glyph;
  base->draw_points = lcd_mem_draw_points;
  base->get_point_color = lcd_mem_get_point_color;
  base->get_vgcanvas = lcd_mem_get_vgcanvas;
  base->get_desired_bitmap_format = lcd_mem_get_desired_bitmap_format;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = lcd_mem_destroy;
  base->resize = lcd_mem_resize;
  base->flush = lcd_mem_flush;
  base->set_vgcanvas = lcd_mem_set_vgcanvas;
  base->set_line_length = lcd_mem_set_line_length_impl;
  base->get_dirty_rect = lcd_mem_get_dirty_rect;
  base->get_dirty_rects = lcd_mem_get_dirty_rects;
  base->set_orientation = lcd_mem_set_orientation;

#ifdef WITH_FAST_LCD_PORTRAIT
  base->get_physical_width = lcd_mem_get_physical_width;
  base->get_physical_height = lcd_mem_get_physical_height;
#endif

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

static lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  lcd_mem_t* lcd = TKMEM_ZALLOC(lcd_mem_t);
  return_value_if_fail(lcd != NULL, NULL);
  return lcd_mem_init(lcd, w, h, alloc);
}

static lcd_t* lcd_mem_create_double_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  lcd_mem_set_online_fb(mem, online_fb);
  lcd_mem_set_offline_fb(mem, offline_fb);

  return lcd;
}

static lcd_t* lcd_mem_create_three_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb,
                                      uint8_t* next_fb) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  lcd_mem_set_next_fb(mem, next_fb);
  lcd_mem_set_online_fb(mem, online_fb);
  lcd_mem_set_offline_fb(mem, offline_fb);

  return lcd;
}

static lcd_t* lcd_mem_create_single_fb(wh_t w, wh_t h, uint8_t* fbuff) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  lcd_mem_set_offline_fb(mem, fbuff);
  mem->online_fb = NULL;
  lcd->flush = NULL;

  return lcd;
}

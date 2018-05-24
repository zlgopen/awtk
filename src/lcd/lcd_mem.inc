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
#include "blend/soft_g2d.h"
#include "base/system_info.h"

#ifdef WITH_G2D
#include "base/g2d.h"
#endif /*WITH_G2D*/

static ret_t lcd_mem_to_bitmap(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  fb->format = LCD_FORMAT;
  fb->data = mem->fbuff;
  fb->w = lcd->w;
  fb->h = lcd->h;

  if (lcd->draw_mode == LCD_DRAW_ANIMATION && mem->online_fbuff) {
    fb->data = mem->online_fbuff;
  }

  return RET_OK;
}

static ret_t lcd_mem_begin_frame(lcd_t* lcd, rect_t* dirty_rect) { return RET_OK; }

static ret_t lcd_mem_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  wh_t i = 0;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  color_t c = lcd->stroke_color;
  pixel_t* fbuff = (pixel_t*)mem->fbuff;
  pixel_t* p = fbuff + y * lcd->w + x;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;

  if (a == 0xff) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < w; i++) p[i] = pixel;
  } else {
    for (i = 0; i < w; i++) p[i] = blend_pixel(p[i], c);
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  wh_t i = 0;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  color_t c = lcd->stroke_color;
  pixel_t* fbuff = (pixel_t*)mem->fbuff;
  pixel_t* p = fbuff + y * lcd->w + x;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;

  if (a == 0xff) {
    pixel_t pixel = color_to_pixel(c);
    for (i = 0; i < h; i++) {
      *p = pixel;
      p += lcd->w;
    }
  } else {
    for (i = 0; i < h; i++) {
      *p = blend_pixel(*p, c);
      p += lcd->w;
    }
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  wh_t i = 0;
  wh_t width = lcd->w;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* fbuff = (pixel_t*)mem->fbuff;
  color_t c = lcd->stroke_color;
  pixel_t pixel = color_to_pixel(c);

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    pixel_t* p = fbuff + point->y * width + point->x;
    if (c.rgba.a == 0xff) {
      *p = pixel;
    } else if (c.rgba.a) {
      *p = blend_pixel(*p, c);
    }
  }

  return RET_OK;
}

color_t lcd_mem_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* fbuff = (pixel_t*)mem->fbuff;
  pixel_t p = fbuff[y * lcd->w + x];

  return to_color(p);
}

static ret_t lcd_mem_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t r;
  bitmap_t fb;

  rect_init(r, x, y, w, h);
  lcd_mem_to_bitmap(lcd, &fb);

#ifdef WITH_G2D
  if(g2d_fill_rect(&fb, &r, lcd->fill_color) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_fill_rect(&fb, &r, lcd->fill_color);
}

static ret_t lcd_mem_draw_glyph(lcd_t* lcd, glyph_t* glyph, rect_t* src, xy_t x, xy_t y) {
  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t width = lcd->w;
  color_t color = lcd->text_color;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  pixel_t* fbuff = (pixel_t*)mem->fbuff;
  pixel_t* dst_p = fbuff + y * width + x;
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;

  for (j = 0; j < sh; j++) {
    for (i = 0; i < sw; i++) {
      uint8_t alpha = src_p[i];
      if (alpha == 0xff) {
        dst_p[i] = color_to_pixel(color);
      } else if (alpha) {
        color.rgba.a = alpha;
        dst_p[i] = blend_pixel(dst_p[i], color);
      }
    }
    src_p += glyph->w;
    dst_p += width;
  }

  return RET_OK;
}

static ret_t lcd_mem_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  bitmap_t fb;
  lcd_mem_to_bitmap(lcd, &fb);

  if (img->format == fb.format && img->format == BITMAP_FMT_RGB565) {
#ifdef WITH_G2D
    if (g2d_copy_image(&fb, img, src, dst->x, dst->y) == RET_OK) {
      return RET_OK;
    }
#endif /*WITH_G2D*/
    return soft_copy_image(&fb, img, src, dst->x, dst->y);
  } else {
#ifdef WITH_G2D
    if (g2d_blend_image(&fb, img, dst, src, lcd->global_alpha) == RET_OK) {
      return RET_OK;
    }
#endif /*WITH_G2D*/
    return soft_blend_image(&fb, img, dst, src, lcd->global_alpha);
  }
}

static vgcanvas_t* lcd_mem_get_vgcanvas(lcd_t* lcd) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  if (mem->vgcanvas == NULL) {
    mem->vgcanvas = vgcanvas_create(lcd->w, lcd->h, mem->format, (uint32_t*)(mem->fbuff));
  }

  return mem->vgcanvas;
}

static ret_t snapshot_destroy(bitmap_t* img) {
  TKMEM_FREE((void*)(img->data));

  return RET_OK;
}

static ret_t lcd_mem_take_snapshot(lcd_t* lcd, bitmap_t* img) {
  uint32_t size = 0;
  void* data = NULL;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;
  memset(img, 0x00, sizeof(bitmap_t));

  img->w = lcd->w;
  img->h = lcd->h;
  img->format = mem->format;

  if (img->format == BITMAP_FMT_RGBA) {
    uint32_t i = 0;
    uint32_t* s = NULL;
    uint32_t* d = NULL;
    uint32_t nr = img->w * img->h;
    size = img->w * img->h * 4;
    data = TKMEM_ALLOC(size);

    return_value_if_fail(data != NULL, RET_FAIL);

    s = (uint32_t*)(mem->fbuff);
    d = (uint32_t*)data;

    for (i = 0; i < nr; i++, s++, d++) {
      uint8_t* ps = (uint8_t*)s;
      uint8_t* pd = (uint8_t*)d;
      pd[0] = ps[3];
      pd[1] = ps[2];
      pd[2] = ps[1];
      pd[3] = ps[0];
    }
  } else if (img->format == BITMAP_FMT_RGB565) {
    size = img->w * img->h * 2;
    data = TKMEM_ALLOC(size);
    return_value_if_fail(data != NULL, RET_FAIL);
    memcpy(data, mem->fbuff, size);
  } else {
    return_value_if_fail(size > 0, RET_FAIL);
  }

  img->destroy = snapshot_destroy;
  img->data = (uint8_t*)data;

  return RET_OK;
}

static bitmap_t* lcd_mem_init_online_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  fb->flags = 0;
  fb->w = lcd->w;
  fb->h = lcd->w;
  fb->format = mem->format;
  fb->data = mem->online_fbuff;

  return fb;
}

static bitmap_t* lcd_mem_init_offline_fb(lcd_t* lcd, bitmap_t* fb) {
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  fb->flags = 0;
  fb->w = lcd->w;
  fb->h = lcd->w;
  fb->format = mem->format;
  fb->data = mem->fbuff;

  return fb;
}

static ret_t lcd_mem_end_frame(lcd_t* lcd) {
  rect_t rect;
  bitmap_t online_fb;
  bitmap_t offline_fb;
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  rect_t* r = &(lcd->dirty_rect);

  if (lcd->draw_mode == LCD_DRAW_OFFLINE) {
    return RET_OK;
  }

  if (mem->online_fbuff == NULL) {
    return RET_OK;
  }

  if (lcd->draw_mode == LCD_DRAW_ANIMATION) {
    return RET_OK;
  }

  if (r == NULL) {
    r = &rect;
    r->x = 0;
    r->y = 0;
    r->w = lcd->w;
    r->h = lcd->h;
  }

  lcd_mem_init_online_fb(lcd, &online_fb);
  lcd_mem_init_offline_fb(lcd, &offline_fb);

#ifdef WITH_G2D
  if(g2d_copy_image(&online_fb, &offline_fb, r, r->x, r->y) == RET_OK) {
    return RET_OK;
  }
#endif /*WITH_G2D*/

  return soft_copy_image(&online_fb, &offline_fb, r, r->x, r->y);
}

static ret_t lcd_mem_destroy(lcd_t* lcd) {
  TKMEM_FREE(lcd);

  return RET_OK;
}

lcd_t* lcd_mem_create(wh_t w, wh_t h, bool_t alloc) {
  system_info_t* info = system_info();

  lcd_mem_t* lcd = TKMEM_ZALLOC(lcd_mem_t);
  return_value_if_fail(lcd != NULL, NULL);

  lcd->format = LCD_FORMAT;
  lcd_t* base = &(lcd->base);

  if (alloc) {
    lcd->fbuff = (uint8_t*)TKMEM_ALLOC(w * h * sizeof(pixel_t));
    return_value_if_fail(lcd->fbuff != NULL, NULL);
  }

  base->begin_frame = lcd_mem_begin_frame;
  base->draw_vline = lcd_mem_draw_vline;
  base->draw_hline = lcd_mem_draw_hline;
  base->fill_rect = lcd_mem_fill_rect;
  base->draw_image = lcd_mem_draw_image;
  base->draw_glyph = lcd_mem_draw_glyph;
  base->draw_points = lcd_mem_draw_points;
  base->get_point_color = lcd_mem_get_point_color;
  base->get_vgcanvas = lcd_mem_get_vgcanvas;
  base->take_snapshot = lcd_mem_take_snapshot;
  base->end_frame = lcd_mem_end_frame;
  base->destroy = lcd_mem_destroy;

  base->w = w;
  base->h = h;
  base->ratio = 1;
  base->type = LCD_FRAMEBUFFER;
  base->global_alpha = 0xff;

  info->lcd_w = base->w;
  info->lcd_h = base->h;
  info->lcd_type = base->type;
  info->device_pixel_ratio = 1;

  return base;
}

lcd_t* lcd_mem_create_double_fb(wh_t w, wh_t h, uint8_t* online_fb, uint8_t* offline_fb) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->fbuff = offline_fb;
  mem->online_fbuff = online_fb;

  return lcd;
}

lcd_t* lcd_mem_create_single_fb(wh_t w, wh_t h, uint8_t* fbuff) {
  lcd_t* lcd = lcd_mem_create(w, h, FALSE);
  lcd_mem_t* mem = (lcd_mem_t*)lcd;

  mem->fbuff = fbuff;

  return lcd;
}

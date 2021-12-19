/**
 * File:   lcd_mem_fragment.h
 * Author: AWTK Develop Team
 * Brief:  mem fragment based implemented lcd interface
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-11-14 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/darray.h"
#include "base/vgcanvas.h"
#include "blend/image_g2d.h"
#include "base/system_info.h"

#include "base/lcd.h"
#include "base/bitmap.h"

typedef struct _lcd_mem_fragment_t {
  lcd_t base;

  xy_t x;
  xy_t y;
  wh_t w;
  wh_t h;

  rect_t dirty_rect;

  bitmap_t fb;
  graphic_buffer_t* gb;
  pixel_t buff[FRAGMENT_FRAME_BUFFER_SIZE];
} lcd_mem_fragment_t;

static ret_t lcd_mem_fragment_begin_frame(lcd_t* lcd, const dirty_rects_t* dirty_rects) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  uint32_t bpp = bitmap_get_bpp_of_format(LCD_FORMAT);

  mem->x = dirty_rects->max.x;
  mem->y = dirty_rects->max.y;

  mem->fb.buffer = mem->gb;
  mem->fb.w = dirty_rects->max.w;
  mem->fb.h = dirty_rects->max.h;
  mem->fb.format = LCD_FORMAT;
  mem->fb.flags = BITMAP_FLAG_OPAQUE;
  mem->fb.line_length = dirty_rects->max.w * bpp;
  graphic_buffer_attach(mem->gb, mem->buff, mem->fb.w, mem->fb.h);

  return RET_OK;
}

static ret_t lcd_mem_fragment_fill_rect_with_color(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h,
                                                   color_t c) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  bitmap_t* fb = &(mem->fb);
  uint32_t dx = x - mem->x;
  uint32_t dy = y - mem->y;
  rect_t r = rect_init(dx, dy, w, h);

  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  return image_fill(fb, &r, c);
}

static ret_t lcd_mem_fragment_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  assert(x >= mem->x && y >= mem->y);
  assert(w <= mem->fb.w && h <= mem->fb.h);

  return lcd_mem_fragment_fill_rect_with_color(lcd, x, y, w, h, lcd->fill_color);
}

static ret_t lcd_mem_fragment_clear_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  bitmap_t* fb = &(mem->fb);
  uint32_t dx = x - mem->x;
  uint32_t dy = y - mem->y;
  color_t c = lcd->fill_color;
  rect_t r = rect_init(dx, dy, w, h);

  assert(x >= mem->x && y >= mem->y);
  assert(w <= mem->fb.w && h <= mem->fb.h);

  c.rgba.a = (c.rgba.a * lcd->global_alpha) / 0xff;

  return image_clear(fb, &r, c);
}

static ret_t lcd_mem_fragment_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  assert(x >= mem->x && y >= mem->y);
  assert(w <= mem->fb.w);

  return lcd_mem_fragment_fill_rect_with_color(lcd, x, y, w, 1, lcd->stroke_color);
}

static ret_t lcd_mem_fragment_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  wh_t i = 0;
  uint32_t dx = x - mem->x;
  uint32_t dy = y - mem->y;
  color_t c = lcd->stroke_color;
  uint8_t* fbuff = (uint8_t*)(mem->buff);
  uint32_t line_length = mem->fb.line_length;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;
  pixel_t* p = (pixel_t*)(fbuff + dy * line_length) + dx;

  assert(x >= mem->x && y >= mem->y);
  assert(h <= mem->fb.h);

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

static ret_t lcd_mem_fragment_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  wh_t i = 0;
  color_t c = lcd->stroke_color;
  pixel_t pixel = color_to_pixel(c);
  uint8_t* fbuff = (uint8_t*)(mem->buff);
  uint32_t line_length = mem->fb.line_length;
  uint8_t a = (c.rgba.a * lcd->global_alpha) / 0xff;

  for (i = 0; i < nr; i++) {
    point_t* point = points + i;
    uint32_t x = point->x - mem->x;
    uint32_t y = point->y - mem->y;
    pixel_t* p = (pixel_t*)(fbuff + y * line_length) + x;

    if (a >= TK_OPACITY_ALPHA) {
      *p = pixel;
    } else if (a >= TK_TRANSPARENT_ALPHA) {
      *p = blend_pixel(*p, c);
    }
  }

  return RET_OK;
}

static color_t lcd_mem_fragment_get_point_color(lcd_t* lcd, xy_t x, xy_t y) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  uint8_t* fbuff = (uint8_t*)(mem->buff);
  uint32_t line_length = mem->fb.line_length;
  uint32_t dx = x - mem->x;
  uint32_t dy = y - mem->y;
  pixel_t p = *((pixel_t*)(fbuff + dy * line_length) + dx);
  color_t c = pixel_to_rgba(p);

  return c;
}

static ret_t lcd_mem_fragment_draw_glyph(lcd_t* lcd, glyph_t* glyph, const rect_t* src, xy_t x, xy_t y) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  wh_t i = 0;
  wh_t j = 0;
  wh_t sx = src->x;
  wh_t sy = src->y;
  wh_t sw = src->w;
  wh_t sh = src->h;
  wh_t w = lcd->w;
  uint8_t a = 0;
  color_t color = lcd->text_color;
  uint8_t global_alpha = lcd->global_alpha;
  uint8_t color_alpha = (color.rgba.a * global_alpha) >> 8;
  uint32_t line_length = mem->fb.line_length;
  uint8_t* fbuff = (uint8_t*)(mem->buff);
  const uint8_t* src_p = glyph->data + glyph->w * sy + sx;
  pixel_t pixel = color_to_pixel(color);
  int32_t dx = x - mem->x;
  int32_t dy = y - mem->y;

  assert(x >= mem->x && y >= mem->y);

  for (j = 0; j < sh; j++) {
    pixel_t* dst_p = (pixel_t*)(fbuff + (dy + j) * line_length) + dx;
    const uint8_t* s = src_p;
    pixel_t* d = dst_p;

    for (i = 0; i < sw; i++, d++, s++) {
      a = (*s * color_alpha) >> 8;
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

static ret_t lcd_mem_fragment_draw_image(lcd_t* lcd, bitmap_t* img, const rectf_t* src, const rectf_t* dst) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  ret_t ret = RET_OK;
  bitmap_t* fb = &(mem->fb);
  int32_t x = dst->x - mem->x;
  int32_t y = dst->y - mem->y;
  rectf_t d = rectf_init(x, y, dst->w, dst->h);
  bool_t is_opaque = (img->flags & BITMAP_FLAG_OPAQUE || img->format == BITMAP_FMT_RGB565) &&
                     lcd->global_alpha >= TK_OPACITY_ALPHA;

  assert(dst->x >= mem->x && dst->y >= mem->y);
  assert(dst->w <= mem->fb.w && dst->h <= mem->fb.h);

  if (img->format == fb->format && is_opaque && src->w == dst->w && src->h == dst->h) {
    rect_t s = rect_from_rectf(src);
    ret = image_copy(fb, img, &s, x, y);
  } else {
    ret = image_blend(fb, img, &d, src, lcd->global_alpha);
  }

  return ret;
}

static ret_t lcd_mem_fragment_flush(lcd_t* lcd) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  int32_t x = mem->x;
  int32_t y = mem->y;
  uint32_t w = mem->fb.w;
  uint32_t h = mem->fb.h;
  pixel_t* p = mem->buff;
  
#ifdef lcd_draw_bitmap_impl
  lcd_draw_bitmap_impl(x, y, w, h, p);
#else
  uint32_t nr = w * h;
  set_window_func(x, y, x + w - 1, y + h - 1);
  while (nr-- > 0) {
    write_data_func(*p++);
  }
#endif
	
  return RET_OK;
}

static ret_t lcd_mem_fragment_end_frame(lcd_t* lcd) {
  return lcd_flush(lcd);
}

static ret_t lcd_mem_fragment_destroy(lcd_t* lcd) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;

  graphic_buffer_destroy(mem->gb);
  TKMEM_FREE(lcd);

  return RET_OK;
}

static ret_t lcd_mem_fragment_resize(lcd_t* lcd, wh_t w, wh_t h, uint32_t line_length) {
  return RET_OK;
}

static ret_t lcd_mem_fragment_set_orientation(lcd_t* lcd, lcd_orientation_t old_orientation, lcd_orientation_t new_orientation) {
  if (tk_is_swap_size_by_orientation(old_orientation, new_orientation)) {
    return lcd_mem_fragment_resize(lcd, lcd->h, lcd->w, 0);
  }
  return RET_OK;
}


static bitmap_format_t lcd_mem_fragment_get_desired_bitmap_format(lcd_t* lcd) {
  return LCD_FORMAT;
}

static lcd_mem_fragment_t s_lcd_mem_fragment;

uint8_t* lcd_mem_fragment_get_buff(lcd_t* lcd) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  return_value_if_fail(lcd != NULL && mem != NULL, NULL);

  return (uint8_t*)mem->buff;
}

static ret_t lcd_mem_fragment_set_clip_rect(lcd_t* lcd, const rect_t* r) {
  rect_t dirty_r; 
  rect_t r_fix = rect_init(0, 0, 0, 0);
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  return_value_if_fail(mem != NULL && r != NULL, RET_BAD_PARAMS);
  lcd_get_dirty_rect(lcd, &dirty_r);
  mem->dirty_rect = rect_intersect(r, &dirty_r);

  return RET_OK;
}

static bool_t lcd_mem_fragment_is_rect_in_clip_rect(lcd_t* lcd, xy_t left, xy_t top, xy_t right, xy_t bottom) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  xy_t clip_left, clip_top, clip_right, clip_bottom;
  return_value_if_fail(mem != NULL, FALSE);
  clip_left = mem->dirty_rect.x;
  clip_top = mem->dirty_rect.y;
  clip_right = mem->dirty_rect.x + mem->dirty_rect.w - 1;
  clip_bottom = mem->dirty_rect.y + mem->dirty_rect.h - 1;

  if (left > clip_right || right < clip_left || top > clip_bottom || bottom < clip_top) {
    return FALSE;
  }
  return TRUE;
}

static ret_t lcd_mem_fragment_get_clip_rect(lcd_t* lcd, rect_t* rect) {
  lcd_mem_fragment_t* mem = (lcd_mem_fragment_t*)lcd;
  return_value_if_fail(mem != NULL, RET_BAD_PARAMS);
  rect->x = mem->dirty_rect.x;
  rect->y = mem->dirty_rect.y;
  rect->w = mem->dirty_rect.w;
  rect->h = mem->dirty_rect.h;

  return RET_OK;
}

lcd_t* lcd_mem_fragment_create(wh_t w, wh_t h) {
  lcd_mem_fragment_t* mem = &s_lcd_mem_fragment;
  lcd_t* base = (lcd_t*)mem;
  system_info_t* info = system_info();

  base->begin_frame = lcd_mem_fragment_begin_frame;
  base->set_clip_rect = lcd_mem_fragment_set_clip_rect;
  base->get_clip_rect = lcd_mem_fragment_get_clip_rect;
  base->is_rect_in_clip_rect = lcd_mem_fragment_is_rect_in_clip_rect;
  base->draw_vline = lcd_mem_fragment_draw_vline;
  base->draw_hline = lcd_mem_fragment_draw_hline;
  base->fill_rect = lcd_mem_fragment_fill_rect;
  base->clear_rect = lcd_mem_fragment_clear_rect;
  base->draw_image = lcd_mem_fragment_draw_image;
  base->draw_glyph = lcd_mem_fragment_draw_glyph;
  base->draw_points = lcd_mem_fragment_draw_points;
  base->get_point_color = lcd_mem_fragment_get_point_color;
  base->get_desired_bitmap_format = lcd_mem_fragment_get_desired_bitmap_format;
  base->end_frame = lcd_mem_fragment_end_frame;
  base->destroy = lcd_mem_fragment_destroy;
  base->resize = lcd_mem_fragment_resize;
  base->flush = lcd_mem_fragment_flush;
  base->set_orientation = lcd_mem_fragment_set_orientation;

  base->w = w;
  base->h = h;
  base->ratio = 1;
  base->global_alpha = 0xff;
  base->type = LCD_FRAMEBUFFER;
  base->support_dirty_rect = TRUE;

  system_info_set_lcd_w(info, base->w);
  system_info_set_lcd_h(info, base->h);
  system_info_set_lcd_type(info, base->type);
  system_info_set_device_pixel_ratio(info, 1);

  memset(&(mem->fb), 0x00, sizeof(bitmap_t));
  mem->gb = graphic_buffer_create_with_data((uint8_t*)(mem->buff), w, h, BITMAP_FMT_NONE);

  return base;
}

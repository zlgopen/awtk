/**
 * File:   canvas.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  canvas provides basic drawings functions.
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "base/canvas.h"

ret_t canvas_translate(canvas_t* c, xy_t dx, xy_t dy) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  c->ox += dx;
  c->oy += dy;

  return RET_OK;
}

ret_t canvas_untranslate(canvas_t* c, xy_t dx, xy_t dy) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  c->ox -= dx;
  c->oy -= dy;

  return RET_OK;
}

canvas_t* canvas_init(canvas_t* c, lcd_t* lcd, font_manager_t* font_manager) {
  return_value_if_fail(c != NULL && lcd != NULL && font_manager != NULL, NULL);

  memset(c, 0x00, sizeof(canvas_t));
  c->lcd = lcd;
  c->font_manager = font_manager;

  return c;
}

wh_t canvas_get_width(canvas_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->lcd->width;
}

wh_t canvas_get_height(canvas_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->lcd->height;
}

ret_t canvas_set_clip_rect(canvas_t* c, rect_t* r) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  if (r) {
    c->clip_left = r->x;
    c->clip_top = r->y;
    c->clip_right = r->x + r->w;
    c->clip_bottom = r->y + r->h;
  } else {
    c->clip_left = 0;
    c->clip_top = 0;
    c->clip_right = c->lcd->width;
    c->clip_bottom = c->lcd->height;
  }

  return RET_OK;
}

ret_t canvas_set_fill_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->lcd->fill_color = color;

  return RET_OK;
}

ret_t canvas_set_text_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->lcd->text_color = color;

  return RET_OK;
}

ret_t canvas_set_stroke_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->lcd->stroke_color = color;

  return RET_OK;
}

ret_t canvas_set_global_alpha(canvas_t* c, uint8_t alpha) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->lcd->global_alpha = alpha;

  return RET_OK;
}

ret_t canvas_set_font(canvas_t* c, const char* name, uint16_t size) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->font_name = name;
  c->font_size = size;
  c->font = font_manager_find(c->font_manager, name, size);

  return RET_OK;
}

wh_t canvas_measure_text(canvas_t* c, wchar_t* str, int32_t nr) {
  glyph_t g;
  wh_t w = 0;
  int32_t i = 0;
  return_value_if_fail(c != NULL && str != NULL && c->font != NULL, 0);

  if (nr < 0) {
    nr = wcslen(str);
  }

  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];
    if (font_find_glyph(c->font, chr, &g, c->font_size) == RET_OK) {
      w += g.w;
    }
  }

  return w;
}

ret_t canvas_begin_frame(canvas_t* c, rect_t* dirty_rect) {
  return_value_if_fail(c != NULL && dirty_rect != NULL, RET_BAD_PARAMS);

  c->ox = 0;
  c->oy = 0;
  canvas_set_clip_rect(c, dirty_rect);

  return lcd_begin_frame(c->lcd, dirty_rect);
}

static ret_t canvas_draw_hline_impl(canvas_t* c, xy_t x, xy_t y, wh_t w) {
  xy_t x2 = x + w;

  if (y < c->clip_top || y > c->clip_bottom || x2 < c->clip_left || x > c->clip_right) {
    return RET_OK;
  }

  x = ftk_max(x, c->clip_left);
  x2 = ftk_min(x2, c->clip_right);
  w = x2 - x;

  return lcd_draw_hline(c->lcd, x, y, w);
}

ret_t canvas_draw_hline(canvas_t* c, xy_t x, xy_t y, wh_t w) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_hline_impl(c, c->ox + x, c->oy + y, w);
}

static ret_t canvas_draw_vline_impl(canvas_t* c, xy_t x, xy_t y, wh_t h) {
  xy_t y2 = y + h;

  if (x < c->clip_left || x > c->clip_right || y2 < c->clip_top || y > c->clip_bottom) {
    return RET_OK;
  }

  y = ftk_max(y, c->clip_top);
  y2 = ftk_min(y2, c->clip_bottom);
  h = y2 - y;

  return lcd_draw_vline(c->lcd, x, y, h);
}

ret_t canvas_draw_vline(canvas_t* c, xy_t x, xy_t y, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_vline_impl(c, c->ox + x, c->oy + y, h);
}

#define MAX_POINTS_PER_DRAW 20

static ret_t canvas_do_draw_points(canvas_t* c, point_t* points, uint32_t nr) {
  uint32_t i = 0;
  uint32_t real_nr = 0;
  xy_t left = c->clip_left;
  xy_t top = c->clip_top;
  xy_t right = c->clip_right;
  xy_t bottom = c->clip_bottom;

  point_t real_points[MAX_POINTS_PER_DRAW];
  return_value_if_fail(nr <= MAX_POINTS_PER_DRAW, RET_BAD_PARAMS);

  for (i = 0; i < nr; i++) {
    point_t* p = points + i;
    if (p->x < left || p->x > right || p->y < top || p->y > bottom) {
      continue;
    }

    real_points[real_nr] = *p;
    real_nr++;
  }

  return lcd_draw_points(c->lcd, real_points, real_nr);
}

static ret_t canvas_draw_points_impl(canvas_t* c, point_t* points, uint32_t nr) {
  int32_t i = 0;
  point_t* p = points;
  int32_t n = (nr / MAX_POINTS_PER_DRAW);
  int32_t r = (nr % MAX_POINTS_PER_DRAW);

  for (i = 0; i <= n; i++) {
    if (i == n) {
      canvas_do_draw_points(c, p, r);
    } else {
      canvas_do_draw_points(c, p, MAX_POINTS_PER_DRAW);
      p += MAX_POINTS_PER_DRAW;
    }
  }

  return RET_OK;
}

ret_t canvas_draw_points(canvas_t* c, point_t* points, uint32_t nr) {
  uint32_t i = 0;
  xy_t ox = 0;
  xy_t oy = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(c != NULL && points != NULL, RET_BAD_PARAMS);

  ox = c->ox;
  oy = c->oy;
  if (ox && oy) {
    for (i = 0; i < nr; i++) {
      point_t* p = points + i;
      p->x += ox;
      p->y += oy;
    }
  }
  ret = canvas_draw_points_impl(c, points, nr);
  if (ox && oy) {
    for (i = 0; i < nr; i++) {
      point_t* p = points + i;
      p->x -= ox;
      p->y -= oy;
    }
  }

  return ret;
}

static ret_t canvas_fill_rect_impl(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  xy_t x2 = x + w;
  xy_t y2 = y + h;

  if (x > c->clip_right || x2 < c->clip_left || y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }

  x = ftk_max(x, c->clip_left);
  y = ftk_max(y, c->clip_top);
  x2 = ftk_min(x2, c->clip_right);
  y2 = ftk_min(y2, c->clip_bottom);
  w = x2 - x;
  h = y2 - y;

  return lcd_fill_rect(c->lcd, x, y, w, h);
}

ret_t canvas_fill_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_fill_rect_impl(c, c->ox + x, c->oy + y, w, h);
}

static ret_t canvas_stroke_rect_impl(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(c != NULL && w > 0 && h > 0, RET_BAD_PARAMS);

  canvas_draw_hline_impl(c, x, y, w);
  canvas_draw_hline_impl(c, x, y + h - 1, w);
  canvas_draw_vline_impl(c, x, y, h);
  canvas_draw_vline_impl(c, x + w - 1, y, h);

  return RET_OK;
}

ret_t canvas_stroke_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_stroke_rect_impl(c, c->ox + x, c->oy + y, w, h);
}

static ret_t canvas_draw_glyph(canvas_t* c, glyph_t* g, xy_t x, xy_t y) {
  rect_t src;
  rect_t dst;
  xy_t x2 = x + g->w;
  xy_t y2 = y + g->h;

  if (x > c->clip_right || x2 < c->clip_left || y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }

  dst.x = ftk_max(x, c->clip_left);
  dst.y = ftk_max(y, c->clip_top);
  dst.w = ftk_min(x2, c->clip_right) - dst.x;
  dst.h = ftk_min(y2, c->clip_bottom) - dst.y;

  src.x = dst.x - x;
  src.y = dst.y - y;
  src.w = dst.w;
  src.h = dst.h;

  return lcd_draw_glyph(c->lcd, g, &src, dst.x, dst.y);
}

static ret_t canvas_draw_char_impl(canvas_t* c, wchar_t chr, xy_t x, xy_t y) {
  glyph_t g;
  uint16_t font_size = c->font_size;
  return_value_if_fail(font_find_glyph(c->font, chr, &g, font_size) == RET_OK, RET_BAD_PARAMS);

  x += g.x;
  y += font_size + g.y;

  return canvas_draw_glyph(c, &g, x, y);
}

ret_t canvas_draw_char(canvas_t* c, wchar_t chr, xy_t x, xy_t y) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_char_impl(c, chr, c->ox + x, c->oy + y);
}

static ret_t canvas_draw_text_impl(canvas_t* c, wchar_t* str, int32_t nr, xy_t x, xy_t y) {
  glyph_t g;
  int32_t i = 0;
  xy_t left = x;
  uint16_t font_size = c->font_size;

  if (nr < 0) {
    nr = wcslen(str);
  }

  y -= font_size * 2 / 3;
  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];
    if (chr == ' ') {
      x += 4 + 1;
    } else if (chr == '\r') {
      if (str[i + 1] != '\n') {
        y += font_size;
        x = left;
      }
    } else if (chr == '\r') {
      y += font_size;
      x = left;
    } else if (font_find_glyph(c->font, chr, &g, c->font_size) == RET_OK) {
      xy_t xx = x + g.x;
      xy_t yy = y + font_size + g.y;

      canvas_draw_glyph(c, &g, xx, yy);
      x += g.w + 1;
    } else {
      x += 4 + 1;
    }
  }

  return RET_OK;
}

ret_t canvas_draw_text(canvas_t* c, wchar_t* str, int32_t nr, xy_t x, xy_t y) {
  return_value_if_fail(c != NULL && str != NULL, RET_BAD_PARAMS);

  return canvas_draw_text_impl(c, str, nr, c->ox + x, c->oy + y);
}

static ret_t canvas_do_draw_image(canvas_t* c, bitmap_t* img, rect_t* s, rect_t* d) {
  rect_t src;
  rect_t dst;

  xy_t x = d->x;
  xy_t y = d->y;
  xy_t x2 = d->x + d->w;
  xy_t y2 = d->y + d->h;

  if (x > c->clip_right || x2 < c->clip_left || y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }

  dst.x = ftk_max(x, c->clip_left);
  dst.y = ftk_max(y, c->clip_top);
  dst.w = ftk_min(x2, c->clip_right) - dst.x;
  dst.h = ftk_min(y2, c->clip_bottom) - dst.y;

  src.x = s->x + (dst.x - x);
  src.y = s->y + (dst.y - y);

  if (src.x >= img->w || src.y >= img->h) {
    return RET_OK;
  }

  src.w = ftk_min((img->w - src.x), dst.w);
  src.h = ftk_min((img->h - src.y), dst.h);
  if (src.w == 0 || src.h == 0) {
    return RET_OK;
  }

  return lcd_draw_image(c->lcd, img, &src, &dst);
}

ret_t canvas_draw_image_at(canvas_t* c, bitmap_t* img, xy_t x, xy_t y) {
  rect_t src;
  rect_t dst;
  return_value_if_fail(c != NULL && img != NULL, RET_BAD_PARAMS);

  rect_init(src, 0, 0, img->w, img->h);
  rect_init(dst, x, y, img->w, img->h);

  return canvas_do_draw_image(c, img, &src, &dst);
}

ret_t canvas_draw_image(canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst) {
  rect_t d;
  return_value_if_fail(c != NULL && img != NULL && src != NULL && dst != NULL, RET_BAD_PARAMS);
  /*not support scale yet*/
  return_value_if_fail(src->w == dst->w && src->h == dst->h, RET_BAD_PARAMS);

  d.x = c->ox + dst->x;
  d.y = c->oy + dst->y;
  d.w = dst->w;
  d.h = dst->h;

  return canvas_do_draw_image(c, img, src, &d);
}

ret_t canvas_draw_image_9patch(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  int32_t w_w = 0;
  int32_t h_h = 0;
  const color_t* p = NULL;
  const color_t* data = NULL;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);
  data = (color_t*)img->data;
  return_value_if_fail(data != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;
  w_w = dst_w - img_w;
  h_h = dst_h - img_h;

  canvas_translate(c, dst->x, dst->y);

  w = ftk_min(img_w, dst_w) >> 1;
  h = ftk_min(img_h, dst_h) >> 1;

  rect_init(s, 0, 0, w, h);
  rect_init(d, 0, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  rect_init(s, w, 0, w, h);
  rect_init(d, dst_w - w, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  rect_init(s, 0, h, w, h);
  rect_init(d, 0, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  rect_init(s, w, h, w, h);
  rect_init(d, dst_w - w, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  x = w;
  if (img_w < dst_w) {
    p = data;

    for (y = 0; y < h; y++) {
      canvas_set_stroke_color(c, p[w]);
      canvas_draw_hline(c, x, y, w_w);
      p += img_w;
    }

    p += img_w * h_h;
    for (y = 0; y < h; y++) {
      canvas_set_stroke_color(c, p[w]);
      canvas_draw_hline(c, x, dst_h - h + y, w_w);
      p += img_w;
    }
  }

  y = h;
  if (img_h < dst_h) {
    p = data + img_w * h;

    for (x = 0; x < w; x++) {
      canvas_set_stroke_color(c, p[x]);
      canvas_draw_vline(c, x, y, h_h);
    }

    for (x = 0; x < w; x++) {
      canvas_set_stroke_color(c, p[w + w_w + x]);
      canvas_draw_vline(c, dst_w - w + x, y, h_h);
    }
  }

  if (img_w < dst_w && img_h < dst_h) {
    p = data + img_w * h + w;
    canvas_set_stroke_color(c, *p);
    canvas_fill_rect(c, w, h, w_w, h_h);
  }

  return RET_OK;
}

ret_t canvas_end_frame(canvas_t* c) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return lcd_end_frame(c->lcd);
}

ret_t canvas_test_paint(canvas_t* c, bool_t pressed, xy_t x, xy_t y) {
  color_t bg = color_init(0xff, 0xff, 0, 0xff);
  color_t fg = color_init(0xff, 0, 0, 0xff);

  canvas_begin_frame(c, NULL);
  canvas_set_clip_rect(c, NULL);
  canvas_set_fill_color(c, bg);
  canvas_set_stroke_color(c, fg);
  canvas_set_text_color(c, fg);
  canvas_set_font(c, NULL, 20);
  canvas_fill_rect(c, 10, 100, 200, 30);
  canvas_stroke_rect(c, 10, 100, 200, 30);
  canvas_draw_hline(c, 10, 300, 100);
  canvas_draw_vline(c, 10, 300, 100);
  canvas_draw_text(c, L"abababababa", 10, 15, 110);

  if (pressed) {
    canvas_set_fill_color(c, fg);
  } else {
    canvas_set_fill_color(c, bg);
  }

  canvas_fill_rect(c, x, y, 10, 10);

  canvas_end_frame(c);

  return RET_OK;
}

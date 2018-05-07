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
#include "base/wuxiaolin.inc"

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

  return c->lcd->w;
}

wh_t canvas_get_height(canvas_t* c) {
  return_value_if_fail(c != NULL, 0);

  return c->lcd->h;
}

ret_t canvas_set_clip_rect(canvas_t* c, rect_t* r) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  if (r) {
    if (c->lcd->set_clip_rect != NULL) {
      c->clip_left = 0;
      c->clip_top = 0;
      c->clip_right = c->lcd->w;
      c->clip_bottom = c->lcd->h;
      lcd_set_clip_rect(c->lcd, r);
    } else {
      c->clip_left = r->x;
      c->clip_top = r->y;
      c->clip_right = r->x + r->w;
      c->clip_bottom = r->y + r->h;
    }
  } else {
    c->clip_left = 0;
    c->clip_top = 0;
    c->clip_right = c->lcd->w;
    c->clip_bottom = c->lcd->h;
  }

  return RET_OK;
}

ret_t canvas_set_fill_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_set_fill_color(c->lcd, color);
  ;

  return RET_OK;
}

ret_t canvas_set_text_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_set_text_color(c->lcd, color);

  return RET_OK;
}

ret_t canvas_set_stroke_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_set_stroke_color(c->lcd, color);

  return RET_OK;
}

ret_t canvas_set_global_alpha(canvas_t* c, uint8_t alpha) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_set_global_alpha(c->lcd, alpha);

  return RET_OK;
}

ret_t canvas_set_font(canvas_t* c, const char* name, uint16_t size) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

  c->font_name = name;
  c->font_size = size;
  if (c->lcd->set_font_name != NULL) {
    lcd_set_font_name(c->lcd, name);
    lcd_set_font_size(c->lcd, size);
  } else {
    c->font = font_manager_find(c->font_manager, name, size);
  }

  return RET_OK;
}

static wh_t canvas_measure_text_default(canvas_t* c, wchar_t* str, int32_t nr) {
  glyph_t g;
  wh_t w = 0;
  int32_t i = 0;
  return_value_if_fail(c != NULL && str != NULL && c->font != NULL, 0);

  if (nr < 0) {
    nr = wcslen(str);
  }

  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];
    if (chr == ' ') {
      w += 4;
    } else if (font_find_glyph(c->font, chr, &g, c->font_size) == RET_OK) {
      w += g.w + 1;
    }
  }

  return w;
}

wh_t canvas_measure_text(canvas_t* c, wchar_t* str, int32_t nr) {
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, 0);

  if (c->lcd->measure_text) {
    return lcd_measure_text(c->lcd, str, nr);
  } else {
    return canvas_measure_text_default(c, str, nr);
  }
}

ret_t canvas_begin_frame(canvas_t* c, rect_t* dirty_rect, lcd_draw_mode_t draw_mode) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->ox = 0;
  c->oy = 0;
  canvas_set_clip_rect(c, dirty_rect);

  return lcd_begin_frame(c->lcd, dirty_rect, draw_mode);
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

static ret_t canvas_draw_line_impl(canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2) {
  if ((x1 < c->clip_left && x2 < c->clip_left) || (x1 > c->clip_right && x2 > c->clip_right) ||
      (y1 < c->clip_top && y2 < c->clip_top) || (y1 > c->clip_bottom && y2 > c->clip_bottom)) {
    return RET_OK;
  }

  if (x1 == x2) {
    return canvas_draw_vline_impl(c, x1, y1, ftk_abs(y2 - y1) + 1);
  } else if (y1 == y2) {
    return canvas_draw_hline_impl(c, x1, y1, ftk_abs(x2 - x1) + 1);
  } else {
    draw_line(c, x1, y1, x2, y2);
    return RET_OK;
  }
}

ret_t canvas_draw_line(canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_line_impl(c, c->ox + x1, c->oy + y1, c->ox + x2, c->oy + y2);
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
  return_value_if_fail(c != NULL && c->lcd != NULL && w > 0 && h > 0, RET_BAD_PARAMS);

  if (c->lcd->stroke_rect != NULL) {
    lcd_stroke_rect(c->lcd, x, y, w, h);
  } else {
    canvas_draw_hline_impl(c, x, y, w);
    canvas_draw_hline_impl(c, x, y + h - 1, w);
    canvas_draw_vline_impl(c, x, y, h);
    canvas_draw_vline_impl(c, x + w - 1, y, h);
  }

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

  y -= font_size * 1 / 3;
  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];
    if (chr == ' ') {
      x += 4;
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
      x += 4;
    }
  }

  return RET_OK;
}

ret_t canvas_draw_text(canvas_t* c, wchar_t* str, int32_t nr, xy_t x, xy_t y) {
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, RET_BAD_PARAMS);
  if (c->lcd->draw_text != NULL) {
    return lcd_draw_text(c->lcd, str, nr, c->ox + x, c->oy + y);
  } else {
    return canvas_draw_text_impl(c, str, nr, c->ox + x, c->oy + y);
  }
}

static ret_t canvas_do_draw_image(canvas_t* c, bitmap_t* img, rect_t* s, rect_t* d) {
  rect_t src;
  rect_t dst;

  xy_t x = d->x;
  xy_t y = d->y;
  xy_t x2 = d->x + d->w;
  xy_t y2 = d->y + d->h;

  if (d->w <= 0 || d->h <= 0 || s->w <= 0 || s->h <= 0 || x > c->clip_right || x2 < c->clip_left ||
      y > c->clip_bottom || y2 < c->clip_top) {
    return RET_OK;
  }

  dst.x = ftk_max(x, c->clip_left);
  dst.y = ftk_max(y, c->clip_top);
  dst.w = ftk_min(x2, c->clip_right) - dst.x;
  dst.h = ftk_min(y2, c->clip_bottom) - dst.y;

  src.x = s->x + (dst.x - x) * s->w / d->w;
  src.y = s->y + (dst.y - y) * s->h / d->h;
  src.w = dst.w * s->w / d->w;
  src.h = dst.h * s->h / d->h;

  if (src.x >= img->w || src.y >= img->h) {
    return RET_OK;
  }

  src.w = ftk_min((img->w - src.x), src.w);
  src.h = ftk_min((img->h - src.y), src.h);

  if (src.w == 0 || src.h == 0 || dst.w == 0 || dst.h == 0) {
    return RET_OK;
  }

  return lcd_draw_image(c->lcd, img, &src, &dst);
}

ret_t canvas_draw_image(canvas_t* c, bitmap_t* img, rect_t* src, rect_t* dst) {
  rect_t d;
  return_value_if_fail(c != NULL && img != NULL && src != NULL && dst != NULL, RET_BAD_PARAMS);

  d.x = c->ox + dst->x;
  d.y = c->oy + dst->y;
  d.w = dst->w;
  d.h = dst->h;

  return canvas_do_draw_image(c, img, src, &d);
}

ret_t canvas_draw_image_repeat(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;

  d = *dst;

  while (y < dst->h) {
    h = ftk_min(img->h, dst->h - y);
    while (x < dst->w) {
      w = ftk_min(img->w, dst->w - x);
      s.w = w;
      s.h = h;

      d.x = x + dst->x;
      d.y = y + dst->y;
      d.w = w;
      d.h = h;
      canvas_draw_image(c, img, &s, &d);
      x += w;
    }
    y += h;
    x = 0;
  }

  return RET_OK;
}

ret_t canvas_draw_image_repeat_x(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  wh_t w = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;

  d = *dst;

  while (x < dst->w) {
    w = ftk_min(img->w, dst->w - x);
    s.w = w;
    d.x = x;
    d.w = w;
    canvas_draw_image(c, img, &s, &d);
    x += w;
  }

  return RET_OK;
}

ret_t canvas_draw_image_repeat_y(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t y = 0;
  wh_t h = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;

  d = *dst;

  while (y < dst->h) {
    h = ftk_min(img->h, dst->h - y);
    s.h = h;
    d.y = y;
    d.h = h;
    canvas_draw_image(c, img, &s, &d);
    y += h;
  }

  return RET_OK;
}

ret_t canvas_draw_image_3patch_y_scale_x(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  wh_t h = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  h = ftk_min(img_h, dst_h) / 3;
  h_h = dst_h - h * 2;

  /*top*/
  rect_init(s, 0, 0, img_w, h);
  rect_init(d, 0, 0, dst_w, h);
  canvas_draw_image(c, img, &s, &d);

  /*middle*/
  rect_init(s, 0, h, img_w, img_h - 2 * h);
  rect_init(d, 0, h, dst_w, h_h);
  canvas_draw_image(c, img, &s, &d);

  /*bottom*/
  rect_init(s, 0, img_h - h, img_w, h);
  rect_init(d, 0, dst_h - h, dst_w, h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_3patch_y(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  wh_t h = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  h = ftk_min(img_h, dst_h) / 3;
  h_h = dst_h - h * 2;

  x = (dst->w - img->w) >> 1;
  /*top*/
  rect_init(s, 0, 0, img_w, h);
  rect_init(d, x, 0, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  /*middle*/
  rect_init(s, 0, h, img_w, img_h - 2 * h);
  rect_init(d, x, h, img_w, h_h);
  canvas_draw_image(c, img, &s, &d);

  /*bottom*/
  rect_init(s, 0, img_h - h, img_w, h);
  rect_init(d, x, dst_h - h, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);
  (void)dst_w;

  return RET_OK;
}

ret_t canvas_draw_image_3patch_x_scale_y(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  wh_t w = 0;
  wh_t w_w = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = ftk_min(img_w, dst_w) / 3;
  w_w = dst_w - w * 2;

  /*left*/
  rect_init(s, 0, 0, w, img_h);
  rect_init(d, 0, 0, w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  /*center*/
  rect_init(s, w, 0, img_w - 2 * w, img_h);
  rect_init(d, w, 0, w_w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  /*right*/
  rect_init(s, img_w - w, 0, w, img_h);
  rect_init(d, dst_w - w, 0, w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_3patch_x(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t y = 0;
  wh_t w = 0;
  wh_t w_w = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = ftk_min(img_w, dst_w) / 3;
  w_w = dst_w - w * 2;

  y = (dst_h - img_h) >> 1;
  /*left*/
  rect_init(s, 0, 0, w, img_h);
  rect_init(d, 0, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  /*center*/
  rect_init(s, w, 0, img_w - 2 * w, img_h);
  rect_init(d, w, y, w_w, img_h);
  canvas_draw_image(c, img, &s, &d);

  /*right*/
  rect_init(s, img_w - w, 0, w, img_h);
  rect_init(d, dst_w - w, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_9patch(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  wh_t w_w = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = ftk_min(img_w, dst_w) / 3;
  h = ftk_min(img_h, dst_h) / 3;

  w_w = dst_w - w * 2;
  h_h = dst_h - h * 2;

  /*draw four corners*/
  /*left top*/
  rect_init(s, 0, 0, w, h);
  rect_init(d, 0, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right top*/
  rect_init(s, img_w - w, 0, w, h);
  rect_init(d, dst_w - w, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*left bottom*/
  rect_init(s, 0, img_h - h, w, h);
  rect_init(d, 0, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right bottom*/
  rect_init(s, img_w - w, img_h - h, w, h);
  rect_init(d, dst_w - w, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*fill center*/
  x = w;
  if (w_w > 0) {
    rect_init(s, w, 0, img_w - 2 * w, h);
    rect_init(d, w, 0, w_w, h);
    canvas_draw_image(c, img, &s, &d);

    rect_init(s, w, img_h - h, img_w - 2 * w, h);
    rect_init(d, w, h + h_h, w_w, h);
    canvas_draw_image(c, img, &s, &d);
  }

  /*fill middle*/
  y = h;
  if (h_h > 0) {
    rect_init(s, 0, h, w, img_h - 2 * h);
    rect_init(d, 0, h, w, h_h);
    canvas_draw_image(c, img, &s, &d);

    rect_init(s, img_w - w, h, w, img_h - 2 * h);
    rect_init(d, w + w_w, h, w, h_h);
    canvas_draw_image(c, img, &s, &d);
  }

  /*fill center/middle*/
  if (w_w > 0 && h_h > 0) {
    rect_init(s, w, h, img_w - 2 * w, img_h - 2 * h);
    rect_init(d, w, h, w_w, h_h);
    canvas_draw_image(c, img, &s, &d);
  }

  canvas_untranslate(c, dst->x, dst->y);

  (void)x;
  (void)y;
  (void)dst_w;

  return RET_OK;
}

ret_t canvas_end_frame(canvas_t* c) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return lcd_end_frame(c->lcd);
}

ret_t canvas_test_paint(canvas_t* c, bool_t pressed, xy_t x, xy_t y) {
  color_t bg = color_init(0xff, 0xff, 0, 0xff);
  color_t fg = color_init(0xff, 0, 0, 0xff);

  canvas_begin_frame(c, NULL, LCD_DRAW_NORMAL);
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

ret_t canvas_draw_image_scale_w(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  wh_t src_h = 0;
  wh_t dst_h = 0;
  float scale = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  scale = (float)(dst->w) / img->w;
  dst_h = ftk_min(img->h * scale, dst->h);
  src_h = ftk_min(img->h, dst_h / scale);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = src_h;

  d = *dst;
  d.h = dst_h;

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_scale_h(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  wh_t src_w = 0;
  wh_t dst_w = 0;
  float scale = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  scale = (float)(dst->h) / img->h;
  dst_w = ftk_min(img->w * scale, dst->w);
  src_w = ftk_min(img->w, dst_w / scale);

  s.x = 0;
  s.y = 0;
  s.h = img->h;
  s.w = src_w;

  d = *dst;
  d.w = dst_w;

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_scale(canvas_t* c, bitmap_t* img, rect_t* dst) {
  rect_t s;
  rect_t d;
  float scale = 0;
  float scalex = 0;
  float scaley = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.h = img->h;
  s.w = img->w;

  scalex = (float)(dst->w) / img->w;
  scaley = (float)(dst->h) / img->h;
  scale = ftk_min(scalex, scaley);

  d.w = img->w * scale;
  d.h = img->h * scale;
  d.x = dst->x + ((dst->w - d.w) >> 1);
  d.y = dst->y + ((dst->h - d.h) >> 1);

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_ex(canvas_t* c, bitmap_t* img, image_draw_type_t draw_type, rect_t* dst) {
  rect_t src;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  switch (draw_type) {
    case IMAGE_DRAW_CENTER:
      return canvas_draw_image_center(c, img, dst);
    case IMAGE_DRAW_SCALE:
      rect_init(src, 0, 0, img->w, img->h);
      return canvas_draw_image(c, img, &src, dst);
    case IMAGE_DRAW_SCALE_AUTO: {
      return canvas_draw_image_scale(c, img, dst);
      case IMAGE_DRAW_SCALE_W:
        return canvas_draw_image_scale_w(c, img, dst);
      case IMAGE_DRAW_SCALE_H:
        return canvas_draw_image_scale_h(c, img, dst);
      case IMAGE_DRAW_REPEAT:
        return canvas_draw_image_repeat(c, img, dst);
      case IMAGE_DRAW_REPEAT_X:
        return canvas_draw_image_repeat_x(c, img, dst);
      case IMAGE_DRAW_REPEAT_Y:
        return canvas_draw_image_repeat_y(c, img, dst);
      case IMAGE_DRAW_9PATCH:
        return canvas_draw_image_9patch(c, img, dst);
      case IMAGE_DRAW_3PATCH_X:
        return canvas_draw_image_3patch_x(c, img, dst);
      case IMAGE_DRAW_3PATCH_Y:
        return canvas_draw_image_3patch_y(c, img, dst);
      case IMAGE_DRAW_3PATCH_X_SCALE_Y:
        return canvas_draw_image_3patch_x_scale_y(c, img, dst);
      case IMAGE_DRAW_3PATCH_Y_SCALE_X:
        return canvas_draw_image_3patch_y_scale_x(c, img, dst);
      default:
        return canvas_draw_image_center(c, img, dst);
    }
  }
}

ret_t canvas_draw_icon(canvas_t* c, bitmap_t* img, xy_t cx, xy_t cy) {
  rect_t src;
  rect_t dst;
  wh_t hw = 0;
  wh_t hh = 0;
  float_t ratio = 0;
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL, RET_BAD_PARAMS);

  ratio = c->lcd->ratio;
  rect_init(src, 0, 0, img->w, img->h);
  if (ratio > 1) {
    float_t w = (img->w / ratio);
    float_t h = (img->h / ratio);
    float_t hw = w/2;
    float_t hh = h/2;

    rect_init(dst, cx - hw, cy - hh, w, h);
  } else {
    hw = img->w >> 1;
    hh = img->h >> 1;
    rect_init(dst, cx - hw, cy - hh, img->w, img->h);
  }

  return canvas_draw_image(c, img, &src, &dst);
}

ret_t canvas_draw_image_center(canvas_t* c, bitmap_t* img, rect_t* dst) {
  xy_t cx = 0;
  xy_t cy = 0;
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  cx = dst->x + (dst->w >> 1);
  cy = dst->y + (dst->h >> 1);

  return canvas_draw_icon(c, img, cx, cy);
}

ret_t canvas_draw_image_at(canvas_t* c, bitmap_t* img, xy_t x, xy_t y) {
  rect_t src;
  rect_t dst;
  float_t ratio = 0;
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL, RET_BAD_PARAMS);

  ratio = c->lcd->ratio;
  rect_init(src, 0, 0, img->w, img->h);

  if (ratio > 1) {
    rect_init(dst, x, y, img->w / ratio, img->h / ratio);
  } else {
    rect_init(dst, x, y, img->w, img->h);
  }

  return canvas_do_draw_image(c, img, &src, &dst);
}

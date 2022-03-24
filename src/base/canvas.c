/**
 * File:   canvas.c
 * Author: AWTK Develop Team
 * Brief:  canvas provides basic drawings functions.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/wstr.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/bidi.h"
#include "base/canvas.h"
#include "base/vgcanvas.h"
#include "tkc/time_now.h"
#include "tkc/color_parser.h"
#include "base/system_info.h"

#include "base/lcd_profile.h"

#ifndef CANVAS_MEASURE_TEXT_CACHE_MAX_LENGTH
#define CANVAS_MEASURE_TEXT_CACHE_MAX_LENGTH 127
#endif

static ret_t canvas_draw_fps(canvas_t* c);
static ret_t canvas_draw_icon_ex(canvas_t* c, bitmap_t* img, const rect_t* src_r, xy_t cx, xy_t cy);
static ret_t canvas_draw_image_center_ex(canvas_t* c, bitmap_t* img, const rect_t* src_r,
                                         const rect_t* dst_in);

static bool_t bitmap_check_rect(bitmap_t* img, const rect_t* r) {
  return_value_if_fail(img != NULL && r != NULL, FALSE);

  return r->x >= 0 && r->y >= 0 && r->w > 0 && r->h > 0 && (r->x + r->w) <= img->w &&
         (r->y + r->h) <= img->h;
}

static rect_t* canvas_fix_rect(const rect_t* r, rect_t* o) {
  if (r != NULL) {
    *o = *r;

    if (o->w < 0) {
      o->w = -o->w;
      o->x = o->x - o->w + 1;
    }

    if (o->h < 0) {
      o->h = -o->h;
      o->y = o->y - o->h + 1;
    }

    return o;
  } else {
    return NULL;
  }
}

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

  c->lcd = lcd_profile_create(lcd);
  c->font_manager = font_manager;

  c->clip_left = 0;
  c->clip_top = 0;
  c->clip_right = canvas_get_width(c) - 1;
  c->clip_bottom = canvas_get_height(c) - 1;

  c->last_text_str = TKMEM_ZALLOCN(wchar_t, CANVAS_MEASURE_TEXT_CACHE_MAX_LENGTH + 1);
  return c;
}

wh_t canvas_get_width(canvas_t* c) {
  return_value_if_fail(c != NULL, 0);

  return lcd_get_width(c->lcd);
}

wh_t canvas_get_height(canvas_t* c) {
  return_value_if_fail(c != NULL, 0);

  return lcd_get_height(c->lcd);
}

ret_t canvas_set_font_manager(canvas_t* c, font_manager_t* font_manager) {
  return_value_if_fail(c != NULL && font_manager != NULL, RET_BAD_PARAMS);

  c->font_manager = font_manager;

  return RET_OK;
}

ret_t canvas_set_assets_manager(canvas_t* c, assets_manager_t* assets_manager) {
  vgcanvas_t* vgcanvas = NULL;
  return_value_if_fail(c != NULL && assets_manager != NULL, RET_BAD_PARAMS);

  vgcanvas = lcd_get_vgcanvas(c->lcd);
  c->assets_manager = assets_manager;
  if (vgcanvas != NULL) {
    vgcanvas_set_assets_manager(vgcanvas, assets_manager);
  }
  if (c->font_manager != NULL) {
    font_manager_set_assets_manager(c->font_manager, assets_manager);
  }

  return RET_OK;
}

ret_t canvas_get_clip_rect(canvas_t* c, rect_t* r) {
  return_value_if_fail(c != NULL && r != NULL, RET_BAD_PARAMS);

  r->x = c->clip_left;
  r->y = c->clip_top;
  r->w = c->clip_right - c->clip_left + 1;
  r->h = c->clip_bottom - c->clip_top + 1;

  if (c->lcd->get_clip_rect != NULL) {
    return lcd_get_clip_rect(c->lcd, r);
  }
  return RET_OK;
}

bool_t canvas_is_rect_in_clip_rect(canvas_t* c, xy_t left, xy_t top, xy_t right, xy_t bottom) {
  if (c->lcd->is_rect_in_clip_rect != NULL) {
    return c->lcd->is_rect_in_clip_rect(c->lcd, left, top, right, bottom);
  } else {
    if (left > c->clip_right || right < c->clip_left || top > c->clip_bottom ||
        bottom < c->clip_top) {
      return FALSE;
    }
    return TRUE;
  }
}

ret_t canvas_set_clip_rect(canvas_t* c, const rect_t* r_in) {
  wh_t lcd_w = 0;
  wh_t lcd_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* r = canvas_fix_rect(r_in, &r_fix);

  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_w = lcd_get_width(c->lcd);
  lcd_h = lcd_get_height(c->lcd);

  if (c->lcd->set_clip_rect != NULL) {
    if (r != NULL) {
      rect_t clip_r = rect_init(tk_max(0, r->x), tk_max(0, r->y), tk_max(0, r->w), tk_max(0, r->h));
      lcd_set_clip_rect(c->lcd, &clip_r);
    } else {
      rect_t clip_r = rect_init(0, 0, lcd_w, lcd_h);
      lcd_set_clip_rect(c->lcd, &clip_r);
    }
#ifdef WITH_GPU
    /* 兼容以前的逻辑，以免以前的用户不使用 canvas_is_rect_in_clip_rect 来判断出问题 */
    /* 把 canvas 的裁剪区设置为无限大，在 opengl 的状态下让 vg 来处理裁剪区的问题 */
    c->clip_left = 0;
    c->clip_top = 0;
    c->clip_right = 0x7fffffff;
    c->clip_bottom = 0x7fffffff;
#endif
  } else {
    if (r != NULL) {
      c->clip_left = tk_max(0, r->x);
      c->clip_top = tk_max(0, r->y);
      c->clip_right = tk_min(lcd_w - 1, r->x + r->w - 1);
      c->clip_bottom = tk_min(lcd_h - 1, r->y + r->h - 1);
    } else {
      c->clip_left = 0;
      c->clip_top = 0;
      c->clip_right = lcd_w - 1;
      c->clip_bottom = lcd_h - 1;
    }
  }

  return RET_OK;
}

ret_t canvas_set_clip_rect_ex(canvas_t* c, const rect_t* r_in, bool_t translate) {
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* r = canvas_fix_rect(r_in, &r_fix);
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  if (r != NULL && translate) {
    rect_t rr = *r;

    rr.x += c->ox;
    rr.y += c->oy;
    return canvas_set_clip_rect(c, &rr);
  } else {
    return canvas_set_clip_rect(c, r);
  }
}

ret_t canvas_set_fill_color(canvas_t* c, color_t color) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  lcd_set_fill_color(c->lcd, color);

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

  c->global_alpha = alpha;
  lcd_set_global_alpha(c->lcd, alpha);

  return RET_OK;
}

ret_t canvas_set_font(canvas_t* c, const char* name, font_size_t size) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

  name = system_info_fix_font_name(name);
  size = system_info()->font_scale * size;

  if (c->font_size != size) {
    c->font_size = size;
    c->last_text_length = 0;
  }
  if (!tk_str_eq(name, c->font_name)) {
    c->last_text_length = 0;
    c->font_name = tk_str_copy(c->font_name, name);
  }

  if (c->last_text_length == 0) {
    if (c->lcd->set_font_name != NULL) {
      lcd_set_font_name(c->lcd, c->font_name);
      lcd_set_font_size(c->lcd, size);
    } else {
      c->font = font_manager_get_font(c->font_manager, c->font_name, c->font_size);
    }
  }

  return RET_OK;
}

ret_t canvas_reset_font(canvas_t* c) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  c->font_size = 0;
  c->last_text_length = 0;
  c->font = NULL;
  TKMEM_FREE(c->font_name);

  return RET_OK;
}

ret_t canvas_set_text_align(canvas_t* c, align_h_t align_h, align_v_t align_v) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

  c->text_align_h = align_h;
  c->text_align_v = align_v;

  return RET_OK;
}

static float_t canvas_measure_text_default(canvas_t* c, const wchar_t* str, uint32_t nr) {
  glyph_t g;
  float_t w = 0;
  uint32_t i = 0;
  return_value_if_fail(c != NULL && str != NULL && c->font != NULL, 0);

  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];
    if (font_get_glyph(c->font, chr, c->font_size, &g) == RET_OK) {
      w += g.advance + 1;
    } else {
      w += 4;
    }
  }

  return w;
}

float_t canvas_measure_text(canvas_t* c, const wchar_t* str, uint32_t nr) {
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, 0);

  if (c->last_text_length != 0 && c->last_text_nr == nr && tk_wstr_eq(c->last_text_str, str)) {
    return c->last_text_length;
  } else {
    if (nr > CANVAS_MEASURE_TEXT_CACHE_MAX_LENGTH) {
      c->last_text_nr = 0;
      c->last_text_str[0] = 0;
    } else {
      tk_memcpy(c->last_text_str, str, nr * sizeof(wchar_t));
      c->last_text_nr = nr;
      c->last_text_str[nr] = 0;
    }
  }

  if (c->lcd->measure_text) {
    c->last_text_length = lcd_measure_text(c->lcd, str, nr);
  } else {
    c->last_text_length = canvas_measure_text_default(c, str, nr);
  }
  return c->last_text_length;
}

float_t canvas_measure_utf8(canvas_t* c, const char* str) {
  wstr_t s;
  float_t ret = 0;
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, 0);

  wstr_init(&s, 0);
  return_value_if_fail(wstr_set_utf8(&s, str) == RET_OK, 0);

  ret = canvas_measure_text(c, s.str, s.size);
  wstr_reset(&s);

  return ret;
}

ret_t canvas_begin_frame(canvas_t* c, const dirty_rects_t* dirty_rects, lcd_draw_mode_t draw_mode) {
  rect_t dirty_rect;
  ret_t ret = RET_OK;
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  if (c->began_frame) {
    return RET_OK;
  } else {
    c->began_frame = TRUE;
  }

  if (c->begin_frame) {
    return c->begin_frame(c, dirty_rects, draw_mode);
  }

  c->ox = 0;
  c->oy = 0;

  canvas_set_global_alpha(c, 0xff);
  lcd_set_canvas(c->lcd, c);
  /* 把当前帧的脏矩形列表传入到 lcd 中和 fb 的脏矩形列表合并得出新的脏矩形列表。*/
  if (c->lcd->support_dirty_rect) {
    ret = lcd_begin_frame(c->lcd, dirty_rects, draw_mode);
  } else {
    ret = lcd_begin_frame(c->lcd, NULL, draw_mode);
  }
  return_value_if_fail(ret == RET_OK, ret);
  /* 获取新的最大的脏矩形，然后设置裁剪区 */
  ret = lcd_get_dirty_rect(c->lcd, &dirty_rect);
  if (c->lcd->support_dirty_rect && ret == RET_OK) {
    if (draw_mode == LCD_DRAW_NORMAL && c->lcd->type == LCD_VGCANVAS) {
      rect_t r = dirty_rect;

      /*for vgcanvas anti alias*/
      r.x--;
      r.y--;
      r.w += 2;
      r.h += 2;

      canvas_set_clip_rect(c, &r);
    } else {
      canvas_set_clip_rect(c, &dirty_rect);
    }
  } else {
    canvas_set_clip_rect(c, NULL);
  }

  return ret;
}

static ret_t canvas_draw_hline_impl(canvas_t* c, xy_t x, xy_t y, wh_t w) {
  rect_t r;
  xy_t x2 = x + w - 1;

  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y)) {
    return RET_OK;
  }
  canvas_get_clip_rect(c, &r);

  x = tk_max(x, r.x);
  x2 = tk_min(x2, r.x + r.w - 1);
  w = x2 - x + 1;

  return lcd_draw_hline(c->lcd, x, y, w);
}

ret_t canvas_draw_hline(canvas_t* c, xy_t x, xy_t y, wh_t w) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  if (w < 0) {
    w = -w;
    x = x - w + 1;
  }

  return canvas_draw_hline_impl(c, c->ox + x, c->oy + y, w);
}

static ret_t canvas_draw_vline_impl(canvas_t* c, xy_t x, xy_t y, wh_t h) {
  rect_t r;
  xy_t y2 = y + h - 1;

  if (!canvas_is_rect_in_clip_rect(c, x, y, x, y2)) {
    return RET_OK;
  }
  canvas_get_clip_rect(c, &r);

  y = tk_max(y, r.y);
  y2 = tk_min(y2, r.y + r.h - 1);
  h = y2 - y + 1;

  return lcd_draw_vline(c->lcd, x, y, h);
}

ret_t canvas_draw_vline(canvas_t* c, xy_t x, xy_t y, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  if (h < 0) {
    h = -h;
    y = y - h + 1;
  }

  return canvas_draw_vline_impl(c, c->ox + x, c->oy + y, h);
}

static ret_t canvas_draw_line_impl(canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2) {
  if (x1 == x2) {
    return canvas_draw_vline_impl(c, x1, y1, tk_abs(y2 - y1) + 1);
  } else if (y1 == y2) {
    return canvas_draw_hline_impl(c, x1, y1, tk_abs(x2 - x1) + 1);
  } else {
    assert(!"Not implemented yet, please use vgcanvas to draw line");
    return RET_NOT_IMPL;
  }
}

ret_t canvas_draw_line(canvas_t* c, xy_t x1, xy_t y1, xy_t x2, xy_t y2) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_line_impl(c, c->ox + x1, c->oy + y1, c->ox + x2, c->oy + y2);
}

#define MAX_POINTS_PER_DRAW 20

static ret_t canvas_do_draw_points(canvas_t* c, const point_t* points, uint32_t nr) {
  uint32_t i = 0;
  uint32_t real_nr = 0;

  point_t real_points[MAX_POINTS_PER_DRAW];
  return_value_if_fail(nr <= MAX_POINTS_PER_DRAW, RET_BAD_PARAMS);

  for (i = 0; i < nr; i++) {
    const point_t* p = points + i;
    xy_t x = p->x + c->ox;
    xy_t y = p->y + c->oy;
    if (!canvas_is_rect_in_clip_rect(c, x, y, x, y)) {
      continue;
    }

    real_points[real_nr].x = x;
    real_points[real_nr].y = y;

    real_nr++;
  }

  return lcd_draw_points(c->lcd, real_points, real_nr);
}

static ret_t canvas_draw_points_impl(canvas_t* c, const point_t* points, uint32_t nr) {
  uint32_t i = 0;
  const point_t* p = points;
  uint32_t n = (nr / MAX_POINTS_PER_DRAW);
  uint32_t r = (nr % MAX_POINTS_PER_DRAW);

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

ret_t canvas_draw_points(canvas_t* c, const point_t* points, uint32_t nr) {
  return_value_if_fail(c != NULL && points != NULL, RET_BAD_PARAMS);

  return canvas_draw_points_impl(c, points, nr);
}

static ret_t canvas_fill_rect_impl(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t r;
  xy_t x2 = x + w - 1;
  xy_t y2 = y + h - 1;

  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
    return RET_OK;
  }
  canvas_get_clip_rect(c, &r);

  x = tk_max(x, r.x);
  y = tk_max(y, r.y);
  x2 = tk_min(x2, r.x + r.w - 1);
  y2 = tk_min(y2, r.y + r.h - 1);
  w = x2 - x + 1;
  h = y2 - y + 1;

  return lcd_fill_rect(c->lcd, x, y, w, h);
}

ret_t canvas_fill_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  return canvas_fill_rect_impl(c, c->ox + x, c->oy + y, w, h);
}

static ret_t canvas_fill_rect_gradient_impl(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h,
                                            gradient_t* gradient) {
  rect_t r;
  xy_t x2 = x + w - 1;
  xy_t y2 = y + h - 1;
  vgcanvas_t* vg = NULL;

  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
    return RET_OK;
  }

  canvas_get_clip_rect(c, &r);

  x = tk_max(x, r.x);
  y = tk_max(y, r.y);
  x2 = tk_min(x2, r.x + r.w - 1);
  y2 = tk_min(y2, r.y + r.h - 1);
  w = x2 - x + 1;
  h = y2 - y + 1;
  if (w == 0 || h == 0) {
    return RET_OK;
  }

  /*FIXME: to support general cases*/
  return_value_if_fail(gradient->type == GRADIENT_LINEAR && gradient->degree == 180,
                       RET_BAD_PARAMS);

#ifndef WITH_GPU
  if (gradient->type == GRADIENT_LINEAR) {
    if (gradient->degree == 180) {
      uint32_t i = 0;
      lcd_t* lcd = c->lcd;
      for (i = 0; i < h; i++) {
        float offset = (float)i / (float)h;
        color_t color = gradient_get_color(gradient, offset);
        lcd_set_stroke_color(lcd, color);
        lcd_draw_hline(lcd, x, y + i, w);
      }
      return RET_OK;
    }
  }
#endif
  vg = canvas_get_vgcanvas(c);
  if (vg != NULL) {
    vg_gradient_t vg_gradient;
    rect_t rect = {x, y, w, h};
    vg_gradient_init_with_gradient(&vg_gradient, &rect, gradient);
    vgcanvas_set_fill_gradient(vg, &vg_gradient);
    vgcanvas_rect(vg, x, y, w, h);
    vgcanvas_fill(vg);
  }

  return RET_NOT_IMPL;
}

ret_t canvas_fill_rect_gradient(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h, gradient_t* gradient) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  return canvas_fill_rect_gradient_impl(c, c->ox + x, c->oy + y, w, h, gradient);
}

static ret_t canvas_clear_rect_impl(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  rect_t r;
  xy_t x2 = x + w - 1;
  xy_t y2 = y + h - 1;

  if (!canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
    return RET_OK;
  }

  canvas_get_clip_rect(c, &r);

  x = tk_max(x, r.x);
  y = tk_max(y, r.y);
  x2 = tk_min(x2, r.x + r.w - 1);
  y2 = tk_min(y2, r.y + r.h - 1);
  w = x2 - x + 1;
  h = y2 - y + 1;

  return lcd_clear_rect(c->lcd, x, y, w, h);
}

ret_t canvas_clear_rect(canvas_t* c, xy_t x, xy_t y, wh_t w, wh_t h) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  return canvas_clear_rect_impl(c, c->ox + x, c->oy + y, w, h);
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

  fix_xywh(x, y, w, h);
  return canvas_stroke_rect_impl(c, c->ox + x, c->oy + y, w, h);
}

static ret_t canvas_draw_glyph(canvas_t* c, glyph_t* g, xy_t x, xy_t y) {
  rect_t r;
  rect_t src;
  rect_t dst;
  xy_t x2 = x + g->w - 1;
  xy_t y2 = y + g->h - 1;

  if (g->data == NULL || !canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
    return RET_OK;
  }
  canvas_get_clip_rect(c, &r);

  dst.x = tk_max(x, r.x);
  dst.y = tk_max(y, r.y);
  dst.w = tk_min(x2, r.x + r.w - 1) - dst.x + 1;
  dst.h = tk_min(y2, r.y + r.h - 1) - dst.y + 1;

  src.x = dst.x - x;
  src.y = dst.y - y;
  src.w = dst.w;
  src.h = dst.h;

  return lcd_draw_glyph(c->lcd, g, &src, dst.x, dst.y);
}

static ret_t canvas_draw_char_impl(canvas_t* c, wchar_t chr, xy_t x, xy_t y) {
  glyph_t g;
  font_size_t font_size = c->font_size;
  font_vmetrics_t vmetrics = font_get_vmetrics(c->font, c->font_size);
  return_value_if_fail(font_get_glyph(c->font, chr, font_size, &g) == RET_OK, RET_BAD_PARAMS);

  x += g.x;
  y += vmetrics.ascent + g.y;

  return canvas_draw_glyph(c, &g, x, y);
}

ret_t canvas_draw_char(canvas_t* c, wchar_t chr, xy_t x, xy_t y) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  return canvas_draw_char_impl(c, chr, c->ox + x, c->oy + y);
}

static ret_t canvas_draw_text_impl(canvas_t* c, const wchar_t* str, uint32_t nr, xy_t x, xy_t y,
                                   bool_t line_breaker) {
  glyph_t g;
  uint32_t i = 0;
  xy_t left = x;
  font_vmetrics_t vmetrics = font_get_vmetrics(c->font, c->font_size);
  font_size_t font_size = c->font_size;
  int32_t baseline = vmetrics.ascent;
  return_value_if_fail(c->font != NULL, RET_BAD_PARAMS);
  for (i = 0; i < nr; i++) {
    wchar_t chr = str[i];

    if (chr == '\r' || chr == '\n') {
      if ((i + 1) == nr) {
        break;
      }

      if (chr == '\r' && str[i + 1] == '\n') {
        i++;
      }

      if (line_breaker) {
        y += font_size;
        x = left;
        continue;
      } else {
        chr = ' ';
      }
    }

    if (font_get_glyph(c->font, chr, c->font_size, &g) == RET_OK) {
      xy_t xx = x + g.x;
      xy_t yy = y + g.y + baseline;

      canvas_draw_glyph(c, &g, xx, yy);
      x += g.advance + 1;
    } else {
      x += 4;
    }
  }

  return RET_OK;
}

ret_t canvas_draw_text(canvas_t* c, const wchar_t* str, uint32_t nr, xy_t x, xy_t y) {
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, RET_BAD_PARAMS);
  if (c->lcd->draw_text != NULL) {
    return lcd_draw_text(c->lcd, str, nr, c->ox + x, c->oy + y);
  } else {
    return canvas_draw_text_impl(c, str, nr, c->ox + x, c->oy + y, FALSE);
  }
}

ret_t canvas_draw_utf8(canvas_t* c, const char* str, xy_t x, xy_t y) {
  wstr_t s;
  ret_t ret = RET_FAIL;
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL, RET_BAD_PARAMS);

  wstr_init(&s, 0);
  return_value_if_fail(wstr_set_utf8(&s, str) == RET_OK, RET_OOM);

  ret = canvas_draw_text(c, s.str, s.size, x, y);
  wstr_reset(&s);

  return ret;
}

static ret_t canvas_do_draw_image(canvas_t* c, bitmap_t* img, const rect_t* s, const rect_t* d) {
  rect_t r;
  rectf_t src;
  rectf_t dst;
  float_t scale_w = 0;
  float_t scale_h = 0;

  xy_t x = d->x;
  xy_t y = d->y;
  xy_t x2 = d->x + d->w - 1;
  xy_t y2 = d->y + d->h - 1;

  if (d->w <= 0 || d->h <= 0 || s->w <= 0 || s->h <= 0 ||
      !canvas_is_rect_in_clip_rect(c, x, y, x2, y2)) {
    return RET_OK;
  }
  canvas_get_clip_rect(c, &r);

  dst.x = tk_max(x, r.x);
  dst.y = tk_max(y, r.y);
  dst.w = tk_min(x2, r.x + r.w - 1) - dst.x + 1;
  dst.h = tk_min(y2, r.y + r.h - 1) - dst.y + 1;

  /* 因为 blend 函数中缩放，使用 256 倍的定点数，所以这里为了减低多次转换数据出现误差 */
  scale_w = ((s->w << 8) / d->w / 256.0f);
  scale_h = ((s->h << 8) / d->h / 256.0f);

  src.x = s->x + (dst.x - x) * scale_w;
  src.y = s->y + (dst.y - y) * scale_h;
  src.w = dst.w * s->w / d->w;
  src.h = dst.h * s->h / d->h;

  if (src.x >= img->w || src.y >= img->h) {
    return RET_OK;
  }

  src.w = tk_min((img->w - src.x), src.w);
  src.h = tk_min((img->h - src.y), src.h);

  if (src.w <= 0 || src.h <= 0 || dst.w <= 0 || dst.h <= 0) {
    return RET_OK;
  }

  return lcd_draw_image(c->lcd, img, &src, &dst);
}

ret_t canvas_draw_image(canvas_t* c, bitmap_t* img, const rect_t* src, const rect_t* dst_in) {
  rect_t d;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && src != NULL && dst != NULL, RET_BAD_PARAMS);

  d.x = c->ox + dst->x;
  d.y = c->oy + dst->y;
  d.w = dst->w;
  d.h = dst->h;

  return canvas_do_draw_image(c, img, src, &d);
}

static ret_t canvas_draw_image_repeat_default(canvas_t* c, bitmap_t* img, const rect_t* src_in,
                                              const rect_t* dst_in, wh_t dst_w, wh_t dst_h) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  xy_t y = 0;
  wh_t sw = 0;
  wh_t sh = 0;
  wh_t dw = 0;
  wh_t dh = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL && src_in != NULL && dst_in != NULL,
                       RET_BAD_PARAMS);

  s.x = src_in->x;
  s.y = src_in->y;
  s.w = src_in->w;
  s.h = src_in->h;

  d = *dst;

  while (y < dst->h) {
    dh = tk_min(dst_h, dst->h - y);
    sh = tk_min(src_in->h, dst->h - y);
    while (x < dst->w) {
      dw = tk_min(dst_w, dst->w - x);
      sw = tk_min(src_in->w, dst->w - x);

      s.w = sw;
      s.h = sh;

      d.w = dw;
      d.h = dh;
      d.x = x + dst->x;
      d.y = y + dst->y;
      canvas_draw_image(c, img, &s, &d);
      x += dw;
    }
    y += dh;
    x = 0;
  }
  return RET_OK;
}

static ret_t canvas_draw_image_repeat_x_impl(canvas_t* c, bitmap_t* img, const rect_t* src_in,
                                             const rect_t* dst_in) {
  rect_t d;
  return_value_if_fail(
      c != NULL && c->lcd != NULL && img != NULL && src_in != NULL && dst_in != NULL,
      RET_BAD_PARAMS);
  d.x = dst_in->x + c->ox;
  d.y = dst_in->y + c->oy;
  d.w = dst_in->w;
  d.h = dst_in->h;

  if (c->lcd->draw_image_repeat != NULL &&
      c->lcd->draw_image_repeat(c->lcd, img, src_in, &d, src_in->w, dst_in->h) == RET_OK) {
    return RET_OK;
  }

  return canvas_draw_image_repeat_default(c, img, src_in, dst_in, src_in->w, dst_in->h);
}

static ret_t canvas_draw_image_repeat_y_impl(canvas_t* c, bitmap_t* img, const rect_t* src_in,
                                             const rect_t* dst_in) {
  rect_t d;
  return_value_if_fail(
      c != NULL && c->lcd != NULL && img != NULL && src_in != NULL && dst_in != NULL,
      RET_BAD_PARAMS);
  d.x = dst_in->x + c->ox;
  d.y = dst_in->y + c->oy;
  d.w = dst_in->w;
  d.h = dst_in->h;

  if (c->lcd->draw_image_repeat != NULL &&
      c->lcd->draw_image_repeat(c->lcd, img, src_in, &d, dst_in->w, src_in->h) == RET_OK) {
    return RET_OK;
  }

  return canvas_draw_image_repeat_default(c, img, src_in, dst_in, dst_in->w, src_in->h);
}

static ret_t canvas_draw_image_repeat_impl(canvas_t* c, bitmap_t* img, const rect_t* src_in,
                                           const rect_t* dst_in) {
  rect_t d;
  return_value_if_fail(
      c != NULL && c->lcd != NULL && img != NULL && src_in != NULL && dst_in != NULL,
      RET_BAD_PARAMS);
  d.x = dst_in->x + c->ox;
  d.y = dst_in->y + c->oy;
  d.w = dst_in->w;
  d.h = dst_in->h;

  if (c->lcd->draw_image_repeat != NULL &&
      c->lcd->draw_image_repeat(c->lcd, img, src_in, &d, src_in->w, src_in->h) == RET_OK) {
    return RET_OK;
  }

  return canvas_draw_image_repeat_default(c, img, src_in, dst_in, src_in->w, src_in->h);
}

ret_t canvas_draw_image_repeat(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;
  canvas_draw_image_repeat_impl(c, img, &s, dst_in);

  return RET_OK;
}

ret_t canvas_draw_image_repeat9(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
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
  wh_t image_w = 0;
  wh_t image_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  image_w = tk_min(img_w, dst_w);
  image_h = tk_min(img_h, dst_h);
  w = image_w >> 1;
  h = image_h >> 1;

  w_w = dst_w - w * 2;
  h_h = dst_h - h * 2;

  /*draw four corners*/
  /*left top*/
  s = rect_init(0, 0, w, h);
  d = rect_init(0, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right top*/
  s = rect_init(img_w - w, 0, w, h);
  d = rect_init(dst_w - w, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*left bottom*/
  s = rect_init(0, img_h - h, w, h);
  d = rect_init(0, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right bottom*/
  s = rect_init(img_w - w, img_h - h, w, h);
  d = rect_init(dst_w - w, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*fill center*/
  x = w;
  if (w_w > 0) {
    int32_t tmp = image_w - w - w;
    s = rect_init(tmp > 0 ? w : w - 1, 0, tmp > 0 ? tmp : 2, h);
    d = rect_init(w, 0, w_w, h);
    canvas_draw_image_repeat_x_impl(c, img, &s, &d);

    s = rect_init(tmp > 0 ? w : w - 1, img_h - h, tmp > 0 ? tmp : 2, h);
    d = rect_init(w, h + h_h, w_w, h);
    canvas_draw_image_repeat_x_impl(c, img, &s, &d);
  }

  /*fill middle*/
  y = h;
  if (h_h > 0) {
    int32_t tmp = image_h - h - h;
    s = rect_init(0, tmp > 0 ? h : h - 1, w, tmp > 0 ? tmp : 2);
    d = rect_init(0, h, w, h_h);
    canvas_draw_image_repeat_y_impl(c, img, &s, &d);

    s = rect_init(img_w - w, tmp > 0 ? h : h - 1, w, tmp > 0 ? tmp : 2);
    d = rect_init(w + w_w, h, w, h_h);
    canvas_draw_image_repeat_y_impl(c, img, &s, &d);
  }

  /*fill center/middle*/
  if (w_w > 0 && h_h > 0) {
    int32_t tmp_w = image_w - w - w;
    int32_t tmp_h = image_h - h - h;
    s = rect_init(tmp_w > 0 ? w : w - 1, tmp_h > 0 ? h : h - 1, tmp_w > 0 ? tmp_w : 2,
                  tmp_h > 0 ? tmp_h : 2);
    d = rect_init(w, h, w_w, h_h);
    canvas_draw_image_repeat_impl(c, img, &s, &d);
  }

  canvas_untranslate(c, dst->x, dst->y);

  (void)x;
  (void)y;
  (void)dst_w;

  return RET_OK;
}

ret_t canvas_draw_image_repeat_x(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;

  canvas_draw_image_repeat_x_impl(c, img, &s, dst_in);

  return RET_OK;
}

ret_t canvas_draw_image_repeat3_x(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  xy_t y = 0;
  wh_t w = 0;
  wh_t w_w = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  wh_t image_w = 0;
  int32_t tmp = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  image_w = tk_min(img_w, dst_w);
  w = image_w >> 1;
  w_w = dst_w - (w << 1);

  y = (dst_h - img_h) >> 1;
  if (y < 0) {
    img_h += y;
    y = 0;
  }

  /*left*/
  s = rect_init(0, 0, w, img_h);
  d = rect_init(0, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  /*center*/
  tmp = image_w - w - w;
  if (dst_w > img_w) {
    s = rect_init(w, 0, tmp > 0 ? tmp : 2, img_h);
    d = rect_init(w, y, w_w, img_h);
    canvas_draw_image_repeat_x_impl(c, img, &s, &d);
  }

  /*right*/
  s = rect_init(img_w - w, 0, w, img_h);
  d = rect_init(dst_w - w, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_repeat_y(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;
  canvas_draw_image_repeat_y_impl(c, img, &s, dst_in);
  return RET_OK;
}

ret_t canvas_draw_image_repeat3_y(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  wh_t h = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  wh_t image_h = 0;
  int32_t tmp = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  image_h = tk_min(img_h, dst_h);
  h = image_h >> 1;
  h_h = dst_h - (h << 1);

  x = (dst->w - img->w) >> 1;
  if (x < 0) {
    img_w += x;
    x = 0;
  }
  /*top*/
  s = rect_init(0, 0, img_w, h);
  d = rect_init(x, 0, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  /*middle*/
  tmp = image_h - h - h;
  if (dst_h > img_h) {
    s = rect_init(0, tmp > 0 ? h : h - 1, img_w, tmp > 0 ? tmp : 2);
    d = rect_init(x, h, img_w, h_h);
    canvas_draw_image_repeat_y_impl(c, img, &s, &d);
  }

  /*bottom*/
  s = rect_init(0, img_h - h, img_w, h);
  d = rect_init(x, dst_h - h, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);
  (void)dst_w;

  return RET_OK;
}

ret_t canvas_draw_image_repeat_y_inverse(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  xy_t y = 0;
  wh_t h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = img->h;

  d = *dst;

  while (y < dst->h) {
    h = tk_min(img->h, dst->h - y);
    s.h = h;
    d.h = h;
    y += h;
    d.y = dst->y + (dst->h - y);

    if (s.h < img->h) {
      s.y = img->h - s.h;
      canvas_draw_image(c, img, &s, &d);
    } else {
      s.y = 0;
      canvas_draw_image(c, img, &s, &d);
    }
  }

  return RET_OK;
}

ret_t canvas_draw_image_patch3_y_scale_x(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  wh_t h = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  h = tk_min(img_h, dst_h) / 3;
  h_h = dst_h - h * 2;

  /*top*/
  s = rect_init(0, 0, img_w, h);
  d = rect_init(0, 0, dst_w, h);
  canvas_draw_image(c, img, &s, &d);

  /*middle*/
  s = rect_init(0, h, img_w, img_h - 2 * h);
  d = rect_init(0, h, dst_w, h_h);
  canvas_draw_image(c, img, &s, &d);

  /*bottom*/
  s = rect_init(0, img_h - h, img_w, h);
  d = rect_init(0, dst_h - h, dst_w, h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_patch3_y(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  xy_t x = 0;
  wh_t h = 0;
  wh_t h_h = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  h = tk_min(img_h, dst_h) / 3;
  h_h = dst_h - h * 2;

  x = (dst->w - img->w) >> 1;
  /*top*/
  s = rect_init(0, 0, img_w, h);
  d = rect_init(x, 0, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  /*middle*/
  s = rect_init(0, h, img_w, img_h - 2 * h);
  d = rect_init(x, h, img_w, h_h);
  canvas_draw_image(c, img, &s, &d);

  /*bottom*/
  s = rect_init(0, img_h - h, img_w, h);
  d = rect_init(x, dst_h - h, img_w, h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);
  (void)dst_w;

  return RET_OK;
}

ret_t canvas_draw_image_patch3_x_scale_y(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  wh_t w = 0;
  wh_t w_w = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = tk_min(img_w, dst_w) / 3;
  w_w = dst_w - w * 2;

  /*left*/
  s = rect_init(0, 0, w, img_h);
  d = rect_init(0, 0, w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  /*center*/
  s = rect_init(w, 0, img_w - 2 * w, img_h);
  d = rect_init(w, 0, w_w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  /*right*/
  s = rect_init(img_w - w, 0, w, img_h);
  d = rect_init(dst_w - w, 0, w, dst_h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_patch3_x(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  xy_t y = 0;
  wh_t w = 0;
  wh_t w_w = 0;
  wh_t img_w = 0;
  wh_t img_h = 0;
  wh_t dst_w = 0;
  wh_t dst_h = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = tk_min(img_w, dst_w) / 3;
  w_w = dst_w - w * 2;

  y = (dst_h - img_h) >> 1;
  /*left*/
  s = rect_init(0, 0, w, img_h);
  d = rect_init(0, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  /*center*/
  s = rect_init(w, 0, img_w - 2 * w, img_h);
  d = rect_init(w, y, w_w, img_h);
  canvas_draw_image(c, img, &s, &d);

  /*right*/
  s = rect_init(img_w - w, 0, w, img_h);
  d = rect_init(dst_w - w, y, w, img_h);
  canvas_draw_image(c, img, &s, &d);

  canvas_untranslate(c, dst->x, dst->y);

  return RET_OK;
}

ret_t canvas_draw_image_patch9(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
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
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);

  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  img_w = img->w;
  img_h = img->h;
  dst_w = dst->w;
  dst_h = dst->h;

  canvas_translate(c, dst->x, dst->y);

  w = tk_min(img_w, dst_w) / 3;
  h = tk_min(img_h, dst_h) / 3;

  w_w = dst_w - w * 2;
  h_h = dst_h - h * 2;

  /*draw four corners*/
  /*left top*/
  s = rect_init(0, 0, w, h);
  d = rect_init(0, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right top*/
  s = rect_init(img_w - w, 0, w, h);
  d = rect_init(dst_w - w, 0, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*left bottom*/
  s = rect_init(0, img_h - h, w, h);
  d = rect_init(0, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*right bottom*/
  s = rect_init(img_w - w, img_h - h, w, h);
  d = rect_init(dst_w - w, dst_h - h, w, h);
  canvas_draw_image(c, img, &s, &d);

  /*fill center*/
  x = w;
  if (w_w > 0) {
    s = rect_init(w, 0, img_w - 2 * w, h);
    d = rect_init(w, 0, w_w, h);
    canvas_draw_image(c, img, &s, &d);

    s = rect_init(w, img_h - h, img_w - 2 * w, h);
    d = rect_init(w, h + h_h, w_w, h);
    canvas_draw_image(c, img, &s, &d);
  }

  /*fill middle*/
  y = h;
  if (h_h > 0) {
    s = rect_init(0, h, w, img_h - 2 * h);
    d = rect_init(0, h, w, h_h);
    canvas_draw_image(c, img, &s, &d);

    s = rect_init(img_w - w, h, w, img_h - 2 * h);
    d = rect_init(w + w_w, h, w, h_h);
    canvas_draw_image(c, img, &s, &d);
  }

  /*fill center/middle*/
  if (w_w > 0 && h_h > 0) {
    s = rect_init(w, h, img_w - 2 * w, img_h - 2 * h);
    d = rect_init(w, h, w_w, h_h);
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
  if (c->began_frame) {
    c->began_frame = FALSE;
  } else {
    return RET_OK;
  }

  if (c->end_frame != NULL) {
    return c->end_frame(c);
  }

  canvas_draw_fps(c);
  canvas_set_global_alpha(c, 0xff);

  return lcd_end_frame(c->lcd);
}

ret_t canvas_draw_image_scale_w(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  wh_t src_h = 0;
  wh_t dst_h = 0;
  float scale = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  scale = (float)(dst->w) / img->w;
  dst_h = tk_min(img->h * scale, dst->h);
  src_h = tk_min(img->h, dst_h / scale);

  s.x = 0;
  s.y = 0;
  s.w = img->w;
  s.h = src_h;

  d = *dst;
  d.h = dst_h;

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_scale_h(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  wh_t src_w = 0;
  wh_t dst_w = 0;
  float scale = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  scale = (float)(dst->h) / img->h;
  dst_w = tk_min(img->w * scale, dst->w);
  src_w = tk_min(img->w, dst_w / scale);

  s.x = 0;
  s.y = 0;
  s.h = img->h;
  s.w = src_w;

  d = *dst;
  d.w = dst_w;

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_scale(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t s;
  rect_t d;
  float scale = 0;
  float scalex = 0;
  float scaley = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  s.x = 0;
  s.y = 0;
  s.h = img->h;
  s.w = img->w;

  scalex = (float)(dst->w) / img->w;
  scaley = (float)(dst->h) / img->h;
  scale = tk_min(scalex, scaley);

  d.w = img->w * scale;
  d.h = img->h * scale;
  d.x = dst->x + ((dst->w - d.w) >> 1);
  d.y = dst->y + ((dst->h - d.h) >> 1);

  return canvas_draw_image(c, img, &s, &d);
}

ret_t canvas_draw_image_scale_down(canvas_t* c, bitmap_t* img, const rect_t* src,
                                   const rect_t* dst_in) {
  rect_t d;
  float scale = 0;
  float scalex = 0;
  float scaley = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && src != NULL && dst != NULL, RET_BAD_PARAMS);

  scalex = (float)(dst->w) / src->w;
  scaley = (float)(dst->h) / src->h;
  scale = tk_min(scalex, scaley);

  if (scale >= 1) {
    d.w = src->w;
    d.h = src->h;
  } else {
    d.w = src->w * scale;
    d.h = src->h * scale;
  }
  d.x = dst->x + ((dst->w - d.w) >> 1);
  d.y = dst->y + ((dst->h - d.h) >> 1);

  return canvas_draw_image(c, img, src, &d);
}

ret_t canvas_draw_image_matrix(canvas_t* c, bitmap_t* img, matrix_t* matrix) {
  draw_image_info_t info;
  return_value_if_fail(c != NULL && img != NULL && matrix != NULL && c->lcd != NULL,
                       RET_BAD_PARAMS);

  info.img = img;
  info.matrix = *matrix;
  info.src = rect_init(0, 0, img->w, img->h);
  info.dst = rect_init(0, 0, img->w, img->h);
  canvas_get_clip_rect(c, &info.clip);

  return lcd_draw_image_matrix(c->lcd, &info);
}

ret_t canvas_draw_image_ex(canvas_t* c, bitmap_t* img, image_draw_type_t draw_type,
                           const rect_t* dst_in) {
  rect_t src;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);
  switch (draw_type) {
    case IMAGE_DRAW_DEFAULT:
      src = rect_init(0, 0, tk_min(dst->w, img->w), tk_min(dst->h, img->h));
      dst->w = src.w;
      dst->h = src.h;
      return canvas_draw_image(c, img, &src, dst);
    case IMAGE_DRAW_CENTER:
      return canvas_draw_image_center(c, img, dst);
    case IMAGE_DRAW_SCALE:
      src = rect_init(0, 0, img->w, img->h);
      return canvas_draw_image(c, img, &src, dst);
    case IMAGE_DRAW_SCALE_AUTO:
      return canvas_draw_image_scale(c, img, dst);
    case IMAGE_DRAW_SCALE_DOWN: {
      rect_t src = rect_init(0, 0, img->w, img->h);
      return canvas_draw_image_scale_down(c, img, &src, dst);
    }
#ifndef AWTK_LITE
    case IMAGE_DRAW_ICON: {
      xy_t cx = dst->x + (dst->w >> 1);
      xy_t cy = dst->y + (dst->h >> 1);
      return canvas_draw_icon(c, img, cx, cy);
    }
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
    case IMAGE_DRAW_REPEAT_Y_INVERSE:
      return canvas_draw_image_repeat_y_inverse(c, img, dst);
    case IMAGE_DRAW_PATCH9:
      return canvas_draw_image_patch9(c, img, dst);
    case IMAGE_DRAW_PATCH3_X:
      return canvas_draw_image_patch3_x(c, img, dst);
    case IMAGE_DRAW_PATCH3_Y:
      return canvas_draw_image_patch3_y(c, img, dst);
    case IMAGE_DRAW_PATCH3_X_SCALE_Y:
      return canvas_draw_image_patch3_x_scale_y(c, img, dst);
    case IMAGE_DRAW_PATCH3_Y_SCALE_X:
      return canvas_draw_image_patch3_y_scale_x(c, img, dst);
    case IMAGE_DRAW_REPEAT9:
      return canvas_draw_image_repeat9(c, img, dst);
    case IMAGE_DRAW_REPEAT3_X:
      return canvas_draw_image_repeat3_x(c, img, dst);
    case IMAGE_DRAW_REPEAT3_Y:
      return canvas_draw_image_repeat3_y(c, img, dst);
#endif /*AWTK_LITE*/
    default:
      return canvas_draw_image_center(c, img, dst);
  }
}

ret_t canvas_draw_image_ex2(canvas_t* c, bitmap_t* img, image_draw_type_t draw_type,
                            const rect_t* src_in, const rect_t* dst_in) {
  rect_t src;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(bitmap_check_rect(img, src_in), RET_BAD_PARAMS);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  switch (draw_type) {
    case IMAGE_DRAW_DEFAULT:
      src = *src_in;
      dst->w = src.w;
      dst->h = src.h;
      return canvas_draw_image(c, img, &src, dst);
    case IMAGE_DRAW_ICON: {
      xy_t cx = dst->x + (dst->w >> 1);
      xy_t cy = dst->y + (dst->h >> 1);
      return canvas_draw_icon_ex(c, img, src_in, cx, cy);
    }
    case IMAGE_DRAW_CENTER:
      return canvas_draw_image_center_ex(c, img, src_in, dst);
    case IMAGE_DRAW_SCALE:
      return canvas_draw_image(c, img, src_in, dst);
    default:
      /*TODO*/
      assert(!"not supported yet!");
      return RET_FAIL;
  }
}

static ret_t canvas_draw_icon_ex(canvas_t* c, bitmap_t* img, const rect_t* src_r, xy_t cx,
                                 xy_t cy) {
  rect_t src;
  rect_t dst;
  wh_t hw = 0;
  wh_t hh = 0;
  float_t ratio = 0;
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL && src_r != NULL, RET_BAD_PARAMS);

  src = *src_r;
  ratio = c->lcd->ratio;
  if (ratio > 1) {
    float_t w = (src.w / ratio);
    float_t h = (src.h / ratio);
    float_t hw = w / 2;
    float_t hh = h / 2;

    dst = rect_init(cx - hw, cy - hh, w, h);
  } else {
    hw = src.w >> 1;
    hh = src.h >> 1;
    dst = rect_init(cx - hw, cy - hh, src.w, src.h);
  }

  return canvas_draw_image(c, img, &src, &dst);
}

ret_t canvas_draw_icon(canvas_t* c, bitmap_t* img, xy_t cx, xy_t cy) {
  rect_t src;
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL, RET_BAD_PARAMS);
  src = rect_init(0, 0, img->w, img->h);

  return canvas_draw_icon_ex(c, img, &src, cx, cy);
}

ret_t canvas_draw_icon_in_rect(canvas_t* c, bitmap_t* img, const rect_t* r_in) {
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* r = canvas_fix_rect(r_in, &r_fix);
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL && r != NULL, RET_BAD_PARAMS);

  return canvas_draw_icon(c, img, r->x + (r->w >> 1), r->y + (r->h >> 1));
}

static ret_t canvas_draw_image_center_ex(canvas_t* c, bitmap_t* img, const rect_t* src_r,
                                         const rect_t* dst_in) {
  xy_t dx = 0;
  xy_t dy = 0;
  xy_t sx = 0;
  xy_t sy = 0;
  wh_t sw = 0;
  wh_t sh = 0;
  rect_t src;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  rect_t* dst = canvas_fix_rect(dst_in, &r_fix);
  return_value_if_fail(c != NULL && img != NULL && dst != NULL, RET_BAD_PARAMS);

  dx = ((dst->w - src_r->w) >> 1);
  dy = ((dst->h - src_r->h) >> 1);

  if (dx < 0) {
    sx = -dx;
    dx = 0;
    sw = src_r->w - 2 * sx;
  } else {
    sw = src_r->w;
  }

  if (dy < 0) {
    sy = -dy;
    dy = 0;
    sh = src_r->h - 2 * sy;
  } else {
    sh = src_r->h;
  }

  dx += dst->x;
  dy += dst->y;

  src = rect_init(sx + src_r->x, sy + src_r->y, sw, sh);
  *dst = rect_init(dx, dy, sw, sh);

  return canvas_draw_image(c, img, &src, dst);
}

ret_t canvas_draw_image_center(canvas_t* c, bitmap_t* img, const rect_t* dst_in) {
  rect_t src;
  return_value_if_fail(c != NULL && img != NULL && dst_in != NULL, RET_BAD_PARAMS);
  src = rect_init(0, 0, img->w, img->h);

  return canvas_draw_image_center_ex(c, img, &src, dst_in);
}

ret_t canvas_draw_image_at(canvas_t* c, bitmap_t* img, xy_t x, xy_t y) {
  rect_t src;
  rect_t dst;
  float_t ratio = 0;
  return_value_if_fail(c != NULL && c->lcd != NULL && img != NULL, RET_BAD_PARAMS);

  x += c->ox;
  y += c->oy;
  ratio = c->lcd->ratio;
  src = rect_init(0, 0, img->w, img->h);

  if (ratio > 1) {
    dst = rect_init(x, y, img->w / ratio, img->h / ratio);
  } else {
    dst = rect_init(x, y, img->w, img->h);
  }

  return canvas_do_draw_image(c, img, &src, &dst);
}

ret_t canvas_set_fps(canvas_t* c, bool_t show_fps, uint32_t fps) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  c->show_fps = show_fps;
  c->fps = fps;

  return RET_OK;
}

static ret_t canvas_draw_fps(canvas_t* c) {
  lcd_t* lcd = c->lcd;

  if (c->show_fps && c->lcd->draw_mode == LCD_DRAW_NORMAL) {
    rect_t r;
    char fps[20];
    wchar_t wfps[20];

    r = rect_init(0, 0, 60, 30);
    canvas_set_font(c, NULL, 16);
    canvas_set_text_color(c, color_init(0xf0, 0xf0, 0xf0, 0xff));
    canvas_set_fill_color(c, color_init(0x20, 0x20, 0x20, 0xff));

    lcd->fps_rect = r;
    tk_snprintf(fps, sizeof(fps), "%dfps", (int)(c->fps));

    tk_utf8_to_utf16(fps, wfps, strlen(fps) + 1);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
    canvas_draw_text(c, wfps, wcslen(wfps), r.x + 8, r.y + 8);
  } else {
    lcd->fps_rect.w = 0;
    lcd->fps_rect.h = 0;
  }

  return RET_OK;
}

float_t canvas_get_font_height(canvas_t* c) {
  font_vmetrics_t vmetrics;
  return_value_if_fail(c != NULL && c->font_size > 0, 0);
  vmetrics = font_get_vmetrics(c->font, c->font_size);

  return vmetrics.ascent - vmetrics.descent;
}

ret_t canvas_draw_text_in_rect(canvas_t* c, const wchar_t* str, uint32_t nr, const rect_t* r_in) {
  int x = 0;
  int y = 0;
  rect_t r_fix = rect_init(0, 0, 0, 0);
  int32_t text_w = 0;
  int32_t height = canvas_get_font_height(c);
  rect_t* r = canvas_fix_rect(r_in, &r_fix);
  return_value_if_fail(c != NULL && str != NULL && r != NULL, RET_BAD_PARAMS);

  text_w = canvas_measure_text(c, str, nr);

  switch (c->text_align_v) {
    case ALIGN_V_TOP:
      y = r->y;
      break;
    case ALIGN_V_BOTTOM:
      y = r->y + (r->h - height);
      break;
    default:
      y = r->y + ((r->h - height) >> 1);
      break;
  }

  switch (c->text_align_h) {
    case ALIGN_H_LEFT:
      x = r->x;
      break;
    case ALIGN_H_RIGHT:
      x = r->x + (r->w - text_w);
      break;
    default:
      x = r->x + ((r->w - text_w) >> 1);
      break;
  }

  return canvas_draw_text(c, str, nr, x, y);
}

#define STR_ELLIPSES L"..."

static ret_t canvas_draw_text_in_rect_ellipses(canvas_t* c, const wchar_t* str, uint32_t nr,
                                               const rect_t* r_in, bidi_type_t type) {
  uint32_t i = 0;
  rect_t r = *r_in;
  float_t text_w = 0;
  float_t ellipses_w = canvas_measure_text(c, STR_ELLIPSES, wcslen(STR_ELLIPSES));

  for (i = 0; i < nr; i++) {
    float_t char_w = canvas_measure_text(c, str + i, 1);
    if ((text_w + char_w + ellipses_w) >= r.w) {
      break;
    }

    text_w += char_w;
  }

  r.w = text_w;
  canvas_draw_text_in_rect(c, str, i, &r);
  r.x = text_w;
  r.w = ellipses_w;
  canvas_draw_text_in_rect(c, STR_ELLIPSES, wcslen(STR_ELLIPSES), &r);

  return RET_OK;
}

ret_t canvas_draw_text_bidi_in_rect(canvas_t* c, const wchar_t* str, uint32_t nr,
                                    const rect_t* r_in, const char* bidi_type, bool_t ellipses) {
  bidi_t b;
  ret_t ret = RET_FAIL;
  return_value_if_fail(c != NULL && str != NULL && r_in != NULL, RET_BAD_PARAMS);

  if (nr == 0) {
    return RET_OK;
  }

  bidi_init(&b, FALSE, FALSE, bidi_type_from_name(bidi_type));
  if (bidi_log2vis(&b, str, nr) == RET_OK) {
    float_t text_w = canvas_measure_text(c, b.vis_str, b.vis_str_size);
    if (ellipses && text_w > r_in->w) {
      ret = canvas_draw_text_in_rect_ellipses(c, b.vis_str, b.vis_str_size, r_in, b.resolved_type);
    } else {
      ret = canvas_draw_text_in_rect(c, b.vis_str, b.vis_str_size, r_in);
    }
  } else {
    assert(!"log2vis failed!");
  }

  bidi_deinit(&b);

  return ret;
}

ret_t canvas_draw_utf8_in_rect(canvas_t* c, const char* str, const rect_t* r) {
  wstr_t s;
  ret_t ret = RET_FAIL;
  return_value_if_fail(c != NULL && c->lcd != NULL && str != NULL && r != NULL, RET_BAD_PARAMS);

  wstr_init(&s, 0);
  return_value_if_fail(wstr_set_utf8(&s, str) == RET_OK, RET_OOM);

  ret = canvas_draw_text_in_rect(c, s.str, s.size, r);
  wstr_reset(&s);

  return ret;
}

vgcanvas_t* canvas_get_vgcanvas(canvas_t* c) {
  vgcanvas_t* vg = NULL;
  return_value_if_fail(c != NULL && c->lcd != NULL, NULL);

  vg = lcd_get_vgcanvas(c->lcd);
  if (vg != NULL) {
    rect_t r;
    canvas_get_clip_rect(c, &r);
    vgcanvas_clip_rect(vg, r.x, r.y, r.w, r.h);
    vgcanvas_begin_path(vg);
    vgcanvas_set_text_align(vg, "left");
    vgcanvas_set_text_baseline(vg, "top");
  }

  return vg;
}

ret_t canvas_set_stroke_color_str(canvas_t* c, const char* color) {
  return_value_if_fail(c != NULL && color != NULL, RET_BAD_PARAMS);

  return canvas_set_stroke_color(c, color_parse(color));
}

ret_t canvas_set_fill_color_str(canvas_t* c, const char* color) {
  return_value_if_fail(c != NULL && color != NULL, RET_BAD_PARAMS);

  return canvas_set_fill_color(c, color_parse(color));
}

ret_t canvas_set_text_color_str(canvas_t* c, const char* color) {
  return_value_if_fail(c != NULL && color != NULL, RET_BAD_PARAMS);

  return canvas_set_text_color(c, color_parse(color));
}

ret_t canvas_save(canvas_t* c) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

#if defined(AWTK_WEB)
  vgcanvas_save(lcd_get_vgcanvas(c->lcd));
#endif /*AWTK_WEB*/

  return RET_OK;
}

ret_t canvas_restore(canvas_t* c) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

#if defined(AWTK_WEB)
  vgcanvas_restore(lcd_get_vgcanvas(c->lcd));
#endif /*AWTK_WEB*/

  return RET_OK;
}

canvas_t* canvas_cast(canvas_t* c) {
  return c;
}

ret_t canvas_reset(canvas_t* c) {
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(c->font_name);
  TKMEM_FREE(c->last_text_str);
  memset(c, 0x00, sizeof(canvas_t));

  return RET_OK;
}

ret_t canvas_get_text_metrics(canvas_t* c, float_t* ascent, float_t* descent, float_t* line_hight) {
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ascent != NULL && descent != NULL && line_hight != NULL, RET_BAD_PARAMS);

  if (c->lcd != NULL && c->lcd->get_text_metrics != NULL) {
    return c->lcd->get_text_metrics(c->lcd, ascent, descent, line_hight);
  } else if (c->font != NULL) {
    font_vmetrics_t vmetrics = font_get_vmetrics(c->font, c->font_size);

    *ascent = vmetrics.ascent;
    *descent = vmetrics.descent;
    *line_hight = vmetrics.line_gap + vmetrics.ascent + vmetrics.descent;
    return RET_OK;
  } else {
    *ascent = 0;
    *descent = 0;
    *line_hight = 0;
    return RET_FAIL;
  }
}

#ifndef WITHOUT_ROUNDED_RECT
#include "ffr_draw_rounded_rect.inc"

ret_t canvas_fill_rounded_rect(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                               const color_t* color, uint32_t radius) {
  gradient_t gradient;
  gradient_init_simple(&gradient, color->color);
  return ffr_draw_fill_rounded_rect_ex(c, r, bg_r, &gradient, radius, radius, radius, radius);
}

ret_t canvas_fill_rounded_rect_gradient(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                        const gradient_t* gradient, uint32_t radius) {
  return ffr_draw_fill_rounded_rect_ex(c, r, bg_r, gradient, radius, radius, radius, radius);
}

ret_t canvas_stroke_rounded_rect(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                 const color_t* color, uint32_t radius, uint32_t border_width) {
  return ffr_draw_stroke_rounded_rect_ex(c, r, bg_r, color, radius, radius, radius, radius,
                                         border_width, BORDER_ALL);
}

ret_t canvas_fill_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                  const color_t* color, uint32_t radius_tl, uint32_t radius_tr,
                                  uint32_t radius_bl, uint32_t radius_br) {
  gradient_t gradient;
  gradient_init_simple(&gradient, color->color);
  return ffr_draw_fill_rounded_rect_ex(c, r, bg_r, &gradient, radius_tl, radius_tr, radius_bl,
                                       radius_br);
}

ret_t canvas_fill_rounded_rect_gradient_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                           const gradient_t* gradient, uint32_t radius_tl,
                                           uint32_t radius_tr, uint32_t radius_bl,
                                           uint32_t radius_br) {
  return ffr_draw_fill_rounded_rect_ex(c, r, bg_r, gradient, radius_tl, radius_tr, radius_bl,
                                       radius_br);
}

ret_t canvas_stroke_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                    const color_t* color, uint32_t radius_tl, uint32_t radius_tr,
                                    uint32_t radius_bl, uint32_t radius_br, uint32_t border_width,
                                    int32_t border_model) {
  return ffr_draw_stroke_rounded_rect_ex(c, r, bg_r, color, radius_tl, radius_tr, radius_bl,
                                         radius_br, border_width, border_model);
}

#else
ret_t canvas_fill_rounded_rect(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                               const color_t* color, uint32_t radius) {
  canvas_set_fill_color(c, *color);
  canvas_fill_rect(c, r->x, r->y, r->w, r->h);

  return RET_OK;
}

ret_t canvas_fill_rounded_rect_gradient(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                        const gradient_t* gradient, uint32_t radius) {
  color_t color = gradient_get_first_color(gradient);
  return canvas_fill_rounded_rect(c, r, bg_r, (const color_t*)&color, radius);
}

ret_t canvas_stroke_rounded_rect(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                 const color_t* color, uint32_t radius, uint32_t border_width) {
  canvas_set_stroke_color(c, *color);
  canvas_stroke_rect(c, r->x, r->y, r->w, r->h);
  return RET_OK;
}

ret_t canvas_fill_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                  const color_t* color, uint32_t radius_tl, uint32_t radius_tr,
                                  uint32_t radius_bl, uint32_t radius_br) {
  canvas_set_fill_color(c, *color);
  canvas_fill_rect(c, r->x, r->y, r->w, r->h);
  return RET_OK;
}

ret_t canvas_fill_rounded_rect_gradient_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                           const gradient_t* gradient, uint32_t radius_tl,
                                           uint32_t radius_tr, uint32_t radius_bl,
                                           uint32_t radius_br) {
  color_t color = gradient_get_first_color(gradient);
  return canvas_fill_rounded_rect_ex(c, r, bg_r, (const color_t*)&color, radius_tl, radius_tr,
                                     radius_bl, radius_br);
}

ret_t canvas_stroke_rounded_rect_ex(canvas_t* c, const rect_t* r, const rect_t* bg_r,
                                    const color_t* color, uint32_t radius_tl, uint32_t radius_tr,
                                    uint32_t radius_bl, uint32_t radius_br, uint32_t border_width,
                                    int32_t border_model) {
  canvas_set_stroke_color(c, *color);
  canvas_stroke_rect(c, r->x, r->y, r->w, r->h);
  return RET_OK;
}
#endif /*WITHOUT_ROUNDED_RECT*/

#ifndef AWTK_LITE
#include "canvas_offline.inc"
#endif
#include "gradient.inc"

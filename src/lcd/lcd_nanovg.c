/**
 * File:   lcd.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lcd interface
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
 * 2018-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

//#define GL_GLEXT_PROTOTYPES
#define NANOVG_GL2_IMPLEMENTATION

#include "base/lcd.h"
#include "base/utf8.h"
#include "lcd/lcd_nanovg.h"
#include "base/resource_manager.h"

#include "glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "nanovg.h"
#include "nanovg_gl.h"

#define MAX_IMAGE_NR 256

typedef struct _lcd_nanovg_t {
  lcd_t base;
  NVGcontext* vg;
  SDL_Window* sdl_window;
  rect_t dirty_rect;

  int font_id;
  float ratio;
  const uint8_t* images[MAX_IMAGE_NR];
} lcd_nanovg_t;

static ret_t lcd_nanovg_set_font_name(lcd_t* lcd, const char* name) {
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  int font_id = nvgFindFont(vg, name);

  if (name == NULL) {
    name = STR_DEFAULT_FONT;
  }

  if (font_id < 0) {
    const resource_info_t* r = resource_manager_ref(RESOURCE_TYPE_FONT, name);
    if (r == NULL || r->subtype != RESOURCE_TYPE_FONT_TTF) {
      name = "default_ttf";
      r = resource_manager_ref(RESOURCE_TYPE_FONT, name);
    }

    if (r != NULL && r->subtype == RESOURCE_TYPE_FONT_TTF) {
      nano->font_id = nvgCreateFontMem(vg, name, (unsigned char*)r->data, r->size, 0);
    }
  } else {
    nano->font_id = font_id;
  }
  lcd->font_name = name;

  return RET_OK;
}

static ret_t lcd_nanovg_set_font_size(lcd_t* lcd, uint32_t size) { return RET_OK; }

static ret_t lcd_nanovg_sync_fill_color(lcd_t* lcd, NVGcontext* vg) {
  color_t color = lcd->fill_color;
  nvgFillColor(vg, nvgRGBA(color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a));

  return RET_OK;
}

static ret_t lcd_nanovg_sync_text_color(lcd_t* lcd, NVGcontext* vg) {
  color_t color = lcd->text_color;
  nvgFillColor(vg, nvgRGBA(color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a));

  return RET_OK;
}

static ret_t lcd_nanovg_sync_stroke_color(lcd_t* lcd, NVGcontext* vg) {
  color_t color = lcd->stroke_color;
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;

  nvgStrokeWidth(vg, 1);
  nvgStrokeColor(vg, nvgRGBA(color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a));

  return RET_OK;
}

static ret_t lcd_nanovg_begin_frame(lcd_t* lcd, rect_t* dirty_rect) {
  int ww = 0;
  int wh = 0;
  int fw = 0;
  int fh = 0;
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;
  NVGcontext* vg = nano->vg;
  SDL_Window* sdl_window = nano->sdl_window;

  if (dirty_rect) {
    nano->dirty_rect = *dirty_rect;
  }

  SDL_GetWindowSize(sdl_window, &ww, &wh);
  SDL_GL_GetDrawableSize(sdl_window, &fw, &fh);
  nano->ratio = (float)fw / (float)ww;

  glViewport(0, 0, fw, fh);
  glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  nvgBeginFrame(vg, ww, wh, nano->ratio);

  return RET_OK;
}

static ret_t lcd_nanovg_set_clip_rect(lcd_t* lcd, rect_t* rect) { return RET_OK; }

static ret_t lcd_nanovg_set_global_alpha(lcd_t* lcd, uint8_t alpha) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  nvgGlobalAlpha(vg, (float)alpha / 255.0);

  return RET_OK;
}

static ret_t lcd_nanovg_draw_vline(lcd_t* lcd, xy_t x, xy_t y, wh_t h) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  lcd_nanovg_sync_stroke_color(lcd, vg);
  nvgBeginPath(vg);
  nvgMoveTo(vg, x, y);
  nvgLineTo(vg, x, y + h);
  nvgStroke(vg);

  return RET_OK;
}

static ret_t lcd_nanovg_draw_hline(lcd_t* lcd, xy_t x, xy_t y, wh_t w) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  lcd_nanovg_sync_stroke_color(lcd, vg);
  nvgBeginPath(vg);
  nvgMoveTo(vg, x, y);
  nvgLineTo(vg, x + w, y);
  nvgStroke(vg);

  return RET_OK;
}

static ret_t lcd_nanovg_fill_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  lcd_nanovg_sync_fill_color(lcd, vg);
  nvgBeginPath(vg);
  nvgRect(vg, x, y, w, h);
  nvgFill(vg);

  return RET_OK;
}

static ret_t lcd_nanovg_stroke_rect(lcd_t* lcd, xy_t x, xy_t y, wh_t w, wh_t h) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  lcd_nanovg_sync_stroke_color(lcd, vg);
  nvgBeginPath(vg);
  nvgRect(vg, x, y, w, h);
  nvgStroke(vg);

  return RET_OK;
}

static ret_t lcd_nanovg_draw_points(lcd_t* lcd, point_t* points, uint32_t nr) {
  uint32_t i = 0;
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;

  lcd_nanovg_sync_stroke_color(lcd, vg);
  nvgBeginPath(vg);
  for (i = 0; i < nr; i++) {
    float x = points[i].x;
    float y = points[i].y;

    nvgMoveTo(vg, x, y);
    nvgLineTo(vg, x, y);
  }
  nvgStroke(vg);

  return RET_OK;
}

static int lcd_nanovg_ensure_image(lcd_nanovg_t* nano, bitmap_t* img) {
  int32_t i = 0;
  NVGcontext* vg = nano->vg;

  for (i = 0; i < MAX_IMAGE_NR; i++) {
    if (nano->images[i] == (img->data)) {
      return i;
    }
  }

  i = nvgCreateImageRGBA(vg, img->w, img->h, NVG_IMAGE_NEAREST, img->data);
  if (i >= 0) {
    assert(i < MAX_IMAGE_NR);
    nano->images[i] = img->data;
  }

  return i;
}

static ret_t lcd_nanovg_draw_image(lcd_t* lcd, bitmap_t* img, rect_t* src, rect_t* dst) {
  int sx = src->x;
  int sy = src->y;
  int sw = src->w;
  int sh = src->h;
  int dx = dst->x;
  int dy = dst->y;
  int dw = dst->w;
  int dh = dst->h;
  int iw = img->w;
  int ih = img->h;
  NVGpaint imgPaint;
  float scaleX = (float)dw / sw;
  float scaleY = (float)dh / sh;
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  int id = lcd_nanovg_ensure_image(nano, img);
  return_value_if_fail(id >= 0, RET_BAD_PARAMS);

  imgPaint = nvgImagePattern(vg, 0, 0, iw, ih, 0, id, 1);

  nvgSave(vg);
  nvgBeginPath(vg);
  nvgTranslate(vg, dx - (sx * scaleX), dy - (sy * scaleY));
  nvgScale(vg, scaleX, scaleY);
  nvgRect(vg, sx, sy, sw, sh);
  nvgFillPaint(vg, imgPaint);
  nvgClosePath(vg);
  nvgFill(vg);
  nvgRestore(vg);

  return RET_OK;
}

wh_t lcd_nanovg_measure_text(lcd_t* lcd, wchar_t* str, int32_t nr) {
  wh_t w = 0;
  int ret = 0;
  float bounds[4];
  char text[1024];
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  nvgFontFaceId(vg, nano->font_id);
  nvgFontSize(vg, lcd->font_size);
  nvgFontSize(vg, lcd->font_size);

  utf8_from_utf16(str, text, sizeof(text));
  ret = nvgTextBounds(vg, 0, 0, text, text + strlen(text), bounds);
  w = bounds[2];

  return ret;
}

static ret_t lcd_nanovg_draw_text(lcd_t* lcd, wchar_t* str, int32_t nr, xy_t x, xy_t y) {
  char text[1024];
  lcd_nanovg_t* nano = (lcd_nanovg_t*)lcd;
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  nvgFontFaceId(vg, nano->font_id);
  nvgFontSize(vg, lcd->font_size);
  lcd_nanovg_sync_text_color(lcd, vg);
  nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

  utf8_from_utf16(str, text, sizeof(text));
  nvgText(vg, x, y, text, text + strlen(text));

  return RET_OK;
}

static ret_t lcd_nanovg_end_frame(lcd_t* lcd) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  SDL_Window* sdl_window = ((lcd_nanovg_t*)lcd)->sdl_window;

  nvgEndFrame(vg);
  SDL_GL_SwapWindow(sdl_window);

  return RET_OK;
}

static ret_t lcd_nanovg_destroy(lcd_t* lcd) {
  NVGcontext* vg = ((lcd_nanovg_t*)lcd)->vg;
  (void)vg;
  return RET_OK;
}

vgcanvas_t* lcd_nanovg_get_vgcanvas(lcd_t* lcd) { return NULL; }

lcd_t* lcd_nanovg_init(SDL_Window* sdl_window) {
  int w = 0;
  int h = 0;
  static lcd_nanovg_t lcd;
  lcd_t* base = &(lcd.base);
  return_value_if_fail(sdl_window != NULL, NULL);

  memset(&lcd, 0x00, sizeof(lcd_nanovg_t));

  lcd.sdl_window = sdl_window;
  base->begin_frame = lcd_nanovg_begin_frame;
  base->set_clip_rect = lcd_nanovg_set_clip_rect;
  base->draw_vline = lcd_nanovg_draw_vline;
  base->draw_hline = lcd_nanovg_draw_hline;
  base->fill_rect = lcd_nanovg_fill_rect;
  base->stroke_rect = lcd_nanovg_stroke_rect;
  base->draw_image = lcd_nanovg_draw_image;
  base->draw_points = lcd_nanovg_draw_points;
  base->draw_text = lcd_nanovg_draw_text;
  base->measure_text = lcd_nanovg_measure_text;
  base->end_frame = lcd_nanovg_end_frame;
  base->get_vgcanvas = lcd_nanovg_get_vgcanvas;
  base->set_font_name = lcd_nanovg_set_font_name;
  base->set_font_size = lcd_nanovg_set_font_size;
  base->set_global_alpha = lcd_nanovg_set_global_alpha;
  base->destroy = lcd_nanovg_destroy;

  SDL_GetWindowSize(sdl_window, &w, &h);
  base->w = (wh_t)w;
  base->h = (wh_t)h;

  gladLoadGL();
  lcd.vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  return base;
}

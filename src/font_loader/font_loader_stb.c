/**
 * File:   font_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb truetype font loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#define STB_TRUETYPE_IMPLEMENTATION

#include "base/mem.h"

#define STBTT_free(p, u) TKMEM_FREE(p)
#define STBTT_malloc(s, u) TKMEM_ALLOC(s)

#include "base/glyph_cache.h"
#include "stb/stb_truetype.h"
#include "font_loader/font_loader_stb.h"

typedef struct _font_stb_t {
  font_t base;
  stbtt_fontinfo stb_font;
  glyph_cache_t cache;
  int ascent;
  int descent;
  int lineGap;

} font_stb_t;

static bool_t font_stb_match(font_t* f, const char* name, uint16_t font_size) {
  (void)font_size;
  return (name == NULL || strcmp(name, f->name) == 0);
}

static int32_t font_stb_get_baseline(font_t* f, uint16_t font_size) {
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  return scale * font->ascent;
}

static ret_t font_stb_find_glyph(font_t* f, wchar_t c, glyph_t* g, uint16_t font_size) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  if (glyph_cache_lookup(&(font->cache), c, font_size, g) == RET_OK) {
    return RET_OK;
  }

  g->data = stbtt_GetCodepointBitmap(sf, 0, scale, c, &w, &h, &x, &y);
  g->x = x;
  g->y = y;
  g->w = w;
  g->h = h;

  glyph_cache_add(&(font->cache), c, font_size, g);
  stbtt_GetGlyphBitmapBox(sf, c, 0, scale, &x1, &y1, &x2, &y2);

  return g->data != NULL ? RET_OK : RET_NOT_FOUND;
}

static ret_t font_stb_destroy(font_t* f) {
  font_stb_t* font = (font_stb_t*)f;
  glyph_cache_deinit(&(font->cache));

  TKMEM_FREE(f);

  return RET_OK;
}

static ret_t destroy_glyph(void* data) {
  glyph_t* g = (glyph_t*)data;
  if (g->data) {
    STBTT_free(g->data, NULL);
  }

  return RET_OK;
}

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t buff_size) {
  font_stb_t* f = NULL;
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_stb_t);
  return_value_if_fail(f != NULL, NULL);

  f->base.name = name;
  f->base.match = font_stb_match;
  f->base.find_glyph = font_stb_find_glyph;
  f->base.get_baseline = font_stb_get_baseline;
  f->base.destroy = font_stb_destroy;

  glyph_cache_init(&(f->cache), 256, destroy_glyph);
  stbtt_InitFont(&(f->stb_font), buff, stbtt_GetFontOffsetForIndex(buff, 0));
  stbtt_GetFontVMetrics(&(f->stb_font), &(f->ascent), &(f->descent), &(f->lineGap));

  return &(f->base);
}

static font_t* font_stb_load(font_loader_t* loader, const char* name, const uint8_t* buff,
                             uint32_t buff_size) {
  (void)loader;

  return font_stb_create(name, buff, buff_size);
}

font_loader_t* font_loader_stb(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_stb_load;

  return &loader;
}

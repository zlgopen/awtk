/**
 * File:   font_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb truetype font loader
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/types_def.h"
#include "font_loader/font_loader_stb.h"

#ifdef WITH_STB_FONT

#define STB_TRUETYPE_IMPLEMENTATION

#include "tkc/mem.h"

#define STBTT_free(p, u) TKMEM_FREE(p)
#define STBTT_malloc(s, u) TKMEM_ALLOC(s)

#include "base/glyph_cache.h"
#include "stb/stb_truetype.h"

typedef struct _font_stb_t {
  font_t base;
  stbtt_fontinfo stb_font;
  glyph_cache_t cache;
  int ascent;
  int descent;
  int line_gap;

} font_stb_t;

static bool_t font_stb_match(font_t* f, const char* name, font_size_t font_size) {
  (void)font_size;
  return (name == NULL || strcmp(name, f->name) == 0);
}

static font_vmetrics_t font_stb_get_vmetrics(font_t* f, font_size_t font_size) {
  font_vmetrics_t vmetrics;
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  vmetrics.ascent = scale * font->ascent;
  vmetrics.descent = scale * font->descent;
  vmetrics.line_gap = scale * font->line_gap;

  return vmetrics;
}

static ret_t font_stb_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int lsb = 0;
  int advance = 0;
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  if (glyph_cache_lookup(&(font->cache), c, font_size, g) == RET_OK) {
    return RET_OK;
  }

  g->data = stbtt_GetCodepointBitmap(sf, 0, scale, c, &w, &h, &x, &y);
  stbtt_GetCodepointHMetrics(sf, c, &advance, &lsb);

  g->x = x;
  g->y = y;
  g->w = w;
  g->h = h;
  g->format = GLYPH_FMT_ALPHA;
  g->advance = advance * scale;

  if (g->data != NULL) {
    glyph_t* gg = glyph_clone(g);
    if (gg != NULL) {
      if (glyph_cache_add(&(font->cache), c, font_size, gg) != RET_OK) {
        TKMEM_FREE(gg);
        gg = NULL;
      }
    }
    if (gg == NULL) {
      STBTT_free(g->data, NULL);
      log_warn("out of memory\n");
      g->data = NULL;
    }
  }

  return g->data != NULL || c == ' ' ? RET_OK : RET_NOT_FOUND;
}

static ret_t font_stb_shrink_cache(font_t* f, uint32_t cache_nr) {
  font_stb_t* font = (font_stb_t*)f;

  return glyph_cache_shrink(&(font->cache), cache_nr);
}

static ret_t font_stb_destroy(font_t* f) {
  font_stb_t* font = (font_stb_t*)f;
  glyph_cache_deinit(&(font->cache));

  TKMEM_FREE(f);

  return RET_OK;
}

static ret_t destroy_glyph(void* data) {
  glyph_t* g = (glyph_t*)data;
  if (g->data != NULL) {
    STBTT_free(g->data, NULL);
  }
  glyph_destroy(g);

  return RET_OK;
}

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t buff_size) {
  font_stb_t* f = NULL;
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_stb_t);
  return_value_if_fail(f != NULL, NULL);

  f->base.match = font_stb_match;
  f->base.destroy = font_stb_destroy;
  f->base.get_glyph = font_stb_get_glyph;
  f->base.get_vmetrics = font_stb_get_vmetrics;
  f->base.shrink_cache = font_stb_shrink_cache;
  f->base.desc = "truetype(stb)";

  tk_strncpy(f->base.name, name, TK_NAME_LEN);

  glyph_cache_init(&(f->cache), TK_GLYPH_CACHE_NR, destroy_glyph);
  stbtt_InitFont(&(f->stb_font), buff, stbtt_GetFontOffsetForIndex(buff, 0));
  stbtt_GetFontVMetrics(&(f->stb_font), &(f->ascent), &(f->descent), &(f->line_gap));

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
#else
font_loader_t* font_loader_stb(void) {
  return NULL;
}

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t buff_size) {
  return NULL;
}
#endif /*WITH_STB_FONT*/

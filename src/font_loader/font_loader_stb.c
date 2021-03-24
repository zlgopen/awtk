/**
 * File:   font_stb.h
 * Author: AWTK Develop Team
 * Brief:  stb truetype font loader
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-03-22 Li YuXin <liuyuxin@zlg.cn> improve mono
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
  bool_t mono;
} font_stb_t;

static ret_t font_stb_gray_to_mono_by_threshold(const glyph_t* gray, glyph_t* mono,
                                                uint32_t threshold) {
  return_value_if_fail(gray != NULL && mono != NULL && gray->format == GLYPH_FMT_ALPHA,
                       RET_BAD_PARAMS);
  uint32_t i = 0;
  uint32_t j = 0;
  uint16_t h = gray->h;
  uint16_t w = gray->w;

  mono->format = GLYPH_FMT_MONO;
  mono->h = h;
  mono->w = w;
  mono->pitch = ((mono->w + 15) >> 4) << 1;
  mono->x = gray->x;
  mono->y = gray->y;
  mono->advance = gray->advance;

  uint32_t nmemb = mono->pitch * h;
  uint8_t* bitmap = TKMEM_CALLOC(nmemb, sizeof(uint8_t));
  return_value_if_fail(bitmap != NULL, RET_OOM);
  for (j = 0; j < h; ++j) {
    for (i = 0; i < w; ++i) {
      if (gray->data[w * j + i] > threshold) {
        uint32_t offset = j * mono->pitch + (i >> 3);
        uint32_t offset_bit = 7 - (i % 8);
        bitmap[offset] += 0x1 << offset_bit;
      }
    }
  }
  mono->data = bitmap;
  return RET_OK;
}

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
  uint8_t* bitmap = NULL;
  font_stb_t* font = (font_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  if (glyph_cache_lookup(&(font->cache), c, font_size, g) == RET_OK) {
    return RET_OK;
  }

  bitmap = stbtt_GetCodepointBitmap(sf, 0, scale, c, &w, &h, &x, &y);
  stbtt_GetCodepointHMetrics(sf, c, &advance, &lsb);

  g->x = x;
  g->y = y;
  g->w = w;
  g->h = h;
  g->format = GLYPH_FMT_ALPHA;
  g->advance = advance * scale;
  g->data = NULL;

  if (bitmap != NULL) {
    if (font->mono) {
      glyph_t* gray_g = glyph_clone(g);
      gray_g->data = bitmap;
      uint32_t threshold =
          font_size > 24
              ? (font_size > 48 ? (font_size > 71 ? (font_size > 95 ? 195 : 175) : 160) : 118)
              : 95;
      font_stb_gray_to_mono_by_threshold(gray_g, g, threshold);
      STBTT_free(bitmap, NULL);
      glyph_destroy(gray_g);
    } else {
      g->data = bitmap;
    }
    return_value_if_fail(g->data != NULL, RET_FAIL);

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

static font_t* font_stb_create_ex(const char* name, const uint8_t* buff, uint32_t buff_size,
                                  bool_t mono) {
  font_stb_t* f = NULL;
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_stb_t);
  return_value_if_fail(f != NULL, NULL);

  f->mono = mono;
  f->base.match = font_stb_match;
  f->base.destroy = font_stb_destroy;
  f->base.get_glyph = font_stb_get_glyph;
  f->base.get_vmetrics = font_stb_get_vmetrics;
  f->base.shrink_cache = font_stb_shrink_cache;
  f->base.desc = mono ? "mono(stb)" : "truetype(stb)";

  tk_strncpy(f->base.name, name, TK_NAME_LEN);

  glyph_cache_init(&(f->cache), TK_GLYPH_CACHE_NR, destroy_glyph);
  stbtt_InitFont(&(f->stb_font), buff, stbtt_GetFontOffsetForIndex(buff, 0));
  stbtt_GetFontVMetrics(&(f->stb_font), &(f->ascent), &(f->descent), &(f->line_gap));

  return &(f->base);
}

font_t* font_stb_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return font_stb_create_ex(name, buff, size, TRUE);
}

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t size) {
#ifdef WITH_LCD_MONO
  return font_stb_create_ex(name, buff, size, TRUE);
#else
  return font_stb_create_ex(name, buff, size, FALSE);
#endif
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

static font_t* font_stb_load_mono(font_loader_t* loader, const char* name, const uint8_t* buff,
                                  uint32_t buff_size) {
  (void)loader;

  return font_stb_create_ex(name, buff, buff_size, TRUE);
}

font_loader_t* font_loader_stb_mono(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_stb_load_mono;

  return &loader;
}
#else
font_loader_t* font_loader_stb(void) {
  return NULL;
}

font_t* font_stb_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}

font_t* font_stb_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}
#endif /*WITH_STB_FONT*/

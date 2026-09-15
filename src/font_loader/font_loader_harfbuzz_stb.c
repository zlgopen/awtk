/**
 * File:   font_loader_hb_stb.c
 * Author: AWTK Develop Team
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-03-24 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "tkc/utils.h"
#include "base/bidi.h"
#include "base/types_def.h"
#include "font_loader/font_loader_harfbuzz_stb.h"

#if defined(WITH_STB_FONT) && \
    (defined(WITH_HARFBUZZ_DATA_TEXT_SHAPING) || defined(WITH_HARFBUZZ_TEXT_SHAPING))

#include "tkc/mem.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_free(p, u) TKMEM_FREE(p)
#define STBTT_malloc(s, u) TKMEM_ALLOC(s)

#include "base/glyph_cache.h"
#include "stb/stb_truetype.h"
#include "harfbuzz/harfbuzz_tools.h"
#include "3rd/harfbuzz/hb.h"

#ifdef WITH_HARFBUZZ_TEXT_SHAPING
#include "harfbuzz/harfbuzz_loader.h"
#elif defined(WITH_HARFBUZZ_DATA_TEXT_SHAPING)
#include "harfbuzz/harfbuzz_data_loader.h"
#endif /* WITH_HARFBUZZ_TEXT_SHAPING */

typedef struct _hb_str_t {
  uint32_t cur_gid;
  uint32_t cur_index;
  hb_info_t* hb_info;
} hb_str_t;

typedef struct _font_hb_stb_t {
  font_t base;
  stbtt_fontinfo stb_font;
  glyph_cache_t cache;
  glyph_cache_t hb_cache;
  int ascent;
  int descent;
  int line_gap;
  bool_t mono;
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob_t* blob;
  hb_face_t* face;
  hb_font_t* font;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/
} font_hb_stb_t;

static ret_t font_hb_stb_gray_to_mono_by_threshold(const glyph_t* gray, glyph_t* mono,
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

static bool_t font_hb_stb_match(font_t* f, const char* name, font_size_t font_size) {
  (void)font_size;
  return (name == NULL || strcmp(name, f->name) == 0);
}

static font_vmetrics_t font_hb_stb_get_vmetrics(font_t* f, font_size_t font_size) {
  int ascender = 0;
  int descender = 0;
  float scale = 0.0f;
  font_hb_stb_t* font = (font_hb_stb_t*)f;
  const stbtt_fontinfo* sf = &(font->stb_font);
  font_vmetrics_t vmetrics = {font_size, 0, 0, 0, 0, 0};

  return_value_if_fail(sf != NULL, vmetrics);
  scale = stbtt_ScaleForPixelHeight(sf, font_size);
  if (scale == INFINITY) {
    scale = stbtt_ScaleForMappingEmToPixels(sf, font_size);
  }
  vmetrics.ascent = tk_roundi(scale * font->ascent);
  vmetrics.descent = tk_roundi(scale * font->descent);
  vmetrics.line_gap = scale * font->line_gap;

  stbtt_GetFontVMetrics(sf, &ascender, &descender, NULL);
  vmetrics.font_ascender = ascender;
  vmetrics.font_descender = descender;
  vmetrics.units_per_em = ttUSHORT(sf->data + sf->head + 18);

  return vmetrics;
}

static glyph_t* font_hb_stb_get_glyph_impl(font_t* f, wchar_t c, font_size_t font_size,
                                           hb_str_t* hb_str, glyph_t* g) {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int lsb = 0;
  int advance = 0;
  glyph_t* gg = NULL;
  uint8_t* bitmap = NULL;
  hb_info_t* hb_info = NULL;
  font_hb_stb_t* font = (font_hb_stb_t*)f;
  stbtt_fontinfo* sf = &(font->stb_font);

  /* 计算公式：scale = font_size / (ascent - descent) */
  float scale = stbtt_ScaleForPixelHeight(sf, font_size);

  /* 某些字库存在ascent - descent等于0的情况，算出来的scale为无穷大inf，此时采用EM size */
  if (scale == INFINITY) {
    scale = stbtt_ScaleForMappingEmToPixels(sf, font_size);
  }

  if (hb_str != NULL && hb_str->hb_info != NULL) {
    hb_info = hb_str->hb_info;
    bitmap = stbtt_GetGlyphBitmap(sf, 0, scale, hb_str->cur_gid, &w, &h, &x, &y);
    stbtt_GetGlyphHMetrics(sf, hb_str->cur_gid, &advance, &lsb);
  } else {
    bitmap = stbtt_GetCodepointBitmap(sf, 0, scale, c, &w, &h, &x, &y);
    stbtt_GetCodepointHMetrics(sf, c, &advance, &lsb);
  }

  g->x = x;
  g->y = y;
  g->w = w;
  g->h = h;
  g->format = GLYPH_FMT_ALPHA;
  g->advance = tk_roundi(advance * scale);
  g->data = NULL;
  if (hb_info != NULL) {
    g->glyph_index = hb_str->cur_gid;
  } else {
    g->glyph_index = stbtt_FindGlyphIndex(sf, c);
  }

  if (hb_str != NULL && hb_info != NULL) {
    g->x += (hb_info[hb_str->cur_index].x_offset * scale);
    g->y -= (hb_info[hb_str->cur_index].y_offset * scale);
    g->bidi_type = hb_info[hb_str->cur_index].bidi_type;
    advance = hb_info[hb_str->cur_index].x_advance;
    g->advance = tk_roundi(advance * scale);
  }

  if (bitmap != NULL) {
    if (font->mono) {
      glyph_t* gray_g = glyph_clone(g);
      gray_g->data = bitmap;
      uint32_t threshold =
          font_size > 24
              ? (font_size > 48 ? (font_size > 71 ? (font_size > 95 ? 195 : 175) : 160) : 118)
              : 95;
      font_hb_stb_gray_to_mono_by_threshold(gray_g, g, threshold);
      STBTT_free(bitmap, NULL);
      glyph_destroy(gray_g);
    } else {
      g->data = bitmap;
    }
    return_value_if_fail(g->data != NULL, NULL);

    gg = glyph_clone(g);
    if (gg != NULL) {
      if (hb_str != NULL) {
        if (glyph_cache_add(&(font->hb_cache), hb_str->cur_gid, font_size, gg) != RET_OK) {
          TKMEM_FREE(gg);
          gg = NULL;
        }
      } else {
        if (glyph_cache_add(&(font->cache), c, font_size, gg) != RET_OK) {
          TKMEM_FREE(gg);
          gg = NULL;
        }
      }
    }
    if (gg == NULL) {
      STBTT_free(g->data, NULL);
      log_warn("out of memory\n");
      g->data = NULL;
    }
  }

  return gg;
}

static ret_t font_hb_stb_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  font_hb_stb_t* font = (font_hb_stb_t*)f;
  if (glyph_cache_lookup(&(font->cache), c, font_size, g) == RET_OK) {
    return RET_OK;
  }
  return font_hb_stb_get_glyph_impl(f, c, font_size, NULL, g) != NULL || c == ' ' ? RET_OK
                                                                                  : RET_NOT_FOUND;
}

static glyphs_t* font_hb_stb_create_glyphs(font_t* f, const wchar_t* str, uint32_t len,
                                           font_size_t font_size, font_raster_params_t* params) {
  bidi_t b;
  wchar_t c = 0;
  uint32_t i = 0;
  hb_str_t hb_str = {0};
  glyphs_t* glyphs = NULL;
  uint32_t glyph_count = 0;
  hb_info_t* hb_info = NULL;
  hb_direction_t* direction = NULL;
  hb_codepoint_t* codepoint = NULL;
  font_hb_stb_t* font = (font_hb_stb_t*)f;
  font_vmetrics_t vmetrics = font_get_vmetrics(f, font_size);

  if (params->shaping) {
#ifdef WITH_HARFBUZZ_DATA_TEXT_SHAPING
    hb_str_info_t* str_info =
        harfbuzz_data_loader_find_info(harfbuzz_data_loader(), f->name, str, len);
    if (str_info != NULL) {
      glyphs = glyphs_create((wchar_t*)str_info->str, str_info->hb_info_size, len, TRUE, font_size,
                             &vmetrics);
      return_value_if_fail(glyphs != NULL, NULL);
      glyphs->font = f;
      hb_info = str_info->hb_info;
      hb_str.hb_info = str_info->hb_info;
      glyph_count = str_info->hb_info_size;
#elif defined(WITH_HARFBUZZ_TEXT_SHAPING)
    // 尝试调用harfbuzz获取整形数据
    hb_info = harfbuzz_shape_text(str, len, f->name, &glyph_count, font->font);
    if (hb_info != NULL && glyph_count > 0) {
      glyphs = glyphs_create((wchar_t*)str, glyph_count, len, TRUE, font_size, &vmetrics);
      if (glyphs == NULL) {
        harfbuzz_shape_text_destroy(hb_info);
        return NULL;
      }
      glyphs->font = f;
      hb_str.hb_info = hb_info;
#endif /* WITH_HARFBUZZ_DATA_TEXT_SHAPING */
      for (i = 0; i < glyph_count; ++i) {
        // 获取每个字模的数据，并赋值到glyphs->glyphs
        hb_str.cur_index = i;
        hb_str.cur_gid = hb_info[i].glyphid;
        if (glyph_cache_lookup(&(font->hb_cache), hb_info[i].glyphid, font_size,
                               &glyphs->glyphs[i]) != RET_OK) {
          if (font_hb_stb_get_glyph_impl(f, str[i], font_size, &hb_str, &glyphs->glyphs[i]) ==
              NULL) {
            glyphs->glyphs[i].w = 0;
            glyphs->glyphs[i].h = 0;
          }
        }
      }
    }
  }
  if (!params->shaping || hb_info == NULL) {
    // 没有整形数据，用回bidi
    bidi_init(&b, FALSE, FALSE, params->bidi_type);
    if (bidi_log2vis(&b, str, len) == RET_OK) {
      glyphs = glyphs_create(b.vis_str, b.vis_str_size, len, FALSE, font_size, &vmetrics);
      goto_error_if_fail(glyphs != NULL);
      glyphs->font = f;
      for (i = 0; i < b.vis_str_size; i++) {
        c = b.vis_str[i];
        if (glyph_cache_lookup(&(font->cache), c, font_size, &glyphs->glyphs[i]) != RET_OK) {
          if (font_hb_stb_get_glyph_impl(f, c, font_size, NULL, &glyphs->glyphs[i]) == NULL) {
            glyphs->glyphs[i].chr = c;
            glyphs->glyphs[i].w = 0;
            glyphs->glyphs[i].h = 0;
          }
        }
      }
    }
  }

  if (params->shaping && hb_info != NULL) {
    hb_set_hb_glyphs_chr(glyphs, hb_info, glyph_count);
    hb_set_glyphs_maps(glyphs, hb_info, glyph_count);
#if defined(WITH_HARFBUZZ_TEXT_SHAPING)
    harfbuzz_shape_text_destroy(hb_info);
#endif
  } else {
    hb_set_normal_glyphs_chr(glyphs, &b);
    bidi_deinit(&b);
  }

  return glyphs;
error:
  bidi_deinit(&b);
  return glyphs;
}

static ret_t font_hb_stb_shrink_cache(font_t* f, uint32_t cache_nr) {
  font_hb_stb_t* font = (font_hb_stb_t*)f;

  glyph_cache_shrink(&(font->hb_cache), cache_nr);
  return glyph_cache_shrink(&(font->cache), cache_nr);
}

static ret_t font_hb_stb_destroy(font_t* f) {
  font_hb_stb_t* font = (font_hb_stb_t*)f;

#ifdef WITH_HARFBUZZ_DATA_TEXT_SHAPING
  harfbuzz_data_loader_t* hb_loader = NULL;
  hb_loader = harfbuzz_data_loader();
  if (hb_loader != NULL) {
    if (hb_loader->refcount > 1) {
      harfbuzz_data_loader_unref(hb_loader);
    } else if (hb_loader->refcount == 1) {
      harfbuzz_data_loader_unref(hb_loader);
      harfbuzz_data_loader_set(NULL);
      hb_loader = NULL;
    }
  }
  log_debug("harfbuzz loader unref\n");
#endif /* WITH_HARFBUZZ_DATA_TEXT_SHAPING */

  glyph_cache_deinit(&(font->cache));
  glyph_cache_deinit(&(font->hb_cache));
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_font_destroy(font->font);
  hb_face_destroy(font->face);
  hb_blob_destroy(font->blob);
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/
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

static font_t* font_hb_stb_create_ex(const char* name, const uint8_t* buff, uint32_t buff_size,
                                     bool_t mono) {
  font_hb_stb_t* f = NULL;
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob_t* hb_blob = NULL;
  hb_face_t* hb_face = NULL;
  hb_font_t* hb_font = NULL;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_hb_stb_t);
  return_value_if_fail(f != NULL, NULL);

  f->mono = mono;
  f->base.match = font_hb_stb_match;
  f->base.destroy = font_hb_stb_destroy;
  f->base.get_glyph = font_hb_stb_get_glyph;
  f->base.create_glyphs = font_hb_stb_create_glyphs;
  f->base.get_vmetrics = font_hb_stb_get_vmetrics;
  f->base.shrink_cache = font_hb_stb_shrink_cache;
  f->base.desc = mono ? "mono(stb)" : "truetype(stb)";

  tk_strncpy(f->base.name, name, MAX_PATH);

  glyph_cache_init(&(f->cache), TK_GLYPH_CACHE_NR, destroy_glyph);
  glyph_cache_init(&(f->hb_cache), TK_GLYPH_CACHE_NR, destroy_glyph);
  stbtt_InitFont(&(f->stb_font), buff, stbtt_GetFontOffsetForIndex(buff, 0));
  stbtt_GetFontVMetrics(&(f->stb_font), &(f->ascent), &(f->descent), &(f->line_gap));
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob = hb_blob_create((const char*)buff, buff_size, HB_MEMORY_MODE_READONLY, NULL, NULL);
  hb_face = hb_face_create(hb_blob, 0);
  hb_font = hb_font_create(hb_face);
  f->blob = hb_blob;
  f->face = hb_face;
  f->font = hb_font;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/

  if (f->ascent == 0 && f->descent == 0) {
    float scale = stbtt_ScaleForMappingEmToPixels(&(f->stb_font), 18);
    f->ascent = (int)(18 / scale);
    f->descent = 0;
    f->line_gap = 0;
  }

  return &(f->base);
}

font_t* font_hb_stb_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return font_hb_stb_create_ex(name, buff, size, TRUE);
}

font_t* font_hb_stb_create(const char* name, const uint8_t* buff, uint32_t size) {
#ifdef WITH_LCD_MONO
  return font_hb_stb_create_ex(name, buff, size, TRUE);
#else
  return font_hb_stb_create_ex(name, buff, size, FALSE);
#endif
}

static font_t* font_hb_stb_load(font_loader_t* loader, const char* name, const uint8_t* buff,
                                uint32_t buff_size) {
  (void)loader;
#ifdef WITH_HARFBUZZ_DATA_TEXT_SHAPING
  harfbuzz_data_loader_t* hb_loader = NULL;
  hb_loader = harfbuzz_data_loader_ref(harfbuzz_data_loader(), NULL);
  if (hb_loader != NULL) {
    if (harfbuzz_data_loader() == NULL) {
      harfbuzz_data_loader_set(hb_loader);
      log_debug("harfbuzz loader first ref\n");
    }
    log_debug("harfbuzz loader ref\n");
  }
#endif /* WITH_HARFBUZZ_DATA_TEXT_SHAPING */

  return font_hb_stb_create(name, buff, buff_size);
}

font_loader_t* font_loader_hb_stb(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_hb_stb_load;

  return &loader;
}

static font_t* font_hb_stb_load_mono(font_loader_t* loader, const char* name, const uint8_t* buff,
                                     uint32_t buff_size) {
  (void)loader;

  return font_hb_stb_create_ex(name, buff, buff_size, TRUE);
}

font_loader_t* font_loader_hb_stb_mono(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_hb_stb_load_mono;

  return &loader;
}
#else
font_loader_t* font_loader_hb_stb(void) {
  return NULL;
}

font_loader_t* font_loader_hb_stb_mono(void) {
  return NULL;
}

font_t* font_hb_stb_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}

font_t* font_hb_stb_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}
#endif /* WITH_STB_FONT && (WITH_HARFBUZZ_DATA_TEXT_SHAPING || WITH_HARFBUZZ_TEXT_SHAPING) */
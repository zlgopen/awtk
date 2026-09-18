/**
 * File:   font_loader_hb_ft.c
 * Author: AWTK Develop Team
 * Brief:  freetype font loader
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-03-10 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "base/bidi.h"
#include "tkc/utils.h"
#include "base/types_def.h"
#include "base/glyph_cache.h"
#include "harfbuzz/harfbuzz_tools.h"

#if defined(WITH_FT_FONT) && \
    (defined(WITH_HARFBUZZ_DATA_TEXT_SHAPING) || defined(WITH_HARFBUZZ_TEXT_SHAPING))
#include "font_loader/font_loader_harfbuzz_freetype.h"
#include "3rd/harfbuzz/hb.h"

#ifdef WITH_HARFBUZZ_TEXT_SHAPING
#include "harfbuzz/harfbuzz_loader.h"
#elif defined(WITH_HARFBUZZ_DATA_TEXT_SHAPING)
#include "harfbuzz/harfbuzz_data_loader.h"
#endif /* WITH_HARFBUZZ_DATA_TEXT_SHAPING */

#ifdef USE_SYSTEM_FREETYPE
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftpfr.h>
#include <freetype/ftadvanc.h>
#else
#ifdef WIN32
#undef interface
#endif /*WIN32*/
/*3rd/freetype_single_header/freetype.h*/
#include "freetype_single_header/freetype.h"
#endif /*USE_SYSTEM_FREETYPE*/

typedef struct _hb_str_t {
  uint32_t cur_gid;
  uint32_t cur_index;
  hb_info_t* hb_info;
} hb_str_t;

typedef struct {
  FT_Face face;
  FT_Library library;
} ft_fontinfo;

typedef struct _font_ft_t {
  font_t base;
  ft_fontinfo ft_font;
  glyph_cache_t hb_cache;
  glyph_cache_t cache;
  bool_t mono;
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob_t* blob;
  hb_face_t* face;
  hb_font_t* font;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/
} font_ft_t;

typedef struct _glyph_ft_t {
  glyph_t glyph;
  void* handle;
} glyph_ft_t;

static glyph_ft_t* glyph_ft_create(void) {
  return TKMEM_ZALLOC(glyph_ft_t);
}

static ret_t glyph_ft_destory(glyph_ft_t* g) {
  return_value_if_fail(g != NULL, RET_BAD_PARAMS);

  if (g->handle != NULL) {
    FT_Done_Glyph((FT_Glyph)g->handle);
  }

  TKMEM_FREE(g);

  return RET_OK;
}

static bool_t font_hb_ft_match(font_t* f, const char* name, font_size_t font_size) {
  (void)font_size;
  return (name == NULL || strcmp(name, f->name) == 0);
}

static ret_t font_hb_ft_get_glyph_impl(font_t* f, wchar_t c, font_size_t font_size,
                                       hb_str_t* hb_str, glyph_t* g) {
  FT_Glyph glyph;
  FT_GlyphSlot glyf;
  ret_t ret = RET_OK;
  float_t scale = 0.0f;
  float_t advance = 0.0f;
  hb_info_t* hb_info = NULL;
  FT_Error ft_ret = FT_Err_Ok;
  font_ft_t* font = (font_ft_t*)f;
  ft_fontinfo* sf = &(font->ft_font);
  uint32_t flags = FT_LOAD_DEFAULT | FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT;

  if (font->mono) {
    flags |= FT_LOAD_TARGET_MONO;
  }

  FT_Set_Char_Size(sf->face, 0, font_size * 64, 0, 50);
  if (hb_str != NULL && hb_str->hb_info != NULL) {
    hb_info = hb_str->hb_info;
    ft_ret = FT_Load_Glyph(sf->face, hb_str->cur_gid, flags);
  } else {
    ft_ret = FT_Load_Char(sf->face, c, flags);
  }

  if (!ft_ret) {
    glyf = sf->face->glyph;
    FT_Get_Glyph(glyf, &glyph);

    g->format = GLYPH_FMT_ALPHA;
    g->h = glyf->bitmap.rows;
    g->w = glyf->bitmap.width;
    g->pitch = glyf->bitmap.pitch;
    g->x = glyf->bitmap_left;
    g->y = -glyf->bitmap_top;
    g->data = glyf->bitmap.buffer;
    g->advance = glyf->metrics.horiAdvance / 64;

    if (hb_info != NULL) {
      scale = ((float_t)font_size / (sf->face->ascender - sf->face->descender));
      g->x += (int16_t)(hb_info[hb_str->cur_index].x_offset * scale);
      g->y -= (int16_t)(hb_info[hb_str->cur_index].y_offset * scale);
      g->bidi_type = hb_info[hb_str->cur_index].bidi_type;
      advance = (hb_info[hb_str->cur_index].x_advance * scale);
      g->advance = (int16_t)tk_roundi(advance);
      g->glyph_index = hb_str->cur_gid;
    } else {
      g->glyph_index = FT_Get_Char_Index(sf->face, c);
    }

    if (g->data != NULL) {
      glyph_ft_t* g_ft = glyph_ft_create();
      if (g_ft != NULL) {
        if (glyf->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
          g->format = GLYPH_FMT_MONO;
        }

        g_ft->glyph = *g;
        g_ft->handle = glyph;
        if (hb_info != NULL) {
          ret = glyph_cache_add(&(font->hb_cache), hb_str->cur_gid, font_size, (glyph_t*)(g_ft));
        } else {
          ret = glyph_cache_add(&(font->cache), c, font_size, (glyph_t*)(g_ft));
        }
        if (ret != RET_OK) {
          TKMEM_FREE(g_ft);
          g_ft = NULL;
        }
      }
      if (g_ft == NULL) {
        FT_Done_Glyph(glyph);
        log_warn("out of memory\n");
        g->data = NULL;
      }
    } else {
      FT_Done_Glyph(glyph);
    }
  }

  return g->data != NULL ? RET_OK : RET_NOT_FOUND;
}

static ret_t font_hb_ft_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  font_ft_t* font = (font_ft_t*)f;
  if (glyph_cache_lookup(&(font->cache), c, font_size, g) == RET_OK) {
    return RET_OK;
  }
  return font_hb_ft_get_glyph_impl(f, c, font_size, NULL, g);
}

static glyphs_t* font_hb_ft_create_glyphs(font_t* f, const wchar_t* str, uint32_t len,
                                          font_size_t font_size, font_raster_params_t* params) {
  bidi_t b;
  wchar_t c = 0;
  uint32_t i = 0;
  hb_str_t hb_str = {0};
  glyphs_t* glyphs = NULL;
  uint32_t glyph_count = 0;
  hb_info_t* hb_info = NULL;
  ret_t has_glyph_cache = RET_FAIL;
  font_ft_t* font = (font_ft_t*)f;
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
    hb_info = harfbuzz_shape_text(str, len, f->name, &glyph_count, font->font);
    // 构造glyphs_t对象
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
        hb_str.cur_index = i;
        hb_str.cur_gid = hb_info[i].glyphid;
        if (glyph_cache_lookup(&(font->hb_cache), hb_info[i].glyphid, font_size,
                               &glyphs->glyphs[i]) != RET_OK) {
          if (font_hb_ft_get_glyph_impl(f, str[i], font_size, &hb_str, &glyphs->glyphs[i]) !=
              RET_OK) {
            glyphs->glyphs[i].w = 0;
            glyphs->glyphs[i].h = 0;
          }
        }
      }
    }
  }
  if (!params->shaping || hb_info == NULL) {
    bidi_init(&b, FALSE, FALSE, params->bidi_type);
    if (bidi_log2vis(&b, str, len) == RET_OK) {
      glyphs = glyphs_create(b.vis_str, b.vis_str_size, len, FALSE, font_size, &vmetrics);
      goto_error_if_fail(glyphs != NULL);
      glyphs->font = f;
      for (i = 0; i < b.vis_str_size; ++i) {
        c = b.vis_str[i];
        if (glyph_cache_lookup(&(font->cache), c, font_size, &glyphs->glyphs[i]) != RET_OK) {
          if (font_hb_ft_get_glyph_impl(f, c, font_size, NULL, &glyphs->glyphs[i]) != RET_OK) {
            glyphs->glyphs[i].chr = (uint16_t)c;
            glyphs->glyphs[i].w = 0;
            glyphs->glyphs[i].h = 0;
          }
        }
      }
    }
  }

  // 设置maps表和字模的chr
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

static font_vmetrics_t font_hb_ft_get_vmetrics(font_t* f, font_size_t font_size) {
  int32_t height = 0;
  font_vmetrics_t vmetrics;
  font_ft_t* font = (font_ft_t*)f;
  ft_fontinfo* sf = &(font->ft_font);

  FT_Set_Char_Size(sf->face, 0, font_size * 64, 0, 50);

  height = FT_MulFix(sf->face->height, sf->face->size->metrics.y_scale);
  vmetrics.ascent = FT_MulFix(sf->face->ascender, sf->face->size->metrics.y_scale);
  vmetrics.descent = FT_MulFix(sf->face->descender, sf->face->size->metrics.y_scale);
  vmetrics.line_gap = height - (vmetrics.ascent - vmetrics.descent);

  vmetrics.font_ascender = sf->face->ascender;
  vmetrics.font_descender = sf->face->descender;
  vmetrics.units_per_em = sf->face->units_per_EM;

  vmetrics.ascent = vmetrics.ascent >> 6;
  vmetrics.descent = vmetrics.descent >> 6;
  vmetrics.line_gap = vmetrics.line_gap >> 6;

  return vmetrics;
}

static ret_t font_hb_ft_shrink_cache(font_t* f, uint32_t cache_nr) {
  font_ft_t* font = (font_ft_t*)f;

  glyph_cache_shrink(&(font->hb_cache), cache_nr);
  return glyph_cache_shrink(&(font->cache), cache_nr);
}

static ret_t font_hb_ft_destroy(font_t* f) {
  font_ft_t* font = (font_ft_t*)f;

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
  FT_Done_FreeType(font->ft_font.library);

  TKMEM_FREE(f);

  return RET_OK;
}

static ret_t destroy_glyph(void* data) {
  return glyph_ft_destory((glyph_ft_t*)(data));
}

static font_t* font_hb_ft_create_ex(const char* name, const uint8_t* buff, uint32_t size,
                                    bool_t mono) {
  font_ft_t* f = NULL;
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob_t* hb_blob = NULL;
  hb_face_t* hb_face = NULL;
  hb_font_t* hb_font = NULL;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_ft_t);
  return_value_if_fail(f != NULL, NULL);

  f->mono = mono;
  if (FT_Init_FreeType(&f->ft_font.library)) {
    TKMEM_FREE(f);
    return NULL;
  }

  if (FT_New_Memory_Face(f->ft_font.library, buff, size, 0, &f->ft_font.face)) {
    FT_Done_FreeType(f->ft_font.library);
    TKMEM_FREE(f);
    return NULL;
  }

  if (FT_Select_Charmap(f->ft_font.face, FT_ENCODING_UNICODE)) {
    FT_Select_Charmap(f->ft_font.face, FT_ENCODING_ADOBE_LATIN_1);
  }

  f->base.match = font_hb_ft_match;
  f->base.destroy = font_hb_ft_destroy;
  f->base.get_glyph = font_hb_ft_get_glyph;
  f->base.create_glyphs = font_hb_ft_create_glyphs;
  f->base.get_vmetrics = font_hb_ft_get_vmetrics;
  f->base.shrink_cache = font_hb_ft_shrink_cache;
  f->base.desc = mono ? "mono(freetype)" : "truetype(freetype)";
#ifdef WITH_HARFBUZZ_TEXT_SHAPING
  hb_blob = hb_blob_create((const char*)buff, size, HB_MEMORY_MODE_READONLY, NULL, NULL);
  hb_face = hb_face_create(hb_blob, 0);
  hb_font = hb_font_create(hb_face);
  f->blob = hb_blob;
  f->face = hb_face;
  f->font = hb_font;
#endif /*WITH_HARFBUZZ_TEXT_SHAPING*/

  tk_strncpy(f->base.name, name, MAX_PATH);

  glyph_cache_init(&(f->cache), TK_GLYPH_CACHE_NR, destroy_glyph);
  glyph_cache_init(&(f->hb_cache), TK_GLYPH_CACHE_NR, destroy_glyph);

  return &(f->base);
}

font_t* font_hb_ft_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return font_hb_ft_create_ex(name, buff, size, TRUE);
}

font_t* font_hb_ft_create(const char* name, const uint8_t* buff, uint32_t size) {
#ifdef WITH_LCD_MONO
  return font_hb_ft_create_ex(name, buff, size, TRUE);
#else
  return font_hb_ft_create_ex(name, buff, size, FALSE);
#endif
}

static font_t* font_hb_ft_load(font_loader_t* loader, const char* name, const uint8_t* buff,
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

  return font_hb_ft_create(name, buff, buff_size);
}

font_loader_t* font_loader_hb_ft(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_hb_ft_load;

  return &loader;
}

static font_t* font_hb_ft_load_mono(font_loader_t* loader, const char* name, const uint8_t* buff,
                                    uint32_t buff_size) {
  (void)loader;

  return font_hb_ft_create_ex(name, buff, buff_size, TRUE);
}

font_loader_t* font_loader_hb_ft_mono(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_TTF;
  loader.load = font_hb_ft_load_mono;

  return &loader;
}
#else
font_loader_t* font_loader_hb_ft(void) {
  return NULL;
}

font_t* font_hb_ft_mono_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}

font_t* font_hb_ft_create(const char* name, const uint8_t* buff, uint32_t size) {
  return NULL;
}
#endif /*WITH_FT_FONT && (WITH_HARFBUZZ_DATA_TEXT_SHAPING || WITH_HARFBUZZ_TEXT_SHAPING)*/
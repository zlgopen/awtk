/**
 * File:   font_gen_tools.c
 * Author: AWTK Develop Team
 * Brief:  font_gen 专用的字体创建入口
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 */

/**
 * History:
 * ================================================================
 * 2026-08-04 Yang Zewu <yangzewu@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/font.h"
#include "font_gen_tools.h"

#ifdef WITH_STB_FONT
#include "font_loader/font_loader_stb.h"
#elif defined(WITH_FT_FONT)
#include "font_loader/font_loader_ft.h"
#endif

#if !defined(WITH_STB_FONT) && !defined(WITH_FT_FONT)

#ifdef WIN32
#undef interface
#endif /*WIN32*/

#include "freetype_single_header/freetype.h"

typedef struct {
  FT_Face face;
  FT_Library library;
} ft_fontinfo;

typedef struct _font_gen_ft_t {
  font_t base;
  ft_fontinfo ft_font;
  bool_t mono;
} font_gen_ft_t;

static bool_t font_gen_ft_match(font_t* f, const char* name, font_size_t font_size) {
  (void)font_size;
  return (name == NULL || strcmp(name, f->name) == 0);
}

static ret_t font_gen_ft_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  FT_GlyphSlot glyf = NULL;
  uint32_t flags = FT_LOAD_DEFAULT | FT_LOAD_RENDER | FT_LOAD_NO_AUTOHINT;
  font_gen_ft_t* font = (font_gen_ft_t*)f;
  ft_fontinfo* sf = &(font->ft_font);

  if (font->mono) {
    flags |= FT_LOAD_TARGET_MONO;
  }

  FT_Set_Char_Size(sf->face, 0, font_size * 64, 0, 50);
  if (FT_Load_Char(sf->face, c, flags)) {
    return RET_NOT_FOUND;
  }

  glyf = sf->face->glyph;
  memset(g, 0, sizeof(glyph_t));
  g->format = GLYPH_FMT_ALPHA;
  g->h = glyf->bitmap.rows;
  g->w = glyf->bitmap.width;
  g->pitch = glyf->bitmap.pitch;
  g->x = glyf->bitmap_left;
  g->y = -glyf->bitmap_top;
  g->data = glyf->bitmap.buffer;
  g->advance = glyf->metrics.horiAdvance / 64;
  g->chr = c;
  g->bidi_type = FONT_BIDI_TYPE_LTR;
  g->glyph_index = FT_Get_Char_Index(sf->face, c);

  if (g->data != NULL && font->mono && glyf->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
    g->format = GLYPH_FMT_MONO;
  }

  return g->data != NULL ? RET_OK : RET_NOT_FOUND;
}

static glyphs_t* font_gen_ft_get_glyphs(font_t* f, const wchar_t* str, uint32_t len,
                                        font_size_t font_size, font_raster_params_t* params) {
  (void)f;
  (void)str;
  (void)len;
  (void)font_size;
  (void)params;
  return NULL;
}

/* 镜像 font_loader_ft.c 的 font_ft_get_vmetrics。 */
static font_vmetrics_t font_gen_ft_get_vmetrics(font_t* f, font_size_t font_size) {
  int32_t height = 0;
  font_vmetrics_t vmetrics;
  font_gen_ft_t* font = (font_gen_ft_t*)f;
  ft_fontinfo* sf = &(font->ft_font);

  FT_Set_Char_Size(sf->face, 0, font_size * 64, 0, 50);

  height = FT_MulFix(sf->face->height, sf->face->size->metrics.y_scale);
  vmetrics.ascent = FT_MulFix(sf->face->ascender, sf->face->size->metrics.y_scale);
  vmetrics.descent = FT_MulFix(sf->face->descender, sf->face->size->metrics.y_scale);
  vmetrics.line_gap = height - (vmetrics.ascent - vmetrics.descent);
  vmetrics.font_ascender = sf->face->ascender;
  vmetrics.font_descender = sf->face->descender;
  vmetrics.units_per_em = sf->face->units_per_EM;

  vmetrics.ascent >>= 6;
  vmetrics.descent >>= 6;
  vmetrics.line_gap >>= 6;

  return vmetrics;
}

static ret_t font_gen_ft_shrink_cache(font_t* f, uint32_t cache_nr) {
  (void)f;
  (void)cache_nr;
  return RET_OK;
}

static ret_t font_gen_ft_destroy(font_t* f) {
  font_gen_ft_t* font = (font_gen_ft_t*)f;

  if (font->ft_font.face != NULL) {
    FT_Done_Face(font->ft_font.face);
    font->ft_font.face = NULL;
  }
  if (font->ft_font.library != NULL) {
    FT_Done_FreeType(font->ft_font.library);
    font->ft_font.library = NULL;
  }

  TKMEM_FREE(f);
  return RET_OK;
}

/* 镜像 font_loader_ft.c 的 font_ft_create_ex。 */
static font_t* font_gen_ft_create(const char* name, const uint8_t* buff, uint32_t size,
                                  bool_t mono) {
  font_gen_ft_t* f = NULL;
  return_value_if_fail(buff != NULL && name != NULL, NULL);

  f = TKMEM_ZALLOC(font_gen_ft_t);
  return_value_if_fail(f != NULL, NULL);

  f->mono = mono;
  if (FT_Init_FreeType(&f->ft_font.library)) {
    TKMEM_FREE(f);
    return NULL;
  }

  if (FT_New_Memory_Face(f->ft_font.library, buff, size, 0, &f->ft_font.face)) {
    FT_Done_FreeType(f->ft_font.library);
    f->ft_font.library = NULL;
    TKMEM_FREE(f);
    return NULL;
  }

  if (FT_Select_Charmap(f->ft_font.face, FT_ENCODING_UNICODE)) {
    FT_Select_Charmap(f->ft_font.face, FT_ENCODING_ADOBE_LATIN_1);
  }

  f->base.match = font_gen_ft_match;
  f->base.destroy = font_gen_ft_destroy;
  f->base.get_glyph = font_gen_ft_get_glyph;
  f->base.get_glyphs = font_gen_ft_get_glyphs;
  f->base.get_vmetrics = font_gen_ft_get_vmetrics;
  f->base.shrink_cache = font_gen_ft_shrink_cache;
  f->base.desc = mono ? "mono(freetype)" : "truetype(freetype)";

  tk_strncpy(f->base.name, name, MAX_PATH + 1);

  return &(f->base);
}

#endif /* !WITH_STB_FONT && !WITH_FT_FONT */

font_t* font_gen_create_font(const char* name, const uint8_t* buff, uint32_t size, bool_t mono) {
  /* 优先复用运行时已编译进 base 库的 loader。
   * 只有这些 loader 都未编译进来时，才回退到本模块自带的 FreeType 实现。 */
#ifdef WITH_STB_FONT
  return mono ? font_stb_mono_create(name, buff, size) : font_stb_create(name, buff, size);
#elif defined(WITH_FT_FONT)
  return mono ? font_ft_mono_create(name, buff, size) : font_ft_create(name, buff, size);
#else
  return font_gen_ft_create(name, buff, size, mono);
#endif
}

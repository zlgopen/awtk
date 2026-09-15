/**
 * File:   font_loader_harfbuzz_data_bitmap.c
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
 * 2025-03-10 Zhao Yingjian <zhaoyingjian@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "base/bidi.h"
#include "tkc/utils.h"
#include "harfbuzz/hb_info.h"
#include "harfbuzz/harfbuzz_tools.h"
#include "harfbuzz/harfbuzz_data_loader.h"
#include "font_loader/font_loader_harfbuzz_data_bitmap.h"

typedef struct _hb_str_t {
  const wchar_t* str;
  uint32_t cur_gid;
  uint32_t cur_index;
  hb_info_t* hb_info;
} hb_str_t;

typedef struct _font_harfbuzz_data_bitmap_t {
  font_t base;
  const uint8_t* buff;
  uint32_t buff_size;
} font_harfbuzz_data_bitmap_t;

static font_harfbuzz_data_bitmap_index_t* find_glyph(font_harfbuzz_data_bitmap_index_t* elms,
                                                     uint32_t nr, wchar_t c, bool_t use_gid) {
  int i = 0;
  int low = 0;
  int mid = 0;
  int result = 0;
  int high = nr - 1;

  if (use_gid) {
    // 如果查找gid，需要遍历
    for (i = 0; i < nr; ++i) {
      if (elms[i].gid == c) {
        return elms + i;
      }
    }
  } else {
    while (low <= high) {
      mid = low + ((high - low) >> 1);
      result = elms[mid].c - c;

      if (result == 0) {
        return elms + mid;
      } else if (result < 0) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
  }

  return NULL;
}

static ret_t font_harfbuzz_data_bitmap_get_glyph_impl(font_t* f, wchar_t c, font_size_t font_size,
                                                      hb_str_t* hb_str, glyph_t* g) {
  int16_t x = 0;
  int16_t y = 0;
  int16_t advance = 0;
  float_t scale = 0.0f;
  const uint8_t* p = NULL;
  hb_info_t* hb_info = NULL;
  font_harfbuzz_data_bitmap_index_t* index = NULL;
  font_harfbuzz_data_bitmap_t* font = (font_harfbuzz_data_bitmap_t*)f;
  font_harfbuzz_data_bitmap_header_t* header = (font_harfbuzz_data_bitmap_header_t*)(font->buff);

  if (hb_str != NULL && hb_str->hb_info != NULL) {
    hb_info = hb_str->hb_info;
    index = find_glyph(header->index, header->char_nr, hb_str->cur_gid, TRUE);
  } else {
    index = find_glyph(header->index, header->char_nr, c, FALSE);
  }
  return_value_if_fail(index != NULL, RET_NOT_FOUND);
  return_value_if_fail(header->font_size == font_size, RET_NOT_FOUND);

  p = (font->buff + index->offset);
  memcpy(g, p, sizeof(glyph_t));
  g->next_glyph = NULL;
  if (hb_info != NULL) {
    scale = ((float_t)font_size / (header->font_ascender - header->font_descender));
    x = (int16_t)tk_roundi((hb_info[hb_str->cur_index].x_offset * scale));
    y = (int16_t)tk_roundi((hb_info[hb_str->cur_index].y_offset * scale));
    advance = (int16_t)tk_roundi((hb_info[hb_str->cur_index].x_advance * scale));
    g->bidi_type = hb_info[hb_str->cur_index].bidi_type;
    g->x += x;
    g->y -= y;
    g->advance = advance;
    g->data =
        font->buff + index->offset + sizeof(glyph_t) - sizeof(g->data) - sizeof(g->next_glyph);
  } else {
    if (c == ' ') {
      g->data = NULL;
    } else {
      g->data =
          font->buff + index->offset + sizeof(glyph_t) - sizeof(g->data) - sizeof(g->next_glyph);
    }
  }

  return RET_OK;
}

static ret_t font_harfbuzz_data_bitmap_get_glyph(font_t* f, wchar_t c, font_size_t font_size,
                                                 glyph_t* g) {
  return font_harfbuzz_data_bitmap_get_glyph_impl(f, c, font_size, NULL, g);
}

static glyphs_t* font_harfbuzz_data_bitmap_create_glyphs(font_t* f, const wchar_t* str,
                                                         uint32_t len, font_size_t font_size,
                                                         font_raster_params_t* params) {
  bidi_t b;
  uint32_t i = 0;
  ret_t ret = RET_OK;
  hb_str_t hb_str = {0};
  glyphs_t* glyphs = NULL;
  font_vmetrics_t vmetrics;
  hb_info_t* hb_info = NULL;
  hb_str_info_t* str_info = NULL;
  char font_size_char[8] = {0};
  char real_font_name[TK_NAME_LEN] = {0};
  font_harfbuzz_data_bitmap_t* font = (font_harfbuzz_data_bitmap_t*)f;
  harfbuzz_data_loader_t* harfbuzz_manager = harfbuzz_data_loader();
  font_harfbuzz_data_bitmap_header_t* header = (font_harfbuzz_data_bitmap_header_t*)(font->buff);
  memset(&vmetrics, 0x0, sizeof(font_vmetrics_t));
  vmetrics.ascent = header->ascent;
  vmetrics.descent = header->descent;
  vmetrics.line_gap = header->line_gap;
  tk_itoa(font_size_char, sizeof(font_size_char), font_size);
  tk_strcpy(real_font_name, f->name);
  if (tk_strrstr(real_font_name, font_size_char) - real_font_name - 1 > 0) {
    real_font_name[tk_strrstr(real_font_name, font_size_char) - real_font_name - 1] = 0;
  }
  str_info = harfbuzz_data_loader_find_info(harfbuzz_manager, f->name, str, len);

  if (str_info != NULL) {
    glyphs = glyphs_create((wchar_t*)str_info->str, str_info->hb_info_size, len, TRUE, font_size,
                           &vmetrics);
    return_value_if_fail(glyphs != NULL, NULL);
    glyphs->font = f;
    hb_str.str = str;
    hb_info = str_info->hb_info;
    hb_str.hb_info = hb_info;
    for (i = 0; i < str_info->hb_info_size; ++i) {
      hb_str.cur_index = i;
      hb_str.cur_gid = hb_str.hb_info[i].glyphid;
      wchar_t c = str[i];
      ret = font_harfbuzz_data_bitmap_get_glyph_impl(f, c, font_size, &hb_str, &glyphs->glyphs[i]);
      if (ret != RET_OK && glyphs->glyphs[i].data == NULL) {
        glyphs->glyphs[i].chr = c;
        glyphs->glyphs[i].w = 0;
        glyphs->glyphs[i].h = 0;
      }
    }
  } else {
    bidi_init(&b, FALSE, FALSE, params->bidi_type);
    if (bidi_log2vis(&b, str, len) == RET_OK) {
      glyphs = glyphs_create(b.vis_str, b.vis_str_size, len, FALSE, font_size, &vmetrics);
      goto_error_if_fail(glyphs != NULL);
      glyphs->font = f;
      for (i = 0; i < b.vis_str_size; i++) {
        wchar_t c = b.vis_str[i];
        if (font_harfbuzz_data_bitmap_get_glyph_impl(f, c, font_size, NULL, &glyphs->glyphs[i]) !=
                RET_OK &&
            glyphs->glyphs[i].data == NULL) {
          glyphs->glyphs[i].chr = c;
          glyphs->glyphs[i].w = 0;
          glyphs->glyphs[i].h = 0;
        }
      }
    }
  }

  // 设置maps表和字模的chr
  if (hb_info != NULL) {
    hb_set_hb_glyphs_chr(glyphs, hb_info, str_info->hb_info_size);
    hb_set_glyphs_maps(glyphs, hb_info, str_info->hb_info_size);
  } else {
    hb_set_normal_glyphs_chr(glyphs, &b);
    bidi_deinit(&b);
  }
  return glyphs;
error:
  bidi_deinit(&b);
  return glyphs;
}

static bool_t font_harfbuzz_data_bitmap_match(font_t* f, const char* name, font_size_t font_size) {
  font_harfbuzz_data_bitmap_t* font = (font_harfbuzz_data_bitmap_t*)f;
  if (name == NULL || strcmp(name, font->base.name) == 0) {
    return TRUE;
  }

  return FALSE;
}

static ret_t font_harfbuzz_data_bitmap_destroy(font_t* f) {
  harfbuzz_data_loader_t* hb_loader = NULL;
  TKMEM_FREE(f);
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
  return RET_OK;
}

static font_vmetrics_t font_harfbuzz_data_bitmap_get_vmetrics(font_t* f, font_size_t font_size) {
  font_vmetrics_t vmetrics;
  font_harfbuzz_data_bitmap_t* font = (font_harfbuzz_data_bitmap_t*)f;
  font_harfbuzz_data_bitmap_header_t* header = (font_harfbuzz_data_bitmap_header_t*)(font->buff);

  vmetrics.ascent = header->ascent;
  vmetrics.descent = header->descent;
  vmetrics.line_gap = header->line_gap;

  vmetrics.units_per_em = 0;
  vmetrics.font_ascender = header->font_ascender;
  vmetrics.font_descender = header->font_descender;

  return vmetrics;
}

font_t* font_harfbuzz_data_bitmap_init(font_harfbuzz_data_bitmap_t* f, const char* name,
                                       const uint8_t* buff, uint32_t buff_size) {
  return_value_if_fail(f != NULL && buff != NULL, NULL);

  f->buff = buff;
  f->buff_size = buff_size;
  f->base.match = font_harfbuzz_data_bitmap_match;
  f->base.get_vmetrics = font_harfbuzz_data_bitmap_get_vmetrics;
  f->base.get_glyph = font_harfbuzz_data_bitmap_get_glyph;
  f->base.create_glyphs = font_harfbuzz_data_bitmap_create_glyphs;
  f->base.destroy = font_harfbuzz_data_bitmap_destroy;
  f->base.desc = "bitmap with harfbuzz font";
  tk_strncpy(f->base.name, name, MAX_PATH);

  return &(f->base);
}

font_t* font_harfbuzz_data_bitmap_create(const char* name, const uint8_t* buff,
                                         uint32_t buff_size) {
  font_harfbuzz_data_bitmap_t* font = NULL;
  return_value_if_fail(buff != NULL && name != NULL && buff_size > 0, NULL);

  font = TKMEM_ZALLOC(font_harfbuzz_data_bitmap_t);
  return_value_if_fail(font != NULL, NULL);

  return font_harfbuzz_data_bitmap_init(font, name, buff, buff_size);
}

static font_t* font_harfbuzz_data_bitmap_load(font_loader_t* loader, const char* name,
                                              const uint8_t* buff, uint32_t buff_size) {
  harfbuzz_data_loader_t* hb_loader = NULL;
  (void)loader;
  hb_loader = harfbuzz_data_loader_ref(harfbuzz_data_loader(), NULL);
  if (hb_loader != NULL) {
    if (harfbuzz_data_loader() == NULL) {
      harfbuzz_data_loader_set(hb_loader);
      log_debug("harfbuzz loader first ref\n");
    }
    log_debug("harfbuzz loader ref\n");
  }

  return font_harfbuzz_data_bitmap_create(name, buff, buff_size);
}

font_loader_t* font_loader_harfbuzz_data_bitmap(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_BMP;
  loader.load = font_harfbuzz_data_bitmap_load;

  return &loader;
}

/**
 * File:   font.h
 * Author: AWTK Develop Team
 * Brief:  font interface
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "font_loader/font_loader_bitmap.h"

typedef struct _font_bitmap_t {
  font_t base;
  const uint8_t* buff;
  uint32_t buff_size;
} font_bitmap_t;

static font_bitmap_index_t* find_glyph(font_bitmap_index_t* elms, uint32_t nr, wchar_t c) {
  int low = 0;
  int mid = 0;
  int result = 0;
  int high = nr - 1;

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

  return NULL;
}

static ret_t font_bitmap_get_glyph(font_t* f, wchar_t c, font_size_t font_size, glyph_t* g) {
  const uint8_t* p = NULL;
  font_bitmap_t* font = (font_bitmap_t*)f;
  font_bitmap_header_t* header = (font_bitmap_header_t*)(font->buff);
  font_bitmap_index_t* index = find_glyph(header->index, header->char_nr, c);
  return_value_if_fail(index != NULL, RET_NOT_FOUND);
  return_value_if_fail(header->font_size == font_size, RET_NOT_FOUND);

  p = (font->buff + index->offset);
  memcpy(g, p, sizeof(glyph_t));
  if (c == ' ') {
    g->data = NULL;
  } else {
    g->data = font->buff + index->offset + sizeof(glyph_t) - sizeof(g->data);
  }

  return RET_OK;
}

static bool_t font_bitmap_match(font_t* f, const char* name, font_size_t font_size) {
  font_bitmap_t* font = (font_bitmap_t*)f;
  if (name == NULL || strcmp(name, font->base.name) == 0) {
    return TRUE;
  }

  return FALSE;
}

static ret_t font_bitmap_destroy(font_t* f) {
  TKMEM_FREE(f);
  return RET_OK;
}

static font_vmetrics_t font_bitmap_get_vmetrics(font_t* f, font_size_t font_size) {
  font_vmetrics_t vmetrics;
  font_bitmap_t* font = (font_bitmap_t*)f;
  font_bitmap_header_t* header = (font_bitmap_header_t*)(font->buff);

  vmetrics.ascent = header->ascent;
  vmetrics.descent = header->descent;
  vmetrics.line_gap = header->line_gap;

  return vmetrics;
}

font_t* font_bitmap_init(font_bitmap_t* f, const char* name, const uint8_t* buff,
                         uint32_t buff_size) {
  return_value_if_fail(f != NULL && buff != NULL, NULL);

  f->buff = buff;
  f->buff_size = buff_size;
  f->base.match = font_bitmap_match;
  f->base.get_vmetrics = font_bitmap_get_vmetrics;
  f->base.get_glyph = font_bitmap_get_glyph;
  f->base.destroy = font_bitmap_destroy;
  f->base.desc = "bitmap font";
  tk_strncpy(f->base.name, name, TK_NAME_LEN);

  return &(f->base);
}

font_t* font_bitmap_create(const char* name, const uint8_t* buff, uint32_t buff_size) {
  font_bitmap_t* font = NULL;
  return_value_if_fail(buff != NULL && name != NULL && buff_size > 0, NULL);

  font = TKMEM_ZALLOC(font_bitmap_t);
  return_value_if_fail(font != NULL, NULL);

  return font_bitmap_init(font, name, buff, buff_size);
}

static font_t* font_bitmap_load(font_loader_t* loader, const char* name, const uint8_t* buff,
                                uint32_t buff_size) {
  (void)loader;

  return font_bitmap_create(name, buff, buff_size);
}

font_loader_t* font_loader_bitmap(void) {
  static font_loader_t loader;
  loader.type = ASSET_TYPE_FONT_BMP;
  loader.load = font_bitmap_load;

  return &loader;
}

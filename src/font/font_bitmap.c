/**
 * File:   font.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  font interface
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "font/font_bitmap.h"
#include "base/mem.h"

typedef struct _font_bitmap_t {
  font_t base;
  const uint8_t* buff;
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

static ret_t font_bitmap_find_glyph(font_t* f, wchar_t c, glyph_t* g) {
  const uint8_t* p = NULL;
  font_bitmap_t* font = (font_bitmap_t*)f;
  font_bitmap_header_t* header = (font_bitmap_header_t*)(font->buff);
  font_bitmap_index_t* index = find_glyph(header->index, header->char_nr, c);
  return_value_if_fail(index != NULL, RET_NOT_FOUND);

  p = (font->buff + index->offset);
  memcpy(g, p, sizeof(glyph_t));
  g->data = font->buff + index->offset + 4;

  return RET_OK;
}

static ret_t font_bitmap_destroy(font_t* f) {
  MEM_FREE(f);
  return RET_OK;
}

font_t* font_bitmap_init(font_bitmap_t* f, const uint8_t* buff) {
  font_bitmap_header_t* header = (font_bitmap_header_t*)(buff);
  return_value_if_fail(f != NULL && buff != NULL && header->magic == FONT_MAGIC, NULL);

  f->buff = buff;
  f->base.scalable = FALSE;
  f->base.name = header->name;
  f->base.size = header->font_size;
  f->base.find_glyph = font_bitmap_find_glyph;
  f->base.destroy = font_bitmap_destroy;

  return &(f->base);
}

font_t* font_bitmap_create(const uint8_t* buff) {
  font_bitmap_t* font = NULL;
  return_value_if_fail(buff != NULL, NULL);

  font = MEM_ZALLOC(font_bitmap_t);
  return_value_if_fail(font != NULL, NULL);

  return font_bitmap_init(font, buff);
}

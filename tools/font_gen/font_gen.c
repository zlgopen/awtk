/**
 * File:   font_gen.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include <wctype.h>
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "common/utils.h"
#include "font_gen/font_gen.h"
#include "base/assets_manager.h"
#include "font_loader/font_loader_stb.h"
#include "font_loader/font_loader_bitmap.h"

#define MAX_CHARS 100 * 1024
#define MAX_BUFF_SIZE 1 * 1024 * 1024

static int char_cmp(const void* a, const void* b) {
  wchar_t c1 = *(wchar_t*)a;
  wchar_t c2 = *(wchar_t*)b;

  return c1 - c2;
}

ret_t font_gen(font_t* font, uint16_t font_size, const char* str, const char* output_filename) {
  uint8_t* buff = (uint8_t*)TKMEM_ALLOC(MAX_BUFF_SIZE);
  uint32_t size = font_gen_buff(font, font_size, str, buff, MAX_BUFF_SIZE);

  output_res_c_source(output_filename, ASSET_TYPE_FONT, ASSET_TYPE_FONT_BMP, buff, size);

  TKMEM_FREE(buff);

  return RET_OK;
}

uint32_t font_gen_buff(font_t* font, uint16_t font_size, const char* str, uint8_t* output_buff,
                       uint32_t buff_size) {
  int i = 0;
  glyph_t g;
  int size = 0;
  uint8_t* p = NULL;
  wchar_t wstr[MAX_CHARS];

  font_bitmap_header_t* header = (font_bitmap_header_t*)output_buff;

  utf8_to_utf16(str, wstr, MAX_CHARS);
  size = wcslen(wstr);

  qsort(wstr, size, sizeof(wchar_t), char_cmp);
  size = unique(wstr, size);

  header->char_nr = size;
  header->font_size = (uint8_t)font_size;
  header->baseline = (uint8_t)font_get_baseline(font, font_size);

  p = (uint8_t*)(header->index + size);
  return_value_if_fail(buff_size > 512, 0);

  for (i = 0; i < size; i++) {
    wchar_t c = wstr[i];
    font_bitmap_index_t* iter = header->index + i;

    iter->c = c;
    iter->offset = p - output_buff;

    if (iswspace(c)) {
      continue;
    }
    printf("%d/%d: 0x%04x\n", i, size, c);
    if (font_get_glyph(font, c, font_size, &g) == RET_OK) {
      uint32_t data_size = g.w * g.h;
      return_value_if_fail(buff_size > (iter->offset + data_size + 4), 0);

      save_uint8(p, g.x);
      save_uint8(p, g.y);
      save_uint8(p, g.w);
      save_uint8(p, g.h);
      save_uint32(p, g.advance);

      memcpy(p, g.data, data_size);
      p += data_size;
    } else if (c > 32) {
      printf("not found %d\n", c);
      exit(0);
    } else {
      iter->offset = 0;
    }
  }

  return p - output_buff;
}

/**
 * File:   font_gen.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
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

#include <wctype.h>
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/fs.h"
#include "base/bitmap.h"
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

const char* font_gen_expand_one(const char* in, str_t* out) {
  const char* p = in;

  if (p[0] == '\0' || p[1] == '\0' || p[2] == '\0' || p[3] == '\0') {
    return p += strlen(p);
  }

  if (p[1] == '-' && p[3] == ']' && p[4] == ']') {
    /*[[a-z]]*/
    char c = p[0];
    char end = p[2];
    while (c <= end) {
      str_append_char(out, c);
      c++;
    }
    p += 5;
  } else {
    /* [[1000-2000]] */
    char buff[32];
    int c = 0;
    int end = 0;
    int n = 0;
    if (strncmp(p, "0x", 2) == 0) {
      n = tk_sscanf(p, "0x%x-0x%x]]", &c, &end);
    } else if (strncmp(p, "0X", 2) == 0) {
      n = tk_sscanf(p, "0X%x-0X%x]]", &c, &end);
    } else {
      n = tk_sscanf(p, "%d-%d]]", &c, &end);
    }
    return_value_if_fail(n == 2, in);
    p = strstr(p, "]]");
    return_value_if_fail(p != NULL, in);
    p += 2;

    while (c <= end) {
      tk_utf8_from_utf16_ex(&c, 1, buff, sizeof(buff));
      str_append(out, buff);
      c++;
    }
  }

  return p;
}

const char* font_gen_expand(const char* in, str_t* out) {
  const char* p = in;

  while (*p) {
    if (*p == '[' && p[1] == '[') {
      p = font_gen_expand_one(p + 2, out);
    } else {
      str_append_char(out, *p);
      p++;
    }
  }

  return out->str;
}

ret_t font_gen(font_t* font, uint16_t font_size, glyph_format_t format, const char* str,
               const char* output_filename, const char* theme) {
  str_t tstr;
  wbuffer_t wbuffer;
  uint32_t size = 0;
  str_init(&tstr, 100000);
  wbuffer_init_extendable(&wbuffer);

  str = font_gen_expand(str, &tstr);
  size = font_gen_buff(font, font_size, format, str, &wbuffer);

  if (strstr(output_filename, ".bin") != NULL) {
    file_write(output_filename, wbuffer.data, size);
  } else {
    output_res_c_source(output_filename, theme, ASSET_TYPE_FONT, ASSET_TYPE_FONT_BMP, wbuffer.data,
                        size);
  }

  str_reset(&tstr);
  wbuffer_deinit(&wbuffer);

  return RET_OK;
}

uint32_t font_gen_buff(font_t* font, uint16_t font_size, glyph_format_t format, const char* str,
                       wbuffer_t* wbuffer) {
  int i = 0;
  glyph_t g;
  int size = 0;
  wchar_t wstr[MAX_CHARS];
  font_vmetrics_t vmetrics = font_get_vmetrics(font, font_size);

  tk_utf8_to_utf16(str, wstr, MAX_CHARS);
  size = wcslen(wstr);

  qsort(wstr, size, sizeof(wchar_t), char_cmp);
  size = unique(wstr, size);

  int32_t header_size = sizeof(font_bitmap_header_t) + (size - 1) * sizeof(font_bitmap_index_t);
  font_bitmap_header_t* header = (font_bitmap_header_t*)TKMEM_ALLOC(header_size);
  memset(header, 0, header_size);
  wbuffer_write_binary(wbuffer, header, header_size);

  header->char_nr = size;
  header->font_size = (uint8_t)font_size;
  header->ascent = vmetrics.ascent;
  header->descent = vmetrics.descent;
  header->line_gap = vmetrics.line_gap;

  memset(&g, 0x00, sizeof(g));
  for (i = 0; i < size; i++) {
    wchar_t c = wstr[i];

    header->index[i].c = c;
    header->index[i].size = 0;
    header->index[i].offset = wbuffer->cursor;

    printf("%d/%d: 0x%04x\n", i, size, c);
    if (font_get_glyph(font, c, font_size, &g) == RET_OK) {
      uint32_t data_size = (g.pitch ? g.pitch : g.w) * g.h;

      wbuffer_write_uint16(wbuffer, g.x);
      wbuffer_write_uint16(wbuffer, g.y);
      wbuffer_write_uint16(wbuffer, g.w);
      wbuffer_write_uint16(wbuffer, g.h);
      wbuffer_write_uint16(wbuffer, g.advance);
      wbuffer_write_uint8(wbuffer, g.format);
      wbuffer_write_uint8(wbuffer, g.pitch);

      if (g.data != NULL) {
        header->index[i].size = data_size;
        wbuffer_write_binary(wbuffer, g.data, data_size);
      }

      if (g.format == GLYPH_FMT_MONO) {
        bitmap_mono_dump(g.data, g.w, g.h);
      }
    } else if (c > 32 && c != 0xfeff) {
      wchar_t arr[] = {c};
      char utf8_arr[32] = {0};
      memset(utf8_arr, 0x00, sizeof(utf8_arr));
      tk_utf8_from_utf16(arr, utf8_arr, sizeof(utf8_arr));
      printf("Warnning: not find char 0x%04x('%s') in TTF\n", c, utf8_arr);
      header->index[i].offset = 0;
    } else {
      header->index[i].offset = 0;
    }
  }
  size = wbuffer->cursor;
  wbuffer->cursor = 0;
  wbuffer_write_binary(wbuffer, header, header_size);
  wbuffer->cursor = size;
  TKMEM_FREE(header);

  return size;
}

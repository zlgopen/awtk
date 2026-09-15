/**
 * File:   line_parser.h
 * Author: AWTK Develop Team
 * Brief:  line_parser structs.
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-8 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_LINE_PARSER_H
#define TK_LINE_PARSER_H

#include "tkc/types_def.h"
#include "base/font.h"
#include "base/line_break.h"

BEGIN_C_DECLS

typedef struct _line_parser_t {
  glyphs_t* glyphs;
  uint32_t size;
  uint32_t width;

  uint32_t total_lines;
  uint32_t total_paragraphs;

  uint32_t index;
  uint32_t nline_numbers;
  uint32_t cline_numbers;
  uint32_t* line_numbers;
  uint32_t curr_line_number;

  /*current line*/
  uint32_t line_index;
  uint32_t line_size;
  bool_t is_force_break;
  bool_t line_wrap;
  bool_t word_wrap;
} line_parser_t;

static ret_t line_parser_next(line_parser_t* parser);
static ret_t line_parser_next_impl(line_parser_t* parser);
static ret_t line_parser_deinit(line_parser_t* parser);

static ret_t line_parser_add_line_number(line_parser_t* parser, uint32_t line_number) {
  if (parser->nline_numbers + 1 > parser->cline_numbers) {
    uint32_t* line_numbers;
    uint32_t cline_numbers = parser->nline_numbers + 1 + parser->cline_numbers / 2;
    line_numbers = TKMEM_REALLOCT(uint32_t, parser->line_numbers, cline_numbers);
    return_value_if_fail(line_numbers != NULL, RET_OOM);
    parser->line_numbers = line_numbers;
    parser->cline_numbers = cline_numbers;
  }

  parser->line_numbers[parser->nline_numbers] = line_number;
  parser->nline_numbers++;
  return RET_OK;
}

static ret_t line_parser_init(line_parser_t* parser, glyphs_t* glyphs, uint32_t font_size,
                              uint32_t width, bool_t line_wrap, bool_t word_wrap) {
  return_value_if_fail(parser != NULL && glyphs != NULL, RET_BAD_PARAMS);
  return_value_if_fail(font_size > 0, RET_BAD_PARAMS);
  memset(parser, 0x00, sizeof(line_parser_t));

  parser->glyphs = glyphs;
  parser->size = glyphs_get_str_length(glyphs);
  parser->width = width;
  parser->line_wrap = line_wrap;
  parser->word_wrap = word_wrap;
  parser->line_index = 0;
  parser->line_size = 0;
  parser->is_force_break = FALSE;

  while (line_parser_next_impl(parser) == RET_OK) {
    if (parser->is_force_break) {
      parser->total_paragraphs++;
    }
    parser->total_lines++;
  }

  if (!parser->is_force_break) {
    parser->total_paragraphs++;
  }

  parser->line_index = 0;
  parser->line_size = 0;

  return RET_OK;
}

static ret_t line_parser_deinit(line_parser_t* parser) {
  if (parser != NULL) {
    if (parser->line_numbers != NULL) {
      TKMEM_FREE(parser->line_numbers);
    }
  }
  return RET_OK;
}

static ret_t line_parser_next(line_parser_t* parser) {
  return_value_if_fail(parser->glyphs != NULL, RET_BAD_PARAMS);
  if (parser->curr_line_number < parser->nline_numbers) {
    uint32_t line_number = parser->curr_line_number++;
    parser->line_index = parser->index;
    parser->index += parser->line_numbers[line_number];
    parser->line_size = parser->line_numbers[line_number];
    return RET_OK;
  }
  return RET_EOS;
}

static ret_t line_parser_next_impl(line_parser_t* parser) {
  int32_t w = 0;
  const wchar_t* str = NULL;
  int32_t char_w = 0;
  uint32_t index = 0;
  uint32_t real_index = 0;
  int32_t last_break_w = 0;
  int32_t last_breakable = 0;
  const glyph_t* g = NULL;
  return_value_if_fail(parser->glyphs != NULL, RET_BAD_PARAMS);

  parser->line_index = parser->line_index + parser->line_size;
  parser->line_size = 0;
  str = glyphs_get_str(parser->glyphs);

  if (parser->line_index >= parser->size) {
    return RET_EOS;
  }

  index = parser->line_index;
  parser->is_force_break = FALSE;
  while (index < parser->size) {
    // 获取当前 index 位置的字模
    real_index = glyphs_get_glyph_index_from_str_index(parser->glyphs, index);
    g = glyphs_get(parser->glyphs, real_index);
    break_if_fail(g != NULL);

    char_w = glyphs_measure(parser->glyphs, real_index, g->glyph_count) + 1;

    // 判断 line_break，并记录数据
    if (index > 0 && parser->word_wrap) {
      if (g != NULL && line_break_check(str[index - 1], g->chr) == LINE_BREAK_ALLOW) {
        last_breakable = index;
        last_break_w = w;
      }
    }

    // 判断字符是否为换行符，并进行换行
    if (g->chr == '\r') {
      index++;
      if (str[index] == '\n') {
        index++;
      }
      parser->is_force_break = TRUE;
      break;
    } else if (g->chr == '\n') {
      index++;
      parser->is_force_break = TRUE;
      break;
    }

    // 判断是否换行，如果有 word_wrap 就将部分数据还原回去
    if (parser->line_wrap) {
      if ((w + char_w) > parser->width) {
        if (parser->word_wrap && last_breakable > 0) {
          w = last_break_w;
          index = last_breakable;
        }
        break;
      }
      w += char_w - 1;
      index += g->str_count;
    } else {
      index += g->str_count;
      w += char_w - 1;
    }
  }

  /* 记录本行：line_index 为行首字符串序号，line_size 为本行字符串数 */
  parser->line_size = index - parser->line_index;
  line_parser_add_line_number(parser, parser->line_size);

  return RET_OK;
}

END_C_DECLS

#endif /*TK_LINE_PARSER_H*/

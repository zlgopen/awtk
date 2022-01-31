/**
 * File:   line_parser.h
 * Author: AWTK Develop Team
 * Brief:  line_parser structs.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/canvas.h"
#include "base/line_break.h"

BEGIN_C_DECLS

typedef struct _line_parser_t {
  canvas_t* c;
  uint32_t size;
  uint32_t width;
  uint32_t font_size;
  const wchar_t* str;

  uint32_t total_lines;
  uint32_t total_paragraphs;

  uint32_t nline_numbers;
  uint32_t cline_numbers;
  uint32_t* line_numbers;
  uint32_t curr_line_number;

  /*current line*/
  const wchar_t* line;
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

static ret_t line_parser_init(line_parser_t* parser, canvas_t* c, const wchar_t* str, uint32_t size,
                              uint32_t font_size, uint32_t width, bool_t line_wrap,
                              bool_t word_wrap) {
  return_value_if_fail(parser != NULL && str != NULL && size > 0, RET_BAD_PARAMS);
  return_value_if_fail(font_size > 0, RET_BAD_PARAMS);

  memset(parser, 0x00, sizeof(line_parser_t));

  parser->c = c;
  parser->str = str;
  parser->size = size;
  parser->width = width;
  parser->font_size = font_size;
  parser->line_wrap = line_wrap;
  parser->word_wrap = word_wrap;
  parser->line = str;
  parser->line_size = 0;
  parser->is_force_break = FALSE;

  while (line_parser_next_impl(parser) == RET_OK) {
    if (parser->is_force_break) {
      parser->total_paragraphs++;
    }
    parser->total_lines++;
    line_parser_add_line_number(parser, parser->line - str);
  }

  if (!parser->is_force_break) {
    parser->total_paragraphs++;
  }

  parser->line = str;
  parser->line_size = 0;

  return RET_OK;
}

static ret_t line_parser_deinit(line_parser_t* parser) {
  if (parser != NULL && parser->line_numbers != NULL) {
    TKMEM_FREE(parser->line_numbers);
  }
  return RET_OK;
}

static ret_t line_parser_next(line_parser_t* parser) {
  return_value_if_fail(parser->str != NULL, RET_BAD_PARAMS);
  if (parser->curr_line_number < parser->nline_numbers) {
    uint32_t line_number = parser->curr_line_number++;
    parser->line = parser->str + parser->line_numbers[line_number];
    if (parser->nline_numbers == 1) {
      parser->line_size = wcslen(parser->str);
    } else {
      if (line_number + 1 < parser->nline_numbers) {
        parser->line_size =
            parser->line_numbers[line_number + 1] - parser->line_numbers[line_number];
      } else {
        parser->line_size = wcslen(parser->line);
      }
    }
    return RET_OK;
  }
  return RET_EOS;
}

static ret_t line_parser_next_impl(line_parser_t* parser) {
  int32_t w = 0;
  int32_t char_w = 0;
  int32_t line_w = 0;
  const wchar_t* p = NULL;
  canvas_t* c = parser->c;
  bool_t is_get_text_width = TRUE;
  return_value_if_fail(parser->str != NULL, RET_BAD_PARAMS);

  parser->line = parser->line + parser->line_size;
  parser->line_size = 0;

  if ((parser->line - parser->str) >= parser->size) {
    return RET_EOS;
  }

  p = parser->line;
  parser->is_force_break = FALSE;
  while ((p - parser->str) < parser->size) {
    if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
      parser->is_force_break = TRUE;
      break;
    } else if (*p == '\n') {
      p++;
      parser->is_force_break = TRUE;
      break;
    }

    if (parser->line_wrap) {
      if (line_w >= parser->width) {
        if (is_get_text_width) {
          w = canvas_measure_text(c, parser->line, p - parser->line);
          is_get_text_width = FALSE;
        }
        char_w = canvas_measure_text(c, p, 1) + 1;
        if ((w + char_w) > parser->width) {
          return_value_if_fail(p > parser->line, RET_FAIL);

          if (((p + 1) - parser->str) < parser->size) {
            if (parser->word_wrap) {
              const wchar_t* tmp_p = p;
              while (line_break_check(*(p - 1), *p) == LINE_BREAK_NO) {
                if (p == parser->line + 1) {
                  p = tmp_p;
                  break;
                }

                if (p > parser->line) {
                  p--;
                }
              }
            }

            if (tk_isspace(*p)) {
              p++;
              w += char_w;
            }
            break;
          }
        }
        w += char_w - 1;
      }
      p++;
      line_w += parser->font_size;
    } else {
      p++;
    }
  }

  parser->line_size = p - parser->line;

  return RET_OK;
}

END_C_DECLS

#endif /*TK_LINE_PARSER_H*/

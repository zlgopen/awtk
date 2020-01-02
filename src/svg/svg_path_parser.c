/**
 * File:   svg_path_parser.c
 * Author: AWTK Develop Team
 * Brief:  svg path_parser
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
 * 2018-11-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "svg/svg_path.h"
#include "svg/svg_path_parser.h"

typedef struct _svg_path_parser_t {
  const char* p;
  const char* path;
  void* ctx;
  tk_visit_t on_path;
} svg_path_parser_t;

typedef enum _token_type_t { TOKEN_NUMBER, TOKEN_CMD, TOKEN_EOF } token_type_t;

static token_type_t svg_path_parser_next_token_type(svg_path_parser_t* parser) {
  const char* p = parser->p;
  while (isspace(*p) || *p == ',') {
    p++;
  }

  parser->p = p;
  if (!*p) {
    return TOKEN_EOF;
  }

  if (isalpha(*p)) {
    return TOKEN_CMD;
  } else {
    return TOKEN_NUMBER;
  }
}

static char svg_path_parser_get_cmd(svg_path_parser_t* parser) {
  char c = parser->p[0];

  parser->p++;

  return c;
}

static float svg_path_parser_get_number(svg_path_parser_t* parser) {
  uint32_t i = 0;
  const char* p = NULL;
  char token[TK_NUM_MAX_LEN + 1];
  memset(token, 0x00, sizeof(token));

  return_value_if_fail(svg_path_parser_next_token_type(parser) == TOKEN_NUMBER, 0);

  p = parser->p;
  if (*p == '+' || *p == '-') {
    token[i++] = *p++;
  }
  while (*p == '.' || (*p >= '0' && *p <= '9')) {
    if (*p == '.' && strrchr(token, '.') != NULL) break;
    token[i++] = *p++;
  }
  token[i] = '\0';
  parser->p = p;

  return tk_atof(token);
}

static ret_t svg_path_parser_parse_cmd(svg_path_parser_t* parser, char c) {
  float x = 0;
  float y = 0;
  float x1 = 0;
  float y1 = 0;
  float x2 = 0;
  float y2 = 0;

  switch (c) {
    case 'M':
    case 'm': {
      svg_path_move_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'M') {
          svg_path_move_init(&path, x, y);
        } else {
          svg_path_move_rel_init(&path, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'Z':
    case 'z': {
      svg_path_t path = {SVG_PATH_Z};
      parser->on_path(parser->ctx, (svg_path_t*)&path);
      break;
    }
    case 'L':
    case 'l': {
      svg_path_line_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'L') {
          svg_path_line_init(&path, x, y);
        } else {
          svg_path_line_rel_init(&path, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'H':
    case 'h': {
      svg_path_hline_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x = svg_path_parser_get_number(parser);

        if (c == 'H') {
          svg_path_hline_init(&path, x);
        } else {
          svg_path_hline_rel_init(&path, x);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'V':
    case 'v': {
      svg_path_vline_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        y = svg_path_parser_get_number(parser);

        if (c == 'V') {
          svg_path_vline_init(&path, y);
        } else {
          svg_path_vline_rel_init(&path, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'C':
    case 'c': {
      svg_path_curve_to_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x1 = svg_path_parser_get_number(parser);
        y1 = svg_path_parser_get_number(parser);
        x2 = svg_path_parser_get_number(parser);
        y2 = svg_path_parser_get_number(parser);
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'C') {
          svg_path_curve_to_init(&path, x1, y1, x2, y2, x, y);
        } else {
          svg_path_curve_to_rel_init(&path, x1, y1, x2, y2, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'S':
    case 's': {
      svg_path_scurve_to_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x2 = svg_path_parser_get_number(parser);
        y2 = svg_path_parser_get_number(parser);
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'S') {
          svg_path_scurve_to_init(&path, x2, y2, x, y);
        } else {
          svg_path_scurve_to_rel_init(&path, x2, y2, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'Q':
    case 'q': {
      svg_path_qcurve_to_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x1 = svg_path_parser_get_number(parser);
        y1 = svg_path_parser_get_number(parser);
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'Q') {
          svg_path_qcurve_to_init(&path, x1, y1, x, y);
        } else {
          svg_path_qcurve_to_rel_init(&path, x1, y1, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'T':
    case 't': {
      svg_path_tcurve_to_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'T') {
          svg_path_tcurve_to_init(&path, x, y);
        } else {
          svg_path_tcurve_to_rel_init(&path, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    case 'A':
    case 'a': {
      svg_path_arc_t path;
      while (svg_path_parser_next_token_type(parser) == TOKEN_NUMBER) {
        float rx = svg_path_parser_get_number(parser);
        float ry = svg_path_parser_get_number(parser);
        float rotation = svg_path_parser_get_number(parser);
        float large_arc = svg_path_parser_get_number(parser);
        float sweep = svg_path_parser_get_number(parser);
        x = svg_path_parser_get_number(parser);
        y = svg_path_parser_get_number(parser);

        if (c == 'A') {
          svg_path_arc_init(&path, rx, ry, rotation, large_arc, sweep, x, y);
        } else {
          svg_path_arc_rel_init(&path, rx, ry, rotation, large_arc, sweep, x, y);
        }
        parser->on_path(parser->ctx, (svg_path_t*)&path);
      }
      break;
    }
    default: { assert(!"not supported path!"); } break;
  }

  return RET_OK;
}

static ret_t svg_path_parser_parse(svg_path_parser_t* parser) {
  token_type_t type = svg_path_parser_next_token_type(parser);

  while (type != TOKEN_EOF) {
    if (type == TOKEN_CMD) {
      char c = svg_path_parser_get_cmd(parser);
      svg_path_parser_parse_cmd(parser, c);
    } else {
      assert(!"unexpected number");
      svg_path_parser_get_number(parser);
    }
    type = svg_path_parser_next_token_type(parser);
  }

  return RET_OK;
}

static ret_t svg_path_parser_init(svg_path_parser_t* parser, const char* path, void* ctx,
                                  tk_visit_t on_path) {
  memset(parser, 0x00, sizeof(*parser));

  parser->p = path;
  parser->ctx = ctx;
  parser->on_path = on_path;

  return RET_OK;
}

ret_t svg_path_parse(const char* path, void* ctx, tk_visit_t on_path) {
  svg_path_parser_t parser;
  return_value_if_fail(path != NULL && on_path != NULL, RET_BAD_PARAMS);

  svg_path_parser_init(&parser, path, ctx, on_path);
  return svg_path_parser_parse(&parser);
}

/**
 * File:   func_call_parser.h
 * Author: AWTK Develop Team
 * Brief:  func_call_parser
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is difunc_call_parseributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-10-20 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/func_call_parser.h"

func_call_parser_t* func_call_parser_init(func_call_parser_t* parser, const char* str,
                                          uint32_t size) {
  return_value_if_fail(parser != NULL && str != NULL, NULL);

  tokenizer_init(&(parser->tokenizer), str, size, "(=,)\r\n\t ");

  return parser;
}

ret_t func_call_parser_parse(func_call_parser_t* parser) {
  const char* token = NULL;
  char name[TK_NAME_LEN + 1];
  tokenizer_t* tokenizer = NULL;
  return_value_if_fail(parser != NULL, RET_BAD_PARAMS);

  tokenizer = &(parser->tokenizer);
  return_value_if_fail(tokenizer_has_more(tokenizer) == TRUE, RET_BAD_PARAMS);

  token = tokenizer_next(tokenizer);
  if (parser->on_name != NULL) {
    parser->on_name(parser, token);
  }

  while (tokenizer_has_more(tokenizer)) {
    token = tokenizer_next(tokenizer);
    tk_strncpy(name, token, TK_NAME_LEN);

    ENSURE(tokenizer_has_more(tokenizer));
    token = tokenizer_next(tokenizer);

    if (parser->on_param != NULL) {
      parser->on_param(parser, name, token);
    }
  }

  if (parser->on_done != NULL) {
    parser->on_done(parser);
  }

  return RET_OK;
}

ret_t func_call_parser_deinit(func_call_parser_t* parser) {
  return_value_if_fail(parser != NULL, RET_BAD_PARAMS);

  tokenizer_deinit(&(parser->tokenizer));
  memset(parser, 0x00, sizeof(func_call_parser_t));

  return RET_OK;
}

typedef struct _object_parser_t {
  func_call_parser_t base;

  object_t* obj;
} object_parser_t;

static ret_t parser_on_param(func_call_parser_t* parser, const char* name, const char* value) {
  object_parser_t* p = (object_parser_t*)parser;

  return object_set_prop_str(p->obj, name, value);
}

static ret_t parser_on_name(func_call_parser_t* parser, const char* func_name) {
  object_parser_t* p = (object_parser_t*)parser;

  return object_set_name(p->obj, func_name);
}

object_t* func_call_parse(const char* str, uint32_t size) {
  object_parser_t parser;
  return_value_if_fail(str != NULL && size > 0, NULL);

  memset(&parser, 0x00, sizeof(parser));
  func_call_parser_init(&(parser.base), str, size);

  parser.obj = object_default_create();
  parser.base.on_name = parser_on_name;
  parser.base.on_param = parser_on_param;
  return_value_if_fail(parser.obj != NULL, NULL);

  func_call_parser_parse(&(parser.base));
  func_call_parser_deinit(&(parser.base));

  return parser.obj;
}

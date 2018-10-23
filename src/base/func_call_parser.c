/**
 * File:   func_call_parser.h
 * Author: AWTK Develop Team
 * Brief:  func_call_parser
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/utils.h"
#include "base/func_call_parser.h"

func_call_parser_t* func_call_parser_init(func_call_parser_t* parser, const char* str,
                                          uint32_t size) {
  return_value_if_fail(parser != NULL && str != NULL, NULL);

  tokenizer_init(&(parser->tokenizer), str, size, "(=,) ");

  return parser;
}

ret_t func_call_parser_parse(func_call_parser_t* parser) {
  const char* token = NULL;
  char name[NAME_LEN + 1];
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
    tk_strncpy(name, token, NAME_LEN);

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

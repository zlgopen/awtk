/**
 * File:   self_layouter_parser.c
 * Author: AWTK Develop Team
 * Brief:  self layouter parser
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/func_call_parser.h"
#include "base/self_layouter_factory.h"
#include "layouters/self_layouter_default.h"

typedef struct _self_layouter_parser_t {
  func_call_parser_t base;

  self_layouter_t* layouter;
} self_layouter_parser_t;

static ret_t self_layouter_parser_on_name(func_call_parser_t* parser, const char* func_name) {
  self_layouter_factory_t* f = self_layouter_factory();
  self_layouter_parser_t* self_layouter_parser = (self_layouter_parser_t*)parser;

  return_value_if_fail(func_name != NULL, RET_BAD_PARAMS);
  self_layouter_parser->layouter = self_layouter_factory_create_layouter(f, func_name);
  return_value_if_fail(self_layouter_parser->layouter != NULL, RET_NOT_IMPL);

  return RET_OK;
}

static ret_t self_layouter_parser_on_param(func_call_parser_t* parser, const char* name,
                                           const char* value) {
  self_layouter_parser_t* self_layouter_parser = (self_layouter_parser_t*)parser;
  self_layouter_set_param_str(self_layouter_parser->layouter, name, value);

  return RET_OK;
}

self_layouter_t* self_layouter_create(const char* params) {
  self_layouter_t* layouter = NULL;
  self_layouter_parser_t parser;
  return_value_if_fail(params != NULL && params[0], NULL);

  memset(&parser, 0x00, sizeof(parser));
  return_value_if_fail(func_call_parser_init(&(parser.base), params, strlen(params)) != NULL, NULL);

  parser.layouter = NULL;
  parser.base.on_name = self_layouter_parser_on_name;
  parser.base.on_param = self_layouter_parser_on_param;

  func_call_parser_parse(&(parser.base));
  layouter = parser.layouter;
  func_call_parser_deinit(&(parser.base));

  return layouter;
}

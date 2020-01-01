/**
 * File:   children_layouter_parser.c
 * Author: AWTK Develop Team
 * Brief:  children layouter factory
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

#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "tkc/func_call_parser.h"
#include "base/children_layouter_factory.h"
#include "layouters/children_layouter_default.h"

typedef struct _children_layouter_parser_t {
  func_call_parser_t base;

  children_layouter_t* layouter;
} children_layouter_parser_t;

static ret_t children_layouter_parser_on_name(func_call_parser_t* parser, const char* func_name) {
  children_layouter_factory_t* f = children_layouter_factory();
  children_layouter_parser_t* children_layouter_parser = (children_layouter_parser_t*)parser;

  return_value_if_fail(func_name != NULL, RET_BAD_PARAMS);

  children_layouter_parser->layouter = children_layouter_factory_create_layouter(f, func_name);
  return_value_if_fail(children_layouter_parser->layouter != NULL, RET_NOT_IMPL);

  return RET_OK;
}

static ret_t children_layouter_parser_on_param(func_call_parser_t* parser, const char* name,
                                               const char* value) {
  children_layouter_parser_t* children_layouter_parser = (children_layouter_parser_t*)parser;
  children_layouter_set_param_str(children_layouter_parser->layouter, name, value);

  return RET_OK;
}

children_layouter_t* children_layouter_create_new(const char* params) {
  children_layouter_t* layouter = NULL;
  children_layouter_parser_t parser;
  return_value_if_fail(params != NULL, NULL);

  memset(&parser, 0x00, sizeof(parser));
  return_value_if_fail(func_call_parser_init(&(parser.base), params, strlen(params)) != NULL, NULL);

  parser.base.on_name = children_layouter_parser_on_name;
  parser.base.on_param = children_layouter_parser_on_param;

  parser.layouter = NULL;
  func_call_parser_parse(&(parser.base));
  layouter = parser.layouter;
  func_call_parser_deinit(&(parser.base));

  return layouter;
}

children_layouter_t* children_layouter_create_old(const char* params) {
  value_t v;
  char name[2];
  tokenizer_t tokenizer;
  tokenizer_t* t = &tokenizer;
  children_layouter_t* layout = children_layouter_default_create();
  return_value_if_fail(layout != NULL && params != NULL, NULL);

  name[1] = '\0';
  tokenizer_init(t, params, strlen(params), " ,");
  while (tokenizer_has_more(t)) {
    const char* key = tokenizer_next(t);

    *name = *key;
    value_set_int(&v, tk_atoi(tk_skip_to_num(key)));
    children_layouter_set_param(layout, name, &v);
  }
  tokenizer_deinit(t);

  return layout;
}

children_layouter_t* children_layouter_create(const char* params) {
  return_value_if_fail(params != NULL && params[0], NULL);
  if (strchr(params, '(')) {
    return children_layouter_create_new(params);
  } else {
    return children_layouter_create_old(params);
  }
}

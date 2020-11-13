/**
 * File:   fscript.c
 * Author: AWTK Develop Team
 * Brief:  a simple functional script language
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-11-09 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "tkc/fscript.h"

struct _fscript_func_call_t;
typedef struct _fscript_func_call_t fscript_func_call_t;

struct _fscript_func_call_t {
  fscript_func_t func;
  fscript_args_t args;
  fscript_func_call_t* next;
};

#define VALUE_TYPE_JSCRIPT_ID 128
#define VALUE_TYPE_JSCRIPT_FUNC VALUE_TYPE_JSCRIPT_ID + 1

static ret_t func_if(object_t* obj, fscript_args_t* args, value_t* result);
static ret_t func_set(object_t* obj, fscript_args_t* args, value_t* result);

static value_t* value_set_func(value_t* v, fscript_func_call_t* func) {
  value_set_pointer(v, func);
  v->type = VALUE_TYPE_JSCRIPT_FUNC;
  return v;
}

static ret_t func_args_extend(fscript_args_t* args) {
  if (args->size < args->capacity) {
    return RET_OK;
  }

  if (args->capacity < FSCRIPT_MAX_ARGS) {
    value_t* new_args = NULL;
    uint16_t capacity = args->capacity + 2;
    capacity = tk_min(capacity, FSCRIPT_MAX_ARGS);
    new_args = TKMEM_REALLOC(args->args, capacity * sizeof(value_t));
    return_value_if_fail(new_args != NULL, RET_OOM);
    args->args = new_args;
    args->capacity = capacity;
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_args_push_str(fscript_args_t* args, const char* str, uint32_t size,
                                bool_t is_id) {
  value_t* v = NULL;
  return_value_if_fail(str != NULL && func_args_extend(args) == RET_OK, RET_BAD_PARAMS);
  v = args->args + args->size++;
  value_dup_str_with_len(v, str, size);
  v->type = is_id ? VALUE_TYPE_JSCRIPT_ID : v->type;

  return RET_OK;
}

static ret_t func_args_push(fscript_args_t* args, value_t* v) {
  return_value_if_fail(v != NULL && func_args_extend(args) == RET_OK, RET_BAD_PARAMS);
  value_deep_copy(args->args + args->size++, v);

  return RET_OK;
}

static ret_t func_args_init(fscript_args_t* args, uint16_t init_args_capacity) {
  memset(args, 0x00, sizeof(fscript_args_t));

  if (init_args_capacity > 0) {
    args->args = TKMEM_ZALLOCN(value_t, init_args_capacity);
    return_value_if_fail(args->args != NULL, RET_OOM);
    args->capacity = init_args_capacity;
  }

  return RET_OK;
}

static ret_t func_args_deinit(fscript_args_t* args) {
  uint32_t i = 0;

  for (i = 0; i < args->size; i++) {
    value_t* v = args->args + i;
    v->type = v->type == VALUE_TYPE_JSCRIPT_ID ? VALUE_TYPE_STRING : v->type;
    value_reset(args->args + i);
  }
  TKMEM_FREE(args->args);
  memset(args, 0x00, sizeof(fscript_args_t));

  return RET_OK;
}

static ret_t fscript_func_call_destroy(fscript_func_call_t* call) {
  fscript_func_call_t* iter = call;
  fscript_func_call_t* next = NULL;

  while (iter != NULL) {
    next = iter->next;
    func_args_deinit(&(call->args));
    TKMEM_FREE(iter);
    iter = next;
  }

  return RET_OK;
}

typedef enum _token_type_t {
  TOKEN_ID = 0,
  TOKEN_FUNC,
  TOKEN_STR,
  TOKEN_NUMBER,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_COMMA
} token_type_t;

typedef struct _token_t {
  const char* token;
  token_type_t type;
  uint32_t size;
  bool_t valid;
} token_t;

typedef struct _fscript_parser_t {
  const char* str;
  const char* cursor;
  object_t* obj;
  token_t token;

  char* error;
  uint32_t row;
  uint32_t col;

  char c;
  str_t temp;
  bool_t ignore;

  fscript_func_call_t* first;
} fscript_parser_t;

typedef struct _fscript_t {
  str_t str;
  object_t* obj;
  fscript_func_call_t* first;
} fscript_t;

static fscript_func_call_t* fscript_func_call_create(fscript_parser_t* parser, const char* name,
                                                     uint32_t size);

fscript_t* fscript_create_impl(fscript_parser_t* parser) {
  fscript_t* fscript = TKMEM_ZALLOC(fscript_t);
  return_value_if_fail(fscript != NULL, NULL);
  fscript->str = parser->temp;
  fscript->obj = OBJECT_REF(parser->obj);
  fscript->first = parser->first;

  parser->obj = NULL;
  parser->first = NULL;
  parser->temp.str = NULL;

  return fscript;
}

static ret_t fscript_exec_func(fscript_t* fscript, fscript_func_call_t* iter, value_t* result) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  fscript_args_t args;

  func_args_init(&args, iter->args.size);
  args.str = &(fscript->str);
  args.size = iter->args.size;
  return_value_if_fail((args.args != NULL || args.size == 0), RET_OOM);
  for (i = 0; i < iter->args.size; i++) {
    value_t* s = iter->args.args + i;
    value_t* d = args.args + i;
    int32_t save_type = s->type;
    if (s->type == VALUE_TYPE_JSCRIPT_ID) {
      s->type = VALUE_TYPE_STRING;
      if (iter->func == func_set && i == 0) {
        value_copy(d, s); /*func_set accept id/str as first param*/
      } else if (object_get_prop(fscript->obj, value_str(s), d) != RET_OK) {
        value_copy(d, s);
      }
    } else if (s->type == VALUE_TYPE_JSCRIPT_FUNC) {
      s->type = VALUE_TYPE_POINTER;
      fscript_func_call_t* func = (fscript_func_call_t*)value_pointer(s);
      if (i > 0 && iter->func == func_if) {
        if (value_bool(args.args) && i == 1) {
          fscript_exec_func(fscript, func, d);
        }
        if (!value_bool(args.args) && i == 2) {
          fscript_exec_func(fscript, func, d);
        }
      } else {
        fscript_exec_func(fscript, func, d);
      }
    } else {
      value_copy(d, s);
    }
    s->type = save_type;
  }

  ret = iter->func(fscript->obj, &args, result);
  func_args_deinit(&args);

  return ret;
}

ret_t fscript_exec(fscript_t* fscript, value_t* result) {
  fscript_func_call_t* iter = NULL;
  return_value_if_fail(fscript != NULL, RET_FAIL);

  iter = fscript->first;
  while (iter != NULL) {
    return_value_if_fail(iter->func != NULL, RET_FAIL);
    value_reset(result);
    return_value_if_fail(fscript_exec_func(fscript, iter, result) == RET_OK, RET_FAIL);
    iter = iter->next;
  }

  return RET_OK;
}

ret_t fscript_destroy(fscript_t* fscript) {
  return_value_if_fail(fscript != NULL, RET_FAIL);

  OBJECT_UNREF(fscript->obj);
  str_reset(&(fscript->str));
  fscript_func_call_destroy(fscript->first);
  memset(fscript, 0x00, sizeof(fscript_t));

  return RET_OK;
}

static ret_t fscript_parser_init(fscript_parser_t* parser, object_t* obj, const char* str) {
  memset(parser, 0x00, sizeof(fscript_parser_t));

  parser->obj = obj;
  parser->str = str;
  parser->cursor = str;
  str_init(&(parser->temp), 64);

  return RET_OK;
}

static ret_t fscript_parser_deinit(fscript_parser_t* parser) {
  str_reset(&(parser->temp));
  TKMEM_FREE(parser->error);
  fscript_func_call_destroy(parser->first);
  return RET_OK;
}

static char fscript_parser_get_char(fscript_parser_t* parser) {
  char c = '\0';
  return_value_if_fail(parser != NULL, c);
  if (parser->c) {
    c = parser->c;
    parser->c = '\0';
    return c;
  }

  c = parser->cursor[0];
  if (c) {
    if (c == '\n') {
      parser->row++;
      parser->col = 0;
    } else {
      parser->col++;
    }

    parser->cursor++;
  }

  return c;
}

static ret_t fscript_parser_unget_char(fscript_parser_t* parser, char c) {
  return_value_if_fail(parser->cursor > parser->str, RET_BAD_PARAMS);

  parser->c = c;

  return RET_OK;
}

static ret_t fscript_parser_set_error(fscript_parser_t* parser, const char* str) {
  return_value_if_fail(parser != NULL && str != NULL, RET_BAD_PARAMS);

  parser->error = tk_str_copy(parser->error, str);
  log_warn("at line(%u) col (%u): %s\n", parser->row, parser->col, str);

  return RET_OK;
}

static ret_t fscript_parser_skip_seperators(fscript_parser_t* parser) {
  char c = '\0';

  do {
    c = fscript_parser_get_char(parser);
  } while (isspace(c) || c == ';');
  fscript_parser_unget_char(parser, c);

  return RET_OK;
}

#define TOKEN_INIT(t, ttype, str) \
  t->type = ttype;                \
  t->token = str->str;            \
  t->size = str->size;

static ret_t fscript_parser_parse_str(fscript_parser_t* parser) {
  char c = '\0';
  bool_t escape = FALSE;
  str_t* str = &(parser->temp);
  token_t* t = &(parser->token);

  str_set(str, "");
  do {
    c = fscript_parser_get_char(parser);
    if ((!escape && c == '\"') || c == '\0') {
      break;
    }
    if (!escape && c == '\\') {
      escape = TRUE;
      continue;
    }
    if (escape) {
      escape = FALSE;
    }
    str_append_char(str, c);
  } while (TRUE);

  TOKEN_INIT(t, TOKEN_STR, str);
  if (c != '\0' && c != '\"') {
    fscript_parser_unget_char(parser, c);
  }

  return RET_OK;
}

static ret_t fscript_parser_parse_id_or_number(fscript_parser_t* parser, token_type_t def_type) {
  char c = '\0';
  str_t* str = &(parser->temp);
  token_t* t = &(parser->token);

  str_set(str, "");
  do {
    c = fscript_parser_get_char(parser);
    if (c != '(' && c != ')' && c != ',' && c != '\"' && c != '\0') {
      str_append_char(str, c);
    } else {
      break;
    }
  } while (TRUE);

  str_trim(str, " \t\r\n");
  TOKEN_INIT(t, (c == '(' ? TOKEN_FUNC : def_type), str);
  if (c != '\0') {
    fscript_parser_unget_char(parser, c);
  }

  return RET_OK;
}

static token_t* fscript_parser_get_token(fscript_parser_t* parser) {
  char c = 0;
  token_t* t = &(parser->token);

  if (t->valid) {
    t->valid = FALSE;
    return t;
  }

  fscript_parser_skip_seperators(parser);
  c = fscript_parser_get_char(parser);

  t->size = 1;
  t->token = parser->cursor - 1;
  switch (c) {
    case '\0': {
      return NULL;
    }
    case '(': {
      t->type = TOKEN_LPAREN;
      return t;
    }
    case ')': {
      t->type = TOKEN_RPAREN;
      return t;
    }
    case ',': {
      t->type = TOKEN_COMMA;
      return t;
    }
    case '>':
    case '<':
    case '!':
    case '=':
    case '*':
    case '/':
    case '%':
    case '|':
    case '^':
    case '~':
    case '&': {
      fscript_parser_skip_seperators(parser);
      c = fscript_parser_get_char(parser);
      if (c == '(') {
        fscript_parser_unget_char(parser, c);
        t->size = parser->cursor - t->token - 1;
      } else {
        t->size = parser->cursor - t->token;
      }

      t->type = TOKEN_FUNC;
      return t;
    }
    case '\"': {
      fscript_parser_parse_str(parser);
      return t;
    }
    default: {
      fscript_parser_unget_char(parser, c);
      if (c == '+' || c == '-' || isdigit(c)) {
        fscript_parser_parse_id_or_number(parser, TOKEN_NUMBER);
      } else {
        fscript_parser_parse_id_or_number(parser, TOKEN_ID);
      }
      return t;
    }
  }
}

static ret_t fscript_parser_expect_token(fscript_parser_t* parser, token_type_t type,
                                         const char* msg) {
  token_t* t = fscript_parser_get_token(parser);
  if (t == NULL || t->type != type) {
    fscript_parser_set_error(parser, msg);
  }

  return RET_OK;
}

static ret_t fscript_parser_unget_token(fscript_parser_t* parser) {
  parser->token.valid = TRUE;
  return RET_OK;
}

static ret_t fscript_parse_func(fscript_parser_t* parser, fscript_func_call_t* call) {
  value_t v;
  bool_t done = FALSE;
  fscript_args_t* args = &(call->args);
  token_t* t = fscript_parser_get_token(parser);

  fscript_parser_expect_token(parser, TOKEN_LPAREN, "expect \"(\"");
  while (parser->error == NULL && t != NULL && t->type != TOKEN_RPAREN) {
    t = fscript_parser_get_token(parser);
    if (t == NULL) {
      fscript_parser_expect_token(parser, TOKEN_LPAREN, "expect \")\"");
      return RET_FAIL;
    }

    switch (t->type) {
      case TOKEN_ID: {
        if (t->token[0] == 't' && strncmp(t->token, "true", 4) == 0) {
          func_args_push(args, value_set_bool(&v, TRUE));
        } else if (t->token[0] == 'f' && strncmp(t->token, "false", 5) == 0) {
          func_args_push(args, value_set_bool(&v, FALSE));
        } else {
          func_args_push_str(args, t->token, t->size, TRUE);
        }
        break;
      }
      case TOKEN_FUNC: {
        fscript_parser_unget_token(parser);
        fscript_func_call_t* acall = fscript_func_call_create(parser, t->token, t->size);
        return_value_if_fail(acall != NULL, RET_BAD_PARAMS);
        fscript_parse_func(parser, acall);
        value_set_func(&v, acall);
        func_args_push(args, &v);
        break;
      }
      case TOKEN_NUMBER: {
        char number[32];
        tk_strncpy(number, t->token, tk_min(t->size, sizeof(number) - 1));
        if (strchr(number, '.') != NULL) {
          value_set_double(&v, tk_atof(number));
        } else {
          value_set_int(&v, tk_atoi(number));
        }
        func_args_push(args, &v);
        break;
      }
      case TOKEN_STR: {
        func_args_push_str(args, t->token, t->size, FALSE);
        break;
      }
      case TOKEN_RPAREN: {
        done = TRUE;
        break;
      }
      default: {
        fscript_parser_set_error(parser, "unexpected token:");
        break;
      }
    }

    if (done) {
      break;
    }

    t = fscript_parser_get_token(parser);
    if (t == NULL) {
      fscript_parser_expect_token(parser, TOKEN_LPAREN, "expect \")\"");
      break;
    }
  }

  return RET_OK;
}

static ret_t fscript_parse(fscript_parser_t* parser) {
  char c = '\0';
  ret_t ret = RET_OK;
  fscript_func_call_t* acall = NULL;
  fscript_func_call_t* last = NULL;

  while (parser->cursor[0]) {
    token_t* t = fscript_parser_get_token(parser);
    if (t != NULL && t->type == TOKEN_FUNC) {
      acall = fscript_func_call_create(parser, t->token, t->size);
      return_value_if_fail(acall != NULL, RET_BAD_PARAMS);

      fscript_parser_unget_token(parser);
      fscript_parse_func(parser, acall);
      if (last == NULL) {
        parser->first = acall;
      } else {
        last->next = acall;
      }
      last = acall;
    }
    fscript_parser_skip_seperators(parser);
    c = fscript_parser_get_char(parser);
    if (c) {
      fscript_parser_unget_char(parser, c);
    } else {
      break;
    }
  }

  return ret;
}

fscript_t* fscript_create(object_t* obj, const char* script) {
  ret_t ret = RET_OK;
  fscript_t* fscript = NULL;
  fscript_parser_t parser;
  return_value_if_fail(obj != NULL && script != NULL, NULL);

  fscript_parser_init(&parser, obj, script);
  ret = fscript_parse(&parser);
  if (ret == RET_OK) {
    fscript = fscript_create_impl(&parser);
    fscript_parser_deinit(&parser);
  } else {
    log_warn("parser error:%s\n", script);
    fscript_parser_deinit(&parser);
  }

  return fscript;
}

ret_t fscript_eval(object_t* obj, const char* script, value_t* result) {
  value_t v;
  ret_t ret = RET_OK;
  fscript_t* fscript = fscript_create(obj, script);
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  value_set_int(&v, 0);
  if (fscript_exec(fscript, &v) == RET_OK && result != NULL) {
    value_deep_copy(result, &v);
  }
  value_reset(&v);
  fscript_destroy(fscript);

  return ret;
}

static ret_t func_sum(object_t* obj, fscript_args_t* args, value_t* result) {
  double v = 0;
  uint32_t i = 0;

  for (i = 0; i < args->size; i++) {
    v += value_double(args->args + i);
  }
  value_set_double(result, v);

  return RET_OK;
}

static ret_t func_int(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_int(result, value_int(args->args));
  return RET_OK;
}

static ret_t func_float(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args));
  return RET_OK;
}

static ret_t func_str(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  str_from_value(str, args->args);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_join(object_t* obj, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  char buff[64];
  str_t* str = args->str;
  value_set_bool(result, FALSE);
  return_value_if_fail(args->size > 1, RET_BAD_PARAMS);

  str_set(str, "");
  value_set_bool(result, TRUE);
  for (i = 1; i < args->size; i++) {
    if (i > 1) {
      str_append(str, value_str(args->args));
    }
    str_append(str, value_str_ex(args->args + i, buff, sizeof(buff)));
  }

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_if(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 3, RET_BAD_PARAMS);

  if (value_bool(args->args)) {
    value_deep_copy(result, args->args + 1);
  } else {
    value_deep_copy(result, args->args + 2);
  }

  return RET_OK;
}

static ret_t func_set(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  if (object_set_prop(obj, value_str(args->args), args->args + 1) == RET_OK) {
    value_set_bool(result, TRUE);
  } else {
    value_set_bool(result, FALSE);
  }

  return RET_OK;
}

static ret_t func_print(object_t* obj, fscript_args_t* args, value_t* result) {
  char buff[64];
  uint32_t i = 0;
  value_set_bool(result, TRUE);
  for (i = 0; i < args->size; i++) {
    log_info("%s ", value_str_ex(args->args + i, buff, sizeof(buff)));
  }
  log_info("\n");

  return RET_OK;
}

static ret_t func_iformat(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  const char* format = NULL;
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  format = value_str(args->args);
  return_value_if_fail(format != NULL, RET_BAD_PARAMS);

  tk_snprintf(str->str, str->capacity - 1, format, value_int(args->args + 1));
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_fformat(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  const char* format = NULL;
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  format = value_str(args->args);
  return_value_if_fail(format != NULL, RET_BAD_PARAMS);

  tk_snprintf(str->str, str->capacity - 1, format, value_double(args->args + 1));
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_and(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_bool(result, value_bool(args->args) && value_bool(args->args + 1));

  return RET_OK;
}

static ret_t func_or(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_bool(result, value_bool(args->args) || value_bool(args->args + 1));

  return RET_OK;
}

static ret_t func_not(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, !value_bool(args->args));

  return RET_OK;
}

static ret_t func_bit_and(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) & value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_or(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) | value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_nor(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) ^ value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_not(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, ~value_uint32(args->args));

  return RET_OK;
}

static ret_t func_div(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) / value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_mod(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_int(result, value_int(args->args) % value_int(args->args + 1));

  return RET_OK;
}

static ret_t func_mul(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) * value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_sub(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) - value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_pow(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, pow(value_double(args->args), value_double(args->args + 1)));

  return RET_OK;
}

static ret_t func_sqrt(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, sqrt(value_double(args->args)));

  return RET_OK;
}

static ret_t func_random(object_t* obj, fscript_args_t* args, value_t* result) {
  long v = random();
  if (args->size == 2) {
    int32_t min = value_int(args->args);
    int32_t max = value_int(args->args + 1);
    if (min < max) {
      v = min + v % (max - min);
    }
  }

  value_set_int(result, v);

  return RET_OK;
}

static ret_t func_time_now(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_uint64(result, time_now_s());

  return RET_OK;
}

static ret_t func_time_now_ms(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_uint64(result, time_now_ms());

  return RET_OK;
}

static ret_t func_time_now_us(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_uint64(result, time_now_us());

  return RET_OK;
}

static ret_t func_le(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) <= 0);
  } else {
    value_set_bool(result, value_double(args->args) <= value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_less(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) < 0);
  } else {
    value_set_bool(result, value_double(args->args) < value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_ge(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) >= 0);
  } else {
    value_set_bool(result, value_double(args->args) >= value_double(args->args + 1));
  }
  return RET_OK;
}

static ret_t func_great(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) > 0);
  } else {
    value_set_bool(result, value_double(args->args) > value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_eq(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_eq(value_str(args->args), value_str(args->args + 1)));
  } else {
    value_set_bool(result, tk_fequal(value_double(args->args), value_double(args->args + 1)));
  }

  return RET_OK;
}

static ret_t func_sin(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, sin(value_double(args->args)));

  return RET_OK;
}

static ret_t func_cos(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, cos(value_double(args->args)));

  return RET_OK;
}

static ret_t func_tan(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, tan(value_double(args->args)));

  return RET_OK;
}

static ret_t func_asin(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, asin(value_double(args->args)));

  return RET_OK;
}

static ret_t func_acos(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, acos(value_double(args->args)));

  return RET_OK;
}

static ret_t func_atan(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, atan(value_double(args->args)));

  return RET_OK;
}

static ret_t func_min(object_t* obj, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  value_set_double(result, tk_min(v1, v2));

  return RET_OK;
}

static ret_t func_max(object_t* obj, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  value_set_double(result, tk_max(v1, v2));

  return RET_OK;
}

static ret_t func_clamp(object_t* obj, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  double v3 = 0;
  return_value_if_fail(args->size == 3, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  v3 = value_double(args->args + 2);
  value_set_double(result, tk_clamp(v1, v2, v3));

  return RET_OK;
}

static ret_t func_abs(object_t* obj, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  return_value_if_fail(args->size == 3, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  value_set_double(result, tk_abs(v1));

  return RET_OK;
}

static ret_t func_len(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  value_set_int(result, tk_strlen(value_str(args->args)));

  return RET_OK;
}

static ret_t func_toupper(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_to_upper(str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_tolower(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_to_lower(str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_trim(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_trim(str, " \t\r\n");
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_substr(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  const char* org = NULL;
  uint32_t total_len = 0;
  uint32_t from = 0;
  uint32_t len = 0;
  return_value_if_fail(args->size == 3, RET_BAD_PARAMS);
  org = value_str(args->args);
  from = value_uint32(args->args + 1);
  len = value_uint32(args->args + 2);
  total_len = tk_strlen(org);
  return_value_if_fail(org != NULL, RET_BAD_PARAMS);

  if (from < total_len) {
    len = tk_min(len, (total_len - from));
    str_set_with_len(str, org + from, len);
  } else {
    str_set(str, "");
  }

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_replace(object_t* obj, fscript_args_t* args, value_t* result) {
  str_t* str = args->str;
  const char* org = NULL;
  const char* replace = NULL;
  const char* replace_with = NULL;
  return_value_if_fail(args->size == 3, RET_BAD_PARAMS);

  org = value_str(args->args);
  replace = value_str(args->args + 1);
  replace_with = value_str(args->args + 2);
  return_value_if_fail(org != NULL && replace != NULL && replace_with != NULL, RET_BAD_PARAMS);

  str_set(str, org);
  str_replace(str, replace, replace_with);

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_contains(object_t* obj, fscript_args_t* args, value_t* result) {
  const char* org = NULL;
  const char* target = NULL;
  value_set_bool(result, FALSE);
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  org = value_str(args->args);
  target = value_str(args->args + 1);
  return_value_if_fail(org != NULL && target != NULL, RET_BAD_PARAMS);
  value_set_bool(result, strstr(org, target) != NULL);

  return RET_OK;
}

static ret_t func_exec(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);
  value_set_bool(result,
                 object_exec(obj, value_str(args->args), value_str(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_noop(object_t* obj, fscript_args_t* args, value_t* result) {
  value_set_bool(result, TRUE);

  return RET_OK;
}

static ret_t func_unset(object_t* obj, fscript_args_t* args, value_t* result) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, object_exec(obj, OBJECT_CMD_REMOVE, value_str(args->args)) == RET_OK);

  return RET_OK;
}

typedef struct _func_entry_t {
  const char* name;
  fscript_func_t func;
  uint32_t max_args_nr;
} func_entry_t;

static const func_entry_t s_builtin_funcs[] = {
    {"print", func_print, 4},
    {"set", func_set, 2},
    {"max", func_max, 2},
    {"min", func_min, 2},
    {"==", func_eq, 2},
    {">=", func_ge, 2},
    {">", func_great, 2},
    {"<=", func_le, 2},
    {"<", func_less, 2},
    {"!", func_not, 1},
    {"||", func_or, 2},
    {"exec", func_exec, 2},
    {"join", func_join, 8},
    {"if", func_if, 3},
    {"int", func_int, 1},
    {"float", func_float, 1},
    {"iformat", func_iformat, 2},
    {"fformat", func_fformat, 2},
    {"time_now", func_time_now, 0},
    {"unset", func_unset, 1},
    {"str", func_str, 1},
    {"sub", func_sub, 2},
    {"substr", func_substr, 3},
    {"sum", func_sum, 8},
    {"tolower", func_tolower, 1},
    {"toupper", func_toupper, 1},
    {"trim", func_trim, 1},
    {"&&", func_and, 2},
    {"abs", func_abs, 1},
    {"acos", func_acos, 1},
    {"and", func_and, 2},
    {"asin", func_asin, 1},
    {"atan", func_atan, 1},
    {"clamp", func_clamp, 3},
    {"contains", func_contains, 2},
    {"cos", func_cos, 1},
    {"div", func_div, 2},
    {"eq", func_eq, 2},
    {"ge", func_ge, 2},
    {"great", func_great, 2},
    {"le", func_le, 2},
    {"len", func_len, 1},
    {"less", func_less, 2},
    {"mul", func_mul, 2},
    {"noop", func_noop, 0},
    {"not", func_not, 1},
    {"or", func_or, 2},
    {"pow", func_pow, 2},
    {"random", func_random, 2},
    {"replace", func_replace, 3},
    {"sin", func_sin, 1},
    {"sqrt", func_sqrt, 1},
    {"tan", func_tan, 1},
    {"time_now_ms", func_time_now_ms, 0},
    {"time_now_us", func_time_now_us, 0},
    {"&", func_bit_and, 2},
    {"^", func_bit_nor, 1},
    {"~", func_bit_not, 1},
    {"|", func_bit_or, 2},
    {"/", func_div, 2},
    {"%", func_mod, 2},
    {"*", func_mul, 2},
    {"-", func_sub, 2},
    {"+", func_sum, 8},
};

static fscript_func_call_t* fscript_func_call_create(fscript_parser_t* parser, const char* name,
                                                     uint32_t size) {
  uint32_t i = 0;
  fscript_func_t func = NULL;
  char func_name[TK_NAME_LEN + 1];
  char full_func_name[2 * TK_NAME_LEN + 1];
  fscript_func_call_t* call = TKMEM_ZALLOC(fscript_func_call_t);
  return_value_if_fail(call != NULL, NULL);

  tk_strncpy(func_name, name, tk_min(size, TK_NAME_LEN));
  for (i = 0; i < ARRAY_SIZE(s_builtin_funcs); i++) {
    const func_entry_t* iter = s_builtin_funcs + i;
    if (tk_str_eq(iter->name, func_name)) {
      call->func = iter->func;
      func_args_init(&(call->args), iter->max_args_nr);
      return call;
    }
  }

  tk_snprintf(full_func_name, sizeof(full_func_name) - 1, "%s%s", STR_FSCRIPT_FUNCTION_PREFIX,
              func_name);
  func = (fscript_func_t)object_get_prop_pointer(parser->obj, full_func_name);

  if (func != NULL) {
    call->func = func;
    func_args_init(&(call->args), 2);

    return call;
  } else {
    TKMEM_FREE(call);
    log_warn("not found func:%s\n", func_name);
  }
  return NULL;
}

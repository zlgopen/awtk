/**
 * File:   fscript.c
 * Author: AWTK Develop Team
 * Brief:  a simple functional script language
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-11-09 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "base/types_def.h"
#ifndef AWTK_LITE
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "tkc/object_default.h"

struct _fscript_func_call_t {
  fscript_func_t func;
  fscript_args_t args;
  fscript_func_call_t* next;
  uint16_t row;
  uint16_t col;
};

#define VALUE_TYPE_JSCRIPT_ID 128
#define VALUE_TYPE_JSCRIPT_FUNC VALUE_TYPE_JSCRIPT_ID + 1

static ret_t func_if(fscript_t* fscript, fscript_args_t* args, value_t* result);
static ret_t func_while(fscript_t* fscript, fscript_args_t* args, value_t* result);
static ret_t func_set(fscript_t* fscript, fscript_args_t* args, value_t* result);
static ret_t func_unset(fscript_t* fscript, fscript_args_t* args, value_t* result);
static ret_t fscript_exec_func(fscript_t* fscript, fscript_func_call_t* iter, value_t* result);

ret_t fscript_set_error(fscript_t* fscript, ret_t code, const char* func, const char* message) {
  fscript->error_code = code;
  fscript->error_func = fscript->curr;
  fscript->error_message = tk_str_copy(fscript->error_message, message);
  if (code != RET_OK) {
    log_debug("(%d:%d): %s code=%d %s\n", fscript->curr->row, fscript->curr->col, func, code,
              message);
  }
  return RET_OK;
}

static value_t* value_set_func(value_t* v, fscript_func_call_t* func) {
  value_set_pointer(v, func);
  v->type = VALUE_TYPE_JSCRIPT_FUNC;
  return v;
}

static fscript_func_call_t* value_func(const value_t* v) {
  return_value_if_fail(v->type == VALUE_TYPE_JSCRIPT_FUNC, NULL);

  return (fscript_func_call_t*)(v->value.ptr);
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

static ret_t func_args_push(fscript_args_t* args, value_t* v) {
  return_value_if_fail(v != NULL && func_args_extend(args) == RET_OK, RET_BAD_PARAMS);
  args->args[args->size++] = *v;
  v->free_handle = FALSE;
  value_reset(v);

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

static ret_t fscript_func_call_destroy(fscript_func_call_t* call);
static ret_t func_args_deinit(fscript_args_t* args) {
  uint32_t i = 0;
  for (i = 0; i < args->size; i++) {
    value_t* v = args->args + i;
    if (v->type == VALUE_TYPE_JSCRIPT_FUNC) {
      fscript_func_call_destroy(value_func(v));
    } else {
      v->type = v->type == VALUE_TYPE_JSCRIPT_ID ? VALUE_TYPE_STRING : v->type;
    }
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
    func_args_deinit(&(iter->args));
    TKMEM_FREE(iter);
    iter = next;
  }

  return RET_OK;
}

typedef enum _token_type_t {
  TOKEN_ID = 1,
  TOKEN_FUNC,
  TOKEN_STR,
  TOKEN_NUMBER,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_COMMA,
  TOKEN_SEMICOLON,
  /*for expr*/
  TOKEN_QUESTION,
  TOKEN_COLON,
  TOKEN_EOF
} token_type_t;

typedef struct _token_t {
  const char* token;
  token_type_t type;
  uint32_t size;
  bool_t valid;
} token_t;

typedef struct _fscript_parser_t {
  object_t* obj;
  token_t token;
  const char* str;
  const char* cursor;

  char c;
  str_t temp;
  uint16_t row;
  uint16_t col;
  fscript_func_call_t* first;
  fscript_parser_error_t* error;
} fscript_parser_t;

static ret_t fexpr_parse(fscript_parser_t* parser, value_t* result);
static ret_t fscript_parse_statements(fscript_parser_t* parser, fscript_func_call_t* acall);
static fscript_func_call_t* fscript_func_call_create(fscript_parser_t* parser, const char* name,
                                                     uint32_t size);
static inline bool_t is_fast_var(const char* name) {
  char c = name[1];
  int32_t index = *name - 'a';

  if (c != '\0' && c != '.') {
    return FALSE;
  }

  if (index < 0 || index >= FSCRIPT_FAST_VAR_NR) {
    return FALSE;
  }

  return TRUE;
}

static value_t* fscript_get_fast_var(fscript_t* fscript, const char* name) {
  int32_t index = name[0] - 'a';
  if (index >= 0 && index < ARRAY_SIZE(fscript->fast_vars)) {
    return fscript->fast_vars + index;
  }

  return NULL;
}

static ret_t fscript_get_var(fscript_t* fscript, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  value_set_str(value, NULL);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  if (*name == '$') {
    name += 1;
  }

  if (is_fast_var(name)) {
    value_t* var = fscript_get_fast_var(fscript, name);
    if (name[1] == '.') {
      if (var->type == VALUE_TYPE_OBJECT) {
        object_t* obj = value_object(var);
        ret = object_get_prop(obj, name + 2, value);
      } else {
        ret = object_get_prop(fscript->obj, name, value);
      }
    } else {
      ret = value_copy(value, var);
    }
  } else {
    ret = object_get_prop(fscript->obj, name, value);
  }

  return ret;
}

static ret_t fscript_set_var(fscript_t* fscript, const char* name, const value_t* value) {
  ret_t ret = RET_FAIL;
  if (is_fast_var(name)) {
    value_t* var = fscript_get_fast_var(fscript, name);
    if (name[1] == '.') {
      if (var->type == VALUE_TYPE_OBJECT) {
        object_t* obj = value_object(var);
        ret = object_set_prop(obj, name + 2, value);
      } else {
        ret = object_set_prop(fscript->obj, name, value);
      }
    } else {
      value_reset(var);
      ret = value_deep_copy(var, value);
    }
  } else {
    ret = object_set_prop(fscript->obj, name, value);
  }

  return ret;
}

static ret_t fscript_eval_arg(fscript_t* fscript, fscript_func_call_t* iter, uint32_t i,
                              value_t* d) {
  value_t v;
  value_t* s = iter->args.args + i;
  int32_t save_type = s->type;
  value_set_str(&v, NULL);
  value_set_str(d, NULL);
  if (s->type == VALUE_TYPE_JSCRIPT_ID) {
    s->type = VALUE_TYPE_STRING;
    if ((iter->func == func_set || iter->func == func_unset) && i == 0) {
      value_copy(d, s); /*func_set accept id/str as first param*/
    } else {
      const char* name = value_str(s);
      if (fscript->while_count > 0) {
        if (tk_str_eq(name, "break")) {
          fscript->breaked = TRUE;
          s->type = save_type;
          return RET_OK;
        } else if (tk_str_eq(name, "continue")) {
          fscript->continued = TRUE;
          s->type = save_type;
          return RET_OK;
        }
      } else if (tk_str_eq(name, "return")) {
        fscript->returned = TRUE;
        s->type = save_type;
        value_set_int(d, 0);
        return RET_OK;
      }

      if (fscript_get_var(fscript, name, d) != RET_OK) {
        if (name == NULL || *name != '$') {
          /*if it is not $var, consider id as string*/
          value_copy(d, s);
        }
      }
    }
  } else if (s->type == VALUE_TYPE_JSCRIPT_FUNC) {
    fscript_exec_func(fscript, value_func(s), d);
  } else {
    value_copy(d, s);
  }
  s->type = save_type;

  return RET_OK;
}

static ret_t fscript_exec_if(fscript_t* fscript, fscript_func_call_t* iter, value_t* result) {
  value_t condition;
  return_value_if_fail(iter->args.size >= 2, RET_FAIL);
  value_set_int(&condition, 0);

  fscript_eval_arg(fscript, iter, 0, &condition);
  if (value_bool(&condition)) {
    fscript_eval_arg(fscript, iter, 1, result);
  } else if (iter->args.size > 2) {
    fscript_eval_arg(fscript, iter, 2, result);
  } else {
    value_set_int(result, 0);
  }

  return RET_OK;
}

static ret_t fscript_exec_while(fscript_t* fscript, fscript_func_call_t* iter, value_t* result) {
  value_t condition;
  bool_t done = FALSE;
  return_value_if_fail(iter->args.size > 1, RET_FAIL);

  fscript->while_count++;
  value_set_int(&condition, 0);
  while (!done && fscript_eval_arg(fscript, iter, 0, &condition) == RET_OK &&
         value_bool(&condition)) {
    uint32_t i = 1;
    for (i = 1; i < iter->args.size; i++) {
      value_reset(result);
      fscript_eval_arg(fscript, iter, i, result);
      if (fscript->breaked) {
        done = TRUE;
        fscript->breaked = FALSE;
        break;
      } else if (fscript->continued) {
        fscript->continued = FALSE;
        break;
      }
    }
  }
  fscript->while_count--;

  return RET_OK;
}

static ret_t fscript_exec_core_func(fscript_t* fscript, fscript_func_call_t* iter,
                                    value_t* result) {
  if (iter->func == func_if) {
    return fscript_exec_if(fscript, iter, result);
  } else if (iter->func == func_while) {
    return fscript_exec_while(fscript, iter, result);
  }

  return RET_NOT_FOUND;
}

static ret_t fscript_exec_ext_func(fscript_t* fscript, fscript_func_call_t* iter, value_t* result) {
  value_t v;
  uint32_t i = 0;
  ret_t ret = RET_OK;
  fscript_args_t args;

  value_set_int(&v, 0);
  func_args_init(&args, iter->args.size);
  args.size = iter->args.size;
  return_value_if_fail((args.args != NULL || args.size == 0), RET_OOM);
  for (i = 0; i < iter->args.size; i++) {
    ret = fscript_eval_arg(fscript, iter, i, args.args + i);
    if (fscript->breaked || fscript->continued || fscript->returned) {
      value_set_int(result, 0);
      func_args_deinit(&args);
      return RET_OK;
    }
  }

  value_set_int(result, 0);
  ret = iter->func(fscript, &args, result);
  func_args_deinit(&args);

  return ret;
}

static ret_t fscript_exec_func(fscript_t* fscript, fscript_func_call_t* iter, value_t* result) {
  fscript->curr = iter;
  result->type = VALUE_TYPE_INVALID;
  if (fscript_exec_core_func(fscript, iter, result) == RET_NOT_FOUND) {
    return_value_if_fail(fscript_exec_ext_func(fscript, iter, result) == RET_OK, RET_FAIL);
  }

  return RET_OK;
}

ret_t fscript_exec(fscript_t* fscript, value_t* result) {
  fscript_func_call_t* iter = NULL;
  return_value_if_fail(fscript != NULL, RET_FAIL);

  value_set_str(result, NULL);
  iter = fscript->first;
  while (iter != NULL) {
    return_value_if_fail(iter->func != NULL, RET_FAIL);
    value_reset(result);
    return_value_if_fail(fscript_exec_func(fscript, iter, result) == RET_OK, RET_FAIL);
    if (fscript->returned) {
      break;
    }
    iter = iter->next;
  }

  return RET_OK;
}

ret_t fscript_destroy(fscript_t* fscript) {
  uint32_t i = 0;
  return_value_if_fail(fscript != NULL, RET_FAIL);

  str_reset(&(fscript->str));
  TKMEM_FREE(fscript->error_message);
  fscript_func_call_destroy(fscript->first);
  for (i = 0; i < ARRAY_SIZE(fscript->fast_vars); i++) {
    value_reset(fscript->fast_vars + i);
  }
  memset(fscript, 0x00, sizeof(fscript_t));
  TKMEM_FREE(fscript);

  return RET_OK;
}

static ret_t fscript_parser_init(fscript_parser_t* parser, object_t* obj, const char* str,
                                 fscript_parser_error_t* error) {
  memset(parser, 0x00, sizeof(fscript_parser_t));

  parser->obj = obj;
  parser->str = str;
  parser->cursor = str;
  parser->error = error;
  str_init(&(parser->temp), 64);

  return RET_OK;
}

static ret_t fscript_parser_deinit(fscript_parser_t* parser) {
  str_reset(&(parser->temp));
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

  if (parser->error != NULL) {
    parser->error->row = parser->row;
    parser->error->col = parser->col;
    parser->error->offset = parser->cursor - parser->str;
    parser->error->message = tk_str_copy(parser->error->message, str);
    parser->error->token = tk_str_copy(parser->error->token, parser->token.token);
  }

  log_warn("code: \"%s\"\n", parser->str);
  log_warn("token: \"%s\"\n", parser->token.token);
  log_warn("at line(%u) col (%u): %s\n", parser->row, parser->col, str);

  return RET_OK;
}

static ret_t fscript_parser_skip_seperators(fscript_parser_t* parser) {
  char c = '\0';
  do {
    c = fscript_parser_get_char(parser);
  } while (isspace(c) || (int)c < 0);
  fscript_parser_unget_char(parser, c);

  return RET_OK;
}

static ret_t fscript_parser_skip_line_comment(fscript_parser_t* parser) {
  char c = '\0';
  do {
    c = fscript_parser_get_char(parser);
  } while (c != '\0' && c != '\r' && c != '\n');

  return RET_OK;
}

static ret_t fscript_parser_skip_block_comment(fscript_parser_t* parser) {
  char c = '\0';
  do {
    c = fscript_parser_get_char(parser);
    if (c == '*' && parser->cursor[0] == '/') {
      c = fscript_parser_get_char(parser);
      break;
    }
  } while (c != '\0');

  return RET_OK;
}

static ret_t fscript_parser_skip_seperators_and_comments(fscript_parser_t* parser) {
  char c = '\0';

  while (TRUE) {
    fscript_parser_skip_seperators(parser);
    c = fscript_parser_get_char(parser);
    if (c == '/') {
      if (parser->cursor[0] == '/') {
        c = fscript_parser_get_char(parser);
        fscript_parser_skip_line_comment(parser);
      } else if (parser->cursor[0] == '*') {
        c = fscript_parser_get_char(parser);
        fscript_parser_skip_block_comment(parser);
      } else {
        break;
      }
    } else {
      break;
    }
  }
  fscript_parser_unget_char(parser, c);

  return RET_OK;
}

#define TOKEN_INIT(t, ttype, str) \
  t->type = ttype;                \
  t->token = str->str;            \
  t->size = str->size;

static ret_t fscript_parser_parse_str(fscript_parser_t* parser, char quota) {
  char c = '\0';
  bool_t escape = FALSE;
  str_t* str = &(parser->temp);
  token_t* t = &(parser->token);

  str_set(str, "");
  do {
    c = fscript_parser_get_char(parser);
    if (escape) {
      if (c == 'r') {
        str_append_char(str, '\r');
      } else if (c == 'n') {
        str_append_char(str, '\n');
      } else if (c == 't') {
        str_append_char(str, '\t');
      } else {
        str_append_char(str, c);
      }
      escape = FALSE;
      continue;
    }

    if (c == quota || c == '\0') {
      break;
    } else if (c == '\\') {
      escape = TRUE;
      continue;
    }
    str_append_char(str, c);
  } while (TRUE);

  TOKEN_INIT(t, TOKEN_STR, str);
  if (c != '\0' && c != quota) {
    fscript_parser_unget_char(parser, c);
  }

  return RET_OK;
}

static ret_t fscript_parser_parse_id_or_number(fscript_parser_t* parser, token_type_t def_type) {
  char c = '\0';
  str_t* str = &(parser->temp);
  token_t* t = &(parser->token);
  c = fscript_parser_get_char(parser);

  if (c) {
    str_set_with_len(str, &c, 1);
  }

  do {
    c = fscript_parser_get_char(parser);
    if (isxdigit(c) || isdigit(c) || isalpha(c) || c == '.' || c == '_' || c == '[' || c == ']' ||
        c == '#') {
      str_append_char(str, c);
    } else {
      break;
    }
  } while (TRUE);

  if (isspace(c)) {
    fscript_parser_skip_seperators_and_comments(parser);
    c = fscript_parser_get_char(parser);
  }

  str_trim(str, " \t\r\n");
  TOKEN_INIT(t, (c == '(' ? TOKEN_FUNC : def_type), str);
  if (c != '\0') {
    fscript_parser_unget_char(parser, c);
  }

  return RET_OK;
}

static token_t* fscript_parser_get_token_ex(fscript_parser_t* parser, bool_t operator) {
  char c = 0;
  str_t* str = &(parser->temp);
  token_t* t = &(parser->token);

  if (t->valid) {
    t->valid = FALSE;
    return t;
  }

  fscript_parser_skip_seperators_and_comments(parser);
  c = fscript_parser_get_char(parser);

  str_set_with_len(str, &c, 1);
  t->token = str->str;
  t->size = 1;
  switch (c) {
    case '\0': {
      t->type = TOKEN_EOF;
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
    case '{': {
      t->type = TOKEN_LBRACKET;
      return t;
    }
    case '}': {
      t->type = TOKEN_RBRACKET;
      return t;
    }
    case ',': {
      t->type = TOKEN_COMMA;
      return t;
    }
    case ';': {
      t->type = TOKEN_SEMICOLON;
      return t;
    }
    case '?': {
      t->type = TOKEN_QUESTION;
      return t;
    }
    case ':': {
      t->type = TOKEN_COLON;
      return t;
    }
    case '*':
    case '/':
    case '%':
    case '^':
    case '#': {
      t->type = TOKEN_FUNC;
      return t;
    }
    case '>':
    case '<':
    case '=': {
      c = fscript_parser_get_char(parser);
      if (str->str[0] == c || c == '=') {
        str_append_char(str, c);
      } else {
        fscript_parser_unget_char(parser, c);
      }
      t->type = TOKEN_FUNC;
      t->size = str->size;
      return t;
    }
    case '~':
    case '!': {
      c = fscript_parser_get_char(parser);
      if (c == '=') {
        str_append_char(str, c);
      } else {
        fscript_parser_unget_char(parser, c);
        do {
          fscript_parser_skip_seperators_and_comments(parser);
          c = fscript_parser_get_char(parser);
          if (c == str->str[0]) {
            str_append_char(str, c);
          } else {
            fscript_parser_unget_char(parser, c);
            break;
          }
        } while (TRUE);
      }
      t->type = TOKEN_FUNC;
      t->size = str->size;
      return t;
    }
    case '|':
    case '&': {
      c = fscript_parser_get_char(parser);
      if (c == str->str[0]) {
        str_append_char(str, c);
      } else {
        fscript_parser_unget_char(parser, c);
      }

      t->type = TOKEN_FUNC;
      t->size = str->size;
      return t;
    }
    case '\"':
    case '\'': {
      fscript_parser_parse_str(parser, c);
      return t;
    }
    default: {
      fscript_parser_unget_char(parser, c);
      if (c == '+' || c == '-') {
        if (operator) {
          t->type = TOKEN_FUNC;
          fscript_parser_get_char(parser);
        } else {
          fscript_parser_parse_id_or_number(parser, TOKEN_NUMBER);
        }
      } else if (isdigit(c)) {
        fscript_parser_parse_id_or_number(parser, TOKEN_NUMBER);
      } else {
        fscript_parser_parse_id_or_number(parser, TOKEN_ID);
      }
      return t;
    }
  }
}

static token_t* fscript_parser_get_token(fscript_parser_t* parser) {
  return fscript_parser_get_token_ex(parser, FALSE);
}

static ret_t fscript_parser_expect_token(fscript_parser_t* parser, token_type_t type,
                                         const char* msg) {
  token_t* t = fscript_parser_get_token(parser);
  if (t == NULL || t->type != type) {
    fscript_parser_set_error(parser, msg);
    return RET_FAIL;
  }

  return RET_OK;
}

static ret_t fscript_parser_unget_token(fscript_parser_t* parser) {
  if (parser->token.type != TOKEN_EOF) {
    parser->token.valid = TRUE;
  }
  return RET_OK;
}

static ret_t token_to_value(token_t* t, value_t* v) {
  if (t->type == TOKEN_NUMBER) {
    char number[64];
    tk_strncpy_s(number, sizeof(number) - 1, t->token, t->size);
    if (strchr(number, '.') != NULL) {
      value_set_double(v, tk_atof(number));
    } else {
      uint64_t vv = 0;
      const char* str = number;
      bool_t has_minus = FALSE;
      if (*str == '-') {
        has_minus = TRUE;
        str++;
      }

      vv = tk_atoul(str);
      if (vv < INT_MAX) {
        if (has_minus) {
          value_set_int32(v, -vv);
        } else {
          value_set_int32(v, vv);
        }
      } else if (vv < UINT32_MAX) {
        if (has_minus) {
          value_set_int64(v, -vv);
        } else {
          value_set_uint32(v, vv);
        }
      } else {
        if (has_minus) {
          value_set_int64(v, -vv);
        } else {
          value_set_uint64(v, vv);
        }
      }
    }
  } else if (t->type == TOKEN_STR) {
    value_dup_str_with_len(v, t->token, t->size);
  } else if (t->type == TOKEN_ID) {
    if (t->token[0] == 't' && strncmp(t->token, "true", 4) == 0) {
      value_set_bool(v, TRUE);
    } else if (t->token[0] == 'f' && strncmp(t->token, "false", 5) == 0) {
      value_set_bool(v, FALSE);
    } else {
      value_dup_str_with_len(v, t->token, t->size);
      v->type = VALUE_TYPE_JSCRIPT_ID;
    }
  } else {
    return RET_FAIL;
  }

  return RET_OK;
}

ret_t fscript_eval(object_t* obj, const char* script, value_t* result) {
  value_t v;
  ret_t ret = RET_OK;
  fscript_t* fscript = fscript_create(obj, script);
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  if (fscript_exec(fscript, &v) == RET_OK && result != NULL) {
    value_deep_copy(result, &v);
  }
  value_reset(&v);
  fscript_destroy(fscript);

  return ret;
}

/*expr parser*/
static ret_t fexpr_parse_block(fscript_parser_t* parser, fscript_func_call_t* acall) {
  value_t v;
  fscript_func_call_t* statements = fscript_func_call_create(parser, "expr", 4);
  return_value_if_fail(statements != NULL, RET_OOM);

  if (fscript_parse_statements(parser, statements) == RET_OK) {
    func_args_push(&(acall->args), value_set_func(&v, statements));
  } else {
    fscript_func_call_destroy(statements);
  }

  fscript_parser_expect_token(parser, TOKEN_RBRACKET, "expect \"}\"");

  return RET_OK;
}

static ret_t fexpr_parse_function(fscript_parser_t* parser, value_t* result);

static ret_t fexpr_parse_if(fscript_parser_t* parser, fscript_func_call_t* acall) {
  token_t* t = NULL;
  fexpr_parse_block(parser, acall);
  t = fscript_parser_get_token(parser);

  if (t != NULL && t->type == TOKEN_ID && tk_str_eq(t->token, "else")) {
    t = fscript_parser_get_token(parser);
    if (t != NULL && t->type == TOKEN_FUNC && tk_str_eq(t->token, "if")) {
      value_t result;
      fscript_parser_unget_token(parser);
      fexpr_parse_function(parser, &result);
      func_args_push(&(acall->args), &result);
      return RET_OK;
    } else {
      fscript_parser_unget_token(parser);
      fscript_parser_expect_token(parser, TOKEN_LBRACKET, "expect \"{\"");
      return fexpr_parse_block(parser, acall);
    }
  } else {
    fscript_parser_unget_token(parser);
    return RET_OK;
  }
}

static ret_t fexpr_parse_while(fscript_parser_t* parser, fscript_func_call_t* acall) {
  return fexpr_parse_block(parser, acall);
}

static ret_t fexpr_parse_function(fscript_parser_t* parser, value_t* result) {
  value_t v;
  fscript_args_t* args = NULL;
  token_t* t = fscript_parser_get_token(parser);
  fscript_func_call_t* acall = NULL;
  if (t->token[0] == '-' && t->size == 1) {
    acall = fscript_func_call_create(parser, "minus", 5);
  } else {
    acall = fscript_func_call_create(parser, t->token, t->size);
  }
  value_set_func(result, acall);
  return_value_if_fail(acall != NULL, RET_BAD_PARAMS);

  args = &(acall->args);
  return_value_if_fail(fscript_parser_expect_token(parser, TOKEN_LPAREN, "expect \"(\"") == RET_OK,
                       RET_FAIL);

  do {
    t = fscript_parser_get_token(parser);
    if (t == NULL) {
      fscript_parser_set_error(parser, "expect \")\"");
      return RET_FAIL;
    }

    if (t->type == TOKEN_RPAREN) {
      break;
    } else {
      fscript_parser_unget_token(parser);
    }
    value_set_int(&v, 0);
    return_value_if_fail(fexpr_parse(parser, &v) == RET_OK, RET_FAIL);
    func_args_push(args, &v);
    t = fscript_parser_get_token(parser);

    if (t == NULL) {
      fscript_parser_set_error(parser, "expect \")\"");
      return RET_FAIL;
    } else if (t->type == TOKEN_RPAREN) {
      break;
    }
    fscript_parser_unget_token(parser);
    fscript_parser_expect_token(parser, TOKEN_COMMA, "expect \",\"");
  } while (TRUE);

  if (acall->func == func_if && acall->args.size == 1) {
    if (fscript_parser_expect_token(parser, TOKEN_LBRACKET, "expect \"{\"") == RET_OK) {
      fexpr_parse_if(parser, acall);
    }
  } else if (acall->func == func_while && acall->args.size == 1) {
    if (fscript_parser_expect_token(parser, TOKEN_LBRACKET, "expect \"{\"") == RET_OK) {
      fexpr_parse_while(parser, acall);
    }
  }

  return RET_OK;
}

static ret_t fexpr_parse_term(fscript_parser_t* parser, value_t* result) {
  ret_t ret = RET_OK;
  token_t* t = fscript_parser_get_token(parser);
  value_set_int(result, 0);
  return_value_if_fail(t != NULL, RET_BAD_PARAMS);

  if (t->type == TOKEN_NUMBER || t->type == TOKEN_ID || t->type == TOKEN_STR) {
    ret = token_to_value(t, result);
  } else if (t->type == TOKEN_FUNC) {
    fscript_parser_unget_token(parser);
    ret = fexpr_parse_function(parser, result);
  } else if (t->type == TOKEN_LPAREN) {
    ret = fexpr_parse(parser, result);
    return_value_if_fail(ret == RET_OK, ret);
    ret = fscript_parser_expect_token(parser, TOKEN_RPAREN, "expect )");
  } else {
    fscript_parser_unget_token(parser);
  }

  return ret;
}

static ret_t fexpr_parse_unary(fscript_parser_t* parser, value_t* result) {
  char c = '\0';
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;
  token_t* t = fscript_parser_get_token_ex(parser, FALSE);
  if (t == NULL || t->type == TOKEN_EOF) {
    return RET_OK;
  }

  c = t->token[0];
  if (c == '!' || c == '~') {
    value_t v;
    uint32_t i = 0;
    bool_t valid = FALSE;
    for (i = 0; i < t->size; i++) {
      if (t->token[i] != c) {
        fscript_parser_set_error(parser, "unexpected token");
        return RET_FAIL;
      } else {
        valid = !valid;
      }
    }

    if (valid) {
      acall = fscript_func_call_create(parser, &c, 1);
      return_value_if_fail(acall != NULL, RET_OOM);
      args = &(acall->args);
      value_set_func(result, acall);
      return_value_if_fail(fexpr_parse_term(parser, &v) == RET_OK, RET_FAIL);
      func_args_push(args, &v);
    } else {
      return_value_if_fail(fexpr_parse_term(parser, result) == RET_OK, RET_FAIL);
    }
  } else {
    fscript_parser_unget_token(parser);
    return_value_if_fail(fexpr_parse_term(parser, result) == RET_OK, RET_FAIL);
  }

  return RET_OK;
}

static ret_t fsexpr_parse_product(fscript_parser_t* parser, value_t* result) {
  value_t v;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;
  return_value_if_fail(fexpr_parse_unary(parser, result) == RET_OK, RET_FAIL);

  while (TRUE) {
    t = fscript_parser_get_token_ex(parser, TRUE);
    if (t == NULL || t->type != TOKEN_FUNC) {
      fscript_parser_unget_token(parser);
      break;
    }

    if (t->token[0] == '*' || t->token[0] == '/' || t->token[0] == '%') {
      acall = fscript_func_call_create(parser, t->token, t->size);
      return_value_if_fail(acall != NULL, RET_OOM);
      args = &(acall->args);
      func_args_push(args, result);
      value_set_func(result, acall);
      fexpr_parse_unary(parser, &v);
      func_args_push(args, &v);
    } else {
      fscript_parser_unget_token(parser);
      break;
    }
  }

  return RET_OK;
}

static ret_t fexpr_parse_sum(fscript_parser_t* parser, value_t* result) {
  value_t v;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;
  return_value_if_fail(fsexpr_parse_product(parser, result) == RET_OK, RET_FAIL);

  while (TRUE) {
    t = fscript_parser_get_token_ex(parser, TRUE);
    if (t == NULL || t->type != TOKEN_FUNC) {
      fscript_parser_unget_token(parser);
      break;
    }

    if (t->token[0] == '+' || t->token[0] == '-') {
      parser->token.valid = FALSE;
      acall = fscript_func_call_create(parser, t->token, t->size);
      return_value_if_fail(acall != NULL, RET_OOM);
      args = &(acall->args);
      func_args_push(args, result);
      value_set_func(result, acall);
      fsexpr_parse_product(parser, &v);
      func_args_push(args, &v);
    } else {
      fscript_parser_unget_token(parser);
      break;
    }
  }

  return RET_OK;
}

static ret_t fexpr_parse_compare(fscript_parser_t* parser, value_t* result) {
  value_t v;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;

  value_set_int(result, 0);
  return_value_if_fail(fexpr_parse_sum(parser, result) == RET_OK, RET_FAIL);

  t = fscript_parser_get_token_ex(parser, TRUE);
  if (t == NULL || t->type != TOKEN_FUNC || tk_str_eq(t->token, "=")) {
    fscript_parser_unget_token(parser);
    return RET_OK;
  }

  if (t->token[0] == '>' || t->token[0] == '<' || t->token[0] == '=' ||
      (t->token[0] == '!' && t->token[1] == '=')) {
    acall = fscript_func_call_create(parser, t->token, t->size);
    return_value_if_fail(acall != NULL, RET_OOM);
    args = &(acall->args);
    func_args_push(args, result);
    value_set_func(result, acall);
    fexpr_parse_sum(parser, &v);
    func_args_push(args, &v);
  } else {
    fscript_parser_unget_token(parser);
  }

  return RET_OK;
}

static ret_t fexpr_parse_logic(fscript_parser_t* parser, value_t* result) {
  value_t v;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;

  value_set_int(result, 0);
  return_value_if_fail(fexpr_parse_compare(parser, result) == RET_OK, RET_FAIL);

  while (TRUE) {
    t = fscript_parser_get_token_ex(parser, TRUE);
    if (t == NULL || t->type != TOKEN_FUNC || tk_str_eq(t->token, "=")) {
      fscript_parser_unget_token(parser);
      break;
    }

    if (t->token[0] == '|' || t->token[0] == '&' || t->token[0] == '^') {
      acall = fscript_func_call_create(parser, t->token, t->size);
      return_value_if_fail(acall != NULL, RET_OOM);
      args = &(acall->args);
      func_args_push(args, result);
      value_set_func(result, acall);
      fexpr_parse_compare(parser, &v);
      func_args_push(args, &v);
    } else {
      fscript_parser_unget_token(parser);
      break;
    }
  }

  return RET_OK;
}

static ret_t fexpr_parse_question(fscript_parser_t* parser, value_t* result) {
  value_t v;
  ret_t ret = RET_OK;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;

  value_set_str(&v, NULL);
  if (fexpr_parse_logic(parser, result) == RET_OK) {
    t = fscript_parser_get_token(parser);
    if (t && t->type == TOKEN_QUESTION) {
      acall = fscript_func_call_create(parser, "if", 2);
      return_value_if_fail(acall != NULL, RET_OOM);
      args = &(acall->args);
      func_args_push(args, result);
      value_set_func(result, acall);

      value_reset(&v);
      return_value_if_fail(fexpr_parse_logic(parser, &v) == RET_OK, RET_FAIL);
      func_args_push(args, &v);

      return_value_if_fail(fscript_parser_expect_token(parser, TOKEN_COLON, "expect :") == RET_OK,
                           RET_FAIL);
      return_value_if_fail(fexpr_parse_logic(parser, &v) == RET_OK, RET_FAIL);
      func_args_push(args, &v);
    } else {
      fscript_parser_unget_token(parser);
    }
  } else {
    ret = RET_FAIL;
  }

  return ret;
}

static ret_t fexpr_parse(fscript_parser_t* parser, value_t* result) {
  value_t v;
  token_t* t = NULL;
  fscript_args_t* args = NULL;
  fscript_func_call_t* acall = NULL;

  value_set_int(result, 0);
  return_value_if_fail(fexpr_parse_question(parser, result) == RET_OK, RET_FAIL);

  t = fscript_parser_get_token_ex(parser, TRUE);
  if (t != NULL && tk_str_eq(t->token, "=")) {
    acall = fscript_func_call_create(parser, t->token, t->size);
    return_value_if_fail(acall != NULL, RET_OOM);
    args = &(acall->args);
    func_args_push(args, result);
    value_set_func(result, acall);
    fexpr_parse_question(parser, &v);
    func_args_push(args, &v);
  } else {
    fscript_parser_unget_token(parser);
  }

  return RET_OK;
}

fscript_t* fscript_create_impl(fscript_parser_t* parser) {
  fscript_t* fscript = TKMEM_ZALLOC(fscript_t);
  return_value_if_fail(fscript != NULL, NULL);
  fscript->str = parser->temp;
  fscript->obj = parser->obj;
  fscript->first = parser->first;

  parser->obj = NULL;
  parser->first = NULL;
  parser->temp.str = NULL;

  return fscript;
}

static ret_t fscript_parse_statements(fscript_parser_t* parser, fscript_func_call_t* acall) {
  value_t v;
  ret_t ret = RET_OK;
  fscript_args_t* args = NULL;
  return_value_if_fail(parser != NULL && acall != NULL, RET_BAD_PARAMS);

  args = &(acall->args);

  do {
    token_t* t = NULL;
    value_set_int(&v, 0);
    ret = fexpr_parse(parser, &v);
    if (ret == RET_OK) {
      func_args_push(args, &v);
      t = fscript_parser_get_token(parser);
      if (t == NULL) {
        break;
      }

      if (t->type == TOKEN_FUNC || t->type == TOKEN_ID || t->type == TOKEN_NUMBER ||
          t->type == TOKEN_STR) {
        fscript_parser_unget_token(parser);
      } else if (t->type == TOKEN_RBRACKET) {
        fscript_parser_unget_token(parser);
        break;
      } else if (t->type != TOKEN_COMMA && t->type != TOKEN_SEMICOLON) {
        fscript_parser_set_error(parser, "unexpected token\n");
        break;
      }
    } else {
      if (v.type == VALUE_TYPE_JSCRIPT_FUNC) {
        fscript_func_call_destroy(value_func(&v));
      } else {
        value_reset(&v);
      }

      break;
    }
  } while (parser->token.type != TOKEN_EOF);

  return RET_OK;
}

fscript_parser_error_t* fscript_parser_error_init(fscript_parser_error_t* error) {
  return_value_if_fail(error != NULL, NULL);
  memset(error, 0x00, sizeof(fscript_parser_error_t));

  return error;
}

ret_t fscript_parser_error_deinit(fscript_parser_error_t* error) {
  return_value_if_fail(error != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(error->token);
  TKMEM_FREE(error->message);

  return RET_OK;
}

ret_t fscript_syntax_check(object_t* obj, const char* script, fscript_parser_error_t* error) {
  ret_t ret = RET_OK;
  fscript_parser_t parser;
  return_value_if_fail(obj != NULL && script != NULL && error != NULL, RET_BAD_PARAMS);
  fscript_parser_error_init(error);
  fscript_parser_init(&parser, obj, script, error);

  parser.first = fscript_func_call_create(&parser, "expr", 4);
  ret = fscript_parse_statements(&parser, parser.first);
  fscript_parser_deinit(&parser);

  return ret;
}

fscript_t* fscript_create(object_t* obj, const char* expr) {
  ret_t ret = RET_OK;
  fscript_parser_t parser;
  fscript_t* fscript = NULL;
  fscript_parser_error_t error;
  return_value_if_fail(expr != NULL, NULL);

  fscript_parser_error_init(&error);
  fscript_parser_init(&parser, obj, expr, &error);
  parser.first = fscript_func_call_create(&parser, "expr", 4);
  ret = fscript_parse_statements(&parser, parser.first);
  if (ret == RET_OK) {
    fscript = fscript_create_impl(&parser);
    fscript_parser_deinit(&parser);
  } else {
    log_warn("parser error:%s\n", expr);
    fscript_parser_deinit(&parser);
  }
  fscript_parser_error_deinit(&error);

  return fscript;
}

/*functions*/

static ret_t func_has_error(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_bool(result, fscript->error_code != RET_OK);
  return RET_OK;
}

static ret_t func_clear_error(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  fscript_set_error(fscript, RET_OK, NULL, NULL);
  return RET_OK;
}

static ret_t func_sum(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v = 0;
  uint32_t i = 0;
  bool_t has_str = FALSE;
  bool_t has_float = FALSE;
  FSCRIPT_FUNC_CHECK(args->size > 0, RET_BAD_PARAMS);

  for (i = 0; i < args->size; i++) {
    int type = args->args[i].type;
    if (type == VALUE_TYPE_STRING) {
      has_str = TRUE;
    }
    if (type == VALUE_TYPE_FLOAT || type == VALUE_TYPE_DOUBLE || type == VALUE_TYPE_FLOAT32) {
      has_float = TRUE;
    }
  }

  if (has_str) {
    str_t str;
    char buff[64];
    str_init(&str, 100);
    for (i = 0; i < args->size; i++) {
      str_append(&str, value_str_ex(args->args + i, buff, sizeof(buff) - 1));
    }
    value_dup_str(result, str.str);
    str_reset(&str);
  } else {
    for (i = 0; i < args->size; i++) {
      v += value_double(args->args + i);
    }
    if (has_float) {
      value_set_double(result, v);
    } else {
      value_set_int(result, (int)v);
    }
  }

  return RET_OK;
}

static ret_t func_int(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int(result, value_int(args->args));
  return RET_OK;
}

static ret_t func_i8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int8(result, value_int8(args->args));
  return RET_OK;
}

static ret_t func_i16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int16(result, value_int16(args->args));
  return RET_OK;
}

static ret_t func_i32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int(result, value_int(args->args));
  return RET_OK;
}

static ret_t func_i64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_int64(result, value_int64(args->args));
  return RET_OK;
}

static ret_t func_u8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint8(result, value_uint8(args->args));
  return RET_OK;
}

static ret_t func_u16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint16(result, value_uint16(args->args));
  return RET_OK;
}

static ret_t func_u32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args));
  return RET_OK;
}

static ret_t func_u64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint64(result, value_uint64(args->args));
  return RET_OK;
}

static ret_t func_f32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_float32(result, value_float32(args->args));
  return RET_OK;
}

static ret_t func_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args));
  return RET_OK;
}

static ret_t func_str(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->args->type == VALUE_TYPE_POINTER) {
    bool_t force_pointer_as_str = args->size == 2 && value_bool(args->args + 1);
    if (force_pointer_as_str) {
      value_set_str(result, (const char*)value_pointer(args->args));
    } else {
      char buff[16];
      tk_snprintf(buff, sizeof(buff) - 1, "%p", value_pointer(args->args));
      str_set(str, buff);
      value_set_str(result, str->str);
    }
  } else {
    str_from_value(str, args->args);
    value_set_str(result, str->str);
  }

  return RET_OK;
}

static ret_t func_one_of(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char sep = ';';
  int32_t index = 0;
  const char* strs = NULL;
  const char* start = NULL;

  value_set_str(result, "");
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(args->args[0].type == VALUE_TYPE_STRING, RET_BAD_PARAMS);
  strs = value_str(args->args);
  FSCRIPT_FUNC_CHECK(strs != NULL, RET_BAD_PARAMS);

  if (args->size > 2 && args->args[2].type == VALUE_TYPE_STRING) {
    const char* p = value_str(args->args + 2);
    if (p != NULL) {
      sep = *p;
    }
  }

  start = strs;
  index = value_int(args->args + 1);

  while (index > 0) {
    while (*start && *start != sep) {
      start++;
    }
    if (*start == '\0') {
      break;
    }
    index--;
    start++;
  }

  if (start != NULL) {
    const char* end = strchr(start, sep);
    if (end != NULL) {
      value_dup_str_with_len(result, start, end - start);
    } else {
      value_dup_str(result, start);
    }
  }

  return RET_OK;
}

static ret_t func_join(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  char buff[64];
  str_t* str = &(fscript->str);
  value_set_bool(result, FALSE);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);

  str_set(str, "");
  value_set_bool(result, TRUE);
  for (i = 1; i < args->size; i++) {
    if (i > 1) {
      str_append(str, value_str_ex(args->args, buff, sizeof(buff)));
    }
    str_append(str, value_str_ex(args->args + i, buff, sizeof(buff)));
  }

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_if(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return RET_OK;
}

static ret_t func_while(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return RET_OK;
}

static ret_t func_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* name = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  name = value_str(args->args);
  FSCRIPT_FUNC_CHECK(name != NULL, RET_BAD_PARAMS);

  value_set_bool(result, fscript_set_var(fscript, name, args->args + 1) == RET_OK);

  return RET_OK;
}

static ret_t func_expr(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  value_deep_copy(result, args->args + args->size - 1);

  return RET_OK;
}

static ret_t func_print(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  value_set_bool(result, TRUE);
  for (i = 0; i < args->size; i++) {
    char buff[64];
    log_info("%s ", value_str_ex(args->args + i, buff, sizeof(buff) - 1));
    (void)buff;
  }
  log_info("\n");

  return RET_OK;
}

static ret_t func_iformat(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  const char* format = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  format = value_str(args->args);
  FSCRIPT_FUNC_CHECK(format != NULL, RET_BAD_PARAMS);

  tk_snprintf(str->str, str->capacity - 1, format, value_int(args->args + 1));
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_fformat(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  const char* format = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  format = value_str(args->args);
  FSCRIPT_FUNC_CHECK(format != NULL, RET_BAD_PARAMS);

  tk_snprintf(str->str, str->capacity - 1, format, value_double(args->args + 1));
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_and(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_bool(result, value_bool(args->args) && value_bool(args->args + 1));

  return RET_OK;
}

static ret_t func_or(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_bool(result, value_bool(args->args) || value_bool(args->args + 1));

  return RET_OK;
}

static ret_t func_not(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, !value_bool(args->args));

  return RET_OK;
}

static ret_t func_minus(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_INT32) {
    value_set_int(result, -value_int(args->args));
  } else {
    value_set_double(result, -value_double(args->args));
  }

  return RET_OK;
}

static ret_t func_div(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) / value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_mod(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_int(result, value_int(args->args) % value_int(args->args + 1));

  return RET_OK;
}

static ret_t func_mul(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) * value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_sub(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_double(result, value_double(args->args) - value_double(args->args + 1));

  return RET_OK;
}

static ret_t func_random(fscript_t* fscript, fscript_args_t* args, value_t* result) {
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

static ret_t func_le(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) <= 0);
  } else {
    value_set_bool(result, value_double(args->args) <= value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_less(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) < 0);
  } else {
    value_set_bool(result, value_double(args->args) < value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_ge(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) >= 0);
  } else {
    value_set_bool(result, value_double(args->args) >= value_double(args->args + 1));
  }
  return RET_OK;
}

static ret_t func_great(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  if (args->args[0].type == VALUE_TYPE_STRING && args->args[1].type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_cmp(value_str(args->args), value_str(args->args + 1)) > 0);
  } else {
    value_set_bool(result, value_double(args->args) > value_double(args->args + 1));
  }

  return RET_OK;
}

static ret_t func_eq(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v1 = NULL;
  value_t* v2 = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  v1 = args->args;
  v2 = args->args + 1;

  if (v1->type == VALUE_TYPE_STRING && v2->type == VALUE_TYPE_STRING) {
    value_set_bool(result, tk_str_eq(value_str(v1), value_str(v2)));
  } else if (v1->type == VALUE_TYPE_INT64 || v2->type == VALUE_TYPE_INT64) {
    value_set_bool(result, value_int64(v1) == value_int64(v2));
  } else if (v1->type == VALUE_TYPE_UINT64 || v2->type == VALUE_TYPE_UINT64) {
    value_set_bool(result, value_uint64(v1) == value_uint64(v2));
  } else {
    value_set_bool(result, tk_fequal(value_double(v1), value_double(v2)));
  }

  return RET_OK;
}

static ret_t func_not_eq(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  func_eq(fscript, args, result);
  value_set_bool(result, !value_bool(result));

  return RET_OK;
}

static ret_t func_assert(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  if (!value_bool(args->args)) {
    fscript_set_error(fscript, RET_FAIL, __FUNCTION__,
                      args->size > 1 ? value_str(args->args + 1) : "unkown");
    if (args->size > 1 && value_bool(args->args + 1)) {
      assert(0);
    } else {
      log_debug("assert failed at: (%d %d)\n", (int)(fscript->curr->row),
                (int)(fscript->curr->col));
    }
  }

  return RET_OK;
}

static ret_t func_min(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  value_set_double(result, tk_min(v1, v2));

  return RET_OK;
}

static ret_t func_max(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  value_set_double(result, tk_max(v1, v2));

  return RET_OK;
}

static ret_t func_clamp(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v1 = 0;
  double v2 = 0;
  double v3 = 0;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);

  v1 = value_double(args->args);
  v2 = value_double(args->args + 1);
  v3 = value_double(args->args + 2);
  value_set_double(result, tk_clamp(v1, v2, v3));

  return RET_OK;
}

static ret_t func_round(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, tk_roundi(value_double(args->args)));
  return RET_OK;
}

static ret_t func_floor(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, floor(value_double(args->args)));
  return RET_OK;
}

static ret_t func_ceil(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_double(result, ceil(value_double(args->args)));
  return RET_OK;
}

static ret_t func_abs(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  value_set_double(result, tk_abs(value_double(args->args)));
  return RET_OK;
}

static ret_t func_len(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char buff[64];
  const char* str = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str = value_str_ex(args->args, buff, sizeof(buff) - 1);
  value_set_uint32(result, tk_strlen(str));

  return RET_OK;
}

static ret_t func_toupper(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_to_upper(str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_eval(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  return fscript_eval(fscript->obj, value_str(args->args), result);
}

static ret_t func_tolower(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_to_lower(str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_trim(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_trim(str, " \t\r\n");
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_substr(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  const char* org = NULL;
  uint32_t total_len = 0;
  uint32_t from = 0;
  uint32_t len = 0;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  org = value_str(args->args);
  from = value_uint32(args->args + 1);
  len = value_uint32(args->args + 2);
  total_len = tk_strlen(org);
  FSCRIPT_FUNC_CHECK(org != NULL, RET_BAD_PARAMS);

  if (from < total_len) {
    len = tk_min(len, (total_len - from));
    str_set_with_len(str, org + from, len);
  } else {
    str_set(str, "");
  }

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_replace(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  const char* org = NULL;
  const char* replace = NULL;
  const char* replace_with = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);

  org = value_str(args->args);
  replace = value_str(args->args + 1);
  replace_with = value_str(args->args + 2);
  FSCRIPT_FUNC_CHECK(org != NULL && replace != NULL && replace_with != NULL, RET_BAD_PARAMS);

  str_set(str, org);
  str_replace(str, replace, replace_with);

  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_contains(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* org = NULL;
  const char* target = NULL;
  value_set_bool(result, FALSE);
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);

  org = value_str(args->args);
  target = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(org != NULL && target != NULL, RET_BAD_PARAMS);
  value_set_bool(result, strstr(org, target) != NULL);

  return RET_OK;
}

static ret_t func_exec(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char buff[64];
  const char* cmd = NULL;
  const char* cmd_args = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  cmd = value_str(args->args);
  cmd_args = value_str_ex(args->args + 1, buff, sizeof(buff) - 1);
  value_set_bool(result, object_exec(fscript->obj, cmd, cmd_args) == RET_OK);
  return RET_OK;
}

static ret_t func_noop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return RET_OK;
}

static ret_t func_unset(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* var = NULL;
  const char* name = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  name = value_str(args->args);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  var = fscript_get_fast_var(fscript, name);
  if (var != NULL) {
    value_reset(var);
  } else {
    object_remove_prop(fscript->obj, name);
  }
  value_set_bool(result, TRUE);

  return RET_OK;
}

typedef struct _func_entry_t {
  const char* name;
  fscript_func_t func;
  uint32_t max_args_nr;
} func_entry_t;

static const func_entry_t s_builtin_funcs[] = {
    {"print", func_print, 4},
    {"expr", func_expr, 4},
    {"=", func_set, 2},
    {"set", func_set, 2},
    {"max", func_max, 2},
    {"min", func_min, 2},
    {"==", func_eq, 2},
    {"!=", func_not_eq, 2},
    {">=", func_ge, 2},
    {">", func_great, 2},
    {"<=", func_le, 2},
    {"<", func_less, 2},
    {"!", func_not, 1},
    {"minus", func_minus, 1},
    {"||", func_or, 2},
    {"and", func_and, 2},
    {"exec", func_exec, 2},
    {"join", func_join, 8},
    {"one_of", func_one_of, 3},
    {"if", func_if, 3},
    {"while", func_while, 10},
    {"int", func_int, 1},
    {"i8", func_i8, 1},
    {"i16", func_i16, 1},
    {"i32", func_i32, 1},
    {"i64", func_i64, 1},
    {"u8", func_u8, 1},
    {"u16", func_u16, 1},
    {"u32", func_u32, 1},
    {"u64", func_u64, 1},
    {"f32", func_f32, 1},
    {"float", func_float, 1},
    {"number", func_float, 1},
    {"iformat", func_iformat, 2},
    {"fformat", func_fformat, 2},
    {"unset", func_unset, 1},
    {"str", func_str, 1},
    {"string", func_str, 1},
    {"sub", func_sub, 2},
    {"assert", func_assert, 2},
    {"substr", func_substr, 3},
    {"has_error", func_has_error, 0},
    {"clear_error", func_clear_error, 0},
    {"sum", func_sum, 8},
    {"tolower", func_tolower, 1},
    {"toupper", func_toupper, 1},
    {"eval", func_eval, 1},
    {"trim", func_trim, 1},
    {"&&", func_and, 2},
    {"abs", func_abs, 1},
    {"round", func_round, 1},
    {"floor", func_floor, 1},
    {"ceil", func_ceil, 1},
    {"clamp", func_clamp, 3},
    {"contains", func_contains, 2},
    {"div", func_div, 2},
    {"eq", func_eq, 2},
    {"ge", func_ge, 2},
    {"great", func_great, 2},
    {"le", func_le, 2},
    {"len", func_len, 1},
    {"strlen", func_len, 1},
    {"less", func_less, 2},
    {"mul", func_mul, 2},
    {"#", func_noop, 0},
    {"noop", func_noop, 0},
    {"seq", func_noop, 4},
    {"not", func_not, 1},
    {"or", func_or, 2},
    {"random", func_random, 2},
    {"replace", func_replace, 3},
    {"/", func_div, 2},
    {"%", func_mod, 2},
    {"*", func_mul, 2},
    {"-", func_sub, 2},
    {"+", func_sum, 8},
};

static object_t* s_global_funcs;
static fscript_func_call_t* fscript_func_call_create(fscript_parser_t* parser, const char* name,
                                                     uint32_t size) {
  uint32_t i = 0;
  fscript_func_t func = NULL;
  char func_name[TK_NAME_LEN + 1];
  char full_func_name[2 * TK_NAME_LEN + 1];
  fscript_func_call_t* call = TKMEM_ZALLOC(fscript_func_call_t);
  return_value_if_fail(call != NULL, NULL);

  call->row = parser->row;
  call->col = parser->col;
  tk_strncpy(func_name, name, tk_min(size, TK_NAME_LEN));
  for (i = 0; i < ARRAY_SIZE(s_builtin_funcs); i++) {
    const func_entry_t* iter = s_builtin_funcs + i;
    if (tk_str_eq(iter->name, func_name)) {
      call->func = iter->func;
      func_args_init(&(call->args), iter->max_args_nr);
      return call;
    }
  }

  if (s_global_funcs != NULL) {
    func = (fscript_func_t)object_get_prop_pointer(s_global_funcs, func_name);
  }

  if (func == NULL) {
    tk_snprintf(full_func_name, sizeof(full_func_name) - 1, "%s%s", STR_FSCRIPT_FUNCTION_PREFIX,
                func_name);
    func = (fscript_func_t)object_get_prop_pointer(parser->obj, full_func_name);
  }

  if (func == NULL) {
    func = func_noop;
    log_warn("not found %s\n", func_name);
  }

  call->func = func;
  func_args_init(&(call->args), 2);

  return call;
}

ret_t fscript_global_init(void) {
  return RET_OK;
}

ret_t fscript_global_deinit(void) {
  OBJECT_UNREF(s_global_funcs);
  return RET_OK;
}

#include "tkc/object_default.h"
ret_t fscript_register_func(const char* name, fscript_func_t func) {
  return_value_if_fail(name != NULL && func != NULL, RET_BAD_PARAMS);
  if (s_global_funcs == NULL) {
    s_global_funcs = object_default_create();
  }

  return object_set_prop_pointer(s_global_funcs, name, (void*)func);
}

double tk_expr_eval(const char* expr) {
  value_t v;
  object_t* obj = NULL;
  value_set_double(&v, 0);
  return_value_if_fail(expr != NULL, 0);

  obj = object_default_create();
  fscript_eval(obj, expr, &v);
  OBJECT_UNREF(obj);

  return value_double(&v);
}

#endif

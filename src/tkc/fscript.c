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

static ret_t func_set(object_t* obj, fscript_args_t* args, value_t* result);

static ret_t func_args_push_str(fscript_args_t* args, const char* str, uint32_t size) {
  char* new_str = NULL;
  return_value_if_fail(str != NULL && args->size < FSCRIPT_MAX_ARGS, RET_BAD_PARAMS);

  new_str = tk_strndup(str, size);
  return_value_if_fail(new_str != NULL, RET_OOM);

  value_set_str(args->args + args->size, new_str);
  args->args[args->size].free_handle = TRUE;
  args->size++;

  return RET_OK;
}

static ret_t func_args_push(fscript_args_t* args, value_t* v) {
  return_value_if_fail(v != NULL && args->size < FSCRIPT_MAX_ARGS, RET_BAD_PARAMS);

  value_deep_copy(args->args + args->size, v);
  args->size++;

  return RET_OK;
}

static ret_t func_args_init(fscript_args_t* args, str_t* str) {
  memset(args, 0x00, sizeof(fscript_args_t));
  args->str = str;
  return RET_OK;
}

static ret_t func_args_deinit(fscript_args_t* args) {
  uint32_t i = 0;

  for (i = 0; i < args->size; i++) {
    value_reset(args->args + i);
  }
  args->size = 0;

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

typedef struct _fscript_t {
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
} fscript_t;

static fscript_func_t fscript_lookup(fscript_t* fscript, const char* name, uint32_t size);

static ret_t fscript_init(fscript_t* fscript, object_t* obj, const char* str) {
  memset(fscript, 0x00, sizeof(fscript_t));

  fscript->obj = obj;
  fscript->str = str;
  fscript->cursor = str;
  str_init(&(fscript->temp), 64);

  return RET_OK;
}

static ret_t fscript_deinit(fscript_t* fscript) {
  str_reset(&(fscript->temp));
  TKMEM_FREE(fscript->error);
  return RET_OK;
}

static char fscript_get_char(fscript_t* fscript) {
  char c = '\0';
  return_value_if_fail(fscript != NULL, c);
  if (fscript->c) {
    c = fscript->c;
    fscript->c = '\0';
    return c;
  }

  c = fscript->cursor[0];
  if (c) {
    if (c == '\n') {
      fscript->row++;
      fscript->col = 0;
    } else {
      fscript->col++;
    }

    fscript->cursor++;
  }

  return c;
}

static ret_t fscript_unget_char(fscript_t* fscript, char c) {
  return_value_if_fail(fscript->cursor > fscript->str, RET_BAD_PARAMS);

  fscript->c = c;

  return RET_OK;
}

typedef struct _func_entry_t {
  const char* name;
  fscript_func_t func;
} func_entry_t;

static ret_t fscript_set_error(fscript_t* fscript, const char* str) {
  return_value_if_fail(fscript != NULL && str != NULL, RET_BAD_PARAMS);

  fscript->error = tk_str_copy(fscript->error, str);
  log_warn("at line(%u) col (%u): %s\n", fscript->row, fscript->col, str);

  return RET_OK;
}

static ret_t fscript_skip_seperators(fscript_t* fscript) {
  char c = '\0';

  do {
    c = fscript_get_char(fscript);
  } while (isspace(c) || c == ';');
  fscript_unget_char(fscript, c);

  return RET_OK;
}

#define TOKEN_INIT(t, ttype, str) \
  t->type = ttype;                \
  t->token = str->str;            \
  t->size = str->size;

static ret_t fscript_parse_str(fscript_t* fscript) {
  char c = '\0';
  bool_t escape = FALSE;
  str_t* str = &(fscript->temp);
  token_t* t = &(fscript->token);

  str_set(str, "");
  do {
    c = fscript_get_char(fscript);
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
    fscript_unget_char(fscript, c);
  }

  return RET_OK;
}

static ret_t fscript_parse_id_or_number(fscript_t* fscript, token_type_t def_type) {
  char c = '\0';
  str_t* str = &(fscript->temp);
  token_t* t = &(fscript->token);

  str_set(str, "");
  do {
    c = fscript_get_char(fscript);
    if (c != '(' && c != ')' && c != ',' && c != '\"' && c != '\0') {
      str_append_char(str, c);
    } else {
      break;
    }
  } while (TRUE);

  str_trim(str, " \t\r\n");
  TOKEN_INIT(t, (c == '(' ? TOKEN_FUNC : def_type), str);
  if (c != '\0') {
    fscript_unget_char(fscript, c);
  }

  return RET_OK;
}

static token_t* fscript_get_token(fscript_t* fscript) {
  char c = 0;
  token_t* t = &(fscript->token);

  if (t->valid) {
    t->valid = FALSE;
    return t;
  }

  fscript_skip_seperators(fscript);
  c = fscript_get_char(fscript);

  t->size = 1;
  t->token = fscript->cursor - 1;
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
      fscript_skip_seperators(fscript);
      c = fscript_get_char(fscript);
      if (c == '(') {
        fscript_unget_char(fscript, c);
        t->size = fscript->cursor - t->token - 1;
      } else {
        t->size = fscript->cursor - t->token;
      }

      t->type = TOKEN_FUNC;
      return t;
    }
    case '\"': {
      fscript_parse_str(fscript);
      return t;
    }
    default: {
      fscript_unget_char(fscript, c);
      if (c == '+' || c == '-' || isdigit(c)) {
        fscript_parse_id_or_number(fscript, TOKEN_NUMBER);
      } else {
        fscript_parse_id_or_number(fscript, TOKEN_ID);
      }
      return t;
    }
  }
}

static ret_t fscript_expect_token(fscript_t* fscript, token_type_t type, const char* msg) {
  token_t* t = fscript_get_token(fscript);
  if (t == NULL || t->type != type) {
    fscript_set_error(fscript, msg);
  }

  return RET_OK;
}

static ret_t fscript_unget_token(fscript_t* fscript) {
  fscript->token.valid = TRUE;
  return RET_OK;
}

static ret_t fscript_exec_func(fscript_t* fscript, value_t* result) {
  value_t v;
  bool_t done = FALSE;
  fscript_args_t args;
  str_t* str = &(fscript->temp);
  token_t* t = fscript_get_token(fscript);
  fscript_func_t func = fscript_lookup(fscript, t->token, t->size);
  bool_t is_if = t && t->size == 2 && strncmp(t->token, "if", 2) == 0;
  return_value_if_fail(func != NULL, RET_NOT_FOUND);

  value_set_int(result, 0);
  func_args_init(&args, &(fscript->temp));
  fscript_expect_token(fscript, TOKEN_LPAREN, "expect \"(\"");

  while (fscript->error == NULL && t != NULL && t->type != TOKEN_RPAREN) {
    t = fscript_get_token(fscript);
    if (t == NULL) {
      fscript_expect_token(fscript, TOKEN_LPAREN, "expect \")\"");
      return RET_FAIL;
    }

    switch (t->type) {
      case TOKEN_ID: {
        if (t->token[0] == 't' && strncmp(t->token, "true", 4) == 0) {
          func_args_push(&args, value_set_bool(&v, TRUE));
        } else if (t->token[0] == 'f' && strncmp(t->token, "false", 5) == 0) {
          func_args_push(&args, value_set_bool(&v, FALSE));
        } else {
          if(args.size > 0 || (func != func_set && args.size == 0)) {
            if(object_get_prop(fscript->obj, str->str, &v) == RET_OK) {
              func_args_push(&args, &v);
            } else {
              func_args_push_str(&args, t->token, t->size);
            }
          } else {
            func_args_push_str(&args, t->token, t->size);
          }
        }
        break;
      }
      case TOKEN_FUNC: {
        bool_t ignore = fscript->ignore;
        fscript_unget_token(fscript);

        if (is_if && args.size > 0) {
          if (value_bool(args.args)) {
            fscript->ignore = args.size != 1;
          } else {
            fscript->ignore = args.size == 1;
          }
        }
        fscript_exec_func(fscript, &v);
        fscript->ignore = ignore;
        func_args_push(&args, &v);
        value_reset(&v);
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
        func_args_push(&args, &v);
        break;
      }
      case TOKEN_STR: {
        func_args_push_str(&args, t->token, t->size);
        break;
      }
      case TOKEN_RPAREN: {
        done = TRUE;
        break;
      }
      default: {
        fscript_set_error(fscript, "unexpected token:");
        break;
      }
    }

    if (done) {
      break;
    }

    t = fscript_get_token(fscript);
    if (t == NULL) {
      fscript_expect_token(fscript, TOKEN_LPAREN, "expect \")\"");
      break;
    }
  }

  if (!fscript->ignore) {
    func(fscript->obj, &args, result);
  }

  func_args_deinit(&args);

  return RET_OK;
}

static ret_t fscript_exec(fscript_t* fscript, value_t* result) {
  char c = '\0';
  ret_t ret = RET_OK;
  while (fscript->cursor[0]) {
    ret = fscript_exec_func(fscript, result);

    fscript_skip_seperators(fscript);
    c = fscript_get_char(fscript);
    if (c) {
      value_reset(result);
      fscript_unget_char(fscript, c);
    } else {
      break;
    }
  }

  return ret;
}

ret_t fscript_eval(object_t* obj, const char* script, value_t* result) {
  value_t v;
  fscript_t fscript;
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && script != NULL, RET_BAD_PARAMS);

  value_set_int(&v, 0);
  fscript_init(&fscript, obj, script);
  ret = fscript_exec(&fscript, &v);

  if (result != NULL) {
    value_deep_copy(result, &v);
  }

  value_reset(&v);
  fscript_deinit(&fscript);

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

static const func_entry_t s_builtin_funcs[] = {
    {"set", func_set},
    {"max", func_max},
    {"min", func_min},
    {"==", func_eq},
    {">=", func_ge},
    {">", func_great},
    {"<=", func_le},
    {"<", func_less},
    {"!", func_not},
    {"||", func_or},
    {"exec", func_exec},
    {"join", func_join},
    {"if", func_if},
    {"int", func_int},
    {"float", func_float},
    {"iformat", func_iformat},
    {"print", func_print},
    {"fformat", func_fformat},
    {"time_now", func_time_now},
    {"unset", func_unset},
    {"str", func_str},
    {"sub", func_sub},
    {"substr", func_substr},
    {"sum", func_sum},
    {"tolower", func_tolower},
    {"toupper", func_toupper},
    {"trim", func_trim},
    {"&&", func_and},
    {"abs", func_abs},
    {"acos", func_acos},
    {"and", func_and},
    {"asin", func_asin},
    {"atan", func_atan},
    {"clamp", func_clamp},
    {"contains", func_contains},
    {"cos", func_cos},
    {"div", func_div},
    {"eq", func_eq},
    {"ge", func_ge},
    {"great", func_great},
    {"le", func_le},
    {"len", func_len},
    {"less", func_less},
    {"mul", func_mul},
    {"noop", func_noop},
    {"not", func_not},
    {"or", func_or},
    {"pow", func_pow},
    {"random", func_random},
    {"replace", func_replace},
    {"sin", func_sin},
    {"sqrt", func_sqrt},
    {"tan", func_tan},
    {"time_now_ms", func_time_now_ms},
    {"time_now_us", func_time_now_us},
    {"&", func_bit_and},
    {"^", func_bit_nor},
    {"~", func_bit_not},
    {"|", func_bit_or},
    {"/", func_div},
    {"%", func_mod},
    {"*", func_mul},
    {"-", func_sub},
    {"+", func_sum},
};

static fscript_func_t fscript_lookup(fscript_t* fscript, const char* name, uint32_t size) {
  uint32_t i = 0;
  char func_name[TK_NAME_LEN + 1];
  char full_func_name[2 * TK_NAME_LEN + 1];

  tk_strncpy(func_name, name, tk_min(size, TK_NAME_LEN));
  for (i = 0; i < ARRAY_SIZE(s_builtin_funcs); i++) {
    if (tk_str_eq(s_builtin_funcs[i].name, func_name)) {
      return s_builtin_funcs[i].func;
    }
  }

  tk_snprintf(full_func_name, sizeof(full_func_name)-1, "%s%s", STR_FSCRIPT_FUNCTION_PREFIX, func_name);
  return (fscript_func_t)object_get_prop_pointer(fscript->obj, full_func_name);
}

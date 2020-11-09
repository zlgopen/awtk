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
#include "tkc/fscript.h"

static ret_t func_args_push_number(fscript_args_t* args, double v) {
  return_value_if_fail(args->size < FSCRIPT_MAX_ARGS, RET_BAD_PARAMS);

  value_set_double(args->args + args->size, v);
  args->size++;

  return RET_OK;
}

static ret_t func_args_push_bool(fscript_args_t* args, bool_t v) {
  return_value_if_fail(args->size < FSCRIPT_MAX_ARGS, RET_BAD_PARAMS);

  value_set_bool(args->args + args->size, v);
  args->size++;

  return RET_OK;
}

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

static ret_t func_args_init(fscript_args_t* args) {
  memset(args, 0x00, sizeof(fscript_args_t));
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
  uint32_t rows;
  uint32_t cols;

  char c;
} fscript_t;

static ret_t fscript_init(fscript_t* fscript, object_t* obj, const char* str) {
  memset(fscript, 0x00, sizeof(fscript_t));

  fscript->obj = obj;
  fscript->str = str;
  fscript->cursor = str;

  return RET_OK;
}

static ret_t fscript_deinit(fscript_t* fscript) {
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
      fscript->rows++;
      fscript->cols = 0;
    } else {
      fscript->cols++;
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

static ret_t func_sum(object_t* obj, fscript_args_t* args, value_t* result) {
  double v = 0;
  uint32_t i = 0;

  for (i = 0; i < args->size; i++) {
    v += value_double(args->args + i);
  }
  value_set_double(result, v);

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

static const func_entry_t s_builtin_funcs[] = {
    {"sum", func_sum},
    {"if", func_if},
};

static fscript_func_t fscript_lookup(fscript_t* fscript, const char* name, uint32_t size) {
  uint32_t i = 0;
  char func_name[TK_NAME_LEN + 1];

  tk_strncpy(func_name, name, tk_min(size, TK_NAME_LEN));
  for (i = 0; i < ARRAY_SIZE(s_builtin_funcs); i++) {
    if (tk_str_eq(s_builtin_funcs[i].name, func_name)) {
      return s_builtin_funcs[i].func;
    }
  }

  return (fscript_func_t)object_get_prop_pointer(fscript->obj, func_name);
}

static ret_t fscript_set_error(fscript_t* fscript, const char* str) {
  return_value_if_fail(fscript != NULL && str != NULL, RET_BAD_PARAMS);

  fscript->error = tk_str_copy(fscript->error, str);
  log_warn("%s", fscript->error);

  return RET_OK;
}

static ret_t fscript_skip_space(fscript_t* fscript) {
  char c = '\0';

  do {
    c = fscript_get_char(fscript);
  } while (isspace(c));
  fscript_unget_char(fscript, c);

  return RET_OK;
}

static token_t* fscript_get_token(fscript_t* fscript) {
  char c = 0;
  token_t* t = &(fscript->token);

  if (t->valid) {
    t->valid = FALSE;
    return t;
  }

  fscript_skip_space(fscript);
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
    case '\"': {
      t->type = TOKEN_STR;
      t->token = fscript->cursor;
      do {
        c = fscript_get_char(fscript);
      } while (c != '\"' && c != '\0');
      t->size = fscript->cursor - t->token;
      if (c == '\"') {
        t->size--;
      }
      return t;
    }
    default: {
      if (isalpha(c) || c == '_') {
        t->type = TOKEN_ID;
        do {
          c = fscript_get_char(fscript);
        } while (isalpha(c) || isdigit(c) || c == '_');
        fscript_unget_char(fscript, c);
      } else if (c == '+' || c == '-' || isdigit(c)) {
        t->type = TOKEN_NUMBER;
        do {
          c = fscript_get_char(fscript);
        } while (isdigit(c) || c == '.');
        fscript_unget_char(fscript, c);
      } else {
        fscript_set_error(fscript, "unexpected char.");
      }

      if (c) {
        t->size = fscript->cursor - t->token - 1;
      } else {
        t->size = fscript->cursor - t->token;
      }

      return t;
    }
  }
}

static ret_t fscript_expect_token(fscript_t* fscript, token_type_t type) {
  token_t* t = fscript_get_token(fscript);
  if (t == NULL || t->type != type) {
    fscript_set_error(fscript, "expected token.");
  }

  return RET_OK;
}

static ret_t fscript_unget_token(fscript_t* fscript) {
  fscript->token.valid = TRUE;
  return RET_OK;
}

static ret_t fscript_exec_func(fscript_t* fscript, value_t* result) {
  value_t v;
  fscript_args_t args;
  token_t* t = fscript_get_token(fscript);
  fscript_func_t func = fscript_lookup(fscript, t->token, t->size);
  return_value_if_fail(func != NULL, RET_NOT_FOUND);

  func_args_init(&args);
  fscript_expect_token(fscript, TOKEN_LPAREN);

  while (fscript->error == NULL && t->type != TOKEN_RPAREN) {
    t = fscript_get_token(fscript);
    switch (t->type) {
      case TOKEN_ID: {
        char id[TK_NAME_LEN + 1];
        tk_strncpy(id, t->token, tk_min(t->size, TK_NAME_LEN));
        if (tk_str_ieq(id, "true")) {
          func_args_push_bool(&args, TRUE);
        } else if (tk_str_ieq(id, "false")) {
          func_args_push_bool(&args, FALSE);
        } else {
          fscript_unget_token(fscript);
          fscript_exec_func(fscript, &v);
          func_args_push(&args, &v);
          value_reset(&v);
        }
        break;
      }
      case TOKEN_NUMBER: {
        double f = tk_atof(t->token);
        func_args_push_number(&args, f);
        break;
      }
      case TOKEN_STR: {
        func_args_push_str(&args, t->token, t->size);
        break;
      }
      default: {
        fscript_set_error(fscript, "unexpected token:");
        break;
      }
    }

    t = fscript_get_token(fscript);
    if (t->type == TOKEN_RPAREN) {
      break;
    }
  }

  func(fscript->obj, &args, result);
  func_args_deinit(&args);

  return RET_OK;
}

static ret_t fscript_exec(fscript_t* fscript, value_t* result) {
  char c = '\0';
  ret_t ret = RET_FAIL;

  while (fscript->cursor[0]) {
    ret = fscript_exec_func(fscript, result);

    fscript_skip_space(fscript);
    c = fscript_get_char(fscript);
    if (c) {
      value_reset(result);
      fscript_unget_char(fscript, c);
    } else {
      break;
    }
  }

  return RET_OK;
}

ret_t fscript_eval(object_t* obj, const char* script, value_t* result) {
  fscript_t fscript;
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && script != NULL && result != NULL, RET_BAD_PARAMS);

  value_set_int(result, 0);
  fscript_init(&fscript, obj, script);
  ret = fscript_exec(&fscript, result);
  fscript_deinit(&fscript);

  return RET_OK;
}

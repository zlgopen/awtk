/**
 * Eval - Simple numerical expression evaluator
 *
 * https://github.com/mattbucknall/eval
 *
 * Copyright (c) 2016 Matthew T. Bucknall
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISIN
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "tkc/utils.h"
#include "tkc/expr_eval.h"

#ifdef WIN32
#define DIRECTORY_SEPARATOR_CHAR '\\'
#else
#define DIRECTORY_SEPARATOR_CHAR '/'
#endif

typedef enum {
  EVAL_TOKEN_TYPE_END,
  EVAL_TOKEN_TYPE_ADD,
  EVAL_TOKEN_TYPE_G,
  EVAL_TOKEN_TYPE_GE,
  EVAL_TOKEN_TYPE_L,
  EVAL_TOKEN_TYPE_LE,
  EVAL_TOKEN_TYPE_NE,
  EVAL_TOKEN_TYPE_E,
  EVAL_TOKEN_TYPE_NOT,
  EVAL_TOKEN_TYPE_OR,
  EVAL_TOKEN_TYPE_AND,
  EVAL_TOKEN_TYPE_BITS_NOT,
  EVAL_TOKEN_TYPE_BITS_OR,
  EVAL_TOKEN_TYPE_BITS_AND,
  EVAL_TOKEN_TYPE_SUBTRACT,
  EVAL_TOKEN_TYPE_MULTIPLY,
  EVAL_TOKEN_TYPE_DIVIDE,
  EVAL_TOKEN_TYPE_OPEN_BRACKET,
  EVAL_TOKEN_TYPE_CLOSE_BRACKET,
  EVAL_TOKEN_TYPE_NUMBER,
  EVAL_TOKEN_TYPE_FUNC,
  EVAL_TOKEN_TYPE_STRING,
  EVAL_TOKEN_TYPE_VARIABLE

} EvalTokenType;

typedef struct {
  EvalTokenType type;

  union {
    double number;
    char name[EVAL_MAX_NAME_LENGTH];
  } value;

} EvalToken;

typedef struct {
  const char* name;
  EvalFunc func;

} EvalFunctionEntry;

typedef struct {
  const char* name;
  double value;

} EvalVariableEntry;

typedef struct {
  const EvalHooks* hooks;
  void* user_data;
  const char* input;
  size_t stack_level;
  EvalToken token;
  ExprStr str;
} EvalContext;

static EvalResult parse_expr(EvalContext* ctx, ExprValue* output);

static int is_digit(char c) {
  return (c >= '0') && (c <= '9');
}

static int is_name_start(char c) {
  return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || (c == '_');
}

static int is_name(char c) {
  return is_name_start(c) || is_digit(c);
}

static int is_exp(char c) {
  return (c == 'e') || (c == 'E');
}

static int is_dp(char c) {
  return (c == '.');
}

static char get_char(EvalContext* ctx) {
  return *(ctx->input++);
}

static void put_char(EvalContext* ctx) {
  ctx->input--;
}

static EvalResult expr_str_init(ExprStr* str, size_t capacity) {
  if (capacity < 31) {
    capacity = 31;
  }

  str->size = 0;
  str->capacity = capacity;
  str->str = (char*)malloc(capacity + 1);

  return str->str ? EVAL_RESULT_OK : EVAL_RESULT_OOM;
}

static void expr_str_clear(ExprStr* str) {
  if (str->str) {
    free(str->str);
    memset(str, 0x00, sizeof(ExprStr));
  }
}

static EvalResult expr_str_append_str(ExprStr* str, const char* other, size_t len) {
  size_t size = str->size + len;
  if (size >= str->capacity) {
    size_t capacity = size;
    char* s = (char*)realloc(str->str, capacity + 1);
    if (s == NULL) {
      return EVAL_RESULT_OOM;
    }
    str->str = s;
    str->capacity = capacity;
  }

  memcpy(str->str + str->size, other, len);
  str->size = size;
  str->str[size] = '\0';

  return EVAL_RESULT_OK;
}

static EvalResult expr_str_append_char(ExprStr* str, char c) {
  return expr_str_append_str(str, &c, 1);
}

static const char* number_to_string(double v, char* str, size_t capacity) {
  if (ceill(v) == v) {
    tk_snprintf(str, capacity, "%u", (unsigned int)v);
  } else {
    tk_snprintf(str, capacity, "%lf", v);
  }

  return str;
}

static EvalResult expr_value_to_string(ExprValue* v) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (v->type == EXPR_VALUE_TYPE_NUMBER) {
    double val = v->v.val;

    if (expr_str_init(&(v->v.str), 63) != EVAL_RESULT_OK) {
      assert(0);
      return EVAL_RESULT_OOM;
    }

    v->type = EXPR_VALUE_TYPE_STRING;
    number_to_string(val, v->v.str.str, v->v.str.capacity);
    v->v.str.size = strlen(v->v.str.str);
  }

  return EVAL_RESULT_OK;
}

static EvalResult expr_value_append_string(ExprValue* v, const char* str, size_t len) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (expr_value_to_string(v) == EVAL_RESULT_OK) {
    return expr_str_append_str(&(v->v.str), str, len);
  }

  return EVAL_RESULT_OOM;
}

void expr_value_init(ExprValue* v) {
  if (v != NULL) {
    memset(v, 0x00, sizeof(ExprValue));
    v->type = EXPR_VALUE_TYPE_NUMBER;
  }

  return;
}

EvalResult expr_value_set_number(ExprValue* v, double val) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (v->type == EXPR_VALUE_TYPE_STRING) {
    expr_str_clear(&(v->v.str));
  }

  v->v.val = val;
  v->type = EXPR_VALUE_TYPE_NUMBER;

  return EVAL_RESULT_OK;
}

EvalResult expr_value_set_string(ExprValue* v, const char* str, size_t len) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (v->type == EXPR_VALUE_TYPE_NUMBER) {
    expr_str_init(&(v->v.str), len);
    v->type = EXPR_VALUE_TYPE_STRING;
  }

  v->v.str.size = 0;
  expr_value_append_string(v, str, len);

  return EVAL_RESULT_OK;
}

static EvalResult expr_value_to_number(ExprValue* v) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (v->type == EXPR_VALUE_TYPE_STRING) {
    double val = atof(v->v.str.str);
    return expr_value_set_number(v, val);
  }

  return EVAL_RESULT_OK;
}

double expr_value_get_number(const ExprValue* v) {
  return_value_if_fail(v != NULL, EVAL_RESULT_OK);

  if (v->type == EXPR_VALUE_TYPE_STRING) {
    return atof(v->v.str.str);
  } else {
    return v->v.val;
  }
}

const char* expr_value_get_string(const ExprValue* v) {
  return_value_if_fail(v != NULL, NULL);

  if (v->type == EXPR_VALUE_TYPE_STRING) {
    return (v->v.str.str);
  } else {
    return NULL;
  }
}

static EvalResult expr_value_op(ExprValue* a, ExprValue* b, EvalTokenType op) {
  return_value_if_fail(a != NULL && b != NULL, EVAL_RESULT_OK);

  if (a->type == EXPR_VALUE_TYPE_STRING || b->type == EXPR_VALUE_TYPE_STRING) {
    expr_value_to_string(a);
    expr_value_to_string(b);
    switch (op) {
      case EVAL_TOKEN_TYPE_MULTIPLY: {
        expr_value_append_string(a, "*", 1);
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_OR: {
        int ret = a->v.str.size || b->v.str.size;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_AND: {
        int ret = a->v.str.size && b->v.str.size;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_BITS_OR: {
        expr_value_append_string(a, "|", 1);
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_BITS_AND: {
        expr_value_append_string(a, "&", 1);
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_DIVIDE: {
        expr_value_append_string(a, "/", 1);
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_SUBTRACT: {
        expr_value_append_string(a, "-", 1);
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_ADD: {
        expr_value_append_string(a, b->v.str.str, b->v.str.size);
        break;
      }
      case EVAL_TOKEN_TYPE_E: {
        int ret = strcmp(a->v.str.str, b->v.str.str) == 0;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_G: {
        int ret = strcmp(a->v.str.str, b->v.str.str) > 0;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_L: {
        int ret = strcmp(a->v.str.str, b->v.str.str) < 0;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_LE: {
        int ret = strcmp(a->v.str.str, b->v.str.str) <= 0;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_NE: {
        int ret = strcmp(a->v.str.str, b->v.str.str) != 0;
        expr_value_set_number(a, ret);
        break;
      }
      case EVAL_TOKEN_TYPE_GE: {
        int ret = strcmp(a->v.str.str, b->v.str.str) >= 0;
        expr_value_set_number(a, ret);
        break;
      }
      default:
        break;
    }
  } else {
    expr_value_to_number(a);
    expr_value_to_number(b);
    switch (op) {
      case EVAL_TOKEN_TYPE_MULTIPLY: {
        a->v.val *= b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_OR: {
        a->v.val = a->v.val || b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_AND: {
        a->v.val = a->v.val && b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_BITS_OR: {
        a->v.val = (unsigned int)a->v.val | (unsigned int)b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_BITS_AND: {
        a->v.val = (unsigned int)a->v.val & (unsigned int)b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_DIVIDE: {
        a->v.val /= b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_ADD: {
        a->v.val += b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_SUBTRACT: {
        a->v.val -= b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_E: {
        a->v.val = a->v.val == b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_G: {
        a->v.val = a->v.val > b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_L: {
        a->v.val = a->v.val < b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_NE: {
        a->v.val = a->v.val != b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_LE: {
        a->v.val = a->v.val <= b->v.val;
        break;
      }
      case EVAL_TOKEN_TYPE_GE: {
        a->v.val = a->v.val >= b->v.val;
        break;
      }
      default:
        break;
    }
  }

  return EVAL_RESULT_OK;
}

void expr_value_clear(ExprValue* v) {
  if (v->type == EXPR_VALUE_TYPE_STRING) {
    expr_value_set_number(v, 0);
  }
}

static EvalResult get_number(EvalContext* ctx) {
  char c;
  double value;
  long exp;
  double power;

  value = 0.0f;
  exp = 0;

  c = get_char(ctx);

  if (!is_dp(c)) {
    if (!is_digit(c)) return EVAL_RESULT_INVALID_LITERAL;

    do {
      value = (value * 10.0f) + (c - '0');
      c = get_char(ctx);

    } while (is_digit(c));
  }

  if (is_dp(c)) {
    c = get_char(ctx);
    if (!is_digit(c)) return EVAL_RESULT_INVALID_LITERAL;

    do {
      value = (value * 10.0f) + (c - '0');
      exp--;

      c = get_char(ctx);

    } while (is_digit(c));
  }

  if (is_exp(c)) {
    int exp_neg;
    int int_val;

    exp_neg = 0;

    c = get_char(ctx);

    switch (c) {
      case '-':
        exp_neg = 1;
        /* fall through */

      case '+':
        c = get_char(ctx);
        /* fall through */

      default:
        break;
    }

    int_val = 0;

    if (!is_digit(c)) return EVAL_RESULT_INVALID_LITERAL;

    do {
      int_val = (int_val * 10) + (c - '0');
      c = get_char(ctx);

    } while (is_digit(c));

    if (exp_neg)
      exp -= int_val;
    else
      exp += int_val;
  }

  power = 10.0f;

  if (exp < 0) {
    exp = -exp;

    while (exp) {
      if (exp & 1) value /= power;
      exp >>= 1;
      power *= power;
    }
  } else {
    while (exp) {
      if (exp & 1) value *= power;
      exp >>= 1;
      power *= power;
    }
  }

  put_char(ctx);

  ctx->token.type = EVAL_TOKEN_TYPE_NUMBER;
  ctx->token.value.number = value;

  return EVAL_RESULT_OK;
}

static EvalResult get_name(EvalContext* ctx, EvalTokenType type) {
  char c;
  int length = 0;

  for (;;) {
    c = get_char(ctx);
    if (!is_name(c)) break;

    if (length >= (EVAL_MAX_NAME_LENGTH - 1)) return EVAL_RESULT_NAME_TOO_LONG;

    ctx->token.value.name[length++] = c;
  }

  put_char(ctx);

  ctx->token.type = type;
  ctx->token.value.name[length] = '\0';

  return EVAL_RESULT_OK;
}

static EvalResult get_string(EvalContext* ctx, EvalTokenType type) {
  char c;
  char last_c = '\0';
  ctx->str.size = 0;
  for (;;) {
    c = get_char(ctx);
    if (c == '\\') {
      last_c = c;
      continue;
    }

    if (c == '"' && last_c != '\\') {
      break;
    }
    last_c = c;
    expr_str_append_char(&(ctx->str), c);
  }

  ctx->token.type = type;

  return EVAL_RESULT_OK;
}

static EvalResult get_token(EvalContext* ctx) {
  char c;

  for (;;) {
    c = get_char(ctx);

    if (c == '\0') {
      put_char(ctx);
      ctx->token.type = EVAL_TOKEN_TYPE_END;
      return EVAL_RESULT_OK;
    } else if (c <= ' ') {
      continue;
    } else if (is_digit(c) || is_dp(c)) {
      put_char(ctx);
      return get_number(ctx);
    } else if (is_name_start(c)) {
      put_char(ctx);
      return get_name(ctx, EVAL_TOKEN_TYPE_FUNC);
    } else if (c == '$') {
      return get_name(ctx, EVAL_TOKEN_TYPE_VARIABLE);
    } else if (c == '"') {
      return get_string(ctx, EVAL_TOKEN_TYPE_STRING);
    } else
      switch (c) {
        case '>': {
          char next_c = get_char(ctx);
          if (next_c == '=') {
            ctx->token.type = EVAL_TOKEN_TYPE_GE;
          } else {
            put_char(ctx);
            ctx->token.type = EVAL_TOKEN_TYPE_G;
          }
          break;
        }
        case '<': {
          char next_c = get_char(ctx);
          if (next_c == '=') {
            ctx->token.type = EVAL_TOKEN_TYPE_LE;
          } else {
            put_char(ctx);
            ctx->token.type = EVAL_TOKEN_TYPE_L;
          }
          break;
        }
        case '!': {
          char next_c = get_char(ctx);
          if (next_c == '=') {
            ctx->token.type = EVAL_TOKEN_TYPE_NE;
          } else {
            put_char(ctx);
            ctx->token.type = EVAL_TOKEN_TYPE_NOT;
          }
          break;
        }
        case '&': {
          char next_c = get_char(ctx);
          if (next_c == '&') {
            ctx->token.type = EVAL_TOKEN_TYPE_AND;
          } else {
            put_char(ctx);
            ctx->token.type = EVAL_TOKEN_TYPE_BITS_AND;
          }
          break;
        }
        case '|': {
          char next_c = get_char(ctx);
          if (next_c == '|') {
            ctx->token.type = EVAL_TOKEN_TYPE_OR;
          } else {
            put_char(ctx);
            ctx->token.type = EVAL_TOKEN_TYPE_BITS_OR;
          }
          break;
        }
        case '=': {
          char next_c = get_char(ctx);
          ctx->token.type = EVAL_TOKEN_TYPE_E;
          if (next_c != '=') {
            put_char(ctx);
          }
          break;
        }
        case '~':
          ctx->token.type = EVAL_TOKEN_TYPE_BITS_NOT;
          break;
        case '+':
          ctx->token.type = EVAL_TOKEN_TYPE_ADD;
          break;
        case '-':
          ctx->token.type = EVAL_TOKEN_TYPE_SUBTRACT;
          break;
        case '*':
          ctx->token.type = EVAL_TOKEN_TYPE_MULTIPLY;
          break;
        case '/':
          ctx->token.type = EVAL_TOKEN_TYPE_DIVIDE;
          break;
        case '(':
          ctx->token.type = EVAL_TOKEN_TYPE_OPEN_BRACKET;
          break;
        case ')':
          ctx->token.type = EVAL_TOKEN_TYPE_CLOSE_BRACKET;
          break;

        default:
          return EVAL_RESULT_ILLEGAL_CHARACTER;
      }

    return EVAL_RESULT_OK;
  }
}

static EvalResult parse_term(EvalContext* ctx, ExprValue* output) {
  EvalResult result;

  if (ctx->token.type == EVAL_TOKEN_TYPE_NUMBER) {
    expr_value_set_number(output, ctx->token.value.number);
  } else if (ctx->token.type == EVAL_TOKEN_TYPE_STRING) {
    expr_value_set_string(output, ctx->str.str, ctx->str.size);
  } else if (ctx->token.type == EVAL_TOKEN_TYPE_OPEN_BRACKET) {
    result = get_token(ctx);
    if (result != EVAL_RESULT_OK) return result;

    result = parse_expr(ctx, output);
    if (result != EVAL_RESULT_OK) return result;

    if (ctx->token.type != EVAL_TOKEN_TYPE_CLOSE_BRACKET) {
      return EVAL_RESULT_EXPECTED_CLOSE_BRACKET;
    }
  } else if (ctx->token.type == EVAL_TOKEN_TYPE_FUNC) {
    EvalFunc func;
    ExprValue arg;
    expr_value_init(&arg);

    if (!ctx->hooks || !ctx->hooks->get_func) {
      return EVAL_RESULT_UNDEFINED_FUNCTION;
    }

    func = ctx->hooks->get_func(ctx->token.value.name, ctx->user_data);
    if (!func) return EVAL_RESULT_UNDEFINED_FUNCTION;

    result = get_token(ctx);
    if (result != EVAL_RESULT_OK) return result;

    if (ctx->token.type != EVAL_TOKEN_TYPE_OPEN_BRACKET) {
      return EVAL_RESULT_EXPECTED_OPEN_BRACKET;
    }

    result = get_token(ctx);
    if (result != EVAL_RESULT_OK) return result;

    result = parse_expr(ctx, &arg);
    if (result != EVAL_RESULT_OK) return result;

    if (ctx->token.type != EVAL_TOKEN_TYPE_CLOSE_BRACKET) {
      return EVAL_RESULT_EXPECTED_CLOSE_BRACKET;
    }

    result = func(&arg, ctx->user_data, output);
    if (result != EVAL_RESULT_OK) return result;
    expr_value_clear(&arg);
  } else if (ctx->token.type == EVAL_TOKEN_TYPE_VARIABLE) {
    if (!ctx->hooks || !ctx->hooks->get_variable) {
      return EVAL_RESULT_UNDEFINED_VARIABLE;
    }

    result = ctx->hooks->get_variable(ctx->token.value.name, ctx->user_data, output);
    if (result != EVAL_RESULT_OK) return result;
  } else {
    return EVAL_RESULT_EXPECTED_TERM;
  }

  return get_token(ctx);
}

static EvalResult parse_unary(EvalContext* ctx, ExprValue* output) {
  EvalResult result;
  int neg = 0;
  int not = 0;
  int bit_not = 0;
  ExprValue value;

  expr_value_init(&value);

  for (;;) {
    if (ctx->token.type == EVAL_TOKEN_TYPE_NOT) {
      not = !not;

      result = get_token(ctx);
      if (result != EVAL_RESULT_OK) return result;
    } else if (ctx->token.type == EVAL_TOKEN_TYPE_BITS_NOT) {
      bit_not = !bit_not;

      result = get_token(ctx);
      if (result != EVAL_RESULT_OK) return result;
    } else if (ctx->token.type == EVAL_TOKEN_TYPE_SUBTRACT) {
      neg = !neg;

      result = get_token(ctx);
      if (result != EVAL_RESULT_OK) return result;
    } else {
      break;
    }
  }

  result = parse_term(ctx, &value);
  if (result != EVAL_RESULT_OK) return result;

  if (value.type == EXPR_VALUE_TYPE_NUMBER) {
    if (neg) {
      value.v.val = -value.v.val;
    }
    if (not) {
      value.v.val = !value.v.val;
    }
    if (bit_not) {
      value.v.val = ~(unsigned int)value.v.val;
    }
  }
  if (value.type == EXPR_VALUE_TYPE_STRING) {
    if (not) {
      expr_value_set_number(&value, !value.v.str.size);
    }
  }

  *output = value;

  return EVAL_RESULT_OK;
}

static EvalResult parse_product(EvalContext* ctx, ExprValue* output) {
  EvalResult result;
  ExprValue lhs;
  ExprValue rhs;

  expr_value_init(&lhs);
  expr_value_init(&rhs);

  result = parse_unary(ctx, &lhs);
  if (result != EVAL_RESULT_OK) return result;

  for (;;) {
    int type = ctx->token.type;
    if (type == EVAL_TOKEN_TYPE_MULTIPLY || type == EVAL_TOKEN_TYPE_DIVIDE ||
        type == EVAL_TOKEN_TYPE_E || type == EVAL_TOKEN_TYPE_L || type == EVAL_TOKEN_TYPE_G ||
        type == EVAL_TOKEN_TYPE_NE || type == EVAL_TOKEN_TYPE_LE || type == EVAL_TOKEN_TYPE_GE ||
        type == EVAL_TOKEN_TYPE_OR || type == EVAL_TOKEN_TYPE_AND ||
        type == EVAL_TOKEN_TYPE_BITS_OR || type == EVAL_TOKEN_TYPE_BITS_AND) {
      result = get_token(ctx);
      if (result != EVAL_RESULT_OK) return result;

      result = parse_unary(ctx, &rhs);
      if (result != EVAL_RESULT_OK) return result;

      expr_value_op(&lhs, &rhs, (EvalTokenType)type);
    } else {
      break;
    }
  }

  *output = lhs;
  expr_value_clear(&rhs);

  return EVAL_RESULT_OK;
}

static EvalResult parse_sum(EvalContext* ctx, ExprValue* output) {
  ExprValue lhs;
  ExprValue rhs;
  EvalResult result;

  expr_value_init(&lhs);
  expr_value_init(&rhs);

  result = parse_product(ctx, &lhs);
  if (result != EVAL_RESULT_OK) return result;

  for (;;) {
    int type = ctx->token.type;
    if (type == EVAL_TOKEN_TYPE_ADD || type == EVAL_TOKEN_TYPE_SUBTRACT) {
      result = get_token(ctx);
      if (result != EVAL_RESULT_OK) return result;

      result = parse_product(ctx, &rhs);
      if (result != EVAL_RESULT_OK) return result;

      expr_value_op(&lhs, &rhs, (EvalTokenType)type);
    } else {
      break;
    }
  }

  *output = lhs;
  expr_value_clear(&rhs);

  return EVAL_RESULT_OK;
}

static EvalResult parse_expr(EvalContext* ctx, ExprValue* output) {
  EvalResult result;

  if (ctx->stack_level >= EVAL_MAX_STACK_DEPTH) {
    return EVAL_RESULT_STACK_OVERFLOW;
  }

  ctx->stack_level++;
  result = parse_sum(ctx, output);
  ctx->stack_level--;

  return result;
}

EvalResult eval_execute(const char* expression, const EvalHooks* hooks, void* user_data,
                        ExprValue* output) {
  EvalContext ctx;
  EvalResult result;

  expr_str_init(&ctx.str, 100);

  ctx.hooks = hooks;
  ctx.user_data = user_data;
  ctx.input = expression;
  ctx.stack_level = 0;

  result = get_token(&ctx);
  if (result != EVAL_RESULT_OK) {
    expr_str_clear(&ctx.str);
    return result;
  }

  result = parse_expr(&ctx, output);
  if (result != EVAL_RESULT_OK) {
    expr_str_clear(&ctx.str);
    return result;
  }

  result = (ctx.token.type == EVAL_TOKEN_TYPE_END) ? EVAL_RESULT_OK : EVAL_RESULT_UNEXPECTED_CHAR;
  expr_str_clear(&ctx.str);

  return result;
}

static EvalResult func_number(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    expr_value_set_number(output, atof(input->v.str.str));
  } else {
    expr_value_set_number(output, expr_value_get_number(input));
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_strlen(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    expr_value_set_number(output, (double)(input->v.str.size));
  } else {
    char buff[64];
    number_to_string(input->v.val, buff, sizeof(buff));
    expr_value_set_number(output, (double)strlen(buff));
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_path(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    size_t i = 0;
    char* p = NULL;
    size_t n = input->v.str.size;
    expr_value_set_string(output, input->v.str.str, input->v.str.size);

    p = output->v.str.str;
    for (i = 0; i < n; i++) {
      char c = p[i];
      if (c == '/' || c == '\\') {
        p[i] = DIRECTORY_SEPARATOR_CHAR;
      }
    }
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_toupper(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    size_t i = 0;
    char* p = NULL;
    size_t n = input->v.str.size;
    expr_value_set_string(output, input->v.str.str, input->v.str.size);

    p = output->v.str.str;
    for (i = 0; i < n; i++) {
      p[i] = toupper(p[i]);
    }
  } else {
    char buff[64];
    number_to_string(input->v.val, buff, sizeof(buff));
    expr_value_set_string(output, buff, strlen(buff));
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_tolower(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    size_t i = 0;
    char* p = NULL;
    size_t n = input->v.str.size;
    expr_value_set_string(output, input->v.str.str, input->v.str.size);

    p = output->v.str.str;
    for (i = 0; i < n; i++) {
      p[i] = tolower(p[i]);
    }
  } else {
    char buff[64];
    number_to_string(input->v.val, buff, sizeof(buff));
    expr_value_set_string(output, buff, strlen(buff));
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_string(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    expr_value_set_string(output, input->v.str.str, input->v.str.size);
  } else {
    expr_value_set_number(output, (expr_value_get_number(input)));
    expr_value_to_string(output);
  }

  return EVAL_RESULT_OK;
}

static EvalResult func_cos(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, cos(expr_value_get_number(input)));

  return EVAL_RESULT_OK;
}

static EvalResult func_sin(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, sin(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_tan(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, tan(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_acos(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, acos(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_asin(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, asin(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_atan(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, atan(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_exp(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, exp(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_log(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, log(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_log10(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, log10(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_sqrt(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, sqrt(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_ceil(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, ceil(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_floor(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, floor(expr_value_get_number(input)));
  return EVAL_RESULT_OK;
}

static EvalResult func_round(const ExprValue* input, void* user_data, ExprValue* output) {
  (void)user_data;
  expr_value_set_number(output, floor(expr_value_get_number(input) + 0.5));
  return EVAL_RESULT_OK;
}

static EvalFunc default_get_func(const char* name, void* user_data) {
  static const EvalFunctionEntry FUNCTIONS[] = {
      {"number", func_number}, {"strlen", func_strlen},   {"path", func_path},
      {"string", func_string}, {"toupper", func_toupper}, {"tolower", func_tolower},
      {"cos", func_cos},       {"sin", func_sin},         {"tan", func_tan},
      {"acos", func_acos},     {"asin", func_asin},       {"atan", func_atan},
      {"exp", func_exp},       {"log", func_log},         {"log10", func_log10},
      {"sqrt", func_sqrt},     {"ceil", func_ceil},       {"floor", func_floor},
      {"int", func_floor},     {"round", func_round}};

  const EvalFunctionEntry* i = FUNCTIONS;
  const EvalFunctionEntry* e = i + (sizeof(FUNCTIONS) / sizeof(*FUNCTIONS));

  while (i != e) {
    if (strcmp(i->name, name) == 0) return i->func;
    i++;
  }

  (void)user_data;

  return 0;
}

#ifndef _HUGE_ENUF
#define _HUGE_ENUF 1e+300
#endif

#ifndef INFINITY
#define INFINITY ((float)(_HUGE_ENUF * _HUGE_ENUF))
#endif /*INFINITY*/

#ifndef NAN
#define NAN ((float)(INFINITY * 0.0F))
#endif /*NAN*/

static EvalResult default_get_variable(const char* name, void* user_data, ExprValue* output) {
  static const EvalVariableEntry VARIABLES[] = {
      {"INFINITY", INFINITY}, {"NAN", NAN}, {"PI", 3.14159265358979f}};

  const EvalVariableEntry* i = VARIABLES;
  const EvalVariableEntry* e = i + (sizeof(VARIABLES) / sizeof(*VARIABLES));

  (void)user_data;
  while (i != e) {
    if (strcmp(i->name, name) == 0) {
      expr_value_set_number(output, i->value);
      return EVAL_RESULT_OK;
    }

    i++;
  }

  return EVAL_RESULT_UNDEFINED_VARIABLE;
}

const EvalHooks* eval_default_hooks(void) {
  static const EvalHooks HOOKS = {default_get_func, default_get_variable};

  return &HOOKS;
}

const char* eval_result_to_string(EvalResult result) {
  const char* STRS[N_EVAL_RESULT_CODES] = {"ok",
                                           "illegal character",
                                           "invalid literal",
                                           "literal out-of-range",
                                           "name too long",
                                           "unexpected character",
                                           "expected term",
                                           "stack overflow",
                                           "undefined function",
                                           "undefined variable",
                                           "expected open bracket",
                                           "expected close bracket"};

  return ((result < N_EVAL_RESULT_CODES)) ? STRS[result] : "undefined error";
}

double tk_expr_eval(const char* expr) {
  ExprValue v;
  EvalResult res;

  expr_value_init(&v);
  res = eval_execute(expr, eval_default_hooks(), NULL, &v);

  return res == EVAL_RESULT_OK ? expr_value_get_number(&v) : 0;
}

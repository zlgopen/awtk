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

#ifndef EVAL_H
#define EVAL_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

#define EVAL_MAX_STACK_DEPTH 8
#define EVAL_MAX_NAME_LENGTH 32

typedef enum _ExprValueType {
  EXPR_VALUE_TYPE_NUMBER = 0,
  EXPR_VALUE_TYPE_STRING,
  EXPR_VALUE_TYPE_NONE
} ExprValueType;

typedef struct _ExprStr {
  size_t size;
  size_t capacity;
  char* str;
} ExprStr;

typedef struct _ExprValue {
  ExprValueType type;
  union {
    double val;
    ExprStr str;
  } v;
} ExprValue;

typedef enum {
  EVAL_RESULT_OK,
  EVAL_RESULT_BAD_PARAMS,
  EVAL_RESULT_ILLEGAL_CHARACTER,
  EVAL_RESULT_INVALID_LITERAL,
  EVAL_RESULT_LITERAL_OUT_OF_RANGE,
  EVAL_RESULT_NAME_TOO_LONG,
  EVAL_RESULT_UNEXPECTED_CHAR,
  EVAL_RESULT_EXPECTED_TERM,
  EVAL_RESULT_STACK_OVERFLOW,
  EVAL_RESULT_UNDEFINED_FUNCTION,
  EVAL_RESULT_UNDEFINED_VARIABLE,
  EVAL_RESULT_EXPECTED_OPEN_BRACKET,
  EVAL_RESULT_EXPECTED_CLOSE_BRACKET,
  EVAL_RESULT_OOM,
  N_EVAL_RESULT_CODES
} EvalResult;

typedef EvalResult (*EvalFunc)(const ExprValue* input, void* user_data, ExprValue* output);

typedef struct {
  EvalFunc (*get_func)(const char* name, void* user_data);
  EvalResult (*get_variable)(const char* name, void* user_data, ExprValue* output);
} EvalHooks;

EvalResult eval_execute(const char* expr, const EvalHooks* hooks, void* ctx, ExprValue* output);

double tk_expr_eval(const char* expr);

const EvalHooks* eval_default_hooks(void);

const char* eval_result_to_string(EvalResult result);

void expr_value_init(ExprValue* v);
void expr_value_clear(ExprValue* v);

double expr_value_get_number(const ExprValue* v);
EvalResult expr_value_set_number(ExprValue* v, double val);

const char* expr_value_get_string(const ExprValue* v);
EvalResult expr_value_set_string(ExprValue* v, const char* str, size_t len);

END_C_DECLS

#endif  // EVAL_H

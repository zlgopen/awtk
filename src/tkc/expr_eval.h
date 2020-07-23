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

/**
 * @class eval_t
 * @annotation ["fake"]
 * 表达式。
 *
 */

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

/**
 * @method eval_execute 
 *
 * 计算表达式的值。
 *
 * @param {const char*} expr 表达式
 * @param {const EvalHooks*} hooks 回调函数。
 * @param {void*} ctx 回调函数的上下文。
 * @param {ExprValue*} output 计算结果。
 *
 * @return {EvalResult} 返回EVAL_RESULT_OK表示成功，否则表示失败。
 */
EvalResult eval_execute(const char* expr, const EvalHooks* hooks, void* ctx, ExprValue* output);

/**
 * @method tk_expr_eval
 *
 * 计算表达式返回浮点数。
 *
 * @param {const char*} expor 表达式
 *
 * @return {double} 返回结果。
 */
double tk_expr_eval(const char* expr);

/**
 * @method tk_expr_eval_str
 *
 * 计算表达式返回字符串。
 *
 * @param {const char*} expor 表达式
 * @param {char*} result 结果字符串
 * @param {uint32_t} 最大长度。
 *
 * @return {const char*} 返回结果。
 */
const char* tk_expr_eval_str(const char* expr, char* result, uint32_t max_size);

/**
 * @method eval_default_hooks
 *
 * 获取内置函数。
 *
 * @return {const EvalHooks*} 回调函数列表。
 */
const EvalHooks* eval_default_hooks(void);

/**
 * @method eval_result_to_string
 *
 * 将错误码转换成字符串。
 *
 * @param {EvalResult} result 值。
 *
 * @return {const char*} 返回对应的字符串。
 */
const char* eval_result_to_string(EvalResult result);

/**
 * @method expr_value_init
 *
 * 初始化值。
 * @param {ExprValue*} v 值对象。
 *
 * @return {void} 返回void。
 */
void expr_value_init(ExprValue* v);

/**
 * @method expr_value_clear
 *
 * 清除值。
 * @param {ExprValue*} v 值对象。
 *
 * @return {void} 返回void。
 */
void expr_value_clear(ExprValue* v);

/**
 * @method expr_value_get_number
 *
 * 获取浮点数值。
 *
 * @param {const ExprValue*} v 值对象。
 *
 * @return {double} 返回浮点数值。
 */
double expr_value_get_number(const ExprValue* v);

/**
 * @method expr_value_set_number
 *
 * 设置浮点数值。
 *
 * @param {ExprValue*} v 值对象。
 * @param {double} val 浮点数的值。
 *
 * @return {EvalResult} 返回EVAL_RESULT_OK表示成功，否则表示失败。
 */
EvalResult expr_value_set_number(ExprValue* v, double val);

/**
 * @method expr_value_get_string
 *
 * 获取字符串。
 *
 * @param {const ExprValue*} v 值对象。
 *
 * @return {const char*} 返回字符串。
 */
const char* expr_value_get_string(const ExprValue* v);

/**
 * @method expr_value_set_string
 *
 * 设置字符串。
 *
 * @param {ExprValue*} v 值对象。
 * @param {const char*} str 字符串。
 * @param {size_t} len 字符串长度。
 *
 * @return {EvalResult} 返回EVAL_RESULT_OK表示成功，否则表示失败。
 */
EvalResult expr_value_set_string(ExprValue* v, const char* str, size_t len);

END_C_DECLS

#endif  // EVAL_H

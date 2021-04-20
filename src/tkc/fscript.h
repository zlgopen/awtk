/**
 * File:   fscript.h
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

#ifndef TK_FSCRIPTS_H
#define TK_FSCRIPTS_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

#define FSCRIPT_MAX_ARGS 128
#define FSCRIPT_FAST_VAR_NR 4

/**
 * @class fscript_args_t
 * 
 * 扩展函数的参数。
 *
 */
typedef struct _fscript_args_t {
  /**
   * @property {uint16_t} size
   * @annotation ["readable"]
   * 参数个数。
   */
  uint16_t size;
  /**
   * @property {uint16_t} capacity
   * @annotation ["readable"]
   * 目前最大容量。
   */
  uint16_t capacity;
  /**
   * @property {value_t*} args
   * @annotation ["readable"]
   * 参数列表。
   */
  value_t* args;

} fscript_args_t;

struct _fscript_func_call_t;
typedef struct _fscript_func_call_t fscript_func_call_t;

/**
 * @class fscript_parser_error_t
 * 
 * 解析错误信息。
 *
 */
typedef struct _fscript_parser_error_t {
  /**
   * @property {int} row
   * @annotation ["readable"]
   * 出现错误的代码行。
   */
  int row;
  /**
   * @property {int} col
   * @annotation ["readable"]
   * 出现错误的代码列。
   */
  int col;

  /**
   * @property {int} offset
   * @annotation ["readable"]
   * 出现错误的代码偏移。
   */
  int offset;

  /**
   * @property {char*} message
   * @annotation ["readable"]
   * 错误信息。
   */
  char* message;

  /**
   * @property {char*} token
   * @annotation ["readable"]
   * 当前的token。
   */
  char* token;
} fscript_parser_error_t;

/**
 * @method fscript_parser_error_deinit
 * 释放error对象中的资源。
 *
 * @param {fscript_parser_error_t*} error 解析错误信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_parser_error_deinit(fscript_parser_error_t* error);

/**
 * @class fscript_t
 * @annotation ["fake"]
 * 
 * 一个简易的函数式脚本引擎。
 * 用法请参考：https://github.com/zlgopen/awtk/blob/master/docs/fscript.md
 *
 */
typedef struct _fscript_t {
  /**
   * @property {str_t} str
   * @annotation ["readable"]
   * C语言实现函数可以使用这个变量，可以有效避免内存分配。
   */
  str_t str;
  /**
   * @property {object_t*} obj
   * @annotation ["readable"]
   * 脚本执行上下文。
   */
  object_t* obj;
  /**
   * @property {value_t*} fast_vars
   * @annotation ["readable"]
   * 快速访问变量。在脚本可以用a/b/c/d来访问，需要优化时使用。
   */
  value_t fast_vars[FSCRIPT_FAST_VAR_NR];

  /*private*/
  ret_t error_code;
  char* error_message;
  fscript_func_call_t* curr;
  fscript_func_call_t* first;
  fscript_func_call_t* error_func;
  bool_t breaked;
  bool_t continued;
  bool_t returned;
  uint8_t while_count;
} fscript_t;

typedef ret_t (*fscript_func_t)(fscript_t* fscript, fscript_args_t* args, value_t* v);

/**
 * @method fscript_create
 * 创建引擎对象，并解析代码。
 * @param {object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 *
 * @return {fscript_t*} 返回fscript对象。
 */
fscript_t* fscript_create(object_t* obj, const char* script);

/**
 * @method fscript_syntax_check
 * 解析代码，分析是否有语法错误。
 *
 * 示例：
 * ```c
 * fscript_parser_error_t error;
 * fscript_syntax_check(obj, "1+1", &error);
 * fscript_parser_error_deinit(&error);
 *```
 *
 * @param {object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {fscript_parser_error_t*} error 用于返回错误信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_syntax_check(object_t* obj, const char* script, fscript_parser_error_t* error);

/**
 * @method fscript_exec
 * 执行解析后的代码。 
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {value_t*} result 执行结果(调用者需要用value_reset函数清除result)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_exec(fscript_t* fscript, value_t* result);

/**
 * @method fscript_set_error
 * 用于扩展函数设置遇到的错误。
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {ret_t} code 错误码。
 * @param {const char*} func 函数名。
 * @param {const char*} message 错误消息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_error(fscript_t* fscript, ret_t code, const char* func, const char* message);

/**
 * @method fscript_destroy
 * 销毁引擎对象。
 * @param {fscript_t*} fscript 脚本引擎对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_destroy(fscript_t* fscript);

/**
 * @method fscript_eval
 * 执行一段脚本。
 * @param {object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {value_t*} result 执行结果(调用者需要用value_reset函数清除result)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_eval(object_t* obj, const char* script, value_t* result);

/**
 * @method fscript_global_init
 * 全局初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_global_init(void);

/**
 * @method fscript_register_func
 * 注册全局自定义函数。
 * @param {const char*} name 函数名(无需加函数前缀)。
 * @param {fscript_func_t*} func 函数指针。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_register_func(const char* name, fscript_func_t func);

/**
 * @method fscript_global_deinit
 * 全局释放。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_global_deinit(void);

/**
 * @method tk_expr_eval
 * 对fscript的简单包装。
 * 
 * @param {const char*} expr 表达式。
 *
 * @return {double} 返回表达式的值。
 */
double tk_expr_eval(const char* expr);

/*注册自定义函数时，属性名的前缀。*/
#define STR_FSCRIPT_FUNCTION_PREFIX "function."

/*用于扩展函数里检查参数*/
#define FSCRIPT_FUNC_CHECK(predicate, code)                        \
  if (!(predicate)) {                                              \
    fscript_set_error(fscript, code, __FUNCTION__, "" #predicate); \
    return code;                                                   \
  }

END_C_DECLS

#endif /*TK_FSCRIPTS_H*/

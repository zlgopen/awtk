/**
 * File:   fscript.h
 * Author: AWTK Develop Team
 * Brief:  a simple functional script language
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/general_factory.h"

BEGIN_C_DECLS

#define FSCRIPT_MAX_ARGS 128

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

struct _fscript_t;
typedef struct _fscript_t fscript_t;

typedef ret_t (*fscript_on_error_t)(void* ctx, fscript_t* fscript);
typedef ret_t (*fscript_func_t)(fscript_t* fscript, fscript_args_t* args, value_t* v);

/**
 * @class fscript_t
 * @annotation ["fake"]
 * 
 * 一个简易的函数式脚本引擎。
 * 用法请参考：https://github.com/zlgopen/awtk/blob/master/docs/fscript.md
 *
 */
struct _fscript_t {
  /**
   * @property {str_t} str
   * @annotation ["readable"]
   * C语言实现函数可以使用这个变量，可以有效避免内存分配。
   */
  str_t str;
  /**
   * @property {tk_object_t*} obj
   * @annotation ["readable"]
   * 脚本执行上下文。
   */
  tk_object_t* obj;
  /**
   * @property {ret_t} error_code
   * @annotation ["readable"]
   * 运行时错误码。
   */
  ret_t error_code;
  /**
   * @property {char*} error_message
   * @annotation ["readable"]
   * 运行时错误信息。
   */
  char* error_message;
  /**
   * @property {int32_t} error_row
   * @annotation ["readable"]
   * 运行时错误的行号。
   */
  int32_t error_row;
  /**
   * @property {int32_t} error_row
   * @annotation ["readable"]
   * 运行时错误的列号。
   */
  int32_t error_col;
  /**
   * @property {uint16_t} lines
   * @annotation ["readable"]
   * 代码总行数。
   */
  uint16_t lines;

  /*private*/
  char* code_id;
  fscript_func_call_t* curr;
  fscript_func_call_t* first;
  fscript_func_call_t* error_func;
  bool_t breaked;
  bool_t continued;
  bool_t returned;
  uint8_t loop_count;

  /*函数局部变量和参数*/
  darray_t* locals;
  /*脚本定义的函数*/
  tk_object_t* funcs_def;

  void* on_error_ctx;
  fscript_on_error_t on_error;
  fscript_func_t print;
};

/**
 * @method fscript_create
 * 创建引擎对象，并解析代码。
 * @param {tk_object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 *
 * @return {fscript_t*} 返回fscript对象。
 */
fscript_t* fscript_create(tk_object_t* obj, const char* script);

/**
 * @method fscript_create_ex
 * 创建引擎对象，并解析代码。
 * @param {tk_object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {bool_t} keep_func_name 是否在func_call结构后保存函数名。
 *
 * @return {fscript_t*} 返回fscript对象。
 */
fscript_t* fscript_create_ex(tk_object_t* obj, const char* script, bool_t keep_func_name);

/**
 * @method fscript_init
 * 初始化引擎对象，并解析代码。
 * @param {fscript_t*} fscript 初始化 fscript 对象。
 * @param {tk_object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {const char*} first_call_name 第一个函数的名字。
 * @param {bool_t} keep_func_name 是否在func_call结构后保存函数名。
 *
 * @return {fscript_t*} 返回fscript对象。
 */
fscript_t* fscript_init(fscript_t* fscript, tk_object_t* obj, const char* script,
                        const char* first_call_name, bool_t keep_func_name);

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
 * @param {tk_object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {fscript_parser_error_t*} error 用于返回错误信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_syntax_check(tk_object_t* obj, const char* script, fscript_parser_error_t* error);

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
 * @method fscript_reload
 * 重新加载代码。 
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {const char*} script 脚本代码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_reload(fscript_t* fscript, const char* script);

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
 * @method fscript_set_on_error
 * 设置错误处理函数。
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {fscript_on_error_t} on_error 错误处理函数。
 * @param {void*} ctx 错误处理函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_on_error(fscript_t* fscript, fscript_on_error_t on_error, void* ctx);

/**
 * @method fscript_set_print_func
 * 设置打印日志的函数。
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {fscript_func_t} print_func 打印日志的函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_print_func(fscript_t* fscript, fscript_func_t print);

/**
 * @method fscript_deinit
 * 清除引擎对象的数据。
 * @param {fscript_t*} fscript 脚本引擎对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_deinit(fscript_t* fscript);

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
 * @param {tk_object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {value_t*} result 执行结果(调用者需要用value_reset函数清除result)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_eval(tk_object_t* obj, const char* script, value_t* result);

/**
 * @method fscript_global_init
 * 全局初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_global_init(void);

/**
 * @method fscript_set_global_object
 * 用于替换默认的全局对象。
 * >仅限于在系统初始化时调用。
 * @param {tk_object_t*} obj 全局对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_global_object(tk_object_t* obj);

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
 * @method fscript_register_funcs
 * 注册全局自定义函数。
 * @param {const general_factory_table_t*} table 函数表。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_register_funcs(const general_factory_table_t* table);

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

/**
 * @method fscript_get_global_object
 * 获取fscript的全局对象。
 * 
 * @return {tk_object_t*} 返回fscript的全局对象。
 */
tk_object_t* fscript_get_global_object(void);

/**
 * @class fscript_func_call_t
 * 
 * 函数描述。
 *
 */
struct _fscript_func_call_t {
  /**
   * @property {void*} ctx
   * @annotation ["readable"]
   * 函数需要的上下文。
   * >目前主要保持自定义函数的实现。
   */
  void* ctx;
  /**
   * @property {uint16_t} row
   * @annotation ["readable"]
   * 对应源代码行号。
   */
  uint16_t row;
  /**
   * @property {uint16_t} row
   * @annotation ["readable"]
   * 对应源代码列号。
   */
  uint16_t col;
  /**
   * @property {fscript_func_t} func
   * @annotation ["readable"]
   * 函数指针。
   */
  fscript_func_t func;
  /*private*/
  fscript_args_t args;
  fscript_func_call_t* next;
};

/*注册自定义函数时，属性名的前缀。*/
#define STR_FSCRIPT_FUNCTION_PREFIX "function."

/*用于扩展函数里检查参数*/
#define FSCRIPT_FUNC_CHECK(predicate, code)                                          \
  if (!(predicate)) {                                                                \
    fscript_set_error(fscript, code, __FUNCTION__, "" #predicate " not satisfied."); \
    return code;                                                                     \
  }

#define FSCRIPT_STR_GLOBAL_PREFIX "global."
#define FSCRIPT_GLOBAL_PREFIX_LEN 7

#define VALUE_TYPE_FSCRIPT_ID VALUE_TYPE_ID
#define VALUE_TYPE_FSCRIPT_FUNC VALUE_TYPE_FUNC

typedef ret_t (*fscript_on_init_t)(fscript_t* fscript, const char* code);
typedef ret_t (*fscript_on_deinit_t)(fscript_t* fscript);
typedef ret_t (*fscript_before_exec_t)(fscript_t* fscript);
typedef ret_t (*fscript_after_exec_t)(fscript_t* fscript);
typedef ret_t (*fscript_set_var_t)(fscript_t* fscript, const char* name, const value_t* v);
typedef ret_t (*fscript_exec_func_t)(fscript_t* fscript, const char* name,
                                     fscript_func_call_t* iter, value_t* result);

/**
 * @method fscript_set_var_default
 * 设置变量的默认实现。
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {const char*} name 变量名。
 * @param {const value_t*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_var_default(fscript_t* fscript, const char* name, const value_t* value);

/**
 * @method fscript_exec_func_default
 * 执行函数的默认实现。
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {fscript_func_call_t*} iter 当前函数。
 * @param {value_t*} result 返回结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_exec_func_default(fscript_t* fscript, fscript_func_call_t* iter, value_t* result);

typedef struct _fscript_hooks_t {
  fscript_on_init_t on_init;
  fscript_on_deinit_t on_deinit;
  fscript_set_var_t set_var;
  fscript_exec_func_t exec_func;
  fscript_before_exec_t before_exec;
  fscript_after_exec_t after_exec;
} fscript_hooks_t;

/**
 * @method fscript_set_hooks
 * 设置回调函数。
 * @param {const fscript_hooks_t*} hooks 回调函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_set_hooks(const fscript_hooks_t* hooks);

/**
 * @method fscript_ensure_locals
 * export for debugger
 * @param {fscript_t*} fscript 脚本引擎对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_ensure_locals(fscript_t* fscript);

END_C_DECLS

#endif /*TK_FSCRIPTS_H*/

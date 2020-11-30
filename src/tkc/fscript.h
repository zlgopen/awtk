/**
 * File:   fscript.h
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

#ifndef TK_FSCRIPTS_H
#define TK_FSCRIPTS_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

#define FSCRIPT_MAX_ARGS 8

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
  value_t fast_vars[4];
  /*private*/
  fscript_func_call_t* first;
} fscript_t;

typedef ret_t (*fscript_func_t)(fscript_t* fscript, fscript_args_t* args, value_t* v);

/**
 * @method fscript_create
 * 创建引擎对象。
 * @param {object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 *
 * @return {fscript_t*} 返回fscript对象。
 */
fscript_t* fscript_create(object_t* obj, const char* script);

/**
 * @method fscript_exec
 * @param {fscript_t*} fscript 脚本引擎对象。
 * @param {value_t*} result 执行结果(调用者需要用value_reset函数清除result)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_exec(fscript_t* fscript, value_t* result);

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

/*注册自定义函数时，属性名的前缀。*/
#define STR_FSCRIPT_FUNCTION_PREFIX "function."

END_C_DECLS

#endif /*TK_FSCRIPTS_H*/

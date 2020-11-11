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
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 参数个数。
   */
  uint32_t size;

  /**
   * @property {value_t*} args
   * @annotation ["readable"]
   * 参数列表。
   */
  value_t args[FSCRIPT_MAX_ARGS];

  /**
   * @property {str_t*} str
   * @annotation ["readable"]
   * 函数实现时可以临时使用的字符串对象，可以避免频繁分配内存。
   */
  str_t* str;
} fscript_args_t;

typedef ret_t (*fscript_func_t)(object_t* obj, fscript_args_t* args, value_t* v);

/**
 * @class fscript_t
 * @annotation ["fake"]
 * 
 * 一个简易的函数式脚本。
 * 用法请参考：https://github.com/zlgopen/awtk/blob/master/docs/fscript.md
 *
 */

/**
 * @method fscript_eval
 * @param {object_t*} obj 脚本执行上下文。
 * @param {const char*} script 脚本代码。
 * @param {value_t*} result 执行结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_eval(object_t* obj, const char* script, value_t* result);

/*注册自定义函数时，属性名的前缀。*/
#define STR_FSCRIPT_FUNCTION_PREFIX "function."

END_C_DECLS

#endif /*TK_FSCRIPTS_H*/

/**
 * File:   fscript_debugger_fscript.h
 * Author: AWTK Develop Team
 * Brief:  debugger_fscript for fscript
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_FSCRIPT_H
#define TK_DEBUGGER_FSCRIPT_H

#include "tkc/fscript.h"
#include "tkc/cond_var.h"
#include "tkc/mutex_nest.h"
#include "debugger/debugger.h"

BEGIN_C_DECLS

/**
 * @class debugger_fscript_t
 * fscript调试器
 *
 */
typedef struct _debugger_fscript_t {
  debugger_t debugger;

  /*private*/
  char* code_id;
  fscript_t* fscript;

  uint32_t last_executed_line;

  int32_t prev_breaked_line;
  uint32_t executed_lines;
  
  int32_t next_stop_line;
  int32_t next_stop_executed_line;
  int32_t next_stop_call_frame_index;

  str_t code;
  bool_t paused;
  str_t callstack;
  tk_mutex_nest_t* mutex;
  tk_cond_var_t* cond_var;
  darray_t break_points;
  darray_t call_stack_frames;
  bool_t code_changed;
} debugger_fscript_t;

/**
 * @method debugger_fscript_create
 * 创建调试器对象。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
debugger_t* debugger_fscript_create(void);

/**
 * @method debugger_fscript_set_fscript
 * 设置fscript对象。
 * @param {debugger_t*} debugger debugger对象。
 * @param {fscript_t*} fscript 脚本对象。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
ret_t debugger_fscript_set_fscript(debugger_t* debugger, fscript_t* fscript);

/**
 * @method debugger_fscript_cast
 * 类型转换。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {debugger_fscript_t*} 返回debugger对象。
 */
debugger_fscript_t* debugger_fscript_cast(debugger_t* debugger);

#define DEBUGGER_FSCRIPT(debugger) debugger_fscript_cast((debugger_t*)debugger);

#define DEBUGGER_LANG_FSCRIPT "fscript"

/*fscript hooks*/
ret_t debugger_fscript_set_var(fscript_t* fscript, const char* name, const value_t* v);
ret_t debugger_fscript_exec_func(fscript_t* fscript, const char* name, fscript_func_call_t* iter,
                                 value_t* result);

END_C_DECLS

#endif /*TK_DEBUGGER_FSCRIPT_H*/

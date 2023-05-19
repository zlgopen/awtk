/**
 * File:   debugger.h
 * Author: AWTK Develop Team
 * Brief:  debugger
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
 * 2022-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_H
#define TK_DEBUGGER_H

#include "tkc/event.h"
#include "tkc/object.h"
#include "tkc/types_def.h"
#include "debugger/debugger_const.h"

BEGIN_C_DECLS

struct _debugger_t;
typedef struct _debugger_t debugger_t;
/**
 * @enum debugger_program_state_t
 * @prefix DEBUGGER_PROGRAM_STATE_
 * 被调试程序的状态。
 */
typedef enum _debugger_program_state_t {
  /** 
   * @const DEBUGGER_PROGRAM_STATE_NONE
   * 初始状态。
   */
  DEBUGGER_PROGRAM_STATE_NONE = 0,
  /** 
   * @const DEBUGGER_PROGRAM_STATE_PAUSED
   * 停止状态(断点/异常等)。
   */
  DEBUGGER_PROGRAM_STATE_PAUSED,
  /** 
   * @const DEBUGGER_PROGRAM_STATE_RUNNING
   * 运行状态(断点/异常等)。
   */
  DEBUGGER_PROGRAM_STATE_RUNNING,
  /** 
   * @const DEBUGGER_PROGRAM_STATE_TERMINATED
   * 终止状态(断点/异常等)。
   */
  DEBUGGER_PROGRAM_STATE_TERMINATED
} debugger_program_state_t;

typedef ret_t (*debugger_lock_t)(debugger_t* debugger);
typedef ret_t (*debugger_unlock_t)(debugger_t* debugger);
typedef ret_t (*debugger_stop_t)(debugger_t* debugger);
typedef ret_t (*debugger_restart_t)(debugger_t* debugger);
typedef ret_t (*debugger_pause_t)(debugger_t* debugger);
typedef bool_t (*debugger_match_t)(debugger_t* debugger, const char* code_id);

typedef ret_t (*debugger_step_over_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_in_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_out_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_loop_over_t)(debugger_t* debugger);
typedef ret_t (*debugger_continue_t)(debugger_t* debugger);
typedef tk_object_t* (*debugger_get_local_t)(debugger_t* debugger, uint32_t frame_index);
typedef tk_object_t* (*debugger_get_self_t)(debugger_t* debugger);
typedef tk_object_t* (*debugger_get_global_t)(debugger_t* debugger);
typedef ret_t (*debugger_get_callstack_t)(debugger_t* debugger, binary_data_t* callstack);
typedef ret_t (*debugger_clear_break_points_t)(debugger_t* debugger);
typedef ret_t (*debugger_set_break_point_t)(debugger_t* debugger, uint32_t line);
typedef ret_t (*debugger_remove_break_point_t)(debugger_t* debugger, uint32_t line);
typedef ret_t (*debugger_get_code_t)(debugger_t* debugger, binary_data_t* code);
typedef ret_t (*debugger_get_break_points_t)(debugger_t* debugger, binary_data_t* break_points);
typedef ret_t (*debugger_get_debuggers_t)(debugger_t* debugger, binary_data_t* debuggers);
typedef ret_t (*debugger_update_code_t)(debugger_t* debugger, const binary_data_t* code);
typedef ret_t (*debugger_launch_t)(debugger_t* debugger, const char* lang,
                                   const binary_data_t* code);
typedef ret_t (*debugger_attach_t)(debugger_t* debugger, const char* lang, const char* code_id);
typedef ret_t (*debugger_deinit_t)(debugger_t* debugger);

typedef debugger_program_state_t (*debugger_get_state_t)(debugger_t* debugger);
typedef ret_t (*debugger_set_state_t)(debugger_t* debugger, debugger_program_state_t state);

/*扩展接口以支持lldb的DAP协议{*/
typedef tk_object_t* (*debugger_get_threads_t)(debugger_t* debugger);
typedef ret_t (*debugger_launch_app_t)(debugger_t* debugger, const char* program,
                                       const char* work_dir, int argc, char* argv[]);

typedef tk_object_t* (*debugger_get_var_t)(debugger_t* debugger, const char* path);
typedef ret_t (*debugger_set_break_point_ex_t)(debugger_t* debugger, const char* position);
typedef ret_t (*debugger_remove_break_point_ex_t)(debugger_t* debugger, const char* position);
typedef ret_t (*debugger_set_current_frame_t)(debugger_t* debugger, uint32_t frame_index);
typedef ret_t (*debugger_dispatch_messages_t)(debugger_t* debugger);
/*}扩展接口以支持lldb的DAP协议{*/

typedef debugger_t* (*debugger_fscript_create_t)(void);

typedef struct _debugger_vtable_t {
  const char* lang;

  debugger_attach_t attach;
  debugger_lock_t lock;
  debugger_unlock_t unlock;
  debugger_stop_t stop;
  debugger_pause_t pause;
  debugger_match_t match;

  debugger_step_in_t step_in;
  debugger_restart_t restart;
  debugger_step_out_t step_out;
  debugger_step_over_t step_over;
  debugger_step_loop_over_t step_loop_over;
  debugger_continue_t continve;
  debugger_get_local_t get_local;
  debugger_get_self_t get_self;
  debugger_get_global_t get_global;
  debugger_get_callstack_t get_callstack;
  debugger_get_code_t get_code;
  debugger_get_debuggers_t get_debuggers;
  debugger_get_break_points_t get_break_points;
  debugger_update_code_t update_code;
  debugger_launch_t launch;
  debugger_set_break_point_t set_break_point;
  debugger_remove_break_point_t remove_break_point;
  debugger_clear_break_points_t clear_break_points;
  debugger_deinit_t deinit;

  debugger_get_state_t get_state;
  debugger_set_state_t set_state;

  /*扩展接口以支持lldb的DAP协议{*/
  debugger_get_threads_t get_threads;
  debugger_launch_app_t launch_app;
  debugger_get_var_t get_var;
  debugger_set_break_point_ex_t set_break_point_ex;
  debugger_remove_break_point_ex_t remove_break_point_ex;
  debugger_set_current_frame_t set_current_frame;
  debugger_dispatch_messages_t dispatch_messages;
  /*}扩展接口以支持lldb的DAP协议{*/
} debugger_vtable_t;

/**
 * @class debugger_t
 * 调试器接口。
 *
 */
struct _debugger_t {
  tk_object_t object;

  /*private*/
  debugger_program_state_t state;
  const debugger_vtable_t* vt;
  uint32_t current_frame_index;
};

/**
 * @method debugger_lock
 * 锁定debugger对象。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_lock(debugger_t* debugger);

/**
 * @method debugger_unlock
 * 解锁debugger对象。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_unlock(debugger_t* debugger);

/**
 * @method debugger_stop
 * 终止程序运行。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_stop(debugger_t* debugger);

/**
 * @method debugger_pause
 * 暂停运行。
 * > 暂停后才能执行next/step_xxx等函数。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_pause(debugger_t* debugger);

/**
 * @method debugger_restart
 * 重新运行。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_restart(debugger_t* debugger);

/**
 * @method debugger_is_paused
 * 查看当前是否处于暂停状态。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {bool_t} 返回TRUE表示处于暂停运行状态。
 */
bool_t debugger_is_paused(debugger_t* debugger);

/**
 * @method debugger_is_running
 * 查看当前是否处于运行状态。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {bool_t} 返回TRUE表示处于暂停运行状态。
 */
bool_t debugger_is_running(debugger_t* debugger);

/**
 * @method debugger_is_paused_or_running 
 * 查看当前是否处于暂停运行状态。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {bool_t} 返回TRUE表示处于暂停运行状态。
 */
bool_t debugger_is_paused_or_running(debugger_t* debugger);

/**
 * @method debugger_match
 * 检查code_id是否与当前debugger匹配。
 *
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} code_id 代码ID。
 *
 * @return {bool_t} 返回TRUE表示匹配到。
 */
bool_t debugger_match(debugger_t* debugger, const char* code_id);

/**
 * @method debugger_step_over
 * 执行到下一行代码。（不进入函数）
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_step_over(debugger_t* debugger);

/**
 * @method debugger_step_in
 * 进入函数。
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_step_in(debugger_t* debugger);

/**
 * @method debugger_step_out
 * 执行到函数结束。
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_step_out(debugger_t* debugger);

/**
 * @method debugger_step_loop_over
 * 执行下一条语句(跳过循环)
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_step_loop_over(debugger_t* debugger);

/**
 * @method debugger_continue
 * 执行到下一个断点。
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_continue(debugger_t* debugger);

/**
 * @method debugger_get_local
 * 获取局部变量对象。
 * 返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 * @param {uint32_t} frame_index frame序数(0开始)。
 *
 * @return {tk_object_t*} 返回局部变量对象。
 */
tk_object_t* debugger_get_local(debugger_t* debugger, uint32_t frame_index);

/**
 * @method debugger_get_self
 * 获取self对象。
 * 返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {tk_object_t*} 返回self对象。
 */
tk_object_t* debugger_get_self(debugger_t* debugger);

/**
 * @method debugger_get_global
 * 获取全局对象。
 * > 处于暂停状态才能执行本命令。
 * 返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {tk_object_t*} 返回全局对象。
 */
tk_object_t* debugger_get_global(debugger_t* debugger);

/**
 * @method debugger_get_callstack
 * 获取callstack。
 * @param {debugger_t*} debugger debugger对象。
 * @param {binary_data_t*} callstack callstack。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_get_callstack(debugger_t* debugger, binary_data_t* callstack);

/**
 * @method debugger_clear_break_points
 * 清除全部断点。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_clear_break_points(debugger_t* debugger);

/**
 * @method debugger_set_break_point
 * 设置断点。
 * @param {debugger_t*} debugger debugger对象。
 * @param {uint32_t} line 代码行号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_set_break_point(debugger_t* debugger, uint32_t line);

/**
 * @method debugger_remove_break_point
 * 清除断点。
 * @param {debugger_t*} debugger debugger对象。
 * @param {uint32_t} line 代码行号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_remove_break_point(debugger_t* debugger, uint32_t line);

/**
 * @method debugger_attach
 * 附近到指定的代码片段。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} lang 代码的语言。
 * @param {const char*} code_id 代码的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_attach(debugger_t* debugger, const char* lang, const char* code_id);

/**
 * @method debugger_launch
 * 执行代码。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} lang 代码的语言。
 * @param {const binary_data_t*} code 代码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_launch(debugger_t* debugger, const char* lang, const binary_data_t* code);

/**
 * @method debugger_deinit
 * 释放资源。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_deinit(debugger_t* debugger);

/**
 * @method debugger_update_code
 * 更新代码。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const binary_data_t*} code 代码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_update_code(debugger_t* debugger, const binary_data_t* code);

/**
 * @method debugger_get_code
 * 获取代码。
 * @param {debugger_t*} debugger debugger对象。
 * @param {binary_data_t*} code 代码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_get_code(debugger_t* debugger, binary_data_t* code);

/**
 * @method debugger_get_debuggers
 * 获取调试器列表。
 * @param {debugger_t*} debugger debugger对象。
 * @param {binary_data_t*} debuggers 调试器列表(每行一个)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_get_debuggers(debugger_t* debugger, binary_data_t* debuggers);

/**
 * @method debugger_get_break_points
 * 获取断点列表。
 * @param {debugger_t*} debugger debugger对象。
 * @param {binary_data_t*} break_points 断点列表(每行一个)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_get_break_points(debugger_t* debugger, binary_data_t* break_points);

/*扩展接口以支持lldb的DAP协议{*/
/**
 * @method debugger_get_threads
 * 获取线程(仅用于调试原生程序，脚本不支持)。
 * 返回数据结构请参考: https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Threads
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {tk_object_t*} 返回全局对象。
 */
tk_object_t* debugger_get_threads(debugger_t* debugger);

/*
 * @method debugger_launch_app
 * 执行程序(仅用于调试原生程序，脚本不支持)。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} program 程序。
 * @param {const char*} work_dir 工作目录。
 * @param {int} argc 参数个数。
 * @param {char**} argv 参数列表。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_launch_app(debugger_t* debugger, const char* program, const char* work_dir, int argc,
                          char* argv[]);

/**
 * @method debugger_set_break_point_ex
 * 设置断点。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} position 位置(函数名或文件名:行号)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_set_break_point_ex(debugger_t* debugger, const char* position);

/**
 * @method debugger_remove_break_point_ex
 * 清除断点。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} position 位置(函数名或文件名:行号)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_remove_break_point_ex(debugger_t* debugger, const char* position);

/**
 * @method debugger_get_var
 * 获取变量的详细信息。
 * 返回数据结构请参考：https://microsoft.github.io/debug-adapter-protocol/specification#Requests_Variables
 *
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} path 变量名或路径。为空或NULL时，返回全部变量。
 * > 可以为路径，如: 
 * > * a.name 为结构a下的成员name。
 * > * a.names[1].first为结构a下的成员names数组中第二个元素下的first成员。
 *
 * @return {tk_object_t*} 返回全局对象。
 */
tk_object_t* debugger_get_var(debugger_t* debugger, const char* path);

/**
 * @method debugger_set_current_frame
 * 从callstack中选择当前的frame。
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 * @param {uint32_t} frame_index frame序数(0表示当前)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_set_current_frame(debugger_t* debugger, uint32_t frame_index);

/**
 * @method debugger_dispatch_messages
 * dispatch_messages(仅适用于客户端)
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_dispatch_messages(debugger_t* debugger);

/**
 * @method debugger_set_state
 * 设置状态。
 * @param {debugger_t*} debugger debugger对象。
 * @param {debugger_program_state_t} state 状态。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_set_state(debugger_t* debugger, debugger_program_state_t state);

/**
 * @method debugger_get_state
 * 获取调试状态。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {debugger_program_state_t} 返回调试状态。
 */
debugger_program_state_t debugger_get_state(debugger_t* debugger);

/*}扩展接口以支持lldb的DAP协议{*/

#define DEBUGGER(debugger) ((debugger_t*)(debugger))

#define DEBUGGER_PROP_CODE_ID "code_id"

END_C_DECLS

#endif /*TK_DEBUGGER_H*/

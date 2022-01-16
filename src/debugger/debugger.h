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

typedef ret_t (*debugger_lock_t)(debugger_t* debugger);
typedef ret_t (*debugger_unlock_t)(debugger_t* debugger);
typedef ret_t (*debugger_stop_t)(debugger_t* debugger);
typedef ret_t (*debugger_pause_t)(debugger_t* debugger);
typedef bool_t (*debugger_match_t)(debugger_t* debugger, const char* code_id);
typedef bool_t (*debugger_is_paused_t)(debugger_t* debugger);

typedef ret_t (*debugger_next_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_in_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_out_t)(debugger_t* debugger);
typedef ret_t (*debugger_step_over_t)(debugger_t* debugger);
typedef ret_t (*debugger_continue_t)(debugger_t* debugger);
typedef tk_object_t* (*debugger_get_local_t)(debugger_t* debugger, uint32_t frame_index);
typedef tk_object_t* (*debugger_get_self_t)(debugger_t* debugger);
typedef tk_object_t* (*debugger_get_global_t)(debugger_t* debugger);
typedef ret_t (*debugger_get_callstack_t)(debugger_t* debugger, binary_data_t* callstack);
typedef ret_t (*debugger_clear_break_points_t)(debugger_t* debugger);
typedef ret_t (*debugger_set_break_point_t)(debugger_t* debugger, uint32_t line);
typedef ret_t (*debugger_remove_break_point_t)(debugger_t* debugger, uint32_t line);
typedef ret_t (*debugger_get_code_t)(debugger_t* debugger, binary_data_t* code);
typedef ret_t (*debugger_update_code_t)(debugger_t* debugger, const binary_data_t* code);
typedef ret_t (*debugger_init_t)(debugger_t* debugger, const char* lang, const char* code_id);
typedef ret_t (*debugger_deinit_t)(debugger_t* debugger);

typedef debugger_t* (*debugger_fscript_create_t)(void);

typedef struct _debugger_vtable_t {
  const char* lang;

  debugger_init_t init;
  debugger_lock_t lock;
  debugger_unlock_t unlock;
  debugger_stop_t stop;
  debugger_pause_t pause;
  debugger_match_t match;
  debugger_is_paused_t is_paused;

  debugger_next_t next;
  debugger_step_in_t step_in;
  debugger_step_out_t step_out;
  debugger_step_over_t step_over;
  debugger_continue_t continve;
  debugger_get_local_t get_local;
  debugger_get_self_t get_self;
  debugger_get_global_t get_global;
  debugger_get_callstack_t get_callstack;
  debugger_get_code_t get_code;
  debugger_update_code_t update_code;
  debugger_set_break_point_t set_break_point;
  debugger_remove_break_point_t remove_break_point;
  debugger_clear_break_points_t clear_break_points;
  debugger_deinit_t deinit;
} debugger_vtable_t;

/**
 * @class debugger_t
 * 调试器接口。
 *
 */
struct _debugger_t {
  tk_object_t object;
  const debugger_vtable_t* vt;
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
 * 停止运行。
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
 * @method debugger_is_paused
 * 查看当前是否处于暂停运行状态。
 *
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {bool_t} 返回TRUE表示处于暂停运行状态。
 */
bool_t debugger_is_paused(debugger_t* debugger);

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
 * @method debugger_next
 * 执行到下一行代码。
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_next(debugger_t* debugger);

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
 * @method debugger_step_over
 * 执行下一条语句(跳过循环)
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_step_over(debugger_t* debugger);

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
 * > 处于暂停状态才能执行本命令。
 * @param {debugger_t*} debugger debugger对象。
 * @param {uint32_t} frame_index frame序数(0表示当前)
 *
 * @return {tk_object_t*} 返回局部变量对象。
 */
tk_object_t* debugger_get_local(debugger_t* debugger, uint32_t frame_index);

/**
 * @method debugger_get_self
 * 获取self对象。
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
 * @method debugger_init
 * 初始化
 * @param {debugger_t*} debugger debugger对象。
 * @param {const char*} lang 代码的语言。
 * @param {const char*} code_id 代码的ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_init(debugger_t* debugger, const char* lang, const char* code_id);

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

#define DEBUGGER(debugger) ((debugger_t*)(debugger))

END_C_DECLS

#endif /*TK_DEBUGGER_H*/

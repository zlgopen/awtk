/**
 * File:   state_machine.h
 * Author: AWTK Develop Team
 * Brief:  state_machine
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-29 Wang LinFu <wanglinfu@zlg.cn> 修改自 https://github.com/misje/stateMachine.git
 *
 */
#ifndef TK_STATE_MACHINE_H
#define TK_STATE_MACHINE_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef struct _tk_sm_state_t tk_sm_state_t;
typedef struct _tk_sm_event_t tk_sm_event_t;

/**
 * @class tk_sm_event_t
 * 状态机事件
 */
struct _tk_sm_event_t {
  int type;
  void* data;
};

/**
 * @class tk_sm_transition_t
 * 状态转换条件
 */
typedef struct {
  int etype;
  void* condition;
  bool_t (*guard)(void* condition, tk_sm_event_t* event);
  void (*action)(void* current_stat_data, tk_sm_event_t* event, void* new_state_data);
  const tk_sm_state_t* next_state;
} tk_sm_transition_t;

/**
 * @class tk_sm_state_t
 * 状态结构体
 */
struct _tk_sm_state_t {
  const tk_sm_state_t* base_state;
  tk_sm_transition_t* transitions;
  size_t ntransitions;
  tk_sm_transition_t* transitions_back;
  size_t ntransitions_back;
  void* data;
  void (*entry_action)(void* state_data, tk_sm_event_t* event);
  void (*exit_action)(void* state_data, tk_sm_event_t* event);
};

/**
 * @class tk_state_machine_t
 * 状态机结构体
 */
typedef struct {
  const tk_sm_state_t* current_state;
  const tk_sm_state_t* previous_state;
} tk_state_machine_t;

/**
 * @method tk_state_machine_init
 * 初始化
 *
 * @param {tk_state_machine_t*}  fsm  状态机地址
 * @param {const tk_sm_state_t*} initial_state 初始状态
 *
 * @return {ret_t} 返回 ret_t 值
 */
ret_t tk_state_machine_init(tk_state_machine_t* fsm, const tk_sm_state_t* initial_state);

/**
 * @method tk_state_machine_handle
 * 事件处理
 *
 * @param {tk_state_machine_t*} fsm 状态机地址
 * @param {tk_sm_event_t*}      event 事件
 *
 * @return {ret_t} 返回 ret_t 值
 */
ret_t tk_state_machine_handle(tk_state_machine_t* fsm, tk_sm_event_t* event);
/**
 * @method tk_state_machine_previous_get
 * 获取上一个状态
 *
 * @param {tk_state_machine_t*} fsm 状态机地址
 *
 * @return {tk_sm_state_t*} 返回 tk_sm_state_t 对象
 */
const tk_sm_state_t* tk_state_machine_previous_get(tk_state_machine_t* fsm);
/**
 * @method tk_state_machine_current_get
 * 获取当前状态
 *
 * @param {tk_state_machine_t*} fsm 状态机地址
 *
 * @return {tk_sm_state_t*} 返回 tk_sm_state_t 对象
 */
const tk_sm_state_t* tk_state_machine_current_get(tk_state_machine_t* fsm);
/**
 * @method tk_state_machine_deinit
 * 反初始化
 *
 * @param {tk_state_machine_t*} fsm 状态机地址
 * @param {tk_sm_event_t*}      event 事件
 *
 * @return {ret_t} 返回 ret_t 值
 */
ret_t tk_state_machine_deinit(tk_state_machine_t* fsm, tk_sm_event_t* event);

END_C_DECLS

#endif /*TK_STATE_MACHINE_H*/

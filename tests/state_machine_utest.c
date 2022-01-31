/**
 * File:   state_machine.c
 * Author: AWTK Develop Team
 * Brief:  state_machine
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-12-29 code generated
 * 2020-12-29 Wang LinFu <wanglinfu@zlg.cn> created
 *
 */
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/state_machine.h"

typedef enum {
  EVENT_IDLE,
  EVENT_A,
  EVENT_B,
  EVENT_C,
  EVENT_D,
  EVENT_E,
  EVENT_MAX_NUM,
} event_type_t;

static bool_t condition_check(void* condition, tk_sm_event_t* event) {
  str_t* str = ((str_t*)event->data);
  return tk_str_start_with(str->str, (const char*)condition);
}

static void on_base_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  ENSURE(tk_str_eq((const char*)old_state_data, "idle"));
  ENSURE(tk_str_eq((const char*)new_state_data, "idle"));
  str_append(str, "base");
}

static void on_idle_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  ENSURE(tk_str_eq((const char*)old_state_data, "idle"));
  str_append(str, ",idle");
}

static void on_common_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  if (tk_str_eq((char*)old_state_data, (char*)new_state_data)) {
    str_append(str, "common,");
  } else {
    str_append(str, "]");
  }
}

static void on_special_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  str_append(str, "special,");
}

static void on_finish_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  str_append(str, "]");
}

static void on_enter_action(void* stateData, tk_sm_event_t* event) {
  str_t* str = ((str_t*)event->data);
  str_append(str, ",[");
}

static void on_exit_action(void* stateData, tk_sm_event_t* event) {
  if (event) {
    str_t* str = ((str_t*)event->data);
    str_append(str, (const char*)stateData);
  }
}

static void on_special_exit(void* stateData, tk_sm_event_t* event) {
  if (event) {
    str_t* str = ((str_t*)event->data);
    str_pop(str);
    str_append(str, "]");
  } else {
    log_debug("deinit");
  }
}

static tk_sm_state_t idle_state, common_state, special_state;

static tk_sm_state_t base_state = {
    .transitions =
        (tk_sm_transition_t[]){
            {EVENT_IDLE, NULL, NULL, on_base_action, &idle_state},
        },
    .ntransitions = 1,
    .data = "base",
};

static tk_sm_state_t idle_state = {
    .base_state = &base_state,
    .transitions =
        (tk_sm_transition_t[]){
            {EVENT_A, "base", condition_check, on_idle_action, &idle_state},
            {EVENT_B, "base", condition_check, on_idle_action, &common_state},
        },
    .ntransitions = 2,
    .data = "idle",
};

static tk_sm_state_t common_state = {
    .base_state = &base_state,
    .transitions =
        (tk_sm_transition_t[]){
            {EVENT_C, NULL, NULL, on_common_action, &common_state},
            {EVENT_D, NULL, NULL, on_common_action, &special_state},
        },
    .ntransitions = 3,
    .data = "common",
    .entry_action = on_enter_action,
    .exit_action = on_exit_action,
};

static tk_sm_state_t special_state = {
    .base_state = &base_state,
    .transitions =
        (tk_sm_transition_t[]){
            {EVENT_E, NULL, NULL, on_special_action, NULL},
            {EVENT_MAX_NUM, NULL, NULL, on_special_action, NULL},
        },
    .ntransitions = 3,
    .data = "special",
    .entry_action = on_enter_action,
    .exit_action = on_special_exit,
};

void state_machine_utest_basic(void) {
  tk_state_machine_t sm;
  ret_t ret;
  int eventid = EVENT_IDLE;
  tk_sm_event_t ev;
  str_t s;
  str_t* str = str_init(&s, 32);
  ret = tk_state_machine_init(&sm, &idle_state);
  ENSURE(RET_OK == ret);

  ev.data = str;
  while (eventid <= EVENT_MAX_NUM) {
    ev.type = eventid;
    ret = tk_state_machine_handle(&sm, &ev);
    ENSURE(RET_OK == ret);

    eventid++;
  }
  ENSURE(tk_state_machine_deinit(&sm, &ev) == RET_OK);
  log_debug("result: %s", str->str);
  ENSURE(tk_str_eq(str->str, "base,idle,idle,[common,common],[special,special]"));
  str_reset(str);
  return;
}

#define MAX_STEP 4

static void on_step_exit_action(void* old_state_data, tk_sm_event_t* event, void* new_state_data) {
  str_t* str = ((str_t*)event->data);
  str_append(str, "-");
  str_append(str, old_state_data);
}

static void on_step_enter(void* stateData, tk_sm_event_t* event) {
  str_t* str = ((str_t*)event->data);
  str_append(str, "+");
  str_append(str, stateData);
}

static void on_step_exit(void* stateData, tk_sm_event_t* event) {
  if (event) {
    str_t* str = ((str_t*)event->data);
    str_append(str, "-");
    str_append(str, (const char*)stateData);
  } else {
    log_debug("deinit");
  }
}

static tk_sm_state_t step1_state, step2_state, step3_state;
static tk_sm_state_t step_init_state = {
    .transitions =
        (tk_sm_transition_t[]){
            {1, NULL, NULL, NULL, &step1_state},
            {MAX_STEP, NULL, NULL, NULL, &step_init_state},
        },
    .ntransitions = 2,
    .data = "0,",
};

static tk_sm_state_t step1_state = {
    .base_state = &step_init_state,
    .transitions =
        (tk_sm_transition_t[]){
            {2, NULL, NULL, NULL, &step2_state},
        },
    .ntransitions = 1,
    .transitions_back =
        (tk_sm_transition_t[]){
            {MAX_STEP, NULL, NULL, on_step_exit_action, &step_init_state},
        },
    .ntransitions_back = 1,
    .data = "1,",
    .entry_action = on_step_enter,
};

static tk_sm_state_t step2_state = {
    .base_state = &step_init_state,
    .transitions =
        (tk_sm_transition_t[]){
            {3, NULL, NULL, NULL, &step3_state},
        },
    .ntransitions = 1,
    .transitions_back =
        (tk_sm_transition_t[]){
            {MAX_STEP, NULL, NULL, on_step_exit_action, &step1_state},
        },
    .ntransitions_back = 1,
    .data = "2,",
    .entry_action = on_step_enter,
};

static tk_sm_state_t step3_state = {
    .base_state = &step_init_state,
    .transitions_back =
        (tk_sm_transition_t[]){
            {MAX_STEP, NULL, NULL, NULL, &step2_state},
        },
    .ntransitions_back = 1,
    .data = "3,",
    .entry_action = on_step_enter,
    .exit_action = on_step_exit,
};

void state_machine_utest_back_transition(void) {
  tk_state_machine_t sm;
  ret_t ret;
  int eventid = 1;
  tk_sm_event_t ev;
  str_t s;
  str_t* str = str_init(&s, 32);
  ret = tk_state_machine_init(&sm, &step_init_state);
  ENSURE(RET_OK == ret);

  ev.data = str;
  while (eventid < MAX_STEP) {
    ev.type = eventid;
    ret = tk_state_machine_handle(&sm, &ev);
    ENSURE(RET_OK == ret);

    eventid++;
  }

  ev.type = eventid;
  ret = tk_state_machine_handle(&sm, &ev);
  ENSURE(RET_OK == ret);
  ENSURE(tk_state_machine_deinit(&sm, &ev) == RET_OK);
  log_debug("result: %s", str->str);
  ENSURE(tk_str_eq(str->str, "+1,+2,+3,-3,-2,-1,"));
  str_reset(str);
  return;
}

void state_machine_utest(void) {
  state_machine_utest_basic();
  state_machine_utest_back_transition();
}

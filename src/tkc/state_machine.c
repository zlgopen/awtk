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
#include "state_machine.h"
#include "tkc/mem.h"

static tk_sm_transition_t* tk_state_machine_get_transition(tk_state_machine_t* fsm,
                                                           tk_sm_event_t* const event,
                                                           bool_t* is_back) {
  const tk_sm_state_t* state = fsm->current_state;
  size_t i;

  *is_back = FALSE;
  for (i = 0; i < state->ntransitions; ++i) {
    tk_sm_transition_t* t = &state->transitions[i];

    /* A transition for the given event has been found: */
    if (t->etype == event->type) {
      if (!t->guard) return t;
      /* If transition is guarded, ensure that the condition is held: */
      else if (t->guard(t->condition, event))
        return t;
    }
  }

  for (i = 0; i < state->ntransitions_back; ++i) {
    tk_sm_transition_t* t = &state->transitions_back[i];

    /* A transition for the given event has been found: */
    if (t->etype == event->type) {
      if (!t->guard) {
        *is_back = TRUE;
        return t;
      }
      /* If transition is guarded, ensure that the condition is held: */
      else if (t->guard(t->condition, event)) {
        *is_back = TRUE;
        return t;
      }
    }
  }
  if (state->base_state) {
    state = state->base_state;
    return_value_if_fail(state->transitions && state->ntransitions, NULL);
    for (i = 0; i < state->ntransitions; ++i) {
      tk_sm_transition_t* t = &state->transitions[i];
      if (!t->next_state) {
        t->next_state = state;
      }
      /* A transition for the given event has been found: */
      if (t->etype == event->type) {
        if (!t->guard) return t;
        /* If transition is guarded, ensure that the condition is held: */
        else if (t->guard(t->condition, event))
          return t;
      }
    }
  }

  /* No transitions found for given event for given state: */
  return NULL;
}

ret_t tk_state_machine_init(tk_state_machine_t* fsm, const tk_sm_state_t* initial_state) {
  return_value_if_fail(fsm, RET_BAD_PARAMS);
  return_value_if_fail(initial_state, RET_BAD_PARAMS);

  /* 初始状态不允许有 transitions_back ，防止无限递归 */
  return_value_if_fail(initial_state->ntransitions_back == 0, RET_BAD_PARAMS);
  return_value_if_fail(initial_state->transitions_back == NULL, RET_BAD_PARAMS);

  fsm->current_state = initial_state;
  fsm->previous_state = NULL;
  return RET_OK;
}

ret_t tk_state_machine_handle(tk_state_machine_t* fsm, tk_sm_event_t* event) {
  return_value_if_fail(fsm && event, RET_BAD_PARAMS);
  return_value_if_fail(fsm->current_state, RET_NOT_IMPL);

  const tk_sm_state_t* next_state;
  bool_t is_back;
  tk_sm_transition_t* transition = tk_state_machine_get_transition(fsm, event, &is_back);
  return_value_if_fail(transition, RET_NOT_FOUND);

  if (transition->next_state) {
    next_state = transition->next_state;
  } else {
    next_state = fsm->current_state;
  }

  if (next_state != fsm->current_state && fsm->current_state->exit_action) {
    fsm->current_state->exit_action(fsm->current_state->data, event);
  }

  /* action 在 exit 之后，如果要在 exit 之前执行的操作，可以放在 guard 中 */
  if (transition->action) {
    transition->action(fsm->current_state->data, event, next_state->data);
  }
  if (!is_back) {
    if (next_state != fsm->current_state && next_state->entry_action) {
      next_state->entry_action(next_state->data, event);
    }
    fsm->previous_state = fsm->current_state;
    fsm->current_state = next_state;
    return RET_OK;
  } else {
    return_value_if_fail(next_state != fsm->current_state, RET_FAIL);
    fsm->previous_state = fsm->current_state;
    fsm->current_state = next_state;
    return tk_state_machine_handle(fsm, event);
  }
}

const tk_sm_state_t* tk_state_machine_current_get(tk_state_machine_t* fsm) {
  return_value_if_fail(fsm, NULL);
  return fsm->current_state;
}

const tk_sm_state_t* tk_state_machine_previous_get(tk_state_machine_t* fsm) {
  return_value_if_fail(fsm, NULL);
  return fsm->previous_state;
}

ret_t tk_state_machine_deinit(tk_state_machine_t* fsm, tk_sm_event_t* event) {
  return_value_if_fail(fsm, RET_BAD_PARAMS);
  if (fsm->current_state->exit_action) {
    fsm->current_state->exit_action(fsm->current_state->data, event);
  }
  memset(fsm, 0, sizeof(*fsm));
  return RET_OK;
}

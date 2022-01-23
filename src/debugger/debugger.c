/**
 * File:   debugger.c
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

#include "debugger/debugger.h"

ret_t debugger_lock(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->lock != NULL, RET_BAD_PARAMS);

  return debugger->vt->lock(debugger);
}

ret_t debugger_unlock(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->unlock != NULL, RET_BAD_PARAMS);

  return debugger->vt->unlock(debugger);
}

ret_t debugger_stop(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->stop != NULL, RET_BAD_PARAMS);

  return debugger->vt->stop(debugger);
}

ret_t debugger_pause(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->pause != NULL, RET_BAD_PARAMS);

  return debugger->vt->pause(debugger);
}

bool_t debugger_is_paused(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, FALSE);
  return_value_if_fail(debugger->vt->is_paused != NULL, FALSE);

  return debugger->vt->is_paused(debugger);
}

bool_t debugger_match(debugger_t* debugger, const char* code_id) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, FALSE);
  return_value_if_fail(debugger->vt->match != NULL && code_id != NULL, FALSE);

  return debugger->vt->match(debugger, code_id);
}

ret_t debugger_next(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->next != NULL, RET_BAD_PARAMS);

  return debugger->vt->next(debugger);
}

ret_t debugger_step_in(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_in != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_in(debugger);
}

ret_t debugger_step_out(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_out != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_out(debugger);
}

ret_t debugger_step_over(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_over != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_over(debugger);
}

ret_t debugger_continue(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->continve != NULL, RET_BAD_PARAMS);

  return debugger->vt->continve(debugger);
}

tk_object_t* debugger_get_local(debugger_t* debugger, uint32_t frame_index) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_local != NULL, NULL);

  return debugger->vt->get_local(debugger, frame_index);
}

tk_object_t* debugger_get_self(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_self != NULL, NULL);

  return debugger->vt->get_self(debugger);
}

tk_object_t* debugger_get_global(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_global != NULL, NULL);

  return debugger->vt->get_global(debugger);
}

ret_t debugger_get_callstack(debugger_t* debugger, binary_data_t* callstack) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_callstack != NULL, RET_BAD_PARAMS);
  return_value_if_fail(callstack != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_callstack(debugger, callstack);
}

ret_t debugger_clear_break_points(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->clear_break_points != NULL, RET_BAD_PARAMS);

  return debugger->vt->clear_break_points(debugger);
}

ret_t debugger_set_break_point(debugger_t* debugger, uint32_t line) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->set_break_point != NULL, RET_BAD_PARAMS);

  return debugger->vt->set_break_point(debugger, line);
}

ret_t debugger_remove_break_point(debugger_t* debugger, uint32_t line) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->remove_break_point != NULL, RET_BAD_PARAMS);

  return debugger->vt->remove_break_point(debugger, line);
}

ret_t debugger_attach(debugger_t* debugger, const char* lang, const char* code_id) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->attach != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lang != NULL && code_id != NULL, RET_BAD_PARAMS);

  return debugger->vt->attach(debugger, lang, code_id);
}

ret_t debugger_get_code(debugger_t* debugger, binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_code != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_code(debugger, code);
}

ret_t debugger_get_debuggers(debugger_t* debugger, binary_data_t* debuggers) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_debuggers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debuggers != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_debuggers(debugger, debuggers);
}

ret_t debugger_get_break_points(debugger_t* debugger, binary_data_t* break_points) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_break_points != NULL, RET_BAD_PARAMS);
  return_value_if_fail(break_points != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_break_points(debugger, break_points);
}

ret_t debugger_update_code(debugger_t* debugger, const binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->update_code != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL && code->data != NULL, RET_BAD_PARAMS);

  return debugger->vt->update_code(debugger, code);
}

ret_t debugger_launch(debugger_t* debugger, const char* lang, const binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->launch != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL && code->data != NULL && lang != NULL, RET_BAD_PARAMS);

  return debugger->vt->launch(debugger, lang, code);
}

ret_t debugger_deinit(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->deinit != NULL, RET_BAD_PARAMS);

  return debugger->vt->deinit(debugger);
}

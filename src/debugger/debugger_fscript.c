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

#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_fscript.h"

static ret_t debugger_fscript_lock(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  log_debug("debugger_fscript_lock\n");
  return tk_mutex_nest_lock(d->mutex);
}

static ret_t debugger_fscript_unlock(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  log_debug("debugger_fscript_unlock\n");
  return tk_mutex_nest_unlock(d->mutex);
}

static ret_t debugger_fscript_stop(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL) {
      /*TODO*/
    }
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_pause(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused == FALSE) {
      ret = RET_OK;
      /*停止到下一行要执行的代码*/
      d->next_stop_executed_line = d->executed_lines + 1;
    }
    ret = d->paused == TRUE ? RET_OK : RET_FAIL;
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static bool_t debugger_fscript_match(debugger_t* debugger, const char* code_id) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, FALSE);

  return tk_str_eq(d->code_id, code_id);
}

static bool_t debugger_fscript_is_paused(debugger_t* debugger) {
  bool_t ret = FALSE;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, FALSE);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = d->paused;
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_next(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      d->next_stop_executed_line = d->executed_lines + 1;
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_step_in(debugger_t* debugger) {
  /*TODO*/
  return debugger_fscript_next(debugger);
}

static ret_t debugger_fscript_step_out(debugger_t* debugger) {
  /*TODO*/
  return debugger_fscript_next(debugger);
}

static ret_t debugger_fscript_step_over(debugger_t* debugger) {
  /*TODO*/
  return debugger_fscript_next(debugger);
}

static ret_t debugger_fscript_continue(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      d->next_stop_line = -1;
      d->next_stop_executed_line = -1;
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_get_code(debugger_t* debugger, binary_data_t* code) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, RET_BAD_PARAMS);

  code->data = d->code.str;
  code->size = d->code.size + 1;

  return RET_OK;
}

tk_object_t* debugger_fscript_get_local(debugger_t* debugger, uint32_t frame_index) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, NULL);
  /*FIXME: 目前只支持当前*/
  return TK_OBJECT_REF(d->fscript->locals);
}

tk_object_t* debugger_fscript_get_self(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, NULL);

  return TK_OBJECT_REF(d->fscript->obj);
}

tk_object_t* debugger_fscript_get_global(debugger_t* debugger) {
  return TK_OBJECT_REF(fscript_get_global_object());
}

static ret_t debugger_fscript_get_callstack(debugger_t* debugger, binary_data_t* callstack) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, RET_BAD_PARAMS);

  callstack->data = d->callstack.str;
  callstack->size = d->callstack.size + 1;

  return RET_OK;
}

static ret_t debugger_fscript_clear_break_points(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  bool_t paused = FALSE;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    d->next_stop_line = -1;
    d->next_stop_executed_line = -1;
    ret = darray_clear(&(d->break_points));

    paused = d->paused;
    debugger_fscript_unlock(debugger);
  }

  if (paused) {
    tk_cond_var_awake(d->cond_var);
  }

  return ret;
}

static ret_t debugger_fscript_set_break_point(debugger_t* debugger, uint32_t line) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = darray_push_unique(&(d->break_points), tk_pointer_from_int(line));
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_remove_break_point(debugger_t* debugger, uint32_t line) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = darray_remove(&(d->break_points), tk_pointer_from_int(line));
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_init(debugger_t* debugger, const char* lang, const char* code_id) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = RET_OK;
    d->code_id = tk_str_copy(d->code_id, code_id);
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_deinit(debugger_t* debugger) {
  debugger_fscript_clear_break_points(debugger);

  return RET_OK;
}

static ret_t debugger_fscript_update_code(debugger_t* debugger, const binary_data_t* code) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = RET_OK;
    str_set_with_len(&(d->code), (char*)(code->data), code->size);
    /*TODO*/
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static const debugger_vtable_t s_debugger_fscript_vtable = {
    .lang = DEBUGGER_LANG_FSCRIPT,
    .init = debugger_fscript_init,
    .lock = debugger_fscript_lock,
    .unlock = debugger_fscript_unlock,
    .stop = debugger_fscript_stop,
    .pause = debugger_fscript_pause,
    .match = debugger_fscript_match,
    .is_paused = debugger_fscript_is_paused,
    .next = debugger_fscript_next,
    .step_in = debugger_fscript_step_in,
    .step_out = debugger_fscript_step_out,
    .step_over = debugger_fscript_step_over,
    .continve = debugger_fscript_continue,
    .get_local = debugger_fscript_get_local,
    .get_self = debugger_fscript_get_self,
    .get_global = debugger_fscript_get_global,
    .get_callstack = debugger_fscript_get_callstack,
    .get_code = debugger_fscript_get_code,
    .update_code = debugger_fscript_update_code,
    .set_break_point = debugger_fscript_set_break_point,
    .remove_break_point = debugger_fscript_remove_break_point,
    .clear_break_points = debugger_fscript_clear_break_points,
    .deinit = debugger_fscript_deinit,
};

debugger_fscript_t* debugger_fscript_cast(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt == &s_debugger_fscript_vtable, NULL);

  return (debugger_fscript_t*)debugger;
}

static ret_t debugger_fscript_on_destroy(tk_object_t* obj) {
  debugger_fscript_t* debugger = DEBUGGER_FSCRIPT(obj);
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  tk_mutex_nest_destroy(debugger->mutex);
  debugger->mutex = NULL;
  tk_cond_var_destroy(debugger->cond_var);
  debugger->cond_var = NULL;

  TKMEM_FREE(debugger->code_id);
  str_reset(&(debugger->code));
  str_reset(&(debugger->callstack));
  darray_deinit(&(debugger->break_points));

  return RET_OK;
}

static const object_vtable_t s_object_debugger_fscript_vtable = {
    .type = "object_debugger_fscript",
    .desc = "object_debugger_fscript",
    .size = sizeof(debugger_fscript_t),
    .is_collection = FALSE,
    .on_destroy = debugger_fscript_on_destroy};

debugger_t* debugger_fscript_create(void) {
  debugger_fscript_t* debugger = NULL;
  debugger = (debugger_fscript_t*)tk_object_create(&s_object_debugger_fscript_vtable);
  return_value_if_fail(debugger != NULL, NULL);

  debugger->mutex = tk_mutex_nest_create();
  debugger->cond_var = tk_cond_var_create();
  debugger->debugger.vt = &s_debugger_fscript_vtable;

  str_init(&(debugger->code), 100);
  str_init(&(debugger->callstack), 100);
  darray_init(&(debugger->break_points), 10, NULL, NULL);

  return (debugger_t*)debugger;
}

ret_t debugger_fscript_print_func(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t str;
  char buff[32];
  uint32_t i = 0;
  debugger_log_event_t event;
  debugger_t* debugger = NULL;

  if (fscript != NULL && fscript->code_id != NULL) {
    debugger = debugger_server_find_debugger(fscript->code_id);
  }

  if (debugger != NULL) {
    uint32_t line = fscript->curr->row;
    value_set_bool(result, TRUE);
    str_init(&str, 100);
    for (i = 0; i < args->size; i++) {
      str_append(&str, value_str_ex(args->args + i, buff, sizeof(buff) - 1));
    }

    debugger_log_event_init(&event, line, str.str);
    emitter_dispatch(EMITTER(debugger), (event_t*)&event);
    str_reset(&str);
  }

  return RET_OK;
}

static ret_t debugger_fscript_on_error(void* ctx, fscript_t* fscript) {
  debugger_error_event_t event;
  debugger_t* debugger = DEBUGGER(ctx);

  debugger_error_event_init(&event, fscript->error_row, fscript->error_message);
  emitter_dispatch(EMITTER(debugger), (event_t*)&event);

  return RET_OK;
}

ret_t debugger_fscript_set_fscript(debugger_t* debugger, fscript_t* fscript) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  d->fscript = fscript;
  if (fscript) {
    d->executed_lines = 0;
    d->last_executed_line = 0;
    d->next_stop_line = -1;
    d->prev_breaked_line = -1;
    d->next_stop_executed_line = -1;
    str_set(&(d->callstack), "<root>\n");
    fscript_set_print_func(fscript, debugger_fscript_print_func);
    fscript_set_on_error(fscript, debugger_fscript_on_error, d);
  } else {
    emitter_dispatch_simple_event(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED);
  }

  return RET_OK;
}

static ret_t debugger_fscript_before_exec_func(debugger_t* debugger, int32_t line) {
  bool_t paused = FALSE;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    bool_t bp = darray_find_index(&(d->break_points), tk_pointer_from_int(line)) >= 0;

    d->paused = bp || line == d->next_stop_line || d->executed_lines == d->next_stop_executed_line;
    paused = d->paused && (d->prev_breaked_line != line);

    if (paused) {
      debugger_breaked_event_t event;
      debugger_breaked_event_init(&event, line);
      emitter_dispatch(EMITTER(debugger), (event_t*)&event);
      d->prev_breaked_line = line;
    }
    debugger_fscript_unlock(debugger);

    if (paused) {
      log_debug("debugger wait for instruction\n");
      tk_cond_var_wait(d->cond_var, 0xffffff);
    }
  }

  return RET_OK;
}

static ret_t debugger_fscript_after_exec_func(debugger_t* debugger, int32_t line) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->last_executed_line != line) {
      d->executed_lines++;
    }
    d->last_executed_line = line;
    debugger_fscript_unlock(debugger);
  }

  return RET_OK;
}

ret_t debugger_fscript_exec_func(fscript_t* fscript, const char* name, fscript_func_call_t* iter,
                                 value_t* result) {
  ret_t ret = RET_FAIL;
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  if (fscript != NULL && fscript->code_id != NULL) {
    debugger = debugger_server_find_debugger(fscript->code_id);
  }

  if (debugger == NULL) {
    return fscript_exec_func_default(fscript, iter, result);
  } else {
    int32_t line = iter->row;

    debugger_fscript_before_exec_func(debugger, line);
    ret = fscript_exec_func_default(fscript, iter, result);
    debugger_fscript_after_exec_func(debugger, line);
  }

  return ret;
}

ret_t debugger_fscript_set_var(fscript_t* fscript, const char* name, const value_t* v) {
  ret_t ret = RET_FAIL;
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  if (fscript != NULL && fscript->code_id != NULL) {
    debugger = debugger_server_find_debugger(fscript->code_id);
  }

  if (debugger == NULL) {
    return fscript_set_var_default(fscript, name, v);
  } else {
    int32_t line = fscript->curr->row;

    debugger_fscript_before_exec_func(debugger, line);
    ret = fscript_set_var_default(fscript, name, v);
    debugger_fscript_after_exec_func(debugger, line);
  }

  return ret;
}

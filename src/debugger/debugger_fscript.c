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

#include "tkc/object_default.h"
#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_fscript.h"

typedef struct _call_stack_frame_t {
  const char* name;
  tk_object_t* locals;
} call_stack_frame_t;

static call_stack_frame_t* call_stack_frame_create(const char* name, tk_object_t* locals) {
  call_stack_frame_t* frame = TKMEM_ZALLOC(call_stack_frame_t);
  return_value_if_fail(frame != NULL, NULL);

  frame->name = name;
  frame->locals = locals;

  return frame;
}

static ret_t call_stack_frame_destroy(call_stack_frame_t* frame) {
  return_value_if_fail(frame != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(frame);

  return RET_OK;
}

static ret_t debugger_fscript_enter_func(debugger_t* debugger, const char* name,
                                         tk_object_t* locals) {
  call_stack_frame_t* frame = NULL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  frame = call_stack_frame_create(name, locals);
  return_value_if_fail(frame != NULL, RET_OOM);

  if (darray_push(&(d->call_stack_frames), frame) != RET_OK) {
    call_stack_frame_destroy(frame);
    return RET_OOM;
  } else {
    log_debug("enter %s\n", name);
    return RET_OK;
  }
}

static ret_t debugger_fscript_leave_func(debugger_t* debugger) {
  call_stack_frame_t* frame = NULL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  frame = (call_stack_frame_t*)darray_pop(&(d->call_stack_frames));
  return_value_if_fail(frame != NULL, RET_BAD_PARAMS);

  log_debug("leave %s\n", frame->name);

  return call_stack_frame_destroy(frame);
}

static ret_t debugger_fscript_lock(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  return tk_mutex_nest_lock(d->mutex);
}

static ret_t debugger_fscript_unlock(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

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

static ret_t debugger_fscript_clear_step_stops(debugger_fscript_t* d) {
  d->next_stop_line = -1;
  d->next_stop_executed_line = -1;
  d->next_stop_call_frame_index = -1;

  return RET_OK;
}

static ret_t debugger_fscript_next(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
      d->next_stop_executed_line = d->executed_lines + 1;
      log_debug("debugger_fscript_next: %d %d\n", d->executed_lines, d->next_stop_executed_line);
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_step_in(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
      d->next_stop_call_frame_index = d->call_stack_frames.size + 1;
      log_debug("set step in:%d\n", d->next_stop_call_frame_index);
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_step_out(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);
  return_value_if_fail(d->call_stack_frames.size > 0, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
      d->next_stop_call_frame_index = d->call_stack_frames.size - 1;
      log_debug("set step out:%d\n", d->next_stop_call_frame_index);
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_step_over(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
      d->next_stop_line = d->prev_breaked_line + 1;
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
  }

  return ret;
}

static ret_t debugger_fscript_continue(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
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
  uint32_t index = 0;
  call_stack_frame_t* frame = NULL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, NULL);
  return_value_if_fail(d->call_stack_frames.size > frame_index, NULL);

  index = d->call_stack_frames.size - frame_index - 1;
  frame = (call_stack_frame_t*)darray_get(&(d->call_stack_frames), index);
  return_value_if_fail(frame != NULL, NULL);

  return TK_OBJECT_REF(frame->locals);
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
  uint32_t i = 0;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, RET_BAD_PARAMS);

  str_clear(&(d->callstack));
  for (i = 0; i < d->call_stack_frames.size; i++) {
    call_stack_frame_t* iter = (call_stack_frame_t*)darray_get(&(d->call_stack_frames), i);
    str_append_more(&(d->callstack), iter->name, "\n", NULL);
  }

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
    debugger_fscript_clear_step_stops(d);
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
    d->code_changed = TRUE;
    str_set_with_len(&(d->code), (char*)(code->data), code->size);
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
  darray_deinit(&(debugger->call_stack_frames));

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
  darray_init(&(debugger->call_stack_frames), 10, (tk_destroy_t)call_stack_frame_destroy, NULL);

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

  if (fscript) {

    if (d->code_changed) {
      d->code_changed = FALSE;
      return_value_if_fail(fscript_reload(fscript, d->code.str) == RET_OK, RET_FAIL);
    }

    d->fscript = fscript;
    d->executed_lines = 0;
    d->last_executed_line = 0;
    d->prev_breaked_line = -1;
    debugger_fscript_clear_step_stops(d);

    fscript_set_print_func(fscript, debugger_fscript_print_func);
    fscript_set_on_error(fscript, debugger_fscript_on_error, d);

    if (fscript->locals == NULL) {
      /*fscript本身是延迟创建locals的，这会导致debugger拿不到locals对象，所以提前创建它。*/
      fscript->locals = object_default_create();
    }

    darray_clear(&(d->call_stack_frames));
    debugger_fscript_enter_func(debugger, "<root>", fscript->locals);
  } else {
    debugger_fscript_leave_func(debugger);
    fscript_set_print_func(d->fscript, NULL);
    fscript_set_on_error(d->fscript, NULL, NULL);
    emitter_dispatch_simple_event(EMITTER(debugger), DEBUGGER_RESP_MSG_COMPLETED);
    d->fscript = NULL;
  }

  return RET_OK;
}

static bool_t debugger_fscript_should_pause(debugger_fscript_t* d, int32_t line) {
  bool_t paused = darray_find_index(&(d->break_points), tk_pointer_from_int(line)) >= 0;

  if (paused) {
    log_debug("break at line:%d\n", line);
  }

  if (d->next_stop_executed_line >= 0 && d->executed_lines >= d->next_stop_executed_line) {
    log_debug("break for step next:next_stop_executed_line=%d line=%d\n",
              d->next_stop_executed_line, line);
    paused = TRUE;
  }

  if (line == d->next_stop_line) {
    log_debug("break for over:next_stop_line=%d line=%d\n", d->next_stop_line, line);
    paused = TRUE;
  }

  if (d->next_stop_call_frame_index == d->call_stack_frames.size) {
    log_debug("break for step in:%d\n", d->call_stack_frames.size);
    paused = TRUE;
  }

  d->paused = paused;

  if (d->prev_breaked_line == line) {
    paused = FALSE;
    log_debug("d->prev_breaked_line == line\n");
  } else {
    d->prev_breaked_line = -1;
  }

  return paused;
}

static ret_t debugger_fscript_before_exec_func(debugger_t* debugger, int32_t line) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    bool_t paused = debugger_fscript_should_pause(d, line);

    if (paused) {
      debugger_breaked_event_t event;
      debugger_breaked_event_init(&event, line);
      emitter_dispatch(EMITTER(debugger), (event_t*)&event);
      d->prev_breaked_line = line;
    }
    debugger_fscript_unlock(debugger);

    if (paused) {
      log_debug("debugger wait for instruction begin\n");
      tk_cond_var_wait(d->cond_var, 0xffffff);
      log_debug("debugger wait for instruction done\n");
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

    /*name不为NUL则是脚本函数*/
    if (name != NULL) {
      if (debugger_fscript_lock(debugger) == RET_OK) {
        debugger_fscript_enter_func(debugger, name, fscript->locals);
        debugger_fscript_unlock(debugger);
      }
    }

    debugger_fscript_before_exec_func(debugger, line);
    ret = fscript_exec_func_default(fscript, iter, result);
    debugger_fscript_after_exec_func(debugger, line);

    if (name != NULL) {
      if (debugger_fscript_lock(debugger) == RET_OK) {
        bool_t paused = FALSE;
        debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
        debugger_fscript_leave_func(debugger);

        if (d->next_stop_call_frame_index == d->call_stack_frames.size) {
          paused = TRUE;
          d->paused = paused;
          log_debug("stop for step out\n");
        }
        debugger_fscript_unlock(debugger);

        if (paused) {
          log_debug("debugger wait for instruction begin\n");
          tk_cond_var_wait(d->cond_var, 0xffffff);
          log_debug("debugger wait for instruction done\n");
        }
      }
    }
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

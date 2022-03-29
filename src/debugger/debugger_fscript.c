/**
 * File:   debugger_fscript.c
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

#include "tkc/async.h"
#include "tkc/object_default.h"
#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_fscript.h"

typedef struct _call_stack_frame_t {
  const char* name;
  darray_t* locals;
} call_stack_frame_t;

int32_t debugger_fscript_get_start_line(fscript_t* fscript) {
  uint32_t i = 0;
  fscript_args_t* args = NULL;
  fscript_func_call_t* iter = NULL;
  if (fscript == NULL || fscript->first == NULL) {
    return 0;
  }

  args = &(fscript->first->args);
  for (i = 0; i < args->size; i++) {
    if (args->args[i].type == VALUE_TYPE_FSCRIPT_FUNC) {
      iter = value_func(args->args + i);
      return iter->row;
    }
  }

  return 0;
}

static call_stack_frame_t* call_stack_frame_create(const char* name, darray_t* locals) {
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

static ret_t debugger_fscript_enter_func(debugger_t* debugger, const char* name, darray_t* locals) {
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
  return_value_if_fail(d != NULL && d->mutex != NULL, RET_BAD_PARAMS);

  return tk_mutex_nest_lock(d->mutex);
}

static ret_t debugger_fscript_unlock(debugger_t* debugger) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->mutex != NULL, RET_BAD_PARAMS);

  return tk_mutex_nest_unlock(d->mutex);
}

static ret_t debugger_fscript_stop(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL) {
      d->fscript->returned = TRUE;
      debugger_clear_break_points(debugger);
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
      d->break_type = DEBUGGER_FSCRIPT_BREAK_STEP_IN;
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
  d->break_type = DEBUGGER_FSCRIPT_BREAK_NONE;

  return RET_OK;
}

static ret_t debugger_fscript_step_in(debugger_t* debugger) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (d->fscript != NULL && d->paused) {
      ret = RET_OK;
      debugger_fscript_clear_step_stops(d);
      d->next_stop_executed_line = d->executed_lines + 1;
      d->break_type = DEBUGGER_FSCRIPT_BREAK_STEP_IN;
      log_debug("step_in: %d %d\n", d->executed_lines, d->next_stop_executed_line);
    }
    debugger_fscript_unlock(debugger);

    if (ret == RET_OK) {
      tk_cond_var_awake(d->cond_var);
    }
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
      debugger_fscript_clear_step_stops(d);
      d->next_stop_executed_line = d->executed_lines + 1;
      d->next_stop_call_frame_index = d->call_stack_frames.size;
      d->break_type = DEBUGGER_FSCRIPT_BREAK_NEXT;
      log_debug("next:executed_lines=%d prev_executed_line=%d\n", d->executed_lines,
                d->prev_executed_line);
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
      log_debug("step out:%d\n", d->next_stop_call_frame_index);
      d->break_type = DEBUGGER_FSCRIPT_BREAK_STEP_OUT;
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
      d->next_stop_line = d->prev_executed_line + 1;
      d->break_type = DEBUGGER_FSCRIPT_BREAK_STEP_OVER;
      log_debug("step over: %d %d\n", d->next_stop_line, d->prev_executed_line);
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
  tk_object_t* locals = object_default_create();
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, NULL);
  return_value_if_fail(d->call_stack_frames.size > frame_index, NULL);

  index = d->call_stack_frames.size - frame_index - 1;
  frame = (call_stack_frame_t*)darray_get(&(d->call_stack_frames), index);
  return_value_if_fail(frame != NULL, NULL);

  if (frame->locals != NULL) {
    uint32_t i = 0;
    for (i = 0; i < frame->locals->size; i++) {
      named_value_t* iter = (named_value_t*)darray_get(frame->locals, i);
      if (iter->value.type != VALUE_TYPE_INVALID) {
        tk_object_set_prop(locals, iter->name, &(iter->value));
      }
    }
  }

  return locals;
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

  str_clear(&(d->temp_str));
  for (i = 0; i < d->call_stack_frames.size; i++) {
    call_stack_frame_t* iter = (call_stack_frame_t*)darray_get(&(d->call_stack_frames), i);
    str_append_more(&(d->temp_str), iter->name, "\n", NULL);
  }

  callstack->data = d->temp_str.str;
  callstack->size = d->temp_str.size + 1;

  return RET_OK;
}

static ret_t debugger_fscript_get_break_points(debugger_t* debugger, binary_data_t* break_points) {
  uint32_t i = 0;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL && d->fscript != NULL, RET_BAD_PARAMS);

  str_clear(&(d->temp_str));
  for (i = 0; i < d->break_points.size; i++) {
    int32_t line = tk_pointer_to_int(darray_get(&(d->break_points), i));
    str_append_int(&(d->temp_str), line);
    str_append(&(d->temp_str), "\n");
  }

  break_points->data = d->temp_str.str;
  break_points->size = d->temp_str.size + 1;

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
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    if (line == DEBUGGER_START_LINE) {
      d->stop_at_start_line = TRUE;
    } else {
      darray_push_unique(&(d->break_points), tk_pointer_from_int(line));
    }
    debugger_fscript_unlock(debugger);
  }

  return RET_OK;
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

static ret_t debugger_fscript_attach(debugger_t* debugger, const char* lang, const char* code_id) {
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

ret_t debugger_fscript_set_code(debugger_t* debugger, const binary_data_t* code, bool_t changed) {
  ret_t ret = RET_FAIL;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    ret = RET_OK;
    d->code_changed = changed;
    str_set_with_len(&(d->code), (char*)(code->data), code->size);
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static ret_t debugger_fscript_update_code(debugger_t* debugger, const binary_data_t* code) {
  return debugger_fscript_set_code(debugger, code, TRUE);
}

static ret_t debugger_fscript_exec_async(void* ctx) {
  value_t v;
  fscript_t* fscript = (fscript_t*)ctx;

  value_set_int(&v, 0);
  fscript_exec(fscript, &v);
  value_reset(&v);
  OBJECT_UNREF(fscript->obj);
  fscript_destroy(fscript);

  return RET_OK;
}

static ret_t debugger_fscript_wait_for_debugger(debugger_t* debugger) {
  int32_t start_line = 0;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(d != NULL, RET_BAD_PARAMS);
  start_line = debugger_fscript_get_start_line(d->fscript);

  return darray_push_unique(&(d->break_points), tk_pointer_from_int(start_line));
}

static ret_t debugger_fscript_launch(debugger_t* debugger, const char* lang,
                                     const binary_data_t* code) {
  ret_t ret = RET_FAIL;
  fscript_t* fscript = NULL;
  tk_object_t* obj = object_default_create();
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    fscript = fscript_create(obj, (char*)(code->data));
    if (fscript != NULL) {
      debugger_fscript_set_code(debugger, code, TRUE);
      if (fscript->code_id == NULL) {
        fscript->code_id = tk_strdup(DEBUGGER_DEFAULT_CODE_ID);
      }
      d->fscript = fscript;
      d->code_id = tk_str_copy(d->code_id, fscript->code_id);
      debugger_fscript_wait_for_debugger(debugger);
      ret = async_call(debugger_fscript_exec_async, NULL, fscript);
    } else {
      TK_OBJECT_UNREF(obj);
    }
    debugger_fscript_unlock(debugger);
  }

  return ret;
}

static const debugger_vtable_t s_debugger_fscript_vtable = {
    .lang = DEBUGGER_LANG_FSCRIPT,
    .attach = debugger_fscript_attach,
    .launch = debugger_fscript_launch,
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
    .get_break_points = debugger_fscript_get_break_points,
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
  tk_mutex_nest_t* mutex = NULL;
  debugger_fscript_t* debugger = DEBUGGER_FSCRIPT(obj);
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  debugger_fscript_clear_break_points(DEBUGGER(obj));
  debugger_fscript_continue(DEBUGGER(obj));

  mutex = debugger->mutex_for_destroy;

  tk_mutex_nest_lock(mutex);
  tk_cond_var_destroy(debugger->cond_var);
  debugger->cond_var = NULL;

  tk_mutex_nest_destroy(debugger->mutex);
  debugger->mutex = NULL;

  TKMEM_FREE(debugger->code_id);
  str_reset(&(debugger->code));
  str_reset(&(debugger->temp_str));
  darray_deinit(&(debugger->break_points));
  darray_deinit(&(debugger->call_stack_frames));

  tk_mutex_nest_unlock(mutex);
  tk_mutex_nest_destroy(mutex);

  return RET_OK;
}

static ret_t debugger_fscript_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  debugger_fscript_t* debugger = DEBUGGER_FSCRIPT(obj);
  return_value_if_fail(debugger != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, DEBUGGER_PROP_CODE_ID)) {
    value_set_str(v, debugger->code_id);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_object_debugger_fscript_vtable = {
    .type = "object_debugger_fscript",
    .desc = "object_debugger_fscript",
    .size = sizeof(debugger_fscript_t),
    .is_collection = FALSE,
    .get_prop = debugger_fscript_get_prop,
    .on_destroy = debugger_fscript_on_destroy};

debugger_t* debugger_fscript_create(void) {
  debugger_fscript_t* debugger = NULL;
  debugger = (debugger_fscript_t*)tk_object_create(&s_object_debugger_fscript_vtable);
  return_value_if_fail(debugger != NULL, NULL);

  debugger->mutex = tk_mutex_nest_create();
  debugger->mutex_for_destroy = tk_mutex_nest_create();
  debugger->cond_var = tk_cond_var_create();
  debugger->debugger.vt = &s_debugger_fscript_vtable;

  str_init(&(debugger->code), 100);
  str_init(&(debugger->temp_str), 100);
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
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  debugger = debugger_server_find_debugger(fscript->code_id);
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
    d->prev_executed_line = 0;
    d->prev_breaked_line = -1;
    debugger_fscript_clear_step_stops(d);

    fscript_set_print_func(fscript, debugger_fscript_print_func);
    fscript_set_on_error(fscript, debugger_fscript_on_error, d);

    fscript_ensure_locals(fscript);
    darray_clear(&(d->call_stack_frames));
    debugger_fscript_enter_func(debugger, "<root>", fscript->locals);

    if (d->stop_at_start_line) {
      d->stop_at_start_line = FALSE;
      debugger_fscript_wait_for_debugger(debugger);
    }
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
    return paused;
  }

  switch (d->break_type) {
    case DEBUGGER_FSCRIPT_BREAK_STEP_IN: {
      if ((d->executed_lines + 1) >= d->next_stop_executed_line) {
        paused = TRUE;
      }
      break;
    }
    case DEBUGGER_FSCRIPT_BREAK_NEXT: {
      if ((d->executed_lines + 1) >= d->next_stop_executed_line) {
        if (d->next_stop_call_frame_index >= d->call_stack_frames.size) {
          paused = TRUE;
        }
      }
      break;
    }
    case DEBUGGER_FSCRIPT_BREAK_STEP_OVER: {
      if (line >= d->next_stop_line) {
        paused = TRUE;
      }
      break;
    }
    default:
      break;
  }

  return paused;
}

static bool_t debugger_fscript_emit_breaked(debugger_t* debugger, int32_t line) {
  debugger_breaked_event_t event;
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (d->prev_breaked_line == line) {
    d->paused = FALSE;
  } else {
    d->paused = TRUE;
    d->prev_breaked_line = line;
  }

  d->prev_breaked_line = line;
  if (d->paused) {
    debugger_breaked_event_init(&event, line);
    emitter_dispatch(EMITTER(debugger), (event_t*)&event);
    log_debug("stop at %d\n", line);
  }

  return d->paused;
}

static ret_t debugger_fscript_before_exec_func(debugger_t* debugger, int32_t line) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    bool_t paused = debugger_fscript_should_pause(d, line);
    if (paused) {
      paused = debugger_fscript_emit_breaked(debugger, line);
    }
    debugger_fscript_unlock(debugger);

    if (paused && d->cond_var != NULL) {
      tk_cond_var_wait(d->cond_var, 0xffffff);
    }
  }

  return RET_OK;
}

static ret_t debugger_fscript_after_exec_func(debugger_t* debugger, int32_t line) {
  debugger_fscript_t* d = DEBUGGER_FSCRIPT(debugger);

  if (debugger_fscript_lock(debugger) == RET_OK) {
    bool_t paused = FALSE;
    log_debug("after exec: prev_executed_line=%d line=%d executed_lines=%d \n",
              d->prev_executed_line, line, d->executed_lines);
    if (d->prev_executed_line != line) {
      d->executed_lines++;
    }
    d->prev_executed_line = line;

    if (d->break_type == DEBUGGER_FSCRIPT_BREAK_STEP_OUT) {
      if (d->next_stop_call_frame_index >= d->call_stack_frames.size) {
        paused = TRUE;
      }
    }

    if (paused) {
      paused = debugger_fscript_emit_breaked(debugger, line);
    }
    debugger_fscript_unlock(debugger);

    if (paused && d->cond_var != NULL) {
      log_debug("step for next\n");
      tk_cond_var_wait(d->cond_var, 0xffffff);
    }
  }

  return RET_OK;
}

ret_t debugger_fscript_exec_func(fscript_t* fscript, const char* name, fscript_func_call_t* iter,
                                 value_t* result) {
  ret_t ret = RET_FAIL;
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  debugger = debugger_server_find_debugger(fscript->code_id);
  if (debugger == NULL) {
    return fscript_exec_func_default(fscript, iter, result);
  } else {
    int32_t line = iter->row;
    tk_mutex_nest_t* mutex = DEBUGGER_FSCRIPT(debugger)->mutex_for_destroy;
    return_value_if_fail(tk_mutex_nest_lock(mutex) == RET_OK, RET_FAIL);

    /*name不为NUL则是脚本函数*/
    if (name != NULL) {
      if (debugger_fscript_lock(debugger) == RET_OK) {
        debugger_fscript_enter_func(debugger, name, fscript->locals);
        debugger_fscript_unlock(debugger);
      }
      ret = fscript_exec_func_default(fscript, iter, result);
      if (debugger_fscript_lock(debugger) == RET_OK) {
        debugger_fscript_leave_func(debugger);
        debugger_fscript_unlock(debugger);
      }
    } else {
      if (fscript->first == iter) {
        log_debug("program exec begin\n");
      }

      debugger_fscript_before_exec_func(debugger, line);
      ret = fscript_exec_func_default(fscript, iter, result);

      if (fscript->first == iter) {
        log_debug("program exec end\n");
      } else {
        debugger_fscript_after_exec_func(debugger, line);
      }
    }
    tk_mutex_nest_unlock(mutex);
  }

  return ret;
}

ret_t debugger_fscript_set_var(fscript_t* fscript, const char* name, const value_t* v) {
  ret_t ret = RET_FAIL;
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);

  debugger = debugger_server_find_debugger(fscript->code_id);
  if (debugger == NULL) {
    return fscript_set_var_default(fscript, name, v);
  } else {
    int32_t line = fscript->curr->row;
    tk_mutex_nest_t* mutex = DEBUGGER_FSCRIPT(debugger)->mutex_for_destroy;
    return_value_if_fail(tk_mutex_nest_lock(mutex) == RET_OK, RET_FAIL);

    debugger_fscript_before_exec_func(debugger, line);
    ret = fscript_set_var_default(fscript, name, v);
    debugger_fscript_after_exec_func(debugger, line);
    tk_mutex_nest_unlock(mutex);
  }

  return ret;
}

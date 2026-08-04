/**
 * File:   object_orchestrator.c
 * Author: AWTK Develop Team
 * Brief:  对象编排器
 *
 * Copyright (c) 2026 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2026-08-03 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "object_orchestrator.h"
#include "tkc/object_hash.h"
#include "tkc/darray.h"

struct _object_orchestrator_t {
  tk_object_t object;
  darray_t undo_stack;
  tk_object_t* cmds;
  object_orchestrator_exec_workflow_t exec_workflow;
  void* workflow_ctx;
  bool_t busy : 1;
};

static ret_t object_orchestrator_on_destroy(tk_object_t* obj) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  darray_deinit(&orchestrator->undo_stack);
  TK_OBJECT_UNREF(orchestrator->cmds);

  return RET_OK;
}

static bool_t object_orchestrator_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, FALSE);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    uint32_t cmds_size = tk_object_get_prop_uint32(orchestrator->cmds, TK_OBJECT_PROP_SIZE, 0);
    return !orchestrator->busy && orchestrator->undo_stack.size == 0 && cmds_size > 0;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    return !orchestrator->busy && orchestrator->undo_stack.size > 0;
  }

  return FALSE;
}

static ret_t object_orchestrator_cmd_on_execed(void* ctx, event_t* e) {
  ret_t ret = RET_SKIP;
  tk_object_t* cmd = NULL;
  cmd_exec_event_t* evt = cmd_exec_event_cast(e);
  object_orchestrator_t* orchestrator = (object_orchestrator_t*)(ctx);
  return_value_if_fail(orchestrator != NULL && evt != NULL, RET_BAD_PARAMS);

  if (RET_OK != evt->result || !tk_str_eq(evt->name, TK_OBJECT_CMD_EXEC)) {
    return RET_SKIP;
  }

  cmd = TK_OBJECT(evt->e.target);
  ret = darray_push(&orchestrator->undo_stack, cmd);
  return_value_if_fail(RET_OK == ret, ret);

  tk_object_ref(cmd);

  return ret;
}

static ret_t object_orchestrator_exec_init_on_visit(void* ctx, const void* data) {
  object_orchestrator_t* orchestrator = (object_orchestrator_t*)(ctx);
  const value_t* v = (const value_t*)(data);
  tk_object_t* cmd = NULL;
  return_value_if_fail(orchestrator != NULL && v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  if (!emitter_exist(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                     orchestrator)) {
    return_value_if_fail(
        TK_INVALID_ID != emitter_on(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                                    orchestrator),
        RET_OOM);
  }

  return RET_OK;
}

static ret_t object_orchestrator_exec_deinit_on_visit(void* ctx, const void* data) {
  object_orchestrator_t* orchestrator = (object_orchestrator_t*)(ctx);
  const value_t* v = (const value_t*)(data);
  tk_object_t* cmd = NULL;
  return_value_if_fail(orchestrator != NULL && v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                      orchestrator);

  return RET_OK;
}

static ret_t object_orchestrator_exec_workflow_default_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  tk_object_t* cmd = NULL;
  const value_t* v = (const value_t*)(data);
  const char* args = (const char*)(ctx);
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  if (!tk_object_can_exec(cmd, TK_OBJECT_CMD_EXEC, args)) {
    return RET_STOP;
  }

  ret = tk_object_exec(cmd, TK_OBJECT_CMD_EXEC, args);

  do {
    TK_FOREACH_VISIT_RESULT_PROCESSING(
        ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__));
  } while (0);

  return ret;
}

static ret_t object_orchestrator_exec_workflow_default(tk_object_t* cmds, const char* args,
                                                       void* ctx) {
  (void)ctx;
  return_value_if_fail(cmds != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(cmds, object_orchestrator_exec_workflow_default_on_visit,
                                (void*)args);
}

static ret_t object_orchestrator_workflow_exec(object_orchestrator_t* orchestrator,
                                               const char* args, bool_t* execed) {
  ret_t ret = RET_OK;
  return_value_if_fail(orchestrator != NULL && execed != NULL, RET_BAD_PARAMS);

  *execed = FALSE;
  ret = tk_object_foreach_prop(orchestrator->cmds, object_orchestrator_exec_init_on_visit,
                               orchestrator);
  goto_error_if_fail(ret == RET_OK);
  {
    object_orchestrator_exec_workflow_t exec_workflow =
        orchestrator->exec_workflow != NULL ? orchestrator->exec_workflow
                                            : object_orchestrator_exec_workflow_default;
    ret = exec_workflow(orchestrator->cmds, args, orchestrator->workflow_ctx);
    *execed = TRUE;
  }
error:
  tk_object_foreach_prop(orchestrator->cmds, object_orchestrator_exec_deinit_on_visit,
                         orchestrator);
  return ret;
}

static ret_t object_orchestrator_exec(tk_object_t* obj, const char* name, const char* args) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, FALSE);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    ret_t ret = RET_OK;
    bool_t execed = FALSE;
    return_value_if_fail(!orchestrator->busy, RET_BUSY);

    orchestrator->busy = TRUE;
    darray_clear(&orchestrator->undo_stack);
    ret = object_orchestrator_workflow_exec(orchestrator, args, &execed);
    orchestrator->busy = FALSE;

    if (execed && RET_OK != ret) { /* 执行失败，回滚之前已经执行的命令 */
      return_value_if_fail(RET_OK == tk_object_exec(obj, TK_OBJECT_CMD_UNDO, args), ret);
    }

    return ret;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    ret_t ret = RET_OK;
    return_value_if_fail(!orchestrator->busy, RET_BUSY);

    orchestrator->busy = TRUE;
    while (orchestrator->undo_stack.size > 0) {
      tk_object_t* cmd = TK_OBJECT(darray_tail(&orchestrator->undo_stack));
      ret = tk_object_exec(cmd, TK_OBJECT_CMD_UNDO, args);
      if (RET_NOT_FOUND == ret) {
        ret = RET_OK;
      }
      TK_FOREACH_VISIT_RESULT_PROCESSING(
          ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__);
          darray_remove_index(&orchestrator->undo_stack, orchestrator->undo_stack.size - 1));
      darray_remove_index(&orchestrator->undo_stack, orchestrator->undo_stack.size - 1);
    }
    orchestrator->busy = FALSE;

    return ret;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_object_orchestrator_vtable = {
    .type = OBJECT_ORCHESTRATOR_TYPE,
    .desc = OBJECT_ORCHESTRATOR_TYPE,
    .size = sizeof(object_orchestrator_t),
    .on_destroy = object_orchestrator_on_destroy,
    .can_exec = object_orchestrator_can_exec,
    .exec = object_orchestrator_exec,
    .compare = tk_object_compare_name_without_nullptr,
};

tk_object_t* object_orchestrator_create(void) {
  object_orchestrator_t* ret =
      (object_orchestrator_t*)tk_object_create(&s_object_orchestrator_vtable);
  return_value_if_fail(ret != NULL, NULL);

  ret->cmds = object_hash_create_ex(FALSE);
  goto_error_if_fail(ret->cmds != NULL);
  object_hash_set_keep_props_order(ret->cmds, TRUE);

  goto_error_if_fail(darray_init(&ret->undo_stack, 8, (tk_destroy_t)tk_object_unref, NULL) != NULL);

  return TK_OBJECT(ret);
error:
  tk_object_unref(TK_OBJECT(ret));
  return NULL;
}

object_orchestrator_t* object_orchestrator_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_orchestrator_vtable, NULL);
  return (object_orchestrator_t*)obj;
}

ret_t object_orchestrator_add_cmd(tk_object_t* obj, const char* name, tk_object_t* cmd) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL && TK_STR_IS_NOT_EMPTY(name) && cmd != NULL,
                       RET_BAD_PARAMS);

  return tk_object_set_prop_object(orchestrator->cmds, name, cmd);
}

ret_t object_orchestrator_set_workflow(tk_object_t* obj,
                                       object_orchestrator_exec_workflow_t exec_workflow,
                                       void* ctx) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  orchestrator->exec_workflow = exec_workflow;
  orchestrator->workflow_ctx = ctx;

  return RET_OK;
}

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
    return orchestrator->undo_stack.size == 0 && cmds_size > 0;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    return orchestrator->undo_stack.size > 0;
  }

  return FALSE;
}

static ret_t object_orchestrator_cmd_on_execed(void* ctx, event_t* e) {
  ret_t ret = RET_SKIP;
  tk_object_t* cmd = NULL;
  cmd_exec_event_t* evt = cmd_exec_event_cast(e);
  object_orchestrator_t* orchestrator = (object_orchestrator_t*)(ctx);
  return_value_if_fail(orchestrator != NULL && evt != NULL, RET_BAD_PARAMS);

  if (RET_OK != evt->result) {
    return RET_SKIP;
  }

  cmd = TK_OBJECT(evt->e.target);
  ret = darray_push(&orchestrator->undo_stack, cmd);
  if (RET_OK == ret) {
    tk_object_ref(cmd);
  }
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
    emitter_on(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed, orchestrator);
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
  const value_t* v = (const value_t*)(data);
  tk_object_t* cmd = NULL;
  const char* args = (const char*)(ctx);
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  if (!tk_object_can_exec(cmd, TK_OBJECT_CMD_EXEC, args)) {
    return RET_STOP;
  }

  return tk_object_exec(cmd, TK_OBJECT_CMD_EXEC, args);
}

static ret_t object_orchestrator_exec_workflow_default(tk_object_t* cmds, const char* args,
                                                       void* ctx) {
  (void)ctx;
  return_value_if_fail(cmds != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(cmds, object_orchestrator_exec_workflow_default_on_visit,
                                (void*)args);
}

inline static object_orchestrator_exec_workflow_t object_orchestrator_exec_workflow(
    object_orchestrator_t* orchestrator) {
  return orchestrator->exec_workflow != NULL ? orchestrator->exec_workflow
                                             : object_orchestrator_exec_workflow_default;
}

static ret_t object_orchestrator_exec(tk_object_t* obj, const char* name, const char* args) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, FALSE);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    ret_t ret = RET_OK;
    darray_clear(&orchestrator->undo_stack);
    tk_object_foreach_prop(orchestrator->cmds, object_orchestrator_exec_init_on_visit,
                           orchestrator);
    ret = object_orchestrator_exec_workflow(orchestrator)(orchestrator->cmds, args,
                                                          orchestrator->workflow_ctx);
    tk_object_foreach_prop(orchestrator->cmds, object_orchestrator_exec_deinit_on_visit,
                           orchestrator);
    return ret;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    ret_t ret = RET_OK;
    tk_object_t* cmd = NULL;
    while ((cmd = TK_OBJECT(darray_pop(&orchestrator->undo_stack))) != NULL) {
      if (tk_object_can_exec(cmd, TK_OBJECT_CMD_UNDO, args)) {
        ret = tk_object_exec(cmd, TK_OBJECT_CMD_UNDO, args);
      } else {
        ret = RET_STOP;
      }
      if (orchestrator->undo_stack.destroy != NULL) {
        orchestrator->undo_stack.destroy(cmd);
      }
      TK_FOREACH_VISIT_RESULT_PROCESSING(
          ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__));
    }
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

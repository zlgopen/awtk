/**
 * File:   object_workflow_cmd.c
 * Author: AWTK Develop Team
 * Brief:  对象工作流命令
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

#include "object_workflow_cmd.h"
#include "tkc/object_override.h"
#include "tkc/object_hash.h"
#include "tkc/darray.h"
#include "tkc/utils.h"

struct _object_workflow_cmd_t {
  tk_object_t object;
  darray_t undo_stack;
  tk_object_t* cmds;
  object_workflow_cmd_runner_t runner;
  void* runner_ctx;
  bool_t running : 1;
  bool_t busy : 1;
};

static bool_t object_workflow_cmd_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, FALSE);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    return !workflow_cmd->busy && workflow_cmd->undo_stack.size == 0;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    return !workflow_cmd->busy && workflow_cmd->undo_stack.size > 0;
  }

  return FALSE;
}

static ret_t object_workflow_cmd_on_child_execed(void* ctx, event_t* e) {
  ret_t ret = RET_SKIP;
  tk_object_t* cmd = NULL;
  cmd_exec_event_t* evt = cmd_exec_event_cast(e);
  object_workflow_cmd_t* workflow_cmd = (object_workflow_cmd_t*)(ctx);
  return_value_if_fail(workflow_cmd != NULL && evt != NULL, RET_BAD_PARAMS);

  if (!workflow_cmd->running ||
      (RET_OK != evt->result || !tk_str_eq(evt->name, TK_OBJECT_CMD_EXEC))) {
    return RET_SKIP;
  }

  cmd = TK_OBJECT(evt->e.target);
  ret = darray_push(&workflow_cmd->undo_stack, cmd);
  return_value_if_fail(RET_OK == ret, ret);

  tk_object_ref(cmd);

  return ret;
}

static ret_t object_workflow_cmd_runner_default_on_visit(void* ctx, const void* data) {
  tk_object_t* cmd = NULL;
  const value_t* v = (const value_t*)(data);
  const char* args = (const char*)(ctx);
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  if (!tk_object_can_exec(cmd, TK_OBJECT_CMD_EXEC, args)) {
    return RET_FAIL;
  }

  return tk_object_exec(cmd, TK_OBJECT_CMD_EXEC, args);
}

static ret_t object_workflow_cmd_runner_default(tk_object_t* obj, const char* args, void* ctx) {
  (void)ctx;
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(workflow_cmd->cmds, object_workflow_cmd_runner_default_on_visit,
                                (void*)args);
}

inline static ret_t object_workflow_cmd_run(object_workflow_cmd_t* workflow_cmd, const char* args) {
  ret_t ret = RET_OK;
  object_workflow_cmd_runner_t runner = NULL;
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  workflow_cmd->running = TRUE;
  runner = workflow_cmd->runner != NULL ? workflow_cmd->runner : object_workflow_cmd_runner_default;
  ret = runner(&workflow_cmd->object, args, workflow_cmd->runner_ctx);
  workflow_cmd->running = FALSE;

  return ret;
}

static ret_t object_workflow_cmd_exec(tk_object_t* obj, const char* name, const char* args) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    ret_t ret = RET_OK;
    uint32_t cmds_size = 0;
    return_value_if_fail(!workflow_cmd->busy, RET_BUSY);

    workflow_cmd->busy = TRUE;
    darray_clear(&workflow_cmd->undo_stack);

    cmds_size = tk_object_get_prop_uint32(workflow_cmd->cmds, TK_OBJECT_PROP_SIZE, 0);
    ret = darray_extend(&workflow_cmd->undo_stack, cmds_size);
    return_value_if_fail(RET_OK == ret, (workflow_cmd->busy = FALSE, ret));

    ret = object_workflow_cmd_run(workflow_cmd, args);
    workflow_cmd->busy = FALSE;

    if (RET_OK != ret) { /* 执行失败，回滚之前已经执行的命令 */
      if (tk_object_can_exec(obj, TK_OBJECT_CMD_UNDO, args)) {
        return_value_if_fail(RET_OK == tk_object_exec(obj, TK_OBJECT_CMD_UNDO, args), ret);
      }
    }

    return ret;
  } else if (tk_str_eq(TK_OBJECT_CMD_UNDO, name)) {
    ret_t ret = RET_OK;
    return_value_if_fail(!workflow_cmd->busy, RET_BUSY);

    workflow_cmd->busy = TRUE;
    while (workflow_cmd->undo_stack.size > 0) {
      tk_object_t* cmd = TK_OBJECT(darray_tail(&workflow_cmd->undo_stack));
      ret = tk_object_exec(cmd, TK_OBJECT_CMD_UNDO, args);
      if (RET_NOT_FOUND == ret) {
        ret = RET_OK;
      }
      TK_FOREACH_VISIT_RESULT_PROCESSING(
          ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__);
          darray_remove_index(&workflow_cmd->undo_stack, workflow_cmd->undo_stack.size - 1));
      darray_remove_index(&workflow_cmd->undo_stack, workflow_cmd->undo_stack.size - 1);
    }
    workflow_cmd->busy = FALSE;

    return ret;
  }

  return RET_NOT_FOUND;
}

static ret_t object_workflow_cmd_exec_deinit_on_visit(void* ctx, const void* data) {
  object_workflow_cmd_t* workflow_cmd = (object_workflow_cmd_t*)(ctx);
  const value_t* v = (const value_t*)(data);
  tk_object_t* cmd = NULL;
  return_value_if_fail(workflow_cmd != NULL && v != NULL, RET_BAD_PARAMS);

  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_workflow_cmd_on_child_execed,
                      workflow_cmd);

  return RET_OK;
}

static ret_t object_workflow_cmd_on_destroy(tk_object_t* obj) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  darray_deinit(&workflow_cmd->undo_stack);

  tk_object_foreach_prop(workflow_cmd->cmds, object_workflow_cmd_exec_deinit_on_visit,
                         workflow_cmd);
  TK_OBJECT_UNREF(workflow_cmd->cmds);

  return RET_OK;
}

static ret_t object_workflow_cmd_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop(workflow_cmd->cmds, name, v);
}

static ret_t object_workflow_cmd_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop(workflow_cmd->cmds, name, v);
}

static ret_t object_workflow_cmd_copy_props(tk_object_t* obj, tk_object_t* src, bool_t overwrite) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_copy_props(workflow_cmd->cmds, src, overwrite);
}

static ret_t object_workflow_cmd_remove_prop(tk_object_t* obj, const char* name) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_remove_prop(workflow_cmd->cmds, name);
}

static ret_t object_workflow_cmd_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(workflow_cmd->cmds, on_prop, ctx);
}

static ret_t object_workflow_cmd_clear_props(tk_object_t* obj) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_clear_props(workflow_cmd->cmds);
}

static value_t* object_workflow_cmd_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* ctx) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, NULL);

  return tk_object_find_prop(workflow_cmd->cmds, cmp, ctx);
}

static ret_t object_workflow_cmd_find_props(tk_object_t* obj, tk_compare_t cmp, const void* ctx,
                                            darray_t* matched) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_find_props(workflow_cmd->cmds, cmp, ctx, matched);
}

static tk_object_t* object_workflow_cmd_clone(tk_object_t* obj) {
  object_workflow_cmd_t* ret = NULL;
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, NULL);

  ret = (object_workflow_cmd_t*)object_workflow_cmd_create();
  return_value_if_fail(ret != NULL, NULL);

  goto_error_if_fail(RET_OK == object_workflow_cmd_set_runner(TK_OBJECT(ret), workflow_cmd->runner,
                                                              workflow_cmd->runner_ctx));
  goto_error_if_fail(RET_OK == tk_object_copy_props(ret->cmds, workflow_cmd->cmds, TRUE) &&
                     tk_object_get_prop_uint32(ret->cmds, TK_OBJECT_PROP_SIZE, 0) ==
                         tk_object_get_prop_uint32(workflow_cmd->cmds, TK_OBJECT_PROP_SIZE, 0));

  return TK_OBJECT(ret);
error:
  TK_OBJECT_UNREF(ret);
  return NULL;
}

static const object_vtable_t s_object_workflow_cmd_vtable = {
    .type = OBJECT_WORKFLOW_CMD_TYPE,
    .desc = OBJECT_WORKFLOW_CMD_TYPE,
    .size = sizeof(object_workflow_cmd_t),
    .on_destroy = object_workflow_cmd_on_destroy,
    .can_exec = object_workflow_cmd_can_exec,
    .exec = object_workflow_cmd_exec,
    .get_prop = object_workflow_cmd_get_prop,
    .set_prop = object_workflow_cmd_set_prop,
    .copy_props = object_workflow_cmd_copy_props,
    .remove_prop = object_workflow_cmd_remove_prop,
    .foreach_prop = object_workflow_cmd_foreach_prop,
    .clear_props = object_workflow_cmd_clear_props,
    .find_prop = object_workflow_cmd_find_prop,
    .find_props = object_workflow_cmd_find_props,
    .clone = object_workflow_cmd_clone,
    .compare = tk_object_compare_name_without_nullptr,
};

#define OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj) \
  (*(object_workflow_cmd_t**)((uint8_t*)(obj) + (obj)->vt->size))

static ret_t object_workflow_cmd_registry_set_prop(tk_object_t* obj, const char* name,
                                                   const value_t* v) {
  ret_t ret = RET_OK;
  tk_object_t *cmd = NULL, *old_cmd = NULL;
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && VALUE_TYPE_OBJECT == v->type, RET_BAD_PARAMS);
  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  old_cmd = tk_object_get_prop_object(workflow_cmd->cmds, name);
  if (old_cmd != NULL) {
    tk_object_ref(old_cmd);
    tk_object_remove_prop(workflow_cmd->cmds, name);
  }

  ret = tk_object_set_prop_object(obj, name, cmd);
  goto_error_if_fail(RET_OK == ret);

  if (!emitter_exist(EMITTER(cmd), EVT_CMD_EXECED, object_workflow_cmd_on_child_execed,
                     workflow_cmd)) {
    goto_error_if_fail_ex(
        TK_INVALID_ID != emitter_on(EMITTER(cmd), EVT_CMD_EXECED,
                                    object_workflow_cmd_on_child_execed, workflow_cmd),
        ret = RET_OOM);
  }

  if (old_cmd != NULL) {
    tk_object_unref(old_cmd);
  }
  return ret;
error:
  tk_object_remove_prop(obj, name);
  if (old_cmd != NULL) {
    tk_object_set_prop_object(workflow_cmd->cmds, name, old_cmd);
    tk_object_unref(old_cmd);
  }
  return ret;
}

static int object_workflow_cmd_child_compare(const void* iter, const void* ctx) {
  const value_t* v = (const value_t*)(iter);
  tk_object_t* v_obj = value_object(v);
  tk_object_t* cmd = TK_OBJECT(ctx);
  return pointer_compare(v_obj, cmd);
}

static int object_workflow_cmd_child_compare_without_self(const void* iter, const void* ctx) {
  const value_t* v = (const value_t*)(iter);
  const value_t* target = (const value_t*)(ctx);
  if (TK_ADR_EQ(v, target)) {
    return -1;
  }
  return object_workflow_cmd_child_compare(iter, value_object(target));
}

static ret_t object_workflow_cmd_registry_remove_prop(tk_object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* cmd = NULL;
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  cmd = tk_object_get_prop_object(obj, name);
  if (cmd != NULL) {
    tk_object_ref(cmd);
    ret = tk_object_remove_prop(obj, name);
    if (RET_OK == ret) {
      if (NULL == tk_object_find_prop(obj, object_workflow_cmd_child_compare, cmd)) {
        emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_workflow_cmd_on_child_execed,
                            workflow_cmd);
      }
    }
    tk_object_unref(cmd);
  }

  return ret;
}

typedef struct _object_workflow_cmd_registry_foreach_ctx_t {
  tk_object_t* obj;
  object_workflow_cmd_t* workflow_cmd;
  tk_visit_t on_prop;
  void* ctx;
} object_workflow_cmd_registry_foreach_ctx_t;

static ret_t object_workflow_cmd_registry_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  object_workflow_cmd_registry_foreach_ctx_t* ctx_info =
      (object_workflow_cmd_registry_foreach_ctx_t*)(ctx);
  return_value_if_fail(ctx_info->on_prop != NULL, RET_BAD_PARAMS);

  ret = ctx_info->on_prop(ctx_info->ctx, data);

  if (RET_REMOVE == ret) {
    bool_t visit = ctx_info->obj->visiting;
    ctx_info->obj->visiting = FALSE;
    if (NULL ==
        tk_object_find_prop(ctx_info->obj, object_workflow_cmd_child_compare_without_self, data)) {
      const value_t* v = (const value_t*)data;
      tk_object_t* cmd = value_object(v);
      emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_workflow_cmd_on_child_execed,
                          ctx_info->workflow_cmd);
    }
    ctx_info->obj->visiting = visit;
  }

  return ret;
}

static ret_t object_workflow_cmd_registry_foreach_prop(tk_object_t* obj, tk_visit_t on_prop,
                                                       void* ctx) {
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(obj, object_workflow_cmd_registry_on_visit,
                                &(object_workflow_cmd_registry_foreach_ctx_t){
                                    .obj = obj,
                                    .workflow_cmd = workflow_cmd,
                                    .on_prop = on_prop,
                                    .ctx = ctx,
                                });
}

static ret_t object_workflow_cmd_registry_clear_props(tk_object_t* obj) {
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  tk_object_foreach_prop(obj, object_workflow_cmd_exec_deinit_on_visit, workflow_cmd);

  return tk_object_clear_props(obj);
}

static ret_t object_workflow_cmd_registry_copy_props(tk_object_t* obj, tk_object_t* src,
                                                     bool_t overwrite) {
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  return tk_object_copy_props_default(workflow_cmd->cmds, src, overwrite);
}

static ret_t object_workflow_cmd_registry_on_destroy(tk_object_t* obj) {
  object_workflow_cmd_t* workflow_cmd = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  workflow_cmd = OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  tk_object_foreach_prop(obj, object_workflow_cmd_exec_deinit_on_visit, workflow_cmd);

  return RET_OK;
}

tk_object_t* object_workflow_cmd_create(void) {
  tk_object_t* cmds_base = NULL;
  static const object_vtable_t s_object_workflow_cmd_registry_vtable = {
      .set_prop = object_workflow_cmd_registry_set_prop,
      .remove_prop = object_workflow_cmd_registry_remove_prop,
      .foreach_prop = object_workflow_cmd_registry_foreach_prop,
      .clear_props = object_workflow_cmd_registry_clear_props,
      .copy_props = object_workflow_cmd_registry_copy_props,
      .on_destroy = object_workflow_cmd_registry_on_destroy,
  };
  object_workflow_cmd_t* ret =
      (object_workflow_cmd_t*)tk_object_create(&s_object_workflow_cmd_vtable);
  return_value_if_fail(ret != NULL, NULL);

  cmds_base = object_hash_create_with_opt(&(object_hash_create_opt_t){
      .enable_path = FALSE,
      .keep_props_order = TRUE,
      .extra_data_size = sizeof(object_workflow_cmd_t*),
  });
  goto_error_if_fail(cmds_base != NULL);
  OBJECT_WORKFLOW_CMD_FROM_CMDS_BASE(cmds_base) = ret;

  ret->cmds = object_override_create(cmds_base, &s_object_workflow_cmd_registry_vtable);
  goto_error_if_fail(ret->cmds != NULL);
  TK_OBJECT_UNREF(cmds_base);

  goto_error_if_fail(darray_init(&ret->undo_stack, 0, (tk_destroy_t)tk_object_unref, NULL) != NULL);

  return TK_OBJECT(ret);
error:
  TK_OBJECT_UNREF(cmds_base);
  tk_object_unref(TK_OBJECT(ret));
  return NULL;
}

object_workflow_cmd_t* object_workflow_cmd_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_workflow_cmd_vtable, NULL);
  return (object_workflow_cmd_t*)obj;
}

ret_t object_workflow_cmd_set_runner(tk_object_t* obj, object_workflow_cmd_runner_t runner,
                                     void* ctx) {
  object_workflow_cmd_t* workflow_cmd = OBJECT_WORKFLOW_CMD(obj);
  return_value_if_fail(workflow_cmd != NULL, RET_BAD_PARAMS);

  workflow_cmd->runner = runner;
  workflow_cmd->runner_ctx = ctx;

  return RET_OK;
}

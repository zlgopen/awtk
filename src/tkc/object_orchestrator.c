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
#include "tkc/object_override.h"
#include "tkc/object_hash.h"
#include "tkc/darray.h"
#include "tkc/utils.h"

struct _object_orchestrator_t {
  tk_object_t object;
  darray_t undo_stack;
  tk_object_t* cmds;
  object_orchestrator_exec_workflow_t exec_workflow;
  void* workflow_ctx;
  bool_t busy : 1;
  bool_t workflow_execing : 1;
};

static bool_t object_orchestrator_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, FALSE);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    return !orchestrator->busy && orchestrator->undo_stack.size == 0;
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

  if (!orchestrator->workflow_execing ||
      (RET_OK != evt->result || !tk_str_eq(evt->name, TK_OBJECT_CMD_EXEC))) {
    return RET_SKIP;
  }

  cmd = TK_OBJECT(evt->e.target);
  ret = darray_push(&orchestrator->undo_stack, cmd);
  return_value_if_fail(RET_OK == ret, ret);

  tk_object_ref(cmd);

  return ret;
}

static ret_t object_orchestrator_exec_workflow_default_on_visit(void* ctx, const void* data) {
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

static ret_t object_orchestrator_exec_workflow_default(tk_object_t* obj, const char* args,
                                                       void* ctx) {
  (void)ctx;
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(orchestrator->cmds,
                                object_orchestrator_exec_workflow_default_on_visit, (void*)args);
}

inline static ret_t object_orchestrator_workflow_exec(object_orchestrator_t* orchestrator,
                                                      const char* args) {
  ret_t ret = RET_OK;
  object_orchestrator_exec_workflow_t exec_workflow = NULL;
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  orchestrator->workflow_execing = TRUE;
  exec_workflow = orchestrator->exec_workflow != NULL ? orchestrator->exec_workflow
                                                      : object_orchestrator_exec_workflow_default;
  ret = exec_workflow(&orchestrator->object, args, orchestrator->workflow_ctx);
  orchestrator->workflow_execing = FALSE;

  return ret;
}

static ret_t object_orchestrator_exec(tk_object_t* obj, const char* name, const char* args) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(TK_OBJECT_CMD_EXEC, name)) {
    ret_t ret = RET_OK;
    uint32_t cmds_size = 0;
    return_value_if_fail(!orchestrator->busy, RET_BUSY);

    orchestrator->busy = TRUE;
    darray_clear(&orchestrator->undo_stack);

    cmds_size = tk_object_get_prop_uint32(orchestrator->cmds, TK_OBJECT_PROP_SIZE, 0);
    ret = darray_extend(&orchestrator->undo_stack, cmds_size);
    return_value_if_fail(RET_OK == ret, (orchestrator->busy = FALSE, ret));

    ret = object_orchestrator_workflow_exec(orchestrator, args);
    orchestrator->busy = FALSE;

    if (RET_OK != ret) { /* 执行失败，回滚之前已经执行的命令 */
      if (tk_object_can_exec(obj, TK_OBJECT_CMD_UNDO, args)) {
        return_value_if_fail(RET_OK == tk_object_exec(obj, TK_OBJECT_CMD_UNDO, args), ret);
      }
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

static ret_t object_orchestrator_on_destroy(tk_object_t* obj) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  darray_deinit(&orchestrator->undo_stack);

  tk_object_foreach_prop(orchestrator->cmds, object_orchestrator_exec_deinit_on_visit,
                         orchestrator);
  TK_OBJECT_UNREF(orchestrator->cmds);

  return RET_OK;
}

static ret_t object_orchestrator_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop(orchestrator->cmds, name, v);
}

static ret_t object_orchestrator_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop(orchestrator->cmds, name, v);
}

static ret_t object_orchestrator_copy_props(tk_object_t* obj, tk_object_t* src, bool_t overwrite) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_copy_props(orchestrator->cmds, src, overwrite);
}

static ret_t object_orchestrator_remove_prop(tk_object_t* obj, const char* name) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_remove_prop(orchestrator->cmds, name);
}

static ret_t object_orchestrator_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(orchestrator->cmds, on_prop, ctx);
}

static ret_t object_orchestrator_clear_props(tk_object_t* obj) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_clear_props(orchestrator->cmds);
}

static value_t* object_orchestrator_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* ctx) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, NULL);

  return tk_object_find_prop(orchestrator->cmds, cmp, ctx);
}

static ret_t object_orchestrator_find_props(tk_object_t* obj, tk_compare_t cmp, const void* ctx,
                                            darray_t* matched) {
  object_orchestrator_t* orchestrator = OBJECT_ORCHESTRATOR(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_find_props(orchestrator->cmds, cmp, ctx, matched);
}

static const object_vtable_t s_object_orchestrator_vtable = {
    .type = OBJECT_ORCHESTRATOR_TYPE,
    .desc = OBJECT_ORCHESTRATOR_TYPE,
    .size = sizeof(object_orchestrator_t),
    .on_destroy = object_orchestrator_on_destroy,
    .can_exec = object_orchestrator_can_exec,
    .exec = object_orchestrator_exec,
    .get_prop = object_orchestrator_get_prop,
    .set_prop = object_orchestrator_set_prop,
    .copy_props = object_orchestrator_copy_props,
    .remove_prop = object_orchestrator_remove_prop,
    .foreach_prop = object_orchestrator_foreach_prop,
    .clear_props = object_orchestrator_clear_props,
    .find_prop = object_orchestrator_find_prop,
    .find_props = object_orchestrator_find_props,
    .compare = tk_object_compare_name_without_nullptr,
};

#define OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj) \
  (*(object_orchestrator_t**)((uint8_t*)(obj) + (obj)->vt->size))

static ret_t object_orchestrator_cmds_set_prop(tk_object_t* obj, const char* name,
                                               const value_t* v) {
  ret_t ret = RET_OK;
  tk_object_t *cmd = NULL, *old_cmd = NULL;
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && VALUE_TYPE_OBJECT == v->type, RET_BAD_PARAMS);
  cmd = value_object(v);
  return_value_if_fail(cmd != NULL, RET_BAD_PARAMS);

  old_cmd = tk_object_get_prop_object(orchestrator->cmds, name);
  if (old_cmd != NULL) {
    tk_object_ref(old_cmd);
    tk_object_remove_prop(orchestrator->cmds, name);
  }

  ret = tk_object_set_prop_object(obj, name, cmd);
  goto_error_if_fail(RET_OK == ret);

  if (!emitter_exist(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                     orchestrator)) {
    goto_error_if_fail_ex(
        TK_INVALID_ID != emitter_on(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                                    orchestrator),
        ret = RET_OOM);
  }

  if (old_cmd != NULL) {
    tk_object_unref(old_cmd);
  }
  return ret;
error:
  tk_object_remove_prop(obj, name);
  if (old_cmd != NULL) {
    tk_object_set_prop_object(orchestrator->cmds, name, old_cmd);
    tk_object_unref(old_cmd);
  }
  return ret;
}

static int object_orchestrator_cmd_compare(const void* iter, const void* ctx) {
  const value_t* v = (const value_t*)(iter);
  tk_object_t* v_obj = value_object(v);
  tk_object_t* cmd = TK_OBJECT(ctx);
  return pointer_compare(v_obj, cmd);
}

static int object_orchestrator_cmd_compare_without_self(const void* iter, const void* ctx) {
  const value_t* v = (const value_t*)(iter);
  const value_t* target = (const value_t*)(ctx);
  if (TK_ADR_EQ(v, target)) {
    return -1;
  }
  return object_orchestrator_cmd_compare(iter, value_object(target));
}

static ret_t object_orchestrator_cmds_remove_prop(tk_object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  tk_object_t* cmd = NULL;
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  cmd = tk_object_get_prop_object(obj, name);
  if (cmd != NULL) {
    tk_object_ref(cmd);
    ret = tk_object_remove_prop(obj, name);
    if (RET_OK == ret) {
      if (NULL == tk_object_find_prop(obj, object_orchestrator_cmd_compare, cmd)) {
        emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                            orchestrator);
      }
    }
    tk_object_unref(cmd);
  }

  return ret;
}

typedef struct _object_orchestrator_cmds_foreach_ctx_t {
  tk_object_t* obj;
  object_orchestrator_t* orchestrator;
  tk_visit_t on_prop;
  void* ctx;
} object_orchestrator_cmds_foreach_ctx_t;

static ret_t object_orchestrator_cmds_on_visit(void* ctx, const void* data) {
  ret_t ret = RET_OK;
  object_orchestrator_cmds_foreach_ctx_t* ctx_info = (object_orchestrator_cmds_foreach_ctx_t*)(ctx);
  return_value_if_fail(ctx_info->on_prop != NULL, RET_BAD_PARAMS);

  ret = ctx_info->on_prop(ctx_info->ctx, data);

  if (RET_REMOVE == ret) {
    bool_t visit = ctx_info->obj->visiting;
    ctx_info->obj->visiting = FALSE;
    if (NULL ==
        tk_object_find_prop(ctx_info->obj, object_orchestrator_cmd_compare_without_self, data)) {
      const value_t* v = (const value_t*)data;
      tk_object_t* cmd = value_object(v);
      emitter_off_by_func(EMITTER(cmd), EVT_CMD_EXECED, object_orchestrator_cmd_on_execed,
                          ctx_info->orchestrator);
    }
    ctx_info->obj->visiting = visit;
  }

  return ret;
}

static ret_t object_orchestrator_cmds_foreach_prop(tk_object_t* obj, tk_visit_t on_prop,
                                                   void* ctx) {
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(obj, object_orchestrator_cmds_on_visit,
                                &(object_orchestrator_cmds_foreach_ctx_t){
                                    .obj = obj,
                                    .orchestrator = orchestrator,
                                    .on_prop = on_prop,
                                    .ctx = ctx,
                                });
}

static ret_t object_orchestrator_cmds_clear_props(tk_object_t* obj) {
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  tk_object_foreach_prop(obj, object_orchestrator_exec_deinit_on_visit, orchestrator);

  return tk_object_clear_props(obj);
}

static ret_t object_orchestrator_cmds_copy_props(tk_object_t* obj, tk_object_t* src,
                                                 bool_t overwrite) {
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  return tk_object_copy_props_default(orchestrator->cmds, src, overwrite);
}

static ret_t object_orchestrator_cmds_on_destroy(tk_object_t* obj) {
  object_orchestrator_t* orchestrator = NULL;
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);
  orchestrator = OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(obj);
  return_value_if_fail(orchestrator != NULL, RET_BAD_PARAMS);

  tk_object_foreach_prop(obj, object_orchestrator_exec_deinit_on_visit, orchestrator);

  return RET_OK;
}

tk_object_t* object_orchestrator_create(void) {
  tk_object_t* cmds_base = NULL;
  static const object_vtable_t s_object_orchestrator_cmds_vtable = {
      .set_prop = object_orchestrator_cmds_set_prop,
      .remove_prop = object_orchestrator_cmds_remove_prop,
      .foreach_prop = object_orchestrator_cmds_foreach_prop,
      .clear_props = object_orchestrator_cmds_clear_props,
      .copy_props = object_orchestrator_cmds_copy_props,
      .on_destroy = object_orchestrator_cmds_on_destroy,
  };
  object_orchestrator_t* ret =
      (object_orchestrator_t*)tk_object_create(&s_object_orchestrator_vtable);
  return_value_if_fail(ret != NULL, NULL);

  cmds_base = object_hash_create_with_opt(&(object_hash_create_opt_t){
      .enable_path = FALSE,
      .keep_props_order = TRUE,
      .extra_data_size = sizeof(object_orchestrator_t*),
  });
  goto_error_if_fail(cmds_base != NULL);
  OBJECT_ORCHESTRATOR_FROM_CMDS_BASE(cmds_base) = ret;

  ret->cmds = object_override_create(cmds_base, &s_object_orchestrator_cmds_vtable);
  goto_error_if_fail(ret->cmds != NULL);
  TK_OBJECT_UNREF(cmds_base);

  goto_error_if_fail(darray_init(&ret->undo_stack, 0, (tk_destroy_t)tk_object_unref, NULL) != NULL);

  return TK_OBJECT(ret);
error:
  TK_OBJECT_UNREF(cmds_base);
  tk_object_unref(TK_OBJECT(ret));
  return NULL;
}

object_orchestrator_t* object_orchestrator_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_orchestrator_vtable, NULL);
  return (object_orchestrator_t*)obj;
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

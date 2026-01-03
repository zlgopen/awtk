/**
 * File:   object_overload.c
 * Author: AWTK Develop Team
 * Brief:  对象方法重载器。
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-12-31 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "object_overload.h"

#include "tkc/mem.h"
#include "tkc/utils.h"

static ret_t object_overload_on_destroy(tk_object_t* obj) {
  ret_t ret = RET_OK;
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->on_destroy != NULL) {
    ret = overload->overload_vt->on_destroy(overload->base_obj);
  }

  TK_OBJECT_UNREF(overload->base_obj);

  return ret;
}

static int32_t object_overload_compare(tk_object_t* obj, tk_object_t* other) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, -1);

  if (overload->overload_vt != NULL && overload->overload_vt->compare != NULL) {
    return overload->overload_vt->compare(overload->base_obj, other);
  }

  return tk_object_compare(overload->base_obj, other);
}

static ret_t object_overload_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->get_prop != NULL) {
    return overload->overload_vt->get_prop(overload->base_obj, name, v);
  }

  return tk_object_get_prop(overload->base_obj, name, v);
}

static ret_t object_overload_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->set_prop != NULL) {
    return overload->overload_vt->set_prop(overload->base_obj, name, v);
  }

  return tk_object_set_prop(overload->base_obj, name, v);
}

static ret_t object_overload_copy_props(tk_object_t* obj, tk_object_t* src, bool_t overwrite) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->copy_props != NULL) {
    return overload->overload_vt->copy_props(overload->base_obj, src, overwrite);
  }

  return tk_object_copy_props(overload->base_obj, src, overwrite);
}

static ret_t object_overload_remove_prop(tk_object_t* obj, const char* name) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->remove_prop != NULL) {
    return overload->overload_vt->remove_prop(overload->base_obj, name);
  }

  return tk_object_remove_prop(overload->base_obj, name);
}

static ret_t object_overload_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->foreach_prop != NULL) {
    return overload->overload_vt->foreach_prop(overload->base_obj, on_prop, ctx);
  }

  return tk_object_foreach_prop(overload->base_obj, on_prop, ctx);
}

static ret_t object_overload_clear_props(tk_object_t* obj) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->clear_props != NULL) {
    return overload->overload_vt->clear_props(overload->base_obj);
  }

  return tk_object_clear_props(overload->base_obj);
}

static value_t* object_overload_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* ctx) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, NULL);

  if (overload->overload_vt != NULL && overload->overload_vt->find_prop != NULL) {
    return overload->overload_vt->find_prop(overload->base_obj, cmp, ctx);
  }

  return tk_object_find_prop(overload->base_obj, cmp, ctx);
}

static ret_t object_overload_find_props(tk_object_t* obj, tk_compare_t cmp, const void* ctx,
                                        darray_t* matched) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->find_props != NULL) {
    return overload->overload_vt->find_props(overload->base_obj, cmp, ctx, matched);
  }

  return tk_object_find_props(overload->base_obj, cmp, ctx, matched);
}

static bool_t object_overload_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, FALSE);

  if (overload->overload_vt != NULL && overload->overload_vt->can_exec != NULL) {
    return overload->overload_vt->can_exec(overload->base_obj, name, args);
  }

  return tk_object_can_exec(overload->base_obj, name, args);
}

static ret_t object_overload_exec(tk_object_t* obj, const char* name, const char* args) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->exec != NULL) {
    return overload->overload_vt->exec(overload->base_obj, name, args);
  }

  return tk_object_exec(overload->base_obj, name, args);
}

static ret_t object_overload_exec_ex(tk_object_t* obj, const char* name, const char* args,
                                     value_t* result) {
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, RET_BAD_PARAMS);

  if (overload->overload_vt != NULL && overload->overload_vt->exec_ex != NULL) {
    return overload->overload_vt->exec_ex(overload->base_obj, name, args, result);
  }

  return tk_object_exec_ex(overload->base_obj, name, args, result);
}

static tk_object_t* object_overload_clone(tk_object_t* obj) {
  tk_object_t* ret = NULL;
  tk_object_t* clone_base_obj = NULL;
  object_overload_t* overload = OBJECT_OVERLOAD(obj);
  return_value_if_fail(overload != NULL, NULL);

  if (overload->overload_vt != NULL && overload->overload_vt->clone != NULL) {
    clone_base_obj = overload->overload_vt->clone(overload->base_obj);
  } else {
    clone_base_obj = tk_object_clone(overload->base_obj);
  }
  return_value_if_fail(clone_base_obj != NULL, NULL);

  ret = object_overload_create(clone_base_obj, overload->overload_vt);

  tk_object_unref(clone_base_obj);
  return ret;
}

static const object_vtable_t s_object_overload_vtable = {
    .type = OBJECT_OVERLOAD_TYPE,
    .desc = OBJECT_OVERLOAD_TYPE,
    .size = sizeof(object_overload_t),
    .on_destroy = object_overload_on_destroy,
    .compare = object_overload_compare,
    .get_prop = object_overload_get_prop,
    .set_prop = object_overload_set_prop,
    .copy_props = object_overload_copy_props,
    .remove_prop = object_overload_remove_prop,
    .foreach_prop = object_overload_foreach_prop,
    .clear_props = object_overload_clear_props,
    .find_prop = object_overload_find_prop,
    .find_props = object_overload_find_props,
    .can_exec = object_overload_can_exec,
    .exec = object_overload_exec,
    .exec_ex = object_overload_exec_ex,
    .clone = object_overload_clone,
};

tk_object_t* object_overload_create(tk_object_t* base_obj, const object_vtable_t* overload_vt) {
  object_overload_t* ret = NULL;
  return_value_if_fail(base_obj != NULL, NULL);

  ret = (object_overload_t*)tk_object_create(&s_object_overload_vtable);
  return_value_if_fail(ret != NULL, NULL);

  ret->base_obj = TK_OBJECT_REF(base_obj);
  ret->overload_vt = overload_vt;

  return TK_OBJECT(ret);
}

object_overload_t* object_overload_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_overload_vtable, NULL);
  return (object_overload_t*)obj;
}

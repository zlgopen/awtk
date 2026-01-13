/**
 * File:   object_override.c
 * Author: AWTK Develop Team
 * Brief:  对象方法重写器。
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

#include "object_override.h"

#include "tkc/mem.h"
#include "tkc/utils.h"

static ret_t object_override_on_destroy(tk_object_t* obj) {
  ret_t ret = RET_OK;
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->on_destroy != NULL) {
    ret = override->override_vt->on_destroy(override->base_obj);
  }

  TK_OBJECT_UNREF(override->base_obj);

  return ret;
}

static int32_t object_override_compare(tk_object_t* obj, tk_object_t* other) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, -1);

  if (override->override_vt != NULL && override->override_vt->compare != NULL) {
    return override->override_vt->compare(override->base_obj, other);
  }

  return tk_object_compare(override->base_obj, other);
}

static ret_t object_override_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->get_prop != NULL) {
    return override->override_vt->get_prop(override->base_obj, name, v);
  }

  return tk_object_get_prop(override->base_obj, name, v);
}

static ret_t object_override_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->set_prop != NULL) {
    return override->override_vt->set_prop(override->base_obj, name, v);
  }

  return tk_object_set_prop(override->base_obj, name, v);
}

static ret_t object_override_copy_props(tk_object_t* obj, tk_object_t* src, bool_t overwrite) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->copy_props != NULL) {
    return override->override_vt->copy_props(override->base_obj, src, overwrite);
  }

  return tk_object_copy_props(override->base_obj, src, overwrite);
}

static ret_t object_override_remove_prop(tk_object_t* obj, const char* name) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->remove_prop != NULL) {
    return override->override_vt->remove_prop(override->base_obj, name);
  }

  return tk_object_remove_prop(override->base_obj, name);
}

static ret_t object_override_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->foreach_prop != NULL) {
    return override->override_vt->foreach_prop(override->base_obj, on_prop, ctx);
  }

  return tk_object_foreach_prop(override->base_obj, on_prop, ctx);
}

static ret_t object_override_clear_props(tk_object_t* obj) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->clear_props != NULL) {
    return override->override_vt->clear_props(override->base_obj);
  }

  return tk_object_clear_props(override->base_obj);
}

static value_t* object_override_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* ctx) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, NULL);

  if (override->override_vt != NULL && override->override_vt->find_prop != NULL) {
    return override->override_vt->find_prop(override->base_obj, cmp, ctx);
  }

  return tk_object_find_prop(override->base_obj, cmp, ctx);
}

static ret_t object_override_find_props(tk_object_t* obj, tk_compare_t cmp, const void* ctx,
                                        darray_t* matched) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->find_props != NULL) {
    return override->override_vt->find_props(override->base_obj, cmp, ctx, matched);
  }

  return tk_object_find_props(override->base_obj, cmp, ctx, matched);
}

static bool_t object_override_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, FALSE);

  if (override->override_vt != NULL && override->override_vt->can_exec != NULL) {
    return override->override_vt->can_exec(override->base_obj, name, args);
  }

  return tk_object_can_exec(override->base_obj, name, args);
}

static ret_t object_override_exec(tk_object_t* obj, const char* name, const char* args) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->exec != NULL) {
    return override->override_vt->exec(override->base_obj, name, args);
  }

  return tk_object_exec(override->base_obj, name, args);
}

static ret_t object_override_exec_ex(tk_object_t* obj, const char* name, const char* args,
                                     value_t* result) {
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, RET_BAD_PARAMS);

  if (override->override_vt != NULL && override->override_vt->exec_ex != NULL) {
    return override->override_vt->exec_ex(override->base_obj, name, args, result);
  }

  return tk_object_exec_ex(override->base_obj, name, args, result);
}

static tk_object_t* object_override_clone(tk_object_t* obj) {
  tk_object_t* ret = NULL;
  tk_object_t* clone_base_obj = NULL;
  object_override_t* override = OBJECT_OVERRIDE(obj);
  return_value_if_fail(override != NULL, NULL);

  if (override->override_vt != NULL && override->override_vt->clone != NULL) {
    clone_base_obj = override->override_vt->clone(override->base_obj);
  } else {
    clone_base_obj = tk_object_clone(override->base_obj);
  }
  return_value_if_fail(clone_base_obj != NULL, NULL);

  ret = object_override_create(clone_base_obj, override->override_vt);

  tk_object_unref(clone_base_obj);
  return ret;
}

static const object_vtable_t s_object_override_vtable = {
    .type = OBJECT_OVERRIDE_TYPE,
    .desc = OBJECT_OVERRIDE_TYPE,
    .size = sizeof(object_override_t),
    .on_destroy = object_override_on_destroy,
    .compare = object_override_compare,
    .get_prop = object_override_get_prop,
    .set_prop = object_override_set_prop,
    .copy_props = object_override_copy_props,
    .remove_prop = object_override_remove_prop,
    .foreach_prop = object_override_foreach_prop,
    .clear_props = object_override_clear_props,
    .find_prop = object_override_find_prop,
    .find_props = object_override_find_props,
    .can_exec = object_override_can_exec,
    .exec = object_override_exec,
    .exec_ex = object_override_exec_ex,
    .clone = object_override_clone,
};

tk_object_t* object_override_create(tk_object_t* base_obj, const object_vtable_t* override_vt) {
  object_override_t* ret = NULL;
  return_value_if_fail(base_obj != NULL, NULL);

  ret = (object_override_t*)tk_object_create(&s_object_override_vtable);
  return_value_if_fail(ret != NULL, NULL);

  ret->base_obj = TK_OBJECT_REF(base_obj);
  ret->override_vt = override_vt;

  return TK_OBJECT(ret);
}

object_override_t* object_override_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_override_vtable, NULL);
  return (object_override_t*)obj;
}

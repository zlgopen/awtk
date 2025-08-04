/**
 * File:   conf_utils.c
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
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
 * 2025-08-04 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "conf_utils.h"
#include "tkc/utils.h"
#include "tkc/object_hash.h"
#include "tkc/object_array.h"
#include "tkc/named_value.h"
#include "tkc/data_reader_mem.h"

#include "conf_io/conf_json.h"

ret_t object_to_json(tk_object_t* obj, str_t* str) {
  return tk_object_to_json(obj, str, 2, 0, FALSE);
}

static ret_t object_from_json_copy_props(tk_object_t* obj, tk_object_t* src);

static ret_t object_from_json_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  value_t value;

  if (v->type == VALUE_TYPE_OBJECT) {
    value_t tmp;
    tk_object_t* o = NULL;
    tk_object_t* v_obj = value_object(v);
    if (v_obj->vt->is_collection) {
      o = object_array_create();
    } else {
      o = object_hash_create();
    }
    object_from_json_copy_props(o, v_obj);
    value_set_object(&tmp, o);
    value_deep_copy(&value, &tmp);
    TK_OBJECT_UNREF(o);
  } else {
    value_copy(&value, v);
  }

  if (tk_object_is_instance_of(obj, OBJECT_ARRAY_TYPE)) {
    ret = object_array_push(obj, &value);
  } else {
    ret = tk_object_set_prop(obj, name, &value);
  }

  value_reset(&value);

  return ret;
}

static ret_t object_from_json_copy_props_on_visit(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  tk_object_t* obj = TK_OBJECT(ctx);
  object_from_json_set_prop(obj, nv->name, &(nv->value));
  return RET_OK;
}

inline static ret_t object_from_json_copy_props(tk_object_t* obj, tk_object_t* src) {
  return_value_if_fail(obj != NULL && src != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(src, object_from_json_copy_props_on_visit, obj);
}

ret_t object_from_json(tk_object_t* obj, const char* str) {
  ret_t ret = RET_OK;
  char url[MAX_PATH + 1];
  tk_object_t* obj_json = NULL;
  return_value_if_fail(obj != NULL && str != NULL, RET_BAD_PARAMS);

  data_reader_mem_build_url(str, tk_strlen(str), url);
  obj_json = conf_json_load_ex(url, FALSE, TRUE);
  return_value_if_fail(obj_json != NULL, RET_OOM);

  tk_object_clear_props(obj);
  ret = object_from_json_copy_props(obj, obj_json);

  TK_OBJECT_UNREF(obj_json);

  return ret;
}

/**
 * File:   view_model_normal.c
 * Author: AWTK Develop Team
 * Brief:  view_model_normal
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "mvvm/base/view_model_normal.h"

static ret_t view_model_normal_on_destroy(object_t* obj) {
  view_model_t* vm = VIEW_MODEL(obj);

  view_model_deinit(vm);

  return RET_OK;
}

static int32_t view_model_normal_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_normal_set_prop(object_t* obj, const char* name, const value_t* v) {
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

static ret_t view_model_normal_get_prop(object_t* obj, const char* name, value_t* v) {
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

static bool_t view_model_normal_can_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL && args != NULL, FALSE);

  return FALSE;
}

static ret_t view_model_normal_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL && args != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

static const object_vtable_t s_view_model_normal_vtable = {
    .type = "view_model_normal",
    .desc = "view_model_normal",
    .size = sizeof(view_model_normal_t),
    .is_collection = FALSE,
    .on_destroy = view_model_normal_on_destroy,

    .compare = view_model_normal_compare,
    .get_prop = view_model_normal_get_prop,
    .set_prop = view_model_normal_set_prop,
    .can_exec = view_model_normal_can_exec,
    .exec = view_model_normal_exec};

view_model_t* view_model_normal_create(model_t* model) {
  object_t* obj = object_create(&s_view_model_normal_vtable);

  return view_model_init(VIEW_MODEL(obj), VIEW_MODEL_NORMAL, model);
}

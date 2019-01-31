/**
 * File:   data_binding.c
 * Author: AWTK Develop Team
 * Brief:  data binding rule
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/value_converter.h"
#include "mvvm/base/value_validator.h"

#define STR_ONCE "Once"
#define STR_TWO_WAY "TwoWay"
#define STR_ONE_WAY "OneWay"
#define STR_ONE_WAY_TO_MODEL "OneWayToModel"

#define STR_CHANGED "Changed"
#define STR_CHANGING "Changing"
#define STR_EXPLICIT "Explicit"

static data_binding_t* data_binding_cast(void* rule);

static ret_t data_binding_on_destroy(object_t* obj) {
  data_binding_t* rule = data_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->props != NULL) {
    object_unref(rule->props);
  }

  return RET_OK;
}

static ret_t data_binding_object_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = value_str(v);
  data_binding_t* rule = data_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);
  if (rule->path == NULL && value == NULL) {
    value = name;
    rule->path = tk_str_copy(rule->path, value);
  } else if (tk_str_eq(DATA_BINDING_MODE, name)) {
    binding_mode_t mode = BINDING_TWO_WAY;

    if (tk_str_eq(value, STR_ONCE)) {
      mode = BINDING_ONCE;
    } else if (tk_str_eq(value, STR_ONCE)) {
      mode = BINDING_ONCE;
    } else if (tk_str_eq(value, STR_TWO_WAY)) {
      mode = BINDING_TWO_WAY;
    } else if (tk_str_eq(value, STR_ONE_WAY)) {
      mode = BINDING_ONE_WAY;
    } else if (tk_str_eq(value, STR_ONE_WAY_TO_MODEL)) {
      mode = BINDING_ONE_WAY_TO_MODEL;
    }

    rule->mode = mode;
  } else if (tk_str_eq(DATA_BINDING_TRIGGER, name)) {
    update_model_trigger_t trigger = UPDATE_WHEN_CHANGED;

    if (tk_str_eq(value, STR_CHANGED)) {
      trigger = UPDATE_WHEN_CHANGED;
    } else if (tk_str_eq(value, STR_CHANGING)) {
      trigger = UPDATE_WHEN_CHANGING;
    } else if (tk_str_eq(value, STR_EXPLICIT)) {
      trigger = UPDATE_WHEN_EXPLICIT;
    }

    rule->trigger = trigger;
  } else if (tk_str_eq(DATA_BINDING_PATH, name)) {
    rule->path = tk_str_copy(rule->path, value);
  } else if (tk_str_eq(DATA_BINDING_PROP, name)) {
    rule->prop = tk_str_copy(rule->prop, value);
  } else if (tk_str_eq(DATA_BINDING_CONVERTER, name)) {
    rule->converter = tk_str_copy(rule->converter, value);
  } else if (tk_str_eq(DATA_BINDING_VALIDATOR, name)) {
    rule->validator = tk_str_copy(rule->validator, value);
  } else {
    if (rule->props == NULL) {
      rule->props = object_default_create();
    }
    ret = object_set_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t data_binding_object_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  data_binding_t* rule = data_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(DATA_BINDING_MODE, name)) {
    value_set_int(v, rule->mode);
  } else if (tk_str_eq(DATA_BINDING_TRIGGER, name)) {
    value_set_int(v, rule->trigger);
  } else if (tk_str_eq(DATA_BINDING_PATH, name)) {
    value_set_str(v, rule->path);
  } else if (tk_str_eq(DATA_BINDING_PROP, name)) {
    value_set_str(v, rule->prop);
  } else if (tk_str_eq(DATA_BINDING_CONVERTER, name)) {
    value_set_str(v, rule->converter);
  } else if (tk_str_eq(DATA_BINDING_VALIDATOR, name)) {
    value_set_str(v, rule->validator);
  } else {
    ret = object_get_prop(rule->props, name, v);
  }

  return ret;
}

static const object_vtable_t s_data_binding_vtable = {.type = "data_binding",
                                                      .desc = "data_binding",
                                                      .size = sizeof(data_binding_t),
                                                      .is_collection = FALSE,
                                                      .on_destroy = data_binding_on_destroy,
                                                      .get_prop = data_binding_object_get_prop,
                                                      .set_prop = data_binding_object_set_prop};

static data_binding_t* data_binding_cast(void* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(obj != NULL && obj->vt == &s_data_binding_vtable, NULL);

  return (data_binding_t*)rule;
}

data_binding_t* data_binding_create(void) {
  data_binding_t* rule = TKMEM_ZALLOC(data_binding_t);
  return_value_if_fail(rule != NULL, NULL);

  ((object_t*)rule)->vt = &s_data_binding_vtable;

  rule->props = object_default_create();

  if (rule->props == NULL) {
    TKMEM_FREE(rule);
    rule = NULL;
  }

  return rule;
}

static ret_t value_to_model(const char* name, const value_t* from, value_t* to) {
  if (name != NULL) {
    value_converter_t* c = value_converter_create(name);
    if (c != NULL) {
      if (value_converter_to_model(c, from, to) == RET_OK) {
        object_unref(OBJECT(c));
        return RET_OK;
      } else {
        log_debug("value_converter_to_model %s failed\n", name);
        object_unref(OBJECT(c));
        return RET_FAIL;
      }
    } else {
      log_debug("not found value_converter %s\n", name);
      return RET_FAIL;
    }
  } else {
    *to = *from;
    return RET_OK;
  }
}

static ret_t value_to_view(const char* name, value_t* from, value_t* to) {
  if (name != NULL) {
    value_converter_t* c = value_converter_create(name);
    if (c != NULL) {
      if (value_converter_to_view(c, from, to) == RET_OK) {
        value_reset(from);
        object_unref(OBJECT(c));
        return RET_OK;
      } else {
        log_debug("value_converter_to_model %s failed\n", name);
        object_unref(OBJECT(c));
        return RET_FAIL;
      }
    } else {
      log_debug("not found value_converter %s\n", name);
      return RET_FAIL;
    }
  } else {
    *to = *from;
    return RET_OK;
  }
}

static bool_t value_is_valid(const char* name, const value_t* value, str_t* msg) {
  bool_t ret = FALSE;
  value_validator_t* validator = NULL;

  if (name == NULL) {
    return TRUE;
  }

  validator = value_validator_create(name);
  if (validator != NULL) {
    ret = value_validator_is_valid(validator, value, msg);
    object_unref(OBJECT(validator));
  } else {
    log_debug("not found validator\n");
  }

  return ret;
}

ret_t data_binding_get_prop(data_binding_t* rule, value_t* v) {
  value_t raw;
  binding_context_t* ctx = NULL;
  return_value_if_fail(rule != NULL && v != NULL, RET_BAD_PARAMS);

  ctx = BINDING_RULE(rule)->binding_context;
  return_value_if_fail(ctx != NULL && ctx->vm != NULL, RET_BAD_PARAMS);

  return_value_if_fail(view_model_get_prop(ctx->vm, rule->path, &raw) == RET_OK, RET_FAIL);

  return value_to_view(rule->converter, &raw, v);
}

ret_t data_binding_set_prop(data_binding_t* rule, const value_t* raw) {
  value_t v;
  binding_context_t* ctx = NULL;
  return_value_if_fail(rule != NULL && raw != NULL, RET_BAD_PARAMS);

  ctx = BINDING_RULE(rule)->binding_context;
  return_value_if_fail(ctx != NULL && ctx->vm != NULL, RET_BAD_PARAMS);

  str_clear(&(ctx->vm->last_error));
  if (!value_is_valid(rule->validator, raw, &(ctx->vm->last_error))) {
    return RET_BAD_PARAMS;
  }

  return_value_if_fail(value_to_model(rule->converter, raw, &v) == RET_OK, RET_FAIL);

  return view_model_set_prop(ctx->vm, rule->path, &v);
}

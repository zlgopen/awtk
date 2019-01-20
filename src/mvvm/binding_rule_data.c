/**
 * File:   binding_rule_data.c
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
#include "mvvm/binding_rule_data.h"

#define STR_PROP_PATH "Path"
#define STR_PROP_MODE "Mode"
#define STR_PROP_TRIGGER "Trigger"
#define STR_PROP_CONVERTER "Converter"
#define STR_PROP_VALIDATOR "Validator"

#define STR_ONCE "Once"
#define STR_TWO_WAY "TwoWay"
#define STR_ONE_WAY "OneWay"
#define STR_ONE_WAY_TO_MODEL "OneWayToModel"

#define STR_CHANGED "Changed"
#define STR_CHANGING "Changing"
#define STR_EXPLICIT "Explicit"

static binding_rule_data_t* binding_rule_data_cast(void* rule);

static ret_t binding_rule_data_on_destroy(object_t* obj) {
  binding_rule_data_t* rule = binding_rule_data_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  str_reset(&(rule->last_error));
  object_unref(rule->props);

  return RET_OK;
}

static ret_t binding_rule_data_remove_prop(object_t* obj, const char* name) {
  binding_rule_data_t* rule = binding_rule_data_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  return object_remove_prop(rule->props, name);
}

static ret_t binding_rule_data_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_FAIL;
  binding_rule_data_t* rule = binding_rule_data_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(STR_PROP_MODE, name)) {
    const char* str = value_str(v);
    binding_mode_t mode = BINDING_TWO_WAY;

    if (tk_str_eq(str, STR_ONCE)) {
      mode = BINDING_ONCE;
    } else if (tk_str_eq(str, STR_ONCE)) {
      mode = BINDING_ONCE;
    } else if (tk_str_eq(str, STR_TWO_WAY)) {
      mode = BINDING_TWO_WAY;
    } else if (tk_str_eq(str, STR_ONE_WAY_TO_MODEL)) {
      mode = BINDING_ONE_WAY_TO_MODEL;
    }

    ret = object_set_prop_int(rule->props, name, mode);
  } else if (tk_str_eq(STR_PROP_TRIGGER, name)) {
    const char* str = value_str(v);
    update_model_trigger_t trigger = UPDATE_WHEN_CHANGED;

    if (tk_str_eq(str, STR_CHANGED)) {
      trigger = UPDATE_WHEN_CHANGED;
    } else if (tk_str_eq(str, STR_CHANGING)) {
      trigger = UPDATE_WHEN_CHANGING;
    } else if (tk_str_eq(str, STR_EXPLICIT)) {
      trigger = UPDATE_WHEN_EXPLICIT;
    }

    ret = object_set_prop_int(rule->props, name, trigger);
  } else {
    ret = object_set_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t binding_rule_data_get_prop(object_t* obj, const char* name, value_t* v) {
  binding_rule_data_t* rule = binding_rule_data_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  return object_get_prop(rule->props, name, v);
}

static ret_t binding_rule_data_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  binding_rule_data_t* rule = binding_rule_data_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  return object_foreach_prop(rule->props, on_prop, ctx);
}

static const object_vtable_t s_binding_rule_data_vtable = {
    .type = "binding_rule_data",
    .desc = "binding_rule_data",
    .size = sizeof(binding_rule_data_t),
    .is_collection = FALSE,
    .on_destroy = binding_rule_data_on_destroy,
    .get_prop = binding_rule_data_get_prop,
    .set_prop = binding_rule_data_set_prop,
    .remove_prop = binding_rule_data_remove_prop,
    .foreach_prop = binding_rule_data_foreach_prop};

static binding_rule_data_t* binding_rule_data_cast(void* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(obj != NULL && obj->vt == &s_binding_rule_data_vtable, NULL);

  return (binding_rule_data_t*)rule;
}

binding_rule_t* binding_rule_data_create(void) {
  binding_rule_data_t* rule = TKMEM_ZALLOC(binding_rule_data_t);
  return_value_if_fail(rule != NULL, NULL);

  ((object_t*)rule)->vt = &s_binding_rule_data_vtable;

  str_init(&(rule->last_error), 0);
  rule->props = object_default_create();

  if (rule->props == NULL) {
    TKMEM_FREE(rule);
    rule = NULL;
  }

  return BINDING_RULE(rule);
}

const char* binding_rule_data_get_path(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(rule != NULL, NULL);

  return object_get_prop_str(obj, STR_PROP_PATH);
}

const char* binding_rule_data_get_converter(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(rule != NULL, NULL);

  return object_get_prop_str(obj, STR_PROP_CONVERTER);
}

const char* binding_rule_data_get_validator(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(rule != NULL, NULL);

  return object_get_prop_str(obj, STR_PROP_VALIDATOR);
}

binding_mode_t binding_rule_data_get_mode(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(rule != NULL, BINDING_TWO_WAY);

  return (binding_mode_t)object_get_prop_int(obj, STR_PROP_MODE, BINDING_TWO_WAY);
}

update_model_trigger_t binding_rule_data_get_trigger(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(rule != NULL, UPDATE_WHEN_CHANGED);

  return (update_model_trigger_t)object_get_prop_int(obj, STR_PROP_TRIGGER, UPDATE_WHEN_CHANGED);
}

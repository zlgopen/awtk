/**
 * File:   command_binding.c
 * Author: AWTK Develop Team
 * Brief:  command binding
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
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/command_binding.h"

static command_binding_t* command_binding_cast(void* rule);

static ret_t command_binding_on_destroy(object_t* obj) {
  command_binding_t* rule = command_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->props != NULL) {
    object_unref(rule->props);
  }

  return RET_OK;
}

static ret_t command_binding_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = value_str(v);
  command_binding_t* rule = command_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->command == NULL && value == NULL) {
    value = name;
    rule->command = tk_str_copy(rule->command, value);
  } else if (tk_str_eq(COMMAND_BINDING_COMMAND, name)) {
    rule->command = tk_str_copy(rule->command, value);
  } else if (tk_str_eq(COMMAND_BINDING_ARGS, name)) {
    rule->args = tk_str_copy(rule->args, value);
  } else if (tk_str_eq(COMMAND_BINDING_EVENT, name)) {
    rule->event = tk_str_copy(rule->event, value);
  } else if (tk_str_eq(COMMAND_BINDING_EVENT_ARGS, name)) {
    rule->event_args = tk_str_copy(rule->event_args, value);
  } else if (tk_str_eq(COMMAND_BINDING_CLOSE_WINDOW, name)) {
    rule->close_window = value != NULL ? tk_atob(value) : TRUE;
  } else if (tk_str_eq(COMMAND_BINDING_UPDATE_MODEL, name)) {
    rule->update_model = value != NULL ? tk_atob(value) : TRUE;
  } else {
    if (rule->props == NULL) {
      rule->props = object_default_create();
    }
    ret = object_set_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t command_binding_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  command_binding_t* rule = command_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(COMMAND_BINDING_COMMAND, name)) {
    value_set_str(v, rule->command);
  } else if (tk_str_eq(COMMAND_BINDING_ARGS, name)) {
    value_set_str(v, rule->args);
  } else if (tk_str_eq(COMMAND_BINDING_EVENT, name)) {
    value_set_str(v, rule->event);
  } else if (tk_str_eq(COMMAND_BINDING_CLOSE_WINDOW, name)) {
    value_set_bool(v, rule->close_window);
  } else if (tk_str_eq(COMMAND_BINDING_UPDATE_MODEL, name)) {
    value_set_bool(v, rule->update_model);
  } else {
    ret = object_get_prop(rule->props, name, v);
  }

  return ret;
}

static const object_vtable_t s_command_binding_vtable = {.type = "command_binding",
                                                         .desc = "command_binding",
                                                         .size = sizeof(command_binding_t),
                                                         .is_collection = FALSE,
                                                         .on_destroy = command_binding_on_destroy,
                                                         .get_prop = command_binding_get_prop,
                                                         .set_prop = command_binding_set_prop};

static command_binding_t* command_binding_cast(void* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(obj != NULL && obj->vt == &s_command_binding_vtable, NULL);

  return (command_binding_t*)rule;
}

binding_rule_t* command_binding_create(void) {
  command_binding_t* rule = TKMEM_ZALLOC(command_binding_t);
  return_value_if_fail(rule != NULL, NULL);

  ((object_t*)rule)->vt = &s_command_binding_vtable;

  return BINDING_RULE(rule);
}

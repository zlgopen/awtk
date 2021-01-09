/**
 * File:   fscript_widget.c
 * Author: AWTK Develop Team
 * Brief:  widget functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-08 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "awtk_global.h"
#include "tkc/fscript.h"
#include "tkc/tokenizer.h"
#include "base/window.h"
#include "base/locale_info.h"
#include "base/widget_factory.h"
#include "base/window_manager.h"
#include "ui_loader/ui_builder_default.h"

static ret_t func_tr(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_dup_str(result, locale_info_tr(locale_info(), value_str(args->args)));

  return RET_OK;
}

static ret_t func_window_open(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  widget = window_open(value_str(args->args));
  value_set_pointer(result, widget);

  return RET_OK;
}

static ret_t func_window_close(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v = NULL;
  ret_t ret = RET_OK;

  if (args->size == 1) {
    v = args->args;
    if (v->type == VALUE_TYPE_STRING) {
      ret = window_close(widget_child(window_manager(), value_str(v)));
    } else if (v->type == VALUE_TYPE_POINTER) {
      ret = window_close(WIDGET(value_pointer(v)));
    } else {
      ret = window_manager_back(window_manager());
    }
  } else {
    widget_t* self = WIDGET(object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    ret = window_close(widget_get_window(self));
  }
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static widget_t* find_target_widget(widget_t* widget, const char* path, uint32_t len) {
  bool_t is_first = TRUE;
  tokenizer_t tokenizer;
  widget_t* iter = widget;
  tokenizer_t* t = NULL;
  return_value_if_fail(widget != NULL && path != NULL, NULL);
  t = tokenizer_init(&tokenizer, path, len, ".");
  return_value_if_fail(t != NULL, NULL);

  while (tokenizer_has_more(t) && iter != NULL) {
    const char* name = tokenizer_next(t);
    if (is_first) {
      if (tk_str_eq(name, STR_PROP_PARENT)) {
        iter = widget->parent;
      } else if (tk_str_eq(name, STR_PROP_WINDOW)) {
        iter = widget_get_window(widget);
      } else if (tk_str_eq(name, STR_PROP_WINDOW_MANAGER)) {
        iter = widget_get_window_manager(widget);
      } else {
        iter = widget_child(iter, name);
      }
      is_first = FALSE;
    } else {
      iter = widget_child(iter, name);
    }
  }
  tokenizer_deinit(t);

  return iter;
}

static ret_t widget_set(widget_t* self, const char* path, const value_t* v) {
  widget_t* widget = self;
  const char* prop = strrchr(path, '.');
  if (prop != NULL) {
    widget = find_target_widget(self, path, prop - path);
    prop++;
  } else {
    prop = path;
  }
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, prop, v);
}

static ret_t widget_get(widget_t* self, const char* path, value_t* v) {
  widget_t* widget = self;
  const char* prop = strrchr(path, '.');
  if (prop != NULL) {
    widget = find_target_widget(self, path, prop - path);
    prop++;
  } else {
    prop = path;
  }
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_get_prop(widget, prop, v);
}

static ret_t func_quit(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_quit();

  return RET_OK;
}

static ret_t func_back(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  window_manager_back(window_manager());

  return RET_OK;
}

static ret_t func_back_to_home(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  window_manager_back_to_home(window_manager());

  return RET_OK;
}

static ret_t func_widget_lookup(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  const char* path = NULL;
  bool_t recursive = FALSE;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    widget = WIDGET(object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
  } else {
    widget = WIDGET(value_pointer(args->args));
    path = value_str(args->args + 1);
    recursive = args->size > 2 ? value_bool(args->args + 2) : FALSE;
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  if(strchr(path, '.') != NULL) {
    widget = find_target_widget(widget, path, strlen(path));
  } else {
    widget = widget_lookup(widget, path, recursive);
  }
  value_set_pointer(result, widget);

  return RET_OK;
}

static ret_t func_widget_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  const char* path = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    widget = WIDGET(object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
  } else {
    widget = WIDGET(value_pointer(args->args));
    path = value_str(args->args + 1);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  return widget_get(widget, path, result);
}

static ret_t func_widget_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v = NULL;
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  const char* path = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);

  if (args->size == 2) {
    widget = WIDGET(object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
    v = args->args + 1;
  } else {
    widget = WIDGET(value_pointer(args->args));
    path = value_str(args->args + 1);
    v = args->args + 2;
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  ret = widget_set(widget, path, v);
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_widget_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  const char* type = NULL;
  widget_t* widget = NULL;
  widget_t* parent = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 6, RET_BAD_PARAMS);
  type = value_str(args->args);
  parent = value_pointer(args->args+1);
  return_value_if_fail(type != NULL && parent != NULL, RET_BAD_PARAMS);
  x = value_int(args->args+2);
  y = value_int(args->args+3);
  w = value_int(args->args+4);
  h = value_int(args->args+5);
  widget = widget_factory_create_widget(widget_factory(), type, parent, x, y, w, h);
  value_set_pointer(result, widget);

  return RET_OK;
}

static ret_t func_widget_destroy(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, widget_destroy(WIDGET(value_pointer(args->args))) == RET_OK);

  return RET_OK;
}

ret_t fscript_widget_register(void) {
  ENSURE(fscript_register_func("tr", func_tr) == RET_OK);
  ENSURE(fscript_register_func("open", func_window_open) == RET_OK);
  ENSURE(fscript_register_func("close", func_window_close) == RET_OK);
  ENSURE(fscript_register_func("quit", func_quit) == RET_OK);
  ENSURE(fscript_register_func("back", func_back) == RET_OK);
  ENSURE(fscript_register_func("widget_lookup", func_widget_lookup) == RET_OK);
  ENSURE(fscript_register_func("widget_get", func_widget_get) == RET_OK);
  ENSURE(fscript_register_func("widget_set", func_widget_set) == RET_OK);
  ENSURE(fscript_register_func("back_to_home", func_back_to_home) == RET_OK);
  ENSURE(fscript_register_func("widget_create", func_widget_create) == RET_OK);
  ENSURE(fscript_register_func("widget_destroy", func_widget_destroy) == RET_OK);

  return RET_OK;
}

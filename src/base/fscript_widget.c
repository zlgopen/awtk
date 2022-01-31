/**
 * File:   fscript_widget.c
 * Author: AWTK Develop Team
 * Brief:  widget functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-08 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "awtk_global.h"
#include "tkc/utf8.h"
#include "tkc/fscript.h"
#include "tkc/tokenizer.h"
#include "tkc/object_default.h"
#include "base/enums.h"
#include "base/main_loop.h"
#include "base/window.h"
#include "base/locale_info.h"
#include "base/object_widget.h"
#include "base/widget_factory.h"
#include "base/window_manager.h"
#include "ui_loader/ui_builder_default.h"

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
      } else if (tk_str_eq(name, STR_PROP_SELF)) {
        iter = widget;
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

static widget_t* to_widget(fscript_t* fscript, const value_t* v) {
  if (v->type == VALUE_TYPE_STRING) {
    widget_t* self = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    const char* path = value_str(v);
    return_value_if_fail(path != NULL, NULL);

    return find_target_widget(self, path, strlen(path));
  } else if (v->type == VALUE_TYPE_POINTER) {
    return WIDGET(value_pointer(v));
  } else if (v->type == VALUE_TYPE_OBJECT) {
    object_widget_t* o = OBJECT_WIDGET(value_object(v));
    return_value_if_fail(o != NULL, NULL);
    return o->widget;
  } else {
    return NULL;
  }
}

static ret_t func_tr(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_dup_str(result, locale_info_tr(locale_info(), value_str(args->args)));

  return RET_OK;
}

static ret_t func_window_open(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  const char* name = NULL;
  tk_object_t* obj_widget = NULL;
  bool_t close_current = FALSE;
  bool_t switch_to_if_exist = FALSE;
  widget_t* wm = window_manager();
  widget_t* self = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  widget_t* curr_win = widget_get_window(self);
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  name = value_str(args->args);
  close_current = args->size > 1 ? value_bool(args->args + 1) : FALSE;
  switch_to_if_exist = args->size > 2 ? value_bool(args->args + 2) : FALSE;
  FSCRIPT_FUNC_CHECK(name != NULL, RET_BAD_PARAMS);

  if (switch_to_if_exist) {
    widget_t* widget = widget_child(wm, name);
    if (widget != NULL) {
      window_manager_switch_to(wm, curr_win, widget, close_current);
      obj_widget = object_widget_create(widget);
      value_set_object(result, obj_widget);
      result->free_handle = TRUE;
      return RET_OK;
    }
  }

  if (close_current) {
    widget = window_open_and_close(value_str(args->args), curr_win);
  } else {
    widget = window_open(value_str(args->args));
  }

  obj_widget = object_widget_create(widget);
  value_set_object(result, obj_widget);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_window_close(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;

  if (args->size == 1) {
    value_t* v = args->args;
    widget_t* wm = window_manager();
    widget_t* win = widget_child(wm, value_str(v));
    FSCRIPT_FUNC_CHECK(win != NULL, RET_BAD_PARAMS);
    ret = window_manager_close_window_force(wm, win);
  } else {
    widget_t* self = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    ret = window_close(widget_get_window(self));
  }
  value_set_bool(result, ret == RET_OK);

  return ret;
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
  ret_t ret = RET_OK;
  widget_t* widget = self;
  const char* prop = strrchr(path, '.');
  if (prop != NULL) {
    widget = find_target_widget(self, path, prop - path);
    prop++;
  } else {
    prop = path;
  }
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  ret = widget_get_prop(widget, prop, v);
  if (ret == RET_OK && v->type == VALUE_TYPE_WSTRING) {
    value_set_str(v, tk_utf8_dup_utf16(value_wstr(v), -1));
    v->free_handle = TRUE;
  }

  return ret;
}

static ret_t my_quit_idle(const timer_info_t* timer) {
  main_loop_t* loop = main_loop();

  loop->app_quited = TRUE;

  return main_loop_quit(loop);
}

static ret_t func_quit(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  timer_add(my_quit_idle, NULL, 0);

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
  tk_object_t* obj_widget = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
  } else {
    widget = to_widget(fscript, args->args);
    path = value_str(args->args + 1);
    recursive = args->size > 2 ? value_bool(args->args + 2) : FALSE;
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  if (strchr(path, '.') != NULL) {
    widget = find_target_widget(widget, path, strlen(path));
  } else {
    widget = widget_lookup(widget, path, recursive);
  }

  obj_widget = object_widget_create(widget);
  value_set_object(result, obj_widget);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_widget_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  const char* path = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
  } else {
    widget = to_widget(fscript, args->args);
    path = value_str(args->args + 1);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  return widget_get(widget, path, result);
}

static ret_t func_widget_eval(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t v;
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  const char* path = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  if (args->size == 1) {
    widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
  } else {
    widget = to_widget(fscript, args->args);
    path = value_str(args->args + 1);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && path != NULL, RET_BAD_PARAMS);

  if (widget_get(widget, path, &v) == RET_OK) {
    ret = fscript_eval(fscript->obj, value_str(&v), result);
    value_reset(&v);
  }

  return ret;
}

static ret_t func_widget_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_t* v = NULL;
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  const char* path = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);

  if (args->size == 2) {
    widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
    path = value_str(args->args);
    v = args->args + 1;
  } else {
    widget = to_widget(fscript, args->args);
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
  tk_object_t* obj_widget = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 6, RET_BAD_PARAMS);
  type = value_str(args->args);
  parent = to_widget(fscript, args->args + 1);
  return_value_if_fail(type != NULL && parent != NULL, RET_BAD_PARAMS);
  x = value_int(args->args + 2);
  y = value_int(args->args + 3);
  w = value_int(args->args + 4);
  h = value_int(args->args + 5);
  widget = widget_factory_create_widget(widget_factory(), type, parent, x, y, w, h);

  obj_widget = object_widget_create(widget);
  value_set_object(result, obj_widget);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_widget_destroy(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  widget_t* widget = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  widget = to_widget(fscript, args->args);
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);

  value_set_bool(result, widget_destroy(widget) == RET_OK);

  return RET_OK;
}

#define STR_PROP_TIMER_ID "_timer_id_"

static ret_t widget_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  ret_t ret = widget_dispatch_simple_event(widget, EVT_TIMER);
  ret = ret == RET_REMOVE ? RET_REMOVE : RET_REPEAT;
  if (ret == RET_REMOVE) {
    widget_set_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  }
  return ret;
}

static ret_t func_widget_add_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  uint32_t duration = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  if (args->size > 1) {
    widget = to_widget(fscript, args->args);
    duration = value_uint32(args->args + 1);
  } else {
    duration = value_uint32(args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && duration > 0, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_remove(id);
    log_debug("timer exist, remove it.\n");
  }

  id = widget_add_timer(widget, widget_on_timer, duration);
  value_set_uint32(result, id);
  widget_set_prop_int(widget, STR_PROP_TIMER_ID, id);

  return RET_OK;
}

static ret_t func_widget_remove_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_remove(id);
    value_set_bool(result, TRUE);
    widget_set_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  } else {
    value_set_bool(result, FALSE);
    log_debug("not found timer\n");
  }

  return RET_OK;
}

static ret_t func_widget_reset_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_reset(id);
    value_set_bool(result, TRUE);
  } else {
    value_set_bool(result, FALSE);
    log_debug("not found timer\n");
  }

  return RET_OK;
}

static ret_t func_widget_modify_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  uint32_t duration = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  if (args->size > 1) {
    widget = to_widget(fscript, args->args);
    duration = value_uint32(args->args + 1);
  } else {
    duration = value_uint32(args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && duration > 0, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_modify(id, duration);
    value_set_bool(result, TRUE);
  } else {
    value_set_bool(result, FALSE);
    log_debug("not found timer\n");
  }

  return RET_OK;
}

static ret_t func_widget_suspend_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_suspend(id);
    value_set_bool(result, TRUE);
  } else {
    value_set_bool(result, FALSE);
    log_debug("not found timer\n");
  }
  return RET_OK;
}

static ret_t func_widget_resume_timer(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t id = 0;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);

  id = widget_get_prop_int(widget, STR_PROP_TIMER_ID, TK_INVALID_ID);
  if (id != TK_INVALID_ID) {
    timer_resume(id);
    value_set_bool(result, TRUE);
  } else {
    value_set_bool(result, FALSE);
    log_debug("not found timer\n");
  }
  return RET_OK;
}

static ret_t func_widget_send_key(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* key = NULL;
  widget_t* widget = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  widget = to_widget(fscript, args->args);
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  key = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(key != NULL, RET_BAD_PARAMS);

  if (*key) {
    key_event_t e;
    const key_type_value_t* kt = keys_type_find(key);
    int32_t code = kt != NULL ? kt->value : *key;

    key_event_init(&e, EVT_KEY_DOWN, widget, code);
    widget_dispatch(widget, (event_t*)&e);
    key_event_init(&e, EVT_KEY_UP, widget, code);
    widget_dispatch(widget, (event_t*)&e);
  }
  value_set_bool(result, *key != '\0');

  return RET_OK;
}

static ret_t func_locale_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  locale_info_t* info = NULL;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  info = widget_get_locale_info(widget);
  FSCRIPT_FUNC_CHECK(info != NULL, RET_BAD_PARAMS);

  obj = object_default_create();
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_OOM);
  tk_object_set_prop_str(obj, "language", info->language);
  tk_object_set_prop_str(obj, "country", info->country);
  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_locale_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  locale_info_t* info = NULL;
  const char* language = NULL;
  const char* country = NULL;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  if (args->size > 2) {
    widget = to_widget(fscript, args->args);
    language = value_str(args->args + 1);
    country = value_str(args->args + 2);
  } else {
    language = value_str(args->args);
    country = value_str(args->args + 1);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && language != NULL && country != NULL, RET_BAD_PARAMS);
  info = widget_get_locale_info(widget);
  FSCRIPT_FUNC_CHECK(info != NULL, RET_BAD_PARAMS);

  value_set_bool(result, locale_info_change(info, language, country) == RET_OK);

  return RET_OK;
}

static ret_t func_theme_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  assets_manager_t* am = NULL;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  if (args->size > 0) {
    widget = to_widget(fscript, args->args);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL, RET_BAD_PARAMS);
  am = widget_get_assets_manager(widget);
  FSCRIPT_FUNC_CHECK(am != NULL, RET_BAD_PARAMS);

  value_dup_str(result, am->theme);

  return RET_OK;
}

static ret_t func_theme_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* name = NULL;
  widget_t* widget = WIDGET(tk_object_get_prop_pointer(fscript->obj, STR_PROP_SELF));
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  if (args->size == 1) {
    name = value_str(args->args);
  } else {
    widget = to_widget(fscript, args->args);
    name = value_str(args->args + 1);
  }
  FSCRIPT_FUNC_CHECK(widget != NULL && name != NULL, RET_BAD_PARAMS);

  value_set_bool(result, widget_set_theme(widget, name) == RET_OK);

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_widget)
FACTORY_TABLE_ENTRY("open", func_window_open)
FACTORY_TABLE_ENTRY("close", func_window_close)
FACTORY_TABLE_ENTRY("back", func_back)
FACTORY_TABLE_ENTRY("back_to_home", func_back_to_home)
FACTORY_TABLE_ENTRY("quit", func_quit)
FACTORY_TABLE_ENTRY("tr", func_tr)
FACTORY_TABLE_ENTRY("widget_lookup", func_widget_lookup)
FACTORY_TABLE_ENTRY("widget_get", func_widget_get)
FACTORY_TABLE_ENTRY("widget_eval", func_widget_eval)
FACTORY_TABLE_ENTRY("widget_set", func_widget_set)
FACTORY_TABLE_ENTRY("widget_create", func_widget_create)
FACTORY_TABLE_ENTRY("widget_destroy", func_widget_destroy)
FACTORY_TABLE_ENTRY("start_timer", func_widget_add_timer)
FACTORY_TABLE_ENTRY("stop_timer", func_widget_remove_timer)
FACTORY_TABLE_ENTRY("reset_timer", func_widget_reset_timer)
FACTORY_TABLE_ENTRY("modify_timer", func_widget_modify_timer)
FACTORY_TABLE_ENTRY("suspend_timer", func_widget_suspend_timer)
FACTORY_TABLE_ENTRY("resume_timer", func_widget_resume_timer)
FACTORY_TABLE_ENTRY("send_key", func_widget_send_key)
FACTORY_TABLE_ENTRY("locale_get", func_locale_get)
FACTORY_TABLE_ENTRY("locale_set", func_locale_set)
FACTORY_TABLE_ENTRY("theme_get", func_theme_get)
FACTORY_TABLE_ENTRY("theme_set", func_theme_set)
FACTORY_TABLE_END()

ret_t fscript_widget_register(void) {
  return fscript_register_funcs(s_ext_widget);
}

/**
 * File:   window_animator_factory.c
 * Author: AWTK Develop Team
 * Brief:  window animator factory
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-03-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "tkc/easing.h"
#include "tkc/func_call_parser.h"
#include "base/window_animator_factory.h"

static window_animator_factory_t* window_animator_factory_init(window_animator_factory_t* factory);
static ret_t window_animator_factory_deinit(window_animator_factory_t* factory);

static window_animator_factory_t* s_window_animator_factory = NULL;

typedef struct _creator_item_t {
  char type[TK_NAME_LEN + 1];
  window_animator_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* type) {
  return strcmp(iter->type, type);
}

window_animator_factory_t* window_animator_factory(void) {
  return s_window_animator_factory;
}

window_animator_factory_t* window_animator_factory_create(void) {
  window_animator_factory_t* factory = TKMEM_ZALLOC(window_animator_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return window_animator_factory_init(factory);
}

static window_animator_factory_t* window_animator_factory_init(window_animator_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t window_animator_factory_register(window_animator_factory_t* factory, const char* type,
                                       window_animator_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->type, type, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

window_animator_t* window_animator_factory_create_animator(window_animator_factory_t* factory,
                                                           bool_t open, object_t* args) {
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && args != NULL, NULL);

  iter = darray_find(&(factory->creators), (void*)args->name);
  return_value_if_fail(iter != NULL, NULL);

  return iter->create(open, args);
}

ret_t window_animator_factory_set(window_animator_factory_t* factory) {
  s_window_animator_factory = factory;

  return RET_OK;
}

static ret_t window_animator_factory_deinit(window_animator_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t window_animator_factory_destroy(window_animator_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  window_animator_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

#ifdef WITH_WINDOW_ANIMATORS
static window_animator_t* window_animator_create_impl(const char* type, bool_t open) {
  value_t v;
  object_t* args = NULL;
  window_animator_t* wa = NULL;
  window_animator_factory_t* factory = window_animator_factory();

  if (factory == NULL || type == NULL || *type == '\0') {
    return NULL;
  }
  args = func_call_parse(type, strlen(type));
  return_value_if_fail(args != NULL && args->name[0], NULL);

  wa = window_animator_factory_create_animator(factory, open, args);
  return_value_if_fail(wa != NULL, NULL);

  if (object_get_prop(args, "duration", &v) == RET_OK) {
    wa->duration = value_int(&v);
  }

  if (object_get_prop(args, "easing", &v) == RET_OK) {
    const key_type_value_t* kv = easing_type_find(value_str(&v));

    if (kv != NULL) {
      wa->easing = easing_get((easing_type_t)(kv->value));
    }
  }

  object_unref(args);

  return wa;
}

window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create_impl(type, TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win);

  return wa;
}

window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create_impl(type, FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win);

  return wa;
}
#else
window_animator_t* window_animator_create_for_close(const char* type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  return NULL;
}

window_animator_t* window_animator_create_for_open(const char* type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  return NULL;
}
#endif /*WITH_WINDOW_ANIMATORS*/

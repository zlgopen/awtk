/**
 * File:   dialog_highlighter_factory.c
 * Author: AWTK Develop Team
 * Brief:  window highlighter factory
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
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "tkc/easing.h"
#include "base/window_manager.h"
#include "tkc/func_call_parser.h"
#include "base/dialog_highlighter_factory.h"

static dialog_highlighter_factory_t* dialog_highlighter_factory_init(
    dialog_highlighter_factory_t* factory);
static ret_t dialog_highlighter_factory_deinit(dialog_highlighter_factory_t* factory);

static dialog_highlighter_factory_t* s_dialog_highlighter_factory = NULL;

typedef struct _creator_item_t {
  char type[TK_NAME_LEN + 1];
  dialog_highlighter_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* type) {
  return strcmp(iter->type, type);
}

dialog_highlighter_factory_t* dialog_highlighter_factory(void) {
  return s_dialog_highlighter_factory;
}

dialog_highlighter_factory_t* dialog_highlighter_factory_create(void) {
  dialog_highlighter_factory_t* factory = TKMEM_ZALLOC(dialog_highlighter_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return dialog_highlighter_factory_init(factory);
}

static dialog_highlighter_factory_t* dialog_highlighter_factory_init(
    dialog_highlighter_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t dialog_highlighter_factory_register(dialog_highlighter_factory_t* factory, const char* type,
                                          dialog_highlighter_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->type, type, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

ret_t dialog_highlighter_on_dialog_destroy(void* ctx, event_t* e) {
  dialog_highlighter_t* h = (dialog_highlighter_t*)ctx;

  dialog_highlighter_destroy(h);

  log_debug("dialog_highlighter_on_dialog_destroy\n");

  return RET_REMOVE;
}

dialog_highlighter_t* dialog_highlighter_factory_create_highlighter(
    dialog_highlighter_factory_t* factory, const char* args, widget_t* dialog) {
  object_t* args_obj = NULL;
  dialog_highlighter_t* h = NULL;
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL, NULL);
  args_obj = func_call_parse(args, strlen(args));
  return_value_if_fail(args_obj != NULL, NULL);

  iter = darray_find(&(factory->creators), (void*)args_obj->name);
  if (iter != NULL) {
    h = iter->create(args_obj);
  }
  object_unref(args_obj);

  if (h != NULL) {
    h->dialog = dialog;
    if (dialog != NULL) {
      widget_on(dialog, EVT_DESTROY, dialog_highlighter_on_dialog_destroy, h);
    }
  }

  return h;
}

ret_t dialog_highlighter_factory_set(dialog_highlighter_factory_t* factory) {
  s_dialog_highlighter_factory = factory;

  return RET_OK;
}

static ret_t dialog_highlighter_factory_deinit(dialog_highlighter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t dialog_highlighter_factory_destroy(dialog_highlighter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  dialog_highlighter_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

/**
 * File:   children_layouter_factory.c
 * Author: AWTK Develop Team
 * Brief:  children layouter factory
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
#include "base/children_layouter_factory.h"

static children_layouter_factory_t* children_layouter_factory_init(
    children_layouter_factory_t* factory);
static ret_t children_layouter_factory_deinit(children_layouter_factory_t* factory);

static children_layouter_factory_t* s_children_layouter_factory = NULL;

typedef struct _creator_item_t {
  char type[TK_NAME_LEN + 1];
  children_layouter_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* type) {
  return strcmp(iter->type, type);
}

children_layouter_factory_t* children_layouter_factory(void) {
  return s_children_layouter_factory;
}

children_layouter_factory_t* children_layouter_factory_create(void) {
  children_layouter_factory_t* factory = TKMEM_ZALLOC(children_layouter_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return children_layouter_factory_init(factory);
}

static children_layouter_factory_t* children_layouter_factory_init(
    children_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t children_layouter_factory_register(children_layouter_factory_t* factory, const char* type,
                                         children_layouter_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->type, type, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

children_layouter_t* children_layouter_factory_create_layouter(children_layouter_factory_t* factory,
                                                               const char* type) {
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && type != NULL, NULL);

  iter = darray_find(&(factory->creators), (void*)type);
  if (iter != NULL) {
    return iter->create();
  }

  return NULL;
}

ret_t children_layouter_factory_set(children_layouter_factory_t* factory) {
  s_children_layouter_factory = factory;

  return RET_OK;
}

static ret_t children_layouter_factory_deinit(children_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t children_layouter_factory_destroy(children_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  children_layouter_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

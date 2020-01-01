/**
 * File:   self_layouter_factory.c
 * Author: AWTK Develop Team
 * Brief:  self layouter factory
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
#include "base/self_layouter_factory.h"

static self_layouter_factory_t* self_layouter_factory_init(self_layouter_factory_t* factory);
static ret_t self_layouter_factory_deinit(self_layouter_factory_t* factory);

static self_layouter_factory_t* s_self_layouter_factory = NULL;

typedef struct _creator_item_t {
  char type[TK_NAME_LEN + 1];
  self_layouter_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* type) {
  return strcmp(iter->type, type);
}

self_layouter_factory_t* self_layouter_factory(void) {
  return s_self_layouter_factory;
}

self_layouter_factory_t* self_layouter_factory_create(void) {
  self_layouter_factory_t* factory = TKMEM_ZALLOC(self_layouter_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return self_layouter_factory_init(factory);
}

static self_layouter_factory_t* self_layouter_factory_init(self_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t self_layouter_factory_register(self_layouter_factory_t* factory, const char* type,
                                     self_layouter_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->type, type, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

self_layouter_t* self_layouter_factory_create_layouter(self_layouter_factory_t* factory,
                                                       const char* type) {
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && type != NULL, NULL);

  iter = darray_find(&(factory->creators), (void*)type);
  if (iter != NULL) {
    return iter->create();
  }

  return NULL;
}

ret_t self_layouter_factory_set(self_layouter_factory_t* factory) {
  s_self_layouter_factory = factory;

  return RET_OK;
}

static ret_t self_layouter_factory_deinit(self_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t self_layouter_factory_destroy(self_layouter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  self_layouter_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

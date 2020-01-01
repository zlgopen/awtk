/**
 * File:   widget_factory.c
 * Author: AWTK Develop Team
 * Brief:  widget factory
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"

#include "tkc/value.h"
#include "tkc/utils.h"
#include "base/widget_factory.h"

static widget_factory_t* widget_factory_init(widget_factory_t* factory);
static ret_t widget_factory_deinit(widget_factory_t* factory);

static widget_factory_t* s_widget_factory = NULL;
typedef struct _creator_item_t {
  char type[TK_NAME_LEN + 1];
  widget_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* type) {
  return strcmp(iter->type, type);
}

widget_factory_t* widget_factory(void) {
  return s_widget_factory;
}

widget_factory_t* widget_factory_create(void) {
  widget_factory_t* factory = TKMEM_ZALLOC(widget_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return widget_factory_init(factory);
}

static widget_factory_t* widget_factory_init(widget_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  emitter_init(EMITTER(factory));
  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t widget_factory_register(widget_factory_t* factory, const char* type, widget_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->type, type, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

widget_t* widget_factory_create_widget(widget_factory_t* factory, const char* type,
                                       widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && type != NULL, NULL);

  iter = darray_find(&(factory->creators), (void*)type);
  return_value_if_fail(iter != NULL, NULL);

  widget = iter->create(parent, x, y, w, h);
  if (widget != NULL) {
    event_t e = event_init(EVT_WIDGET_CREATED, widget);
    emitter_dispatch(EMITTER(factory), &e);
  }

  return widget;
}

ret_t widget_factory_set(widget_factory_t* factory) {
  s_widget_factory = factory;

  return RET_OK;
}

static ret_t widget_factory_deinit(widget_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));
  emitter_deinit(EMITTER(factory));

  return RET_OK;
}

ret_t widget_factory_destroy(widget_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  widget_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

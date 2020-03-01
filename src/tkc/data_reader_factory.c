/**
 * File:   data_reader_factory.c
 * Author: AWTK Develop Team
 * Brief:  data reader factory
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-03-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/data_reader_factory.h"

static data_reader_factory_t* data_reader_factory_init(data_reader_factory_t* factory);
static ret_t data_reader_factory_deinit(data_reader_factory_t* factory);

static data_reader_factory_t* s_data_reader_factory = NULL;

typedef struct _creator_item_t {
  char protocol[TK_NAME_LEN + 1];
  data_reader_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, const char* protocol) {
  return strcmp(iter->protocol, protocol);
}

data_reader_factory_t* data_reader_factory(void) {
  return s_data_reader_factory;
}

data_reader_factory_t* data_reader_factory_create(void) {
  data_reader_factory_t* factory = TKMEM_ZALLOC(data_reader_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return data_reader_factory_init(factory);
}

static data_reader_factory_t* data_reader_factory_init(data_reader_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t data_reader_factory_register(data_reader_factory_t* factory, const char* protocol,
                                   data_reader_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && protocol != NULL && create != NULL, RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  tk_strncpy(item->protocol, protocol, TK_NAME_LEN);
  darray_push(&(factory->creators), item);

  return RET_OK;
}

data_reader_t* data_reader_factory_create_reader(data_reader_factory_t* factory, const char* url) {
  char protocol[TK_NAME_LEN + 1];
  const char* p = NULL;
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && url != NULL, NULL);

  p = strchr(url, ':');
  memset(protocol, 0x00, sizeof(protocol));

  if (p != NULL) {
    return_value_if_fail((p - url) < TK_NAME_LEN, NULL);
    strncpy(protocol, url, p - url);
  } else {
    strncpy(protocol, "file", 4);
  }

  iter = darray_find(&(factory->creators), (void*)protocol);
  if (iter != NULL) {
    p = strstr(url, "://");

    if (p == NULL) {
      p = url;
    } else {
      p += 3;
    }

    return iter->create(p);
  }

  return NULL;
}

ret_t data_reader_factory_set(data_reader_factory_t* factory) {
  s_data_reader_factory = factory;

  return RET_OK;
}

static ret_t data_reader_factory_deinit(data_reader_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t data_reader_factory_destroy(data_reader_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  data_reader_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}

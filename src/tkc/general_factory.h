/**
 * File:   general_factory.h
 * Author: AWTK Develop Team
 * Brief:  data writer factory
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2028-08-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GENERAL_FACTORY_H
#define TK_GENERAL_FACTORY_H

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"

BEGIN_C_DECLS

typedef struct _general_factory_const_item_t {
  const char* name;
  tk_create_t create;
} general_factory_const_item_t, general_factory_table_t;

#define FACTORY_TABLE_BEGIN(name) static const general_factory_const_item_t name[] = {
#define FACTORY_TABLE_ENTRY(name, create) {name, (tk_create_t)create},
#define FACTORY_TABLE_END() \
  { NULL, NULL }            \
  }                         \
  ;

typedef struct _general_factory_item_t {
  const char* name;
  tk_create_t create;
  uint8_t const_name;
  char data[1];
} general_factory_item_t;

static inline general_factory_item_t* general_factory_item_create(const char* name,
                                                                  tk_create_t create,
                                                                  bool_t is_const_name) {
  general_factory_item_t* item = NULL;
  uint32_t name_len = tk_strlen(name);
  uint32_t len = sizeof(general_factory_item_t);
  return_value_if_fail(create != NULL && name != NULL, NULL);

  if (!is_const_name) {
    len += name_len;
  }
  item = (general_factory_item_t*)TKMEM_ALLOC(len);
  return_value_if_fail(item != NULL, NULL);

  item->create = create;
  item->const_name = is_const_name;
  if (is_const_name) {
    item->name = name;
  } else {
    tk_strncpy(item->data, name, name_len);
    item->name = item->data;
  }

  return item;
}

static inline ret_t general_factory_item_destroy(general_factory_item_t* item) {
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(item);
  return RET_OK;
}

static inline int general_factory_item_compare_by_name(const void* a, const void* b) {
  const general_factory_item_t* aa = (const general_factory_item_t*)a;
  const char* name = (const char*)b;

  return strcmp(aa->name, name);
}

static inline int general_factory_item_compare(const void* a, const void* b) {
  const general_factory_item_t* aa = (const general_factory_item_t*)a;
  const general_factory_item_t* bb = (const general_factory_item_t*)b;

  return strcmp(aa->name, bb->name);
}

typedef struct _general_factory_t {
  darray_t sorted;
  darray_t creators;
} general_factory_t;

static inline general_factory_t* general_factory_init(general_factory_t* factory) {
  memset(factory, 0x00, sizeof(general_factory_t));

  darray_init(&(factory->sorted), 5, NULL, general_factory_item_compare);
  darray_init(&(factory->creators), 5, (tk_destroy_t)general_factory_item_destroy,
              general_factory_item_compare_by_name);

  return factory;
}

static inline ret_t general_factory_unregister(general_factory_t* factory, const char* name) {
  return_value_if_fail(factory != NULL && name != NULL, RET_BAD_PARAMS);

  darray_remove_all(&(factory->sorted), general_factory_item_compare_by_name, (void*)name);

  return darray_remove_all(&(factory->creators), general_factory_item_compare_by_name, (void*)name);
}

static inline ret_t general_factory_register_ex(general_factory_t* factory, const char* name,
                                                tk_create_t create, bool_t is_const_name) {
  general_factory_item_t* item = NULL;
  return_value_if_fail(factory != NULL && name != NULL && create != NULL, RET_BAD_PARAMS);
  item = general_factory_item_create(name, create, is_const_name);
  return_value_if_fail(item != NULL, RET_OOM);

  general_factory_unregister(factory, name);
  if (darray_push(&(factory->creators), item) != RET_OK) {
    general_factory_item_destroy(item);
    return RET_OOM;
  }

  return darray_sorted_insert(&(factory->sorted), item, general_factory_item_compare, TRUE);
}

static inline ret_t general_factory_register_table(general_factory_t* factory,
                                                   const general_factory_const_item_t* table) {
  const general_factory_const_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && table != NULL, RET_BAD_PARAMS);

  iter = (const general_factory_const_item_t*)(table);
  while (iter->name != NULL) {
    ENSURE(darray_sorted_insert(&(factory->sorted), (void*)iter, general_factory_item_compare,
                                TRUE) == RET_OK);
    iter++;
  }

  return RET_OK;
}

static inline ret_t general_factory_register(general_factory_t* factory, const char* name,
                                             tk_create_t create) {
  return general_factory_register_ex(factory, name, create, TRUE);
}

static inline tk_create_t general_factory_find(general_factory_t* factory, const char* name) {
  const general_factory_item_t* item = NULL;
  return_value_if_fail(factory != NULL && name != NULL, NULL);

  item = (const general_factory_item_t*)darray_bsearch(
      &(factory->sorted), general_factory_item_compare_by_name, (void*)name);

  return item != NULL ? item->create : NULL;
}

static inline ret_t general_factory_deinit(general_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);
  darray_deinit(&(factory->sorted));
  darray_deinit(&(factory->creators));
  memset(factory, 0x00, sizeof(general_factory_t));

  return RET_OK;
}

static inline general_factory_t* general_factory_create(void) {
  general_factory_t* factroy = TKMEM_ZALLOC(general_factory_t);

  return general_factory_init(factroy);
}

static inline ret_t general_factory_destroy(general_factory_t* factory) {
  general_factory_deinit(factory);
  TKMEM_FREE(factory);
  return RET_OK;
}

END_C_DECLS

#endif /*TK_GENERAL_FACTORY_H*/

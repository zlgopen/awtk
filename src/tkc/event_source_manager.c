/**
 * File:   event_source_manager.c
 * Author: AWTK Develop Team
 * Brief:  event manager manager
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/event_source_manager.h"

ret_t event_source_manager_init(event_source_manager_t* manager) {
  return_value_if_fail(manager != NULL, RET_BAD_PARAMS);

  darray_init(&(manager->sources), 5, (tk_destroy_t)(object_unref), NULL);
  darray_init(&(manager->dispatching_sources), 5, (tk_destroy_t)(object_unref), NULL);

  return RET_OK;
}

ret_t event_source_manager_deinit(event_source_manager_t* manager) {
  return_value_if_fail(manager != NULL, RET_BAD_PARAMS);

  darray_deinit(&(manager->sources));
  darray_deinit(&(manager->dispatching_sources));

  return RET_OK;
}

static ret_t event_source_manager_prepare(event_source_manager_t* manager) {
  uint32_t i = 0;
  uint32_t n = 0;
  event_source_t** sources = NULL;
  return_value_if_fail(manager != NULL && manager->dispatch != NULL, RET_BAD_PARAMS);

  n = manager->sources.size;
  sources = (event_source_t**)(manager->sources.elms);

  for (i = 0; i < n; i++) {
    event_source_t* iter = sources[i];
    object_ref(OBJECT(iter));
    darray_push(&(manager->dispatching_sources), iter);
  }

  return RET_OK;
}

ret_t event_source_manager_dispatch(event_source_manager_t* manager) {
  return_value_if_fail(event_source_manager_prepare(manager) == RET_OK, RET_BAD_PARAMS);

  manager->dispatch(manager);
  darray_clear(&(manager->dispatching_sources));

  return RET_OK;
}

ret_t event_source_manager_add(event_source_manager_t* manager, event_source_t* source) {
  return_value_if_fail(manager != NULL && source != NULL, RET_BAD_PARAMS);
  object_ref(OBJECT(source));
  source->manager = manager;

  return darray_push(&(manager->sources), source);
}

bool_t event_source_manager_exist(event_source_manager_t* manager, event_source_t* source) {
  return_value_if_fail(manager != NULL && source != NULL, RET_BAD_PARAMS);

  return darray_find(&(manager->sources), source) != NULL;
}

ret_t event_source_manager_remove(event_source_manager_t* manager, event_source_t* source) {
  return_value_if_fail(manager != NULL && source != NULL, RET_BAD_PARAMS);

  source->manager = NULL;
  return darray_remove(&(manager->sources), source);
}

static int event_source_compare_by_tag(const void* a, const void* b) {
  const event_source_t* source = (const event_source_t*)a;

  return (source->tag == b) ? 0 : 1;
}

ret_t event_source_manager_remove_by_tag(event_source_manager_t* manager, void* tag) {
  return darray_remove_all(&(manager->sources), event_source_compare_by_tag, tag);
}

ret_t event_source_manager_destroy(event_source_manager_t* manager) {
  return_value_if_fail(manager != NULL && manager->destroy != NULL, RET_BAD_PARAMS);

  event_source_manager_deinit(manager);

  return manager->destroy(manager);
}

uint32_t event_source_manager_get_wakeup_time(event_source_manager_t* manager) {
  uint32_t i = 0;
  uint32_t t = 0;
  uint32_t n = 0;
  event_source_t* iter = NULL;
  uint32_t wakeup_time = 0xffff;
  event_source_t** sources = NULL;
  return_value_if_fail(manager != NULL, 0);

  n = manager->sources.size;
  sources = (event_source_t**)(manager->sources.elms);

  if (n > 0) {
    for (i = 0; i < n; i++) {
      iter = sources[i];
      t = event_source_get_wakeup_time(iter);
      if (t < wakeup_time) {
        wakeup_time = t;
      }
    }
  } else {
    wakeup_time = 16;
  }

  return tk_min(16, wakeup_time);
}

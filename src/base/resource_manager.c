/**
 * File:   resource_manager.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  resource manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/array.h"
#include "base/resource_manager.h"

static array_t s_resources;

ret_t resource_manager_init(uint32_t init_res_nr) {
  return array_init(&(s_resources), init_res_nr) ? RET_OK : RET_FAIL;
}

const resource_info_t* resource_manager_ref(resource_type_t type, const char* name) {
  uint32_t i = 0;
  const resource_info_t* iter = NULL;
  const resource_info_t** all = (const resource_info_t**)s_resources.elms;
  return_value_if_fail(name != NULL, NULL);

  for (i = 0; i < s_resources.size; i++) {
    iter = all[i];
    if (type == iter->type && strcmp(name, iter->name) == 0) {
      return iter;
    }
  }

  return NULL;
}

ret_t resource_manager_unref(const resource_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t resource_manager_add(const void* data) {
  const resource_info_t* info = (const resource_info_t*)data;
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  return array_push(&s_resources, (void*)info) ? RET_OK : RET_FAIL;
}

ret_t resource_manager_deinit() {
  array_deinit(&(s_resources));
  return RET_OK;
}

const resource_info_t** resource_manager_get_all(uint32_t* size) {
  if (size != NULL) {
    *size = s_resources.size;
  }

  return (const resource_info_t**)s_resources.elms;
}

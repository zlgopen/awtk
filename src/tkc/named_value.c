/**
 * File:   named_value.c
 * Author: AWTK Develop Team
 * Brief:  named value
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
 * 2019-01-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/named_value.h"

named_value_t* named_value_cast(named_value_t* nv) {
  return_value_if_fail(nv != NULL, NULL);

  return nv;
}

named_value_t* named_value_init(named_value_t* nv, const char* name, const value_t* value) {
  return_value_if_fail(nv != NULL, NULL);
  memset(nv, 0x00, sizeof(named_value_t));

  if (name != NULL) {
    named_value_set_name(nv, name);
  }

  if (value != NULL) {
    named_value_set_value(nv, value);
  }

  return nv;
}

named_value_t* named_value_create(void) {
  named_value_t* nv = TKMEM_ZALLOC(named_value_t);
  return_value_if_fail(nv != NULL, NULL);

  return named_value_init(nv, NULL, NULL);
}

ret_t named_value_set_name(named_value_t* nv, const char* name) {
  return_value_if_fail(nv != NULL, RET_BAD_PARAMS);

  nv->name = tk_str_copy(nv->name, name);

  return RET_OK;
}

ret_t named_value_set_value(named_value_t* nv, const value_t* value) {
  return_value_if_fail(nv != NULL && value != NULL, RET_BAD_PARAMS);

  if (value_equal(&(nv->value), value)) {
    return RET_OK;
  }

  value_reset(&(nv->value));
  return value_deep_copy(&(nv->value), value);
}

ret_t named_value_deinit(named_value_t* nv) {
  return_value_if_fail(nv != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(nv->name);
  value_reset(&(nv->value));

  return RET_OK;
}

ret_t named_value_destroy(named_value_t* nv) {
  return_value_if_fail(nv != NULL, RET_BAD_PARAMS);

  named_value_deinit(nv);
  TKMEM_FREE(nv);

  return RET_OK;
}

value_t* named_value_get_value(named_value_t* nv) {
  return_value_if_fail(nv != NULL, NULL);

  return &(nv->value);
}

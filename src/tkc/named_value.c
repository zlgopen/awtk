/**
 * File:   named_value.c
 * Author: AWTK Develop Team
 * Brief:  named value
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#define TK_EXTRA_DATA(obj) (((char*)(obj)) + sizeof(*obj))

named_value_t* named_value_create_ex(const char* name, const value_t* value) {
  char* p = NULL;
  named_value_t* nv = NULL;
  uint32_t slen = tk_strlen(name) + 1;
  return_value_if_fail(name != NULL && value != NULL, NULL);

  nv = (named_value_t*)TKMEM_ALLOC(sizeof(named_value_t) + slen);
  return_value_if_fail(nv != NULL, NULL);
  memset(nv, 0x00, sizeof(named_value_t));

  p = TK_EXTRA_DATA(nv);
  memcpy(p, name, slen);
  nv->name = p;
  named_value_set_value(nv, value);

  return nv;
}

ret_t named_value_set_name(named_value_t* nv, const char* name) {
  const char* p = NULL;
  return_value_if_fail(nv != NULL, RET_BAD_PARAMS);

  p = TK_EXTRA_DATA(nv);
  if (p != nv->name) {
    nv->name = tk_str_copy(nv->name, name);
  } else {
    nv->name = tk_strdup(name);
  }

  return RET_OK;
}

ret_t named_value_set_value(named_value_t* nv, const value_t* value) {
  return_value_if_fail(nv != NULL && value != NULL, RET_BAD_PARAMS);

  if (value_equal(&(nv->value), value)) {
    return RET_OK;
  }

  if (!value_equal(value, &(nv->value))) {
    value_reset(&(nv->value));
    return value_deep_copy(&(nv->value), value);
  }

  return RET_OK;
}

ret_t named_value_deinit(named_value_t* nv) {
  char* p = NULL;
  return_value_if_fail(nv != NULL, RET_BAD_PARAMS);
  p = TK_EXTRA_DATA(nv);

  if (p != nv->name) {
    TKMEM_FREE(nv->name);
  }
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

int32_t named_value_compare_by_name(named_value_t* nv, const char* name) {
  return_value_if_fail(nv != NULL && name != NULL, -1);

  return tk_str_cmp(nv->name, name);
}

int32_t named_value_compare(named_value_t* nv, const named_value_t* other) {
  return_value_if_fail(nv != NULL && other != NULL, -1);

  return tk_str_cmp(nv->name, other->name);
}

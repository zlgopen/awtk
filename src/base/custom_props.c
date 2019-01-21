/**
 * File:   custom_props.c
 * Author: AWTK Develop Team
 * Brief:  custom_props
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/wstr.h"
#include "base/custom_props.h"

static ret_t value_free(value_t* v);

custom_props_t* custom_props_create(uint32_t capacity) {
  custom_props_t* props = (custom_props_t*)TKMEM_ZALLOC(custom_props_t);
  return_value_if_fail(props != NULL, NULL);

  if (capacity > 0) {
    props->props = TKMEM_ZALLOCN(custom_prop_t, capacity);
    if (props->props != NULL) {
      props->capacity = capacity;
    }
  }

  return props;
}

ret_t custom_props_get(custom_props_t* props, const char* name, value_t* v) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(props != NULL && props->props != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  for (i = 0, n = props->size; i < n; i++) {
    custom_prop_t* iter = props->props + i;
    if (tk_str_eq(iter->name, name)) {
      *v = iter->value;

      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

static ret_t value_free(value_t* v) {
  if (v->type == VALUE_TYPE_WSTRING) {
    TKMEM_FREE(v->value.wstr);
    v->value.wstr = NULL;
  } else if (v->type == VALUE_TYPE_STRING) {
    TKMEM_FREE(v->value.str);
    v->value.str = NULL;
  }

  return RET_OK;
}

static ret_t value_assign(value_t* v, const value_t* other) {
  *v = *other;
  if (v->type == VALUE_TYPE_WSTRING) {
    if (v->value.wstr != NULL) {
      v->value.wstr = wcs_dup(v->value.wstr);
      return_value_if_fail(v->value.wstr != NULL, RET_OOM);
    }
  } else if (v->type == VALUE_TYPE_STRING) {
    if (v->value.str != NULL) {
      v->value.str = tk_strdup(v->value.str);
      return_value_if_fail(v->value.str != NULL, RET_OOM);
    }
  }

  return RET_OK;
}

static ret_t custom_props_extend(custom_props_t* props, int32_t size) {
  custom_prop_t* new_props = NULL;
  int32_t capacity = props->size + size;

  if (capacity < props->capacity) {
    return RET_OK;
  }

  new_props = TKMEM_REALLOCT(custom_prop_t, props->props, capacity);
  if (new_props != NULL) {
    props->props = new_props;
    props->capacity = capacity;

    return RET_OK;
  }

  return RET_OOM;
}

ret_t custom_props_set(custom_props_t* props, const char* name, const value_t* v) {
  uint32_t i = 0;
  uint32_t n = 0;
  custom_prop_t* iter = NULL;
  return_value_if_fail(props != NULL && props->props != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  for (i = 0, n = props->size; i < n; i++) {
    iter = props->props + i;
    if (tk_str_eq(iter->name, name)) {
      value_free(&(iter->value));
      return value_assign(&(iter->value), v);
    }
  }

  return_value_if_fail(custom_props_extend(props, 1) == RET_OK, RET_OOM);

  iter = props->props + props->size++;
  tk_strncpy(iter->name, name, TK_NAME_LEN);

  return value_assign(&(iter->value), v);
}

ret_t custom_props_destroy(custom_props_t* props) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(props != NULL, RET_BAD_PARAMS);

  if (props->props != NULL) {
    for (i = 0, n = props->size; i < n; i++) {
      custom_prop_t* iter = props->props + i;
      value_free(&(iter->value));
    }
    TKMEM_FREE(props->props);
  }

  TKMEM_FREE(props);

  return RET_OK;
}

/**
 * File:   self_layouter.c
 * Author: AWTK Develop Team
 * Brief:  self layouter
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/self_layouter.h"

const char* self_layouter_to_string(self_layouter_t* layouter) {
  if (layouter == NULL) {
    return NULL;
  }

  return_value_if_fail(layouter->vt != NULL && layouter->vt->to_string != NULL, NULL);

  return layouter->vt->to_string(layouter);
}

ret_t self_layouter_layout(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(widget != NULL && area != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->layout != NULL, RET_FAIL);

  return layouter->vt->layout(layouter, widget, area);
}

ret_t self_layouter_get_param(self_layouter_t* layouter, const char* name, value_t* v) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(name != NULL && v != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->get_param != NULL, RET_FAIL);

  return layouter->vt->get_param(layouter, name, v);
}

ret_t self_layouter_set_param(self_layouter_t* layouter, const char* name, const value_t* v) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(name != NULL && v != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->set_param != NULL, RET_FAIL);

  return layouter->vt->set_param(layouter, name, v);
}

ret_t self_layouter_set_param_str(self_layouter_t* layouter, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return self_layouter_set_param(layouter, name, &v);
}

float_t self_layouter_get_param_float(self_layouter_t* layouter, const char* name, float_t defval) {
  value_t v;
  if (self_layouter_get_param(layouter, name, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

int32_t self_layouter_get_param_int(self_layouter_t* layouter, const char* name, int32_t defval) {
  value_t v;
  if (self_layouter_get_param(layouter, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

ret_t self_layouter_destroy(self_layouter_t* layouter) {
  if (layouter == NULL) {
    return RET_OK;
  }

  return_value_if_fail(layouter->vt != NULL && layouter->vt->destroy != NULL, RET_OK);

  return layouter->vt->destroy(layouter);
}

self_layouter_t* self_layouter_clone(self_layouter_t* layouter) {
  if (layouter != NULL && layouter->vt != NULL && layouter->vt->clone != NULL) {
    return layouter->vt->clone(layouter);
  }

  return NULL;
}

#ifdef WITHOUT_LAYOUTER
self_layouter_t* self_layouter_create(const char* params) {
  return NULL;
}
#endif /*WITHOUT_LAYOUTER*/

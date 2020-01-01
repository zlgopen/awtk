/**
 * File:   children_layouter.c
 * Author: AWTK Develop Team
 * Brief:  children layouter
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

#include "base/children_layouter.h"

const char* children_layouter_to_string(children_layouter_t* layouter) {
  if (layouter == NULL) {
    return NULL;
  }

  return_value_if_fail(layouter->vt != NULL && layouter->vt->to_string != NULL, NULL);

  return layouter->vt->to_string(layouter);
}

ret_t children_layouter_layout(children_layouter_t* layouter, widget_t* widget) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(widget != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->layout != NULL, RET_FAIL);

  return layouter->vt->layout(layouter, widget);
}

ret_t children_layouter_get_param(children_layouter_t* layouter, const char* name, value_t* v) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(name != NULL && v != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->get_param != NULL, RET_FAIL);

  return layouter->vt->get_param(layouter, name, v);
}

ret_t children_layouter_set_param(children_layouter_t* layouter, const char* name,
                                  const value_t* v) {
  if (layouter == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(name != NULL && v != NULL, RET_FAIL);
  return_value_if_fail(layouter->vt != NULL && layouter->vt->set_param != NULL, RET_FAIL);

  return layouter->vt->set_param(layouter, name, v);
}

ret_t children_layouter_set_param_str(children_layouter_t* layouter, const char* name,
                                      const char* value) {
  value_t v;
  value_set_str(&v, value);

  return children_layouter_set_param(layouter, name, &v);
}

float_t children_layouter_get_param_float(children_layouter_t* layouter, const char* name,
                                          float_t defval) {
  value_t v;
  if (children_layouter_get_param(layouter, name, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

int32_t children_layouter_get_param_int(children_layouter_t* layouter, const char* name,
                                        int32_t defval) {
  value_t v;
  if (children_layouter_get_param(layouter, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

bool_t children_layouter_is_valid(children_layouter_t* layouter) {
  if (layouter == NULL) {
    return FALSE;
  }

  return_value_if_fail(layouter->vt != NULL && layouter->vt->is_valid != NULL, FALSE);

  return layouter->vt->is_valid(layouter);
}

ret_t children_layouter_destroy(children_layouter_t* layouter) {
  if (layouter == NULL) {
    return RET_OK;
  }

  return_value_if_fail(layouter->vt != NULL && layouter->vt->destroy != NULL, RET_OK);

  return layouter->vt->destroy(layouter);
}

children_layouter_t* children_layouter_clone(children_layouter_t* layouter) {
  if (layouter != NULL && layouter->vt != NULL && layouter->vt->clone != NULL) {
    return layouter->vt->clone(layouter);
  }

  return NULL;
}

#ifdef WITHOUT_LAYOUTER
children_layouter_t* children_layouter_create(const char* params) {
  return NULL;
}
#endif /*WITHOUT_LAYOUTER*/

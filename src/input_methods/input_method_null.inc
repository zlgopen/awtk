/**
 * File:   input_method_null.c
 * Author: AWTK Develop Team
 * Brief:  input method null
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
 * 2018-06-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/input_method.h"

static ret_t input_method_null_request(input_method_t* im, widget_t* widget) {
  (void)im;
  (void)widget;

  return RET_OK;
}

static ret_t input_method_null_destroy(input_method_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  emitter_deinit(&(im->emitter));
  TKMEM_FREE(im);

  return RET_OK;
}

input_method_t* input_method_null_create(void) {
  input_method_t* im = TKMEM_ZALLOC(input_method_t);
  return_value_if_fail(im != NULL, NULL);

  im->request = input_method_null_request;
  im->destroy = input_method_null_destroy;

  emitter_init(&(im->emitter));

  return im;
}

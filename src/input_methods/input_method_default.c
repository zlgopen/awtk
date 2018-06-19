/**
 * File:   input_method_default.c
 * Author: AWTK Develop Team
 * Brief:  input method default
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/mem.h"
#include "base/window.h"
#include "base/keyboard.h"
#include "base/input_method.h"
#include "ui_loader/ui_builder_default.h"

static ret_t input_method_default_show_keyboard(input_method_t* im) {
  value_t v;
  int32_t input_type = 0;
  const char* keyboard = NULL;

  value_set_int(&v, 0);
  widget_get_prop(im->widget, WIDGET_PROP_INPUT_TYPE, &v);

  input_type = value_int(&v);
  switch (input_type) {
    case INPUT_PHONE: {
      keyboard = "kb_phone";
      break;
    }
    case INPUT_INT: {
      keyboard = "kb_int";
      break;
    }
    case INPUT_FLOAT: {
      keyboard = "kb_float";
      break;
    }
    case INPUT_UINT: {
      keyboard = "kb_uint";
      break;
    }
    case INPUT_UFLOAT: {
      keyboard = "kb_ufloat";
      break;
    }
    case INPUT_HEX: {
      keyboard = "kb_hex";
      break;
    }
    case INPUT_EMAIL:
    case INPUT_PASSWORD: {
      keyboard = "kb_ascii";
      break;
    }
    default: {
      keyboard = "kb_default";
      break;
    }
  }

  im->keyboard = window_open(keyboard);

  return RET_OK;
}

static ret_t input_method_default_request(input_method_t* im, widget_t* widget) {
  im->widget = widget;
  if (widget != NULL) {
    input_method_default_show_keyboard(im);
  } else {
    if (im->keyboard != NULL) {
      keyboard_close(im->keyboard);
      im->keyboard = NULL;
    }
  }

  return RET_OK;
}

input_method_t* input_method_create(void) {
  input_method_t* im = TKMEM_ZALLOC(input_method_t);
  return_value_if_fail(im != NULL, NULL);

  im->request = input_method_default_request;
  emitter_init(&(im->emitter));

  return im;
}

ret_t input_method_destroy(input_method_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  emitter_deinit(&(im->emitter));
  TKMEM_FREE(im);

  return RET_OK;
}

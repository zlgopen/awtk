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
#include "base/idle.h"
#include "base/window.h"
#include "base/keyboard.h"
#include "base/input_method.h"
#include "ui_loader/ui_builder_default.h"

static ret_t input_method_default_show_keyboard(input_method_t* im) {
  value_t v;
  int32_t input_type = 0;
  const char* keyboard = NULL;
  const char* open_anim_hint = "";
  const char* close_anim_hint = "";

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

  if (im->keyboard != NULL) {
    if (im->input_type == input_type) {
      return RET_OK;
    } else {
      value_set_str(&v, "");
      widget_set_prop(im->keyboard, WIDGET_PROP_OPEN_ANIM_HINT, &v);
      widget_set_prop(im->keyboard, WIDGET_PROP_CLOSE_ANIM_HINT, &v);

      keyboard_close(im->keyboard);
      im->keyboard = NULL;

      close_anim_hint = "bottom_to_top";
    }
  } else {
    open_anim_hint = "bottom_to_top";
    close_anim_hint = "bottom_to_top";
  }

  im->input_type = input_type;
  im->keyboard = keyboard_open(keyboard);

  value_set_str(&v, open_anim_hint);
  widget_set_prop(im->keyboard, WIDGET_PROP_OPEN_ANIM_HINT, &v);

  value_set_str(&v, close_anim_hint);
  widget_set_prop(im->keyboard, WIDGET_PROP_CLOSE_ANIM_HINT, &v);

  return RET_OK;
}

typedef struct _idle_close_info_t {
  widget_t* widget;
  widget_t* keyboard;
  input_method_t* im;
} idle_close_info_t;

static ret_t on_idle_close_keyboard(const idle_info_t* idle) {
  idle_close_info_t* info = (idle_close_info_t*)(idle->ctx);
  input_method_t* im = info->im;

  if (im->keyboard == info->keyboard && im->widget == info->widget) {
    keyboard_close(im->keyboard);
    im->keyboard = NULL;
    im->widget = NULL;
  }

  return RET_OK;
}

static ret_t input_method_default_request(input_method_t* im, widget_t* widget) {
  if (im->widget == widget) {
    return RET_OK;
  }

  if (widget != NULL) {
    im->widget = widget;
    input_method_default_show_keyboard(im);
  } else {
    if (im->keyboard != NULL) {
      idle_close_info_t* info = TKMEM_ZALLOC(idle_close_info_t);

      if (info != NULL) {
        info->im = im;
        info->widget = im->widget;
        info->keyboard = im->keyboard;

        idle_add(on_idle_close_keyboard, info);
      } else {
        keyboard_close(im->keyboard);
        im->keyboard = NULL;
        im->widget = NULL;
      }
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

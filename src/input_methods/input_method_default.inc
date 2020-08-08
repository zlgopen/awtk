/**
 * File:   input_method_default.c
 * Author: AWTK Develop Team
 * Brief:  input method default
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
#include "tkc/utils.h"
#include "base/idle.h"
#include "base/enums.h"
#include "base/window.h"
#include "keyboard/keyboard.h"

#include "base/input_method.h"
#include "base/window_manager.h"
#include "ui_loader/ui_builder_default.h"

static ret_t input_method_default_on_edit_destroy(void* ctx, event_t* e);
static ret_t input_method_default_on_keyboard_destroy(void* ctx, event_t* e);

static const char* input_type_to_keyboard_name(int32_t input_type) {
  const char* keyboard = NULL;

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
    case INPUT_IPV4:
    case INPUT_TIME:
    case INPUT_DATE:
    case INPUT_TIME_FULL: {
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
    case INPUT_CUSTOM:
    case INPUT_CUSTOM_PASSWORD: {
      log_debug("custom keyboard\n");
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

  return keyboard;
}

static int32_t widget_get_input_type(widget_t* widget) {
  value_t v;
  int32_t input_type = 0;

  value_set_int(&v, INPUT_TEXT);
  widget_get_prop(widget, WIDGET_PROP_INPUT_TYPE, &v);

  if (v.type == VALUE_TYPE_STRING) {
    const key_type_value_t* kv = input_type_find(value_str(&v));
    if (kv != NULL) {
      input_type = kv->value;
    }
  } else {
    input_type = value_int(&v);
  }

  return input_type;
}

static const char* widget_get_keyboard(widget_t* widget) {
  int32_t input_type = widget_get_input_type(widget);
  const char* keyboard = widget_get_prop_str(widget, WIDGET_PROP_KEYBOARD, NULL);

  if (keyboard == NULL) {
    keyboard = input_type_to_keyboard_name(input_type);
  }

  return keyboard;
}

static ret_t on_push_window(void* ctx, event_t* e) {
  input_method_t* im = (input_method_t*)ctx;

  im->win_old_y = im->win->y;
  im->win->y -= im->win_delta_y;
  widget_invalidate_force(im->win, NULL);

  return RET_REMOVE;
}

static ret_t input_method_default_restore_win_position(input_method_t* im) {
  if (im->win != NULL) {
    im->win->y = im->win_old_y;
    widget_invalidate_force(im->win, NULL);
    im->win = NULL;
  }

  return RET_OK;
}

static ret_t input_method_on_win_close(void* ctx, event_t* e) {
  value_t v;
  input_method_t* im = (input_method_t*)ctx;
  input_method_default_restore_win_position(im);

  if (im->keyboard != NULL) {
    widget_get_prop(im->keyboard, WIDGET_PROP_CLOSE_ANIM_HINT, &v);
    if (im->idle_close_id == TK_INVALID_ID) {
      window_close_force(im->keyboard);
      
      im->keyboard = NULL;
      im->widget = NULL;
      im->win = NULL;
    }
  }

  return RET_REMOVE;
}

static ret_t input_method_default_on_keyboard_open(void* ctx, event_t* e) {
  input_method_t* im = (input_method_t*)ctx;
  im->busy = FALSE;

  return RET_REMOVE;
}

static ret_t input_type_open_keyboard(input_method_t* im, const char* keyboard_name,
                                      bool_t open_anim) {
  value_t v;
  point_t p = {0, 0};
  widget_t* widget = im->widget;
  widget_t* win = widget_get_window(widget);
  const char* close_anim_hint = WINDOW_ANIMATOR_POPUP;
  const char* open_anim_hint = open_anim ? close_anim_hint : "";

  if (keyboard_name == NULL) {
    return RET_OK;
  }

  widget_to_screen(widget, &p);
  im->keyboard = window_open(keyboard_name);
  if (im->keyboard == NULL) {
    log_debug("Not found keyboard [%s], try default.\n", keyboard_name);
    im->keyboard = window_open("kb_default");
  }
  return_value_if_fail(im->keyboard != NULL, RET_NOT_FOUND);

  im->busy = TRUE;
  widget_on(im->keyboard, EVT_WINDOW_OPEN, input_method_default_on_keyboard_open, im);
  widget_on(im->keyboard, EVT_DESTROY, input_method_default_on_keyboard_destroy, im);

  if ((p.y + widget->h) > im->keyboard->y) {
    close_anim_hint = "vtranslate";
    open_anim_hint = close_anim_hint;

    im->win = win;
    im->win_delta_y = win->y + win->h - im->keyboard->y;
    widget_on(im->keyboard, EVT_WINDOW_OPEN, on_push_window, im);
  } else if (win->y + win->h < im->keyboard->y) {
    /*when new keyboard height is not eq old keyboard height*/
    close_anim_hint = "vtranslate";
    im->win_delta_y += win->y + win->h - im->keyboard->y;
    win->y = im->keyboard->y - win->h;
    widget_invalidate_force(im->win, NULL);
  } else if (p.y < 0) {
    im->win = win;
    im->win->y += im->win_delta_y;
    im->win_delta_y = 0;
    widget_invalidate_force(im->win, NULL);
  }

  value_set_str(&v, open_anim_hint);
  widget_set_prop(im->keyboard, WIDGET_PROP_OPEN_ANIM_HINT, &v);
  value_set_str(&v, close_anim_hint);
  widget_set_prop(im->keyboard, WIDGET_PROP_CLOSE_ANIM_HINT, &v);

  widget_off_by_func(win, EVT_WINDOW_CLOSE, input_method_on_win_close, im);
  widget_on(win, EVT_WINDOW_CLOSE, input_method_on_win_close, im);

  return RET_OK;
}

static ret_t input_method_default_show_keyboard(input_method_t* im) {
  value_t v;
  point_t p = {0, 0};
  bool_t open_anim = TRUE;
  widget_t* widget = im->widget;
  const char* keyboard_name = widget_get_keyboard(widget);

  widget_to_screen(widget, &p);
  if (im->keyboard != NULL) {
    bool_t edit_is_in_view = (p.y >= 0) && ((p.y + widget->h) <= im->keyboard->y);

    if (tk_str_eq(im->keyboard_name, keyboard_name) && edit_is_in_view) {
      /*old edit and new edit has same input type, reuse old keyboard*/

      return RET_OK;
    } else {
      /*keyboard is open, close old one, disable open animation of new one*/
      value_set_str(&v, "");
      widget_set_prop(im->keyboard, WIDGET_PROP_OPEN_ANIM_HINT, &v);
      widget_set_prop(im->keyboard, WIDGET_PROP_CLOSE_ANIM_HINT, &v);
      window_close(im->keyboard);
      im->keyboard = NULL;

      open_anim = FALSE;
    }
  }

  tk_strncpy(im->keyboard_name, keyboard_name, TK_NAME_LEN);

  return input_type_open_keyboard(im, keyboard_name, open_anim);
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
    input_method_default_restore_win_position(im);
    window_close(im->keyboard);
    im->keyboard = NULL;
    im->widget = NULL;
  } else if (im->widget == NULL) {
    window_close_force(info->keyboard);
    im->keyboard = NULL;
    im->widget = NULL;
  }
  im->idle_close_id = TK_INVALID_ID;

  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t input_method_default_ensure_suggest_words(input_method_t* im) {
  if (im->data != NULL) {
    return RET_OK;
  }

#if !defined(WITHOUT_SUGGEST_WORDS) && !defined(WITH_IME_NULL)
  if (im->data == NULL) {
    const asset_info_t* suggest_words =
        assets_manager_ref(assets_manager(), ASSET_TYPE_DATA, "suggest_words_zh_cn.dat");
    if (suggest_words != NULL) {
      im->suggest_words = suggest_words_create((const asset_info_t*)(suggest_words));
      im->data = (void*)suggest_words;
    }
  }
#endif /*WITHOUT_SUGGEST_WORDS*/

  return RET_OK;
}

static ret_t input_method_default_on_keyboard_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  input_method_t* im = (input_method_t*)ctx;

  if (im->keyboard == widget) {
    input_method_default_restore_win_position(im);
    im->win = NULL;
    im->widget = NULL;
    im->keyboard = NULL;
  }

  return RET_REMOVE;
}

static ret_t input_method_default_on_edit_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  input_method_t* im = (input_method_t*)ctx;

  if (im->widget == widget) {
    input_method_request(im, NULL);
  }

  return RET_REMOVE;
}

static ret_t input_method_default_request(input_method_t* im, widget_t* widget) {
  if (im->widget == widget) {
    return RET_OK;
  }

  input_engine_reset_input(im->engine);
  input_method_dispatch_candidates(im, "", 0, 0);
  input_method_default_ensure_suggest_words(im);
  if (im->idle_close_id != TK_INVALID_ID) {
    idle_info_t* info = (idle_info_t*)idle_find(im->idle_close_id);

    if (info != NULL) {
      TKMEM_FREE(info->ctx);
      info->ctx = NULL;
    }
    idle_remove(im->idle_close_id);
    im->idle_close_id = TK_INVALID_ID;
  }

  if (widget != NULL) {
    im->widget = widget;
    input_method_default_show_keyboard(im);
    widget_off_by_func(widget, EVT_DESTROY, input_method_default_on_edit_destroy, im);
    widget_on(widget, EVT_DESTROY, input_method_default_on_edit_destroy, im);
  } else {
    if (im->keyboard != NULL) {
      idle_close_info_t* info = TKMEM_ZALLOC(idle_close_info_t);
      /*do not close immediately, so we have chance to reuse it.*/
      if (info != NULL) {
        info->im = im;
        info->widget = im->widget;
        info->keyboard = im->keyboard;

        im->idle_close_id = idle_add(on_idle_close_keyboard, info);
      } else {
        window_close(im->keyboard);
        im->keyboard = NULL;
        im->widget = NULL;
      }
    }
  }

  return RET_OK;
}

ret_t input_method_default_destroy(input_method_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  emitter_deinit(&(im->emitter));
  input_engine_destroy(im->engine);
  suggest_words_destroy(im->suggest_words);
  assets_manager_unref(assets_manager(), (const asset_info_t*)(im->data));

  TKMEM_FREE(im);

  return RET_OK;
}

input_method_t* input_method_default_create(void) {
  input_method_t* im = TKMEM_ZALLOC(input_method_t);
  return_value_if_fail(im != NULL, NULL);

  im->action_button_enable = TRUE;
  im->request = input_method_default_request;
  im->destroy = input_method_default_destroy;

  emitter_init(&(im->emitter));
  im->engine = input_engine_create(im);

  return im;
}

/**
 * File:   input_method.h
 * Author: AWTK Develop Team
 * Brief:  input method interface.
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

#include "base/keys.h"
#include "tkc/utils.h"
#include "base/input_method.h"

ret_t input_method_dispatch(input_method_t* im, event_t* e) {
  return_value_if_fail(im != NULL && e != NULL, RET_BAD_PARAMS);

  return emitter_dispatch(&(im->emitter), e);
}

ret_t input_method_dispatch_preedit(input_method_t* im) {
  event_t e = event_init(EVT_IM_PREEDIT, im);

  return input_method_dispatch_to_widget(im, &e);
}

ret_t input_method_dispatch_preedit_confirm(input_method_t* im) {
  event_t e = event_init(EVT_IM_PREEDIT_CONFIRM, im);

  return input_method_dispatch_to_widget(im, &e);
}

ret_t input_method_dispatch_preedit_abort(input_method_t* im) {
  event_t e = event_init(EVT_IM_PREEDIT_ABORT, im);

  return input_method_dispatch_to_widget(im, &e);
}

ret_t input_method_dispatch_to_widget(input_method_t* im, event_t* e) {
  return_value_if_fail(im != NULL && im->widget != NULL && e != NULL, RET_BAD_PARAMS);

  e->target = im->widget;
  return widget_dispatch(im->widget, e);
}

uint32_t input_method_on(input_method_t* im, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(im != NULL && on_event != NULL, 0);

  return emitter_on(&(im->emitter), type, on_event, ctx);
}

ret_t input_method_off(input_method_t* im, uint32_t id) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  return emitter_off(&(im->emitter), id);
}

ret_t input_method_request(input_method_t* im, widget_t* widget) {
  return_value_if_fail(im != NULL && im->request != NULL, RET_BAD_PARAMS);
  return_value_if_fail(!(im->busy), RET_FAIL);

  if (im->widget == widget) {
    return RET_OK;
  }

  return im->request(im, widget);
}

static input_method_t* s_input_method = NULL;
input_method_t* input_method(void) {
  return s_input_method;
}

ret_t input_method_set(input_method_t* im) {
  s_input_method = im;

  return RET_OK;
}

ret_t input_method_update_action_button_info(input_method_t* im, const char* text, bool_t enable) {
  im_action_button_info_event_t e;
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  e.text = text;
  e.enable = enable;
  e.e = event_init(EVT_IM_ACTION_INFO, im);

  im->action_button_enable = enable;
  tk_strncpy(im->action_buton_text, text ? text : "", TK_NAME_LEN);

  return input_method_dispatch(im, (event_t*)(&e));
}

ret_t input_method_dispatch_action(input_method_t* im) {
  event_t e = event_init(EVT_IM_ACTION, im);
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  input_method_dispatch(im, &e);

  if (im->engine != NULL && im->engine->keys.size > 0) {
    char text[64];
    tk_strncpy(text, im->engine->keys.str, sizeof(text) - 1);
    input_method_commit_text(im, text);
  }

  return input_method_dispatch_to_widget(im, &e);
}

static ret_t input_method_dispatch_key_only(input_method_t* im, uint32_t key) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, NULL, key);

  input_method_dispatch_to_widget(input_method(), (event_t*)&e);
  e.e.type = EVT_KEY_UP;
  input_method_dispatch_to_widget(input_method(), (event_t*)&e);

  return RET_OK;
}

ret_t input_method_dispatch_key(input_method_t* im, uint32_t key) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);

  if (im->engine != NULL) {
    if (input_engine_input(im->engine, (char)key) == RET_OK) {
      return RET_OK;
    }
  }

  return input_method_dispatch_key_only(im, key);
}

ret_t input_method_dispatch_keys(input_method_t* im, const char* keys) {
  return_value_if_fail(im != NULL && keys != NULL, RET_BAD_PARAMS);

  if (im->engine != NULL) {
    return input_engine_search(im->engine, keys);
  }

  return RET_OK;
}

ret_t input_method_set_lang(input_method_t* im, const char* lang) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);
  if (im->engine != NULL) {
    if (input_engine_set_lang(im->engine, lang) == RET_OK) {
      event_t e = event_init(EVT_IM_LANG_CHANGED, im);
      input_method_dispatch(im, &e);

      return RET_OK;
    }
  }

  return RET_FAIL;
}

const char* input_method_get_lang(input_method_t* im) {
  return_value_if_fail(im != NULL, NULL);

  if (im->engine != NULL) {
    return input_engine_get_lang(im->engine);
  }

  return NULL;
}

ret_t input_method_dispatch_candidates(input_method_t* im, const char* strs, uint32_t nr,
                                       int32_t selected) {
  im_candidates_event_t ce;

  ce.e = event_init(EVT_IM_SHOW_CANDIDATES, im);
  ce.candidates_nr = nr;
  ce.candidates = strs;
  ce.selected = selected;

  return input_method_dispatch(im, (event_t*)(&ce));
}

ret_t input_method_dispatch_pre_candidates(input_method_t* im, const char* strs, uint32_t nr,
                                           int32_t selected) {
  im_candidates_event_t ce;

  ce.e = event_init(EVT_IM_SHOW_PRE_CANDIDATES, im);
  ce.candidates_nr = nr;
  ce.candidates = strs;
  ce.selected = selected;

  return input_method_dispatch(im, (event_t*)(&ce));
}

ret_t input_method_commit_text_ex(input_method_t* im, bool_t replace, const char* text) {
  im_commit_event_t e;
  return_value_if_fail(im != NULL && text != NULL, RET_BAD_PARAMS);

  if (im->engine) {
    input_engine_reset_input(im->engine);
    input_method_dispatch_candidates(im, "", 0, 0);
  }

  return input_method_dispatch_to_widget(input_method(), im_commit_event_init(&e, text, replace));
}

ret_t input_method_commit_text(input_method_t* im, const char* text) {
  return input_method_commit_text_ex(im, FALSE, text);
}

ret_t input_method_destroy(input_method_t* im) {
  return_value_if_fail(im != NULL, RET_BAD_PARAMS);
  if (im->destroy) {
    im->destroy(im);
  }

  return RET_OK;
}

event_t* im_commit_event_init(im_commit_event_t* e, const char* text, bool_t replace) {
  return_value_if_fail(e != NULL && text != NULL, NULL);

  memset(e, 0x00, sizeof(*e));

  e->text = text;
  e->replace = replace;
  e->e = event_init(EVT_IM_COMMIT, NULL);

  return (event_t*)e;
}

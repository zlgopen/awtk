/**
 * File:   demo_edit_suggest.c
 * Author: AWTK Develop Team
 * Brief:  edit输入建议demo
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-02-11 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "awtk.h"

#define APP_TYPE APP_DESKTOP

static darray_t* s_suggest_words_origin = NULL;
static tk_object_t* s_suggest_words = NULL;
static tk_object_t* s_suggest_words_list = NULL;

static int get_suggest_words_cmp(const void* iter, const void* ctx) {
  const char* str = (const char*)iter;
  const char* prefix = (const char*)ctx;
  return !tk_str_case_start_with(str, prefix);
}

static ret_t suggest_words_list_on_update(void* ctx, event_t* e) {
  uint32_t i = 0;
  const char* input = NULL;
  value_change_event_t* evt = value_change_event_cast(e);
  return_value_if_fail(evt != NULL, RET_BAD_PARAMS);

  input = value_str(&evt->new_value);

  tk_object_clear_props(s_suggest_words_list);

  log_debug("%s:[\n", __FUNCTION__);

  for (i = 0; i < s_suggest_words_origin->size; i++) {
    const char* iter = (const char*)darray_get(s_suggest_words_origin, i);
    if (0 == get_suggest_words_cmp(iter, input)) {
      value_t v;
      tk_object_t* obj = object_default_create_ex(FALSE);
      str_t desc;

      str_init(&desc, 0);
      str_set(&desc, "This is description by ");
      str_append(&desc, iter);
      object_set_prop_str(obj, "desc", desc.str);

      object_set_prop_str(obj, EDIT_EX_DEFAULT_SUGGEST_WORDS_INPUT_NAME, iter);

      object_array_push(s_suggest_words_list, value_set_object(&v, obj));

      log_debug("%s,\n", iter);

      str_reset(&desc);
      TK_OBJECT_UNREF(obj);
    }
  }

  log_debug("]\n");

  return RET_OK;
}

static ret_t suggest_words_on_update(void* ctx, event_t* e) {
  uint32_t i = 0;
  const char* input = NULL;
  value_change_event_t* evt = value_change_event_cast(e);
  return_value_if_fail(evt != NULL, RET_BAD_PARAMS);

  input = value_str(&evt->new_value);

  tk_object_clear_props(s_suggest_words);

  log_debug("%s:[\n", __FUNCTION__);

  for (i = 0; i < s_suggest_words_origin->size; i++) {
    const char* iter = (const char*)darray_get(s_suggest_words_origin, i);
    if (0 == get_suggest_words_cmp(iter, input)) {
      value_t v;
      object_array_push(s_suggest_words, value_set_str(&v, iter));
      log_debug("%s,\n", iter);
    }
  }

  log_debug("]\n");

  return RET_OK;
}

static ret_t mledit_suggest_words_init(widget_t* mledit) {
  tokenizer_t t;
  const char* iter = NULL;
  str_t suggest_words;
  str_init(&suggest_words, 0);
  str_from_wstr(&suggest_words, widget_get_text(mledit));
  tokenizer_init(&t, suggest_words.str, suggest_words.size, "\n");

  for (iter = tokenizer_next(&t); iter != NULL; iter = tokenizer_next(&t)) {
    darray_push(s_suggest_words_origin, tk_strdup(iter));
  }

  str_reset(&suggest_words);

  return RET_OK;
}

static ret_t on_close_window(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  (void)e;
  return window_close(win);
}

/**
 * 子控件初始化(主要是设置click回调、初始显示信息)
 */
static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = WIDGET(ctx);

  if (widget->name != NULL) {
    const char* name = widget->name;
    const char* type = widget_get_type(widget);

    if (tk_str_eq(name, "close()")) {
      widget_on(widget, EVT_CLICK, on_close_window, win);
    } else if (tk_str_eq(type, WIDGET_TYPE_EDIT_EX)) {
      edit_ex_t* edit_ex = edit_ex_cast(widget);
      if (TK_STR_IS_NOT_EMPTY(edit_ex->suggest_words_item_formats)) {
        edit_ex_set_suggest_words(widget, s_suggest_words_list);
      } else {
        edit_ex_set_suggest_words(widget, s_suggest_words);
      }
    } else if (tk_str_eq(type, WIDGET_TYPE_MLEDIT) && tk_str_eq(name, "suggest_words")) {
      mledit_suggest_words_init(widget);
    }
  }

  return RET_OK;
}

/**
 * 初始化窗口上的子控件
 */
static void init_children_widget(widget_t* widget, void* ctx) {
  widget_foreach(widget, init_widget, ctx);
}

/**
 * 初始化
 */
ret_t application_init(void) {
  widget_t* win = window_open("edit_suggest");

  s_suggest_words_origin = darray_create(32, default_destroy, (tk_compare_t)tk_str_cmp);

  s_suggest_words = object_array_create();
  emitter_on(EMITTER(s_suggest_words), EVT_VALUE_CHANGED, suggest_words_on_update, NULL);

  s_suggest_words_list = object_array_create();
  emitter_on(EMITTER(s_suggest_words_list), EVT_VALUE_CHANGED, suggest_words_list_on_update, NULL);

  init_children_widget(win, (void*)win);

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  TK_OBJECT_UNREF(s_suggest_words_list);
  TK_OBJECT_UNREF(s_suggest_words);
  darray_destroy(s_suggest_words_origin);

  log_debug("application_exit\n");

  return RET_OK;
}

#include "awtk_main.inc"

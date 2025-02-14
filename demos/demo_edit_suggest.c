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

static int get_suggest_words_cmp(const void* iter, const void* ctx) {
  const char* str = (const char*)iter;
  const char* prefix = (const char*)ctx;
  return !tk_str_case_start_with(str, prefix);
}

static ret_t suggest_words_on_update(void* ctx, event_t* e) {
  ret_t ret = RET_OK;
  darray_t matched;
  const char* input = NULL;
  value_change_event_t* evt = value_change_event_cast(e);
  return_value_if_fail(evt != NULL, RET_BAD_PARAMS);

  input = value_str(&evt->new_value);
  darray_init(&matched, 32, NULL, (tk_compare_t)tk_str_cmp);
  ret = darray_find_all(s_suggest_words_origin, get_suggest_words_cmp, (void*)input, &matched);
  if (RET_OK == ret) {
    uint32_t i = 0;
    tk_object_clear_props(s_suggest_words);
    for (i = 0; i < matched.size; i++) {
      value_t v;
      const char* iter = darray_get(&matched, i);
      object_array_push(s_suggest_words, value_set_str(&v, (const char*)iter));
    }
  }
  darray_deinit(&matched);

  return ret;
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
      edit_ex_set_suggest_words(widget, s_suggest_words);
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

  init_children_widget(win, (void*)win);

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  TK_OBJECT_UNREF(s_suggest_words);
  darray_destroy(s_suggest_words_origin);

  log_debug("application_exit\n");

  return RET_OK;
}

#include "awtk_main.inc"

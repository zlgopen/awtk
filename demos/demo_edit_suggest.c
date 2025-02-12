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

static darray_t* s_suggest_words = NULL;

static int get_suggest_words_cmp(const void* iter, const void* ctx) {
  const char* str = (const char*)iter;
  const char* prefix = (const char*)ctx;
  return !tk_str_case_start_with(str, prefix);
}

static void* get_suggest_words(widget_t* widget, const char* input, void* ctx) {
  darray_t* ret = darray_create(32, NULL, (tk_compare_t)tk_str_cmp);
  if (RET_OK == darray_find_all(s_suggest_words, get_suggest_words_cmp, (void*)input, ret) &&
      ret->size > 0) {
    return ret;
  }
  darray_destroy(ret);
  return NULL;
}

static ret_t free_suggest_words(widget_t* widget, void* suggest_words, void* ctx) {
  return darray_destroy((darray_t*)suggest_words);
}

static ret_t mledit_suggest_words_init(widget_t* mledit) {
  tokenizer_t t;
  const char* iter = NULL;
  str_t suggest_words;
  str_init(&suggest_words, 0);
  str_from_wstr(&suggest_words, widget_get_text(mledit));
  tokenizer_init(&t, suggest_words.str, suggest_words.size, "\n");

  for (iter = tokenizer_next(&t); iter != NULL; iter = tokenizer_next(&t)) {
    darray_push(s_suggest_words, tk_strdup(iter));
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
    } else if (tk_str_eq(type, WIDGET_TYPE_EDIT)) {
      edit_set_suggest_words_callback_simple(widget, get_suggest_words, free_suggest_words, NULL,
                                             NULL);
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

  s_suggest_words = darray_create(32, default_destroy, (tk_compare_t)tk_str_cmp);

  init_children_widget(win, (void*)win);

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  darray_destroy(s_suggest_words);

  log_debug("application_exit\n");

  return RET_OK;
}

#include "awtk_main.inc"

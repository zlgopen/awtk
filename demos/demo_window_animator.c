/**
 * File:   demo_window_animator.c
 * Author: AWTK Develop Team
 * Brief:  窗口动画 Demo
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-12-17 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "awtk.h"

#define WINDOW_NAME_PREFIX "win_anim/"

typedef enum _demo_window_animator_state_t {
  DEMO_WINDOW_ANIMATOR_STATE_NORMAL = 0,
  DEMO_WINDOW_ANIMATOR_STATE_NEW,
  DEMO_WINDOW_ANIMATOR_STATE_ERROR,
} demo_window_animator_state_t;

static void init_children_widget(widget_t* widget, void* ctx);

static widget_t* window_open_with_prefix(const char* name) {
  char name_with_prefix[TK_NAME_LEN + 1] = {0};
  tk_snprintf(name_with_prefix, ARRAY_SIZE(name_with_prefix), "%s%s", WINDOW_NAME_PREFIX, name);
  return window_open(name_with_prefix);
}

static ret_t new_window_set_param(widget_t* new_win, widget_t* curr_win) {
  widget_t* state_pages = NULL;
  widget_t *widget_fullscreen = NULL, *widget_new_fullscreen = NULL;
  bool_t fullscreen = FALSE;
  widget_t *widget_anim_hint = NULL, *widget_anim_duration = NULL, *widget_anim_easing = NULL;
  char anim_hint[TK_NAME_LEN + 1] = {0}, anim_duration[TK_NAME_LEN + 1] = {0},
                               anim_easing[TK_NAME_LEN + 1] = {0};
  char anim[ARRAY_SIZE(anim_hint) + ARRAY_SIZE(anim_duration) + ARRAY_SIZE(anim_easing) +
            TK_NAME_LEN + 1] = {0};
  char widget_name[TK_NAME_LEN + 1] = {0};

  tk_snprintf(widget_name, ARRAY_SIZE(widget_name), "anim_hint(%s)", new_win->name);
  widget_anim_hint = widget_lookup(curr_win, widget_name, TRUE);
  if (widget_anim_hint != NULL) {
    widget_get_text_utf8(widget_anim_hint, anim_hint, ARRAY_SIZE(anim_hint));
  }

  tk_snprintf(widget_name, ARRAY_SIZE(widget_name), "anim_hint(%s):duration", new_win->name);
  widget_anim_duration = widget_lookup(curr_win, widget_name, TRUE);
  if (widget_anim_duration != NULL) {
    widget_get_text_utf8(widget_anim_duration, anim_duration, ARRAY_SIZE(anim_duration));
  }

  tk_snprintf(widget_name, ARRAY_SIZE(widget_name), "anim_hint(%s):easing", new_win->name);
  widget_anim_easing = widget_lookup(curr_win, widget_name, TRUE);
  if (widget_anim_easing != NULL) {
    widget_get_text_utf8(widget_anim_easing, anim_easing, ARRAY_SIZE(anim_easing));
  }

  if (!tk_str_eq(anim_hint, "none")) {
    tk_snprintf(anim, ARRAY_SIZE(anim), "%s(duration=%s,easing=%s)", anim_hint, anim_duration,
                anim_easing);
    widget_set_prop_bool(new_win, WIDGET_PROP_DISABLE_ANIM, FALSE);
    widget_set_prop_str(new_win, WIDGET_PROP_ANIM_HINT, anim);
    log_debug("%s\r\n", anim);
  } else {
    widget_set_prop_bool(new_win, WIDGET_PROP_DISABLE_ANIM, TRUE);
    log_debug("none\r\n");
  }

  tk_snprintf(widget_name, ARRAY_SIZE(widget_name), "fullscreen(%s)", new_win->name);
  widget_fullscreen = widget_lookup(curr_win, widget_name, TRUE);
  if (widget_fullscreen != NULL) {
    fullscreen = widget_get_value_int(widget_fullscreen);
    window_set_fullscreen(new_win, fullscreen);
  }

  widget_new_fullscreen = widget_lookup(new_win, "fullscreen()", TRUE);
  if (widget_new_fullscreen != NULL) {
    widget_set_value_int(widget_new_fullscreen, fullscreen);
  }

  state_pages = widget_lookup(new_win, "state()", TRUE);
  if (state_pages != NULL) {
    pages_set_active(state_pages, DEMO_WINDOW_ANIMATOR_STATE_NEW);
  }

  widget_set_prop_int(new_win, WIDGET_PROP_CLOSABLE, WINDOW_CLOSABLE_YES);

  return RET_OK;
}

static ret_t on_open_window(void* ctx, event_t* e) {
  widget_t* new_win = NULL;
  widget_t* curr_win = widget_get_window(WIDGET(e->target));
  const char* name = (const char*)ctx;

  new_win = window_open_with_prefix(name);
  if (new_win != NULL) {
    new_window_set_param(new_win, curr_win);
    init_children_widget(new_win, (void*)new_win);
  }

  return RET_OK;
}

static ret_t on_close_window(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  (void)e;
  return window_close(win);
}

static ret_t on_fullscreen(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  value_change_event_t* evt = value_change_event_cast(e);

  return window_set_fullscreen(win, value_bool(&evt->new_value));
}

/**
 * 子控件初始化(主要是设置click回调、初始显示信息)
 */
static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = WIDGET(ctx);

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (strstr(name, "open:") != NULL) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
    } else if (tk_str_eq(name, "close()")) {
      widget_on(widget, EVT_CLICK, on_close_window, win);
    } else if (tk_str_eq(name, "fullscreen()")) {
      widget_on(widget, EVT_VALUE_CHANGED, on_fullscreen, win);
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
  widget_t* win = NULL;
  widget_t* state_pages = NULL;
  window_open("system_bar");
  win = window_open_with_prefix("window");

  init_children_widget(win, (void*)win);

  widget_set_prop_int(win, WIDGET_PROP_CLOSABLE, WINDOW_CLOSABLE_NO);

  state_pages = widget_lookup(win, "state()", TRUE);
  if (state_pages != NULL) {
#ifdef WITHOUT_WINDOW_ANIMATORS
    pages_set_active(state_pages, DEMO_WINDOW_ANIMATOR_STATE_ERROR);
#else
    pages_set_active(state_pages, DEMO_WINDOW_ANIMATOR_STATE_NORMAL);
#endif /* WITHOUT_WINDOW_ANIMATORS */
  }

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

/**
 * File:   demo_ui_app.c
 * Author: AWTK Develop Team
 * Brief:  Demo UI
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-07-01 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "awtk.h"

#define WINDOW_NAME_PREFIX "uiex/"
#define THEME_NAME_PREFIX "uiex_"

#define PRELOAD_DURATION 2000 /* 预加载窗口显示时长（单位：毫秒） */
#define ENABLE_PAGE_SWITCH 0  /* 是否开启滚动切换页面功能 */

#define PAGE_NUM 18
#if !defined(PAGE_NUM) || PAGE_NUM <= 0
#error "PAGE_NUM not define or less than or equal to zero!"
#endif /* PAGE_NUM */

#define LANDSCAPE_WIDTH_THRESHOLD 540 /* 横屏窗口阈值 */

#define PRELOAD_NAME "preload"

#ifdef WITH_FS_RES
#ifndef APP_DEFAULT_FONT
#define APP_DEFAULT_FONT "default_full"
#endif /* APP_DEFAULT_FONT */
#endif /* WITH_FS_RES */

#ifdef AWTK_WEB
static bool_t s_is_full_font = TRUE;
#else
static bool_t s_is_full_font = FALSE;
#endif /* AWTK_WEB */

static const char* s_win_name = "menu_${device_orientation}";
static const char* s_sysbar_name = "sysbar_${device_orientation}";

/*** common **********************************************************************/
extern ret_t assets_set_global_theme(const char* name);

/* 在 dialog 切换语言或主题时，使用此函数刷新 dialog 高亮背景 */
static ret_t refresh_prev_window(void) {
  bitmap_t img = {0};
  widget_t* prev_win = window_manager_get_top_main_window(window_manager());
  return window_manager_snap_prev_window(window_manager(), prev_win, &img);
}

static ret_t change_language(bool_t is_chinese) {
  ret_t ret = RET_FAIL;
  const char* country = is_chinese ? "CN" : "US";
  const char* language = is_chinese ? "zh" : "en";
  if (strcmp(locale_info()->country, country) != 0 ||
      strcmp(locale_info()->language, language) != 0) {
    ret = locale_info_change(locale_info(), language, country);
  } else {
    event_t e = event_init(EVT_LOCALE_CHANGED, locale_info());
    ret = locale_info_reload(locale_info());
    emitter_dispatch(locale_info()->emitter, &e);
  }
  return ret;
}

static ret_t on_language_changed(void* ctx, event_t* e) {
  value_change_event_t* evt = value_change_event_cast(e);
  return change_language(value_bool(&(evt->new_value)));
}

static ret_t on_dialog_language_changed(void* ctx, event_t* e) {
  ret_t ret = on_language_changed(ctx, e);
  refresh_prev_window();

  return ret;
}

static ret_t change_theme(bool_t is_dark) {
  return assets_set_global_theme(is_dark ? "dark" : "default");
}

static ret_t on_change_theme(void* ctx, event_t* e) {
  value_change_event_t* evt = value_change_event_cast(e);
  return change_theme(value_bool(&(evt->new_value)));
}

static ret_t on_dialog_change_theme(void* ctx, event_t* e) {
  ret_t ret = on_change_theme(ctx, e);
  refresh_prev_window();

  return ret;
}

#define DIALOG_INFO_THEME THEME_NAME_PREFIX "dialog_info"
#define DIALOG_WARN_THEME THEME_NAME_PREFIX "dialog_warn"
#define DIALOG_CONFIRM_THEME THEME_NAME_PREFIX "dialog_confirm"
extern ret_t dialog_simple_show(const char* stitle, const char* scontent, const char* theme,
                                bool_t has_ok, bool_t has_cancel);

static ret_t common_init_widget(void* ctx, const void* iter);

/**
 * 点击对话框外，退出对话框
 */
static ret_t on_dialog_quit(void* ctx, event_t* e) {
  widget_t* dialog_win = (widget_t*)ctx;
  widget_t* wm = window_manager();
  dialog_t* dialog = DIALOG(dialog_win);
  pointer_event_t* evt = (pointer_event_t*)e;
  rect_t dialog_r = {dialog_win->x, dialog_win->y, dialog_win->w, dialog_win->h};
  rect_t wm_r = {wm->x, wm->y, wm->w, wm->h};
  return_value_if_fail(dialog != NULL && evt != NULL, RET_BAD_PARAMS);

  if (rect_contains(&wm_r, evt->x, evt->y) && !rect_contains(&dialog_r, evt->x, evt->y)) {
    return dialog->is_model ? dialog_quit(dialog_win, DIALOG_QUIT_CANCEL)
                            : window_close(dialog_win);
  }

  return RET_OK;
}

static widget_t* window_open_with_prefix(const char* name) {
  char name_with_prefix[TK_NAME_LEN + 1] = {0};

  tk_snprintf(name_with_prefix, ARRAY_SIZE(name_with_prefix), "%s%s", WINDOW_NAME_PREFIX, name);
  return window_open(name_with_prefix);
}

static ret_t on_quit(void* ctx, event_t* e) {
  return tk_quit();
}

static ret_t on_close_window(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  return window_close(win);
}

static const char* get_name(const char* path) {
  const char* name = path;
  uint32_t i = 0;
  uint32_t j = 0;
  for (; path[i] != '\0'; i++) {
    if (path[i] == '\\' || path[i] == '/') {
      j = i;
    }
  }
  if (j != 0) {
    name = path + j + 1;
  }
  return name;
}

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* path = (const char*)ctx;
  const char* name = get_name(path);

  if (tk_str_eq(name, "toast")) {
    dialog_toast("Hello! AWTK.", 2000);
  } else if (tk_str_eq(name, "info")) {
    dialog_simple_show("info", "Hello! AWTK.", DIALOG_INFO_THEME, TRUE, FALSE);
  } else if (tk_str_eq(name, "warn")) {
    dialog_simple_show("Warning", "Not sufficient funds!", DIALOG_WARN_THEME, TRUE, FALSE);
  } else if (tk_str_eq(name, "confirm")) {
    dialog_simple_show("Confirm", "Are you ready?", DIALOG_CONFIRM_THEME, TRUE, TRUE);
  } else {
    widget_t* target = widget_lookup(window_manager(), name, TRUE);

    if (target != NULL) {
      widget_t* win = widget_get_window(WIDGET(e->target));
      window_manager_switch_to(window_manager(), win, target, FALSE);
    } else {
      widget_t* win = window_open(path);
      widget_foreach(win, common_init_widget, NULL);
    }
  }

  (void)e;

  return RET_OK;
}

static ret_t on_open_or_close_window(void* ctx, event_t* e) {
  ret_t ret = RET_FAIL;
  widget_t* widget = NULL;
  return_value_if_fail(e != NULL, RET_BAD_PARAMS);

  widget = WIDGET(e->target);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget_get_value(widget)) {
    ret = on_open_window(ctx, e);
  } else {
    const char* path = (const char*)ctx;
    const char* name = get_name(path);
    widget_t* win = widget_lookup(window_manager(), name, FALSE);
    ret = on_close_window((void*)win, e);
  }
  return ret;
}

/** scroll_view 中含有 pages 时，
 *  由于 widget_ensure_visible_in_viewport() 函数的作用，
 *  需要用此函数将 scroll_view 重新滚动到最顶。
**/
static ret_t on_scroll_to_top(const idle_info_t* idle) {
  widget_t* scroll_view = WIDGET(idle->ctx);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  scroll_view_set_offset(scroll_view, 0, 0);

  return RET_REMOVE;
}

static widget_t* find_bind_value_target(widget_t* widget, const char* name) {
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  if (tk_str_start_with(name, "bind_value:")) {
    widget_t* parent = widget->parent;
    const char* subname = NULL;
    tokenizer_t t;

    name += 11;
    tokenizer_init(&t, name, tk_strlen(name), "/");

    while ((subname = tokenizer_next(&t)) != NULL) {
      if (tokenizer_has_more(&t)) {
        if (tk_str_eq(subname, "..")) {
          parent = parent->parent;
        } else {
          while (parent != NULL) {
            widget_t* tmp = widget_lookup(parent, subname, FALSE);
            if (tmp != NULL) {
              parent = tmp;
              break;
            } else {
              parent = parent->parent;
            }
          }
        }
      } else {
        target = widget_lookup(parent, subname, FALSE);
      }
    }
  }
  return target;
}

static ret_t on_bind_value_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* target = WIDGET(e->target);
  return_value_if_fail(widget != NULL && target != NULL, RET_BAD_PARAMS);

  return widget_set_prop_float(widget, "animate:value", widget_get_value(target));
}

static ret_t common_init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = widget_get_window(widget);
  (void)ctx;

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (strstr(name, "open:") != NULL) {
      const char* type = widget_get_type(widget);
      if (tk_str_eq(type, WIDGET_TYPE_BUTTON)) {
        widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
        widget_on(widget, EVT_LONG_PRESS, on_open_window, (void*)(name + 5));
      } else if (tk_str_eq(type, WIDGET_TYPE_CHECK_BUTTON)) {
        const char* true_name = get_name(name + 5);
        widget_set_value(widget, widget_lookup(window_manager(), true_name, FALSE) != NULL);
        widget_on(widget, EVT_VALUE_CHANGED, on_open_or_close_window, (void*)(name + 5));
      }
    } else if (tk_str_eq(name, "close()")) {
      widget_on(widget, EVT_POINTER_UP, on_close_window, win);
    } else if (tk_str_eq(name, "quit()")) {
      widget_on(widget, EVT_POINTER_UP, on_quit, NULL);
    } else if (tk_str_start_with(name, "bind_value:")) {
      widget_t* target = find_bind_value_target(widget, name);
      widget_on(target, EVT_VALUE_CHANGED, on_bind_value_changed, (void*)widget);
    }
  }

  return RET_OK;
}

/*** menu_bar **********************************************************************/
#define MENU_BAR_NAME "menu_bar"
static char s_page_name[32] = {0};

static ret_t on_page_change(void* ctx, event_t* e) {
  ret_t ret = RET_FAIL;
  widget_t* pages = WIDGET(ctx);

  widget_off_by_func(widget_get_window(pages), EVT_WINDOW_TO_FOREGROUND, on_page_change, pages);
  ret = pages_set_active_by_name(pages, s_page_name);
  memset(s_page_name, 0, ARRAY_SIZE(s_page_name));

  return ret;
}

static ret_t on_tb_value_changed(void* ctx, event_t* e) {
  widget_t* tb = WIDGET(e->target);
  widget_t* pages = WIDGET(ctx);
  widget_t* dialog_win = widget_get_window(tb);
  dialog_t* dialog = DIALOG(dialog_win);
  return_value_if_fail(tb != NULL && pages != NULL && dialog != NULL, RET_BAD_PARAMS);

  if (widget_get_value(tb)) {
    tk_snprintf(s_page_name, ARRAY_SIZE(s_page_name), "page_%s",
                tb->name + 3); /* tb_xxx -> page_xxx */
    widget_on(widget_get_window(pages), EVT_WINDOW_TO_FOREGROUND, on_page_change, pages);

    return dialog->is_model ? dialog_quit(dialog_win, DIALOG_QUIT_OK) : window_close(dialog_win);
  }

  return RET_OK;
}

static ret_t menu_bar_init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  pages_t* pages = PAGES(ctx);

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (tk_str_eq(name, "view_menu")) {
      uint32_t j = 0;
      WIDGET_FOR_EACH_CHILD_BEGIN(widget, tb, i)
      const char* type = widget_get_type(tb);
      if (tk_str_eq(type, WIDGET_TYPE_TAB_BUTTON)) {
        if (pages->active == j++) {
          widget_set_value(tb, TRUE);
        }
      }
      WIDGET_FOR_EACH_CHILD_END()

      WIDGET_FOR_EACH_CHILD_BEGIN(widget, tb, i)
      const char* type = widget_get_type(tb);
      if (tk_str_eq(type, WIDGET_TYPE_TAB_BUTTON)) {
        widget_on(tb, EVT_VALUE_CHANGED, on_tb_value_changed, &pages->widget);
      }
      WIDGET_FOR_EACH_CHILD_END()
    } else if (tk_str_eq(name, "rb_dark")) {
      widget_set_value(widget, tk_str_eq(assets_manager()->theme, "dark"));
      widget_on(widget, EVT_VALUE_CHANGED, on_dialog_change_theme, NULL);
    } else if (tk_str_eq(name, "rb_zh")) {
      if (s_is_full_font) {
        widget_set_value(widget, tk_str_eq(locale_info()->language, "zh"));
        widget_on(widget, EVT_VALUE_CHANGED, on_dialog_language_changed, NULL);
      } else {
        widget_set_value(widget, FALSE);
        widget_set_enable(widget->parent, FALSE);
      }
    }
  }

  return RET_OK;
}

static ret_t on_menu_bar_move_resize(void* ctx, event_t* e) {
  widget_t* menu_bar = WIDGET(e->target);
  return widget_move_resize(menu_bar, 0, 0, menu_bar->w, window_manager()->h);
}

static ret_t open_menu_bar(widget_t* pages) {
  widget_t* dialog = widget_lookup(window_manager(), MENU_BAR_NAME, FALSE);
  if (dialog != NULL) {
    return RET_SKIP;
  }
  widget_set_as_key_target(pages);
  dialog = window_open_with_prefix(MENU_BAR_NAME);

  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  widget_foreach(dialog, menu_bar_init_widget, pages);
  widget_on(dialog, EVT_POINTER_UP, on_dialog_quit, dialog);
  widget_on(dialog, EVT_MOVE_RESIZE, on_menu_bar_move_resize, NULL);
  dialog_modal(dialog);
  return RET_OK;
}

/*** page_button **********************************************************************/
static ret_t on_fps_visable(void* ctx, event_t* e) {
  value_change_event_t* vce = value_change_event_cast(e);
  return_value_if_fail(vce != NULL, RET_BAD_PARAMS);

  window_manager_set_show_fps(window_manager(), value_bool(&vce->new_value));
  widget_invalidate_force(window_manager(), NULL);
  return RET_OK;
}

static ret_t page_button_init(widget_t* page) {
  widget_t* cb_fps = NULL;
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  cb_fps = widget_lookup(page, "cb_fps", TRUE);
  if (cb_fps != NULL && tk_str_eq(widget_get_type(cb_fps), WIDGET_TYPE_CHECK_BUTTON)) {
    widget_set_value(cb_fps, WINDOW_MANAGER(window_manager())->show_fps);
    widget_on(cb_fps, EVT_VALUE_CHANGED, on_fps_visable, NULL);
  }

  return RET_OK;
}

/*** page_label **********************************************************************/
static ret_t change_rich_text(widget_t* rich_text) {
  const char* tr_text = NULL;
  return_value_if_fail(rich_text != NULL, RET_BAD_PARAMS);

  tr_text = locale_info_tr(widget_get_locale_info(rich_text), rich_text->tr_text);
  widget_set_text_utf8(rich_text, tr_text);

  return RET_OK;
}

static ret_t on_change_rich_text(void* ctx, event_t* e) {
  widget_t* rich_text = WIDGET(ctx);
  return change_rich_text(rich_text);
}

static ret_t on_rich_text_destroy(void* ctx, event_t* e) {
  widget_t* rich_text = WIDGET(e->target);
  widget_t* win = WIDGET(ctx);
  return widget_off_by_func(win, EVT_LOCALE_CHANGED, on_change_rich_text, (void*)rich_text);
}

static ret_t page_label_init(widget_t* page) {
  widget_t* rich_text = NULL;
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  rich_text = widget_lookup(page, "rich_text", TRUE);
  if (rich_text != NULL && tk_str_eq(widget_get_type(rich_text), WIDGET_TYPE_RICH_TEXT)) {
    widget_t* win = widget_get_window(rich_text);
    change_rich_text(rich_text);
    widget_on(win, EVT_LOCALE_CHANGED, on_change_rich_text, (void*)rich_text);
    widget_on(rich_text, EVT_DESTROY, on_rich_text_destroy, (void*)win);
  }

  return RET_OK;
}

/*** page_slider **********************************************************************/
static ret_t page_slider_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);
  return widget_foreach(page, common_init_widget, NULL);
}

/*** page_image **********************************************************************/
#define CHAT_BUBBLE_ADD_CHAR_INTERVAL 150 /* 聊天气泡添加字符时间（毫秒）间隔 */

static char s_chat_bubble_origin[64] = "";
static const char* s_chat_bubble = " ";
static uint32_t s_chat_bubble_len = 1;
static uint32_t s_chat_bubble_curr_len = 0;

static ret_t add_char_to_chat_bubble(const timer_info_t* timer) {
  widget_t* lb_chat_bubble = timer->ctx;
  wstr_t str;
  return_value_if_fail(lb_chat_bubble != NULL, RET_REMOVE);

  wstr_init(&str, s_chat_bubble_curr_len);
  if (s_chat_bubble_curr_len > 0) {
    wstr_set_utf8_with_len(&str, s_chat_bubble, s_chat_bubble_curr_len);
  } else {
    wstr_set_utf8(&str, " ");
  }
  widget_set_text(lb_chat_bubble, str.str);
  wstr_reset(&str);

  s_chat_bubble_curr_len = (s_chat_bubble_curr_len + 1) % (s_chat_bubble_len + 1);

  return RET_REPEAT;
}

static ret_t change_chat_content(widget_t* chat_bubble) {
  s_chat_bubble = locale_info_tr(widget_get_locale_info(chat_bubble), s_chat_bubble_origin);
  s_chat_bubble_len = tk_strlen(s_chat_bubble);

  if (s_chat_bubble_curr_len > s_chat_bubble_len - 1) {
    s_chat_bubble_curr_len = s_chat_bubble_len - 1;
  }
  return RET_OK;
}

static ret_t on_change_chat_content(void* ctx, event_t* e) {
  widget_t* lb_chat_bubble = WIDGET(ctx);
  return change_chat_content(lb_chat_bubble);
}

static ret_t on_chat_bubble_destroy(void* ctx, event_t* e) {
  widget_t* lb_chat_bubble = WIDGET(e->target);
  widget_t* win = WIDGET(ctx);
  return widget_off_by_func(win, EVT_LOCALE_CHANGED, on_change_chat_content, (void*)lb_chat_bubble);
}

static ret_t page_image_init(widget_t* page) {
  widget_t* lb_chat_bubble = NULL;
  widget_t* scroll_view_to_top = NULL;
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  lb_chat_bubble = widget_lookup(page, "lb_chat_bubble", TRUE);
  if (lb_chat_bubble != NULL && widget_get_visible(lb_chat_bubble)) {
    widget_t* win = widget_get_window(lb_chat_bubble);
    widget_get_text_utf8(lb_chat_bubble, s_chat_bubble_origin, ARRAY_SIZE(s_chat_bubble_origin));
    change_chat_content(lb_chat_bubble);
    widget_on(win, EVT_LOCALE_CHANGED, on_change_chat_content, (void*)lb_chat_bubble);
    widget_on(lb_chat_bubble, EVT_DESTROY, on_chat_bubble_destroy, (void*)win);
    widget_add_timer(lb_chat_bubble, add_char_to_chat_bubble, CHAT_BUBBLE_ADD_CHAR_INTERVAL);
  }

  scroll_view_to_top = widget_lookup(page, "scroll_view_to_top", TRUE);
  if (scroll_view_to_top != NULL &&
      tk_str_eq(widget_get_type(scroll_view_to_top), WIDGET_TYPE_SCROLL_VIEW)) {
    widget_add_idle(scroll_view_to_top, on_scroll_to_top);
  }

  return RET_OK;
}

/*** page_mledit **********************************************************************/
static ret_t widget_dispatch_callback(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);

  return widget_dispatch(widget, (event_t*)ctx);
}

static ret_t on_push_window_cancel(void* ctx, event_t* e) {
  input_method_t* im = (input_method_t*)ctx;
  /**
   *  返回 RET_STOP 在 该函数注册之前的该事件（EVT_WINDOW_OPEN）的事件处理函数都会失效，
   *  由于 input_method_default.inc 中的 on_push_window() 
   *  和 input_method_default_on_keyboard_open() 失效，所以需要做以下处理：
   */
  im->busy = FALSE;
  im->win_old_y = im->win->y;

  WIDGET_FOR_EACH_CHILD_BEGIN(im->keyboard, iter, i)
  ret_t ret = widget_foreach(iter, widget_dispatch_callback, (void*)e);
  if (ret == RET_STOP || ret == RET_DONE) {
    return ret;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_STOP;
}

static ret_t on_keyboard_reset_ani(void* ctx, event_t* e) {
  widget_t* kb = WIDGET(e->target);
  return_value_if_fail(kb != NULL, RET_BAD_PARAMS);

  widget_set_prop_str(kb, WIDGET_PROP_OPEN_ANIM_HINT, "popup");
  widget_set_prop_str(kb, WIDGET_PROP_CLOSE_ANIM_HINT, "popup");

  return RET_OK;
}

static ret_t on_mledit_view_adjust_layout(void* ctx, event_t* e) {
  widget_t* mledit_view = WIDGET(ctx);
  widget_t* kb = WIDGET(e->target);
  return_value_if_fail(mledit_view != NULL && kb != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_OPEN: {
      char param[64] = {0};
      tk_snprintf(param, sizeof(param), "default(x=0, y=0, w=100%%, h=-%d)", kb->h);
      widget_set_self_layout(mledit_view, param);
      break;
    }
    case EVT_WINDOW_CLOSE: {
      widget_set_self_layout(mledit_view, "default(x=0, y=0, w=100%, h=100%)");
      break;
    }
    default:
      break;
  }
  widget_layout(mledit_view);

  return RET_OK;
}

static ret_t on_input_method_start_idle(const idle_info_t* idle) {
  widget_t* mledit_view = WIDGET(idle->ctx);
  input_method_t* im = input_method();
  return_value_if_fail(mledit_view != NULL && im != NULL, RET_REMOVE);

  if (im->keyboard != NULL) {
    /* 取消将窗口往上拉 */
    widget_on(im->keyboard, EVT_WINDOW_OPEN, on_push_window_cancel, (void*)im);

    /* 重新设置键盘的窗口动画 */
    widget_on(im->keyboard, EVT_WINDOW_WILL_OPEN, on_keyboard_reset_ani, NULL);

    /* 键盘在打开和关闭时都调整 mledit_view 自身布局 */
    widget_on(im->keyboard, EVT_WINDOW_OPEN, on_mledit_view_adjust_layout, (void*)mledit_view);
    widget_on(im->keyboard, EVT_WINDOW_CLOSE, on_mledit_view_adjust_layout, (void*)mledit_view);
  }

  return RET_REMOVE;
}

static ret_t on_input_method_start(void* ctx, event_t* e) {
  widget_t* mledit_view = WIDGET(ctx);
  return_value_if_fail(mledit_view != NULL, RET_BAD_PARAMS);

  /* 此时 im->keyboard 还无法获取，需要使用 idle 来绑定事件 */
  widget_add_idle(mledit_view, on_input_method_start_idle);

  return RET_OK;
}

static ret_t on_page_mledit_close(void* ctx, event_t* e) {
  widget_off_by_func(window_manager(), EVT_IM_START, on_input_method_start, ctx);
  return RET_OK;
}

static ret_t page_mledit_init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* page = WIDGET(ctx);

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (tk_str_eq(name, "mledit_view")) {
      widget_on(window_manager(), EVT_IM_START, on_input_method_start, widget);
      widget_on(page, EVT_VPAGE_CLOSE, on_page_mledit_close, widget);
    }
  }

  return RET_OK;
}

static ret_t page_mledit_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  return widget_foreach(page, page_mledit_init_widget, page);
}

/*** page_tab_ctrl **********************************************************************/
static ret_t page_tab_ctrl_init(widget_t* page) {
  widget_t* scroll_view_to_top = NULL;
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  scroll_view_to_top = widget_lookup(page, "scroll_view_to_top", TRUE);
  if (scroll_view_to_top != NULL &&
      tk_str_eq(widget_get_type(scroll_view_to_top), WIDGET_TYPE_SCROLL_VIEW)) {
    widget_add_idle(scroll_view_to_top, on_scroll_to_top);
  }

  return RET_OK;
}

/*** page_color **********************************************************************/
static ret_t on_paint_linear_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0x82, 0xb4, 0x43, 0xff);
  color_t ecolor = color_init(0xff, 0xff, 0xff, 0xff);
  rect_t r = rect_init(0, 0, widget->w, widget->h);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  vgcanvas_set_fill_linear_gradient(vg, r.x, r.y, r.x + r.w, r.y, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_paint_radial_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0x82, 0xb4, 0x43, 0xff);
  color_t ecolor = color_init(0xff, 0xff, 0xff, 0xff);
  rect_t r = rect_init(0, 0, widget->w, widget->h);
  uint32_t radial = tk_max(r.w, r.h) / 2;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  vgcanvas_set_fill_radial_gradient(vg, r.x + r.w / 2, r.y + r.h / 2, 0, radial, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_paint_stroke_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0xd8, 0xd4, 0, 0xff);
  color_t ecolor = color_init(0x19, 0xa9, 0xeb, 0xff);
  uint32_t r = tk_min(widget->w, widget->h) / 3;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_stroke_linear_gradient(vg, 0, 0, widget->w, widget->h, scolor, ecolor);

  vgcanvas_move_to(vg, 10, widget->h / 4);
  vgcanvas_set_line_width(vg, 5);

  vgcanvas_line_to(vg, widget->w / 2, widget->h);
  vgcanvas_line_to(vg, widget->w / 2, 0);
  vgcanvas_line_to(vg, widget->w - 10, widget->h * 3 / 4);
  vgcanvas_stroke(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, widget->w / 2, widget->h / 2, r, 0, M_PI * 2, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t page_color_init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  (void)ctx;

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (tk_str_eq(name, "paint_linear_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_linear_gradient, NULL);
    } else if (tk_str_eq(name, "paint_radial_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_radial_gradient, NULL);
    } else if (tk_str_eq(name, "paint_stroke_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_stroke_gradient, NULL);
    }
  }

  return RET_OK;
}

static ret_t page_color_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);
  return widget_foreach(page, page_color_init_widget, NULL);
}

/*** page_animate **********************************************************************/
static ret_t page_animate_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);
  return widget_foreach(page, common_init_widget, NULL);
}

/*** page_chart **********************************************************************/
static ret_t paint_line_chart(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  bool_t is_dark = tk_str_eq(assets_manager()->theme, "dark");
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t color_point = color_init(0x70, 0x99, 0x18, 0xff);
  color_t color_point_bg =
      is_dark ? color_init(0x26, 0x26, 0x26, 0xff) : color_init(0xf7, 0xf7, 0xf7, 0xff);
  color_t color_line = color_init(0x70, 0x99, 0x18, 0xff);
  color_t color_axis = color_init(0x80, 0x80, 0x80, 0xff);
  const int32_t x_div_part = 10;
  const int32_t y_div_part = 5;
  const int32_t x_axis_offset = 20;
  const int32_t y_axis_offset = 50;
  const int32_t x_axis_end_sep = 20;
  const int32_t y_axis_end_sep = 10;
  point_t x_asis_p = {tk_max(y_axis_offset - 6, 0), widget->h - x_axis_offset};
  point_t y_asis_p = {y_axis_offset, y_axis_end_sep};
  int32_t x_axis_len = widget->w - x_asis_p.x - x_axis_end_sep;
  int32_t y_axis_len = widget->h - y_asis_p.y - tk_max(x_axis_offset - 6, 0);
  point_t org_p = {y_axis_offset, widget->h - x_axis_offset};
  const point_t data_p[11] = {{0, 0}, {1, 10}, {2, 30}, {3, 20}, {4, 40}, {5, 10},
                              {6, 0}, {7, 20}, {8, 40}, {9, 30}, {10, 50}};
  point_t tr_point[11] = {{0, 0}};
  point_t tmp_p;
  int32_t i = 0;

  vgcanvas_save(vg);

  canvas_set_stroke_color(c, color_axis);
  canvas_draw_hline(c, x_asis_p.x, x_asis_p.y, x_axis_len);
  canvas_draw_vline(c, y_asis_p.x, y_asis_p.y, y_axis_len);

  canvas_set_font(c, "default", 20);
  canvas_set_text_color(c, color_axis);
  canvas_set_text_align(c, ALIGN_H_CENTER, ALIGN_V_MIDDLE);

  x_axis_len -= 15;
  y_axis_len -= 15;

  for (tmp_p = org_p; tmp_p.x <= org_p.x + x_axis_len; tmp_p.x += x_axis_len / x_div_part) {
    wstr_t lb_str;
    rect_t r;
    wstr_init(&lb_str, 0);
    wstr_from_int(&lb_str, i++);
    r = rect_init(tmp_p.x - (c->font_size * lb_str.size / 2), tmp_p.y + 6,
                  c->font_size * lb_str.size, c->font_size);
    canvas_draw_text_in_rect(c, lb_str.str, lb_str.size, &r);
    wstr_reset(&lb_str);
    canvas_draw_vline(c, tmp_p.x - (c->font_size * lb_str.size / 2), tmp_p.y, 6);
  }

  canvas_set_text_align(c, ALIGN_H_LEFT, ALIGN_V_MIDDLE);
  i = 0;
  for (tmp_p = org_p; tmp_p.y >= org_p.y - y_axis_len; tmp_p.y -= y_axis_len / y_div_part) {
    if (i != 0) {
      wstr_t lb_str;
      rect_t r;
      wstr_init(&lb_str, 0);
      wstr_from_int(&lb_str, i);
      r = rect_init(tmp_p.x - 6 - tk_max(c->font_size * (lb_str.size - 1), 0),
                    tmp_p.y - c->font_size / 2, c->font_size * lb_str.size, c->font_size);
      canvas_draw_text_in_rect(c, lb_str.str, lb_str.size, &r);
      wstr_reset(&lb_str);
      canvas_draw_hline(c, tmp_p.x - 6, tmp_p.y, 6);
    }
    i += 10;
  }

  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_set_line_width(vg, 2);
  vgcanvas_set_stroke_color(vg, color_line);
  for (i = 0; i < ARRAY_SIZE(data_p); i++) {
    tr_point[i].x = y_axis_offset + x_axis_len * data_p[i].x / x_div_part;
    tr_point[i].y = widget->h - (x_axis_offset + y_axis_len * data_p[i].y / (y_div_part * 10));

    if (i == 0) {
      vgcanvas_move_to(vg, tr_point[i].x, tr_point[i].y);
    } else {
      vgcanvas_line_to(vg, tr_point[i].x, tr_point[i].y);
    }
  }
  vgcanvas_stroke(vg);

  vgcanvas_set_fill_color(vg, color_point);
  for (i = 0; i < ARRAY_SIZE(tr_point); i++) {
    vgcanvas_ellipse(vg, tr_point[i].x, tr_point[i].y, 4, 4);
    vgcanvas_fill(vg);
  }

  vgcanvas_set_fill_color(vg, color_point_bg);
  for (i = 0; i < ARRAY_SIZE(tr_point); i++) {
    vgcanvas_ellipse(vg, tr_point[i].x, tr_point[i].y, 2, 2);
    vgcanvas_fill(vg);
  }

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t page_chart_init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  (void)ctx;

  if (widget->name != NULL) {
    const char* name = widget->name;

    if (tk_str_eq(name, "paint_line_chart")) {
      widget_on(widget, EVT_PAINT, paint_line_chart, NULL);
    }
  }

  return RET_OK;
}

static ret_t page_chart_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);
  return widget_foreach(page, page_chart_init_widget, NULL);
}

/*** page_window **********************************************************************/
static ret_t page_window_init(widget_t* page) {
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);
  return widget_foreach(page, common_init_widget, NULL);
}

/*** main **********************************************************************/
typedef ret_t (*page_enter_func_t)(widget_t* page);

static page_enter_func_t s_page_enter_func_array[PAGE_NUM] = {NULL};

static const char* preload_info[] = {"Time init...", "Memory init...", "System info init...",
                                     "Reource to load..."};

static ret_t fix_widget_size(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* root = WIDGET(ctx);
  widget_t* parent = widget->parent;

  if (widget == root) {
    return RET_OK;
  }

  if (widget->x + widget->w > root->w) {
    while (parent != NULL) {
      scroll_view_t* sv_parent = (scroll_view_t*)parent;

      if (tk_str_eq(WIDGET_TYPE_SCROLL_VIEW, widget_get_type(parent)) && sv_parent->xslidable) {
        break;
      }
      parent = parent->parent;
    }
    if (parent == NULL) {
      widget_resize(widget, root->w - widget->x - 20, widget->h);
    }
  } else if (tk_str_eq(WIDGET_TYPE_GRID, widget_get_type(widget)) ||
             tk_str_eq(WIDGET_TYPE_ROW, widget_get_type(widget))) {
    bool_t want_to_fix = FALSE;

    if (widget->w != 0) {
      WIDGET_FOR_EACH_CHILD_BEGIN(widget, child, i)
      if (child->w == 0 || child->h == 0) {
        want_to_fix = TRUE;
      }
      WIDGET_FOR_EACH_CHILD_END()
    }

    if (want_to_fix) {
      value_t v;
      uint16_t real_w, real_h, real_row, w, h = 0;
      uint8_t x_margin, y_margin, spacing = 0;
      uint32_t visible_children_num = 0;

      WIDGET_FOR_EACH_CHILD_BEGIN(widget, child, i)
      if (widget_get_visible(child)) {
        visible_children_num++;
      }
      WIDGET_FOR_EACH_CHILD_END()

      children_layouter_get_param(widget->children_layout, "w", &v);
      w = value_uint16(&v);
      children_layouter_get_param(widget->children_layout, "h", &v);
      h = value_uint16(&v);
      children_layouter_get_param(widget->children_layout, "x", &v);
      x_margin = value_uint8(&v);
      children_layouter_get_param(widget->children_layout, "y", &v);
      y_margin = value_uint8(&v);
      children_layouter_get_param(widget->children_layout, "s", &v);
      spacing = value_uint8(&v);

      real_w = 2 * x_margin + tk_max((int32_t)((w + spacing) * visible_children_num - spacing), 0);

      real_row = tk_min(
          visible_children_num,
          tk_roundi(tk_max((1.0 * real_w / widget->w) + 0.5 * ((real_w % widget->w) != 0), 1)));
      real_h = 2 * y_margin + real_row * (h + spacing) - spacing;

      if (tk_str_eq(WIDGET_TYPE_GRID, widget_get_type(widget->parent)) ||
          tk_str_eq(WIDGET_TYPE_ROW, widget_get_type(widget->parent))) {
        widget_resize(widget->parent, widget->parent->w, widget->parent->h + real_h - widget->h);
      }
      widget_resize(widget, root->w, real_h);
    }
  }

  return RET_OK;
}

static ret_t change_menu_title(widget_t* win, widget_t* page) {
  widget_t* menu_ti = widget_lookup(win, "lb_menu_ti", TRUE);
  return_value_if_fail(menu_ti != NULL && page != NULL, RET_BAD_PARAMS);

  return widget_set_tr_text(menu_ti, page->tr_text);
}

#if defined(ENABLE_PAGE_SWITCH) && ENABLE_PAGE_SWITCH
#define PAGE_SWITCH_THRESHOLD 100
#define PAGE_SWITCH_PROP_ENABLE "page_switch_enable"

static ret_t on_page_switch_before(void* ctx, event_t* e) {
  widget_t* page = WIDGET(ctx);
  scroll_view_t* scroll_view = NULL;
  pointer_event_t* pe = (pointer_event_t*)e;
  int32_t index = -1;
  int32_t move_dy = 0;
  int32_t y = 0;
  return_value_if_fail(page != NULL && pe != NULL, RET_BAD_PARAMS);

  scroll_view = SCROLL_VIEW(widget_lookup_by_type(page, WIDGET_TYPE_SCROLL_VIEW, TRUE));
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  index = widget_index_of(page);

  move_dy = scroll_view->down.y - pe->y;
  y = scroll_view->yoffset_save + move_dy;

  if (scroll_view->pressed &&
      ((0 < index && y < -PAGE_SWITCH_THRESHOLD) ||
       (index < PAGE_NUM - 1 &&
        y + scroll_view->widget.h > scroll_view->virtual_h + PAGE_SWITCH_THRESHOLD))) {
    scroll_view->pressed = FALSE;
    widget_set_prop_bool(&scroll_view->widget, PAGE_SWITCH_PROP_ENABLE, TRUE);
  }

  return RET_OK;
}

static ret_t on_page_switch(void* ctx, event_t* e) {
  widget_t* page = WIDGET(ctx);
  widget_t* win = NULL;
  widget_t* pages = NULL;
  widget_t* view_menu = NULL;
  scroll_view_t* scroll_view = SCROLL_VIEW(e->target);
  pointer_event_t* pe = (pointer_event_t*)e;
  int32_t old_index = -1;
  int32_t new_index = -1;
  int32_t move_dy = 0;
  int32_t y = 0;
  ret_t ret = RET_SKIP;
  bool_t switch_enable = FALSE;
  return_value_if_fail(page != NULL && pe != NULL && scroll_view != NULL, RET_BAD_PARAMS);

  switch_enable = widget_get_prop_bool(&scroll_view->widget, PAGE_SWITCH_PROP_ENABLE, FALSE);
  if (!switch_enable) {
    return ret;
  }

  win = widget_get_window(page);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);
  view_menu = widget_lookup(win, "view_menu", TRUE);

  pages = page->parent;
  return_value_if_fail(pages != NULL, RET_BAD_PARAMS);

  old_index = new_index = widget_index_of(page);

  move_dy = scroll_view->down.y - pe->y;
  y = scroll_view->yoffset_save + move_dy;

  if (y < -PAGE_SWITCH_THRESHOLD) {
    new_index--;
  } else if (y + scroll_view->widget.h > scroll_view->virtual_h + PAGE_SWITCH_THRESHOLD) {
    new_index++;
  }
  new_index = tk_min(tk_max(0, new_index), PAGE_NUM - 1);

  if (new_index != old_index) {
    if (view_menu != NULL) {
      widget_t* new_page = widget_get_child(pages, new_index);
      if (new_page != NULL) {
        char tb_name[32] = {0};
        widget_t* tb = NULL;
        tk_snprintf(tb_name, ARRAY_SIZE(tb_name), "tb_%s",
                    new_page->name + 5); /* page_xxx -> tb_xxx */
        tb = widget_lookup(view_menu, tb_name, TRUE);
        widget_set_value(tb, TRUE);
      }
    } else {
      pages_set_active(pages, new_index);
    }
    ret = RET_STOP;
  }
  widget_set_prop_bool(&scroll_view->widget, PAGE_SWITCH_PROP_ENABLE, FALSE);
  return ret;
}
#endif /* ENABLE_PAGE_SWITCH */

static ret_t on_page_enter(void* ctx, event_t* e) {
#if defined(ENABLE_PAGE_SWITCH) && ENABLE_PAGE_SWITCH
  widget_t* scroll_view = NULL;
#endif /* ENABLE_PAGE_SWITCH */
  widget_t* page = WIDGET(e->target);
  page_enter_func_t enter_func = (page_enter_func_t)ctx;
  return_value_if_fail(page != NULL, RET_BAD_PARAMS);

  change_menu_title(widget_get_window(page), page);

  widget_foreach(page, fix_widget_size, page);

#if defined(ENABLE_PAGE_SWITCH) && ENABLE_PAGE_SWITCH
  scroll_view = widget_lookup_by_type(page, WIDGET_TYPE_SCROLL_VIEW, TRUE);
  if (scroll_view != NULL) {
    widget_on(scroll_view, EVT_POINTER_UP_BEFORE_CHILDREN, on_page_switch_before, page);
    widget_on(scroll_view, EVT_POINTER_UP, on_page_switch, page);
  }
#endif /* ENABLE_PAGE_SWITCH */

  if (enter_func) {
    enter_func(page);
  }
  return RET_OK;
}

static ret_t on_menu_bar_open(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* top_win = window_manager_get_top_window(window_manager());
  widget_t* vpages = widget_lookup(win, "vpages", TRUE);

  return_value_if_fail(vpages != NULL, RET_BAD_PARAMS);

  if (!(top_win == win || widget_is_overlay(top_win))) {
    if (widget_is_dialog(top_win) && dialog_is_modal(top_win)) {
      dialog_quit(top_win, DIALOG_QUIT_CANCEL);
    } else {
      window_close(top_win);
    }
  }

  return open_menu_bar(vpages);
}

#define WIDGET_PROP_DISABLE_IN_1M "disable_in_1m"
#define WIDGET_PROP_INVISIBLE_IN_1M "invisible_in_1m"
static ret_t on_adject_vpage_in_1m_assets(void* ctx, event_t* e);
static ret_t adject_widget_in_1m_assets(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  bool_t invisible_in_1m = widget_get_prop_bool(widget, WIDGET_PROP_INVISIBLE_IN_1M, FALSE);

  if (invisible_in_1m) {
    widget_set_visible(widget, FALSE);
    widget_set_sensitive(widget, FALSE);
  } else {
    bool_t disable_in_1m = widget_get_prop_bool(widget, WIDGET_PROP_DISABLE_IN_1M, FALSE);
    if (disable_in_1m) {
      widget_set_enable(widget, FALSE);
    }

    if (tk_str_eq(widget->name, "vpages")) {
      WIDGET_FOR_EACH_CHILD_BEGIN(widget, page, i)
      widget_on(page, EVT_VPAGE_WILL_OPEN, on_adject_vpage_in_1m_assets, NULL);
      WIDGET_FOR_EACH_CHILD_END()
    }
  }

  return RET_OK;
}

static ret_t on_adject_vpage_in_1m_assets(void* ctx, event_t* e) {
  widget_t* vpage = WIDGET(e->target);
  return_value_if_fail(vpage != NULL, RET_BAD_PARAMS);
  return widget_foreach(vpage, adject_widget_in_1m_assets, NULL);
}

static ret_t on_adject_win_in_1m_assets(void* ctx, event_t* e) {
  window_event_t* we = window_event_cast(e);
  return_value_if_fail(we != NULL, RET_BAD_PARAMS);

  return widget_foreach(we->window, adject_widget_in_1m_assets, NULL);
}

static bool_t check_is_use_1m_assets(void) {
  const asset_info_t* res = assets_manager_ref(assets_manager(), ASSET_TYPE_IMAGE, "logo_dynamic");
  bool_t is_use_1m_assets = (res == NULL);

  if (res != NULL) {
    assets_manager_unref(assets_manager(), res);
  }
  return is_use_1m_assets;
}

static ret_t vpage_change_kb_type_without_zh(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  const widget_vtable_t* vt = widget->vt;

  while (vt != NULL) {
    if (tk_str_eq(vt->type, WIDGET_TYPE_EDIT)) {
      edit_t* edit = EDIT(widget);
      if (edit != NULL && tk_str_eq(edit->keyboard, WINDOW_NAME_PREFIX "kb_default")) {
        edit_set_keyboard(widget, WINDOW_NAME_PREFIX "kb_ascii");
      }
      break;
    } else if (tk_str_eq(vt->type, WIDGET_TYPE_MLEDIT)) {
      mledit_t* mledit = MLEDIT(widget);
      if (mledit != NULL && tk_str_eq(mledit->keyboard, WINDOW_NAME_PREFIX "kb_default")) {
        mledit_set_keyboard(widget, WINDOW_NAME_PREFIX "kb_ascii");
      }
      break;
    }
    vt = (vt->parent != NULL) ? vt->parent : NULL;
  }

  return RET_OK;
}

static ret_t on_vpage_change_kb_type_without_zh(void* ctx, event_t* e) {
  widget_t* vpage = WIDGET(e->target);
  return_value_if_fail(vpage != NULL, RET_BAD_PARAMS);
  return widget_foreach(vpage, vpage_change_kb_type_without_zh, NULL);
}

static ret_t on_change_kb_type_without_zh(void* ctx, event_t* e) {
  window_event_t* we = window_event_cast(e);
  return_value_if_fail(we != NULL && we->window != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(we->window, iter, i)
  if (tk_str_eq(iter->name, "vpages")) {
    WIDGET_FOR_EACH_CHILD_BEGIN(iter, page, i)
    widget_on(page, EVT_VPAGE_WILL_OPEN, on_vpage_change_kb_type_without_zh, NULL);
    WIDGET_FOR_EACH_CHILD_END()
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

/**
 * 子控件初始化(主要是设置click回调、初始显示信息)
 */
static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = WIDGET(ctx);

  if (widget->name != NULL) {
    const char* name = widget->name;
    /* system_bar */
    if (tk_str_eq(name, "rb_dark")) {
      widget_set_value(widget, tk_str_eq(assets_manager()->theme, "dark"));
      widget_on(widget, EVT_VALUE_CHANGED, on_change_theme, NULL);
    } else if (tk_str_eq(name, "rb_zh")) {
      if (s_is_full_font) {
        widget_set_value(widget, tk_str_eq(locale_info()->language, "zh"));
        widget_on(widget, EVT_VALUE_CHANGED, on_language_changed, NULL);
      } else {
        widget_set_value(widget, FALSE);
        widget_set_enable(widget->parent, FALSE);
      }
    } else if (tk_str_eq(name, "btn_menu_on")) {
      widget_on(widget, EVT_CLICK, on_menu_bar_open, win);
    } else if (tk_str_eq(name, "img_logo")) {
      widget_t* sys_bar = widget->parent;
      uint32_t x = sys_bar->w;
      WIDGET_FOR_EACH_CHILD_BEGIN(sys_bar, child, i)
      if (child != widget && !tk_str_eq(child->name, "btn_menu_on")) {
        x = tk_min(child->x, x);
      }
      WIDGET_FOR_EACH_CHILD_END()
      if (x < widget->x + widget->w) {
        widget_set_visible(widget, FALSE);
        widget_set_sensitive(widget, FALSE);
      }
    }
    /* win */
    else if (tk_str_eq(name, "vpages")) {
      WIDGET_FOR_EACH_CHILD_BEGIN(widget, page, i)
      widget_on(page, EVT_VPAGE_WILL_OPEN, on_page_enter, s_page_enter_func_array[i]);
      WIDGET_FOR_EACH_CHILD_END()
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

#if defined(PRELOAD_DURATION) && PRELOAD_DURATION > 0
static ret_t timer_preload(const timer_info_t* timer) {
  widget_t* win_preload = WIDGET(timer->ctx);
  widget_t* lb_info = widget_lookup(win_preload, "lb_info", TRUE);
  widget_t* lb_progress_c = widget_lookup(win_preload, "lb_progress_c", TRUE);
  widget_t* progress_circle = widget_lookup(lb_progress_c, "progress_circle", TRUE);
  static uint32_t cur_time = 0;
  static uint32_t cur_item = 0;
  uint32_t progress_value = cur_time * 10 * 100 / PRELOAD_DURATION;
  char num_str[8] = {0};

  tk_snprintf(num_str, ARRAY_SIZE(num_str), "%u%%", progress_value / 10);
  if (progress_circle != NULL) {
    widget_set_value(progress_circle, progress_value);
  }
  widget_set_text_utf8(lb_progress_c, num_str);

  if (cur_time >= cur_item * PRELOAD_DURATION / ARRAY_SIZE(preload_info)) {
    char ani_str[128] = {0};
    tk_snprintf(ani_str, ARRAY_SIZE(ani_str),
                "opacity(duration=%d,yoyo_times=1,easing=sin_inout,from=0,to=255)",
                tk_max(PRELOAD_DURATION / ARRAY_SIZE(preload_info), 0));
    widget_set_tr_text(lb_info, preload_info[cur_item++]);
    widget_animator_create(lb_info, ani_str);
  }

  cur_time += timer->duration;

  if (cur_time >= PRELOAD_DURATION) {
    WIDGET_FOR_EACH_CHILD_BEGIN(window_manager(), root, i)
    if (root != win_preload) {
      widget_set_visible(root, TRUE);
    }
    WIDGET_FOR_EACH_CHILD_END()
    window_close(win_preload);
    return RET_REMOVE;
  }
  return RET_REPEAT;
}

static widget_t* show_preload_window(void) {
  widget_t* preload = window_open_with_prefix(PRELOAD_NAME);
  widget_add_timer(preload, timer_preload, 20);
  return preload;
}
#endif /* PRELOAD_DURATION > 0 */

static ret_t set_win_name(void) {
  bool_t is_portrait = FALSE;
  widget_t* wm = window_manager();
  return_value_if_fail(wm != NULL, RET_FAIL);

#ifdef WITH_LCD_PORTRAIT
  is_portrait = TRUE;
#elif WITH_LCD_LANDSCAPE
  is_portrait = (wm->h <= LANDSCAPE_WIDTH_THRESHOLD) ? TRUE : FALSE;
#else
  is_portrait = (wm->w <= LANDSCAPE_WIDTH_THRESHOLD || wm->w < wm->h) ? TRUE : FALSE;
#endif /* WITH_LCD_PORTRAIT && WITH_LCD_LANDSCAPE */

  if (is_portrait) {
    s_win_name = "menu_portrait";
    s_sysbar_name = "sysbar_portrait";
  } else {
    s_win_name = "menu_landscape";
    s_sysbar_name = "sysbar_landscape";
  }

  return RET_OK;
}

/**
 * 初始化
 */
ret_t application_init(void) {
  widget_t* win = NULL;
  set_win_name();

  if (check_is_use_1m_assets()) {
    widget_on(window_manager(), EVT_WINDOW_WILL_OPEN, on_adject_win_in_1m_assets, NULL);
  }

#if !defined(WITH_SDL) && !defined(LINUX)
  widget_factory_register(widget_factory(), "vpage", vpage_create);
#endif /* WITH_SDL && LINUX */

#ifdef APP_DEFAULT_FONT
  if (tk_str_eq(APP_DEFAULT_FONT, "default_full")) {
    s_is_full_font = TRUE;
  }
#endif /* APP_DEFAULT_FONT */

  if (!s_is_full_font) {
    widget_on(window_manager(), EVT_WINDOW_WILL_OPEN, on_change_kb_type_without_zh, NULL);
  }

#ifdef APP_DEFAULT_LANGUAGE
  if (!tk_str_eq(APP_DEFAULT_LANGUAGE, "zh") && !tk_str_eq(APP_DEFAULT_LANGUAGE, "en")) {
    log_debug("not support language:%s!\n", APP_DEFAULT_LANGUAGE);
    change_language(FALSE);
  } else {
    change_language(is_full_font && tk_str_eq(APP_DEFAULT_LANGUAGE, "zh"));
  }
#else
  change_language(FALSE);
#endif /* APP_DEFAULT_LANGUAGE */

  s_page_enter_func_array[0] = page_button_init;
  s_page_enter_func_array[3] = page_label_init;
  s_page_enter_func_array[6] = page_slider_init;
  s_page_enter_func_array[8] = page_image_init;
  s_page_enter_func_array[9] = page_mledit_init;
  s_page_enter_func_array[12] = page_tab_ctrl_init;
  s_page_enter_func_array[13] = page_color_init;
  s_page_enter_func_array[14] = page_animate_init;
  s_page_enter_func_array[16] = page_chart_init;
  s_page_enter_func_array[17] = page_window_init;

  window_open_with_prefix(s_sysbar_name);
  win = window_open_with_prefix(s_win_name);

  WIDGET_FOR_EACH_CHILD_BEGIN(window_manager(), root, i)
  widget_set_visible_only(root, FALSE);
  init_children_widget(root, (void*)win);
  WIDGET_FOR_EACH_CHILD_END()

#if defined(PRELOAD_DURATION) && PRELOAD_DURATION > 0
  show_preload_window();
#endif /* PRELOAD_DURATION > 0 */

  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void) {
  log_debug("application_exit\n");
  tk_mem_dump();
  return RET_OK;
}

#include "awtk_main.inc"

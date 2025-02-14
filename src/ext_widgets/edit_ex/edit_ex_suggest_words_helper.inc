/**
 * File:   edit_ex_suggest_words_helper.inc
 * Author: AWTK Develop Team
 * Brief:  edit_ex输入建议帮助函数
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
 * 2025-02-13 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_EDIT_EX_SUGGEST_WORDS_HELPER_INC
#define TK_EDIT_EX_SUGGEST_WORDS_HELPER_INC

#include "tkc/types_def.h"

BEGIN_C_DECLS

#include "base/window.h"
#include "base/window_manager.h"
#include "widgets/popup.h"
#include "widgets/combo_box_item.h"
#include "ext_widgets/edit_ex/edit_ex.h"
#include "ext_widgets/scroll_view/list_view.h"
#include "ext_widgets/scroll_view/scroll_view.h"
#include "ext_widgets/scroll_view/scroll_bar.h"

#define EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN 1
#define EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR 5

static ret_t edit_ex_suggest_words_popup_on_open(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  widget_t* window = WIDGET(e->target);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  widget_set_sensitive(window, TRUE);

  return RET_OK;
}

static ret_t edit_ex_suggest_words_popup_on_close(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  edit_ex->suggest_words_popup = NULL;

  return RET_OK;
}

static ret_t edit_ex_widget_set_prop_by_object_on_visit(void* ctx, const void* data) {
  widget_t* widget = WIDGET(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  return_value_if_fail(widget != NULL && nv != NULL, RET_BAD_PARAMS);

  widget_set_prop(widget, nv->name, &nv->value);

  return RET_OK;
}

static widget_t* edit_ex_create_suggest_words_popup(widget_t* widget) {
  /* base on combo_box_ex_create_scroll_popup() */
  int32_t w = 0;
  int32_t h = 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
  int32_t item_height = 0;
  const char* applet_name = NULL;
  widget_t* win = NULL;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  tk_object_t* obj_props = NULL;
  return_value_if_fail(edit_ex != NULL, NULL);

  w = widget->w;
  item_height = widget->h;
  win = popup_create(NULL, 0, 0, w, h);
  widget_set_name(win, EDIT_EX_SUGGEST_WORDS_UI_NAME_POPUP);
  applet_name = widget_get_prop_str(win, WIDGET_PROP_APPLET_NAME, NULL);
  widget_set_prop_str(win, WIDGET_PROP_APPLET_NAME, applet_name);

  widget_set_prop_bool(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, TRUE);

  widget_use_style(win, "combobox_popup");

  widget_set_prop_str(win, WIDGET_PROP_THEME, "combobox_ex_popup");

  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_PREV_KEY, "up");
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY, "down");

  obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, win->name);
  tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, win);

  { /* children */
    char param[128] = {'\0'};
    widget_t* list_view = NULL;
    widget_t* vbar = NULL;
    widget_t* scroll_view = NULL;
    // create list view
    list_view = list_view_create(win, 0, 0, 0, 0);
    widget_set_name(list_view, EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW);
    widget_set_prop_int(list_view, WIDGET_PROP_ITEM_HEIGHT, item_height);
    widget_set_prop_bool(list_view, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR, TRUE);

    tk_snprintf(param, sizeof(param) - 1, "default(x=%d,y=%d,w=%d,h=%d)",
                EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN, EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN,
                -2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN,
                -2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN);
    widget_set_self_layout(list_view, param);

    obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, list_view->name);
    tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, list_view);
    // create vbar
    vbar = scroll_bar_create(list_view, 0, 0, 12, 0);
    widget_set_name(vbar, EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR);

    tk_snprintf(param, sizeof(param) - 1, "default(x=r,y=0,w=%d,h=100%%)", vbar->w);
    widget_set_self_layout(vbar, param);

    obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, vbar->name);
    tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, vbar);
    // create scroll view
    scroll_view = scroll_view_create(list_view, 0, 0, -vbar->w, 0);
    widget_set_name(scroll_view, EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW);

    tk_snprintf(param, sizeof(param) - 1, "default(x=0,y=0,w=%d,h=100%%)", scroll_view->w);
    widget_set_self_layout(scroll_view, param);

    obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, scroll_view->name);
    tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, scroll_view);
  }

  widget_set_focused_internal(widget, TRUE);

  return win;
}

static ret_t edit_ex_suggest_words_popup_confirm(widget_t* widget, widget_t* item) {
  /* base on combo_box_on_item_click() */
  edit_ex_t* edit_ex = EDIT_EX(widget);
  ENSURE(edit_ex);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (item != NULL) {
    widget_set_text(widget, widget_get_text(item));
  }

  widget->target = NULL;
  widget->key_target = NULL;
  window_close(edit_ex->suggest_words_popup);
  widget_set_focused_internal(widget, TRUE);

  return RET_OK;
}

static ret_t edit_ex_suggest_words_popup_on_item_click(void* ctx, event_t* e) {
  widget_t* item = WIDGET(e->target);
  widget_t* widget = WIDGET(ctx);
  return_value_if_fail(widget != NULL && item != NULL, RET_BAD_PARAMS);

  return edit_ex_suggest_words_popup_confirm(widget, item);
}

static ret_t edit_ex_suggest_words_focus_by_key(widget_t* widget, int32_t key) {
  ret_t ret = RET_SKIP;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (edit_ex->suggest_words_popup != NULL) {
    if (key == TK_KEY_DOWN) {
      ret = widget_focus_first(edit_ex->suggest_words_popup);
    }
  }

  return ret;
}

typedef struct _edit_ex_update_suggest_words_popup_ctx_t {
  edit_ex_t* edit_ex;
  widget_t* scroll_view;
  uint32_t curr_index;
} edit_ex_update_suggest_words_popup_ctx_t;

static ret_t edit_ex_update_suggest_words_popup_on_item_visit(void* ctx, const void* data) {
  /* base on combo_box_visit_item() */
  edit_ex_update_suggest_words_popup_ctx_t* actx = (edit_ex_update_suggest_words_popup_ctx_t*)(ctx);
  widget_t* iter = WIDGET(data);
  return_value_if_fail(actx != NULL && iter != NULL, RET_BAD_PARAMS);
  return_value_if_fail(actx->edit_ex != NULL && actx->scroll_view != NULL, RET_BAD_PARAMS);

  if (iter == actx->scroll_view) {
    return RET_OK;
  }

  if (actx->curr_index != 0) {
    widget_set_visible(iter, TRUE);
    widget_set_sensitive(iter, TRUE);
    if (iter->emitter == NULL ||
        !emitter_exist(iter->emitter, EVT_CLICK, edit_ex_suggest_words_popup_on_item_click,
                       actx->edit_ex)) {
      widget_on(iter, EVT_CLICK, edit_ex_suggest_words_popup_on_item_click, actx->edit_ex);
    }
    actx->curr_index--;
  } else {
    widget_set_sensitive(iter, FALSE);
    widget_set_visible(iter, FALSE);
  }

  return RET_OK;
}

static ret_t edit_ex_update_suggest_words_popup_on_visit(void* ctx, const void* data) {
  edit_ex_update_suggest_words_popup_ctx_t* actx = (edit_ex_update_suggest_words_popup_ctx_t*)(ctx);
  const value_t* v = (const value_t*)data;
  const char* suggest = NULL;
  widget_t* item = NULL;
  return_value_if_fail(actx != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(actx->scroll_view != NULL, RET_BAD_PARAMS);

  suggest = value_str(v);
  return_value_if_fail(suggest != NULL, RET_FAIL);

  item = widget_get_child(actx->scroll_view, actx->curr_index);
  if (item == NULL) {
    item = combo_box_item_create(actx->scroll_view, 0, 0, 0, 0);
    return_value_if_fail(item != NULL, RET_OOM);
  }
  widget_set_text_utf8(item, suggest);
  actx->curr_index++;

  return RET_OK;
}

static ret_t edit_ex_update_suggest_words_popup(widget_t* widget) {
  /* base on combo_box_ex_create_scroll_popup() */
  int32_t w = 0;
  int32_t h = 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
  int32_t nr = 0;
  int32_t item_height = 0;
  widget_t* list_view = NULL;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  edit_ex_update_suggest_words_popup_ctx_t update_ctx = {
      .edit_ex = edit_ex,
  };
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);
  return_value_if_fail(edit_ex->suggest_words != NULL && edit_ex->suggest_words_popup != NULL,
                       RET_FAIL);

  list_view =
      widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW, TRUE);
  return_value_if_fail(list_view != NULL, RET_FAIL);

  update_ctx.scroll_view =
      widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW, TRUE);
  return_value_if_fail(update_ctx.scroll_view != NULL, RET_FAIL);

  item_height = widget_get_prop_int(list_view, WIDGET_PROP_ITEM_HEIGHT, widget->h);
  nr = tk_object_get_prop_int(edit_ex->suggest_words, TK_OBJECT_PROP_SIZE, 0);
  if (nr <= EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR) {
    h = nr * item_height + 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
  } else {
    h = EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR * item_height +
        2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
  }
  w = tk_max_int(widget->w,
                 widget_get_prop_int(edit_ex->suggest_words_popup, WIDGET_PROP_MIN_W, 0));

  tk_object_foreach_prop(edit_ex->suggest_words, edit_ex_update_suggest_words_popup_on_visit,
                         &update_ctx);
  widget_foreach(update_ctx.scroll_view, edit_ex_update_suggest_words_popup_on_item_visit,
                 &update_ctx);

  widget_resize(edit_ex->suggest_words_popup, w, h);

  return RET_OK;
}

static ret_t edit_ex_suggest_words_popup_calc_position(widget_t* widget, point_t* p) {
  /* base on combo_box_combobox_popup_calc_position() */
  widget_t* wm = window_manager();
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(wm != NULL && edit_ex != NULL && edit_ex->suggest_words_popup != NULL,
                       RET_BAD_PARAMS);

  *p = point_init(0, 0);
  widget_to_screen(widget, p);

  if ((p->y + widget->h + edit_ex->suggest_words_popup->h) < wm->h) {
    p->y += widget->h;
  } else if (p->y >= edit_ex->suggest_words_popup->h) {
    p->y -= edit_ex->suggest_words_popup->h;
  } else {
    p->y = 0;
  }

  return RET_OK;
}

static ret_t edit_ex_update_suggest_words(widget_t* widget, const char* input) {
  ret_t ret = RET_OK;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  value_change_event_t evt;
  point_t p;
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (edit_ex->suggest_words == NULL) {
    goto close;
  }

  /* 通知 suggest_words 更新 */
  value_change_event_init(&evt, EVT_VALUE_CHANGED, NULL);
  value_set_str(&evt.new_value, input);
  ret = emitter_dispatch(EMITTER(edit_ex->suggest_words), &evt.e);
  return_value_if_fail(RET_OK == ret, ret);

  if (tk_object_get_prop_int(edit_ex->suggest_words, TK_OBJECT_PROP_SIZE, 0) == 0) {
    goto close;
  }

  if (edit_ex->suggest_words_popup == NULL) {
    edit_ex->suggest_words_popup = edit_ex_create_suggest_words_popup(widget);
    return_value_if_fail(edit_ex->suggest_words_popup != NULL, RET_FAIL);

    /* 为了焦点不移动到 suggest_words_popup 上，所以打开前先将 sensitive 设置为 FALSE，打开窗口后再设置为 TRUE。 */
    widget_set_sensitive(edit_ex->suggest_words_popup, FALSE);
    widget_on(edit_ex->suggest_words_popup, EVT_WINDOW_OPEN, edit_ex_suggest_words_popup_on_open,
              widget);

    widget_on(edit_ex->suggest_words_popup, EVT_WINDOW_CLOSE, edit_ex_suggest_words_popup_on_close,
              widget);
  }

  edit_ex_update_suggest_words_popup(widget);
  widget_layout(edit_ex->suggest_words_popup);

  if (RET_OK == edit_ex_suggest_words_popup_calc_position(widget, &p)) {
    widget_move(edit_ex->suggest_words_popup, p.x, p.y);
  }

  return ret;
close:
  if (edit_ex->suggest_words_popup != NULL) {
    window_close(edit_ex->suggest_words_popup);
  }
  return RET_OK;
}

static ret_t edit_ex_show_suggest_words(widget_t* widget) {
  ret_t ret = RET_OK;
  str_t str;
  str_init(&str, 0);
  str_from_wstr(&str, widget_get_text(widget));

  ret = edit_ex_update_suggest_words(widget, str.str);

  str_reset(&str);
  return ret;
}

END_C_DECLS

#endif /*TK_EDIT_EX_SUGGEST_WORDS_HELPER_INC*/

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

#include "tkc/named_value.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "widgets/popup.h"
#include "ext_widgets/edit_ex/edit_ex.h"
#include "ext_widgets/scroll_view/list_view.h"
#include "ext_widgets/scroll_view/scroll_view.h"
#include "ext_widgets/scroll_view/scroll_bar.h"

#include "ext_widgets/edit_ex/edit_ex_suggest_words_item_formats_parse.inc"

#define EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN 1
#define EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR 5
#define EDIT_SUGGEST_WORDS_SCROLL_BAR_DEFAULT_WH 12

static ret_t edit_ex_suggest_words_popup_on_open(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  widget_t* window = WIDGET(e->target);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  return widget_set_sensitive(window, !edit_ex->edit.is_key_inputing);
}

static ret_t edit_ex_suggest_words_popup_on_close(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  edit_ex->suggest_words_popup = NULL;

  return RET_OK;
}

static ret_t edit_ex_suggest_words_popup_on_key_down_before_children(void* ctx, event_t* e) {
  edit_ex_t* edit_ex = EDIT_EX(ctx);
  key_event_t* evt = key_event_cast(e);
  return_value_if_fail(edit_ex != NULL && evt != NULL, RET_BAD_PARAMS);

  switch (evt->key) {
    /* 删除 */
    case TK_KEY_DELETE:
    case TK_KEY_BACKSPACE:
    /* 移动光标 */
    case TK_KEY_LEFT:
    case TK_KEY_RIGHT: {
      /* 转抛给 edit_ex */
      key_event_t clone_evt = *evt;

      clone_evt.e.type = EVT_KEY_DOWN;
      widget_dispatch(WIDGET(edit_ex), &clone_evt.e);

      clone_evt.e.type = EVT_KEY_UP;
      widget_dispatch(WIDGET(edit_ex), &clone_evt.e);

      return RET_STOP;
    } break;
    default: {
    } break;
  }

  return RET_OK;
}

inline static bool_t edit_ex_widget_set_prop_is_support(const char* name) {
  const char* not_support_props[] = {
      WIDGET_PROP_NAME,
  };
  uint32_t i = 0;
  for (i = 0; i < ARRAY_SIZE(not_support_props); i++) {
    if (tk_str_eq(name, not_support_props[i])) {
      return FALSE;
    }
  }
  return TRUE;
}

static ret_t edit_ex_widget_set_prop_by_object_on_visit(void* ctx, const void* data) {
  widget_t* widget = WIDGET(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  return_value_if_fail(widget != NULL && nv != NULL, RET_BAD_PARAMS);

  if (edit_ex_widget_set_prop_is_support(nv->name)) {
    widget_set_prop(widget, nv->name, &nv->value);
  } else {
    log_error("%s : Not support set prop \"%s\"!\n", __FUNCTION__, nv->name);
  }

  return RET_OK;
}

inline static widget_t* edit_ex_create_suggest_words_hbar_create(edit_ex_t* edit_ex,
                                                                 widget_t* popup) {
  tk_object_t* obj_props = NULL;
  char param[128] = {'\0'};
  widget_t* list_view = widget_lookup(popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW, TRUE);
  widget_t* hbar = scroll_bar_create(list_view, 0, 0, -EDIT_SUGGEST_WORDS_SCROLL_BAR_DEFAULT_WH,
                                     EDIT_SUGGEST_WORDS_SCROLL_BAR_DEFAULT_WH);

  tk_snprintf(param, sizeof(param) - 1, "default(x=0,y=b,w=%d,h=%d)", hbar->w, hbar->h);
  widget_set_self_layout(hbar, param);

  widget_set_name(hbar, EDIT_EX_SUGGEST_WORDS_UI_NAME_HBAR);
  obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, hbar->name);
  tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, hbar);

  return hbar;
}

inline static ret_t edit_ex_create_suggest_words_vbar_init(edit_ex_t* edit_ex, widget_t* vbar,
                                                           widget_t* popup) {
  tk_object_t* obj_props = NULL;
  char param[128] = {'\0'};
  widget_t* hbar = widget_lookup(popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_HBAR, TRUE);

  if (hbar != NULL) {
    int32_t hbar_h = widget_get_prop_int(hbar, WIDGET_PROP_LAYOUT_H, hbar->h);
    widget_resize(vbar, hbar_h, -hbar_h);
    tk_snprintf(param, sizeof(param) - 1, "default(x=r,y=0,w=%d,h=%d)", vbar->w, vbar->h);
  } else {
    widget_resize(vbar, EDIT_SUGGEST_WORDS_SCROLL_BAR_DEFAULT_WH, vbar->h);
    tk_snprintf(param, sizeof(param) - 1, "default(x=r,y=0,w=%d,h=100%%)", vbar->w);
  }
  widget_set_self_layout(vbar, param);

  obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, vbar->name);
  tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, vbar);

  return RET_OK;
}

inline static ret_t edit_ex_create_suggest_words_scroll_view_init(edit_ex_t* edit_ex,
                                                                  widget_t* scroll_view,
                                                                  widget_t* popup) {
  tk_object_t* obj_props = NULL;
  char param[128] = {'\0'};
  widget_t* hbar = widget_lookup(popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_HBAR, TRUE);
  widget_t* vbar = widget_lookup(popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR, TRUE);
  int32_t vbar_w = widget_get_prop_int(vbar, WIDGET_PROP_LAYOUT_W, vbar->w);

  if (hbar != NULL) {
    int32_t hbar_h = widget_get_prop_int(hbar, WIDGET_PROP_LAYOUT_H, hbar->h);
    widget_resize(scroll_view, -vbar_w, -hbar_h);
    tk_snprintf(param, sizeof(param) - 1, "default(x=0,y=0,w=%d,h=%d)", scroll_view->w,
                scroll_view->h);
  } else {
    widget_resize(scroll_view, -vbar_w, scroll_view->h);
    tk_snprintf(param, sizeof(param) - 1, "default(x=0,y=0,w=%d,h=100%%)", scroll_view->w);
  }
  widget_set_self_layout(scroll_view, param);

  obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, scroll_view->name);
  tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, scroll_view);

  return RET_OK;
}

static widget_t* edit_ex_create_suggest_words_popup(widget_t* widget) {
  /* base on combo_box_ex_create_scroll_popup() */
  int32_t w = 0;
  int32_t h = 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
  int32_t item_height = 0;
  const char* applet_name = NULL;
  const char* theme = NULL;
  widget_t* win = NULL;
  widget_t* window_for_edit_ex = NULL;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  tk_object_t* obj_props = NULL;
  return_value_if_fail(edit_ex != NULL, NULL);

  window_for_edit_ex = widget_get_window(widget);

  w = tk_max_int(widget->w, tk_object_get_prop_int(
                                edit_ex->suggest_words_ui_props,
                                EDIT_EX_SUGGEST_WORDS_UI_NAME_POPUP "." WIDGET_PROP_MIN_W, 0));
  item_height = widget->h;
  win = popup_create(NULL, 0, 0, w, h);

  applet_name = widget_get_prop_str(window_for_edit_ex, WIDGET_PROP_APPLET_NAME, NULL);
  widget_set_prop_str(win, WIDGET_PROP_APPLET_NAME, applet_name);

  theme = widget_get_prop_str(window_for_edit_ex, WIDGET_PROP_THEME, NULL);
  widget_set_prop_str(win, WIDGET_PROP_THEME, theme);

  widget_set_prop_bool(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, TRUE);

  widget_use_style(win, "combobox_popup");

  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_PREV_KEY, "up");
  widget_set_prop_str(win, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY, "down");

  widget_set_name(win, EDIT_EX_SUGGEST_WORDS_UI_NAME_POPUP);
  obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, win->name);
  tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, win);

  { /* children */
    char param[128] = {'\0'};
    widget_t* list_view = NULL;
    widget_t* vbar = NULL;
    widget_t* scroll_view = NULL;
    // create list view
    list_view = list_view_create(win, 0, 0, 0, 0);
    widget_set_prop_int(list_view, WIDGET_PROP_ITEM_HEIGHT, item_height);
    widget_set_prop_bool(list_view, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR, TRUE);

    tk_snprintf(param, sizeof(param) - 1, "default(x=%d,y=%d,w=%d,h=%d)",
                EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN, EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN,
                -2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN,
                -2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN);
    widget_set_self_layout(list_view, param);

    widget_set_name(list_view, EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW);
    obj_props = tk_object_get_prop_object(edit_ex->suggest_words_ui_props, list_view->name);
    tk_object_foreach_prop(obj_props, edit_ex_widget_set_prop_by_object_on_visit, list_view);
    // create hbar
    if (w < tk_object_get_prop_int(
                edit_ex->suggest_words_ui_props,
                EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW "." WIDGET_PROP_ITEM_WIDTH, w)) {
      edit_ex_create_suggest_words_hbar_create(edit_ex, win);
    }
    // create vbar
    vbar = scroll_bar_create(list_view, 0, 0, 0, 0);
    widget_set_name(vbar, EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR);
    edit_ex_create_suggest_words_vbar_init(edit_ex, vbar, win);
    // create scroll view
    scroll_view = scroll_view_create(list_view, 0, 0, 0, 0);
    widget_set_name(scroll_view, EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW);
    edit_ex_create_suggest_words_scroll_view_init(edit_ex, scroll_view, win);
  }

  widget_set_focused_internal(widget, TRUE);

  return win;
}

static ret_t edit_ex_suggest_words_popup_confirm(widget_t* widget, widget_t* item) {
  /* base on combo_box_on_item_click() */
  ret_t ret = RET_OK;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  ENSURE(edit_ex);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (item != NULL) {
    if (edit_ex->suggest_words_model_items != NULL &&
        edit_ex->suggest_words_model_items->size > 0) {
      tk_object_t* data = NULL;
      char index_str[32] = {'\0'};
      const char* text = NULL;
      const char* input_name = TK_STR_IS_NOT_EMPTY(edit_ex->suggest_words_input_name)
                                   ? edit_ex->suggest_words_input_name
                                   : EDIT_EX_DEFAULT_SUGGEST_WORDS_INPUT_NAME;
      int32_t index = widget_index_of(item);
      goto_error_if_fail_ex(index >= 0, ret = RET_FAIL);

      tk_snprintf(index_str, sizeof(index_str) - 1, "[%d]", index);

      data = tk_object_get_prop_object(edit_ex->suggest_words, index_str);
      goto_error_if_fail_ex(data != NULL, ret = RET_NOT_FOUND);

      text = tk_object_get_prop_str(data, input_name);
      ret = widget_set_text_utf8(widget, text);
    } else {
      const wchar_t* text = widget_get_text(item);
      ret = widget_set_text(widget, text);
    }
    if (RET_OK == ret) {
      edit_set_cursor(widget, -1);
    }
  }

error:
  widget->target = NULL;
  widget->key_target = NULL;
  window_close(edit_ex->suggest_words_popup);
  widget_set_focused_internal(widget, TRUE);

  return ret;
}

static ret_t edit_ex_suggest_words_popup_on_item_click(void* ctx, event_t* e) {
  widget_t* item = WIDGET(e->target);
  widget_t* widget = WIDGET(ctx);
  return_value_if_fail(widget != NULL && item != NULL, RET_BAD_PARAMS);

  return edit_ex_suggest_words_popup_confirm(widget, item);
}

inline static ret_t edit_ex_suggest_words_enable_focus(widget_t* widget, bool_t enable) {
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);
  if (edit_ex->suggest_words_popup != NULL) {
    widget_set_sensitive(edit_ex->suggest_words_popup, enable);
  }
  return RET_OK;
}

static ret_t edit_ex_suggest_words_focus_by_key(widget_t* widget, uint32_t key) {
  ret_t ret = RET_SKIP;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);

  if (edit_ex->suggest_words_popup != NULL) {
    if (key_code_is_down(key)) {
      ret = widget_focus_first(edit_ex->suggest_words_popup);
      if (RET_OK == ret) {
        widget_set_sensitive(edit_ex->suggest_words_popup, TRUE);
      }
    }
  }

  return ret;
}

typedef struct _edit_ex_update_suggest_words_popup_ctx_t {
  edit_ex_t* edit_ex;
  widget_t* list_view;
  widget_t* scroll_view;
  darray_t* used_format_names;
  uint32_t curr_index;
  int32_t item_width;
  bool_t index_changed : 1;
} edit_ex_update_suggest_words_popup_ctx_t;

static ret_t edit_ex_update_suggest_words_popup_on_item_visit(void* ctx, const void* data) {
  /* base on combo_box_visit_item() */
  edit_ex_update_suggest_words_popup_ctx_t* actx = (edit_ex_update_suggest_words_popup_ctx_t*)(ctx);
  widget_t* iter = WIDGET(data);
  return_value_if_fail(actx != NULL && iter != NULL, RET_BAD_PARAMS);
  return_value_if_fail(actx->edit_ex != NULL, RET_BAD_PARAMS);

  if (actx->curr_index != 0) {
    widget_set_visible(iter, TRUE);
    widget_set_sensitive(iter, TRUE);
    if (iter->emitter == NULL ||
        !emitter_exist(iter->emitter, EVT_CLICK, edit_ex_suggest_words_popup_on_item_click,
                       actx->edit_ex)) {
      widget_on(iter, EVT_CLICK, edit_ex_suggest_words_popup_on_item_click, actx->edit_ex);
    }

    if (actx->edit_ex->suggest_words_model_items != NULL &&
        actx->edit_ex->suggest_words_model_items->size > 0) {
      const char* iter_format_name =
          widget_get_prop_str(iter, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "[0]");

      if (darray_bsearch_index(actx->used_format_names, NULL, (void*)iter_format_name) < 0) {
        uint32_t item_children_size = widget_count_children(iter);
        if (item_children_size > 0) {
          widget_t* last = NULL;
          if (iter->w != actx->item_width) {
            widget_set_prop_int(iter, WIDGET_PROP_W, actx->item_width);
            widget_set_prop_int(iter, WIDGET_PROP_H, LIST_VIEW(actx->list_view)->item_height);
            widget_layout_children(iter);
          }
          last = widget_get_child(iter, item_children_size - 1);
          actx->item_width = tk_max(actx->item_width, last->x + last->w);
        }
        darray_sorted_insert(actx->used_format_names, tk_strdup(iter_format_name), NULL, TRUE);
      }
    }

    actx->curr_index--;
  } else {
    widget_set_sensitive(iter, FALSE);
    widget_set_visible(iter, FALSE);
  }

  return RET_OK;
}

typedef struct _edit_ex_item_set_suggest_words_ctx_t {
  widget_t* widget;
  tk_object_t* suggest_words;
} edit_ex_item_set_suggest_words_ctx_t;

static ret_t edit_ex_item_set_suggest_words_on_visit(void* ctx, const void* data) {
  edit_ex_item_set_suggest_words_ctx_t* actx = (edit_ex_item_set_suggest_words_ctx_t*)(ctx);
  const named_value_t* nv = (const named_value_t*)(data);
  return_value_if_fail(actx != NULL && nv != NULL, RET_BAD_PARAMS);
  return_value_if_fail(actx->widget != NULL && actx->suggest_words != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(nv->name, EDIT_EX_SUGGEST_WORDS_ITEM_FORMAT_PREFIX)) {
    value_t tmp;
    if (RET_OK == tk_object_get_prop(actx->suggest_words, value_str(&nv->value), &tmp)) {
      const char* name = nv->name + sizeof(EDIT_EX_SUGGEST_WORDS_ITEM_FORMAT_PREFIX) - 1;
      widget_set_prop(actx->widget, name, &tmp);
    }
  }

  return RET_OK;
}

static ret_t edit_ex_item_set_suggest_words(tk_object_t* suggest_words, widget_t* widget) {
  if (widget->custom_props != NULL) {
    edit_ex_item_set_suggest_words_ctx_t ctx = {.widget = widget, .suggest_words = suggest_words};
    return tk_object_foreach_prop(widget->custom_props, edit_ex_item_set_suggest_words_on_visit,
                                  &ctx);
  }
  return RET_OK;
}

static ret_t edit_ex_update_suggest_words_popup_on_visit(void* ctx, const void* data) {
  edit_ex_update_suggest_words_popup_ctx_t* actx = (edit_ex_update_suggest_words_popup_ctx_t*)(ctx);
  const value_t* v = (const value_t*)data;
  widget_t* item = NULL;
  return_value_if_fail(actx != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(
      actx->edit_ex != NULL && actx->scroll_view != NULL && actx->list_view != NULL,
      RET_BAD_PARAMS);

  if (actx->edit_ex->suggest_words_model_items != NULL &&
      actx->edit_ex->suggest_words_model_items->size > 0) {
    tk_object_t* suggest_words = value_object(v);
    const char* format_name = NULL;
    widget_t* model = NULL;
    return_value_if_fail(suggest_words != NULL, RET_FAIL);

    model = edit_ex_suggest_words_model_items_find(
        actx->edit_ex->suggest_words_model_items,
        tk_object_get_prop_str(suggest_words, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME));
    return_value_if_fail(model != NULL, RET_FAIL);

    format_name = widget_get_prop_str(model, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "[0]");

    item = widget_get_child(actx->scroll_view, actx->curr_index);

    if (item != NULL) {
      const char* item_format_name =
          widget_get_prop_str(item, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "[0]");
      if (!tk_str_eq(item_format_name, format_name)) {
        item = NULL;
        WIDGET_FOR_EACH_CHILD_BEGIN_R(actx->scroll_view, iter, i)
        const char* iter_format_name = NULL;
        if (i <= actx->curr_index) {
          break;
        }
        iter_format_name = widget_get_prop_str(iter, EDIT_EX_SUGGEST_WORDS_PROP_FORMAT_NAME, "[0]");
        if (tk_str_eq(iter_format_name, format_name)) {
          item = iter;
          widget_restack(item, actx->curr_index);
          actx->index_changed = TRUE;
          break;
        }
        WIDGET_FOR_EACH_CHILD_END()
      }
    }

    if (item == NULL) {
      item = widget_clone(model, actx->scroll_view);
      return_value_if_fail(item != NULL, RET_OOM);
      widget_restack(item, actx->curr_index);
      actx->index_changed = TRUE;
    }

    WIDGET_FOR_EACH_CHILD_BEGIN(item, iter, i)
    widget_foreach(iter, (tk_visit_t)edit_ex_item_set_suggest_words, suggest_words);
    WIDGET_FOR_EACH_CHILD_END()
  } else {
    const char* suggest = value_str(v);
    return_value_if_fail(suggest != NULL, RET_FAIL);

    actx->index_changed = FALSE;

    item = widget_get_child(actx->scroll_view, actx->curr_index);
    if (item == NULL) {
      item = edit_ex_suggest_words_create_item(actx->edit_ex, actx->scroll_view);
      return_value_if_fail(item != NULL, RET_OOM);
      actx->index_changed = TRUE;
    }
    widget_set_text_utf8(item, suggest);
  }

  if (actx->index_changed) {
    ret_t ret = RET_OK;
    value_t tmp;
    if ((actx->curr_index + 1) % 2 != 0) { /* 0 为第一项，所以是奇数项 */
      ret = widget_get_prop(actx->list_view, EDIT_EX_PROP_SUGGEST_WORDS_ITEM_ODD_STYLE, &tmp);
    } else {
      ret = widget_get_prop(actx->list_view, EDIT_EX_PROP_SUGGEST_WORDS_ITEM_EVEN_STYLE, &tmp);
    }
    if (RET_OK == ret) {
      widget_use_style(item, value_str(&tmp));
    }
  }

  actx->curr_index++;

  return RET_OK;
}

static ret_t edit_ex_layout_suggest_words_popup(widget_t* widget) {
  /* base on combo_box_ex_create_scroll_popup() */
  int32_t w = 0;
  edit_ex_t* edit_ex = EDIT_EX(widget);
  darray_t used_format_names;
  edit_ex_update_suggest_words_popup_ctx_t update_ctx = {
      .edit_ex = edit_ex,
      .used_format_names = &used_format_names,
  };
  return_value_if_fail(edit_ex != NULL, RET_BAD_PARAMS);
  return_value_if_fail(edit_ex->suggest_words != NULL && edit_ex->suggest_words_popup != NULL,
                       RET_FAIL);

  update_ctx.list_view =
      widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_LIST_VIEW, TRUE);
  return_value_if_fail(update_ctx.list_view != NULL, RET_FAIL);

  update_ctx.scroll_view =
      widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_SCROLL_VIEW, TRUE);
  return_value_if_fail(update_ctx.scroll_view != NULL, RET_FAIL);

  darray_init(&used_format_names, 4, default_destroy, (tk_compare_t)tk_str_cmp);

  w = tk_max_int(widget->w,
                 widget_get_prop_int(edit_ex->suggest_words_popup, WIDGET_PROP_MIN_W, 0));
  update_ctx.item_width = widget_get_prop_int(update_ctx.list_view, WIDGET_PROP_ITEM_WIDTH, 0);
  update_ctx.item_width = tk_max(update_ctx.item_width, w);

  if (edit_ex->suggest_words_popup->w != w) {
    widget_resize(edit_ex->suggest_words_popup, w, edit_ex->suggest_words_popup->h);
    widget_layout(edit_ex->suggest_words_popup);
  }

  tk_object_foreach_prop(edit_ex->suggest_words, edit_ex_update_suggest_words_popup_on_visit,
                         &update_ctx);

  WIDGET_FOR_EACH_CHILD_BEGIN(update_ctx.scroll_view, iter, i)
  edit_ex_update_suggest_words_popup_on_item_visit(&update_ctx, iter);
  WIDGET_FOR_EACH_CHILD_END()
  widget_set_prop_int(update_ctx.list_view, WIDGET_PROP_ITEM_WIDTH, update_ctx.item_width);

  { /* 根据 item_width 来决定是否需要创建水平滚动条 */
    int32_t nr = 0;
    int32_t item_height = 0;
    int32_t h = 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
    bool_t reinit = FALSE;
    widget_t* hbar =
        widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_HBAR, TRUE);
    if (w < update_ctx.item_width) {
      if (hbar == NULL) {
        hbar = edit_ex_create_suggest_words_hbar_create(edit_ex, edit_ex->suggest_words_popup);
        reinit = TRUE;
      }
    } else {
      if (hbar != NULL) {
        widget_destroy(hbar);
        hbar = NULL;
        reinit = TRUE;
      }
    }

    item_height = widget_get_prop_int(update_ctx.list_view, WIDGET_PROP_ITEM_HEIGHT, widget->h);
    nr = tk_object_get_prop_int(edit_ex->suggest_words, TK_OBJECT_PROP_SIZE, 0);
    if (nr <= EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR) {
      h = nr * item_height + 2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
    } else {
      h = EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MAXNR * item_height +
          2 * EDIT_SUGGEST_WORDS_WINDOW_DEFAULT_MARGIN;
    }
    if (hbar != NULL) {
      h += widget_get_prop_int(hbar, WIDGET_PROP_LAYOUT_H, hbar->h);
    }
    widget_resize(edit_ex->suggest_words_popup, w, h);

    if (reinit) {
      widget_t* vbar =
          widget_lookup(edit_ex->suggest_words_popup, EDIT_EX_SUGGEST_WORDS_UI_NAME_VBAR, TRUE);
      edit_ex_create_suggest_words_vbar_init(edit_ex, vbar, edit_ex->suggest_words_popup);
      edit_ex_create_suggest_words_scroll_view_init(edit_ex, update_ctx.scroll_view,
                                                    edit_ex->suggest_words_popup);
    }
  }

  darray_deinit(&used_format_names);

  return RET_OK;
}

inline static ret_t edit_ex_suggest_words_popup_calc_position(widget_t* widget, point_t* p) {
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

    /* 为了焦点不移动到 suggest_words_popup 上，所以打开前先将 sensitive 设置为 FALSE，打开窗口后若没有处于输入中时再设置为 TRUE。 */
    widget_set_sensitive(edit_ex->suggest_words_popup, FALSE);
    widget_on(edit_ex->suggest_words_popup, EVT_WINDOW_OPEN, edit_ex_suggest_words_popup_on_open,
              widget);

    widget_on(edit_ex->suggest_words_popup, EVT_WINDOW_CLOSE, edit_ex_suggest_words_popup_on_close,
              widget);

    widget_on(edit_ex->suggest_words_popup, EVT_KEY_DOWN_BEFORE_CHILDREN,
              edit_ex_suggest_words_popup_on_key_down_before_children, widget);
  }

  edit_ex_layout_suggest_words_popup(widget);
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

ret_t edit_ex_update_suggest_words_popup(widget_t* widget) {
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

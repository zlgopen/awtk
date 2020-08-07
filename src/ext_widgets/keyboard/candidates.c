/**
 * File:   candidates.c
 * Author: AWTK Develop Team
 * Brief:  candidates
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "widgets/button.h"
#include "keyboard/candidates.h"
#include "base/input_method.h"
#include "base/system_info.h"
#include "base/widget_vtable.h"

static ret_t candidates_on_im_candidates_event(void* ctx, event_t* e);

static ret_t candidates_on_button_focused(void* ctx, event_t* e) {
  char str[32];
  widget_t* button = WIDGET(e->target);
  input_method_t* im = input_method();
  wstr_t* text = &(button->text);
  candidates_t* candidates = CANDIDATES(ctx);
  return_value_if_fail(im != NULL && text->size > 0, RET_FAIL);
  tk_utf8_from_utf16(text->str, str, sizeof(str) - 1);

  if (candidates->pre) {
    input_method_dispatch_keys(im, str);
  }

  return RET_OK;
}

static ret_t candidates_on_button_click(void* ctx, event_t* e) {
  char str[32];
  widget_t* button = WIDGET(e->target);
  input_method_t* im = input_method();
  wstr_t* text = &(button->text);
  candidates_t* candidates = CANDIDATES(ctx);
  return_value_if_fail(im != NULL, RET_FAIL);

  if (text->size > 0) {
    wchar_t c = text->str[text->size - 1];

    tk_utf8_from_utf16(text->str, str, sizeof(str) - 1);
    if (input_method_commit_text(im, str) == RET_OK) {
      if (!candidates->pre) {
        suggest_words_t* suggest_words = im->suggest_words;
        if (suggest_words && suggest_words_find(suggest_words, c) == RET_OK) {
          input_method_dispatch_candidates(im, suggest_words->words, suggest_words->words_nr, -1);
          if (suggest_words->words_nr > 0) {
            widget_set_focused(widget_get_child(button->parent, 0), TRUE);
            candidates->is_suggest = TRUE;
          }
          log_debug("suggest_words->words:%s\n", suggest_words->words);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t candidates_create_button(widget_t* widget) {
  candidates_t* candidates = CANDIDATES(widget);
  widget_t* button = button_create(widget, 0, 0, 0, 0);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  widget_use_style(button, "candidates");
  if (candidates->pre) {
    widget_on(button, EVT_FOCUS, candidates_on_button_focused, widget);
    widget_on(button, EVT_CLICK, candidates_on_button_click, widget);
  } else {
    widget_on(button, EVT_CLICK, candidates_on_button_click, widget);
  }

  if (candidates->button_style != NULL) {
    widget_use_style(button, candidates->button_style);
  }

  widget_set_focusable(button, TRUE);

  return RET_OK;
}

static ret_t candidates_ensure_children(widget_t* widget, uint32_t nr) {
  uint32_t i = 0;
  uint32_t size = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children && nr <= widget->children->size) {
    return RET_OK;
  }

  size = widget->children ? widget->children->size : 0;
  for (i = size; i < nr; i++) {
    candidates_create_button(widget);
  }

  return_value_if_fail(widget->children->size == nr, RET_OOM);

  return RET_OK;
}

static uint32_t candidates_calc_child_width(canvas_t* c, widget_t* widget) {
  wstr_t* str = &(widget->text);

  return canvas_measure_text(c, str->str, str->size) + 8;
}

static ret_t candidates_relayout_children(widget_t* widget) {
  uint32_t i = 0;
  xy_t margin = 2;
  wh_t child_w = 0;
  xy_t child_x = margin;
  xy_t child_y = margin;
  widget_t* iter = NULL;
  widget_t* focused = NULL;
  uint32_t nr = widget->children->size;
  wh_t child_h = widget->h - margin * 2;
  candidates_t* candidates = CANDIDATES(widget);
  widget_t** children = (widget_t**)(widget->children->elms);
  style_t* style = children[0]->astyle;
  canvas_t* c = widget_get_canvas(widget);
  const char* font = system_info_fix_font_name(NULL);
  int32_t child_margin = style_get_int(style, STYLE_ID_MARGIN, 0);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);

  canvas_set_font(c, font, font_size);
  for (i = 0; i < nr; i++) {
    iter = children[i];
    child_w = candidates_calc_child_width(c, iter) + child_margin* 2;
    if (iter->text.size) {
      widget_set_visible(iter, TRUE, FALSE);
    } else {
      child_w = 0;
      widget_set_visible(iter, FALSE, FALSE);
    }
    widget_move_resize(iter, child_x, child_y, child_w, child_h);
    child_x += child_w + margin;

    if (iter->focused) {
      focused = iter;
    }
  }

  hscrollable_set_virtual_w(candidates->hscrollable, child_x + 30);
  if (focused != NULL && focused->x > widget->w / 2) {
    hscrollable_set_xoffset(candidates->hscrollable, focused->x);
  } else {
    hscrollable_set_xoffset(candidates->hscrollable, 0);
  }

  return RET_OK;
}

static ret_t candidates_update_candidates(widget_t* widget, const char* strs, uint32_t nr,
                                          int32_t selected) {
  uint32_t i = 0;
  widget_t* iter = NULL;
  const char* text = strs;
  widget_t** children = NULL;
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL && strs != NULL, RET_BAD_PARAMS);
  return_value_if_fail(candidates_ensure_children(widget, nr + 1) == RET_OK, RET_OOM);

  children = (widget_t**)(widget->children->elms);

  for (i = 0; i < nr; i++) {
    iter = children[i];
    widget_set_visible(iter, TRUE, FALSE);
    widget_set_text_utf8(iter, text);
    if (selected == i) {
      widget_set_focused(iter, TRUE);
    } else {
      widget_set_focused(iter, FALSE);
    }
    text += strlen(text) + 1;
  }

  for (; i < widget->children->size; i++) {
    iter = children[i];
    widget_set_visible(iter, FALSE, FALSE);
    widget_set_text_utf8(iter, "");
  }

  candidates->candidates_nr = nr;

  if (candidates->auto_hide) {
    widget_set_visible(widget, nr > 0, FALSE);
  } else {
    widget_set_visible(widget, TRUE, FALSE);
  }

  candidates_relayout_children(widget);
  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

static ret_t candidates_on_destroy_default(widget_t* widget) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(widget != NULL && candidates != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(candidates->button_style);
  hscrollable_destroy(candidates->hscrollable);
  input_method_off(input_method(), candidates->event_id);

  return RET_OK;
}

static ret_t candidates_on_paint_self(widget_t* widget, canvas_t* c) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(widget != NULL && candidates != NULL, RET_BAD_PARAMS);

  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t candidates_on_event(widget_t* widget, event_t* e) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_OPEN) {
    if (candidates->pre) {
      candidates->event_id = input_method_on(input_method(), EVT_IM_SHOW_PRE_CANDIDATES,
                                             candidates_on_im_candidates_event, candidates);
    } else {
      candidates->event_id = input_method_on(input_method(), EVT_IM_SHOW_CANDIDATES,
                                             candidates_on_im_candidates_event, candidates);
    }
  }

  return hscrollable_on_event(candidates->hscrollable, e);
}

static ret_t candidates_on_paint_children(widget_t* widget, canvas_t* c) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  return hscrollable_on_paint_children(candidates->hscrollable, c);
}

static ret_t candidates_get_prop(widget_t* widget, const char* name, value_t* v) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CANDIDATES_PROP_PRE)) {
    value_set_bool(v, candidates->pre);
    return RET_OK;
  } else if (tk_str_eq(name, CANDIDATES_PROP_SELECT_BY_NUM)) {
    value_set_bool(v, candidates->select_by_num);
    return RET_OK;
  } else if (tk_str_eq(name, CANDIDATES_PROP_AUTO_HIDE)) {
    value_set_bool(v, candidates->auto_hide);
    return RET_OK;
  } else if (tk_str_eq(name, CANDIDATES_PROP_BUTTON_STYLE)) {
    value_set_str(v, candidates->button_style);
    return RET_OK;
  } else if (candidates->hscrollable != NULL) {
    return hscrollable_get_prop(candidates->hscrollable, name, v);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t candidates_set_prop(widget_t* widget, const char* name, const value_t* v) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CANDIDATES_PROP_PRE)) {
    return candidates_set_pre(widget, value_bool(v));
  } else if (tk_str_eq(name, CANDIDATES_PROP_SELECT_BY_NUM)) {
    return candidates_set_select_by_num(widget, value_bool(v));
  } else if (tk_str_eq(name, CANDIDATES_PROP_AUTO_HIDE)) {
    return candidates_set_auto_hide(widget, value_bool(v));
  } else if (tk_str_eq(name, CANDIDATES_PROP_BUTTON_STYLE)) {
    return candidates_set_button_style(widget, value_str(v));
  }
  if (candidates->hscrollable != NULL) {
    return hscrollable_set_prop(candidates->hscrollable, name, v);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t candidates_move_focus(widget_t* widget, bool_t next) {
  widget_t* focus = NULL;
  int32_t next_focus = 0;
  candidates_t* candidates = CANDIDATES(widget);
  uint32_t nr = candidates->candidates_nr;

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->focused) {
    if (next) {
      next_focus = i + 1;
    } else {
      if (i > 0) {
        next_focus = i - 1;
      }
    }
  }
  WIDGET_FOR_EACH_CHILD_END();

  next_focus = next_focus % nr;
  focus = widget_get_child(widget, next_focus);
  widget_set_focused(focus, TRUE);

  return RET_OK;
}

static ret_t candidates_on_keyup(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* child = NULL;
  candidates_t* candidates = CANDIDATES(widget);
  uint32_t nr = candidates->candidates_nr;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (nr > 1) {
    if (e->key >= TK_KEY_1 && e->key <= TK_KEY_9 && candidates->select_by_num) {
      int32_t i = e->key - (int32_t)TK_KEY_0 - 1;

      if (i >= 0 && i < nr) {
        event_t click = event_init(EVT_CLICK, NULL);
        child = widget_get_child(widget, i);

        if (child->text.size > 0 && child->visible) {
          widget_dispatch(child, &click);
          ret = RET_STOP;
        }
      }
    } else if (e->key == TK_KEY_LEFT || e->key == TK_KEY_RIGHT) {
      if (nr > 2) {
        candidates_move_focus(widget, e->key == TK_KEY_RIGHT);
        ret = RET_STOP;
      }
    } else if (e->key == TK_KEY_BACKSPACE || e->key == TK_KEY_DELETE) {
      if (candidates->is_suggest) {
        char words[2] = {0};
        input_method_dispatch_candidates(input_method(), words, 0, 0);
        ret = RET_STOP;
      }
    }
  }

  return ret;
}

static const char* const s_candidates_properties[] = {
    CANDIDATES_PROP_PRE, CANDIDATES_PROP_SELECT_BY_NUM, CANDIDATES_PROP_BUTTON_STYLE,
    CANDIDATES_PROP_AUTO_HIDE, NULL};

TK_DECL_VTABLE(candidates) = {.size = sizeof(candidates_t),
                              .scrollable = TRUE,
                              .type = WIDGET_TYPE_CANDIDATES,
                              .parent = TK_PARENT_VTABLE(widget),
                              .clone_properties = s_candidates_properties,
                              .persistent_properties = s_candidates_properties,
                              .create = candidates_create,
                              .on_event = candidates_on_event,
                              .on_paint_self = candidates_on_paint_self,
                              .on_paint_children = candidates_on_paint_children,
                              .get_prop = candidates_get_prop,
                              .set_prop = candidates_set_prop,
                              .on_keyup = candidates_on_keyup,
                              .on_destroy = candidates_on_destroy_default};

static ret_t candidates_on_im_candidates_event(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  candidates_t* candidates = CANDIDATES(widget);
  im_candidates_event_t* evt = (im_candidates_event_t*)e;

  candidates->is_suggest = FALSE;
  if (candidates->pre) {
    if (e->type == EVT_IM_SHOW_PRE_CANDIDATES) {
      candidates_update_candidates(widget, evt->candidates, evt->candidates_nr, evt->selected);
    }
  } else {
    if (e->type == EVT_IM_SHOW_CANDIDATES) {
      candidates_update_candidates(widget, evt->candidates, evt->candidates_nr, evt->selected);
    }
  }

  return RET_OK;
}

widget_t* candidates_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(candidates), x, y, w, h);
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, NULL);

  candidates->select_by_num = TRUE;
  candidates->hscrollable = hscrollable_create(widget);
  hscrollable_set_always_scrollable(candidates->hscrollable, TRUE);

  ENSURE(candidates->hscrollable != NULL);

  return widget;
}

widget_t* candidates_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, candidates), NULL);

  return widget;
}

ret_t candidates_set_pre(widget_t* widget, bool_t pre) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  candidates->pre = pre;

  return RET_OK;
}

ret_t candidates_set_select_by_num(widget_t* widget, bool_t select_by_num) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  candidates->select_by_num = select_by_num;

  return RET_OK;
}

ret_t candidates_set_auto_hide(widget_t* widget, bool_t auto_hide) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  candidates->auto_hide = auto_hide;

  return RET_OK;
}

ret_t candidates_set_button_style(widget_t* widget, const char* button_style) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, RET_BAD_PARAMS);

  candidates->button_style = tk_str_copy(candidates->button_style, button_style);

  return RET_OK;
}

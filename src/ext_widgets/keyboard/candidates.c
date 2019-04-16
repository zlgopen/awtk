/**
 * File:   candidates.c
 * Author: AWTK Develop Team
 * Brief:  candidates
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/widget_vtable.h"

static ret_t candidates_on_button_click(void* ctx, event_t* e) {
  char str[32];
  input_method_t* im = input_method();
  wstr_t* text = &(WIDGET(ctx)->text);
  wchar_t c = text->str[text->size - 1];
  return_value_if_fail(im != NULL && text->size > 0, RET_FAIL);

  utf8_from_utf16(text->str, str, sizeof(str) - 1);
  if (input_method_commit_text(im, str) == RET_OK) {
    suggest_words_t* suggest_words = im->suggest_words;
    if (suggest_words && suggest_words_find(suggest_words, c) == RET_OK) {
      input_method_dispatch_candidates(im, suggest_words->words, suggest_words->words_nr);
    }
  }

  (void)e;

  return RET_OK;
}

static ret_t candidates_create_button(widget_t* widget) {
  widget_t* button = button_create(widget, 0, 0, 0, 0);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  widget_use_style(button, "candidates");
  widget_on(button, EVT_CLICK, candidates_on_button_click, button);

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
  wh_t w = widget->w;
  xy_t child_x = margin;
  xy_t child_y = margin;
  widget_t* iter = NULL;
  uint32_t nr = widget->children->size;
  wh_t child_h = widget->h - margin * 2;
  candidates_t* candidates = CANDIDATES(widget);
  widget_t** children = (widget_t**)(widget->children->elms);
  canvas_t* c = candidates->canvas;
  style_t* style = children[0]->astyle;
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);

  canvas_set_font(c, TK_DEFAULT_FONT, font_size);
  for (i = 0; i < nr; i++) {
    iter = children[i];
    child_w = candidates_calc_child_width(candidates->canvas, iter);
    if ((child_x + child_w + margin) < w && iter->text.size > 0) {
      widget_set_visible(iter, TRUE, FALSE);
      widget_move_resize(iter, child_x, child_y, child_w, child_h);
    } else {
      widget_set_visible(iter, FALSE, FALSE);
      widget_move_resize(iter, 0, 0, 0, 0);
    }

    child_x += child_w + margin;
  }

  return RET_OK;
}

static ret_t candidates_update_candidates(widget_t* widget, const char* strs, uint32_t nr) {
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
    widget_set_text_utf8(iter, text);
    text += strlen(text) + 1;
  }

  for (; i < widget->children->size; i++) {
    iter = children[i];
    widget_set_text_utf8(iter, "");
  }

  candidates_relayout_children(widget);
  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

static ret_t candidates_on_destroy_default(widget_t* widget) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(widget != NULL && candidates != NULL, RET_BAD_PARAMS);

  input_method_off(input_method(), candidates->event_id);

  return RET_OK;
}

static ret_t candidates_on_paint_self(widget_t* widget, canvas_t* c) {
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(widget != NULL && candidates != NULL, RET_BAD_PARAMS);

  candidates->canvas = c;
  return widget_paint_helper(widget, c, NULL, NULL);
}

TK_DECL_VTABLE(candidates) = {.size = sizeof(candidates_t),
                              .type = WIDGET_TYPE_CANDIDATES,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = candidates_create,
                              .on_paint_self = candidates_on_paint_self,
                              .on_destroy = candidates_on_destroy_default};

static ret_t candidates_on_im_candidates_event(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  im_candidates_event_t* evt = (im_candidates_event_t*)e;

  return candidates_update_candidates(widget, evt->candidates, evt->candidates_nr);
}

widget_t* candidates_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(candidates), x, y, w, h);
  candidates_t* candidates = CANDIDATES(widget);
  return_value_if_fail(candidates != NULL, NULL);

  candidates->event_id = input_method_on(input_method(), EVT_IM_SHOW_CANDIDATES,
                                         candidates_on_im_candidates_event, candidates);

  return widget;
}

widget_t* candidates_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, candidates), NULL);

  return widget;
}

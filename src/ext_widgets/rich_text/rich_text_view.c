/**
 * File:   rich_text_view.h
 * Author: AWTK Develop Team
 * Brief:  rich_text_view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "scroll_view/scroll_bar.h"
#include "rich_text/rich_text.h"
#include "rich_text/rich_text_view.h"

static ret_t rich_text_view_sync_scroll_bar_to_rich_text(widget_t* widget) {
  rich_text_view_t* rich_text_view = RICH_TEXT_VIEW(widget);
  rich_text_t* rich_text = RICH_TEXT(rich_text_view->rich_text);
  int32_t yoffset = widget_get_value(rich_text_view->scroll_bar);

  if (rich_text != NULL) {
    emitter_disable(rich_text_view->rich_text->emitter);
    widget_set_prop_int(rich_text_view->rich_text, WIDGET_PROP_YOFFSET, yoffset);
    emitter_enable(rich_text_view->rich_text->emitter);
  }

  return RET_OK;
}

static ret_t rich_text_view_on_scroll_bar_changed(void* ctx, event_t* e) {
  rich_text_view_sync_scroll_bar_to_rich_text(WIDGET(ctx));
  return RET_OK;
}

static ret_t rich_text_view_sync_rich_text_to_scroll_bar(widget_t* widget) {
  int32_t value = 0;
  rich_text_view_t* rich_text_view = RICH_TEXT_VIEW(widget);
  int32_t yoffset = widget_get_prop_int(rich_text_view->rich_text, WIDGET_PROP_YOFFSET, 0);
  int32_t virtual_h = widget_get_prop_int(rich_text_view->rich_text, WIDGET_PROP_VIRTUAL_H, 0);
  int32_t h = rich_text_view->rich_text->h;
  int32_t max = tk_max(virtual_h, h);

  if (max > h) {
    value = (yoffset * max) / (max - h);
  }

  if (rich_text_view->scroll_bar != NULL) {
    emitter_disable(rich_text_view->scroll_bar->emitter);
    widget_set_prop_int(rich_text_view->scroll_bar, WIDGET_PROP_MAX, max);
    widget_set_prop_int(rich_text_view->scroll_bar, WIDGET_PROP_VALUE, value);
    emitter_enable(rich_text_view->scroll_bar->emitter);
  }

  return RET_OK;
}

static ret_t rich_text_view_on_rich_text_scrolled(void* ctx, event_t* e) {
  prop_change_event_t* evt = (prop_change_event_t*)e;

  if (tk_str_eq(evt->name, WIDGET_PROP_YOFFSET)) {
    rich_text_view_sync_rich_text_to_scroll_bar(WIDGET(ctx));
  }

  return RET_OK;
}

static ret_t rich_text_view_on_add_child(widget_t* widget, widget_t* child) {
  rich_text_view_t* rich_text_view = RICH_TEXT_VIEW(widget);

  if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_RICH_TEXT)) {
    rich_text_view->rich_text = child;
    widget_on(child, EVT_PROP_CHANGED, rich_text_view_on_rich_text_scrolled, widget);
  } else if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR) ||
             tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR_DESKTOP) ||
             tk_str_eq(widget_get_type(child), WIDGET_TYPE_SCROLL_BAR_MOBILE)) {
    rich_text_view->scroll_bar = child;
    widget_on(child, EVT_VALUE_CHANGED, rich_text_view_on_scroll_bar_changed, widget);
  }

  return RET_FAIL;
}

static ret_t rich_text_view_on_remove_child(widget_t* widget, widget_t* child) {
  rich_text_view_t* rich_text_view = RICH_TEXT_VIEW(widget);

  widget_off_by_ctx(child, widget);
  if (rich_text_view->rich_text == child) {
    rich_text_view->rich_text = NULL;
  }

  if (rich_text_view->scroll_bar == child) {
    rich_text_view->scroll_bar = NULL;
  }

  return RET_FAIL;
}

TK_DECL_VTABLE(rich_text_view) = {.size = sizeof(rich_text_view_t),
                                  .type = WIDGET_TYPE_RICH_TEXT_VIEW,
                                  .parent = TK_PARENT_VTABLE(widget),
                                  .on_add_child = rich_text_view_on_add_child,
                                  .on_remove_child = rich_text_view_on_remove_child,
                                  .create = rich_text_view_create};

widget_t* rich_text_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(rich_text_view), x, y, w, h);
}

widget_t* rich_text_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, rich_text_view), NULL);

  return widget;
}

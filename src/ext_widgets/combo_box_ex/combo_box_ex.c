/**
 * File:   combo_box_ex.c
 * Author: AWTK Develop Team
 * Brief:  combo_box_ex
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-21 Li DaHeng <linany1990@163.com> created
 *
 */

#include "base/layout.h"
#include "widgets/popup.h"
#include "widgets/combo_box_item.h"
#include "ext_widgets/scroll_view/list_view.h"
#include "ext_widgets/scroll_view/scroll_view.h"
#include "ext_widgets/scroll_view/scroll_bar.h"
#include "ext_widgets/combo_box_ex/combo_box_ex.h"
#include "ext_widgets/scroll_view/list_item_seperator.h"

#define COMBO_BOX_EX_DEFAULT_MAXNR 5
#define COMBO_BOX_EX_DEFAULT_MARGIN 1

static ret_t combo_box_ex_create_popup_items(combo_box_t* combo_box, widget_t* parent,
                                             int32_t extra_w) {
  uint32_t w = 0;
  uint32_t max_w = 0;
  combo_box_option_t* iter = NULL;
  widget_t* win = widget_get_window(parent);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  while (iter != NULL) {
    widget_t* item = NULL;
    const char* text = iter->text;

    if (tk_str_start_with(text, COMBO_BOX_EX_SEPERATOR_PREFIX)) {
      item = list_item_seperator_create(parent, 0, 0, 0, 0);
      text += strlen(COMBO_BOX_EX_SEPERATOR_PREFIX);
      widget_set_prop_bool(item, WIDGET_PROP_RADIO, FALSE);
      widget_set_prop_bool(item, WIDGET_PROP_VALUE, TRUE);
    } else {
      item = combo_box_item_create(parent, 0, 0, 0, 0);
      widget_set_value(item, iter->value);
    }

    if (combo_box->localize_options) {
      widget_set_tr_text(item, text);
    } else {
      widget_set_text_utf8(item, text);
    }
    w = widget_measure_text(item, item->text.str);
    max_w = tk_max_int(w, max_w);

    iter = iter->next;
  }

  w = tk_max_int(max_w + extra_w, win->w);
  widget_resize(win, w, win->h);

  return RET_OK;
}

static ret_t combo_box_ex_on_layout_children_for_combobox_popup(widget_t* widget) {
  combo_box_t* combo_box = COMBO_BOX(widget);
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  if (combo_box->combobox_popup != NULL && combo_box->open_window == NULL) {
    point_t p = {0, 0};
    int32_t w = combo_box->combobox_popup->w;
    int32_t margin = COMBO_BOX_EX_DEFAULT_MARGIN;
    int32_t item_height = combo_box->item_height;
    int32_t nr = combo_box_count_options(widget);
    int32_t h = nr * item_height + 2 * margin;

    if (nr <= COMBO_BOX_EX_DEFAULT_MAXNR) {
      h = nr * item_height + 2 * margin;
    } else {
      h = COMBO_BOX_EX_DEFAULT_MAXNR * item_height + 2 * margin;
    }

    combo_box_combobox_popup_calc_position(widget, w, h, &p);
    widget_move_resize(combo_box->combobox_popup, p.x, p.y, w, h);
  }
  return RET_OK;
}

static widget_t* combo_box_ex_create_scroll_popup(combo_box_t* combo_box) {
  value_t v;
  widget_t* win = NULL;
  widget_t* list_view = NULL;
  widget_t* scroll_bar = NULL;
  widget_t* scroll_view = NULL;
  widget_t* combo_box_win = NULL;
  const char* applet_name = NULL;
  widget_t* widget = WIDGET(combo_box);
  int32_t margin = COMBO_BOX_EX_DEFAULT_MARGIN;
  int32_t item_height = combo_box->item_height;
  int32_t nr = combo_box_count_options(widget);
  int32_t w = widget->w;
  int32_t h = nr * item_height + 2 * margin;
  if (nr <= COMBO_BOX_EX_DEFAULT_MAXNR) {
    h = nr * item_height + 2 * margin;
  } else {
    h = COMBO_BOX_EX_DEFAULT_MAXNR * item_height + 2 * margin;
  }

  // create popup
  win = popup_create(NULL, 0, 0, w, h);
  combo_box_win = widget_get_window(widget);
  applet_name = widget_get_prop_str(combo_box_win, WIDGET_PROP_APPLET_NAME, NULL);
  widget_set_prop_str(win, WIDGET_PROP_APPLET_NAME, applet_name);

  value_set_bool(&v, TRUE);
  widget_set_prop(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, &v);
  widget_set_prop_str(win, WIDGET_PROP_THEME, "combobox_ex_popup");

  w -= 2 * margin;
  h -= 2 * margin;

  // create list view
  list_view = list_view_create(win, margin, margin, w, h);
  widget_set_prop(list_view, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR, &v);
  value_set_int32(&v, item_height);
  widget_set_prop(list_view, WIDGET_PROP_ITEM_HEIGHT, &v);
  // create scroll view
  scroll_view = scroll_view_create(list_view, 0, 0, -12, h);
  scroll_bar = scroll_bar_create(list_view, 0, 0, 12, h);

  widget_use_style(win, "combobox_popup");
  combo_box_ex_create_popup_items(combo_box, scroll_view, item_height + 2 * margin + scroll_bar->w);

  w = win->w;
  h = win->h;
  w -= 2 * margin;
  h -= 2 * margin;
  widget_move_resize(list_view, margin, margin, w, h);
  widget_move_resize(scroll_view, 0, 0, w - scroll_bar->w, h);
  widget_move(scroll_bar, w - scroll_bar->w, 0);
  widget_layout(win);

  combo_box->combobox_popup = win;
  widget_on(win, EVT_WINDOW_CLOSE, combo_box_combobox_popup_on_close_func, widget);

  return win;
}

static widget_t* custom_open_popup(widget_t* combo_box) {
  return combo_box_ex_create_scroll_popup(COMBO_BOX(combo_box));
}

widget_t* combo_box_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* combo_box = combo_box_create(parent, x, y, w, h);
  return_value_if_fail(combo_box != NULL, NULL);

  widget_set_prop_str(combo_box, WIDGET_PROP_TYPE, WIDGET_TYPE_COMBO_BOX_EX);
  combo_box_set_custom_open_popup(combo_box, custom_open_popup,
                                  combo_box_ex_on_layout_children_for_combobox_popup);

  return combo_box;
}

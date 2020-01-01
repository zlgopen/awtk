/**
 * File:   combo_box_ex.c
 * Author: AWTK Develop Team
 * Brief:  combo_box_ex
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

static ret_t combo_box_create_popup_items(combo_box_t* combo_box, widget_t* parent) {
  combo_box_option_t* iter = NULL;
  return_value_if_fail(combo_box != NULL, RET_BAD_PARAMS);

  iter = combo_box->option_items;
  while (iter != NULL) {
    widget_t* item = combo_box_item_create(parent, 0, 0, 0, 0);

    widget_set_value(item, iter->value);
    if (combo_box->localize_options) {
      widget_set_tr_text(item, iter->text);
    } else {
      widget_set_text_utf8(item, iter->text);
    }

    iter = iter->next;
  }

  return RET_OK;
}

static widget_t* combo_box_create_scroll_popup(combo_box_t* combo_box) {
  value_t v;
  widget_t* widget = WIDGET(combo_box);
  int32_t margin = 2;
  int32_t item_height = combo_box->item_height;
  int32_t nr = combo_box_count_options(widget);
  int32_t w = widget->w;
  int32_t h = nr * item_height + 2 * margin;
  const uint8_t maxNr = 5;
  if (nr <= maxNr) {
    h = nr * item_height + 2 * margin;
  } else {
    h = maxNr * item_height + 2 * margin;
  }

  // create popup
  widget_t* win = popup_create(NULL, 0, 0, w, h);
  value_set_bool(&v, TRUE);
  widget_set_prop(win, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE, &v);

  // create list view
  widget_t* list_view = list_view_create(win, 0, 0, w, h);
  widget_set_prop(list_view, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR, &v);
  value_set_int32(&v, item_height);
  widget_set_prop(list_view, WIDGET_PROP_ITEM_HEIGHT, &v);
  // create scroll view
  widget_t* scroll_view = scroll_view_create(list_view, 0, 0, -12, h);
  widget_t* scroll_bar = scroll_bar_create(list_view, 0, 0, 0, 0);
  widget_set_self_layout(scroll_bar, "default(x=right, y=0,w=12, h=100%%)");

  widget_use_style(win, "combobox_popup");
  combo_box_create_popup_items(combo_box, scroll_view);
  widget_layout(win);

  return win;
}

static widget_t* custom_open_popup(widget_t* combo_box) {
  return combo_box_create_scroll_popup(COMBO_BOX(combo_box));
}

widget_t* combo_box_ex_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* combo_box = combo_box_create(parent, x, y, w, h);
  return_value_if_fail(combo_box != NULL, NULL);

  widget_set_prop_str(combo_box, WIDGET_PROP_TYPE, WIDGET_TYPE_COMBO_BOX_EX);
  combo_box_set_custom_open_popup(combo_box, custom_open_popup);

  return combo_box;
}

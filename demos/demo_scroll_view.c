/**
 * File:   demo_scroll_view.c
 * Author: AWTK Develop Team
 * Brief:  test scroll view
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-22 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

#include "awtk.h"
#include "ext_widgets.h"

widget_t* bar_h = NULL;
widget_t* bar_v = NULL;
widget_t* scroll_view = NULL;

#define SCROLL_BAR_H_WIDGT_NAME "bar_h"
#define SCROLL_BAR_V_WIDGT_NAME "bar_v"
#define BUTTON_SET_FOCUSE_STRING "focused:"

static int32_t scroll_bar_value_to_scroll_view_offset_y(scroll_bar_t* scroll_bar,
                                                        scroll_view_t* sv) {
  int32_t range = 0;
  float_t percent = 0;
  range = scroll_bar->virtual_size;
  percent = range > 0 ? (float_t)scroll_bar->value / (float_t)(range) : 0;
  return percent * (sv->virtual_h - sv->widget.h);
}

static int32_t scroll_bar_value_to_scroll_view_offset_x(scroll_bar_t* scroll_bar,
                                                        scroll_view_t* sv) {
  int32_t range = 0;
  float_t percent = 0;
  range = scroll_bar->virtual_size;
  percent = range > 0 ? (float_t)scroll_bar->value / (float_t)(range) : 0;
  return percent * (sv->virtual_w - sv->widget.w);
}

static int32_t scroll_bar_value_from_scroll_view_offset_y(scroll_bar_t* scroll_bar,
                                                          scroll_view_t* sv) {
  int32_t range = 0;
  float_t percent = 0;

  range = sv->virtual_h - sv->widget.h;
  percent = range > 0 ? (float_t)sv->yoffset / (float_t)range : 0;

  return percent * scroll_bar->virtual_size;
}

static int32_t scroll_bar_value_from_scroll_view_offset_x(scroll_bar_t* scroll_bar,
                                                          scroll_view_t* sv) {
  int32_t range = 0;
  float_t percent = 0;

  range = sv->virtual_w - sv->widget.w;
  percent = range > 0 ? (float_t)sv->xoffset / (float_t)range : 0;

  return percent * scroll_bar->virtual_size;
}

static ret_t scroll_bar_value_changed(void* ctx, event_t* e) {
  scroll_view_t* sv = SCROLL_VIEW(scroll_view);
  scroll_bar_t* scroll_bar_h = SCROLL_BAR(bar_h);
  scroll_bar_t* scroll_bar_v = SCROLL_BAR(bar_v);

  int32_t offset_x = scroll_bar_value_to_scroll_view_offset_x(scroll_bar_h, sv);
  int32_t offset_y = scroll_bar_value_to_scroll_view_offset_y(scroll_bar_v, sv);

  scroll_view_set_offset(scroll_view, offset_x, offset_y);

  return RET_OK;
}

static ret_t on_set_focuse_item(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;

  if (name != NULL) {
    widget_t* item = widget_lookup(scroll_view, name, TRUE);
    widget_set_focused(item, TRUE);
  }

  return RET_OK;
}

static ret_t scroll_view_offset_changed(void* ctx, event_t* e) {
  scroll_view_t* sv = SCROLL_VIEW(scroll_view);
  scroll_bar_t* scroll_bar_h = SCROLL_BAR(bar_h);
  scroll_bar_t* scroll_bar_v = SCROLL_BAR(bar_v);

  int32_t value_x = scroll_bar_value_from_scroll_view_offset_x(scroll_bar_h, sv);
  int32_t value_y = scroll_bar_value_from_scroll_view_offset_y(scroll_bar_v, sv);

  scroll_bar_set_value_only(bar_h, value_x);
  scroll_bar_set_value_only(bar_v, value_y);

  return RET_OK;
}

static ret_t install_one(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  if (widget->name != NULL) {
    const char* name = widget->name;
    if (strstr(name, BUTTON_SET_FOCUSE_STRING) != NULL) {
      widget_on(widget, EVT_CLICK, on_set_focuse_item,
                (void*)(name + tk_strlen(BUTTON_SET_FOCUSE_STRING)));
    } else if (tk_str_eq(name, SCROLL_BAR_H_WIDGT_NAME)) {
      bar_h = widget;
      widget_on(widget, EVT_VALUE_CHANGED, scroll_bar_value_changed, widget);
    } else if (tk_str_eq(name, SCROLL_BAR_V_WIDGT_NAME)) {
      bar_v = widget;
      widget_on(widget, EVT_VALUE_CHANGED, scroll_bar_value_changed, widget);
    }
  } else if (tk_str_eq(widget->vt->type, "scroll_view")) {
    scroll_view = widget;
    widget_on(widget, EVT_SCROLL, scroll_view_offset_changed, widget);
  }
  return RET_OK;
}

ret_t on_idle_scroll_view_set_virtual_wh(const idle_info_t* idle) {
  scroll_view_t* sv = SCROLL_VIEW(scroll_view);
  scroll_bar_set_params(bar_h, sv->virtual_w, 10);
  scroll_bar_set_params(bar_v, sv->virtual_h, 10);
  return RET_OK;
}

ret_t application_init() {
  widget_t* win = window_open("scroll_view");
  widget_foreach(win, install_one, win);
  idle_add(on_idle_scroll_view_set_virtual_wh, win);
  return RET_OK;
}

ret_t application_exit() {
  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"

/**
 * File:   check_button.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  check_button
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/check_button.h"
#include "base/image_manager.h"
#include "base/mem.h"
#include "base/widget_vtable.h"
#include "base/wstr.h"

enum { BORDER = 2 };

static ret_t check_button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  check_button_t* check_button = CHECK_BUTTON(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      if (check_button->radio) {
        check_button_set_value(widget, TRUE);
      } else {
        check_button_set_value(widget, !(check_button->value));
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t check_button_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  bitmap_t bitmap;
  const char* icon_name = NULL;
  style_t* style = &(widget->style);
  check_button_t* check_button = CHECK_BUTTON(widget);
  color_t color = color_init(0xff, 0xff, 0xff, 0xff);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  icon_name = style_get_str(style, STYLE_ID_ICON, NULL);
  return_value_if_fail(icon_name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image_manager_load(default_im(), icon_name, &bitmap) == RET_OK, RET_FAIL);

  canvas_set_text_color(c, style_get_color(style, STYLE_ID_TEXT_COLOR, color));
  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_BG_COLOR, color));

  if (bitmap.data != NULL) {
    rect_t src;
    rect_t dst;

    x = BORDER;
    y = (widget->h - bitmap.h) >> 1;
    rect_init(src, 0, 0, bitmap.w, bitmap.h);
    rect_init(dst, x, y, bitmap.w, bitmap.h);
    canvas_draw_image(c, &bitmap, &src, &dst);
  }

  x += widget->h;
  if (check_button->text.size > 0) {
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
    uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, 20);

    canvas_set_font(c, font_name, font_size);
    y = (widget->h >> 1);
    canvas_draw_text(c, check_button->text.str, check_button->text.size, x, y);
  }

  return RET_OK;
}

static ret_t check_button_set_value_only(widget_t* widget, bool_t value) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  check_button->value = value;
  widget->state = WIDGET_STATE_NORMAL;
  if (value) {
    widget_set_state(widget, WIDGET_STATE_CHECKED);
  } else {
    widget_set_state(widget, WIDGET_STATE_UNCHECKED);
  }

  return RET_OK;
}

ret_t check_button_set_value(widget_t* widget, bool_t value) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  check_button_set_value_only(widget, value);

  if (check_button->radio && widget->parent != NULL) {
    uint32_t i = 0;
    array_t* children = widget->parent->children;

    for (i = 0; i < children->size; i++) {
      widget_t* iter = (widget_t*)(children->elms[i]);

      if (iter != widget && iter->type == WIDGET_RADIO_BUTTON) {
        check_button_set_value_only(iter, !value);
      }
    }
  }

  return RET_OK;
}

ret_t check_button_set_text(widget_t* widget, const wchar_t* text) {
  check_button_t* button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_set(&(button->text), text);
}

static ret_t check_button_get_prop(widget_t* widget, const char* name, value_t* v) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "value") == 0) {
    value_set_uint8(v, check_button->value);
    return RET_OK;
  } else if (strcmp(name, "text") == 0) {
    value_set_wstr(v, check_button->text.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t check_button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "value") == 0) {
    return check_button_set_value(widget, value_int(v));
  } else if (strcmp(name, "text") == 0) {
    return check_button_set_text(widget, value_wstr(v));
  }

  return RET_NOT_FOUND;
}

static ret_t check_button_destroy(widget_t* widget) {
  check_button_t* check_button = CHECK_BUTTON(widget);
  wstr_reset(&(check_button->text));

  return RET_OK;
}

static const widget_vtable_t s_check_button_vtable = {.on_event = check_button_on_event,
                                                      .on_paint_self = check_button_on_paint_self,
                                                      .get_prop = check_button_get_prop,
                                                      .set_prop = check_button_set_prop,
                                                      .destroy = check_button_destroy};

widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  check_button_t* check_button = MEM_ZALLOC(check_button_t);
  return_value_if_fail(check_button != NULL, NULL);

  widget = WIDGETP(check_button);
  widget_init(widget, parent, WIDGET_CHECK_BUTTON);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_check_button_vtable;
  check_button_set_value(widget, FALSE);

  return widget;
}

widget_t* check_button_create_radio(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = check_button_create(parent, x, y, w, h);
  check_button_t* check_button = CHECK_BUTTON(widget);
  return_value_if_fail(widget != NULL, NULL);

  check_button->radio = TRUE;
  widget->type = WIDGET_RADIO_BUTTON;
  widget->state = WIDGET_STATE_NORMAL;
  check_button_set_value(widget, FALSE);

  return widget;
}

/**
 * File:   label.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/label.h"
#include "base/mem.h"

static ret_t label_on_paint_self(widget_t* widget, canvas_t* c) {
  label_t* label = LABEL(widget);

  return widget_paint_helper(widget, c, NULL, &(label->text));
}

ret_t label_set_text(widget_t* widget, const wchar_t* text) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_set(&(label->text), text);
}

static ret_t label_get_prop(widget_t* widget, const char* name, value_t* v) {
  label_t* label = LABEL(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    value_set_wstr(v, label->text.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t label_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    return label_set_text(widget, value_wstr(v));
  }

  return RET_NOT_FOUND;
}

static ret_t label_destroy(widget_t* widget) {
  label_t* label = LABEL(widget);
  wstr_reset(&(label->text));

  return RET_OK;
}

static const widget_vtable_t s_label_vtable = {.on_paint_self = label_on_paint_self,
                                               .get_prop = label_get_prop,
                                               .set_prop = label_set_prop,
                                               .destroy = label_destroy};

widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  label_t* label = MEM_ZALLOC(label_t);
  return_value_if_fail(label != NULL, NULL);

  widget = WIDGETP(label);
  widget_init(widget, parent, WIDGET_LABEL);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_label_vtable;

  wstr_init(&(label->text), 0);

  return widget;
}

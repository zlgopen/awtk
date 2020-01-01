/**
 * File:   line_number.h
 * Author: AWTK Develop Team
 * Brief:  line_number
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
 * 2019-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mledit/line_number.h"
#include "base/widget_vtable.h"
#include "base/window_manager.h"

static ret_t line_number_do_paint_self(widget_t* widget, canvas_t* c) {
  uint32_t y = 0;
  uint32_t x = 2;
  uint32_t line = 0;
  char str[32] = {0};
  wstr_t* text = &(widget->text);
  uint32_t w = widget->w - 2 * x;
  rect_t r = rect_init(0, 0, 0, 0);
  line_number_t* line_number = LINE_NUMBER(widget);
  int32_t yoffset = line_number->yoffset;
  int32_t line_height = line_number->line_height;

  return_value_if_fail(line_height > 0, RET_BAD_PARAMS);

  if (style_is_valid(widget->astyle)) {
    widget_prepare_text_style(widget, c);

    while (1) {
      y = line_height * line + line_number->top_margin;
      if ((y + line_height) < yoffset) {
        line++;
        continue;
      }

      if (y >= (yoffset + widget->h)) {
        break;
      }

      r = rect_init(x, y - yoffset, w, line_height);
      tk_snprintf(str, sizeof(str), "%u", (line + 1));

      wstr_set_utf8(text, str);
      canvas_draw_text_in_rect(c, text->str, text->size, &r);

      line++;
    }
  }

  return RET_OK;
}

static ret_t line_number_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t save_r = {0, 0, 0, 0};
  rect_t clip_r = {0, 0, 0, 0};
  rect_t edit_r = {0, 0, 0, 0};
  line_number_t* line_number = LINE_NUMBER(widget);

  canvas_get_clip_rect(c, &save_r);

  edit_r = rect_init(c->ox, c->oy + line_number->top_margin, widget->w,
                     widget->h - line_number->top_margin - line_number->bottom_margin);
  clip_r = rect_intersect(&save_r, &edit_r);

  canvas_set_clip_rect(c, &clip_r);
  line_number_do_paint_self(widget, c);
  canvas_set_clip_rect(c, &save_r);

  return RET_OK;
}

ret_t line_number_set_yoffset(widget_t* widget, int32_t yoffset) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);
  line_number->yoffset = yoffset;

  return widget_invalidate_force(widget, NULL);
}

ret_t line_number_set_line_height(widget_t* widget, int32_t line_height) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);
  line_number->line_height = line_height;

  return widget_invalidate_force(widget, NULL);
}

ret_t line_number_set_top_margin(widget_t* widget, int32_t top_margin) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);
  line_number->top_margin = top_margin;

  return widget_invalidate_force(widget, NULL);
}

ret_t line_number_set_bottom_margin(widget_t* widget, int32_t bottom_margin) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);
  line_number->bottom_margin = bottom_margin;

  return widget_invalidate_force(widget, NULL);
}

static ret_t line_number_get_prop(widget_t* widget, const char* name, value_t* v) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

static ret_t line_number_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(line_number) = {.size = sizeof(line_number_t),
                               .type = WIDGET_TYPE_LINE_NUMBER,
                               .enable_pool = TRUE,
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = line_number_create,
                               .set_prop = line_number_set_prop,
                               .get_prop = line_number_get_prop,
                               .on_paint_self = line_number_on_paint_self};

widget_t* line_number_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(line_number), x, y, w, h);
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, NULL);

  return widget;
}

widget_t* line_number_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, line_number), NULL);

  return widget;
}

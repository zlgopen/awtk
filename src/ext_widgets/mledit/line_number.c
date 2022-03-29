/**
 * File:   line_number.h
 * Author: AWTK Develop Team
 * Brief:  line_number
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static uint32_t line_number_get_line_num(line_number_t* line_number, uint32_t line) {
  uint32_t line_num;

  if (line_number->lines_of_each_row == NULL || line >= line_number->lines_of_each_row_len) {
    line_num = 1;
  } else {
    line_num = tk_max(line_number->lines_of_each_row[line], 1);
  }

  return line_num;
}

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
    uint32_t line_index = 0;
    color_t trans = color_init(0, 0, 0, 0);
    widget_prepare_text_style_ex(widget, c, trans, NULL, TK_DEFAULT_FONT_SIZE, ALIGN_H_RIGHT,
                                 ALIGN_V_TOP);
    color_t active_bg =
        style_get_color(widget->astyle, LINE_NUMBER_STYLE_ACTIVE_LINE_BG_COLOR, trans);
    color_t highlight_bg =
        style_get_color(widget->astyle, LINE_NUMBER_STYLE_HIGHLIGHT_LINE_BG_COLOR, trans);

    while (1) {
      if (line > 0) {
        line_index += line_number_get_line_num(line_number, line - 1);
      }

      y = line_height * line_index + line_number->top_margin;
      if ((y + line_height) < yoffset) {
        line++;
        continue;
      }

      if (y >= (yoffset + widget->h)) {
        break;
      }

      r = rect_init(x, y - yoffset, w, line_height);
      tk_snprintf(str, sizeof(str), "%u", (line + 1));

      if (line == line_number->active_line && active_bg.rgba.a) {
        canvas_set_fill_color(c, active_bg);
        canvas_fill_rect(c, r.x, r.y, r.w, r.h);
      } else if (line_number_is_highlight_line(widget, line) && highlight_bg.rgba.a) {
        canvas_set_fill_color(c, highlight_bg);
        canvas_fill_rect(c, r.x, r.y, r.w, r.h);
      }
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
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);
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

ret_t line_number_set_lines_of_each_row(widget_t* widget, const uint32_t* lines_of_each_row,
                                        uint32_t len) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL && lines_of_each_row != NULL && len > 0, RET_BAD_PARAMS);

  if (line_number->lines_of_each_row == NULL || line_number->lines_of_each_row_len != len) {
    line_number->lines_of_each_row = TKMEM_REALLOCT(uint32_t, line_number->lines_of_each_row, len);
    return_value_if_fail(line_number->lines_of_each_row != NULL,
                         (line_number->lines_of_each_row_len = 0, RET_OOM));
    line_number->lines_of_each_row_len = len;
  }
  memcpy(line_number->lines_of_each_row, lines_of_each_row, sizeof(uint32_t) * len);

  return widget_invalidate_force(widget, NULL);
}

static ret_t line_number_on_destroy(widget_t* widget) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);

  if (line_number->lines_of_each_row != NULL) {
    TKMEM_FREE(line_number->lines_of_each_row);
    line_number->lines_of_each_row_len = 0;
  }

  return RET_OK;
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
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = line_number_create,
                               .set_prop = line_number_set_prop,
                               .get_prop = line_number_get_prop,
                               .on_paint_self = line_number_on_paint_self,
                               .on_destroy = line_number_on_destroy};

widget_t* line_number_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(line_number), x, y, w, h);
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, NULL);
  line_number->active_line = -1;

  return widget;
}

widget_t* line_number_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, line_number), NULL);

  return widget;
}

static ret_t line_number_ensure_highlight_lines(line_number_t* line_number) {
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);

  if (line_number->highlight_lines == NULL) {
    line_number->highlight_lines = typed_array_create(VALUE_TYPE_INT32, 10);
  }

  return line_number->highlight_lines != NULL ? RET_OK : RET_FAIL;
}

bool_t line_number_is_highlight_line(widget_t* widget, int32_t line) {
  line_number_t* line_number = LINE_NUMBER(widget);
  if (line_number != NULL && line_number->highlight_lines != NULL) {
    value_t v;
    uint32_t i = 0;
    for (i = 0; i < line_number->highlight_lines->size; i++) {
      typed_array_get(line_number->highlight_lines, i, &v);
      if (value_int(&v) == line) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

ret_t line_number_add_highlight_line(widget_t* widget, int32_t line) {
  value_t v;
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number_ensure_highlight_lines(line_number) == RET_OK, RET_OOM);
  widget_invalidate(widget, NULL);

  return typed_array_push(line_number->highlight_lines, value_set_int(&v, line));
}

ret_t line_number_set_active_line(widget_t* widget, int32_t line) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);

  line_number->active_line = line;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t line_number_clear_highlight(widget_t* widget) {
  line_number_t* line_number = LINE_NUMBER(widget);
  return_value_if_fail(line_number != NULL, RET_BAD_PARAMS);

  return typed_array_clear(line_number->highlight_lines);
}

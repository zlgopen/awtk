/**
 * File:   text_edit.c
 * Author: AWTK Develop Team
 * Brief:  text_edit
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
 * 2019-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <wctype.h>

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/events.h"
#include "base/text_edit.h"
#include "base/line_break.h"
#include "base/clip_board.h"
#include "base/input_method.h"

#define CHAR_SPACING 1
#define FONT_BASELINE 1.25f
#define STB_TEXTEDIT_CHARTYPE wchar_t
#define STB_TEXTEDIT_NEWLINE (wchar_t)('\n')
#define STB_TEXTEDIT_STRING text_edit_t

#if !defined(WITH_SDL)
#define STB_TEXTEDIT_UNDOSTATECOUNT 10
#define STB_TEXTEDIT_UNDOCHARCOUNT 32
#endif /*WITH_SDL*/
#define STB_TEXTEDIT_GETWIDTH_NEWLINE 0xffff

#include "stb/stb_textedit.h"

typedef struct _text_layout_info_t {
  int32_t w;
  int32_t h;
  int32_t ox;
  int32_t oy;

  uint32_t virtual_w;
  uint32_t virtual_h;
  uint32_t widget_w;
  uint32_t widget_h;
  uint32_t margin_l;
  uint32_t margin_t;
  uint32_t margin_r;
  uint32_t margin_b;
} text_layout_info_t;

typedef struct _row_info_t {
  uint32_t offset;
  uint16_t length;
  uint16_t text_w;
  uint16_t x;
} row_info_t;

typedef struct _rows_t {
  uint32_t size;
  uint32_t capacity;
  row_info_t info[1];
} rows_t;

typedef struct _text_edit_impl_t {
  text_edit_t text_edit;
  STB_TexteditState state;

  rows_t* rows;
  point_t caret;
  bool_t wrap_word;
  bool_t single_line;
  bool_t caret_visible;
  uint32_t line_height;
  uint32_t last_line_number;
  text_layout_info_t layout_info;

  bool_t preedit;
  uint32_t preedit_chars_nr;

  /*for single line edit*/
  wchar_t mask_char;
  bool_t mask;
  wstr_t tips;

  void* on_state_changed_ctx;
  text_edit_on_state_changed_t on_state_changed;
} text_edit_impl_t;

#define DECL_IMPL(te) text_edit_impl_t* impl = (text_edit_impl_t*)(te)

static ret_t text_edit_notify(text_edit_t* text_edit);
static int32_t text_edit_calc_x(text_edit_t* text_edit, row_info_t* iter);

static align_h_t widget_get_text_align_h(widget_t* widget) {
  return (align_h_t)style_get_int(widget->astyle, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_LEFT);
}

static ret_t widget_get_text_layout_info(widget_t* widget, text_layout_info_t* info) {
  value_t v;
  style_t* style = widget->astyle;
  return_value_if_fail(widget != NULL && info != NULL && style != NULL, RET_BAD_PARAMS);

  value_set_int(&v, 0);
  info->widget_w = widget->w;
  info->widget_h = widget->h;
  info->virtual_w = widget->w;
  info->virtual_h = widget->h;

  if (widget_get_prop(widget, WIDGET_PROP_LEFT_MARGIN, &v) == RET_OK) {
    info->margin_l = value_int(&v);
  }

  if (info->margin_l == 0) {
    info->margin_l = style_get_int(style, STYLE_ID_MARGIN_LEFT, 1);
  }

  if (info->margin_l == 0) {
    info->margin_l = style_get_int(style, STYLE_ID_MARGIN, 1);
  }

  if (widget_get_prop(widget, WIDGET_PROP_RIGHT_MARGIN, &v) == RET_OK) {
    info->margin_r = value_int(&v);
  }

  if (info->margin_r == 0) {
    info->margin_r = style_get_int(style, STYLE_ID_MARGIN_RIGHT, 1);
  }

  if (info->margin_r == 0) {
    info->margin_r = style_get_int(style, STYLE_ID_MARGIN, 1);
  }

  if (widget_get_prop(widget, WIDGET_PROP_TOP_MARGIN, &v) == RET_OK) {
    info->margin_t = value_int(&v);
  }

  if (info->margin_t == 0) {
    info->margin_t = style_get_int(style, STYLE_ID_MARGIN_TOP, 1);
  }

  if (info->margin_t == 0) {
    info->margin_t = style_get_int(style, STYLE_ID_MARGIN, 1);
  }

  if (widget_get_prop(widget, WIDGET_PROP_BOTTOM_MARGIN, &v) == RET_OK) {
    info->margin_b = value_int(&v);
  }

  if (info->margin_b == 0) {
    info->margin_b = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, 1);
  }

  if (info->margin_b == 0) {
    info->margin_b = style_get_int(style, STYLE_ID_MARGIN, 1);
  }

  info->w = info->widget_w - info->margin_l - info->margin_r;
  info->h = info->widget_h - info->margin_t - info->margin_b;

  return RET_OK;
}

static rows_t* rows_create(uint32_t capacity) {
  uint32_t msize = sizeof(rows_t) + capacity * sizeof(row_info_t);
  rows_t* rows = (rows_t*)TKMEM_ALLOC(msize);
  return_value_if_fail(rows != NULL, NULL);

  memset(rows, 0x00, msize);
  rows->capacity = capacity;

  return rows;
}

static row_info_t* rows_find_by_offset(rows_t* rows, uint32_t offset) {
  uint32_t i = 0;

  for (i = 0; i < rows->size; i++) {
    row_info_t* iter = rows->info + i;
    if (iter->offset == offset) {
      return iter;
    }
  }

  return NULL;
}

static ret_t rows_destroy(rows_t* rows) {
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rows);

  return RET_OK;
}

static ret_t text_edit_set_caret_pos(text_edit_impl_t* impl, uint32_t x, uint32_t y,
                                     uint32_t font_size) {
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t caret_top = layout_info->margin_t + y;
  uint32_t caret_bottom = layout_info->margin_t + y + font_size;
  uint32_t caret_left = layout_info->margin_l + x;
  uint32_t caret_right = layout_info->margin_l + x + 1;

  uint32_t view_top = layout_info->oy + layout_info->margin_t;
  uint32_t view_bottom = layout_info->oy + layout_info->margin_t + layout_info->h;
  uint32_t view_left = layout_info->ox + layout_info->margin_l;
  uint32_t view_right = layout_info->ox + layout_info->margin_l + layout_info->w;

  impl->caret.x = x;
  impl->caret.y = y;

  if (view_top > caret_top) {
    layout_info->oy = caret_top - layout_info->margin_t;
  }

  if (view_bottom < caret_bottom) {
    layout_info->oy = caret_bottom - layout_info->h;
  }

  if (view_left > caret_left) {
    layout_info->ox = caret_left - layout_info->margin_l;
  }

  if (view_right < caret_right) {
    layout_info->ox = caret_right - layout_info->w;
  }

  if (impl->wrap_word) {
    layout_info->ox = 0;
  }

  if (impl->single_line) {
    layout_info->oy = 0;
  }

  return RET_OK;
}

static uint32_t text_edit_measure_text(canvas_t* c, wchar_t* str, wchar_t mask_char,
                                       uint32_t size) {
  uint32_t i = 0;
  uint32_t w = 0;

  for (i = 0; i < size; i++) {
    wchar_t chr = mask_char ? mask_char : str[i];

    w += canvas_measure_text(c, &chr, 1) + CHAR_SPACING;
  }

  return w;
}

static row_info_t* text_edit_single_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                     uint32_t offset) {
  uint32_t y = 0;
  uint32_t caret_x = 0;
  DECL_IMPL(text_edit);
  canvas_t* c = text_edit->c;
  wstr_t* text = &(text_edit->widget->text);
  STB_TexteditState* state = &(impl->state);
  row_info_t* row = impl->rows->info + row_num;
  wchar_t mask_char = impl->mask ? impl->mask_char : 0;
  text_layout_info_t* layout_info = &(impl->layout_info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);
  uint32_t text_w = text_edit_measure_text(c, text->str, mask_char, text->size);
  uint32_t caret_text_w = text_edit_measure_text(c, text->str, mask_char, state->cursor);

  assert(offset == 0 && row_num == 0);
  memset(row, 0x00, sizeof(row_info_t));

  row->offset = 0;
  row->text_w = text_w;
  row->length = text->size;
  row->x = text_edit_calc_x(text_edit, row);
  layout_info->virtual_h = tk_max(y, layout_info->widget_h);

  caret_x = caret_text_w;
  if (text_w < layout_info->w) {
    layout_info->ox = 0;
    if (align_h == ALIGN_H_RIGHT) {
      caret_x = layout_info->w - (text_w - caret_text_w);
    } else if (align_h == ALIGN_H_CENTER) {
      caret_x = (layout_info->w - text_w) / 2 + caret_text_w;
    }
  }
  text_edit_set_caret_pos(impl, caret_x, y, c->font_size);

  return row;
}

static row_info_t* text_edit_multi_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                    uint32_t offset) {
  uint32_t i = 0;
  uint32_t x = 0;
  DECL_IMPL(text_edit);
  wchar_t last_char = 0;
  canvas_t* c = text_edit->c;
  wstr_t* text = &(text_edit->widget->text);
  STB_TexteditState* state = &(impl->state);
  row_info_t* row = impl->rows->info + row_num;
  uint32_t line_height = impl->line_height;
  uint32_t y = line_height * row_num;
  uint32_t last_breakable_i = 0;
  uint32_t last_breakable_x = 0;
  text_layout_info_t* layout_info = &(impl->layout_info);

  memset(row, 0x00, sizeof(row_info_t));
  for (i = offset; i < text->size; i++) {
    wchar_t* p = text->str + i;
    break_type_t word_break = LINE_BREAK_NO;
    break_type_t line_break = LINE_BREAK_NO;
    uint32_t char_w = canvas_measure_text(c, p, 1) + CHAR_SPACING;

    if (i == state->cursor) {
      text_edit_set_caret_pos(impl, x, y, c->font_size);
    }

    last_char = *p;
    line_break = line_break_check(*p, p[1]);
    if (line_break == LINE_BREAK_MUST) {
      i++;
      break;
    }

    if (impl->wrap_word) {
      if ((x + char_w) > layout_info->w) {
        if (last_breakable_x > 0) {
          i = last_breakable_i + 1;
          x = last_breakable_x;
        }
        if (i == offset) {
          i++;
        }
        break;
      }

      x += char_w;
      word_break = word_break_check(*p, p[1]);
      if (word_break == LINE_BREAK_ALLOW && line_break == LINE_BREAK_ALLOW) {
        last_breakable_x = x;
        last_breakable_i = i;
      }
    } else {
      x += char_w;
    }
  }

  if (i == state->cursor && state->cursor == text->size) {
    if (last_char == STB_TEXTEDIT_NEWLINE) {
      text_edit_set_caret_pos(impl, 0, y + line_height, c->font_size);
    } else {
      text_edit_set_caret_pos(impl, x, y, c->font_size);
    }
  }

  if (last_char == STB_TEXTEDIT_NEWLINE) {
    impl->last_line_number = row_num + 1;
  } else {
    impl->last_line_number = row_num;
  }

  row->text_w = x;
  row->offset = offset;
  row->length = i - offset;
  layout_info->virtual_h = tk_max(y, layout_info->widget_h);

  return row;
}

static row_info_t* text_edit_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                         uint32_t offset) {
  DECL_IMPL(text_edit);
  if (impl->single_line) {
    return text_edit_single_line_layout_line(text_edit, row_num, offset);
  } else {
    return text_edit_multi_line_layout_line(text_edit, row_num, offset);
  }
}

static ret_t text_edit_layout_impl(text_edit_t* text_edit) {
  uint32_t i = 0;
  uint32_t offset = 0;
  DECL_IMPL(text_edit);
  row_info_t* iter = NULL;
  uint32_t max_rows = impl->rows->capacity;
  wstr_t* text = &(text_edit->widget->text);
  uint32_t size = text_edit->widget->text.size;
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t char_w = canvas_measure_text(text_edit->c, text->str, 1) + CHAR_SPACING;
  impl->caret.x = 0;
  impl->caret.y = 0;
  impl->rows->size = 0;

  if (text_edit->c == NULL) {
    return RET_OK;
  }

  widget_prepare_text_style(text_edit->widget, text_edit->c);
  impl->line_height = text_edit->c->font_size * FONT_BASELINE;

  widget_get_text_layout_info(text_edit->widget, layout_info);

  if (layout_info->w < char_w) {
    return RET_OK;
  }

  while ((offset < size || size == 0) && i < max_rows) {
    iter = text_edit_layout_line(text_edit, i, offset);
    if (iter == NULL || iter->length == 0) {
      break;
    }
    offset += iter->length;
    i++;
  }

  if (impl->state.cursor > offset && !impl->single_line) {
    impl->state.cursor = offset;
    impl->last_line_number = max_rows;
    text_edit_set_caret_pos(impl, iter->x, offset ? impl->line_height * (i - 1) : 0,
                            text_edit->c->font_size);
  } else if (offset < size) {
    text->size = offset;
    text->str[offset] = 0;
  }

  impl->rows->size = i;

  text_edit_notify(text_edit);

  return RET_OK;
}

ret_t text_edit_layout(text_edit_t* text_edit) {
  if (text_edit == NULL || text_edit->c == NULL || text_edit->widget == NULL ||
      text_edit->widget->initializing || text_edit->widget->loading) {
    return RET_BAD_PARAMS;
  }

  return text_edit_layout_impl(text_edit);
}

static void text_edit_layout_for_stb(StbTexteditRow* row, STB_TEXTEDIT_STRING* str, int offset) {
  DECL_IMPL(str);
  if (str->c == NULL) return;
  uint32_t font_size = str->c->font_size;
  row_info_t* info = rows_find_by_offset(impl->rows, offset);

  if (info != NULL) {
    row->x0 = info->x;
    row->x1 = info->x + info->text_w;
    row->num_chars = info->length;
  } else {
    row->x0 = 0;
    row->x1 = 0;
    row->num_chars = 1;
  }

  row->ymin = 0;
  row->ymax = font_size;
  row->baseline_y_delta = impl->line_height;

  return;
}

static ret_t text_edit_paint_caret(text_edit_t* text_edit, canvas_t* c) {
  DECL_IMPL(text_edit);
  widget_t* widget = text_edit->widget;
  style_t* style = widget->astyle;
  color_t black = color_init(0, 0, 0, 0xff);
  text_layout_info_t* layout_info = &(impl->layout_info);
  color_t caret_color = style_get_color(style, STYLE_ID_TEXT_COLOR, black);
  uint32_t x = layout_info->margin_l + impl->caret.x - layout_info->ox;
  uint32_t y = layout_info->margin_t + impl->caret.y - layout_info->oy;

  if (impl->single_line) {
    y += (layout_info->h - c->font_size) / 2;
  }

  canvas_set_stroke_color(c, caret_color);
  canvas_draw_vline(c, x, y, c->font_size);

  return RET_OK;
}

static ret_t text_edit_paint_tips_text(text_edit_t* text_edit, canvas_t* c) {
  DECL_IMPL(text_edit);
  wstr_t* text = &(impl->tips);
  text_layout_info_t* layout_info = &(impl->layout_info);

  if (text->size > 0) {
    rect_t r =
        rect_init(layout_info->margin_l, layout_info->margin_t, layout_info->w, layout_info->h);
    canvas_draw_text_in_rect(c, text->str, text->size, &r);
  }

  return RET_OK;
}

static int32_t text_edit_calc_x(text_edit_t* text_edit, row_info_t* iter) {
  DECL_IMPL(text_edit);
  canvas_t* c = text_edit->c;
  widget_t* widget = text_edit->widget;
  wstr_t* text = &(widget->text);
  text_layout_info_t* layout_info = &(impl->layout_info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);

  uint32_t row_width = text_edit_measure_text(c, text->str + iter->offset, 0, iter->length);
  if (row_width < layout_info->w) {
    switch (align_h) {
      case ALIGN_H_CENTER: {
        return (layout_info->w - row_width) / 2;
      }
      case ALIGN_H_RIGHT: {
        return (layout_info->w - row_width);
      }
      default: { break; }
    }
  }

  return 0;
}

static ret_t text_edit_paint_line(text_edit_t* text_edit, canvas_t* c, row_info_t* iter,
                                  uint32_t y) {
  uint32_t x = 0;
  uint32_t k = 0;
  widget_t* widget = text_edit->widget;

  DECL_IMPL(text_edit);
  wstr_t* text = &(widget->text);
  style_t* style = widget->astyle;
  STB_TexteditState* state = &(impl->state);
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t view_left = layout_info->ox + layout_info->margin_l;
  uint32_t view_right = layout_info->ox + layout_info->margin_l + layout_info->w;

  color_t black = color_init(0, 0, 0, 0xff);
  color_t white = color_init(0xf0, 0xf0, 0xf0, 0xff);

  uint32_t select_start = tk_min(state->select_start, state->select_end);
  uint32_t select_end = tk_max(state->select_start, state->select_end);

  if (impl->single_line) {
    x = layout_info->margin_l + text_edit_calc_x(text_edit, iter);
  } else {
    x = layout_info->margin_l;
  }

  for (k = 0; k < iter->length; k++) {
    uint32_t offset = iter->offset + k;
    wchar_t chr = impl->mask ? impl->mask_char : text->str[offset];
    uint32_t char_w = canvas_measure_text(c, &chr, 1);

    if ((x + char_w) < view_left) {
      x += char_w + CHAR_SPACING;
      continue;
    }

    if (x > view_right) {
      break;
    }

    if (chr != STB_TEXTEDIT_NEWLINE) {
      uint32_t rx = x - layout_info->ox;
      uint32_t ry = y - layout_info->oy;
      uint32_t cursor = state->cursor;

      bool_t selected = offset >= select_start && offset < select_end;
      bool_t preedit =
          impl->preedit && offset < cursor && offset >= (cursor - impl->preedit_chars_nr);

      if (selected || preedit) {
        color_t select_bg_color = style_get_color(style, STYLE_ID_SELECTED_BG_COLOR, white);
        color_t select_text_color = style_get_color(style, STYLE_ID_SELECTED_TEXT_COLOR, black);

        canvas_set_fill_color(c, select_bg_color);
        canvas_fill_rect(c, rx, ry, char_w + CHAR_SPACING, c->font_size);

        canvas_set_text_color(c, select_text_color);
      } else {
        color_t text_color = style_get_color(style, STYLE_ID_TEXT_COLOR, black);
        canvas_set_text_color(c, text_color);
      }

      canvas_draw_text(c, &chr, 1, rx, ry);
      x += char_w + CHAR_SPACING;
    }
  }

  return RET_OK;
}

static ret_t text_edit_paint_real_text(text_edit_t* text_edit, canvas_t* c) {
  uint32_t i = 0;
  uint32_t y = 0;
  DECL_IMPL(text_edit);
  rows_t* rows = impl->rows;
  uint32_t line_height = impl->line_height;
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t view_top = layout_info->oy + layout_info->margin_t;
  uint32_t view_bottom = layout_info->oy + layout_info->margin_t + layout_info->h;

  for (i = 0; i < rows->size; i++) {
    row_info_t* iter = rows->info + i;

    if (impl->single_line) {
      y = (layout_info->h - c->font_size) / 2 + layout_info->margin_t;
    } else {
      y = i * line_height + layout_info->margin_t;
    }

    if ((y + c->font_size) < view_top) {
      continue;
    }

    if (y > view_bottom) {
      break;
    }

    text_edit_paint_line(text_edit, c, iter, y);
  }

  return RET_OK;
}

static ret_t text_edit_paint_text(text_edit_t* text_edit, canvas_t* c) {
  widget_t* widget = text_edit->widget;

  if (widget->text.size > 0) {
    return text_edit_paint_real_text(text_edit, c);
  } else {
    return text_edit_paint_tips_text(text_edit, c);
  }
}

static ret_t text_edit_do_paint(text_edit_t* text_edit, canvas_t* c) {
  bool_t is_notify = FALSE;
  uint32_t new_line_height = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && c != NULL, RET_BAD_PARAMS);

  if (text_edit->c != NULL) {
    text_edit->c = c;
  } else {
    text_edit->c = c;
    text_edit_layout(text_edit);
  }

  widget_prepare_text_style(text_edit->widget, c);

  new_line_height = c->font_size * FONT_BASELINE;
  is_notify = impl->line_height != new_line_height;
  impl->line_height = new_line_height;

  if (text_edit_paint_text(text_edit, c) == RET_OK) {
    DECL_IMPL(text_edit);
    STB_TexteditState* state = &(impl->state);

    if (state->select_start == state->select_end && impl->caret_visible) {
      text_edit_paint_caret(text_edit, c);
    }
  }

  if (is_notify) {
    text_edit_layout(text_edit);
  }

  return RET_OK;
}

ret_t text_edit_paint(text_edit_t* text_edit, canvas_t* c) {
  rect_t save_r;
  rect_t clip_r;
  rect_t edit_r;
  DECL_IMPL(text_edit);
  text_layout_info_t* layout_info = &(impl->layout_info);

  if (text_edit->c != NULL) {
    text_edit->c = c;
  } else {
    text_edit->c = c;
    text_edit_layout(text_edit);
  }

  canvas_get_clip_rect(c, &save_r);

  edit_r = rect_init(c->ox + layout_info->margin_l, c->oy + layout_info->margin_t, layout_info->w,
                     layout_info->h);
  clip_r = rect_intersect(&save_r, &edit_r);

  canvas_save(c);
  canvas_set_clip_rect(c, &clip_r);
  text_edit_do_paint(text_edit, c);
  canvas_restore(c);

  canvas_set_clip_rect(c, &save_r);

  return RET_OK;
}

static int text_edit_remove(STB_TEXTEDIT_STRING* str, int pos, int num) {
  wstr_t* text = &(str->widget->text);
  wstr_remove(text, pos, num);

  return TRUE;
}

static int text_edit_get_char_width(STB_TEXTEDIT_STRING* str, int pos, int offset) {
  wstr_t* text = &(str->widget->text);
  wchar_t chr = text->str[pos + offset];

  if (chr == STB_TEXTEDIT_NEWLINE) {
    return STB_TEXTEDIT_GETWIDTH_NEWLINE;
  } else {
    return canvas_measure_text(str->c, &chr, 1) + CHAR_SPACING;
  }
}

static int text_edit_insert(STB_TEXTEDIT_STRING* str, int pos, STB_TEXTEDIT_CHARTYPE* newtext,
                            int num) {
  wstr_t* text = &(str->widget->text);
  wstr_insert(text, pos, newtext, num);

  return TRUE;
}

#define KEYDOWN_BIT 0x80000000
#define STB_TEXTEDIT_STRINGLEN(str) ((str)->widget->text.size)
#define STB_TEXTEDIT_LAYOUTROW text_edit_layout_for_stb
#define STB_TEXTEDIT_GETWIDTH(str, n, i) text_edit_get_char_width(str, n, i)
#define STB_TEXTEDIT_KEYTOTEXT(key) (((key)&KEYDOWN_BIT) ? 0 : (key))
#define STB_TEXTEDIT_GETCHAR(str, i) (((str)->widget->text).str[i])
#define STB_TEXTEDIT_IS_SPACE(ch) iswspace(ch)
#define STB_TEXTEDIT_DELETECHARS text_edit_remove
#define STB_TEXTEDIT_INSERTCHARS text_edit_insert

#define STB_TEXTEDIT_K_SHIFT 0x40000000
#define STB_TEXTEDIT_K_CONTROL 0x20000000
#define STB_TEXTEDIT_K_LEFT (KEYDOWN_BIT | 1)
#define STB_TEXTEDIT_K_RIGHT (KEYDOWN_BIT | 2)      // VK_RIGHT
#define STB_TEXTEDIT_K_UP (KEYDOWN_BIT | 3)         // VK_UP
#define STB_TEXTEDIT_K_DOWN (KEYDOWN_BIT | 4)       // VK_DOWN
#define STB_TEXTEDIT_K_LINESTART (KEYDOWN_BIT | 5)  // VK_HOME
#define STB_TEXTEDIT_K_LINEEND (KEYDOWN_BIT | 6)    // VK_END
#define STB_TEXTEDIT_K_TEXTSTART (STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_TEXTEND (STB_TEXTEDIT_K_LINEEND | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_DELETE (KEYDOWN_BIT | 7)     // VK_DELETE
#define STB_TEXTEDIT_K_BACKSPACE (KEYDOWN_BIT | 8)  // VK_BACKSPACE
#define STB_TEXTEDIT_K_UNDO (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'z')
#define STB_TEXTEDIT_K_REDO (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'y')
#define STB_TEXTEDIT_K_INSERT (KEYDOWN_BIT | 9)  // VK_INSERT
#define STB_TEXTEDIT_K_WORDLEFT (STB_TEXTEDIT_K_LEFT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_WORDRIGHT (STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_PGUP (KEYDOWN_BIT | 10)    // VK_PGUP -- not implemented
#define STB_TEXTEDIT_K_PGDOWN (KEYDOWN_BIT | 11)  // VK_PGDOWN -- not implemented

#define STB_TEXTEDIT_IMPLEMENTATION 1

#include "stb/stb_textedit.h"

text_edit_t* text_edit_create(widget_t* widget, bool_t single_line) {
  text_edit_impl_t* impl = NULL;
  return_value_if_fail(widget != NULL, NULL);

  impl = TKMEM_ZALLOC(text_edit_impl_t);
  return_value_if_fail(impl != NULL, NULL);

  impl->wrap_word = !single_line;
  impl->text_edit.widget = widget;
  impl->single_line = single_line;

  wstr_init(&(impl->tips), 0);
  stb_textedit_initialize_state(&(impl->state), single_line);
  if (!single_line) {
    text_edit_set_max_rows((text_edit_t*)impl, 100);
  } else {
    text_edit_set_max_rows((text_edit_t*)impl, 1);
  }

  return (text_edit_t*)impl;
}

ret_t text_edit_set_caret_visible(text_edit_t* text_edit, bool_t caret_visible) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit, RET_BAD_PARAMS);

  impl->caret_visible = caret_visible;

  return RET_OK;
}

ret_t text_edit_invert_caret_visible(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit, RET_BAD_PARAMS);

  impl->caret_visible = !impl->caret_visible;

  return RET_OK;
}

ret_t text_edit_set_max_rows(text_edit_t* text_edit, uint32_t max_rows) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && max_rows >= 1, RET_BAD_PARAMS);

  if (impl->rows != NULL) {
    rows_destroy(impl->rows);
    impl->rows = NULL;
  }

  if (impl->rows == NULL) {
    impl->rows = rows_create(max_rows);
  }

  return RET_OK;
}

ret_t text_edit_set_canvas(text_edit_t* text_edit, canvas_t* canvas) {
  return_value_if_fail(text_edit != NULL && canvas != NULL, RET_BAD_PARAMS);

  text_edit->c = canvas;
  text_edit_layout(text_edit);

  return RET_OK;
}

static point_t text_edit_normalize_point(text_edit_t* text_edit, xy_t x, xy_t y) {
  DECL_IMPL(text_edit);
  point_t point = {x, y};
  text_layout_info_t* layout_info = &(impl->layout_info);

  widget_to_local(text_edit->widget, &point);

  point.x = point.x - layout_info->margin_l + layout_info->ox;
  point.y = point.y - layout_info->margin_t + layout_info->oy;

  return point;
}

ret_t text_edit_click(text_edit_t* text_edit, xy_t x, xy_t y) {
  point_t point;
  DECL_IMPL(text_edit);
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, text_edit->c);
  point = text_edit_normalize_point(text_edit, x, y);
  stb_textedit_click(text_edit, &(impl->state), point.x, point.y);
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_drag(text_edit_t* text_edit, xy_t x, xy_t y) {
  point_t point;
  DECL_IMPL(text_edit);
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, text_edit->c);
  point = text_edit_normalize_point(text_edit, x, y);
  stb_textedit_drag(text_edit, &(impl->state), point.x, point.y);
  text_edit_layout(text_edit);

  return RET_OK;
}

static ret_t text_edit_handle_shortcut(text_edit_t* text_edit, key_event_t* evt,
                                       STB_TexteditState* state, wstr_t* text) {
#ifdef MACOS
  if (evt->cmd) {
#else
  if (evt->ctrl) {
#endif
    uint32_t key = evt->key;
    char c = tolower(key);
    if (c == 'z' || c == 'Z') {
      stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_UNDO);
    } else if (c == 'y' || c == 'Y') {
      stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_REDO);
    } else if (c == 'c' || c == 'C') {
      text_edit_copy(text_edit);
    } else if (c == 'x' || c == 'X') {
      text_edit_cut(text_edit);
    } else if (c == 'a' || c == 'A') {
      state->select_start = 0;
      state->select_end = text->size;
    } else if (c == 'v' || c == 'V') {
      value_t v;
      wstr_t str;
      const char* data = clip_board_get_text();
      if (data != NULL) {
        value_set_str(&v, data);
        wstr_init(&str, 0);
        wstr_from_value(&str, &v);
        wstr_normalize_newline(&str, STB_TEXTEDIT_NEWLINE);
        text_edit_paste(text_edit, str.str, str.size);
        wstr_reset(&str);
      }
    } else if (key > 128 || !isprint(key)) {
      return RET_FAIL;
    }

    text_edit_layout(text_edit);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t text_edit_key_up(text_edit_t* text_edit, key_event_t* evt) {
  uint32_t key = 0;
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  input_method_t* im = input_method();
  return_value_if_fail(im != NULL, RET_FAIL);
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && evt != NULL, RET_FAIL);

  key = evt->key;
  widget = text_edit->widget;
  if (key == TK_KEY_OPEN_INPUT_METHOD) {
    input_method_request(im, widget);
    ret = RET_STOP;
  } else if (key == TK_KEY_CLOSE_INPUT_METHOD) {
    input_method_request(im, NULL);
    ret = RET_STOP;
  } else if (key == TK_KEY_TOGGLE_INPUT_METHOD) {
    if (im->widget == widget) {
      input_method_request(im, NULL);
    } else {
      input_method_request(im, widget);
    }
    ret = RET_STOP;
  }

  return ret;
}

ret_t text_edit_key_down(text_edit_t* text_edit, key_event_t* evt) {
  uint32_t key = 0;
  wstr_t* text = NULL;
  DECL_IMPL(text_edit);
  STB_TexteditState* state = NULL;
  text_layout_info_t* layout_info = NULL;
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  key = evt->key;
  state = &(impl->state);
  text = &(text_edit->widget->text);
  layout_info = &(impl->layout_info);

  switch (key) {
#ifdef SDL2
    case TK_KEY_KP_DIVIDE:
    case TK_KEY_KP_MULTIPLY:
    case TK_KEY_KP_MINUS:
    case TK_KEY_KP_PLUS:
    case TK_KEY_KP_PERIOD:
    case TK_KEY_NUMLOCKCLEAR:
    case TK_KEY_KP_0:
    case TK_KEY_KP_1:
    case TK_KEY_KP_2:
    case TK_KEY_KP_3:
    case TK_KEY_KP_4:
    case TK_KEY_KP_5:
    case TK_KEY_KP_6:
    case TK_KEY_KP_7:
    case TK_KEY_KP_8:
    case TK_KEY_KP_9:
      return RET_OK;
    case TK_KEY_KP_ENTER:
#endif
    case TK_KEY_RETURN: {
      key = STB_TEXTEDIT_NEWLINE;
      break;
    }
    case TK_KEY_LEFT: {
      key = STB_TEXTEDIT_K_LEFT;
      break;
    }
    case TK_KEY_RIGHT: {
      key = STB_TEXTEDIT_K_RIGHT;
      break;
    }
    case TK_KEY_DOWN: {
      key = STB_TEXTEDIT_K_DOWN;
      break;
    }
    case TK_KEY_UP: {
      key = STB_TEXTEDIT_K_UP;
      break;
    }
    case TK_KEY_HOME: {
      key = STB_TEXTEDIT_K_LINESTART;
      break;
    }
    case TK_KEY_END: {
      key = STB_TEXTEDIT_K_LINEEND;
      break;
    }
    case TK_KEY_DELETE: {
      key = STB_TEXTEDIT_K_DELETE;
      break;
    }
    case TK_KEY_BACKSPACE: {
      key = STB_TEXTEDIT_K_BACKSPACE;
      break;
    }
    case TK_KEY_INSERT: {
      key = STB_TEXTEDIT_K_INSERT;
      break;
    }
    case TK_KEY_PAGEDOWN: {
      if (impl->single_line) {
        key = STB_TEXTEDIT_K_LINEEND;
      } else {
        int32_t lines = layout_info->h / impl->line_height;
        if ((layout_info->virtual_h - layout_info->oy) > layout_info->h) {
          key = STB_TEXTEDIT_K_DOWN;
          while (lines-- > 0) {
            stb_textedit_key(text_edit, state, key);
          }
          text_edit_layout(text_edit);
        }

        return RET_OK;
      }
      break;
    }
    case TK_KEY_PAGEUP: {
      if (impl->single_line) {
        key = STB_TEXTEDIT_K_LINESTART;
      } else {
        int32_t lines = tk_min(layout_info->oy, layout_info->h) / impl->line_height;

        if (lines > 0) {
          key = STB_TEXTEDIT_K_UP;
          while (lines-- > 0) {
            stb_textedit_key(text_edit, state, key);
          }
          text_edit_layout(text_edit);
        }

        return RET_OK;
      }
      break;
    }
    case TK_KEY_F1:
    case TK_KEY_F2:
    case TK_KEY_F3:
    case TK_KEY_F4:
    case TK_KEY_F5:
    case TK_KEY_F6:
    case TK_KEY_F7:
    case TK_KEY_F8:
    case TK_KEY_F9:
    case TK_KEY_F10:
    case TK_KEY_F11:
    case TK_KEY_F12:
    case TK_KEY_LSHIFT:
    case TK_KEY_RSHIFT:
    case TK_KEY_LCTRL:
    case TK_KEY_RCTRL:
    case TK_KEY_LALT:
    case TK_KEY_RALT:
    case TK_KEY_COMMAND:
    case TK_KEY_MENU: {
      return RET_OK;
    }
    default: {
      if (key < 128 && isprint(key)) {
        app_type_t app_type = system_info()->app_type;
        if (app_type == APP_DESKTOP || app_type == APP_MOBILE) {
          text_edit_handle_shortcut(text_edit, evt, state, text);
          return RET_OK;
        }
      }
      break;
    }
  }

  if (text_edit_handle_shortcut(text_edit, evt, state, text) == RET_OK) {
    return RET_OK;
  }

  if (evt->shift) {
    key |= STB_TEXTEDIT_K_SHIFT;
  }

  stb_textedit_key(text_edit, state, key);
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_copy(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  wstr_t* text = NULL;
  uint32_t select_end = 0;
  uint32_t select_start = 0;
  STB_TexteditState* state = NULL;
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  state = &(impl->state);
  text = &(text_edit->widget->text);
  select_start = tk_min(state->select_start, state->select_end);
  select_end = tk_max(state->select_start, state->select_end);

  if (select_end > select_start) {
    str_t str;
    wstr_t wstr;
    wchar_t* start = text->str + select_start;
    uint32_t size = select_end - select_start;

    wstr_init(&wstr, size + 1);
    wstr_append_with_len(&wstr, start, size);

    str_init(&str, 0);
    str_from_wstr(&str, wstr.str);
    clip_board_set_text(str.str);

    str_reset(&str);
    wstr_reset(&wstr);
  }

  return RET_OK;
}

ret_t text_edit_cut(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (text_edit_copy(text_edit) == RET_OK) {
    stb_textedit_cut(text_edit, &(impl->state));
  }

  return RET_OK;
}

ret_t text_edit_paste(text_edit_t* text_edit, const wchar_t* str, uint32_t size) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && str != NULL, RET_BAD_PARAMS);

  stb_textedit_paste(text_edit, &(impl->state), str, size);
  if (impl->preedit) {
    impl->preedit_chars_nr += size;
  }
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_set_cursor(text_edit_t* text_edit, uint32_t cursor) {
  wstr_t* text = NULL;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  text = &(text_edit->widget->text);

  if (cursor > text->size) {
    cursor = text->size;
  }

  if (impl->state.cursor != cursor) {
    impl->state.cursor = cursor;
    text_edit_layout(text_edit);
  }

  return RET_OK;
}

uint32_t text_edit_get_cursor(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, 0);

  return impl->state.cursor;
}

ret_t text_edit_set_wrap_word(text_edit_t* text_edit, bool_t wrap_word) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->wrap_word = wrap_word;
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_set_mask(text_edit_t* text_edit, bool_t mask) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (impl->mask != mask) {
    impl->mask = mask;
    text_edit_layout(text_edit);
  }

  return RET_OK;
}

ret_t text_edit_set_tips(text_edit_t* text_edit, const char* tips) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  wstr_set_utf8(&(impl->tips), tips);
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_set_mask_char(text_edit_t* text_edit, wchar_t mask_char) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->mask_char = mask_char;
  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_set_select(text_edit_t* text_edit, uint32_t start, uint32_t end) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (start > end) {
    uint32_t t = start;
    start = end;
    end = t;
  }

  impl->state.select_start = start;
  impl->state.select_end = tk_min(end, text_edit->widget->text.size);

  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_select_all(text_edit_t* text_edit) {
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  return text_edit_set_select(text_edit, 0, text_edit->widget->text.size);
}

ret_t text_edit_unselect(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->state.select_end = impl->state.select_start;

  text_edit_layout(text_edit);

  return RET_OK;
}

ret_t text_edit_get_state(text_edit_t* text_edit, text_edit_state_t* state) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && state != NULL, RET_BAD_PARAMS);

  memset(state, 0x00, sizeof(text_edit_state_t));

  state->ox = impl->layout_info.ox;
  state->oy = impl->layout_info.oy;
  state->virtual_w = impl->layout_info.virtual_w;
  state->virtual_h = impl->layout_info.virtual_h;
  state->rows = impl->rows->size;
  state->caret = impl->caret;
  state->preedit = impl->preedit;
  state->line_height = impl->line_height;

  state->cursor = impl->state.cursor;
  state->max_rows = impl->rows->capacity;
  state->last_line_number = impl->last_line_number;

  state->select_start = tk_min(impl->state.select_start, impl->state.select_end);
  state->select_end = tk_max(impl->state.select_start, impl->state.select_end);

  state->mask = impl->mask;
  state->wrap_word = impl->wrap_word;
  state->mask_char = impl->mask_char;
  state->caret_visible = impl->caret_visible;
  state->single_line = impl->single_line;

  return RET_OK;
}

ret_t text_edit_destroy(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  wstr_reset(&(impl->tips));
  rows_destroy(impl->rows);
  TKMEM_FREE(text_edit);

  return RET_OK;
}

ret_t text_edit_set_offset(text_edit_t* text_edit, int32_t ox, int32_t oy) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->layout_info.ox = ox;
  impl->layout_info.oy = oy;
  text_edit_notify(text_edit);

  return RET_OK;
}

static ret_t text_edit_notify(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  if (impl->on_state_changed != NULL) {
    text_edit_state_t state = {0};
    text_edit_get_state(text_edit, &state);
    impl->on_state_changed(impl->on_state_changed_ctx, &state);
  }

  return RET_OK;
}

ret_t text_edit_set_on_state_changed(text_edit_t* text_edit,
                                     text_edit_on_state_changed_t on_state_changed, void* ctx) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->on_state_changed = on_state_changed;
  impl->on_state_changed_ctx = ctx;

  return RET_OK;
}

ret_t text_edit_preedit(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->preedit = TRUE;
  impl->preedit_chars_nr = 0;

  return RET_OK;
}

ret_t text_edit_preedit_confirm(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->preedit = FALSE;
  impl->preedit_chars_nr = 0;

  return RET_OK;
}

ret_t text_edit_preedit_clear(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (impl->preedit_chars_nr > 0) {
    text_edit_remove(text_edit, impl->state.cursor - impl->preedit_chars_nr,
                     impl->preedit_chars_nr);
    impl->state.cursor = impl->state.cursor - impl->preedit_chars_nr;
    impl->preedit_chars_nr = 0;
  }

  return RET_OK;
}

ret_t text_edit_preedit_abort(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->preedit = FALSE;
  text_edit_preedit_clear(text_edit);

  return RET_OK;
}

/**
 * File:   text_edit.c
 * Author: AWTK Develop Team
 * Brief:  text_edit
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
 * 2019-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <wctype.h>

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "base/bidi.h"
#include "base/line_parser.h"
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

#define GET_CANVAS(text_edit) widget_get_canvas(WIDGET(text_edit->widget))
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

typedef struct _line_info_t {
  uint16_t x;
  uint32_t offset;
  uint16_t length;
  uint16_t text_w;
} line_info_t;

typedef struct _row_info_t {
  uint16_t length;
  uint32_t line_num;
  darray_t info;
} row_info_t;

typedef struct _rows_t {
  uint32_t size;
  uint32_t capacity;
  uint32_t* row_line;
  row_info_t row[1];
} rows_t;

typedef struct _text_edit_impl_t {
  text_edit_t text_edit;
  STB_TexteditState state;

  rows_t* rows;
  uint32_t max_chars;
  point_t caret;
  bool_t overwrite;
  bool_t wrap_word;
  bool_t single_line;
  bool_t caret_visible;
  uint32_t line_height;
  uint32_t last_line_number;
  uint32_t last_row_number;
  uint32_t caret_line_index;
  text_layout_info_t layout_info;

  bool_t preedit;
  uint32_t preedit_chars_nr;

  bool_t lock_scrollbar_value;

  /*for single line edit*/
  wchar_t mask_char;
  bool_t mask;
  wstr_t tips;
  bool_t is_mlines;

  bool_t is_first_time_layout;
  int32_t font_size;
  const char* font_name;
  void* on_state_changed_ctx;
  text_edit_on_state_changed_t on_state_changed;
} text_edit_impl_t;

#define DECL_IMPL(te) text_edit_impl_t* impl = (text_edit_impl_t*)(te)

static ret_t text_edit_notify(text_edit_t* text_edit);
static bool_t text_edit_is_need_layout(text_edit_t* text_edit);
static int32_t text_edit_calc_x(text_edit_t* text_edit, line_info_t* iter);

static align_h_t widget_get_text_align_h(widget_t* widget) {
  return (align_h_t)style_get_int(widget->astyle, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_LEFT);
}

#define TEXT_EDIT_GET_WIDGET_MARGIN(widget, style, out_value, type)             \
  {                                                                             \
    value_t v;                                                                  \
    value_set_int(&v, 0);                                                       \
    if (widget_get_prop((widget), WIDGET_PROP_##type##_MARGIN, &v) == RET_OK) { \
      (out_value) = value_int(&v);                                              \
    }                                                                           \
    TEXT_EDIT_GET_STYLE_MARGIN(style, out_value, type)                          \
  }

static ret_t widget_get_text_layout_info(widget_t* widget, text_layout_info_t* info) {
  style_t* style = widget->astyle;
  return_value_if_fail(widget != NULL && info != NULL && style != NULL, RET_BAD_PARAMS);

  info->widget_w = widget->w;
  info->widget_h = widget->h;
  info->virtual_w = tk_max(info->virtual_w, widget->w);
  info->virtual_h = tk_max(info->virtual_h, widget->h);

  TEXT_EDIT_GET_WIDGET_MARGIN(widget, style, info->margin_l, LEFT);
  TEXT_EDIT_GET_WIDGET_MARGIN(widget, style, info->margin_r, RIGHT);
  TEXT_EDIT_GET_WIDGET_MARGIN(widget, style, info->margin_t, TOP);
  TEXT_EDIT_GET_WIDGET_MARGIN(widget, style, info->margin_b, BOTTOM);

  info->w = info->widget_w - info->margin_l - info->margin_r;
  info->h = info->widget_h - info->margin_t - info->margin_b;

  return RET_OK;
}

static rows_t* rows_create(uint32_t capacity) {
  uint32_t msize = sizeof(rows_t) + capacity * sizeof(row_info_t);
  rows_t* rows = (rows_t*)TKMEM_ALLOC(msize);
  uint32_t i = 0;
  return_value_if_fail(rows != NULL, NULL);

  memset(rows, 0x00, msize);
  for (i = 0; i < capacity; i++) {
    darray_init(&rows->row[i].info, 4, default_destroy, NULL);
    darray_push(&rows->row[i].info, TKMEM_ZALLOC(line_info_t));
    rows->row[i].line_num = 1;
  }

  rows->row_line = TKMEM_ZALLOCN(uint32_t, capacity);
  rows->capacity = capacity;

  return rows;
}

static line_info_t* line_find_by_offset(rows_t* rows, uint32_t offset) {
  uint32_t i = 0;
  return_value_if_fail(rows != NULL, NULL);

  for (i = 0; i < rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = rows->row + i;
    for (j = 0; j < row->line_num; j++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      if (line->offset == offset) {
        return line;
      }
    }
  }

  return NULL;
}

static ret_t rows_destroy(rows_t* rows) {
  uint32_t i = 0;
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  for (i = 0; i < rows->capacity; i++) {
    darray_deinit(&rows->row[i].info);
  }
  TKMEM_FREE(rows->row_line);
  TKMEM_FREE(rows);

  return RET_OK;
}

static ret_t text_edit_set_caret_pos(text_edit_impl_t* impl, uint32_t x, uint32_t y,
                                     uint32_t font_size, uint32_t line_number) {
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
  impl->caret_line_index = line_number;

  if (!impl->lock_scrollbar_value) {
    if (view_top > caret_top) {
      layout_info->oy = caret_top - layout_info->margin_t;
    }

    if (view_bottom < caret_bottom) {
      layout_info->oy = caret_bottom - layout_info->h;
    }

    if (view_left > caret_left) {
      layout_info->ox = caret_left - layout_info->margin_l;
    }

    if (view_right < caret_right || (view_left > caret_left + layout_info->w)) {
      layout_info->ox = caret_right - layout_info->w - layout_info->margin_l;
    }
  }

  if (layout_info->ox < 0) {
    layout_info->ox = 0;
  }

  if (impl->wrap_word) {
    layout_info->ox = 0;
  }

  if (impl->single_line) {
    layout_info->oy = 0;
  }

  return RET_OK;
}

static uint32_t text_edit_measure_text_on_canvas(text_edit_t* text_edit, wchar_t* str,
                                                 wchar_t mask_char, uint32_t size, canvas_t* c) {
  uint32_t i = 0;
  uint32_t w = 0;
  DECL_IMPL(text_edit);

  for (i = 0; i < size; i++) {
    bool_t preedit = impl->preedit && i < impl->state.cursor &&
                     i >= (impl->state.cursor - impl->preedit_chars_nr);
    wchar_t chr = (mask_char && !preedit) ? mask_char : str[i];

    w += canvas_measure_text(c, &chr, 1) + CHAR_SPACING;
  }

  return w;
}

static uint32_t text_edit_measure_text(text_edit_t* text_edit, wchar_t* str, wchar_t mask_char,
                                       uint32_t size) {
  return text_edit_measure_text_on_canvas(text_edit, str, mask_char, size, GET_CANVAS(text_edit));
}

static row_info_t* text_edit_single_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                     uint32_t line_index, uint32_t offset) {
  uint32_t y = 0;
  uint32_t caret_x = 0;
  uint32_t view_left = 0;
  uint32_t caret_left = 0;
  DECL_IMPL(text_edit);
  canvas_t* c = GET_CANVAS(text_edit);
  wstr_t* text = &(text_edit->widget->text);
  STB_TexteditState* state = &(impl->state);
  row_info_t* row = impl->rows->row + row_num;
  wchar_t mask_char = impl->mask ? impl->mask_char : 0;
  text_layout_info_t* layout_info = &(impl->layout_info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);
  uint32_t text_w = text_edit_measure_text(text_edit, text->str, mask_char, text->size);
  uint32_t caret_text_w = text_edit_measure_text(text_edit, text->str, mask_char, state->cursor);
  line_info_t* line = (line_info_t*)darray_head(&row->info);

  assert(offset == 0 && row_num == 0);

  memset(row, 0x00, sizeof(row_info_t) - sizeof(darray_t));

  row->line_num = 1;
  line->offset = 0;
  line->text_w = text_w;
  line->length = text->size;
  row->length = line->length;
  line->x = text_edit_calc_x(text_edit, line);
  layout_info->virtual_h = tk_max(y, layout_info->widget_h);

  caret_x = caret_text_w;
  caret_left = layout_info->margin_l + caret_x;
  view_left = layout_info->ox + layout_info->margin_l;
  if ((text_w < layout_info->w) ||
      (view_left + c->font_size >= caret_left && state->cursor == text->size)) {
    layout_info->ox = 0;
    if (align_h == ALIGN_H_RIGHT) {
      caret_x = layout_info->w - (text_w - caret_text_w);
    } else if (align_h == ALIGN_H_CENTER) {
      caret_x = (layout_info->w - text_w) / 2 + caret_text_w;
    }
  }
  text_edit_set_caret_pos(impl, caret_x, y, c->font_size, line_index);

  return row;
}

static row_info_t* text_edit_multi_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                    uint32_t line_index, uint32_t offset) {
  uint32_t i = 0;
  uint32_t x = 0;
  DECL_IMPL(text_edit);
  wchar_t last_char = 0;
  point_t caret = {-1, -1};
  canvas_t* c = GET_CANVAS(text_edit);
  wstr_t* text = &(text_edit->widget->text);
  STB_TexteditState* state = &(impl->state);
  row_info_t* row = impl->rows->row + row_num;
  uint32_t line_height = impl->line_height;
  uint32_t y = line_index * line_height;
  uint32_t offset0 = offset;
  uint32_t last_breakable_i = 0;
  uint32_t last_breakable_x = 0;
  line_info_t* last_line = NULL;
  text_layout_info_t* layout_info = &(impl->layout_info);

  memset(row, 0x00, sizeof(row_info_t) - sizeof(darray_t));
  row->line_num = 1;

  for (i = offset; i < text->size; i++) {
    wchar_t* p = text->str + i;
    break_type_t word_break = LINE_BREAK_NO;
    break_type_t line_break = LINE_BREAK_NO;
    uint32_t char_w = canvas_measure_text(c, p, 1) + CHAR_SPACING;

    if (i == state->cursor) {
      caret.x = x;
      caret.y = y;
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
          last_breakable_x = 0;
        }
        if (i == offset) {
          i++;
        }

        last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
        memset(last_line, 0x00, sizeof(line_info_t));
        last_line->text_w = x;
        last_line->offset = offset;
        last_line->length = i - offset;

        row->line_num++;
        if (row->info.size < row->line_num) {
          darray_push(&row->info, TKMEM_ZALLOC(line_info_t));
        }

        p = text->str + i;
        char_w = canvas_measure_text(c, p, 1) + CHAR_SPACING;

        x = char_w;
        y += line_height;
        offset = i;
        line_index++;
        if (state->cursor == i) {
          caret.x = 0;
          caret.y = y;
        }
        continue;
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

  while (row->info.size > row->line_num) {
    row->info.destroy(darray_pop(&row->info));
  }

  if (last_char == STB_TEXTEDIT_NEWLINE) {
    impl->last_row_number = row_num + 1;
    impl->last_line_number = line_index + 1;
  } else {
    impl->last_row_number = row_num;
    impl->last_line_number = line_index;
  }

  if (i == state->cursor && state->cursor == text->size) {
    if (last_char == STB_TEXTEDIT_NEWLINE) {
      caret.x = 0;
      caret.y = y + line_height;
    } else {
      caret.x = x;
      caret.y = y;
    }
  }
  if (caret.x >= 0 && caret.y >= 0) {
    /* 计算好了再统一修改光标坐标，以免多次修改导致滚动条的位置突变 */
    text_edit_set_caret_pos(impl, caret.x, caret.y, c->font_size, line_index);
  }

  last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
  memset(last_line, 0x00, sizeof(line_info_t));
  last_line->text_w = x;
  last_line->offset = offset;
  last_line->length = i - offset;

  row->length = i - offset0;
  layout_info->virtual_h = tk_max(y + line_height, layout_info->widget_h);

  return row;
}

static ret_t text_edit_fix_oy(text_edit_impl_t* impl) {
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t total_line_height = (impl->last_line_number + 1) * impl->line_height;
  uint32_t max_oy = (total_line_height > layout_info->h) ? total_line_height - layout_info->h : 0;

  layout_info->oy = tk_min(layout_info->oy, max_oy);

  return RET_OK;
}

static row_info_t* text_edit_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                         uint32_t line_index, uint32_t offset) {
  DECL_IMPL(text_edit);
  if (impl->single_line) {
    return text_edit_single_line_layout_line(text_edit, row_num, line_index, offset);
  } else {
    return text_edit_multi_line_layout_line(text_edit, row_num, line_index, offset);
  }
}

static ret_t text_edit_layout_impl(text_edit_t* text_edit) {
  uint32_t i = 0;
  uint32_t offset = 0;
  DECL_IMPL(text_edit);
  row_info_t* iter = NULL;
  canvas_t* c = GET_CANVAS(text_edit);
  uint32_t max_rows = impl->rows->capacity;
  wstr_t* text = &(text_edit->widget->text);
  uint32_t size = text_edit->widget->text.size;
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t char_w = 0;
  uint32_t line_index = 0;
  impl->caret.x = 0;
  impl->caret.y = 0;
  impl->rows->size = 0;

  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, c);
  impl->line_height = c->font_size * FONT_BASELINE;
  widget_get_text_layout_info(text_edit->widget, layout_info);
  char_w = canvas_measure_text(c, text->str, 1) + CHAR_SPACING;

  if (layout_info->w < char_w) {
    return RET_OK;
  }

  while ((offset < size || size == 0) && i < max_rows) {
    iter = text_edit_layout_line(text_edit, i, line_index, offset);
    if (iter == NULL || iter->length == 0) {
      break;
    }
    line_index += iter->line_num;
    offset += iter->length;
    i++;
  }

  if (offset < size) {
    text->size = offset;
    text->str[offset] = L'\0';
  }

  impl->rows->size = i;

  text_edit_fix_oy(impl);

  text_edit_notify(text_edit);

  return RET_OK;
}

ret_t text_edit_layout(text_edit_t* text_edit) {
  if (text_edit == NULL || GET_CANVAS(text_edit) == NULL || text_edit->widget == NULL ||
      text_edit->widget->initializing || text_edit->widget->loading) {
    return RET_BAD_PARAMS;
  }

  return text_edit_layout_impl(text_edit);
}

static void text_edit_layout_for_stb(StbTexteditRow* row, STB_TEXTEDIT_STRING* str, int offset) {
  DECL_IMPL(str);
  canvas_t* c = GET_CANVAS(str);
  if (c == NULL) return;
  uint32_t font_size = c->font_size;
  line_info_t* info = line_find_by_offset(impl->rows, offset);

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

static ret_t text_edit_paint_tips_mlines_text(text_edit_t* text_edit, canvas_t* c,
                                              line_parser_t* p) {
  int32_t y = 0;
  int32_t w = 0;
  int32_t font_size = 0;
  int32_t line_height = 0;
  DECL_IMPL(text_edit);
  text_layout_info_t* layout_info = 0;
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && c != NULL, RET_BAD_PARAMS);

  font_size = c->font_size;
  layout_info = &(impl->layout_info);
  line_height = font_size + style_get_int(text_edit->widget->astyle, STYLE_ID_SPACER, 2);

  w = layout_info->w;
  y = layout_info->margin_t;
  while (line_parser_next(p) == RET_OK) {
    uint32_t size = 0;
    rect_t r = rect_init(layout_info->margin_l, y, w, font_size);

    if ((y + font_size) > layout_info->h) {
      break;
    }

    for (size = 0; size < p->line_size; size++) {
      if (CHAR_IS_LINE_BREAK(p->line[size])) {
        break;
      }
    }
    canvas_draw_text_in_rect(c, p->line, size, &r);

    y += line_height;
  }
  return RET_OK;
}

static ret_t text_edit_paint_tips_text(text_edit_t* text_edit, canvas_t* c) {
  DECL_IMPL(text_edit);
  wstr_t* text = &(impl->tips);
  text_layout_info_t* layout_info = &(impl->layout_info);

  if (text->size > 0) {
    if (impl->is_mlines) {
      line_parser_t p;
      line_parser_init(&p, c, (const wchar_t*)(text->str), text->size, c->font_size, layout_info->w,
                       TRUE, TRUE);
      if (p.total_lines > 1) {
        text_edit_paint_tips_mlines_text(text_edit, c, &p);
      } else {
        align_h_t align_h = c->text_align_h;
        align_v_t align_v = c->text_align_v;
        rect_t r =
            rect_init(layout_info->margin_l, layout_info->margin_t, layout_info->w, layout_info->h);
        canvas_set_text_align(c, align_h, ALIGN_V_TOP);
        canvas_draw_text_in_rect(c, text->str, text->size, &r);
        canvas_set_text_align(c, align_h, align_v);
      }
      line_parser_deinit(&p);
    } else {
      rect_t r =
          rect_init(layout_info->margin_l, layout_info->margin_t, layout_info->w, layout_info->h);
      canvas_draw_text_in_rect(c, text->str, text->size, &r);
    }
  }

  return RET_OK;
}

static int32_t text_edit_calc_x_on_canvas(text_edit_t* text_edit, line_info_t* iter, canvas_t* c) {
  DECL_IMPL(text_edit);
  widget_t* widget = text_edit->widget;
  wstr_t* text = &(widget->text);
  wchar_t chr = impl->mask ? impl->mask_char : 0;
  text_layout_info_t* layout_info = &(impl->layout_info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);

  uint32_t row_width =
      text_edit_measure_text_on_canvas(text_edit, text->str + iter->offset, chr, iter->length, c);
  if (row_width < layout_info->w) {
    switch (align_h) {
      case ALIGN_H_CENTER: {
        return (layout_info->w - row_width) / 2;
      }
      case ALIGN_H_RIGHT: {
        return (layout_info->w - row_width);
      }
      default: {
        break;
      }
    }
  }

  return 0;
}

static int32_t text_edit_calc_x(text_edit_t* text_edit, line_info_t* iter) {
  return text_edit_calc_x_on_canvas(text_edit, iter, GET_CANVAS(text_edit));
}

static ret_t text_edit_paint_line(text_edit_t* text_edit, canvas_t* c, line_info_t* iter,
                                  uint32_t y) {
  bidi_t b;
  uint32_t x = 0;
  uint32_t k = 0;
  widget_t* widget = text_edit->widget;
  const char* bidi_type = widget_get_prop_str(widget, WIDGET_PROP_BIDI, NULL);
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
    x = layout_info->margin_l + text_edit_calc_x_on_canvas(text_edit, iter, c);
  } else {
    x = layout_info->margin_l;
  }

  bidi_init(&b, FALSE, FALSE, bidi_type_from_name(bidi_type));
  ENSURE(bidi_log2vis(&b, text->str + iter->offset, iter->length) == RET_OK);

  for (k = 0; k < iter->length; k++) {
    uint32_t offset = iter->offset + k;
    uint32_t cursor = state->cursor;
    bool_t selected = offset >= select_start && offset < select_end;
    bool_t preedit =
        impl->preedit && offset < cursor && offset >= (cursor - impl->preedit_chars_nr);
    wchar_t chr = (impl->mask && !preedit) ? impl->mask_char : b.vis_str[k];
    uint32_t char_w = canvas_measure_text(c, &chr, 1);

    if ((x + char_w) < view_left) {
      x += char_w + CHAR_SPACING;
      continue;
    }

    if (x > view_right) {
      break;
    }

    if (chr != STB_TEXTEDIT_NEWLINE) {
      xy_t rx = x - layout_info->ox;
      xy_t ry = y - layout_info->oy;

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

      /*FIXME: 密码编辑时，*字符本身偏高，看起来不像居中。但是无法拿到字模信息，只好手工修正一下。*/
      if (impl->mask && !preedit && impl->mask_char == '*') {
        int32_t oy = c->font_size / 6;
        canvas_draw_text(c, &chr, 1, rx, ry + oy);
      } else {
        canvas_draw_text(c, &chr, 1, rx, ry);
      }

      x += char_w + CHAR_SPACING;
    }
  }
  bidi_deinit(&b);

  return RET_OK;
}

static ret_t text_edit_paint_real_text(text_edit_t* text_edit, canvas_t* c) {
  DECL_IMPL(text_edit);
  rows_t* rows = impl->rows;
  uint32_t line_height = impl->line_height;
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t view_top = layout_info->oy + layout_info->margin_t;
  uint32_t view_bottom = layout_info->oy + layout_info->margin_t + layout_info->h;

  uint32_t i = 0;
  uint32_t k = 0;
  for (i = 0; i < rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = rows->row + i;

    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      uint32_t y = 0;

      if (impl->single_line) {
        y = (layout_info->h - c->font_size) / 2 + layout_info->margin_t;

      } else {
        y = k * line_height + layout_info->margin_t;
      }

      if ((y + c->font_size) < view_top) {
        continue;
      }

      if (y > view_bottom) {
        break;
      }

      text_edit_paint_line(text_edit, c, line, y);
    }
  }

  return RET_OK;
}

static ret_t text_edit_paint_text(text_edit_t* text_edit, canvas_t* c) {
  widget_t* widget = text_edit->widget;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->text.size > 0) {
    return text_edit_paint_real_text(text_edit, c);
  } else {
    color_t trans =
        style_get_color(widget->astyle, STYLE_ID_TEXT_COLOR, color_init(0x0, 0x0, 0x0, 0x0));
    color_t tc = style_get_color(widget->astyle, STYLE_ID_TIPS_TEXT_COLOR, trans);
    align_h_t align_h =
        (align_h_t)style_get_int(widget->astyle, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_LEFT);
    align_v_t align_v =
        (align_v_t)style_get_int(widget->astyle, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);
    canvas_set_text_color(c, tc);
    canvas_set_text_align(c, align_h, align_v);
    return text_edit_paint_tips_text(text_edit, c);
  }
}

static ret_t text_edit_do_paint(text_edit_t* text_edit, canvas_t* c) {
  bool_t is_notify = FALSE;
  uint32_t new_line_height = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && c != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, c);
  widget_get_text_layout_info(text_edit->widget, &(impl->layout_info));

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
  style_t* style = text_edit->widget->astyle;

  if (impl->is_first_time_layout) {
    text_edit_layout(text_edit);
    impl->is_first_time_layout = FALSE;

    impl->font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
    impl->font_name = system_info_fix_font_name(style_get_str(style, STYLE_ID_FONT_NAME, NULL));
  } else {
    if (text_edit_is_need_layout(text_edit)) {
      text_edit_layout(text_edit);
    }
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
    return canvas_measure_text(GET_CANVAS(str), &chr, 1) + CHAR_SPACING;
  }
}

static int text_edit_insert(STB_TEXTEDIT_STRING* str, int pos, STB_TEXTEDIT_CHARTYPE* newtext,
                            int num) {
  bool_t ret = FALSE;
  wstr_t* text = &(str->widget->text);
  DECL_IMPL(str);
  uint32_t line_break_num = impl->rows->capacity > impl->last_row_number
                                ? impl->rows->capacity - impl->last_row_number
                                : 0;
  uint32_t i = 0;

  for (i = 0; i < num; i++) {
    if (i + 1 < num && TWINS_WCHAR_IS_LINE_BREAK(newtext[i], newtext[i + 1])) {
      line_break_num--;
      i++;
    } else if (WCHAR_IS_LINE_BREAK(newtext[i])) {
      line_break_num--;
    }
    if (line_break_num == 0) {
      num = i;
      break;
    }
  }

  if (impl->max_chars > 0) {
    uint32_t surplus_char = impl->max_chars > text->size ? impl->max_chars - text->size : 0;
    num = tk_min(num, surplus_char);
  }

  if (num > 0) {
    wstr_insert(text, pos, newtext, num);
    ret = TRUE;
  }

  return (int)ret;
}

#define KEYDOWN_BIT 0x80000000
#define STB_TEXTEDIT_STRINGLEN(str) ((str)->widget->text.size)
#define STB_TEXTEDIT_LAYOUTROW text_edit_layout_for_stb
#define STB_TEXTEDIT_GETWIDTH(str, n, i) text_edit_get_char_width(str, n, i)
#define STB_TEXTEDIT_KEYTOTEXT(key) (((key)&KEYDOWN_BIT) ? 0 : ((uint16_t)key))
#define STB_TEXTEDIT_GETCHAR(str, i) (((str)->widget->text).str[i])
#define STB_TEXTEDIT_IS_SPACE(ch) iswspace(ch)
#define STB_TEXTEDIT_DELETECHARS text_edit_remove
#define STB_TEXTEDIT_INSERTCHARS text_edit_insert

#define STB_TEXTEDIT_K_SHIFT 0x40000000
#define STB_TEXTEDIT_K_CONTROL 0x20000000
#define STB_TEXTEDIT_K_LEFT (KEYDOWN_BIT | 1)
#define STB_TEXTEDIT_K_RIGHT (KEYDOWN_BIT | 2)  // VK_RIGHT
#define STB_TEXTEDIT_K_UP (KEYDOWN_BIT | 3)  // VK_UP
#define STB_TEXTEDIT_K_DOWN (KEYDOWN_BIT | 4)  // VK_DOWN
#define STB_TEXTEDIT_K_LINESTART (KEYDOWN_BIT | 5)  // VK_HOME
#define STB_TEXTEDIT_K_LINEEND (KEYDOWN_BIT | 6)  // VK_END
#define STB_TEXTEDIT_K_TEXTSTART (STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_TEXTEND (STB_TEXTEDIT_K_LINEEND | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_DELETE (KEYDOWN_BIT | 7)  // VK_DELETE
#define STB_TEXTEDIT_K_BACKSPACE (KEYDOWN_BIT | 8)  // VK_BACKSPACE
#define STB_TEXTEDIT_K_UNDO (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'z')
#define STB_TEXTEDIT_K_REDO (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'y')
#define STB_TEXTEDIT_K_INSERT (KEYDOWN_BIT | 9)  // VK_INSERT
#define STB_TEXTEDIT_K_WORDLEFT (STB_TEXTEDIT_K_LEFT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_WORDRIGHT (STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_PGUP (KEYDOWN_BIT | 10)  // VK_PGUP -- not implemented
#define STB_TEXTEDIT_K_PGDOWN (KEYDOWN_BIT | 11)  // VK_PGDOWN -- not implemented

#define STB_TEXTEDIT_IMPLEMENTATION 1

#include "stb/stb_textedit.h"

text_edit_t* text_edit_create(widget_t* widget, bool_t single_line) {
  text_edit_impl_t* impl = NULL;
  return_value_if_fail(widget != NULL, NULL);

  impl = TKMEM_ZALLOC(text_edit_impl_t);
  return_value_if_fail(impl != NULL, NULL);

  impl->is_first_time_layout = TRUE;
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

ret_t text_edit_set_max_chars(text_edit_t* text_edit, uint32_t max_chars) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->max_chars = max_chars;

  return RET_OK;
}

uint32_t text_edit_get_height(text_edit_t* text_edit, uint32_t offset) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, 0);

  for (i = 0; i < impl->rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = impl->rows->row + i;
    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      if (offset < line->offset + line->length) {
        return impl->line_height * k;
      }
    }
  }

  return impl->line_height * ((k > 1) ? k - 1 : 0);
}

const uint32_t* text_edit_get_lines_of_each_row(text_edit_t* text_edit) {
  uint32_t* lines_of_each_row = NULL;
  uint32_t size = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && impl != NULL && impl->rows != NULL, NULL);

  size = impl->rows->capacity;

  if (size) {
    uint32_t i = 0;
    lines_of_each_row = impl->rows->row_line;
    memset(lines_of_each_row, 0x00, sizeof(uint32_t) * size);

    for (i = 0; i < impl->rows->size; i++) {
      lines_of_each_row[i] = impl->rows->row[i].line_num;
    }
  }

  return lines_of_each_row;
}

static uint32_t text_edit_get_line_break_offset(text_edit_t* text_edit, uint32_t num) {
  uint32_t offset = 0;
  uint32_t i = 0;
  wstr_t* text = NULL;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL, -1);
  return_value_if_fail(0 < num && num < impl->rows->capacity, -1);

  if (num >= impl->rows->size) {
    return -1;
  }

  text = &text_edit->widget->text;

  for (i = 0; i < num; i++) {
    offset += impl->rows->row[i].length;
  }

  if (offset >= 2 && TWINS_WCHAR_IS_LINE_BREAK(text->str[offset - 2], text->str[offset - 1])) {
    offset -= 2;
  } else if (offset >= 1 && WCHAR_IS_LINE_BREAK(text->str[offset - 1])) {
    offset--;
  } else {
    offset = -1;
  }

  return offset;
}

ret_t text_edit_set_canvas(text_edit_t* text_edit, canvas_t* canvas) {
  return_value_if_fail(text_edit != NULL && canvas != NULL, RET_BAD_PARAMS);

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

static bool_t text_edit_is_need_layout(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  style_t* style = text_edit->widget->astyle;
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  const char* font_name = system_info_fix_font_name(style_get_str(style, STYLE_ID_FONT_NAME, NULL));

  if (!tk_str_eq(font_name, impl->font_name) || font_size != impl->font_size) {
    impl->font_name = font_name;
    impl->font_size = font_size;
    return TRUE;
  }
  return FALSE;
}

static ret_t text_edit_update_caret_pos(text_edit_t* text_edit) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t k = 0;
  uint32_t y = 0;
  DECL_IMPL(text_edit);
  uint32_t line_index = 0;
  rows_t* rows = impl->rows;
  bool_t is_setting = FALSE;
  canvas_t* c = GET_CANVAS(text_edit);
  uint32_t font_size = impl->font_size;
  uint32_t line_height = impl->line_height;
  wstr_t* text = &(text_edit->widget->text);

  canvas_set_font(c, impl->font_name, font_size);

  for (i = 0; i < rows->size; i++) {
    row_info_t* row = rows->row + i;
    for (j = 0; j < row->line_num; j++, y += line_height, line_index++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      uint32_t line_offset_begin = line->offset;
      uint32_t line_offset_end = line->offset + line->length;
      if ((line_offset_begin <= impl->state.cursor && impl->state.cursor < line_offset_end) ||
          (j + 1 == row->line_num && impl->state.cursor == line_offset_end)) {
        uint32_t x = line->x;
        wchar_t last_char = 0;
        wchar_t* p = text->str + line_offset_begin;
        uint32_t offset = impl->state.cursor - line_offset_begin;
        for (k = 0; k < line->length; k++, p++) {
          if (offset == k) {
            break;
          }
          x += (canvas_measure_text(c, p, 1) + CHAR_SPACING);
          last_char = *p;
        }
        is_setting = TRUE;
        if (last_char == STB_TEXTEDIT_NEWLINE) {
          text_edit_set_caret_pos(impl, 0, y + line_height, c->font_size, line_index);
        } else {
          text_edit_set_caret_pos(impl, x, y, c->font_size, line_index);
        }
        break;
      }
    }
    if (is_setting) {
      break;
    }
  }
  text_edit_fix_oy(impl);
  text_edit_notify(text_edit);

  return RET_OK;
}

ret_t text_edit_click(text_edit_t* text_edit, xy_t x, xy_t y) {
  point_t point;
  DECL_IMPL(text_edit);
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, GET_CANVAS(text_edit));
  point = text_edit_normalize_point(text_edit, x, y);
  stb_textedit_click(text_edit, &(impl->state), point.x, point.y);

  if (impl->single_line || text_edit_is_need_layout(text_edit)) {
    text_edit_layout(text_edit);
  } else {
    text_edit_update_caret_pos(text_edit);
  }

  return RET_OK;
}

ret_t text_edit_drag(text_edit_t* text_edit, xy_t x, xy_t y) {
  point_t point;
  DECL_IMPL(text_edit);
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  widget_prepare_text_style(text_edit->widget, GET_CANVAS(text_edit));
  point = text_edit_normalize_point(text_edit, x, y);
  stb_textedit_drag(text_edit, &(impl->state), point.x, point.y);

  if (impl->single_line || text_edit_is_need_layout(text_edit)) {
    text_edit_layout(text_edit);
  } else {
    text_edit_update_caret_pos(text_edit);
  }

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
    } else if (key > 128 || !tk_isprint(key)) {
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
  bool_t move_caret_pos = FALSE;
  STB_TexteditState* state = NULL;
  text_layout_info_t* layout_info = NULL;
  return_value_if_fail(impl != NULL, RET_BAD_PARAMS);

  key = evt->key;
  state = &(impl->state);
  text = &(text_edit->widget->text);
  layout_info = &(impl->layout_info);

  switch (key) {
#ifdef WITH_SDL
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
      move_caret_pos = TRUE;
      key = STB_TEXTEDIT_K_LEFT;
      break;
    }
    case TK_KEY_RIGHT: {
      move_caret_pos = TRUE;
      key = STB_TEXTEDIT_K_RIGHT;
      break;
    }
    case TK_KEY_DOWN: {
      move_caret_pos = TRUE;
      key = STB_TEXTEDIT_K_DOWN;
      break;
    }
    case TK_KEY_UP: {
      move_caret_pos = TRUE;
      key = STB_TEXTEDIT_K_UP;
      break;
    }
    case TK_KEY_HOME: {
      move_caret_pos = TRUE;
      state->cursor = 0;
      key = STB_TEXTEDIT_K_LINESTART;
      break;
    }
    case TK_KEY_END: {
      move_caret_pos = TRUE;
      state->cursor = text->size;
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
      move_caret_pos = TRUE;
      if (impl->single_line) {
        key = STB_TEXTEDIT_K_LINEEND;
      } else {
        int32_t lines = layout_info->h / impl->line_height;
        int32_t next_lines = impl->caret_line_index + lines;
        int32_t next_y = next_lines * impl->line_height;
        if (layout_info->virtual_h > next_y) {
          while (lines-- > 0) {
            stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_DOWN);
          }
        } else {
          state->cursor = text->size;
          stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_LINEEND);
        }
        goto layout;

        return RET_OK;
      }
      break;
    }
    case TK_KEY_PAGEUP: {
      move_caret_pos = TRUE;
      if (impl->single_line) {
        key = STB_TEXTEDIT_K_LINESTART;
      } else {
        int32_t lines = layout_info->h / impl->line_height;
        int32_t next_lines = impl->caret_line_index - lines;

        if (next_lines > 0) {
          while (lines-- > 0) {
            stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_UP);
          }
        } else {
          state->cursor = 0;
          stb_textedit_key(text_edit, state, STB_TEXTEDIT_K_LINESTART);
        }
        goto layout;
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
    case TK_KEY_CAPSLOCK:
    case TK_KEY_COMMAND:
    case TK_KEY_MENU:
    case TK_KEY_WHEEL: {
      return RET_OK;
    }
    default: {
      if (key < 128 && tk_isprint(key)) {
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
layout:
  if (!impl->single_line && move_caret_pos && !text_edit_is_need_layout(text_edit)) {
    text_edit_update_caret_pos(text_edit);
  } else {
    text_edit_layout(text_edit);
  }

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

char* text_edit_get_selected_text(text_edit_t* text_edit) {
  uint32_t size = 0;
  text_edit_state_t state = {0};
  char* ret = NULL;
  return_value_if_fail(text_edit != NULL, NULL);

  text_edit_get_state(text_edit, &state);
  size = state.select_end - state.select_start;

  if (size > 0) {
    ret = TKMEM_ZALLOCN(char, size + 1);
    return_value_if_fail(ret != NULL, NULL);

    tk_utf8_from_utf16_ex(text_edit->widget->text.str + state.select_start, size, ret, size + 1);
  }

  return ret;
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

ret_t text_edit_set_overwrite(text_edit_t* text_edit, bool_t overwrite) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->overwrite = overwrite;
  text_edit_notify(text_edit);

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

ret_t text_edit_set_tips(text_edit_t* text_edit, const char* tips, bool_t mlines) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->is_mlines = mlines;
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
  state->max_chars = impl->max_chars;
  state->caret = impl->caret;
  state->preedit = impl->preedit;
  state->line_height = impl->line_height;

  state->cursor = impl->state.cursor;
  state->max_rows = impl->rows->capacity;
  state->last_row_number = impl->last_row_number;
  state->last_line_number = impl->last_line_number;

  state->select_start = tk_min(impl->state.select_start, impl->state.select_end);
  state->select_end = tk_max(impl->state.select_start, impl->state.select_end);

  state->mask = impl->mask;
  state->overwrite = impl->overwrite;
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

static ret_t text_edit_insert_wtext_with_len(text_edit_t* text_edit, uint32_t offset,
                                             const wchar_t* wtext, uint32_t len) {
  ret_t ret = RET_FAIL;
  uint32_t size = 0;
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && wtext != NULL,
                       RET_BAD_PARAMS);

  size = wcslen(wtext);
  len = tk_min(len, size);
  offset = tk_min(offset, text_edit->widget->text.size);

  ret = !!text_edit_insert(text_edit, offset, (wchar_t*)wtext, len) ? RET_OK : RET_SKIP;

  if (ret == RET_OK) {
    if (offset + len != text_edit_get_cursor(text_edit)) {
      text_edit_set_cursor(text_edit, offset + len);
    } else {
      text_edit_layout(text_edit);
    }
  }

  return ret;
}

static ret_t text_edit_insert_text_with_len(text_edit_t* text_edit, uint32_t offset,
                                            const char* text, uint32_t len) {
  ret_t ret = RET_FAIL;
  wstr_t s = {0};
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && text != NULL,
                       RET_BAD_PARAMS);

  wstr_set_utf8(&s, text);
  ret = text_edit_insert_wtext_with_len(text_edit, offset, s.str, len);
  wstr_reset(&s);

  return ret;
}

ret_t text_edit_insert_text(text_edit_t* text_edit, uint32_t offset, const char* text) {
  return text_edit_insert_text_with_len(text_edit, offset, text, tk_strlen(text));
}

static ret_t text_edit_overwrite_text_fix_select(text_edit_t* text_edit, uint32_t insert_offset,
                                                 uint32_t insert_len, uint32_t remove_offset,
                                                 uint32_t remove_len) {
  DECL_IMPL(text_edit);
  wstr_t* wstr = &text_edit->widget->text;
  uint32_t select_start = impl->state.select_start;
  uint32_t select_end = impl->state.select_end;

  if (impl->state.select_start > insert_offset) {
    select_start = tk_min(select_start + insert_len, wstr->size);
  }
  if (impl->state.select_end > insert_offset) {
    select_end = tk_min(select_end + insert_len, wstr->size);
  }

  if (tk_max(impl->state.select_start, impl->state.select_end) > remove_offset &&
      tk_min(impl->state.select_start, impl->state.select_end) < remove_len) {
    select_start = select_end = 0;
  } else {
    if (impl->state.select_start > remove_offset) {
      select_start -= remove_len;
    }
    if (impl->state.select_end > remove_offset) {
      select_end -= remove_len;
    }
  }
  impl->state.select_start = select_start;
  impl->state.select_end = select_end;

  return RET_OK;
}

static ret_t text_edit_overwrite_text_not_at_last_row(text_edit_t* text_edit, uint32_t* p_offset,
                                                      const char* text, uint32_t len) {
  DECL_IMPL(text_edit);
  wstr_t* wstr = &text_edit->widget->text;
  wstr_t s = {0};
  /* 获取最后一个换行符的偏移位置 */
  uint32_t last_row_line_break =
      text_edit_get_line_break_offset(text_edit, impl->rows->capacity - 1);
  uint32_t rm_len = 0;
  uint32_t rm_index = 0;

  if (last_row_line_break < wstr->size) {
    rm_index = tk_max(*p_offset, last_row_line_break);
    if (wstr->size > rm_index) {
      rm_len = wstr->size - rm_index;
      wstr_remove(wstr, rm_index, rm_len);
    }
  }

  wstr_set_utf8_with_len(&s, text, len);
  wstr_insert(wstr, *p_offset, s.str, s.size);
  text_edit_overwrite_text_fix_select(text_edit, *p_offset, s.size, rm_index, rm_len);
  wstr_reset(&s);

  if (*p_offset + len != text_edit_get_cursor(text_edit)) {
    text_edit_set_cursor(text_edit, *p_offset + len);
  } else {
    text_edit_layout(text_edit);
  }

  return RET_OK;
}

static ret_t text_edit_overwrite_text_at_last_row(text_edit_t* text_edit, uint32_t* p_offset,
                                                  const char* text, uint32_t len) {
  DECL_IMPL(text_edit);
  wstr_t* wstr = &text_edit->widget->text;
  uint32_t line_break_num = impl->rows->capacity > impl->last_row_number
                                ? impl->rows->capacity - impl->last_row_number
                                : 0;
  wstr_t s = {0};
  uint32_t rm_len = 0;

  if (line_break_num <= 1) {
    rm_len = impl->rows->row[0].length;
    wstr_remove(wstr, 0, rm_len);
    *p_offset = tk_min(*p_offset > rm_len ? *p_offset - rm_len : 0, wstr->size);

    if (wstr->size > *p_offset) {
      wstr_remove(wstr, *p_offset, wstr->size - *p_offset);
      text_edit_overwrite_text_fix_select(text_edit, 0, 0, *p_offset, wstr->size - *p_offset);
    }
  }

  wstr_set_utf8_with_len(&s, text, len);
  wstr_insert(wstr, *p_offset, s.str, s.size);
  text_edit_overwrite_text_fix_select(text_edit, *p_offset, s.size, 0, rm_len);
  wstr_reset(&s);

  if (*p_offset + len != text_edit_get_cursor(text_edit)) {
    text_edit_set_cursor(text_edit, *p_offset + len);
  } else {
    text_edit_layout(text_edit);
  }

  return RET_OK;
}

ret_t text_edit_overwrite_text(text_edit_t* text_edit, uint32_t* p_offset, const char* text,
                               uint32_t len) {
  DECL_IMPL(text_edit);
  wstr_t* wstr = NULL;
  uint32_t text_size = 0;
  ret_t ret = RET_FAIL;
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && text != NULL,
                       RET_BAD_PARAMS);

  wstr = &text_edit->widget->text;
  text_size = tk_strlen(text);
  *p_offset = tk_min(wstr->size, *p_offset);
  len = tk_min(len, text_size);

  /* 插入位置 不在最后一行 */
  if (*p_offset < wstr->size - impl->rows->row[impl->rows->capacity - 1].length) {
    ret = text_edit_overwrite_text_not_at_last_row(text_edit, p_offset, text, len);
  } else {
    ret = text_edit_overwrite_text_at_last_row(text_edit, p_offset, text, len);
  }
  *p_offset += len;

  return ret;
}

ret_t text_edit_set_lock_scrollbar_value(text_edit_t* text_edit, bool_t lock) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->lock_scrollbar_value = lock;

  return RET_OK;
}

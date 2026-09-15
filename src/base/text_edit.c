/**
 * File:   text_edit.c
 * Author: AWTK Develop Team
 * Brief:  text_edit
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#define STB_TEXTEDIT_NEWLINER (wchar_t)('\r')
#define STB_TEXTEDIT_STRING text_edit_t

#if !defined(WITH_SDL)
#define STB_TEXTEDIT_UNDOSTATECOUNT 10
#define STB_TEXTEDIT_UNDOCHARCOUNT 32
#endif /*WITH_SDL*/
#define STB_TEXTEDIT_GETWIDTH_NEWLINE 0xffff

#include "stb/stb_textedit.h"

#ifndef BRIEFLY_SHOW_CHAR_TIMER_DURATION
#define BRIEFLY_SHOW_CHAR_TIMER_DURATION 800
#endif /*BRIEFLY_SHOW_CHARS_TIMER_DURATION*/

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
  int32_t glyph_count;
  int32_t* glyph_arr;
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
  glyphs_t* glyphs;
  glyphs_t* mask_glyphs;
  uint32_t max_chars;
  point_t caret;
  bool_t wrap_word;
  bool_t single_line;
  bool_t caret_visible;
  uint32_t line_height;
  uint32_t last_line_number;
  uint32_t last_row_number;
  uint32_t caret_row_index;
  uint32_t caret_line_index;
  text_layout_info_t layout_info;

  bool_t preedit;
  uint32_t preedit_chars_nr;

  bool_t briefly_show_char;
  uint32_t briefly_show_char_done_timer_id;

  bool_t lock_scrollbar_value;

  /*for single line edit*/
  wchar_t mask_char;
  bool_t mask;

  wstr_t tips;
  bool_t tips_is_mlines;
  bool_t is_first_time_layout;
  int32_t font_size;
  const char* font_name;
  void* on_state_changed_ctx;
  text_edit_on_state_changed_t on_state_changed;
  void* on_text_will_delete_ctx;
  text_edit_on_text_will_delete_t on_text_will_delete;
  void* on_char_will_input_ctx;
  text_edit_on_char_will_input_t on_char_will_input;
} text_edit_impl_t;

#define DECL_IMPL(te) text_edit_impl_t* impl = (text_edit_impl_t*)(te)

static ret_t text_edit_notify(text_edit_t* text_edit);
static bool_t text_edit_is_need_layout(text_edit_t* text_edit);
static ret_t text_edit_update_caret_pos(text_edit_t* text_edit);
static int32_t text_edit_calc_x_on_canvas(text_edit_t* text_edit, line_info_t* iter,
                                          glyphs_t* glyphs, glyphs_t* mask_glyphs, canvas_t* c);
static ret_t text_edit_select_word_impl(text_edit_t* text_edit, uint32_t cursor, int32_t* start,
                                        int32_t* end);
static wh_t text_edit_measure_char(STB_TEXTEDIT_STRING* str, canvas_t* c,
                                   STB_TEXTEDIT_CHARTYPE* iter, STB_TEXTEDIT_CHARTYPE* chr);

ret_t text_edit_adjust_ime_rect(rect_t* r, float_t device_pixel_ratio) {
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);

#ifdef MACOS
  /* SDL/Cocoa IME 使用窗口点坐标，与 widget_to_screen 一致 */
  (void)device_pixel_ratio;
#else
  r->x = (xy_t)(r->x * device_pixel_ratio);
  r->y = (xy_t)(r->y * device_pixel_ratio);
  r->w = (wh_t)(r->w * device_pixel_ratio);
  r->h = (wh_t)(r->h * device_pixel_ratio);
#endif /*MACOS*/

  return RET_OK;
}

#ifdef WITH_SDL
#include "platforms/pc/sdl_api.h"
#include "base/native_window.h"
#include "base/window_manager.h"

static ret_t text_edit_update_input_rect(text_edit_t* text_edit) {
  point_t p = {0, 0};
  DECL_IMPL(text_edit);
  SDL_Rect r = {0, 0, 0, 0};
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL, RET_BAD_PARAMS);
  widget_t* widget = text_edit->widget;

  if (system_info()->app_type != APP_DESKTOP) {
    return RET_OK;
  }
  if (!widget->focused) {
    return RET_OK;
  }

  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t x = layout_info->margin_l + impl->caret.x - layout_info->ox;
  uint32_t y = layout_info->margin_t + impl->caret.y - layout_info->oy;

  widget_to_screen(widget, &p);
  p.x = p.x + x;
  p.y = p.y + y;

  {
    rect_t ir = rect_init(p.x, p.y, text_edit->widget->w, impl->font_size);
    text_edit_adjust_ime_rect(&ir, system_info()->device_pixel_ratio);
    r.x = ir.x;
    r.y = ir.y;
    r.w = ir.w;
    r.h = ir.h;
  }

#ifdef AWTK_SDL3
  {
    native_window_t* nw =
        (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
    SDL_Window* win = nw != NULL ? (SDL_Window*)nw->handle : NULL;
    if (win != NULL) {
      SDL_SetTextInputArea(win, &r, 0);
    }
  }
#else
  SDL_SetTextInputRect(&r);
#endif

  return RET_OK;
}
#else
static ret_t text_edit_update_input_rect(text_edit_t* text_edit) {
  return RET_OK;
}
#endif /*WITH_SDL*/

static ret_t line_info_destroy(void* data) {
  line_info_t* line_info = (line_info_t*)data;
  if (line_info->glyph_arr != NULL) {
    TKMEM_FREE(line_info->glyph_arr);
  }
  TKMEM_FREE(data);

  return RET_OK;
}

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
    TEXT_EDIT_GET_STYLE_MARGIN(style, out_value, type);                         \
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

static glyphs_t* text_edit_create_glyphs(text_edit_t* text_edit, const wchar_t* str,
                                         uint32_t size) {
  font_t* font = NULL;
  glyphs_t* glyphs = NULL;
  font_raster_params_t params;
  DECL_IMPL(text_edit);
  widget_t* widget = text_edit->widget;
  style_t* style = widget->astyle;
  font_manager_t* fm = widget_get_font_manager(widget);
  bool_t shaping = widget_get_shaping(widget);
  const char* bidi_type = widget_get_bidi(widget);
  const char* font_name = system_info_fix_font_name(style_get_str(style, STYLE_ID_FONT_NAME, NULL));
  uint32_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  return_value_if_fail(text_edit != NULL && fm != NULL, NULL);

  if (str != NULL && size > 0) {
    font = font_manager_get_font(fm, font_name, font_size);
    if (font != NULL) {
      font_get_raster_params(font, &params);
      params.shaping = shaping;
      params.bidi_type = bidi_type_from_name(bidi_type);
      font_set_raster_params(font, &params);

      glyphs = font_create_glyphs(font, str, size, font_size, NULL);
    }
  }

  return glyphs;
}

static rows_t* rows_create(uint32_t capacity) {
  uint32_t msize = sizeof(rows_t) + capacity * sizeof(row_info_t);
  rows_t* rows = (rows_t*)TKMEM_ALLOC(msize);
  uint32_t i = 0;
  return_value_if_fail(rows != NULL, NULL);

  memset(rows, 0x00, msize);
  for (i = 0; i < capacity; i++) {
    darray_init(&rows->row[i].info, 4, line_info_destroy, NULL);
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
                                     uint32_t font_size, uint32_t line_number, uint32_t row_num) {
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
  impl->caret_row_index = row_num;

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

static inline bool_t text_edit_is_preedit_char(text_edit_t* text_edit, uint32_t index) {
  DECL_IMPL(text_edit);
  return impl->preedit && index < impl->state.cursor &&
         index >= (impl->state.cursor - impl->preedit_chars_nr);
}

static inline bool_t text_edit_is_briefly_show_char(text_edit_t* text_edit, uint32_t index) {
  DECL_IMPL(text_edit);
  return impl->briefly_show_char && index == (impl->state.cursor - 1);
}

static uint32_t text_edit_measure_text_on_canvas(text_edit_t* text_edit, glyphs_t* glyphs,
                                                 glyphs_t* mask_glyphs, uint32_t start,
                                                 uint32_t size, canvas_t* c) {
  uint32_t i = 0;
  uint32_t w = 0;
  DECL_IMPL(text_edit);
  uint32_t mask_w = mask_glyphs != NULL ? glyphs_measure(mask_glyphs, 0, 1) : 0;
  for (i = 0; i < size; i++) {
    bool_t preedit = text_edit_is_preedit_char(text_edit, i);
    bool_t briefly_show = text_edit_is_briefly_show_char(text_edit, i);
    int32_t chr_w = (mask_w > 0 && (!preedit && !briefly_show) && impl->mask)
                        ? mask_w
                        : glyphs_measure(glyphs, i + start, 1);
    if (chr_w > 0) {
      w += chr_w + CHAR_SPACING;
    }
  }

  return w;
}

static void text_edit_adjust_hscroll(text_layout_info_t* layout_info, uint32_t text_w,
                                     uint32_t caret_x, uint32_t font_size) {
  if (layout_info->ox + font_size > caret_x) {
    if (layout_info->ox < font_size) {
      layout_info->ox = 0;
    } else {
      layout_info->ox -= font_size;
    }
  } else if (caret_x + font_size > layout_info->ox + layout_info->w) {
    // 超出可视区域右边缘的文本宽度
    int32_t text_outside_right_w = text_w - layout_info->ox - layout_info->w;
    text_outside_right_w = tk_max(text_outside_right_w, 0);
    layout_info->ox += tk_min(text_outside_right_w, font_size);
  }

  {
    // 文本结束位置到可视区域右边缘的空白宽度
    int32_t blank_inside_right_w = layout_info->w - (text_w - layout_info->ox);
    if (blank_inside_right_w > 0) {
      layout_info->ox -= blank_inside_right_w;
      layout_info->ox = tk_max(0, layout_info->ox);
    }
  }
}

// 填充 glyph_arr 数组中的数据
static void text_edit_fill_glyph_arr(line_info_t* line, glyphs_t* glyphs, uint32_t str_start,
                                     uint32_t str_len) {
  int32_t cap = 0;
  line->glyph_arr = glyphs_get_glyph_indexs_from_str_indexs(
      glyphs, str_start, str_len, line->glyph_arr, line->glyph_count, &cap);
  line->glyph_count = cap;
}

// 记录当前行数据
static void text_edit_finish_line(row_info_t* row, glyphs_t* glyphs, uint32_t line_start,
                                  uint32_t line_end, uint32_t x, bool_t add_num_line) {
  line_info_t* line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
  line->text_w = x;
  line->offset = line_start;
  line->length = line_end - line_start;
  text_edit_fill_glyph_arr(line, glyphs, line_start, line->length);
  if (add_num_line) {
    row->line_num++;
  }
  if (row->info.size < row->line_num) {
    darray_push(&row->info, TKMEM_ZALLOC(line_info_t));
  }
}

static row_info_t* text_edit_single_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                     uint32_t line_index, uint32_t offset,
                                                     glyphs_t* glyphs, glyphs_t* mask_glyphs) {
  int32_t i = 0;
  uint32_t y = 0;
  uint32_t index = 0;
  uint32_t caret_x = 0;
  DECL_IMPL(text_edit);
  const glyph_t* g = NULL;
  uint32_t caret_text_w = 0;
  canvas_t* c = GET_CANVAS(text_edit);
  wstr_t* text = &(text_edit->widget->text);
  STB_TexteditState* state = &(impl->state);
  row_info_t* row = impl->rows->row + row_num;
  text_layout_info_t* layout_info = &(impl->layout_info);
  line_info_t* line = (line_info_t*)darray_head(&row->info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);
  int32_t cursor_index = glyphs_get_glyph_index_from_str_index(glyphs, state->cursor);
  uint32_t text_w = text_edit_measure_text_on_canvas(text_edit, glyphs, mask_glyphs, 0,
                                                     glyphs_get_length(glyphs), c);

  if (cursor_index < 0) {
    // 小于0 说明光标在末尾，查找最后一个字符的字模所在处
    index = glyphs_get_glyph_index_from_str_index(glyphs, state->cursor - 1);
    g = glyphs_get(glyphs, index);
    caret_text_w = text_edit_measure_text_on_canvas(text_edit, glyphs, mask_glyphs, index,
                                                    glyphs_get_length(glyphs) - index, c);
    if (g != NULL && g->bidi_type == FONT_BIDI_TYPE_RTL) {
      caret_text_w = text_w - caret_text_w;
    } else {
      caret_text_w = text_w;
    }
    assert(state->cursor >= text->size);
  } else {
    index = cursor_index;
    caret_text_w =
        text_edit_measure_text_on_canvas(text_edit, glyphs, mask_glyphs, 0, cursor_index, c);
    g = glyphs_get(glyphs, index);
    if (g != NULL && g->bidi_type == FONT_BIDI_TYPE_RTL) {
      caret_text_w += glyphs_measure(glyphs, index, g->glyph_count);
    }
  }

  assert(offset == 0 && row_num == 0);

  memset(row, 0x00, sizeof(row_info_t) - sizeof(darray_t));
  line->offset = 0;
  line->text_w = text_w;
  line->length = glyphs_get_str_length(glyphs);
  text_edit_fill_glyph_arr(line, glyphs, 0, line->length);
  line->x = text_edit_calc_x_on_canvas(text_edit, line, glyphs, mask_glyphs, c);
  row->line_num = 1;
  row->length = line->length;
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

  if (impl->state.select_start == impl->state.select_end) {
    text_edit_adjust_hscroll(layout_info, text_w, caret_x, c->font_size);
  }

  y += (layout_info->h - c->font_size) / 2;
  text_edit_set_caret_pos(impl, caret_x, y, c->font_size, line_index, row_num);

  return row;
}

static row_info_t* text_edit_multi_line_layout_line(text_edit_t* text_edit, uint32_t row_num,
                                                    uint32_t line_index, uint32_t offset,
                                                    glyphs_t* glyphs) {
  uint32_t i = 0;
  uint32_t x = 0;
  uint32_t gc = 0;
  uint32_t char_w = 0;
  uint32_t real_index = 0;
  uint32_t str_len = 0;
  uint32_t line_start = offset;
  uint32_t line_end = 0;
  uint32_t last_breakable_i = 0;
  uint32_t last_breakable_x = 0;
  const glyph_t* g = NULL;
  const glyph_t* last_g = NULL;
  DECL_IMPL(text_edit);
  row_info_t* row = impl->rows->row + row_num;
  uint32_t line_height = impl->line_height;
  uint32_t y = line_index * line_height;
  uint32_t offset0 = offset;
  line_info_t* last_line = NULL;
  text_layout_info_t* layout_info = &(impl->layout_info);

  str_len = glyphs_get_str_length(glyphs);

  memset(row, 0x00, sizeof(row_info_t) - sizeof(darray_t));
  row->line_num = 1;

  for (i = offset; i < str_len;) {
    real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
    g = glyphs_get(glyphs, real_index);
    if (g == NULL) break;
    gc = g->glyph_count;

    // 换行符强制换行
    if (g->chr == STB_TEXTEDIT_NEWLINER || g->chr == STB_TEXTEDIT_NEWLINE) {
      i++;
      if (g->chr == STB_TEXTEDIT_NEWLINER && i < str_len) {
        real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
        g = glyphs_get(glyphs, real_index);
        if (g != NULL && g->chr == STB_TEXTEDIT_NEWLINE) {
          i++;
        }
      }
      line_end = i;
      break;
    } else {
      char_w = (uint32_t)glyphs_measure(glyphs, real_index, gc) + CHAR_SPACING;
      if (impl->wrap_word && (x + char_w) > layout_info->w) {
        // 换行，有 last_break 数据就使用，没有以当前位置换行
        if (last_breakable_x > 0) {
          x = last_breakable_x;
          line_end = last_breakable_i;
        } else if (line_start == i) {
          x = char_w;
          line_end = i + g->str_count;
        } else {
          line_end = i;
        }
      } else {
        // 记录 line_break 信息
        if (impl->wrap_word && i > 0) {
          real_index = glyphs_get_glyph_index_from_str_index(glyphs, i - 1);
          last_g = glyphs_get(glyphs, real_index);
          if (last_g != NULL && line_break_check(last_g->chr, g->chr) == LINE_BREAK_ALLOW) {
            last_breakable_i = i;
            last_breakable_x = x;
          }
        }
        x += char_w;
        i += g->str_count;
        continue;
      }
    }

    text_edit_finish_line(row, glyphs, line_start, line_end, x, TRUE);
    x = 0;
    y += line_height;
    line_index++;
    line_start = line_end;
    last_breakable_i = 0;
    last_breakable_x = 0;
    i = line_start;
  }

  if (g->chr == STB_TEXTEDIT_NEWLINE || g->chr == STB_TEXTEDIT_NEWLINER) {
    impl->last_row_number = row_num + 1;
    impl->last_line_number = line_index + 1;
  } else {
    impl->last_row_number = row_num;
    impl->last_line_number = line_index;
  }

  while (row->info.size > row->line_num) {
    row->info.destroy(darray_pop(&row->info));
  }

  if (i > line_start || line_start == 0) {
    text_edit_finish_line(row, glyphs, line_start, i, x, FALSE);
  } else {
    // 去除末尾空行多余的 line
    row->line_num--;
    while (row->info.size > row->line_num) {
      row->info.destroy(darray_pop(&row->info));
    }
  }

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
                                         uint32_t line_index, uint32_t offset, glyphs_t* glyphs,
                                         glyphs_t* mask_glyphs) {
  DECL_IMPL(text_edit);
  if (impl->single_line) {
    return text_edit_single_line_layout_line(text_edit, row_num, line_index, offset, glyphs,
                                             mask_glyphs);
  } else {
    return text_edit_multi_line_layout_line(text_edit, row_num, line_index, offset, glyphs);
  }
}

/* 用于layout指定位置的text文本 */
static ret_t text_edit_layout_fragment(text_edit_t* text_edit, uint32_t start, uint32_t end,
                                       row_info_t* row_tmp, uint32_t* line_index, uint32_t* row_num,
                                       uint32_t row_start, uint32_t row_end,
                                       uint32_t* glyph_count_diff) {
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t x = 0;
  uint32_t gc = 0;
  uint32_t char_w = 0;
  uint32_t real_index = 0;
  uint32_t str_len = 0;
  uint32_t line_start = start;
  uint32_t line_end = 0;
  uint32_t last_breakable_i = 0;
  uint32_t last_breakable_x = 0;
  uint32_t glyph_count = 0;
  uint32_t old_glyph_count = 0;
  const glyph_t* g = NULL;
  const glyph_t* last_g = NULL;
  DECL_IMPL(text_edit);
  text_layout_info_t* layout_info = &(impl->layout_info);
  row_info_t* row = row_tmp + *row_num;
  glyphs_t* glyphs = impl->glyphs;

  // 记录待替换的行原先有多少字模
  for (i = row_start; i < row_end; i++) {
    row = impl->rows->row + i;
    for (k = 0; k < row->line_num; k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, k);
      old_glyph_count += line->glyph_count;
    }
  }

  if (glyphs == NULL) {
    *glyph_count_diff = old_glyph_count;
    return RET_OK;
  }
  row = row_tmp + *row_num;
  row->line_num = 1;
  str_len = glyphs_get_str_length(glyphs);

  for (i = start; i < end;) {
    real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
    g = glyphs_get(glyphs, real_index);
    if (g == NULL) break;
    gc = g->glyph_count;
    glyph_count += gc;

    /* 换行符强制换行 */
    if (g->chr == STB_TEXTEDIT_NEWLINER || g->chr == STB_TEXTEDIT_NEWLINE) {
      i++;
      if (g->chr == STB_TEXTEDIT_NEWLINER && i < end) {
        real_index = glyphs_get_glyph_index_from_str_index(glyphs, i);
        g = glyphs_get(glyphs, real_index);
        if (g != NULL && g->chr == STB_TEXTEDIT_NEWLINE) {
          i++;
        }
      }
      line_end = i;
      text_edit_finish_line(row, glyphs, line_start, line_end, x, FALSE);
      (*row_num)++;
      (*line_index)++;
      while (row->info.size > row->line_num) {
        row->info.destroy(darray_pop(&row->info));
      }
      // 后续还有内容的话就将下一行初始化
      if (i < end) {
        row = row_tmp + *row_num;
        row->line_num = 1;
      }
      x = 0;
      line_start = i;
      last_breakable_i = 0;
      last_breakable_x = 0;
      continue;
    }

    // 超过行长，换行并记录当前行信息
    char_w = (uint32_t)glyphs_measure(glyphs, real_index, gc) + CHAR_SPACING;
    if (impl->wrap_word && (x + char_w) > layout_info->w) {
      if (last_breakable_x > 0) {
        x = last_breakable_x;
        line_end = last_breakable_i;
      } else if (line_start == i) {
        x = char_w;
        line_end = i + g->str_count;
      } else {
        line_end = i;
      }
      text_edit_finish_line(row, glyphs, line_start, line_end, x, TRUE);
      (*line_index)++;
      while (row->info.size > row->line_num) {
        row->info.destroy(darray_pop(&row->info));
      }
      x = 0;
      line_start = line_end;
      last_breakable_i = 0;
      last_breakable_x = 0;
      i = line_start;
      continue;
    }

    /* 记录可换行位置 */
    if (impl->wrap_word && i > start) {
      real_index = glyphs_get_glyph_index_from_str_index(glyphs, i - 1);
      last_g = glyphs_get(glyphs, real_index);
      if (last_g != NULL && line_break_check(last_g->chr, g->chr) == LINE_BREAK_ALLOW) {
        last_breakable_i = i;
        last_breakable_x = x;
      }
    }
    x += char_w;
    i += g->str_count;
  }

  /* 收尾最后一行 */
  if (i > line_start) {
    text_edit_finish_line(row, glyphs, line_start, i, x, FALSE);
    (*row_num)++;
    (*line_index)++;
  }
  // 计算替换前后的字模差
  if (old_glyph_count > glyph_count) {
    *glyph_count_diff = old_glyph_count - glyph_count;
  } else {
    *glyph_count_diff = glyph_count - old_glyph_count;
  }

  return RET_OK;
}

/* 用于前移或者后移某部分行数据 */
static ret_t text_edit_row_transfer(text_edit_t* text_edit, uint32_t start, uint32_t interval,
                                    bool_t forward, uint32_t change_num, bool_t overwrite,
                                    uint32_t glyph_count_diff) {
  uint32_t i, j, k;
  DECL_IMPL(text_edit);
  row_info_t* row;
  row_info_t* row_temp;

  if (forward) {
    // 清理前面的行给后面前移使用
    for (i = start; i < interval + start; i++) {
      row = impl->rows->row + i;
      darray_deinit(&row->info);
    }
    // 前移 interval 行
    for (i = start; i < impl->rows->size - interval; i++) {
      row = impl->rows->row + i;
      row_temp = impl->rows->row + interval + i;
      for (j = 0; j < row_temp->line_num; j++) {
        line_info_t* line = (line_info_t*)darray_get(&row_temp->info, j);
        line->offset = line->offset - change_num;
        for (k = 0; k < line->glyph_count; k++) {
          line->glyph_arr[k] = line->glyph_arr[k] - glyph_count_diff;
        }
      }
      row->length = row_temp->length;
      row->line_num = row_temp->line_num;
      row->info = row_temp->info;
    }
    // 清理末尾的行
    for (; i < impl->rows->size; i++) {
      row = impl->rows->row + i;
      row->length = 0;
      row->line_num = 1;
      darray_init(&row->info, 4, line_info_destroy, NULL);
      darray_push(&row->info, TKMEM_ZALLOC(line_info_t));
    }
  } else {
    // 超出的部分不进行处理，没有超出的清理内容
    for (i = impl->rows->size + interval; i > impl->rows->size; i--) {
      if (i > impl->rows->capacity || (overwrite && i <= impl->rows->size)) {
        continue;
      }
      row = impl->rows->row + i - 1;
      darray_deinit(&row->info);
    }
    // 后移 interval 行
    for (i = impl->rows->size; i > start + 1; i--) {
      row = impl->rows->row + i - 1;
      if (i + interval > impl->rows->capacity) {
        darray_deinit(&row->info);
        continue;
      }
      row_temp = impl->rows->row + i + interval - 1;
      for (j = 0; j < row->line_num; j++) {
        line_info_t* line = (line_info_t*)darray_get(&row->info, j);
        line->offset = line->offset + change_num;
        for (k = 0; k < line->glyph_count; k++) {
          line->glyph_arr[k] = line->glyph_arr[k] + glyph_count_diff;
        }
      }
      row_temp->length = row->length;
      row_temp->line_num = row->line_num;
      row_temp->info = row->info;
    }
    // 初始化中间空出来的部分
    for (; i < start + interval + 1; i++) {
      row = impl->rows->row + i;
      row->length = 0;
      row->line_num = 1;
      darray_init(&row->info, 4, line_info_destroy, NULL);
      darray_push(&row->info, TKMEM_ZALLOC(line_info_t));
    }
  }
  return RET_OK;
}

ret_t text_edit_multi_line_insert_text_layout(text_edit_t* text_edit, uint32_t offset,
                                              uint32_t insert_length, const wchar_t* wtext,
                                              bool_t overwrite, uint32_t rm_num) {
  uint32_t i, j, k;
  uint32_t row_num = 0;
  uint32_t row_num_tmp = 0;
  uint32_t rm_row_num = 0;
  uint32_t line_index = 0;
  uint32_t line_index_tmp = 0;
  uint32_t glyph_count_diff = 0;
  wstr_t s = {0};
  wchar_t last_char = 0;
  DECL_IMPL(text_edit);
  row_info_t* row = NULL;
  canvas_t* c = GET_CANVAS(text_edit);
  wstr_t* text = &(text_edit->widget->text);
  uint32_t line_height = impl->line_height;
  uint32_t offset0 = offset;
  uint32_t rm_line_offset = 0;
  uint32_t insert_line_offset = 0;
  line_info_t* last_line = NULL;
  text_layout_info_t* layout_info = &(impl->layout_info);
  uint32_t insert_row_num = 0;
  uint32_t layout_row_num = 0;
  row_info_t* row_tmp = NULL;
  widget_prepare_text_style(text_edit->widget, GET_CANVAS(text_edit));
  if (impl->glyphs != NULL) {
    glyphs_destroy(impl->glyphs);
  }
  impl->glyphs = text_edit_create_glyphs(text_edit, text->str, text->size);

  if (insert_length == 0) {
    return RET_SKIP;
  }

  wstr_init(&s, 0);
  wstr_set_with_len(&s, wtext, insert_length);
  for (i = 0; i < insert_length; i++) {
    wchar_t* p = s.str + i;
    break_type_t line_break = line_break_check(*p, p[1]);
    if (line_break == LINE_BREAK_MUST) {
      insert_row_num++;
    }
  }
  layout_row_num = insert_row_num + 1;

  // 文本为空或者插入行大于容量，需要全部重新layout，直接使用text_edit_layout
  if (impl->rows->size == 0 || impl->rows->capacity <= layout_row_num) {
    text_edit_layout(text_edit);
    wstr_reset(&s);
    return RET_OK;
  }

  /* overwrite模式的处理 */
  if (overwrite && rm_num > 0) {
    /* 插入的字符串同时也是被移除的字符串的处理 */
    if (offset < rm_num) {
      uint32_t end = 0;
      row_tmp = TKMEM_ZALLOCN(row_info_t, layout_row_num);
      // 初始化 row_tmp
      for (i = 0; i < layout_row_num; i++) {
        row_tmp[i].line_num = 1;
        darray_init(&row_tmp[i].info, 4, line_info_destroy, NULL);
        darray_push(&row_tmp[i].info, TKMEM_ZALLOC(line_info_t));
      }
      // 计算被移除后最顶上的行还有多少字符
      for (i = 0; i < impl->rows->size; i++) {
        row_num_tmp++;
        row = impl->rows->row + i;
        last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
        line_index_tmp += row->line_num;
        if (offset0 < last_line->offset + last_line->length) {
          if (last_line->offset + last_line->length + insert_length >= rm_num) {
            end = last_line->offset + last_line->length + insert_length - rm_num;
            break;
          }
        }
      }
      // 对最前方剩余的字符进行 layout，layout 后根据 remove 前后的行数进行前移或后移调整
      text_edit_layout_fragment(text_edit, 0, end, row_tmp, &line_index, &row_num, 0, row_num_tmp,
                                &glyph_count_diff);
      if (row_num > row_num_tmp) {
        text_edit_row_transfer(text_edit, row_num_tmp, row_num - row_num_tmp, FALSE, 0, 0,
                               glyph_count_diff);
      } else {
        text_edit_row_transfer(text_edit, row_num, row_num_tmp - row_num, TRUE, 0, 0,
                               glyph_count_diff);
      }
      // 将 layout 后的行存储到空位中，并将后续行的数据进行完善
      for (i = 0; i < row_num; i++) {
        row = impl->rows->row + i;
        row->length = row_tmp[i].length;
        row->line_num = row_tmp[i].line_num;
        darray_deinit(&row->info);
        row->info = row_tmp[i].info;
      }
      impl->rows->size = impl->rows->size + row_num - row_num_tmp;
      impl->last_row_number = impl->rows->size;
      impl->last_line_number = impl->last_line_number + line_index - line_index_tmp;
      for (i = row_num; i < impl->rows->size; i++) {
        row = impl->rows->row + i;
        for (j = 0; j < row->line_num; j++) {
          line_info_t* line = (line_info_t*)darray_get(&row->info, j);
          line->offset = line->offset + insert_length - rm_num;
        }
      }
      if (row_num < layout_row_num) {
        for (i = row_num; i < layout_row_num; i++) {
          darray_deinit(&row_tmp[i].info);
        }
      }
      TKMEM_FREE(row_tmp);
      wstr_reset(&s);
      return RET_OK;
    }
    // 计算被移除的字符一共占用多少行
    for (i = 0; i < impl->rows->size; i++) {
      row = impl->rows->row + i;
      last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
      line_index_tmp += row->line_num;
      rm_line_offset = last_line->offset + last_line->length;
      row_num++;
      if (rm_num <= last_line->offset + last_line->length) {
        break;
      }
    }
    // 对移除字符后的首行进行 layout，然后用差值进行前移
    text_edit_layout_fragment(text_edit, rm_num, rm_line_offset, impl->rows->row, &line_index,
                              &row_num_tmp, 0, row_num, &glyph_count_diff);

    if (row_num_tmp == 0) {
      i = 0;
    } else {
      i = 1;
    }

    rm_row_num = row_num - row_num_tmp;
    text_edit_row_transfer(text_edit, i, rm_row_num, TRUE, rm_num, TRUE, glyph_count_diff);

    impl->rows->size = impl->rows->size - rm_row_num;
    impl->last_row_number = impl->rows->size;
    impl->last_line_number = impl->last_line_number + line_index - line_index_tmp;
    offset0 = offset0 - rm_num;
  }

  row_num = 0;
  row_num_tmp = 0;
  line_index = 0;
  line_index_tmp = 0;
  /* 查找插入的行位置 */
  for (i = 0; i < impl->rows->size; i++) {
    row = impl->rows->row + i;
    last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
    insert_line_offset = last_line->offset + last_line->length + insert_length;
    if (offset0 < last_line->offset + last_line->length) {
      break;
    }
    row_num++;
  }
  /* 计算特殊情况下实际插入的行数和需要layout的行数 */
  last_char = *(text->str + text->size - insert_length - 1);
  line_index_tmp = row->line_num;
  if (offset0 == text->size - insert_length) {
    // layout_row_num默认多计算一行，如果插入在末尾且末尾不为换行符时需减去
    if (!(last_char == STB_TEXTEDIT_NEWLINE || last_char == STB_TEXTEDIT_NEWLINER)) {
      row_num--;
      if (insert_row_num > 0 && (*(s.str + insert_length - 1) == STB_TEXTEDIT_NEWLINE ||
                                 *(s.str + insert_length - 1) == STB_TEXTEDIT_NEWLINER)) {
        insert_row_num--;
        layout_row_num--;
      }
    } else {
      // 插入字符没有换行符，记为一行
      if (insert_row_num == 0) {
        insert_row_num = 1;
        layout_row_num = 1;
      } else if (!(*(s.str + insert_length - 1) == STB_TEXTEDIT_NEWLINE ||
                   *(s.str + insert_length - 1) == STB_TEXTEDIT_NEWLINER)) {
        insert_row_num++;
      }
      layout_row_num = insert_row_num;
      line_index_tmp = 0;
    }
  }

  row_tmp = TKMEM_ZALLOCN(row_info_t, layout_row_num);
  // 计算偏移位置，初始化临时 row
  if (row_num > 0) {
    row = impl->rows->row + row_num - 1;
    last_line = (line_info_t*)darray_get(&row->info, row->line_num - 1);
    offset0 = last_line->offset + last_line->length;
  } else {
    offset0 = 0;
  }
  offset = offset0;

  for (i = 0; i < layout_row_num; i++) {
    row_tmp[i].line_num = 1;
    darray_init(&row_tmp[i].info, 4, line_info_destroy, NULL);
    darray_push(&row_tmp[i].info, TKMEM_ZALLOC(line_info_t));
  }
  row = impl->rows->row + row_num;
  // layout插入行，并根据layout的结果后移指定行数
  text_edit_layout_fragment(text_edit, offset0, insert_line_offset, row_tmp, &line_index,
                            &row_num_tmp, row_num, row_num + 1, &glyph_count_diff);
  text_edit_row_transfer(text_edit, row_num, insert_row_num, FALSE, insert_length, overwrite,
                         glyph_count_diff);
  // 将layout好的行塞入指定位置
  for (i = 0; i < layout_row_num; i++) {
    if (i + row_num > impl->rows->capacity) {
      darray_deinit(&row_tmp[i].info);
      continue;
    }
    row = impl->rows->row + i + row_num;
    row->length = row_tmp[i].length;
    row->line_num = row_tmp[i].line_num;
    darray_deinit(&row->info);
    row->info = row_tmp[i].info;
  }

  impl->rows->size = tk_min(impl->rows->size + insert_row_num, impl->rows->capacity);
  impl->last_row_number = impl->rows->size;
  impl->last_line_number = impl->last_line_number + line_index - line_index_tmp;
  layout_info->virtual_h = tk_max(impl->last_line_number * line_height, layout_info->widget_h);

  TKMEM_FREE(row_tmp);
  wstr_reset(&s);

  return RET_OK;
}

static ret_t text_edit_layout_impl(text_edit_t* text_edit) {
  uint32_t i = 0;
  uint32_t offset = 0;
  DECL_IMPL(text_edit);
  row_info_t* iter = NULL;
  glyphs_t* glyphs = NULL;
  uint32_t size = 0;
  uint32_t str_len = 0;
  uint32_t glyphs_len = 0;
  glyphs_t* mask_glyphs = NULL;
  canvas_t* c = GET_CANVAS(text_edit);
  uint32_t max_rows = impl->rows->capacity;
  wstr_t* text = &(text_edit->widget->text);
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
  glyphs = text_edit_create_glyphs(text_edit, text->str, text->size);
  if (impl->glyphs != NULL) {
    glyphs_destroy(impl->glyphs);
  }
  impl->glyphs = glyphs;
  if (glyphs != NULL) {
    char_w = glyphs_measure(glyphs, 0, 1);
  }

  if (impl->mask) {
    mask_glyphs = text_edit_create_glyphs(text_edit, &impl->mask_char, 1);
    if (impl->mask_glyphs != NULL) {
      glyphs_destroy(impl->mask_glyphs);
    }
    impl->mask_glyphs = mask_glyphs;
  }

  if (layout_info->w < char_w) {
    return RET_OK;
  }

  if (glyphs != NULL) {
    str_len = glyphs_get_str_length(glyphs);
    while ((offset < str_len || str_len == 0) && i < max_rows) {
      iter = text_edit_layout_line(text_edit, i, line_index, offset, glyphs, mask_glyphs);
      if (iter == NULL || iter->length == 0) {
        break;
      }
      line_index += iter->line_num;
      offset += iter->length;
      i++;
    }
  } else {
    if (impl->single_line) {
      uint32_t y = (layout_info->h - c->font_size) / 2;
      text_edit_set_caret_pos(impl, 0, y, c->font_size, 0, 0);
    }
  }

  if (glyphs != NULL && offset < glyphs_get_str_length(glyphs)) {
    text->size = offset;
    text->str[offset] = L'\0';
  }

  impl->rows->size = i;

  text_edit_fix_oy(impl);

  text_edit_update_input_rect(text_edit);
  if (!impl->single_line) {
    text_edit_update_caret_pos(text_edit);
  }

  text_edit_notify(text_edit);

  return RET_OK;
}

ret_t text_edit_layout(text_edit_t* text_edit) {
  if (text_edit == NULL || GET_CANVAS(text_edit) == NULL || text_edit->widget == NULL ||
      text_edit->widget->initializing || text_edit->widget->astyle == NULL) {
    return RET_BAD_PARAMS;
  }
  if (text_edit->ignore_layout) {
    return RET_OK;
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
    row->num_glyph = info->glyph_count;
    row->glyph_arr = info->glyph_arr;
  } else {
    row->x0 = 0;
    row->x1 = 0;
    row->num_chars = 1;
    row->num_glyph = 0;
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

  canvas_set_stroke_color(c, caret_color);
  canvas_draw_vline(c, x, y, c->font_size);

  return RET_OK;
}

static ret_t text_edit_paint_tips_mlines_text(text_edit_t* text_edit, canvas_t* c, line_parser_t* p,
                                              glyphs_t* glyphs) {
  int32_t y = 0;
  int32_t w = 0;
  int32_t font_size = 0;
  int32_t line_height = 0;
  const wchar_t* str = NULL;
  DECL_IMPL(text_edit);
  widget_t* widget = text_edit->widget;
  text_layout_info_t* layout_info = 0;
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && c != NULL, RET_BAD_PARAMS);

  str = glyphs_get_str(glyphs);
  font_size = c->font_size;
  layout_info = &(impl->layout_info);
  line_height = font_size + style_get_int(text_edit->widget->astyle, STYLE_ID_SPACER, 2);

  w = layout_info->w;
  y = layout_info->margin_t;
  while (line_parser_next(p) == RET_OK) {
    uint32_t size = 0;
    uint32_t glyph_count = p->line_size;
    rect_t r = rect_init(layout_info->margin_l, y, w, font_size);

    if ((y + font_size) > layout_info->h) {
      break;
    }

    if (glyph_count > 0) {
      for (size = 0; size < p->line_size; size++) {
        if (CHAR_IS_LINE_BREAK(str[size + p->line_index])) {
          break;
        }
      }
      widget_draw_text_in_rect_with_glyphs(widget, c, glyphs, p->line_index, size, &r, FALSE);
    }

    y += line_height;
  }

  return RET_OK;
}

static ret_t text_edit_paint_tips_text(text_edit_t* text_edit, canvas_t* c) {
  DECL_IMPL(text_edit);
  uint32_t glyphs_length = 0;
  wstr_t* text = &(impl->tips);
  widget_t* widget = text_edit->widget;
  text_layout_info_t* layout_info = &(impl->layout_info);

  if (text->size > 0) {
    glyphs_t* glyphs = widget_create_glyphs(widget, c, (const wchar_t*)text->str, text->size);
    if (glyphs != NULL) {
      glyphs_length = glyphs_get_length(glyphs);
      if (impl->tips_is_mlines) {
        line_parser_t p;
        line_parser_init(&p, glyphs, c->font_size, layout_info->w, TRUE, TRUE);
        if (p.total_lines > 1) {
          text_edit_paint_tips_mlines_text(text_edit, c, &p, glyphs);
        } else {
          align_h_t align_h = c->text_align_h;
          align_v_t align_v = c->text_align_v;
          rect_t r = rect_init(layout_info->margin_l, layout_info->margin_t, layout_info->w,
                               layout_info->h);
          canvas_set_text_align(c, align_h, ALIGN_V_TOP);
          canvas_draw_text_bidi_in_rect_by_glyphs(c, glyphs, 0, glyphs_length, &r, FALSE);
          canvas_set_text_align(c, align_h, align_v);
        }
        line_parser_deinit(&p);
      } else {
        rect_t r =
            rect_init(layout_info->margin_l, layout_info->margin_t, layout_info->w, layout_info->h);
        canvas_draw_text_bidi_in_rect_by_glyphs(c, glyphs, 0, glyphs_length, &r, FALSE);
      }
      glyphs_destroy(glyphs);
    }
  }

  return RET_OK;
}

static int32_t text_edit_calc_x_on_canvas(text_edit_t* text_edit, line_info_t* iter,
                                          glyphs_t* glyphs, glyphs_t* mask_glyphs, canvas_t* c) {
  DECL_IMPL(text_edit);
  widget_t* widget = text_edit->widget;
  wstr_t* text = &(widget->text);
  text_layout_info_t* layout_info = &(impl->layout_info);
  align_h_t align_h = widget_get_text_align_h(text_edit->widget);

  uint32_t row_width = text_edit_measure_text_on_canvas(text_edit, glyphs, mask_glyphs,
                                                        iter->offset, iter->glyph_count, c);
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

static ret_t text_edit_paint_line(text_edit_t* text_edit, canvas_t* c, line_info_t* iter,
                                  uint32_t y, glyphs_t* glyphs, glyphs_t* mask_glyphs) {
  uint32_t x = 0;
  uint32_t k = 0;
  const glyph_t* last_g = NULL;
  bool_t is_fill_rect = FALSE;
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
  color_t select_bg_color = style_get_color(style, STYLE_ID_SELECTED_BG_COLOR, white);
  color_t select_text_color = style_get_color(style, STYLE_ID_SELECTED_TEXT_COLOR, black);
  color_t text_color = style_get_color(style, STYLE_ID_TEXT_COLOR, black);

  uint32_t select_start = tk_min(state->select_start, state->select_end);
  uint32_t select_end = tk_max(state->select_start, state->select_end);

  if (impl->single_line) {
    x = layout_info->margin_l + iter->x;
  } else {
    x = layout_info->margin_l;
  }

  for (k = 0; k < iter->glyph_count; k++) {
    uint32_t offset = glyphs_get_str_index_from_glyph_index(impl->glyphs, iter->glyph_arr[k]);
    bool_t selected = offset >= select_start && offset < select_end;
    bool_t preedit = text_edit_is_preedit_char(text_edit, offset);
    bool_t briefly_show = text_edit_is_briefly_show_char(text_edit, offset);
    int32_t char_w = glyphs_measure(glyphs, iter->glyph_arr[k], 1);
    const glyph_t* g = glyphs_get(glyphs, iter->glyph_arr[k]);
    wchar_t chr =
        (impl->mask && (!preedit && !briefly_show)) ? impl->mask_char : (g != NULL ? g->chr : 0);

    bool_t draw_space = FALSE;
    if (impl->single_line && chr == STB_TEXTEDIT_NEWLINE) {
      draw_space = TRUE;
      char_w = 4;
    }
    if (char_w > 0 && impl->mask && (!preedit && !briefly_show)) {
      char_w = glyphs_measure(mask_glyphs, 0, 1);
    }

    if ((x + char_w) < view_left) {
      x += (char_w == 0 ? char_w : char_w + CHAR_SPACING);
      continue;
    }

    if (x > view_right) {
      break;
    }

    if (chr != STB_TEXTEDIT_NEWLINE) {
      xy_t rx = x - layout_info->ox;
      xy_t ry = y - layout_info->oy;

      // 处理 char_w == 0 时，后面的背景色会覆盖 char_w 为 0 的字模的问题
      if (selected || preedit) {
        if (char_w == 0 && !is_fill_rect) {
          last_g = glyphs_get(glyphs, iter->glyph_arr[k] + g->glyph_count - 1);
          canvas_set_fill_color(c, select_bg_color);
          canvas_fill_rect(
              c, rx, ry, glyphs_measure(glyphs, iter->glyph_arr[k], g->glyph_count) + CHAR_SPACING,
              c->font_size);
          is_fill_rect = TRUE;
        } else if (char_w > 0) {
          if (is_fill_rect && last_g == g) {
            is_fill_rect = FALSE;
          } else {
            canvas_set_fill_color(c, select_bg_color);
            canvas_fill_rect(c, rx, ry, char_w + CHAR_SPACING, c->font_size);
          }
        }
        canvas_set_text_color(c, select_text_color);
      } else {
        canvas_set_text_color(c, text_color);
      }

      /*FIXME: 密码编辑时，*字符本身偏高，看起来不像居中。但是无法拿到字模信息，只好手工修正一下。*/
      if (impl->mask && (!preedit && !briefly_show)) {
        int32_t oy = 0;
        if (impl->mask_char == '*') {
          oy = c->font_size / 6;
        }
        canvas_draw_text_by_glyphs(c, mask_glyphs, 0, 1, rx, ry + oy);
      } else if (!draw_space) {
        canvas_draw_text_by_glyphs(c, glyphs, iter->glyph_arr[k], 1, rx, ry);
      }

      x += (char_w == 0 ? char_w : char_w + CHAR_SPACING);
    }
  }

  return RET_OK;
}

static ret_t text_edit_paint_real_text(text_edit_t* text_edit, canvas_t* c) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  rows_t* rows = impl->rows;
  glyphs_t* mask_glyphs = NULL;
  uint32_t line_height = impl->line_height;
  text_layout_info_t* layout_info = &(impl->layout_info);
  int32_t view_top = layout_info->oy + layout_info->margin_t;
  int32_t view_bottom = layout_info->oy + layout_info->margin_t + layout_info->h;
  glyphs_t* glyphs = impl->glyphs;
  return_value_if_fail(glyphs != NULL, RET_FAIL);
  mask_glyphs = impl->mask_glyphs;

  for (i = 0; i < rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = rows->row + i;

    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      int32_t y = 0;

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

      text_edit_paint_line(text_edit, c, line, y, glyphs, mask_glyphs);
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

  if (is_notify || impl->glyphs == NULL || !glyphs_get_valid(impl->glyphs) ||
      !tk_str_eq(impl->glyphs->font->name, c->font_name) ||
      impl->glyphs->font_size != c->font_size) {
    text_edit_layout(text_edit);
  }

  if (text_edit_paint_text(text_edit, c) == RET_OK) {
    DECL_IMPL(text_edit);
    STB_TexteditState* state = &(impl->state);

    if (state->select_start == state->select_end && impl->caret_visible) {
      text_edit_paint_caret(text_edit, c);
    }
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
    impl->font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
    impl->font_name = system_info_fix_font_name(style_get_str(style, STYLE_ID_FONT_NAME, NULL));

    text_edit_layout(text_edit);
    impl->is_first_time_layout = FALSE;
  } else if (text_edit_is_need_layout(text_edit)) {
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

static wh_t text_edit_measure_char(STB_TEXTEDIT_STRING* str, canvas_t* c,
                                   STB_TEXTEDIT_CHARTYPE* iter, STB_TEXTEDIT_CHARTYPE* chr) {
  STB_TEXTEDIT_CHARTYPE _chr;
  if (NULL == chr) {
    chr = &_chr;
  }
  *chr = *iter;
  DECL_IMPL(str);
  if (NULL == c) {
    c = GET_CANVAS(str);
  }

  if (impl->single_line) {
    if (*chr == STB_TEXTEDIT_NEWLINE) {
      *chr = ' ';
    } else if (*chr == STB_TEXTEDIT_NEWLINER && iter[1] == STB_TEXTEDIT_NEWLINE) {
      return 0;
    }
  }

  return canvas_measure_text(c, chr, 1) + CHAR_SPACING;
}

static int text_edit_get_char_width(STB_TEXTEDIT_STRING* str, int pos, int offset) {
  STB_TEXTEDIT_CHARTYPE* iter = &str->widget->text.str[pos + offset];
  DECL_IMPL(str);
  const glyph_t* g = glyphs_get(impl->glyphs, pos + offset);
  STB_TEXTEDIT_CHARTYPE chr = g->chr;
  uint32_t mask_w = impl->mask_glyphs != NULL ? glyphs_measure(impl->mask_glyphs, 0, 1) : 0;
  bool_t preedit = text_edit_is_preedit_char(str, pos + offset);
  bool_t briefly_show = text_edit_is_briefly_show_char(str, pos + offset);

  if (chr == STB_TEXTEDIT_NEWLINE) {
    return 0;
  } else if (g != NULL) {
    // 直接获取整个字模簇的宽度
    int32_t chr_w = (mask_w > 0 && (!preedit && !briefly_show) && impl->mask)
                        ? mask_w
                        : glyphs_measure(impl->glyphs, pos + offset, g->glyph_count);
    if (chr_w > 0) {
      chr_w += CHAR_SPACING;
    }
    return chr_w;
  }
  return text_edit_measure_char(str, NULL, iter, NULL);
}

static int text_edit_get_glyphs_num(STB_TEXTEDIT_STRING* str, int pos, bool_t is_str) {
  DECL_IMPL(str);
  const glyph_t* g = NULL;
  if (pos < 0 || impl->glyphs == NULL) {
    return 1;
  }
  g = glyphs_get(impl->glyphs, (uint32_t)pos);
  if (g != NULL) {
    if (is_str) {
      return g->str_count;
    }
    return g->glyph_count;
  }
  return 1;
}

static int text_edit_get_glyphs_str_num(STB_TEXTEDIT_STRING* str, int pos) {
  return text_edit_get_glyphs_num(str, pos, TRUE);
}

static int text_edit_get_glyphs_glyph_num(STB_TEXTEDIT_STRING* str, int pos) {
  return text_edit_get_glyphs_num(str, pos, FALSE);
}

static int text_edit_get_bidi_type(STB_TEXTEDIT_STRING* str, int pos) {
  DECL_IMPL(str);
  const glyph_t* g = glyphs_get(impl->glyphs, pos);

  if (g != NULL) {
    return g->bidi_type;
  } else {
    return 0;
  }
}

static STB_TEXTEDIT_CHARTYPE text_edit_get_char(STB_TEXTEDIT_STRING* str, int offset) {
  STB_TEXTEDIT_CHARTYPE chr = str->widget->text.str[offset];
  DECL_IMPL(str);
  if (impl->single_line) {
    if (chr == STB_TEXTEDIT_NEWLINE) {
      return ' ';
    }
  }
  return chr;
}

static int text_edit_insert(STB_TEXTEDIT_STRING* str, int pos, STB_TEXTEDIT_CHARTYPE* newtext,
                            int num) {
  wstr_t* text = &(str->widget->text);
  DECL_IMPL(str);
  uint32_t line_break_num = impl->rows->capacity > impl->last_row_number
                                ? impl->rows->capacity - impl->last_row_number
                                : 0;
  uint32_t i = 0;

  for (i = 0; i < num; i++) {
    if ((i + 1) < num && (TWINS_WCHAR_IS_LINE_BREAK(newtext[i], newtext[i + 1]))) {
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
  }

  return num;
}

#define KEYDOWN_BIT 0x80000000
#define STB_TEXTEDIT_STRINGLEN(str) ((str)->widget->text.size)
#define STB_TEXTEDIT_LAYOUTROW text_edit_layout_for_stb
#define STB_TEXTEDIT_GETWIDTH(str, n, i) text_edit_get_char_width(str, n, i)
#define STB_TEXTEDIT_STR_TO_GLYPH(str, n) \
  glyphs_get_glyph_index_from_str_index(((text_edit_impl_t*)str)->glyphs, n)
#define STB_TEXTEDIT_GLYPH_TO_STR(str, n) \
  glyphs_get_str_index_from_glyph_index(((text_edit_impl_t*)str)->glyphs, n)
#define STB_TEXTEDIT_GET_GLYPHS_STR_NUM(str, n) text_edit_get_glyphs_str_num(str, n)
#define STB_TEXTEDIT_GET_GLYPHS_GLYPH_NUM(str, n) text_edit_get_glyphs_glyph_num(str, n)
#define STB_TEXTEDIT_GETBIDITYPE(str, n) text_edit_get_bidi_type(str, n)
#define STB_TEXTEDIT_KEYTOTEXT(key) (((key)&KEYDOWN_BIT) ? 0 : ((uint16_t)key))
#define STB_TEXTEDIT_GETCHAR(str, i) text_edit_get_char(str, i)
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

ret_t text_edit_get_offset_at_line(text_edit_t* text_edit, uint32_t line_index, uint32_t* start,
                                   uint32_t* end) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && start != NULL && end != NULL, RET_BAD_PARAMS);

  for (i = 0; i < impl->rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = impl->rows->row + i;
    assert(row != NULL);

    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      assert(line != NULL);

      if (k == line_index) {
        *start = line->offset;
        *end = line->offset + line->length;
        return RET_OK;
      }
    }
  }

  return RET_NOT_FOUND;
}

ret_t text_edit_get_offset_at_row(text_edit_t* text_edit, uint32_t row_index, uint32_t* start,
                                  uint32_t* end) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && start != NULL && end != NULL, RET_BAD_PARAMS);

  if (row_index < impl->rows->size) {
    line_info_t* line = NULL;
    row_info_t* row = impl->rows->row + row_index;
    assert(row != NULL);

    line = (line_info_t*)darray_get(&row->info, 0);
    assert(line != NULL);

    *start = line->offset;
    *end = line->offset + row->length;
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

int32_t text_edit_get_line_at(text_edit_t* text_edit, uint32_t offset) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, -1);

  for (i = 0; i < impl->rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = impl->rows->row + i;
    assert(row != NULL);

    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      assert(line != NULL);

      if (offset < line->offset + line->length) {
        return k;
      }
    }
  }

  return -1;
}

int32_t text_edit_get_row_at(text_edit_t* text_edit, uint32_t offset) {
  uint32_t i = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, -1);

  for (i = 0; i < impl->rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = impl->rows->row + i;
    assert(row != NULL);

    for (j = 0; j < row->line_num; j++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      assert(line != NULL);

      if (offset < line->offset + line->length) {
        return i;
      }
    }
  }

  return -1;
}

int32_t text_edit_get_row_of_line(text_edit_t* text_edit, uint32_t line) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, -1);

  for (i = 0; i < impl->rows->size; i++) {
    row_info_t* row = impl->rows->row + i;
    assert(row != NULL);

    if (line >= k && line < k + row->line_num) {
      return i;
    }

    k += row->line_num;
  }

  return -1;
}

ret_t text_edit_get_line_of_row(text_edit_t* text_edit, uint32_t row_index, uint32_t* start,
                                uint32_t* end) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && start != NULL && end != NULL, RET_BAD_PARAMS);

  if (row_index < impl->rows->size) {
    for (i = 0; i < impl->rows->size; i++) {
      row_info_t* row = impl->rows->row + i;
      assert(row != NULL);

      if (i == row_index) {
        *start = k;
        *end = k + row->line_num;
        return RET_OK;
      }

      k += row->line_num;
    }
  }

  return RET_NOT_FOUND;
}

uint32_t text_edit_get_height(text_edit_t* text_edit, uint32_t offset) {
  uint32_t i = 0;
  uint32_t k = 0;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, 0);

  for (i = 0; i < impl->rows->size; i++) {
    uint32_t j = 0;
    row_info_t* row = impl->rows->row + i;
    assert(row != NULL);

    for (j = 0; j < row->line_num; j++, k++) {
      line_info_t* line = (line_info_t*)darray_get(&row->info, j);
      assert(line != NULL);

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

// 获取指定行内指定字模所在位置
static uint16_t text_edit_get_glyphs_pos(glyphs_t* glyphs, line_info_t* line, uint32_t index) {
  uint16_t x = line->x;
  uint32_t i = 0;
  for (i = 0; i < line->glyph_count;) {
    if (line->glyph_arr[i] == index) break;
    const glyph_t* lg = glyphs_get(glyphs, line->glyph_arr[i]);
    if (lg != NULL && lg->chr != STB_TEXTEDIT_NEWLINE && lg->chr != STB_TEXTEDIT_NEWLINER) {
      x += glyphs_measure(glyphs, line->glyph_arr[i], lg->glyph_count) + CHAR_SPACING;
      i += lg->glyph_count;
    } else {
      i++;
    }
  }
  return x;
}

static ret_t text_edit_update_caret_pos(text_edit_t* text_edit) {
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t y = 0;
  uint32_t line_index = 0;
  DECL_IMPL(text_edit);
  row_info_t* row = NULL;
  line_info_t* line = NULL;
  glyphs_t* glyphs = impl->glyphs;
  rows_t* rows = impl->rows;
  bool_t is_setting = FALSE;
  canvas_t* c = GET_CANVAS(text_edit);
  uint32_t font_size = impl->font_size;
  uint32_t line_height = impl->line_height;
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);

  if (glyphs == NULL || !glyphs_get_valid(glyphs)) {
    text_edit_fix_oy(impl);
    text_edit_notify(text_edit);
    return RET_OK;
  }

  for (i = 0; i < rows->size; i++) {
    row = rows->row + i;
    for (j = 0; j < row->line_num; j++, y += line_height, line_index++) {
      line = (line_info_t*)darray_get(&row->info, j);
      if (line == NULL) continue;

      uint32_t line_begin = line->offset;
      uint32_t line_end = line->offset + line->length;

      if ((line_begin <= impl->state.cursor && impl->state.cursor <= line_end) ||
          (j + 1 == row->line_num && impl->state.cursor == line_end)) {
        uint32_t x = 0;
        const glyph_t* g = NULL;
        int32_t target_gi = 0;

        if (impl->state.cursor > line_begin) {
          // 光标不在行首，取前一字模
          target_gi = glyphs_get_glyph_index_from_str_index(glyphs, impl->state.cursor - 1);
          g = glyphs_get(glyphs, target_gi);

          if (g != NULL && (g->chr == STB_TEXTEDIT_NEWLINE || g->chr == STB_TEXTEDIT_NEWLINER)) {
            // 换行符后光标移到下一行起始，按行首逻辑计算
            line_info_t* next_line = NULL;
            row_info_t* next_row = NULL;
            if (i + 1 < rows->size) {
              next_row = rows->row + i + 1;
              next_line = (line_info_t*)darray_get(&next_row->info, 0);
            }
            y += line_height;
            line_index++;
            // 查找下一行的行首字模位置
            if (next_line != NULL) {
              target_gi = glyphs_get_glyph_index_from_str_index(glyphs, next_line->offset);
              g = glyphs_get(glyphs, target_gi);
              x = text_edit_get_glyphs_pos(glyphs, next_line, target_gi);
              // 行首为 RTL，移动到字符右侧显示
              if (g != NULL && g->bidi_type == FONT_BIDI_TYPE_RTL) {
                x += glyphs_measure(glyphs, target_gi, g->glyph_count);
              }
            } else {
              x = 0;
            }
          } else {
            // 普通字符：累加到光标字模前的所有字模宽度
            x = text_edit_get_glyphs_pos(glyphs, line, target_gi);
            // 字符为 LTR，光标在字模右侧
            if (g != NULL && g->bidi_type != FONT_BIDI_TYPE_RTL) {
              x += glyphs_measure(glyphs, target_gi, g->glyph_count);
            }
          }
        } else {
          // 光标在行首：找行首字模在视觉序中的位置
          target_gi = glyphs_get_glyph_index_from_str_index(glyphs, impl->state.cursor);
          g = glyphs_get(glyphs, target_gi);
          x = text_edit_get_glyphs_pos(glyphs, line, target_gi);
          // 行首为 RTL，移动到字符右侧显示
          if (g != NULL && g->bidi_type == FONT_BIDI_TYPE_RTL) {
            x += glyphs_measure(glyphs, target_gi, g->glyph_count);
          }
        }

        is_setting = TRUE;
        text_edit_set_caret_pos(impl, x, y, font_size, line_index, i);
        break;
      }
    }
    if (is_setting) break;
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

  if (impl->single_line) {
    text_edit_layout(text_edit);
  } else {
    text_edit_update_caret_pos(text_edit);
  }

  input_method_t* im = input_method();
  if (im != NULL && im->is_native && im->keyboard == NULL &&
      !widget_get_prop_bool(text_edit->widget, WIDGET_PROP_READONLY, FALSE)) {
    input_method_request(im, NULL);
    input_method_request(im, text_edit->widget);
  }

  text_edit_update_input_rect(text_edit);

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

static ret_t text_edit_paste_from_clip_board(text_edit_t* text_edit) {
  value_t v;
  wstr_t str;
  DECL_IMPL(text_edit);
  const char* data = clip_board_get_text();
  if (data != NULL) {
    value_set_str(&v, data);
    wstr_init(&str, 0);
    wstr_from_value(&str, &v);
    wstr_normalize_newline(&str, impl->single_line ? L' ' : STB_TEXTEDIT_NEWLINE);
    text_edit_paste(text_edit, str.str, str.size);
    wstr_reset(&str);
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
      text_edit_paste_from_clip_board(text_edit);
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

static ret_t text_edit_briefly_show_char_done_impl(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (impl->briefly_show_char) {
    impl->briefly_show_char = FALSE;
    text_edit_layout(text_edit);

    widget_invalidate(text_edit->widget, NULL);
  }

  return RET_OK;
}

static ret_t text_edit_briefly_show_char_done_on_timer(const timer_info_t* timer) {
  text_edit_t* text_edit = (text_edit_t*)(timer->ctx);
  DECL_IMPL(text_edit);

  text_edit_briefly_show_char_done_impl(text_edit);

  impl->briefly_show_char_done_timer_id = TK_INVALID_ID;

  return RET_REMOVE;
}

static ret_t text_edit_briefly_show_char(text_edit_t* text_edit) {
  ret_t ret = RET_FAIL;
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (impl->briefly_show_char_done_timer_id == TK_INVALID_ID) {
    impl->briefly_show_char_done_timer_id = timer_add(text_edit_briefly_show_char_done_on_timer,
                                                      text_edit, BRIEFLY_SHOW_CHAR_TIMER_DURATION);
  } else {
    timer_reset(impl->briefly_show_char_done_timer_id);
  }

  ret = (impl->briefly_show_char_done_timer_id != TK_INVALID_ID) ? RET_OK : RET_FAIL;

  if (ret == RET_OK) {
    impl->briefly_show_char = TRUE;
  }

  return ret;
}

static ret_t text_edit_briefly_show_char_done(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  if (impl->briefly_show_char_done_timer_id != TK_INVALID_ID) {
    timer_remove(impl->briefly_show_char_done_timer_id);
    impl->briefly_show_char_done_timer_id = TK_INVALID_ID;
  }

  return text_edit_briefly_show_char_done_impl(text_edit);
}

inline static ret_t text_edit_key_down_delete_word_prepare(text_edit_t* text_edit, key_event_t* evt,
                                                           bool_t is_prev) {
#ifdef MACOS
  bool_t is_control = evt->cmd;
#else
  bool_t is_control = evt->ctrl;
#endif
  DECL_IMPL(text_edit);
  STB_TexteditState* state = &(impl->state);
  ret_t ret = RET_OK;

  if (is_control && state->select_start == state->select_end) {
    uint32_t cursor = text_edit_get_cursor(text_edit);
    text_edit->ignore_layout = TRUE;
    if (is_prev) {
      ret = text_edit_select_word_impl(text_edit, cursor, NULL, (int32_t*)&cursor);
    } else {
      ret = text_edit_select_word_impl(text_edit, cursor, (int32_t*)&cursor, NULL);
    }
    text_edit->ignore_layout = FALSE;
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
#ifdef WIN32
    case TK_KEY_CLEAR:  // win32下关闭小键盘，数字5会触发TK_KEY_CLEAR
#endif
#endif
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
#ifndef AWTK_WEB
    case TK_KEY_KP_ENTER:
#endif /*AWTK_WEB*/
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
      key = STB_TEXTEDIT_K_LINESTART;
      break;
    }
    case TK_KEY_END: {
      move_caret_pos = TRUE;
      key = STB_TEXTEDIT_K_LINEEND;
      break;
    }
    case TK_KEY_DELETE: {
      text_edit_key_down_delete_word_prepare(text_edit, evt, FALSE);
      key = STB_TEXTEDIT_K_DELETE;
      break;
    }
    case TK_KEY_BACKSPACE: {
      text_edit_key_down_delete_word_prepare(text_edit, evt, TRUE);
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
#ifndef AWTK_WEB
    case TK_KEY_LSHIFT:
    case TK_KEY_LALT:
    case TK_KEY_LCTRL:
#endif /*AWTK_WEB*/
    case TK_KEY_RSHIFT:
    case TK_KEY_RCTRL:
    case TK_KEY_RALT:
    case TK_KEY_CAPSLOCK:
    case TK_KEY_COMMAND:
    case TK_KEY_MENU:
    case TK_KEY_WHEEL: {
      return RET_OK;
    }
    default: {
      if (key < 128 && tk_isprint(key)) {
        if (input_method_is_native(input_method())) {
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

  if (impl->on_text_will_delete) {
    delete_type_t delete_type;
    if (key == STB_TEXTEDIT_K_DELETE) {
      delete_type = DELETE_BY_KEY_DELETE;
    } else if (key == STB_TEXTEDIT_K_BACKSPACE) {
      delete_type = DELETE_BY_KEY_BACKSPACE;
    } else if (key < 128 && tk_isprint(key)) {
      delete_type = DELETE_BY_INPUT;
    } else {
      goto on_text_will_delete_end;
    }
    if (impl->on_text_will_delete(impl->on_text_will_delete_ctx, delete_type) == RET_STOP) {
      return RET_STOP;
    }
  }
on_text_will_delete_end:

  if (key < 128 && tk_isprint(key)) {
    if (impl->on_char_will_input) {
      if (impl->on_char_will_input(impl->on_char_will_input_ctx, (wchar_t)key) == RET_STOP) {
        return RET_STOP;
      }
    }
    text_edit_briefly_show_char(text_edit);
  } else {
    text_edit_briefly_show_char_done(text_edit);
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

  text_edit_update_input_rect(text_edit);

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
    ret = tk_utf8_dup_utf16(text_edit->widget->text.str + state.select_start, size);
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

inline static bool_t text_edit_str_is_delete_key(const wchar_t* str, uint32_t size, uint32_t* key,
                                                 delete_type_t* type) {
  uint32_t tmp_key = 0;
  delete_type_t tmp_type = DELETE_BY_INPUT;
  return_value_if_fail(str != NULL, FALSE);

  if (NULL == key) {
    key = &tmp_key;
  }
  if (NULL == type) {
    type = &tmp_type;
  }

  if (size == 1) {
    char text[4] = {0};
    tk_utf8_from_utf16(str, text, sizeof(text));
    if (1 == tk_strlen(text)) {
      switch (*text) {
        case TK_KEY_BACKSPACE: {
          *key = STB_TEXTEDIT_K_BACKSPACE;
          *type = DELETE_BY_KEY_BACKSPACE;
          return TRUE;
        }
        case TK_KEY_DELETE: {
          *key = STB_TEXTEDIT_K_DELETE;
          *type = DELETE_BY_KEY_DELETE;
          return TRUE;
        }
        default: {
        } break;
      }
    }
  }

  return FALSE;
}

ret_t text_edit_paste(text_edit_t* text_edit, const wchar_t* str, uint32_t size) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL && str != NULL, RET_BAD_PARAMS);

  if (text_edit_str_is_delete_key(str, size, NULL, NULL)) {
    /* input_method 发过来的，text_edit_key_down 已经处理了，所以这里跳过 */
    return RET_SKIP;
  }

  if (impl->on_text_will_delete) {
    if (impl->on_text_will_delete(impl->on_text_will_delete_ctx, DELETE_BY_INPUT) == RET_STOP) {
      return RET_STOP;
    }
  }
  if (impl->on_char_will_input) {
    uint32_t i, count;
    for (i = 0, count = 0; i < size; i++) {
      if (impl->on_char_will_input(impl->on_char_will_input_ctx, str[i]) == RET_STOP) {
        continue;
      }
      stb_textedit_paste(text_edit, &(impl->state), str + i, 1);
      count++;
    }
    if (count == 0) {
      return RET_SKIP;
    }
    size = count;
  } else {
    stb_textedit_paste(text_edit, &(impl->state), str, size);
  }

  if (impl->preedit) {
    impl->preedit_chars_nr += size;
  }

  if (size == 1) {
    text_edit_briefly_show_char(text_edit);
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
    if (impl->single_line) {
      text_edit_layout(text_edit);
    } else {
      text_edit_update_caret_pos(text_edit);
      text_edit_update_input_rect(text_edit);
    }
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

ret_t text_edit_set_tips(text_edit_t* text_edit, const char* tips, bool_t mlines) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->tips_is_mlines = mlines;
  if (TK_STR_IS_EMPTY(tips)) {
    wstr_clear(&(impl->tips));
  } else {
    wstr_set_utf8(&(impl->tips), tips);
  }
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
    tk_swap(start, end, uint32_t);
  }

  impl->state.select_start = start;
  impl->state.select_end = tk_min(end, text_edit->widget->text.size);

  if (impl->single_line) {
    text_edit_layout(text_edit);
  } else {
    text_edit_update_caret_pos(text_edit);
    text_edit_update_input_rect(text_edit);
  }

  return RET_OK;
}

static ret_t text_edit_select_word_impl(text_edit_t* text_edit, uint32_t cursor, int32_t* start,
                                        int32_t* end) {
  ret_t ret = RET_SKIP;

  if (start != NULL && end != NULL) {
    ret = text_edit_set_select(text_edit, *start, *end);
  } else {
    int32_t left = 0, right = 0;
    uint32_t len = text_edit->widget->text.size;
    wchar_t* text = text_edit->widget->text.str;

    ret = tk_wstr_select_word(text, len, cursor, &left, &right);
    if (RET_OK == ret) {
      if (start != NULL && right <= *start) {
        if (right + 1 <= len) {
          ret = tk_wstr_select_word(text, len, right + 1, &left, &right);
        } else {
          ret = RET_SKIP;
        }
      } else if (end != NULL && *end <= left) {
        if (left >= 1) {
          ret = tk_wstr_select_word(text, len, left - 1, &left, &right);
        } else {
          ret = RET_SKIP;
        }
      }
      if (RET_OK == ret) {
        ret = text_edit_set_select(text_edit, (start != NULL) ? *start : left,
                                   (end != NULL) ? *end : right);
      }
    }
  }

  return ret;
}

ret_t text_edit_select_word(text_edit_t* text_edit, uint32_t cursor) {
  return text_edit_select_word_impl(text_edit, cursor, NULL, NULL);
}

ret_t text_edit_select_all(text_edit_t* text_edit) {
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  return text_edit_set_select(text_edit, 0, text_edit->widget->text.size);
}

ret_t text_edit_unselect(text_edit_t* text_edit) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->state.select_end = impl->state.select_start;

  if (impl->single_line) {
    text_edit_layout(text_edit);
  } else {
    text_edit_update_caret_pos(text_edit);
    text_edit_update_input_rect(text_edit);
  }

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
  state->current_row_index = impl->caret_row_index;
  state->current_line_index = impl->caret_line_index;
  state->cursor = impl->state.cursor;
  state->max_rows = impl->rows->capacity;
  state->last_row_number = impl->last_row_number;
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
  if (impl->briefly_show_char_done_timer_id != TK_INVALID_ID) {
    timer_remove(impl->briefly_show_char_done_timer_id);
  }
  if (impl->glyphs != NULL) {
    glyphs_destroy(impl->glyphs);
    impl->glyphs = NULL;
  }
  if (impl->mask_glyphs != NULL) {
    glyphs_destroy(impl->mask_glyphs);
    impl->mask_glyphs = NULL;
  }

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

ret_t text_edit_set_on_text_will_delete(text_edit_t* text_edit,
                                        text_edit_on_text_will_delete_t on_text_will_delete,
                                        void* ctx) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->on_text_will_delete = on_text_will_delete;
  impl->on_text_will_delete_ctx = ctx;

  return RET_OK;
}

ret_t text_edit_set_on_char_will_input(text_edit_t* text_edit,
                                       text_edit_on_char_will_input_t on_char_will_input,
                                       void* ctx) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->on_char_will_input = on_char_will_input;
  impl->on_char_will_input_ctx = ctx;

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
  DECL_IMPL(text_edit);
  uint32_t size = 0;
  return_value_if_fail(text_edit != NULL && text_edit->widget != NULL && wtext != NULL,
                       RET_BAD_PARAMS);

  size = wcslen(wtext);
  len = tk_min(len, size);
  offset = tk_min(offset, text_edit->widget->text.size);

  len = text_edit_insert(text_edit, offset, (wchar_t*)wtext, len);

  ret = len > 0 ? RET_OK : RET_SKIP;

  if (ret == RET_OK) {
    if (impl->single_line) {
      text_edit_layout(text_edit);
    } else {
      text_edit_multi_line_insert_text_layout(text_edit, offset, len, wtext, FALSE, 0);
    }
    if (offset + len != text_edit_get_cursor(text_edit)) {
      text_edit_set_cursor(text_edit, offset + len);
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

  wstr_init(&s, 0);
  wstr_set_utf8(&s, text);
  ret = text_edit_insert_wtext_with_len(text_edit, offset, s.str, len);
  wstr_reset(&s);

  return ret;
}

ret_t text_edit_insert_text(text_edit_t* text_edit, uint32_t offset, const char* text) {
  return text_edit_insert_text_with_len(text_edit, offset, text, tk_strlen(text));
}

ret_t text_edit_set_lock_scrollbar_value(text_edit_t* text_edit, bool_t lock) {
  DECL_IMPL(text_edit);
  return_value_if_fail(text_edit != NULL, RET_BAD_PARAMS);

  impl->lock_scrollbar_value = lock;

  return RET_OK;
}

#include "window.h"
#include "window_manager.h"

static ret_t text_edit_on_copy(void* ctx, event_t* e) {
  text_edit_t* text_edit = (text_edit_t*)ctx;
  text_edit_copy(text_edit);

  return RET_OK;
}

static ret_t text_edit_on_cut(void* ctx, event_t* e) {
  text_edit_t* text_edit = (text_edit_t*)ctx;
  text_edit_cut(text_edit);
  text_edit_layout(text_edit);

  return RET_OK;
}

static ret_t text_edit_on_paste(void* ctx, event_t* e) {
  text_edit_t* text_edit = (text_edit_t*)ctx;
  text_edit_paste_from_clip_board(text_edit);

  return RET_OK;
}

static ret_t text_edit_on_select_all(void* ctx, event_t* e) {
  text_edit_t* text_edit = (text_edit_t*)ctx;
  text_edit_select_all(text_edit);

  return RET_OK;
}

ret_t text_edit_show_context_menu(text_edit_t* text_edit, int32_t x, int32_t y) {
  widget_t* win = window_open("edit_menu");
  widget_t* wm = window_manager();

  if (win != NULL) {
    DECL_IMPL(text_edit);
    widget_t* copy = widget_lookup(win, "copy", TRUE);
    widget_t* cut = widget_lookup(win, "cut", TRUE);
    widget_t* paste = widget_lookup(win, "paste", TRUE);
    widget_t* select_all = widget_lookup(win, "select_all", TRUE);

    if ((x + win->w) > wm->w) {
      x = wm->w - win->w;
    }

    if ((y + win->h) > wm->h) {
      y = wm->h - win->h;
    }

    if (copy != NULL) {
      if (impl->state.select_start != impl->state.select_end) {
        widget_on(copy, EVT_CLICK, text_edit_on_copy, text_edit);
      } else {
        widget_set_enable(copy, FALSE);
      }
    }
    bool_t is_readonly = widget_get_prop_bool(text_edit->widget, WIDGET_PROP_READONLY, FALSE);
    if (cut != NULL) {
      if (!is_readonly && impl->state.select_start != impl->state.select_end) {
        widget_on(cut, EVT_CLICK, text_edit_on_cut, text_edit);
      } else {
        widget_set_enable(cut, FALSE);
      }
    }

    if (paste != NULL) {
      if (!is_readonly && clip_board_get_text() != NULL) {
        widget_on(paste, EVT_CLICK, text_edit_on_paste, text_edit);
      } else {
        widget_set_enable(paste, FALSE);
      }
    }

    if (select_all != NULL) {
      if (text_edit->widget->text.size > 0) {
        widget_on(select_all, EVT_CLICK, text_edit_on_select_all, text_edit);
      } else {
        widget_set_enable(select_all, FALSE);
      }
    }

    widget_move(win, x, y);
    widget_set_prop_bool(win, WIDGET_PROP_IS_KEYBOARD, TRUE);
  }

  return RET_OK;
}

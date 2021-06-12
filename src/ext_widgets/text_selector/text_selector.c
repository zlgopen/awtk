/**
 * File:   text_selector.h
 * Author: AWTK Develop Team
 * Brief:  text_selector
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
 * 2018-09-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/easing.h"
#include "widgets/button.h"
#include "base/layout.h"
#include "widgets/popup.h"
#include "base/window.h"
#include "tkc/tokenizer.h"
#include "text_selector/text_selector.h"
#include "widget_animators/widget_animator_scroll.h"

static const wchar_t* text_selector_get_wtext(widget_t* widget);
static ret_t text_selector_set_all_options_localize_text(widget_t* widget);
static ret_t text_selector_scroll_to(widget_t* widget, int32_t yoffset_end);
static ret_t text_selector_sync_yoffset_with_selected_index(text_selector_t* text_selector, bool_t is_anim);

const char* s_text_selector_properties[] = {WIDGET_PROP_TEXT,
                                            WIDGET_PROP_VALUE,
                                            WIDGET_PROP_OPTIONS,
                                            TEXT_SELECTOR_PROP_VISIBLE_NR,
                                            WIDGET_PROP_SELECTED_INDEX,
                                            WIDGET_PROP_LOCALIZE_OPTIONS,
                                            TEXT_SELECTOR_PROP_Y_SPEED_SCALE,
                                            TEXT_SELECTOR_PROP_LOOP_OPTIONS,
                                            TEXT_SELECTOR_PROP_ANIMATION_TIME,
                                            NULL};

static ret_t text_selector_paint_mask(widget_t* widget, canvas_t* c) {
  int32_t i = 0;
  int32_t y = 0;
  int32_t n = widget->h / 2;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  easing_func_t easing = easing_get(EASING_CUBIC_IN);
  color_t fc = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  const char* fg_image = style_get_str(style, STYLE_ID_FG_IMAGE, NULL);
  color_t mask_color = style_get_color(style, STYLE_ID_MASK_COLOR, trans);

  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  int32_t visible_nr = text_selector->visible_nr;
  int32_t item_height = text_selector->draw_widget_h / visible_nr;

  if (fc.rgba.a) {
    canvas_set_stroke_color(c, fc);
    for (i = 1; i < visible_nr; i++) {
      y = i * item_height;
      canvas_draw_hline(c, 0, y, widget->w);
    }
  }

  if (mask_color.rgba.a) {
    for (i = 0; i < n; i++) {
      y = n - i - 1;

      mask_color.rgba.a = 0xff * easing((float_t)i / (float_t)n);
      canvas_set_stroke_color(c, mask_color);
      canvas_draw_hline(c, 0, y, widget->w);
    }

    for (i = 0; i < n; i++) {
      y = widget->h - n + i;

      mask_color.rgba.a = 0xff * easing((float_t)i / (float_t)n);
      canvas_set_stroke_color(c, mask_color);
      canvas_draw_hline(c, 0, y, widget->w);
    }
  }

  if (fg_image != NULL && *fg_image) {
    bitmap_t img;
    rect_t r = rect_init(0, 0, widget->w, widget->h);
    if (widget_load_image(widget, fg_image, &img) == RET_OK) {
      image_draw_type_t draw_type =
          (image_draw_type_t)style_get_int(style, STYLE_ID_FG_IMAGE_DRAW_TYPE, IMAGE_DRAW_CENTER);
      canvas_draw_image_ex(c, &img, draw_type, (const rect_t*)&r);
    }
  }

  return RET_OK;
}

static int32_t text_selector_range_yoffset(int32_t value, int32_t min_yoffset, int32_t max_yoffset,
                                           int32_t item_height, int32_t empty_item_height,
                                           bool_t loop_options) {
  if (value < min_yoffset) {
    int32_t tmp = max_yoffset + empty_item_height + item_height;
    if (loop_options && tmp != 0) {
      int32_t n = tk_abs(value / tmp) + 1;
      value = value + tmp * n;
    } else {
      value = min_yoffset;
    }
  } else if (value > max_yoffset) {
    int32_t tmp = max_yoffset + empty_item_height + item_height;
    if (loop_options && tmp != 0) {
      value = value % tmp;
    } else {
      value = max_yoffset;
    }
  }
  return value;
}

static ret_t text_selector_prepare_highlight_style(widget_t* widget, canvas_t* c, float_t d,
                                                   bool_t set_color) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  color_t hg_tc = style_get_color(style, STYLE_ID_HIGHLIGHT_TEXT_COLOR, trans);
  const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  uint16_t hg_font_size = style_get_int(style, STYLE_ID_HIGHLIGHT_FONT_SIZE, font_size);
  const char* hg_font_name = style_get_str(style, STYLE_ID_HIGHLIGHT_FONT_NAME, font_name);
  uint16_t tmp_font_size = hg_font_size;

  if (hg_tc.rgba.a > 0 && set_color) {
    canvas_set_text_color(c, hg_tc);
  } else {
    canvas_set_text_color(c, tc);
  }

  if (hg_font_size != font_size) {
    if (d >= 1.0f) {
      tmp_font_size = hg_font_size;
    } else {
      tmp_font_size = font_size + tk_roundi((hg_font_size - font_size) * d);
    }
  }
  canvas_set_font(c, hg_font_name, tmp_font_size);

  return RET_OK;
}

static ret_t text_selector_paint_text(widget_t* widget, canvas_t* c, rect_t* r,
                                      text_selector_option_t* iter, int32_t empty_item_height,
                                      int32_t item_height) {
  uint32_t d = tk_abs(r->y - empty_item_height);

  if (d < item_height) {
    text_selector_prepare_highlight_style(widget, c, (item_height - d) / (float_t)item_height,
                                          d < item_height / 2);
  } else {
    widget_prepare_text_style(widget, c);
  }
  return canvas_draw_text_in_rect(c, iter->text.str, iter->text.size, r);
}

static ret_t text_selector_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  uint32_t y = 0;
  uint32_t i = 0;
  text_selector_option_t* iter = NULL;
  int32_t max_yoffset, tolal_height;
  int32_t yoffset, visible_nr, item_height;
  int32_t options_nr, empty_item_height, min_yoffset;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  yoffset = text_selector->yoffset;
  visible_nr = text_selector->visible_nr;
  item_height = text_selector->draw_widget_h / visible_nr;
  r = rect_init(0, 0, widget->w, item_height);

  options_nr = text_selector_count_options(widget);
  empty_item_height = (text_selector->visible_nr / 2) * item_height;
  min_yoffset = -empty_item_height;
  max_yoffset = (options_nr * item_height + empty_item_height) - text_selector->draw_widget_h;
  tolal_height = max_yoffset + empty_item_height;
  return_value_if_fail(options_nr > 0, RET_BAD_PARAMS);

  widget_prepare_text_style(widget, c);
  iter = text_selector->option_items;

  yoffset = text_selector_range_yoffset(yoffset, min_yoffset, max_yoffset, item_height,
                                        empty_item_height, text_selector->loop_options);

  while (iter != NULL) {
    r.y = y - yoffset;

    if (text_selector->loop_options) {
      if (yoffset <= 0 && r.y > tolal_height) {
        /* 当 yoffset <= 0 说明顶部有空位，需要显示，
         * r.y 是选项加上 yoffset ，如果 r.y 大于 tolal_height 说明这个选项是加上 yoffset 后偏移出来的尾部。
         * 然后把该选项设置到顶部的空位上面。
         */
        r.y = r.y - tolal_height - item_height;
      } else if (r.y + max_yoffset < empty_item_height) {
        /* max_yoffset 为总队列长度，而 r.y 一定为负数，empty_item_height 是做多的剩余空位高度
         * 符合该条件的选项，等于是选项加上 yoffset 后大于总队列长度，意味着选项为偏移出来的头部。
         * 然后把该选项设置到尾部的空位上面。
         */
        r.y = empty_item_height + item_height + r.y + max_yoffset;
      }
    }

    if ((r.y + item_height) >= 0 && r.y < widget->h) {
      text_selector_paint_text(widget, c, &r, iter, empty_item_height, item_height);
    }

    i++;
    iter = iter->next;
    y += item_height;
  }

  return RET_OK;
}

static ret_t text_selector_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r_save;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  rect_t r = rect_init(c->ox, c->oy, widget->w, widget->h);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  canvas_get_clip_rect(c, &r_save);
  r = rect_intersect(&r_save, &r);
  if (r.w > 0 && r.h > 0) {
    canvas_translate(c, 0, text_selector->draw_widget_y);
    canvas_set_clip_rect(c, &r);
    text_selector_paint_self(widget, c);
    text_selector_paint_mask(widget, c);
    canvas_set_clip_rect(c, &r_save);
    canvas_untranslate(c, 0, text_selector->draw_widget_y);
  }

  return RET_OK;
}

static ret_t text_selector_set_real_draw_info(widget_t* widget) {
  int32_t tmp_h = 0;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  tmp_h = widget->h % text_selector->visible_nr;
  if (tmp_h == 0) {
    text_selector->draw_widget_y = 0;
    text_selector->draw_widget_h = widget->h;
  } else {
    text_selector->draw_widget_y = tmp_h / 2;
    text_selector->draw_widget_h = widget->h - tmp_h;
  }
  return RET_OK;
}

static ret_t text_selector_on_layout_children(widget_t* widget) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  text_selector_set_real_draw_info(widget);
  if (!text_selector->is_init) {
    text_selector_sync_yoffset_with_selected_index(text_selector, FALSE);
    text_selector->is_init = TRUE;
  }
  return RET_OK;
}

static ret_t text_selector_on_destroy(widget_t* widget) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  str_reset(&(text_selector->text));
  text_selector_reset_options(widget);

  if (text_selector->locale_info_id != TK_INVALID_ID) {
    locale_info_off(locale_info(), text_selector->locale_info_id);
  }

  return RET_OK;
}

ret_t text_selector_parse_options(widget_t* widget, const char* str) {
  int32_t i = 0;
  tokenizer_t tokenizer;
  tokenizer_t* t = &tokenizer;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  text_selector->options = tk_strdup(str);
  tokenizer_init(t, str, strlen(str), ";");

  while (tokenizer_has_more(t)) {
    int32_t value = 0;
    const char* token = tokenizer_next(t);
    if (*token) {
      const char* text = strchr(token, ':');
      if (text != NULL) {
        text++;
        value = tk_atoi(token);
      } else {
        text = token;
        value = i;
      }

      text_selector_append_option(widget, value, text);
      i++;
    }
  }
  tokenizer_deinit(t);

  return RET_OK;
}

ret_t text_selector_set_range_options_ex(widget_t* widget, int32_t start, uint32_t nr, int32_t step,
                                         const char* format) {
  char text[64];
  uint32_t i = 0;
  return_value_if_fail(widget != NULL && nr < 300 && format != NULL, RET_BAD_PARAMS);

  for (i = 0; i < nr; i++) {
    int32_t value = start + i * step;
    tk_snprintf(text, sizeof(text) - 1, format, value);
    text_selector_append_option(widget, value, text);
  }

  return RET_OK;
}

ret_t text_selector_set_range_options(widget_t* widget, int32_t start, uint32_t nr, int32_t step) {
  return text_selector_set_range_options_ex(widget, start, nr, step, "%d");
}

ret_t text_selector_set_options(widget_t* widget, const char* options) {
  return_value_if_fail(widget != NULL && options != NULL, RET_BAD_PARAMS);

  text_selector_reset_options(widget);
  if (strchr(options, ':') == NULL && strchr(options, '-') != NULL) {
    int nr = 0;
    int end = 0;
    int step = 1;
    int start = 0;
    char format[41];
    memset(format, 0x00, sizeof(format));

    nr = tk_sscanf(options, "%d-%d-%40s", &start, &end, format);

    if (nr < 3) {
      tk_strncpy(format, "%d", sizeof(format) - 1);
    } else {
      char* f = strchr(format, '-');
      if (f != NULL) {
        int p = f - format;
        step = tk_atoi(f + 1);
        memset((void*)f, 0x0, sizeof(format) - p);
      }
    }

    return text_selector_set_range_options_ex(widget, start, end - start + 1, step, format);
  } else {
    return text_selector_parse_options(widget, options);
  }
}

static ret_t text_selector_get_prop(widget_t* widget, const char* name, value_t* v) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    value_set_wstr(v, text_selector_get_wtext(widget));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, text_selector_get_value(widget));
    return RET_OK;
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_VISIBLE_NR)) {
    value_set_int(v, text_selector->visible_nr);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    value_set_int(v, text_selector->selected_index);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, text_selector->yoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPTIONS)) {
    value_set_str(v, text_selector->options);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALIZE_OPTIONS)) {
    value_set_bool(v, text_selector->localize_options);
    return RET_OK;
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_Y_SPEED_SCALE)) {
    value_set_float(v, text_selector->yspeed_scale);
    return RET_OK;
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_LOOP_OPTIONS)) {
    value_set_bool(v, text_selector->loop_options);
    return RET_OK;
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_ANIMATION_TIME)) {
    value_set_uint32(v, text_selector->animating_time);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t text_selector_set_prop(widget_t* widget, const char* name, const value_t* v) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    text_selector_set_value(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    text_selector_set_text(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_VISIBLE_NR)) {
    text_selector_set_visible_nr(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED_INDEX)) {
    text_selector_set_selected_index(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    text_selector->yoffset = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPTIONS)) {
    text_selector_set_options(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALIZE_OPTIONS)) {
    return text_selector_set_localize_options(widget, value_bool(v));
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_Y_SPEED_SCALE)) {
    return text_selector_set_yspeed_scale(widget, value_float(v));
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_LOOP_OPTIONS)) {
    return text_selector_set_loop_options(widget, value_bool(v));
  } else if (tk_str_eq(name, TEXT_SELECTOR_PROP_ANIMATION_TIME)) {
    return text_selector_set_animating_time(widget, value_uint32(v));
  }

  return RET_NOT_FOUND;
}

static ret_t text_selector_on_pointer_down(text_selector_t* text_selector, pointer_event_t* e) {
  velocity_t* v = &(text_selector->velocity);

  velocity_reset(v);
  text_selector->ydown = e->y;
  text_selector->yoffset_save = text_selector->yoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}
static ret_t text_selector_on_pointer_move(text_selector_t* text_selector, pointer_event_t* e) {
  velocity_t* v = &(text_selector->velocity);
  int32_t dy = e->y - text_selector->ydown;
  velocity_update(v, e->e.time, e->x, e->y);

  if (text_selector->wa == NULL && dy) {
    text_selector->yoffset = text_selector->yoffset_save - dy;
  }

  return RET_OK;
}

static ret_t text_selector_set_selected_index_only(text_selector_t* text_selector, int32_t index) {
  widget_t* widget = WIDGET(text_selector);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  if (index != text_selector->selected_index) {
    value_change_event_t evt;
    text_selector_option_t* option = text_selector_get_option(widget, index);
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_uint32(&(evt.old_value), text_selector->selected_index);
    value_set_uint32(&(evt.new_value), index);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      text_selector->last_selected_index = text_selector->selected_index;
      text_selector->selected_index = index;
      if (widget->tr_text != NULL) {
        widget->tr_text = tk_str_copy(widget->tr_text, option->tr_text);
      }
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  }

  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t text_selector_sync_selected_index_with_yoffset(text_selector_t* text_selector) {
  int32_t mid_index = text_selector->visible_nr / 2;
  int32_t item_height = text_selector->draw_widget_h / text_selector->visible_nr;
  int32_t selected_index = text_selector->yoffset / item_height + mid_index;
  if (text_selector->loop_options) {
    selected_index = selected_index % text_selector_count_options(WIDGET(text_selector));
  }

  return text_selector_set_selected_index_only(text_selector, selected_index);
}

static int32_t text_selector_get_yoffset_for_selected_index(text_selector_t* text_selector, int32_t mid_index, int32_t item_height) {
  int32_t yoffset = 0;
  if (text_selector->loop_options) {
    int32_t options_nr = text_selector_count_options(WIDGET(text_selector));
    int32_t n = options_nr >> 1;
    int32_t d = n - tk_abs(tk_abs(text_selector->selected_index - text_selector->last_selected_index) - n);
    yoffset = text_selector->yoffset;
    if (text_selector->selected_index > text_selector->last_selected_index) {
      if (text_selector->selected_index - d == text_selector->last_selected_index) {
        yoffset += (d * item_height);
      } else {
        yoffset -= (d * item_height);
      }
    } else {
      if ((text_selector->last_selected_index + d + 1) % options_nr == text_selector->selected_index) {
        yoffset += ((d + 1) * item_height);
      } else {
        yoffset -= (d * item_height);
      }
    }
  } else {
    yoffset = (text_selector->selected_index - mid_index) * item_height;
  }
  return yoffset;
}

static ret_t text_selector_sync_yoffset_with_selected_index(text_selector_t* text_selector, bool_t is_anim) {
  widget_t* widget = WIDGET(text_selector);
  int32_t mid_index = text_selector->visible_nr / 2;
  int32_t item_height = text_selector->draw_widget_h / text_selector->visible_nr;
  if (is_anim && !widget->loading) {
    int32_t yoffset = text_selector_get_yoffset_for_selected_index(text_selector, mid_index, item_height);
    if (text_selector->wa != NULL) {
      widget_animator_destroy(text_selector->wa);
      text_selector->wa = NULL;
    }
    return text_selector_scroll_to(widget, yoffset);
  } else {
    text_selector->yoffset = (text_selector->selected_index - mid_index) * item_height;
    return widget_invalidate(widget, NULL);
  }
}

static ret_t text_selector_on_scroll_done(void* ctx, event_t* e) {
  int32_t options_nr = 0;
  int32_t item_height = 0;
  int32_t min_yoffset = 0;
  int32_t max_yoffset = 0;
  int32_t empty_item_height = 0;
  widget_t* widget = WIDGET(ctx);
  text_selector_t* text_selector = TEXT_SELECTOR(ctx);
  return_value_if_fail(text_selector != NULL && widget != NULL, RET_BAD_PARAMS);

  options_nr = text_selector_count_options(widget);
  item_height = text_selector->draw_widget_h / text_selector->visible_nr;
  empty_item_height = (text_selector->visible_nr / 2) * item_height;
  min_yoffset = -empty_item_height;
  max_yoffset = (options_nr * item_height + empty_item_height) - text_selector->draw_widget_h;

  text_selector->wa = NULL;

  if (text_selector->loop_options) {
    text_selector->yoffset =
        text_selector_range_yoffset(text_selector->yoffset, min_yoffset, max_yoffset, item_height,
                                    empty_item_height, text_selector->loop_options);
  }

  text_selector_sync_selected_index_with_yoffset(text_selector);

  return RET_REMOVE;
}

static ret_t text_selector_scroll_to(widget_t* widget, int32_t yoffset_end) {
  int32_t yoffset = 0;
  int32_t item_height = 0;
  int32_t min_yoffset = 0;
  int32_t max_yoffset = 0;
  int32_t empty_item_height = 0;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  int32_t options_nr = text_selector_count_options(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  item_height = text_selector->draw_widget_h / text_selector->visible_nr;
  empty_item_height = (text_selector->visible_nr / 2) * item_height;
  min_yoffset = -empty_item_height;
  max_yoffset = (options_nr * item_height + empty_item_height) - text_selector->draw_widget_h;

  if (!text_selector->loop_options) {
    if (yoffset_end < min_yoffset) {
      yoffset_end = min_yoffset;
    }

    if (yoffset_end > (max_yoffset)) {
      yoffset_end = max_yoffset;
    }
  }

  yoffset_end = tk_roundi((float)yoffset_end / (float)item_height) * item_height;

  yoffset = text_selector->yoffset;
  if (yoffset == yoffset_end) {
    return RET_OK;
  }

#ifndef WITHOUT_WIDGET_ANIMATORS
  text_selector->wa = widget_animator_scroll_create(widget, text_selector->animating_time, 0, EASING_SIN_INOUT);
  return_value_if_fail(text_selector->wa != NULL, RET_OOM);

  widget_animator_scroll_set_params(text_selector->wa, 0, yoffset, 0, yoffset_end);
  widget_animator_on(text_selector->wa, EVT_ANIM_END, text_selector_on_scroll_done, text_selector);
  widget_animator_start(text_selector->wa);
#else
  text_selector->yoffset = yoffset_end;
  text_selector_on_scroll_done(widget, NULL);
#endif /*WITHOUT_WIDGET_ANIMATORS*/
  return RET_OK;
}

static ret_t text_selector_on_pointer_up(text_selector_t* text_selector, pointer_event_t* e) {
  int32_t yoffset_end = 0;
  widget_t* widget = WIDGET(text_selector);
  velocity_t* v = &(text_selector->velocity);
  int32_t item_height = text_selector->draw_widget_h / text_selector->visible_nr;

  velocity_update(v, e->e.time, e->x, e->y);
  yoffset_end = text_selector->yoffset - v->yv * text_selector->yspeed_scale;

  if (e->y == text_selector->ydown) {
    /*click*/
    int32_t index = 0;
    int32_t mid_index = text_selector->visible_nr / 2;
    point_t p = {e->x, e->y};
    widget_to_local(widget, &p);
    index = p.y / item_height;
    if (index == mid_index) {
      return RET_OK;
    } else {
      yoffset_end =
          text_selector->yoffset + item_height * (index - mid_index) * text_selector->yspeed_scale;
    }
  }

  text_selector_scroll_to(widget, yoffset_end);

  return RET_OK;
}

static ret_t text_selector_up(widget_t* widget) {
  int32_t yoffset = 0;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  yoffset = text_selector->yoffset - (text_selector->draw_widget_h / text_selector->visible_nr);

  return text_selector_scroll_to(widget, yoffset);
}

static ret_t text_selector_down(widget_t* widget) {
  int32_t yoffset = 0;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  yoffset = text_selector->yoffset + (text_selector->draw_widget_h / text_selector->visible_nr);

  return text_selector_scroll_to(widget, yoffset);
}

static ret_t text_selector_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL && e != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN:
      text_selector->pressed = TRUE;
      widget_grab(widget->parent, widget);
      text_selector_on_pointer_down(text_selector, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      text_selector->pressed = FALSE;
      text_selector_on_pointer_up(text_selector, (pointer_event_t*)e);
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      text_selector->pressed = FALSE;
      text_selector->yoffset = text_selector->yoffset_save;
      break;
    }
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      if (evt->key == TK_KEY_UP) {
        text_selector_up(widget);
      } else if (evt->key == TK_KEY_DOWN) {
        text_selector_down(widget);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (evt->pressed && text_selector->pressed) {
        text_selector_on_pointer_move(text_selector, evt);
        widget_invalidate(widget, NULL);
        ret = RET_STOP;
      }
      break;
    }
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;

      if (evt->dy > 0) {
        text_selector_down(widget);
      } else if (evt->dy < 0) {
        text_selector_up(widget);
      }

      ret = RET_STOP;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      text_selector_sync_yoffset_with_selected_index(text_selector, FALSE);
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t text_selector_on_locale_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  if (text_selector->localize_options) {
    text_selector_set_all_options_localize_text(widget);
  }

  return RET_OK;
}

TK_DECL_VTABLE(text_selector) = {.size = sizeof(text_selector_t),
                                 .inputable = TRUE,
                                 .type = WIDGET_TYPE_TEXT_SELECTOR,
                                 .clone_properties = s_text_selector_properties,
                                 .persistent_properties = s_text_selector_properties,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = text_selector_create,
                                 .on_paint_self = text_selector_on_paint_self,
                                 .on_layout_children = text_selector_on_layout_children,
                                 .set_prop = text_selector_set_prop,
                                 .get_prop = text_selector_get_prop,
                                 .on_destroy = text_selector_on_destroy,
                                 .on_event = text_selector_on_event};

widget_t* text_selector_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(text_selector), x, y, w, h);
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, NULL);

  text_selector->visible_nr = 5;
  text_selector->pressed = FALSE;
  text_selector->is_init = FALSE;
  text_selector->yspeed_scale = 1.0f;

  text_selector->locale_info_id = TK_INVALID_ID;
  text_selector->animating_time = TK_ANIMATING_TIME;

  return widget;
}

ret_t text_selector_reset_options(widget_t* widget) {
  text_selector_option_t* iter = NULL;
  text_selector_option_t* next = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  if (text_selector->wa != NULL) {
    widget_animator_destroy(text_selector->wa);
    text_selector->wa = NULL;
  }

  iter = text_selector->option_items;
  while (iter != NULL) {
    next = iter->next;
    TKMEM_FREE(iter->tr_text);
    wstr_reset(&(iter->text));
    TKMEM_FREE(iter);
    iter = next;
  }
  text_selector->option_items = NULL;
  TKMEM_FREE(text_selector->options);
  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t text_selector_set_options_localize_text(widget_t* widget,
                                                     text_selector_option_t* option) {
  const char* tr_text = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL && option != NULL, RET_BAD_PARAMS);

  if (text_selector->localize_options) {
    tr_text = locale_info_tr(widget_get_locale_info(widget), option->tr_text);
    wstr_set_utf8(&(option->text), tr_text);
  } else {
    wstr_set_utf8(&(option->text), option->tr_text);
  }

  return RET_OK;
}

static ret_t text_selector_set_all_options_localize_text(widget_t* widget) {
  text_selector_option_t* iter = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(widget != NULL && text_selector != NULL, RET_BAD_PARAMS);

  iter = text_selector->option_items;
  while (iter != NULL) {
    text_selector_set_options_localize_text(widget, iter);
    iter = iter->next;
  }

  return RET_OK;
}

ret_t text_selector_append_option(widget_t* widget, int32_t value, const char* text) {
  int32_t size = 0;
  text_selector_option_t* iter = NULL;
  text_selector_option_t* option = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);

  return_value_if_fail(text_selector != NULL && text != NULL, RET_BAD_PARAMS);
  size = sizeof(text_selector_option_t);

  option = (text_selector_option_t*)TKMEM_ALLOC(size);
  return_value_if_fail(option != NULL, RET_OOM);

  memset(option, 0x00, size);
  wstr_init(&(option->text), 0);
  option->tr_text = tk_str_copy(option->tr_text, text);

  option->value = value;
  text_selector_set_options_localize_text(widget, option);

  if (text_selector->option_items != NULL) {
    iter = text_selector->option_items;
    while (iter->next != NULL) iter = iter->next;
    iter->next = option;
  } else {
    text_selector->option_items = option;
  }

  return widget_invalidate(widget, NULL);
}

int32_t text_selector_count_options(widget_t* widget) {
  int32_t nr = 0;
  text_selector_option_t* iter = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, nr);

  iter = text_selector->option_items;
  while (iter != NULL) {
    nr++;
    iter = iter->next;
  }

  return nr;
}

int32_t text_selector_get_option_by_value(widget_t* widget, int32_t value) {
  uint32_t i = 0;
  text_selector_option_t* iter = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, -1);

  iter = text_selector->option_items;
  while (iter != NULL) {
    if (value == iter->value) {
      return i;
    }

    i++;
    iter = iter->next;
  }

  return -1;
}

int32_t text_selector_get_option_by_text(widget_t* widget, const char* text) {
  wstr_t str;
  uint32_t i = 0;
  text_selector_option_t* iter = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, -1);

  wstr_init(&str, 0);
  wstr_set_utf8(&str, text);
  iter = text_selector->option_items;
  while (iter != NULL) {
    if (wcscmp(str.str, iter->text.str) == 0) {
      wstr_reset(&str);
      return i;
    }

    i++;
    iter = iter->next;
  }
  wstr_reset(&str);

  return -1;
}

text_selector_option_t* text_selector_get_option(widget_t* widget, uint32_t index) {
  uint32_t i = 0;
  text_selector_option_t* iter = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, NULL);

  iter = text_selector->option_items;
  while (iter != NULL) {
    if (i == index) {
      return iter;
    }

    i++;
    iter = iter->next;
  }

  return NULL;
}

ret_t text_selector_set_selected_index(widget_t* widget, uint32_t index) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  text_selector_option_t* option = text_selector_get_option(widget, index);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);
  return_value_if_fail(option != NULL, RET_BAD_PARAMS);

  if (index != text_selector->selected_index) {
    text_selector_set_selected_index_only(text_selector, index);
    text_selector_sync_yoffset_with_selected_index(text_selector, TRUE);
  }

  return widget_invalidate(widget, NULL);
}

ret_t text_selector_set_visible_nr(widget_t* widget, uint32_t visible_nr) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  if (visible_nr > 1) {
    text_selector->visible_nr = visible_nr == 3 ? 3 : 5;
  } else {
    text_selector->visible_nr = 1;
  }
  text_selector_sync_yoffset_with_selected_index(text_selector, FALSE);

  return widget_invalidate(widget, NULL);
}

int32_t text_selector_get_value(widget_t* widget) {
  text_selector_option_t* option = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, -1);

  option = text_selector_get_option(widget, text_selector->selected_index);

  return option != NULL ? option->value : -1;
}

ret_t text_selector_set_value(widget_t* widget, int32_t value) {
  int32_t index = text_selector_get_option_by_value(widget, value);
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL && index >= 0, RET_BAD_PARAMS);

  return text_selector_set_selected_index(widget, index);
}

static const wchar_t* text_selector_get_wtext(widget_t* widget) {
  text_selector_option_t* option = NULL;
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, NULL);

  option = text_selector_get_option(widget, text_selector->selected_index);

  return option != NULL ? option->text.str : NULL;
}

const char* text_selector_get_text(widget_t* widget) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, NULL);

  str_from_wstr(&(text_selector->text), text_selector_get_wtext(widget));

  return text_selector->text.str;
}

ret_t text_selector_set_text(widget_t* widget, const char* text) {
  int32_t index = text_selector_get_option_by_text(widget, text);
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL && index >= 0, RET_BAD_PARAMS);

  return text_selector_set_selected_index(widget, index);
}

ret_t text_selector_set_loop_options(widget_t* widget, bool_t loop_options) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  text_selector->loop_options = loop_options;

  return RET_OK;
}

ret_t text_selector_set_localize_options(widget_t* widget, bool_t localize_options) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  text_selector->localize_options = localize_options;

  if (text_selector->localize_options) {
    text_selector->locale_info_id =
        locale_info_on(locale_info(), EVT_LOCALE_CHANGED, text_selector_on_locale_changed, widget);
  } else {
    if (text_selector->locale_info_id != TK_INVALID_ID) {
      locale_info_off(locale_info(), text_selector->locale_info_id);
    }
  }

  return text_selector_set_all_options_localize_text(widget);
}

ret_t text_selector_set_yspeed_scale(widget_t* widget, float_t yspeed_scale) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  text_selector->yspeed_scale = yspeed_scale;

  return RET_OK;
}

ret_t text_selector_set_animating_time(widget_t* widget, uint32_t animating_time) {
  text_selector_t* text_selector = TEXT_SELECTOR(widget);
  return_value_if_fail(text_selector != NULL, RET_BAD_PARAMS);

  text_selector->animating_time = animating_time;

  return RET_OK;
}

widget_t* text_selector_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, text_selector), NULL);

  return widget;
}

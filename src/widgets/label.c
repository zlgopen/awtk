/**
 * File:   label.h
 * Author: AWTK Develop Team
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 * 2018-12-05 Li XianJing <xianjimli@hotmail.com> supported multi line
 *
 */

#include "tkc/mem.h"
#include "widgets/label.h"
#include "base/widget_vtable.h"
#include "base/window_manager.h"

uint32_t line_breaker_count(const wchar_t* str) {
  uint32_t lines = 1;
  const wchar_t* p = str;

  while (*p) {
    if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
      lines++;
    } else if (*p == '\n') {
      p++;
      lines++;
    } else {
      p++;
    }
  }

  return lines;
}

ret_t line_breaker_break(const wchar_t* str, line_breaker_on_line_t on_line, void* ctx) {
  uint32_t i = 0;
  uint32_t line_len = 0;
  const wchar_t* end = str;
  const wchar_t* start = str;
  uint32_t lines = line_breaker_count(str);

  for (i = 0; (i < lines) && *start; i++) {
    while (*end != '\r' && *end != '\n' && *end) {
      end++;
    }
    line_len = end - start;

    on_line(ctx, i, start, line_len);

    if (*end == '\r') {
      end++;
      if (*end == '\n') {
        end++;
      }
    } else {
      end++;
    }

    start = end;
  }

  return RET_OK;
}

typedef struct _ctx_info_t {
  uint32_t x;
  int32_t y;
  uint32_t w;
  uint32_t pre_line_w;
  canvas_t* c;
  widget_t* widget;
  uint32_t line_height;
} ctx_info_t;

static ret_t label_on_line(void* ctx, uint32_t index, const wchar_t* str, uint32_t size) {
  ctx_info_t* info = (ctx_info_t*)ctx;

  if (info->y < 0 || (info->y + info->line_height) > info->widget->h) {
    info->y += info->line_height;
    return RET_OK;
  }

  if (size > 0) {
    rect_t r = rect_init(info->x, info->y, info->w, info->line_height);
    canvas_draw_text_in_rect(info->c, (wchar_t*)str, size, &r);
  }

  info->y += info->line_height;

  return RET_OK;
}

static ret_t label_paint_text_mlines(widget_t* widget, canvas_t* c, const wchar_t* str,
                                     uint32_t size, uint32_t lines) {
  ctx_info_t ctx;
  style_t* style = widget->astyle;
  int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 2);
  align_v_t align_v = (align_v_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);

  ctx.c = c;
  ctx.y = 0;
  ctx.x = margin;
  ctx.widget = widget;
  ctx.line_height = c->font_size;
  ctx.w = widget->w - 2 * margin;

  switch (align_v) {
    case ALIGN_V_TOP: {
      ctx.y = margin;
      break;
    }
    case ALIGN_V_BOTTOM: {
      ctx.y = widget->h - ctx.line_height * lines - margin;
      break;
    }
    default: {
      ctx.y = ((int32_t)(widget->h) - (int32_t)(ctx.line_height * lines)) / 2;
      break;
    }
  }

  widget_prepare_text_style(widget, c);

  return line_breaker_break(str, label_on_line, &ctx);
}

static ret_t label_paint_text(widget_t* widget, canvas_t* c, const wchar_t* str, uint32_t size) {
  label_t* label = LABEL(widget);
  uint32_t lines = line_breaker_count(str);

  if (lines > 1) {
    return label_paint_text_mlines(widget, c, str, size, lines);
  } else {
    wstr_t str = widget->text;

    if (label->length >= 0) {
      str.size = tk_min(label->length, str.size);
    }

    return widget_paint_helper(widget, c, NULL, &str);
  }
}

static ret_t label_on_paint_self(widget_t* widget, canvas_t* c) {
  if (widget->text.size > 0 && style_is_valid(widget->astyle)) {
    widget_prepare_text_style(widget, c);
    label_paint_text(widget, c, widget->text.str, widget->text.size);
  }

  return RET_OK;
}

static ret_t label_on_line_measure(void* ctx, uint32_t index, const wchar_t* str, uint32_t size) {
  ctx_info_t* info = (ctx_info_t*)ctx;
  float_t text_w = canvas_measure_text(info->c, str, size);

  info->w = tk_max(info->w, text_w);
  info->w = tk_max(info->w, info->pre_line_w);
  info->pre_line_w = info->w;
  info->y += info->line_height;

  return RET_OK;
}

ret_t label_resize_to_content(widget_t* widget, uint32_t min_w, uint32_t max_w, uint32_t min_h,
                              uint32_t max_h) {
  wh_t w = 0;
  wh_t h = 0;
  ctx_info_t ctx;
  int32_t margin = 0;
  style_t* style = NULL;
  label_t* label = LABEL(widget);
  canvas_t* c = widget_get_canvas(widget);

  return_value_if_fail(label != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  margin = style_get_int(style, STYLE_ID_MARGIN, 2);
  widget_prepare_text_style(widget, c);

  ctx.c = c;
  ctx.w = 0;
  ctx.pre_line_w = 0;
  ctx.y = margin;
  ctx.x = margin;
  ctx.widget = widget;
  ctx.line_height = c->font_size;

  line_breaker_break(widget->text.str, label_on_line_measure, &ctx);

  w = ctx.w + 10;
  w = tk_min(w, max_w);
  w = tk_max(w, min_w);

  h = ctx.y + margin;
  h = tk_min(h, max_h);
  h = tk_max(h, min_h);

  widget_resize(widget, w, h);

  return RET_OK;
}

ret_t label_set_length(widget_t* widget, int32_t length) {
  label_t* label = LABEL(widget);
  return_value_if_fail(label != NULL, RET_BAD_PARAMS);
  label->length = length;

  return widget_invalidate_force(widget, NULL);
}

static ret_t label_get_prop(widget_t* widget, const char* name, value_t* v) {
  label_t* label = LABEL(widget);
  return_value_if_fail(label != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    double d = 0;
    wstr_to_float(&(widget->text), &d);
    value_set_double(v, d);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LENGTH)) {
    value_set_int(v, label->length);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t label_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return wstr_from_value(&(widget->text), v);
  } else if (tk_str_eq(name, WIDGET_PROP_LENGTH)) {
    return label_set_length(widget, tk_roundi(value_float(v)));
  }

  return RET_NOT_FOUND;
}

static const char* const s_label_properties[] = {WIDGET_PROP_LENGTH, NULL};

TK_DECL_VTABLE(label) = {.size = sizeof(label_t),
                         .type = WIDGET_TYPE_LABEL,
                         .parent = TK_PARENT_VTABLE(widget),
                         .clone_properties = s_label_properties,
                         .persistent_properties = s_label_properties,
                         .create = label_create,
                         .set_prop = label_set_prop,
                         .get_prop = label_get_prop,
                         .on_paint_self = label_on_paint_self};

widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(label), x, y, w, h);
  label_t* label = LABEL(widget);
  return_value_if_fail(label != NULL, NULL);

  label->length = -1;

  return widget;
}

widget_t* label_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, label), NULL);

  return widget;
}

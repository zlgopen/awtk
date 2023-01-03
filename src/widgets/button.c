/**
 * File:   button.h
 * Author: AWTK Develop Team
 * Brief:  button
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "widgets/button.h"
#include "base/widget_vtable.h"

static ret_t button_remove_timer(widget_t* widget) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_REMOVE);

  if (button->timer_id != TK_INVALID_ID) {
    timer_remove(button->timer_id);
    button->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

static ret_t button_on_repeat(const timer_info_t* info) {
  pointer_event_t evt;
  button_t* button = NULL;
  widget_t* widget = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  button = BUTTON(info->ctx);
  widget = WIDGET(info->ctx);
  return_value_if_fail(button != NULL && widget != NULL, RET_REMOVE);

  evt.x = 0;
  evt.y = 0;
  evt.e = event_init(EVT_CLICK, widget);
  evt.e.size = sizeof(pointer_event_t);

  button->repeat_nr++;
  widget_dispatch(widget, (event_t*)&evt);

  return RET_REPEAT;
}

static ret_t button_notify_pressed_changed(widget_t* widget) {
  value_t v;
  prop_change_event_t e;
  button_t* button = BUTTON(widget);

  e.e = event_init(EVT_PROP_CHANGED, widget);
  e.name = "pressed";
  value_set_bool(&v, button->pressed);
  e.value = &v;

  return widget_dispatch(widget, (event_t*)&e);
}

static ret_t button_pointer_up_cleanup_impl(widget_t* widget, bool_t ungrab) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && widget != NULL, RET_BAD_PARAMS);

  button->pressed = FALSE;
  button_notify_pressed_changed(widget);
  button_remove_timer(widget);
  if (ungrab) {
    widget_ungrab(widget->parent, widget);
  }
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  if (button->enable_preview) {
    widget_t* wm = widget_get_window_manager(widget);
    widget_off_by_ctx(wm, widget);
  }

  return RET_OK;
}

static ret_t button_pointer_up_cleanup(widget_t* widget) {
  return button_pointer_up_cleanup_impl(widget, TRUE);
}

static ret_t button_on_long_press(const timer_info_t* info) {
  pointer_event_t evt;
  widget_t* widget = WIDGET(info->ctx);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  button_pointer_up_cleanup_impl(widget, FALSE);
  widget_dispatch(widget, pointer_event_init(&evt, EVT_LONG_PRESS, widget, 1, 1));

  return RET_REMOVE;
}

#define TK_BUTTON_PREVIEW_SCALE 1.6

static ret_t button_get_preview_rect(widget_t* widget, rect_t* rect) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);
  if (rect != NULL) {
    widget_t* wm = widget_get_window_manager(widget);
    int32_t w = widget->w * TK_BUTTON_PREVIEW_SCALE;
    int32_t h = widget->h * TK_BUTTON_PREVIEW_SCALE;
    int32_t x = -(w - widget->w) / 2;
    int32_t y = -h;
    point_t p = {x, y};

    widget_to_screen(widget, &p);
    if (p.x < 0) {
      p.x = 0;
    }
    if ((p.x + w) > wm->w) {
      p.x = wm->w - w;
    }

    *rect = rect_init(p.x, p.y, w, h);
  }

  return RET_OK;
}

static ret_t button_invalidate(widget_t* widget, const rect_t* rect) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  if (button->enable_preview) {
    rect_t r;
    widget_t* wm = widget_get_window_manager(widget);
    button_get_preview_rect(widget, &r);

    widget_invalidate_force(wm, &r);
  }

  return widget_invalidate_default(widget, rect);
}

static ret_t button_draw_preview(void* ctx, event_t* e) {
  rect_t r;
  widget_t* widget = WIDGET(ctx);
  point_t p = {0, 0};
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0xd0);
  color_t bg_color = style_get_color(style, STYLE_ID_BG_COLOR, trans);
  color_t border_color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  color_t text_color = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  int32_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  const char* font = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  int32_t wox = style_get_int(style, STYLE_ID_X_OFFSET, 0);
  int32_t woy = style_get_int(style, STYLE_ID_Y_OFFSET, 0);
  int32_t round_r = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);

  widget_to_screen(widget, &p);
  canvas_untranslate(c, ox, oy);

  button_get_preview_rect(widget, &r);
  r.y += (round_r ? round_r : 2) + woy;

  canvas_set_fill_color(c, bg_color);
  if (round_r > 0) {
    canvas_fill_rounded_rect(c, &r, &r, &bg_color, round_r);
  } else {
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }

  canvas_set_text_color(c, text_color);
  canvas_set_font(c, font, font_size * TK_BUTTON_PREVIEW_SCALE);
  canvas_draw_text_in_rect(c, widget->text.str, widget->text.size, &r);

  canvas_set_stroke_color(c, border_color);
  if (round_r > 0) {
    canvas_stroke_rounded_rect(c, &r, &r, &border_color, round_r, 1);
  } else {
    canvas_stroke_rect(c, r.x, r.y, r.w, r.h);
  }

  p.x = wox;
  p.y = woy;
  widget_to_screen(widget, &p);
  r = rect_init(p.x + 1, p.y, widget->w - 2, widget->h - 1);
  widget_fill_bg_rect(widget, c, &r, IMAGE_DRAW_CENTER);

  canvas_translate(c, ox, oy);

  return RET_OK;
}

static ret_t button_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && widget != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      button->repeat_nr = 0;
      button->pressed = TRUE;
      button_notify_pressed_changed(widget);
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      button_remove_timer(widget);
      if (button->repeat > 0) {
        button->timer_id = timer_add(button_on_repeat, widget, button->repeat);
      } else if (button->enable_long_press) {
        assert(button->long_press_time > 0);
        button->timer_id = timer_add(button_on_long_press, widget, button->long_press_time);
      }

      widget_grab(widget->parent, widget);

      if (button->enable_preview) {
        widget_t* wm = widget_get_window_manager(widget);
        widget_on(wm, EVT_AFTER_PAINT, button_draw_preview, widget);
      }

      break;
    }
    case EVT_BLUR:
    case EVT_POINTER_DOWN_ABORT: {
      button_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t click;
      pointer_event_t* up = pointer_event_cast(e);
      pointer_event_init(&click, EVT_CLICK, widget, up->x, up->y);

      if (button->pressed && widget_is_point_in(widget, click.x, click.y, FALSE)) {
        button_pointer_up_cleanup(widget);
        ret = widget_dispatch(widget, (event_t*)(&click));
      } else {
        button_pointer_up_cleanup(widget);
      }

      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (button->pressed || !evt->pressed) {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      break;
    }
    default:
      break;
  }

  return ret;
}

ret_t button_set_repeat(widget_t* widget, int32_t repeat) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->repeat = repeat;

  return RET_OK;
}

ret_t button_set_long_press_time(widget_t* widget, uint32_t long_press_time) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->long_press_time = long_press_time;

  return RET_OK;
}

ret_t button_set_enable_long_press(widget_t* widget, bool_t enable_long_press) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->enable_long_press = enable_long_press;

  return RET_OK;
}

ret_t button_set_enable_preview(widget_t* widget, bool_t enable_preview) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  button->enable_preview = enable_preview;

  return RET_OK;
}

static ret_t button_get_prop(widget_t* widget, const char* name, value_t* v) {
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    value_set_int(v, button->repeat);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LONG_PRESS_TIME)) {
    value_set_int(v, button->long_press_time);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    value_set_bool(v, button->enable_long_press);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_PREVIEW)) {
    value_set_bool(v, button->enable_preview);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t button_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LONG_PRESS_TIME)) {
    value_set_int(v, TK_LONG_PRESS_TIME);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    value_set_bool(v, FALSE);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_PREVIEW)) {
    value_set_bool(v, FALSE);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    return button_set_repeat(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_LONG_PRESS_TIME)) {
    return button_set_long_press_time(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_LONG_PRESS)) {
    return button_set_enable_long_press(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE_PREVIEW)) {
    return button_set_enable_preview(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static ret_t button_on_destroy(widget_t* widget) {
  return button_remove_timer(widget);
}

static ret_t button_auto_adjust_size(widget_t* widget) {
  button_t* button = BUTTON(widget);
  canvas_t* c = widget_get_canvas(widget);
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(button != NULL && widget->astyle != NULL, RET_BAD_PARAMS);

  if (!widget->auto_adjust_size || !widget_is_window_created(widget)) {
    return RET_OK;
  } else {
    xy_t w = 0;
    xy_t h = 0;
    style_t* style = widget->astyle;
    int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 2);
    int32_t margin_top = style_get_int(style, STYLE_ID_MARGIN_TOP, margin);
    int32_t margin_left = style_get_int(style, STYLE_ID_MARGIN_LEFT, margin);
    int32_t margin_right = style_get_int(style, STYLE_ID_MARGIN_RIGHT, margin);
    int32_t margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);

    widget_prepare_text_style(widget, c);
    h = c->font_size + margin_top + margin_bottom;
    w = canvas_measure_text(c, widget->text.str, widget->text.size) + margin_left + margin_right;
    widget->w = w;
    widget->h = h;

    return RET_OK;
  }
}

static const char* const s_button_properties[] = {WIDGET_PROP_REPEAT, WIDGET_PROP_LONG_PRESS_TIME,
                                                  WIDGET_PROP_ENABLE_LONG_PRESS,
                                                  WIDGET_PROP_ENABLE_PREVIEW, NULL};

TK_DECL_VTABLE(button) = {.size = sizeof(button_t),
                          .type = WIDGET_TYPE_BUTTON,
                          .space_key_to_activate = TRUE,
                          .return_key_to_activate = TRUE,
                          .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                          .create = button_create,
                          .invalidate = button_invalidate,
                          .clone_properties = s_button_properties,
                          .persistent_properties = s_button_properties,
                          .on_event = button_on_event,
                          .set_prop = button_set_prop,
                          .get_prop = button_get_prop,
                          .get_prop_default_value = button_get_prop_default_value,
                          .auto_adjust_size = button_auto_adjust_size,
                          .on_destroy = button_on_destroy,
                          .on_paint_self = widget_on_paint_self_default};

widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(button), x, y, w, h);
  button_t* button = BUTTON(widget);
  return_value_if_fail(button != NULL, NULL);

  button->repeat = 0;
  button->repeat_nr = 0;
  button->pressed = FALSE;
  button->enable_long_press = FALSE;
  button->timer_id = TK_INVALID_ID;
  button->long_press_time = TK_LONG_PRESS_TIME;

  return widget;
}

widget_t* button_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, button), NULL);

  return widget;
}

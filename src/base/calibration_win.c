/**
 * File:   calibration_win.h
 * Author: AWTK Develop Team
 * Brief:  touch screen calibration win
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-10-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "base/calibration_win.h"

static point_t calibration_win_get_current_point(widget_t* widget, uint32_t point_name) {
  point_t pt = {0, 0};
  calibration_win_t* win = CALIBRATION_WIN(widget);

  switch (point_name) {
    case PT_TOP_LEFT: {
      pt.x = win->x_offset;
      pt.y = win->y_offset;
      break;
    };
    case PT_TOP_RIGHT: {
      pt.x = widget->w - win->x_offset;
      pt.y = win->y_offset;
      break;
    };
    case PT_BOTTOM_RIGHT: {
      pt.x = widget->w - win->x_offset;
      pt.y = widget->h - win->y_offset;
      break;
    };
    case PT_BOTTOM_LEFT: {
      pt.x = win->x_offset;
      pt.y = widget->h - win->y_offset;
      break;
    };
    case PT_CENTER: {
      pt.x = widget->w >> 1;
      pt.y = widget->h >> 1;
      break;
    };
  }

  return pt;
}

static ret_t calibration_win_invalidate(widget_t* widget, uint32_t point_name) {
  calibration_win_t* win = CALIBRATION_WIN(widget);
  if (point_name < PT_MAX_NR) {
    uint32_t cross_size = win->cross_size;
    point_t pt = calibration_win_get_current_point(widget, point_name);
    rect_t r = rect_init(pt.x - cross_size / 2, pt.y - cross_size / 2, cross_size, cross_size);

    widget->dirty = FALSE;
    widget_invalidate(widget, &r);
  }

  return RET_OK;
}

static ret_t calibration_win_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  calibration_win_t* win = CALIBRATION_WIN(widget);
  if (win->cursor == PT_MAX_NR) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_UP: {
      point_t* p = win->points + win->cursor;
      pointer_event_t* evt = (pointer_event_t*)e;

      p->x = evt->x;
      p->y = evt->y;

      if (win->on_click != NULL) {
        win->on_click(win->on_click_ctx, win->cursor, *p);
      }
      log_debug("click: %d {%d, %d}\n", win->cursor, p->x, p->y);

      calibration_win_invalidate(widget, win->cursor);
      win->cursor++;
      calibration_win_invalidate(widget, win->cursor);

      if (win->cursor == PT_MAX_NR) {
        p = win->points;
        if (win->on_done != NULL) {
          if (win->on_done(win->on_done_ctx, win->points) == RET_OK) {
            window_close(widget);
          } else {
            win->cursor = 0;
          }
        } else {
          window_close(widget);
        }
        log_debug("lt:{%d,%d} lr:{%d,%d} br:{%d,%d} bl:{%d,%d} center:{%d,%d}\n", p[PT_TOP_LEFT].x,
                  p[PT_TOP_LEFT].y, p[PT_TOP_RIGHT].x, p[PT_TOP_RIGHT].y, p[PT_BOTTOM_RIGHT].x,
                  p[PT_BOTTOM_RIGHT].y, p[PT_BOTTOM_LEFT].x, p[PT_BOTTOM_LEFT].y, p[PT_CENTER].x,
                  p[PT_CENTER].y);
      }
      break;
    }
    default:
      break;
  }

  return window_base_on_event(widget, e);
}

static ret_t calibration_win_on_paint_self(widget_t* widget, canvas_t* c) {
  point_t pt = {0, 0};
  wstr_t* text = &(widget->text);
  style_t* style = widget->astyle;
  color_t black = color_init(0, 0, 0, 0xff);
  calibration_win_t* win = CALIBRATION_WIN(widget);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, black);

  widget_prepare_text_style(widget, c);

  if (text->size > 0) {
    rect_t r = rect_init(0, 40, widget->w, widget->h);
    canvas_draw_text_in_rect(c, text->str, text->size, &r);
  }

  canvas_set_stroke_color(c, fg);

  pt = calibration_win_get_current_point(widget, win->cursor);
  canvas_draw_hline(c, pt.x - win->cross_size / 2, pt.y, win->cross_size);
  canvas_draw_vline(c, pt.x, pt.y - win->cross_size / 2, win->cross_size);

  return RET_OK;
}

static const widget_vtable_t s_calibration_win_vtable = {
    .size = sizeof(calibration_win_t),
    .type = WIDGET_TYPE_CALIBRATION_WIN,
    .create = calibration_win_create,
    .on_event = calibration_win_on_event,
    .set_prop = window_base_set_prop,
    .get_prop = window_base_get_prop,
    .on_paint_begin = window_base_on_paint_begin,
    .on_paint_end = window_base_on_paint_end,
    .on_paint_self = calibration_win_on_paint_self};

ret_t calibration_win_set_on_done(widget_t* widget, calibration_win_on_done_t on_done, void* ctx) {
  calibration_win_t* win = CALIBRATION_WIN(widget);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  win->on_done = on_done;
  win->on_done_ctx = ctx;

  return RET_OK;
}

ret_t calibration_win_set_on_click(widget_t* widget, calibration_win_on_click_t on_click,
                                   void* ctx) {
  calibration_win_t* win = CALIBRATION_WIN(widget);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  win->on_click = on_click;
  win->on_click_ctx = ctx;

  return RET_OK;
}

widget_t* calibration_win_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  calibration_win_t* win = TKMEM_ZALLOC(calibration_win_t);
  widget_t* widget = WIDGET(win);
  return_value_if_fail(win != NULL, NULL);

  win->cursor = 0;
  win->x_offset = 20;
  win->y_offset = 20;
  win->cross_size = 16;

  window_base_init(widget, parent, &s_calibration_win_vtable, x, y, w, h);
  widget_resize(widget, widget->parent->w, widget->parent->h);

  return widget;
}

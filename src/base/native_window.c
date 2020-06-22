/**
 * File:   native_window.h
 * Author: AWTK Develop Team
 * Brief:  native window
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"
#include "base/native_window.h"

ret_t native_window_move(native_window_t* win, xy_t x, xy_t y, bool_t force) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->move != NULL) {
    if (force || !(win->shared)) {
      return win->vt->move(win, x, y);
    }
  }

  return RET_OK;
}

ret_t native_window_resize(native_window_t* win, wh_t w, wh_t h, bool_t force) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->resize != NULL) {
    if (force || !(win->shared)) {
      return win->vt->resize(win, w, h);
    }
  }

  return RET_OK;
}

canvas_t* native_window_get_canvas(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, NULL);

  if (win->vt->get_canvas != NULL) {
    return win->vt->get_canvas(win);
  }

  return NULL;
}

ret_t native_window_update_last_dirty_rect(native_window_t* win) {
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  win->last_dirty_rect = win->dirty_rect;

  return RET_OK;
}

rect_t native_window_calc_dirty_rect(native_window_t* win) {
  rect_t* ldr = NULL;
  rect_t r = rect_init(0, 0, 0, 0);
  return_value_if_fail(win != NULL, r);

  r = win->dirty_rect;
  ldr = &(win->last_dirty_rect);

  rect_merge(&r, ldr);

  return rect_fix(&r, win->rect.w, win->rect.h);
}

ret_t native_window_invalidate(native_window_t* win, rect_t* r) {
  rect_t* dr = NULL;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  dr = &(win->dirty_rect);

  if (r != NULL) {
    rect_merge(dr, r);
  } else {
    dr->x = 0;
    dr->y = 0;
    dr->w = win->rect.w;
    dr->h = win->rect.h;
  }

  return RET_OK;
}

ret_t native_window_on_resized(native_window_t* win, wh_t w, wh_t h) {
  lcd_t* lcd = NULL;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  lcd = native_window_get_canvas(win)->lcd;
  return_value_if_fail(lcd != NULL, RET_BAD_PARAMS);

  lcd_resize(lcd, w, h, 0);

  return RET_OK;
}

ret_t native_window_gl_make_current(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(win->vt->gl_make_current != NULL, RET_BAD_PARAMS);

  return win->vt->gl_make_current(win);
}

ret_t native_window_swap_buffer(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(win->vt->swap_buffer != NULL, RET_BAD_PARAMS);

  return win->vt->swap_buffer(win);
}

ret_t native_window_get_info(native_window_t* win, native_window_info_t* info) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(win->vt->get_info != NULL && info != NULL, RET_BAD_PARAMS);

  return win->vt->get_info(win, info);
}

ret_t native_window_begin_frame(native_window_t* win, lcd_draw_mode_t mode) {
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  if (win->dirty_rect.w > 0 && win->dirty_rect.h > 0) {
    rect_t r = native_window_calc_dirty_rect(win);
    if (r.w > 0 && r.h > 0) {
      canvas_t* c = native_window_get_canvas(win);
      canvas_begin_frame(c, &r, mode);
      win->dirty = TRUE;

      return RET_OK;
    }
  }

  return RET_FAIL;
}

ret_t native_window_paint(native_window_t* win, widget_t* widget) {
  return_value_if_fail(win != NULL && widget != NULL, RET_BAD_PARAMS);

  if (win->dirty && widget->visible) {
    canvas_t* c = native_window_get_canvas(win);
    widget_paint(widget, c);
  }

  return RET_OK;
}

ret_t native_window_end_frame(native_window_t* win) {
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  if (win->dirty) {
    canvas_t* c = native_window_get_canvas(win);
    canvas_end_frame(c);
    native_window_update_last_dirty_rect(win);
  }
  native_window_clear_dirty_rect(win);

  return RET_OK;
}

ret_t native_window_clear_dirty_rect(native_window_t* win) {
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  win->dirty = FALSE;
  win->dirty_rect = rect_init(win->rect.w, win->rect.h, 0, 0);

  return RET_OK;
}

ret_t native_window_preprocess_event(native_window_t* win, event_t* e) {
  return_value_if_fail(win != NULL && win->vt != NULL && e != NULL, RET_BAD_PARAMS);

  if (win->vt->preprocess_event != NULL) {
    win->vt->preprocess_event(win, e);
  }

  return RET_OK;
}

ret_t native_window_minimize(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->minimize != NULL) {
    return win->vt->minimize(win);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_maximize(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->maximize != NULL) {
    return win->vt->maximize(win);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_restore(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->restore != NULL) {
    return win->vt->restore(win);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_center(native_window_t* win) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->center != NULL) {
    return win->vt->center(win);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_show_border(native_window_t* win, bool_t show) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->show_border != NULL) {
    return win->vt->show_border(win, show);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_set_fullscreen(native_window_t* win, bool_t fullscreen) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->set_fullscreen != NULL) {
    return win->vt->set_fullscreen(win, fullscreen);
  }

  return RET_NOT_IMPL;
}

ret_t native_window_set_cursor(native_window_t* win, const char* name, bitmap_t* img) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->set_cursor != NULL) {
    return win->vt->set_cursor(win, name, img);
  }

  return RET_NOT_IMPL;
}

/**
 * File:   native_window.h
 * Author: AWTK Develop Team
 * Brief:  native window
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

ret_t native_window_set_orientation(native_window_t* win, lcd_orientation_t old_orientation,
                                    lcd_orientation_t new_orientation) {
  return_value_if_fail(win != NULL && win->vt != NULL, RET_BAD_PARAMS);

  if (win->vt->set_orientation != NULL) {
    return win->vt->set_orientation(win, old_orientation, new_orientation);
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

  return RET_OK;
}

rect_t native_window_calc_dirty_rect(native_window_t* win) {
  rect_t r = rect_init(0, 0, 0, 0);
  return_value_if_fail(win != NULL, r);

  r = win->dirty_rects.max;

  return rect_fix(&r, win->rect.w, win->rect.h);
}

ret_t native_window_invalidate(native_window_t* win, const rect_t* r) {
  rect_t arect;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);
  arect = rect_fix((rect_t*)r, win->rect.w, win->rect.h);

  return dirty_rects_add(&(win->dirty_rects), &arect);
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
  canvas_t* c = NULL;
  const dirty_rects_t* dr = NULL;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  dr = &(win->dirty_rects);
  if (dr->nr == 0) {
    return RET_FAIL;
  }

  c = native_window_get_canvas(win);
  canvas_begin_frame(c, dr, mode);

  return RET_OK;
}

ret_t native_window_end_frame(native_window_t* win) {
  canvas_t* c = NULL;
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  c = native_window_get_canvas(win);
  canvas_end_frame(c);
  native_window_update_last_dirty_rect(win);
  native_window_clear_dirty_rect(win);

  return RET_OK;
}

ret_t native_window_clear_dirty_rect(native_window_t* win) {
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  dirty_rects_reset(&(win->dirty_rects));

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

ret_t native_window_set_title(native_window_t* win, const char* app_name) {
  return tk_object_set_prop_str(TK_OBJECT(win), NATIVE_WINDOW_PROP_TITLE, app_name);
}

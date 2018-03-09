/**
 * File:   dialog.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/dialog.h"
#include "base/group_box.h"
#include "base/image_manager.h"
#include "base/main_loop.h"
#include "base/mem.h"
#include "base/window_manager.h"

enum { TITLE_H = 32 };

static ret_t dialog_on_paint_self(widget_t* widget, canvas_t* c) {
  xy_t x = 0;
  xy_t y = 0;
  style_t* style = &(widget->style);
  dialog_t* dialog = DIALOG(widget);
  color_t color = color_init(0x80, 0x80, 0x80, 0xff);

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_FG_COLOR, color));
  canvas_fill_rect(c, 0, 0, widget->w, TITLE_H);

  canvas_set_fill_color(c, style_get_color(style, STYLE_ID_BG_COLOR, color));
  canvas_fill_rect(c, 0, TITLE_H, widget->w, widget->h - TITLE_H);

  canvas_set_stroke_color(c, style_get_color(style, STYLE_ID_BORDER_COLOR, color));
  canvas_stroke_rect(c, 0, 0, widget->w, widget->h);

  if (dialog->icon.data != NULL) {
    rect_t src;
    rect_t dst;

    y = (TITLE_H - dialog->icon.h) >> 1;
    x = y;
    rect_init(src, 0, 0, dialog->icon.w, dialog->icon.h);
    rect_init(dst, x, y, dialog->icon.w, dialog->icon.h);
    canvas_draw_image(c, &(dialog->icon), &src, &dst);
  }

  x += TITLE_H;
  y = (TITLE_H >> 1);
  if (dialog->title.size > 0) {
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
    uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, 20);

    canvas_set_font(c, font_name, font_size);
    canvas_set_fill_color(c, style_get_color(style, STYLE_ID_FG_COLOR, color));
    canvas_draw_text(c, dialog->title.str, dialog->title.size, x, y);
  }

  return RET_OK;
}

static ret_t dialog_get_prop(widget_t* widget, const char* name, value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    value_set_wstr(v, dialog->title.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dialog_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (strcmp(name, "text") == 0) {
    return wstr_set(&(dialog->title), value_wstr(v));
  } else if (strcmp(name, "icon") == 0) {
    return dialog_set_icon(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_dialog_vtable = {.on_paint_self = dialog_on_paint_self,
                                                .get_prop = dialog_get_prop,
                                                .set_prop = dialog_set_prop};

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  dialog_t* dialog = MEM_ZALLOC(dialog_t);
  return_value_if_fail(dialog != NULL, NULL);

  widget = WIDGETP(dialog);
  widget_init(widget, NULL, WIDGET_DIALOG);
  widget->vt = &s_dialog_vtable;

  if (parent == NULL) {
    parent = default_wm();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  wstr_init(&(dialog->title), 0);
  widget_set_state(widget, WIDGET_STATE_NORMAL);
  dialog->client = group_box_create(widget, 0, TITLE_H, widget->w, widget->h - TITLE_H);

  log_debug("%s\n", __func__);

  return widget;
}

ret_t dialog_set_icon(widget_t* widget, const char* name) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(image_manager_load(default_im(), name, &(dialog->icon)) == RET_OK,
                       RET_BAD_PARAMS);

  return RET_OK;
}

ret_t dialog_set_title(widget_t* widget, const wchar_t* title) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL && title != NULL, RET_BAD_PARAMS);

  return wstr_set(&(dialog->title), title);
}

uint32_t dialog_modal(widget_t* widget) {
  dialog_t* dialog = DIALOG(widget);
  bool_t running = main_loop_get_default()->running;
  return_value_if_fail(dialog != NULL, RET_BAD_PARAMS);

  log_debug("%s run\n", __func__);

  widget_invalidate(widget, NULL);
  main_loop_run(main_loop_get_default());
  main_loop_get_default()->running = running;

  log_debug("%s quit\n", __func__);

  return dialog->quit_code;
}

ret_t dialog_quit(widget_t* widget, uint32_t code) {
  dialog_t* dialog = DIALOG(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  dialog->quit_code = code;
  main_loop_quit(main_loop_get_default());

  return RET_OK;
}

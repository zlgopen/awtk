/**
 * File:   widget.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic class of all widget
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

#ifndef LFTK_WIDGET_H
#define LFTK_WIDGET_H

#include "base/value.h"
#include "base/rect.h"
#include "base/events.h"
#include "base/emitter.h"
#include "base/canvas.h"
#include "base/theme.h"

BEGIN_C_DECLS

struct _widget_t;
typedef struct _widget_t widget_t;

typedef enum _widget_state_t {
  WIDGET_STATE_NONE,
  WIDGET_STATE_NORMAL,
  WIDGET_STATE_PRESSED,
  WIDGET_STATE_OVER,
  WIDGET_STATE_DISABLE,
  WIDGET_STATE_FOCUS,
  WIDGET_STATE_SELECTED,
  WIDGET_STATE_CHECKED,
  WIDGET_STATE_UNCHECKED
}widget_state_t;

typedef ret_t (*widget_invalidate_t)(widget_t* widget, rect_t* r);
typedef ret_t (*widget_on_event_t)(widget_t* widget, event_t* e);
typedef ret_t (*widget_on_paint_self_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_paint_children_t)(widget_t* widget, canvas_t* c);
typedef ret_t (*widget_on_keydown_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_keyup_t)(widget_t* widget, key_event_t* e);
typedef ret_t (*widget_on_click_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_down_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_move_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_on_pointer_up_t)(widget_t* widget, pointer_event_t* e);
typedef ret_t (*widget_get_prop_t)(widget_t* widget, const char* name, value_t* v);
typedef ret_t (*widget_set_prop_t)(widget_t* widget, const char* name, const value_t* v);
typedef ret_t (*widget_grab_t)(widget_t* widget, widget_t* child);
typedef ret_t (*widget_ungrab_t)(widget_t* widget, widget_t* child);
typedef widget_t* (*widget_find_target_t)(widget_t* widget, xy_t x, xy_t y);
typedef ret_t (*widget_destroy_t)(widget_t* widget);

typedef struct _widget_vtable_t {
  widget_get_prop_t get_prop;
  widget_set_prop_t set_prop;
  widget_on_click_t on_click;
  widget_on_keyup_t on_keyup;
  widget_on_keydown_t on_keydown;
  widget_on_paint_self_t on_paint_self;
  widget_on_paint_children_t on_paint_children;
  widget_on_pointer_down_t on_pointer_down;
  widget_on_pointer_move_t on_pointer_move;
  widget_on_pointer_up_t on_pointer_up;
  widget_invalidate_t invalidate;
  widget_on_event_t on_event;
  widget_grab_t grab;
  widget_ungrab_t ungrab;
  widget_find_target_t find_target;
  widget_destroy_t  destroy;
}widget_vtable_t;

enum {
  WIDGET_NONE = 0,
  WIDGET_WINDOW_MANAGER,
  WIDGET_NORMAL_WINDOW,
  WIDGET_TOOLBAR,
  WIDGET_DIALOG,
  WIDGET_POPUP,
  WIDGET_LABEL,
  WIDGET_BUTTON,
  WIDGET_IMAGE,
  WIDGET_EDIT,
  WIDGET_PROGRESS_BAR,
  WIDGET_GROUP_BOX,
  WIDGET_CHECK_BUTTON,
  WIDGET_RADIO_BUTTON,
};

struct _widget_t {
  xy_t x;
  xy_t y;
  wh_t w;
  wh_t h;
  uint8_t type;
  uint8_t subtype:4; /*for style*/
  uint8_t state:4;
  uint8_t enable:1;
  uint8_t visible:1;
  uint8_t selected:1;
  uint8_t initializing:1;
  char name[NAME_LEN+1];

  style_t style;
  widget_t* parent;
  widget_t* target;
  array_t* children;
  emitter_t* emitter;
  const widget_vtable_t* vt;
};

widget_t* widget_init(widget_t* widget, widget_t* parent, uint8_t type);

ret_t widget_update_style(widget_t* widget);

uint32_t widget_count_children(widget_t* widget);
widget_t* widget_get_child(widget_t* widget, uint32_t index);

ret_t widget_move(widget_t* widget, xy_t x, xy_t y);
ret_t widget_resize(widget_t* widget, wh_t w, wh_t h);
ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t widget_set_value(widget_t* widget, uint32_t value);
ret_t widget_set_text(widget_t* widget, const wchar_t* text);

uint32_t widget_get_value(widget_t* widget);
const wchar_t* widget_get_text(widget_t* widget);

ret_t widget_to_local(widget_t* widget, point_t* p);
ret_t widget_to_global(widget_t* widget, point_t* p);

ret_t widget_set_name(widget_t* widget, const char* name);
ret_t widget_set_enable(widget_t* widget, bool_t enable);
ret_t widget_set_selected(widget_t* widget, bool_t selected);
ret_t widget_set_state(widget_t* widget, widget_state_t state);

ret_t widget_remove_children(widget_t* widget);
ret_t widget_add_child(widget_t* widget, widget_t* child);
ret_t widget_remove_child(widget_t* widget, widget_t* child);

widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y);
widget_t* widget_lookup(widget_t* widget, const char* name);
widget_t* widget_lookup_child(widget_t* widget, const char* name);

ret_t widget_set_visible(widget_t* widget, bool_t visible);
ret_t widget_set_visible_recursive(widget_t* widget, bool_t visible);

ret_t widget_dispatch(widget_t* widget, event_t* e);
ret_t widget_on(widget_t* widget, event_type_t type, event_handler on_event, void* ctx); 
ret_t widget_off(widget_t* widget, event_type_t type, event_handler on_event, void* ctx); 

ret_t widget_paint(widget_t* widget, canvas_t* c);
ret_t widget_invalidate(widget_t* widget, rect_t* r);

/*virtual functions wrapper*/
ret_t widget_on_paint_self(widget_t* widget, canvas_t* c);
ret_t widget_on_paint_children(widget_t* widget, canvas_t* c);
ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v);
ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v);
ret_t widget_on_paint(widget_t* widget, canvas_t* c);
ret_t widget_on_keydown(widget_t* widget, key_event_t* e);
ret_t widget_on_keyup(widget_t* widget, key_event_t* e);
ret_t widget_on_click(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e);
ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e);
ret_t widget_grab(widget_t* widget, widget_t* child);
ret_t widget_ungrab(widget_t* widget, widget_t* child);
ret_t widget_destroy(widget_t* widget);

#define WIDGET(w) (&(w.widget))
#define WIDGETP(w) (widget_t*)(w)

typedef widget_t* (*widget_create_t)(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

END_C_DECLS

#endif/*LFTK_WIDGET_H*/


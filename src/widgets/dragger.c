/**
 * File:   dragger.h
 * Author: AWTK Develop Team
 * Brief:  dragger
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/dragger.h"

static ret_t dragger_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t dragger_move(widget_t* widget, xy_t dx, xy_t dy) {
  xy_t x = 0;
  xy_t y = 0;
  dragger_t* dragger = DRAGGER(widget);
  return_value_if_fail(dragger != NULL, RET_BAD_PARAMS);

  x = dragger->save_x + dx;
  y = dragger->save_y + dy;

  x = tk_max(x, dragger->x_min);
  y = tk_max(y, dragger->y_min);
  x = tk_min(x, dragger->x_max);
  y = tk_min(y, dragger->y_max);

  if (x != widget->x || y != widget->y) {
    event_t evt = event_init(EVT_DRAG, widget);
    widget_move(widget, x, y);
    widget_dispatch(widget, (event_t*)&evt);
  }

  return RET_OK;
}

static ret_t dragger_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  dragger_t* dragger = DRAGGER(widget);
  return_value_if_fail(dragger != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* pointer_event = (pointer_event_t*)e;
      event_t evt = event_init(EVT_DRAG_START, widget);
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      widget_dispatch(widget, (event_t*)&evt);
      widget_grab(widget->parent, widget);

      dragger->down_x = pointer_event->x;
      dragger->down_y = pointer_event->y;
      dragger->save_x = widget->x;
      dragger->save_y = widget->y;
      dragger_move(widget, 0, 0);
      dragger->dragging = TRUE;
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      event_t evt = event_init(EVT_DRAG_END, widget);
      dragger_move(widget, 0, 0);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      widget_dispatch(widget, (event_t*)&evt);
      widget_ungrab(widget->parent, widget);
      dragger->dragging = FALSE;
      dragger->moving = FALSE;
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* pointer_event = (pointer_event_t*)e;
      event_t evt = event_init(EVT_DRAG_END, widget);
      if (dragger->moving) {
        dragger_move(widget, pointer_event->x - dragger->down_x,
                     pointer_event->y - dragger->down_y);
      }
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      widget_dispatch(widget, (event_t*)&evt);
      widget_ungrab(widget->parent, widget);
      dragger->dragging = FALSE;
      dragger->moving = FALSE;
      break;
    }
    case EVT_POINTER_MOVE: {
      if (dragger->dragging) {
        pointer_event_t* pointer_event = (pointer_event_t*)e;
        dragger_move(widget, pointer_event->x - dragger->down_x,
                     pointer_event->y - dragger->down_y);
        dragger->moving = TRUE;
      }
      return RET_STOP;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, WIDGET_STATE_OVER);
      break;
    default:
      break;
  }

  return RET_OK;
}

ret_t dragger_set_range(widget_t* widget, xy_t x_min, xy_t y_min, xy_t x_max, xy_t y_max) {
  dragger_t* dragger = DRAGGER(widget);
  return_value_if_fail(dragger != NULL && x_min <= x_max && y_min <= y_max, RET_BAD_PARAMS);

  dragger->x_min = x_min;
  dragger->x_max = x_max;
  dragger->y_min = y_min;
  dragger->y_max = y_max;

  return RET_OK;
}

static ret_t dragger_get_prop(widget_t* widget, const char* name, value_t* v) {
  dragger_t* dragger = DRAGGER(widget);
  return_value_if_fail(dragger != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_X_MIN)) {
    value_set_int(v, dragger->x_min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_X_MAX)) {
    value_set_int(v, dragger->x_max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_Y_MIN)) {
    value_set_int(v, dragger->y_min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_Y_MAX)) {
    value_set_int(v, dragger->y_max);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t dragger_set_prop(widget_t* widget, const char* name, const value_t* v) {
  dragger_t* dragger = DRAGGER(widget);
  return_value_if_fail(dragger != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_X_MIN)) {
    dragger->x_min = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_X_MAX)) {
    dragger->x_max = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_Y_MIN)) {
    dragger->y_min = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_Y_MAX)) {
    dragger->y_max = value_int(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* const s_dragger_clone_properties[] = {
    WIDGET_PROP_X_MIN, WIDGET_PROP_X_MAX, WIDGET_PROP_Y_MIN, WIDGET_PROP_Y_MAX, NULL};
TK_DECL_VTABLE(dragger) = {.size = sizeof(dragger_t),
                           .type = WIDGET_TYPE_DRAGGER,
                           .clone_properties = s_dragger_clone_properties,
                           .parent = TK_PARENT_VTABLE(widget),
                           .create = dragger_create,
                           .set_prop = dragger_set_prop,
                           .get_prop = dragger_get_prop,
                           .on_event = dragger_on_event,
                           .on_paint_self = dragger_on_paint_self};

widget_t* dragger_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(dragger), x, y, w, h);
}

widget_t* dragger_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dragger), NULL);

  return widget;
}

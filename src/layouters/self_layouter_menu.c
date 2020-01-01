/**
 * File:   self_layouter_menu_menu.c
 * Author: AWTK Develop Team
 * Brief:  self layouter menu
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
 * 2019-06-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/widget.h"
#include "awtk_global.h"
#include "tkc/tokenizer.h"
#include "base/window_manager.h"
#include "tkc/func_call_parser.h"
#include "layouters/self_layouter_menu.h"

typedef enum _layout_position_t {
  POSITION_POINT = 'p',
  POSITION_LEFT = 'l',
  POSITION_RIGHT = 'r',
  POSITION_UP = 'u',
  POSITION_DOWN = 'd',
} layout_position_t;

ret_t self_layouter_menu_get_param(self_layouter_t* layouter, const char* name, value_t* v) {
  self_layouter_menu_t* l = (self_layouter_menu_t*)layouter;

  switch (*name) {
    case 'x': {
      value_set_int(v, l->x_attr);
      return RET_OK;
    }
    case 'y': {
      value_set_int(v, l->y_attr);
      return RET_OK;
    }
    case 'p': {
      value_set_int(v, l->position);
      return RET_OK;
    }
    case 'w': {
      if (name[1]) {
        value_set_int(v, l->w_attr);
      } else {
        value_set_int(v, l->w);
      }

      return RET_OK;
    }
    case 'h': {
      if (name[1]) {
        value_set_int(v, l->h_attr);
      } else {
        value_set_int(v, l->h);
      }

      return RET_OK;
    }
    default: {
      assert(!"not support param");
      return RET_FAIL;
    }
  }
}

ret_t self_layouter_menu_set_param(self_layouter_t* layouter, const char* name, const value_t* v) {
  const char* value = value_str(v);
  self_layouter_menu_t* layout = (self_layouter_menu_t*)layouter;

  switch (*name) {
    case 'x': {
      const char* x = value;
      if (x[0] == 'c') {
        layout->x_attr = X_ATTR_CENTER;
      } else if (x[0] == 'r') {
        layout->x_attr = X_ATTR_RIGHT;
      } else {
        if (isdigit(*x)) {
          layout->x_attr = tk_atoi(x);
        } else {
          layout->x_attr = X_ATTR_DEFAULT;
        }
      }
      break;
    }
    case 'y': {
      const char* y = value;
      if (y[0] == 'm') {
        layout->y_attr = Y_ATTR_MIDDLE;
      } else if (y[0] == 'b') {
        layout->y_attr = Y_ATTR_BOTTOM;
      } else {
        if (isdigit(*y)) {
          layout->y_attr = tk_atoi(y);
        } else {
          layout->y_attr = X_ATTR_DEFAULT;
        }
      }
      break;
    }
    case 'p': {
      const char* p = value;
      if (p[0] == 'l') {
        layout->position = POSITION_LEFT;
      } else if (p[0] == 'r') {
        layout->position = POSITION_RIGHT;
      } else if (p[0] == 'u') {
        layout->position = POSITION_UP;
      } else if (p[0] == 'd') {
        layout->position = POSITION_DOWN;
      } else {
        layout->position = POSITION_POINT;
      }
      break;
    }
    case 'w': {
      const char* w = value;
      layout->w = tk_atoi(w);
      layout->w_attr = W_ATTR_PIXEL;
      if (w != NULL) {
        if (strchr(w, '%') != NULL) {
          layout->w_attr = W_ATTR_PERCENT;
        }
      }
      break;
    }
    case 'h': {
      const char* h = value;
      layout->h = tk_atoi(h);
      layout->h_attr = H_ATTR_PIXEL;
      if (h != NULL) {
        if (strchr(h, '%') != NULL) {
          layout->h_attr = H_ATTR_PERCENT;
        }
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t widget_layout_calc(self_layouter_menu_t* layout, rect_t* r, wh_t parent_w,
                                wh_t parent_h) {
  xy_t x = r->x;
  xy_t y = r->y;
  uint8_t w_attr = layout->w_attr;
  uint8_t h_attr = layout->h_attr;
  wh_t w = w_attr == W_ATTR_UNDEF ? r->w : layout->w;
  wh_t h = h_attr == H_ATTR_UNDEF ? r->h : layout->h;

  if (parent_w > 0 && parent_h > 0) {
    if (w_attr == W_ATTR_PERCENT) {
      w = (parent_w * w) / 100;
    }

    if (w < 0) {
      w = parent_w + w;
    }

    if (h_attr == H_ATTR_PERCENT) {
      h = (parent_h * h) / 100;
    }

    if (h < 0) {
      h = parent_h + h;
    }

    r->w = w;
    r->h = h;

    switch (layout->position) {
      case POSITION_POINT: {
        int32_t px = layout->pressed.x;
        int32_t py = layout->pressed.y;

        if ((px + r->w) <= parent_w) {
          x = px;
        } else if (px >= r->w) {
          x = px - r->w;
        } else {
          x = parent_w - r->w;
        }

        if ((py + r->h) <= parent_h) {
          y = py;
        } else if (py >= r->h) {
          y = py - r->h;
        } else {
          y = parent_h - r->h;
        }
        break;
      }
      case POSITION_LEFT:
      case POSITION_RIGHT: {
        int32_t px = layout->trigger_widget_rect.x;
        int32_t py = layout->trigger_widget_rect.y;
        int32_t pw = layout->trigger_widget_rect.w;
        int32_t ph = layout->trigger_widget_rect.h;

        if (layout->position == POSITION_LEFT) {
          x = px - r->w;
          x -= layout->x_attr;
        } else {
          x = px + pw;
          x += layout->x_attr;
        }

        switch (layout->y_attr) {
          case Y_ATTR_MIDDLE: {
            y = py + (ph - r->h) / 2;
            if ((y + r->h) > parent_h) {
              y = parent_h - r->h;
            } else if (y < 0) {
              y = 0;
            }
            break;
          }
          case Y_ATTR_BOTTOM: {
            y = py + ph - r->h;
            break;
          }
          default: {
            y = py;
            break;
          }
        }
        break;
      }
      case POSITION_UP:
      case POSITION_DOWN: {
        int32_t px = layout->trigger_widget_rect.x;
        int32_t py = layout->trigger_widget_rect.y;
        int32_t pw = layout->trigger_widget_rect.w;
        int32_t ph = layout->trigger_widget_rect.h;

        if (layout->position == POSITION_DOWN) {
          y = py + ph;
          y -= layout->y_attr;
        } else {
          y = py - r->h;
          y += layout->y_attr;
        }

        switch (layout->x_attr) {
          case X_ATTR_CENTER: {
            x = px + (pw - r->w) / 2;
            if ((x + r->w) > parent_w) {
              x = parent_w - r->w;
            } else if (x < 0) {
              x = 0;
            }
            break;
          }
          case X_ATTR_RIGHT: {
            x = px + pw - r->w;
            break;
          }
          default: {
            x = px;
            break;
          }
        }

        break;
      }
      default:
        break;
    }
  }

  r->x = x;
  r->y = y;

  return RET_OK;
}

ret_t widget_layout_self_menu_with_rect(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
  self_layouter_menu_t* l = (self_layouter_menu_t*)layouter;

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (self_layouter_menu_is_valid(layouter)) {
    widget_layout_calc(l, &r, area->w, area->h);
    widget_move_resize(widget, r.x + area->x, r.y + area->y, r.w, r.h);

    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t widget_layout_self_get_trigger(self_layouter_t* layouter, point_t* pressed,
                                            rect_t* widget_rect) {
  point_t p = {0, 0};
  widget_t* iter = NULL;
  pressed->x = tk_get_pointer_x();
  pressed->y = tk_get_pointer_y();

  iter = window_manager_get_prev_window(window_manager());
  return_value_if_fail(iter != NULL, RET_BAD_PARAMS);

  while (iter->key_target != NULL) {
    iter = iter->key_target;
  }

  return_value_if_fail(iter != NULL, RET_BAD_PARAMS);

  widget_to_screen(iter, &p);

  widget_rect->x = p.x;
  widget_rect->y = p.y;
  widget_rect->w = iter->w;
  widget_rect->h = iter->h;

  return RET_OK;
}

ret_t self_layouter_menu_layout(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  self_layouter_menu_t* l = (self_layouter_menu_t*)layouter;
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(widget), RET_BAD_PARAMS);

  widget_layout_self_get_trigger(layouter, &(l->pressed), &(l->trigger_widget_rect));

  return widget_layout_self_menu_with_rect(layouter, widget, area);
}

static ret_t self_layouter_menu_destroy(self_layouter_t* layouter) {
  self_layouter_menu_t* l = (self_layouter_menu_t*)layouter;
  str_reset(&(layouter->params));
  TKMEM_FREE(l);

  return RET_OK;
}

static self_layouter_t* self_layouter_menu_clone(self_layouter_t* layouter) {
  self_layouter_menu_t* l = TKMEM_ZALLOC(self_layouter_menu_t);

  memcpy(l, layouter, sizeof(*l));
  str_init(&(l->layouter.params), 0);
  str_set(&(l->layouter.params), layouter->params.str);

  return (self_layouter_t*)l;
}

static const self_layouter_vtable_t s_self_layouter_menu_vtable = {
    .type = "menu",
    .clone = self_layouter_menu_clone,
    .get_param = self_layouter_menu_get_param,
    .set_param = self_layouter_menu_set_param,
    .layout = self_layouter_menu_layout,
    .destroy = self_layouter_menu_destroy};

bool_t self_layouter_menu_is_valid(self_layouter_t* layouter) {
  return layouter && layouter->vt == &s_self_layouter_menu_vtable;
}

self_layouter_t* self_layouter_menu_create(void) {
  self_layouter_t* l = NULL;
  self_layouter_menu_t* layouter = NULL;

  layouter = TKMEM_ZALLOC(self_layouter_menu_t);
  return_value_if_fail(layouter != NULL, NULL);

  l = (self_layouter_t*)layouter;

  layouter->x_attr = 0;
  layouter->y_attr = 0;
  layouter->w_attr = W_ATTR_UNDEF;
  layouter->h_attr = H_ATTR_UNDEF;
  layouter->position = POSITION_POINT;

  str_init(&(l->params), 0);
  l->vt = &s_self_layouter_menu_vtable;

  return (self_layouter_t*)layouter;
}

ret_t widget_layout_self_set_trigger(self_layouter_t* layouter, point_t pressed,
                                     rect_t widget_rect) {
  self_layouter_menu_t* l = (self_layouter_menu_t*)layouter;
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);

  l->pressed = pressed;
  l->trigger_widget_rect = widget_rect;

  return RET_OK;
}

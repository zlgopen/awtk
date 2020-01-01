/**
 * File:   self_layouter_default_default.c
 * Author: AWTK Develop Team
 * Brief:  self layouter default
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/widget.h"
#include "tkc/tokenizer.h"
#include "tkc/func_call_parser.h"
#include "layouters/self_layouter_default.h"

const char* self_layouter_default_to_string(self_layouter_t* layouter) {
  char value[32];
  str_t* str = &(layouter->params);
  self_layouter_default_t* layout = (self_layouter_default_t*)layouter;

  str_set(str, "default(");
  memset(value, 0x00, sizeof(value));

  switch (layout->x_attr) {
    case X_ATTR_UNDEF: {
      *value = '\0';
      break;
    }
    case X_ATTR_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "x=%d%%,", (int)layout->x);
      break;
    }
    case X_ATTR_CENTER: {
      tk_snprintf(value, sizeof(value) - 1, "x=c:%d,", (int)layout->x);
      break;
    }
    case X_ATTR_CENTER_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "x=c:%d%%,", (int)layout->x);
      break;
    }
    case X_ATTR_RIGHT: {
      tk_snprintf(value, sizeof(value) - 1, "x=r:%d,", (int)layout->x);
      break;
    }
    case X_ATTR_RIGHT_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "x=r:%d%%,", (int)layout->x);
      break;
    }
    default: {
      tk_snprintf(value, sizeof(value) - 1, "x=%d,", (int)layout->x);
      break;
    }
  }
  str_append(str, value);

  switch (layout->y_attr) {
    case Y_ATTR_UNDEF: {
      *value = '\0';
      break;
    }
    case Y_ATTR_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "y=%d%%,", (int)layout->y);
      break;
    }
    case Y_ATTR_MIDDLE: {
      tk_snprintf(value, sizeof(value) - 1, "y=m:%d,", (int)layout->y);
      break;
    }
    case Y_ATTR_MIDDLE_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "y=m:%d%%,", (int)layout->y);
      break;
    }
    case Y_ATTR_BOTTOM: {
      tk_snprintf(value, sizeof(value) - 1, "y=b:%d,", (int)layout->y);
      break;
    }
    case Y_ATTR_BOTTOM_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "y=b:%d%%,", (int)layout->y);
      break;
    }
    default: {
      tk_snprintf(value, sizeof(value) - 1, "y=%d,", (int)layout->y);
      break;
    }
  }
  str_append(str, value);

  switch (layout->w_attr) {
    case W_ATTR_UNDEF: {
      *value = '\0';
      break;
    }
    case W_ATTR_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "w=%d%%,", (int)layout->w);
      break;
    }
    default: {
      tk_snprintf(value, sizeof(value) - 1, "w=%d,", (int)layout->w);
      break;
    }
  }
  str_append(str, value);

  switch (layout->h_attr) {
    case H_ATTR_UNDEF: {
      *value = '\0';
      break;
    }
    case H_ATTR_PERCENT: {
      tk_snprintf(value, sizeof(value) - 1, "h=%d%%", (int)layout->h);
      break;
    }
    default: {
      tk_snprintf(value, sizeof(value) - 1, "h=%d", (int)layout->h);
      break;
    }
  }
  str_append(str, value);

  str_append(str, ")");

  return str->str;
}

ret_t self_layouter_default_get_param(self_layouter_t* layouter, const char* name, value_t* v) {
  self_layouter_default_t* l = (self_layouter_default_t*)layouter;

  switch (*name) {
    case 'x': {
      if (name[1]) {
        value_set_int(v, l->x_attr);
      } else {
        value_set_int(v, l->x);
      }

      return RET_OK;
    }
    case 'y': {
      if (name[1]) {
        value_set_int(v, l->y_attr);
      } else {
        value_set_int(v, l->y);
      }

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

ret_t self_layouter_default_set_param(self_layouter_t* layouter, const char* name,
                                      const value_t* v) {
  const char* value = value_str(v);
  self_layouter_default_t* layout = (self_layouter_default_t*)layouter;

  switch (*name) {
    case 'x': {
      const char* x = value;
      if (x[0] == 'c') {
        const char* v = strchr(x, ':');
        if (v != NULL) {
          layout->x = tk_atoi(v + 1);
        }
        if (strchr(x, '%') != NULL) {
          layout->x_attr = X_ATTR_CENTER_PERCENT;
        } else {
          layout->x_attr = X_ATTR_CENTER;
        }
      } else if (x[0] == 'r') {
        const char* v = strchr(x, ':');
        if (v != NULL) {
          layout->x = tk_atoi(v + 1);
        }
        if (strchr(x, '%') != NULL) {
          layout->x_attr = X_ATTR_RIGHT_PERCENT;
        } else {
          layout->x_attr = X_ATTR_RIGHT;
        }
      } else if (strchr(x, '%') != NULL) {
        layout->x = tk_atoi(x);
        layout->x_attr = X_ATTR_PERCENT;
      } else {
        layout->x = tk_atoi(x);
        layout->x_attr = X_ATTR_DEFAULT;
      }
      break;
    }
    case 'y': {
      const char* y = value;
      if (y[0] == 'm') {
        const char* v = strchr(y, ':');
        if (v != NULL) {
          layout->y = tk_atoi(v + 1);
        }
        if (strchr(y, '%') != NULL) {
          layout->y_attr = Y_ATTR_MIDDLE_PERCENT;
        } else {
          layout->y_attr = Y_ATTR_MIDDLE;
        }
      } else if (y[0] == 'b') {
        const char* v = strchr(y, ':');
        if (v != NULL) {
          layout->y = tk_atoi(v + 1);
        }
        if (strchr(y, '%') != NULL) {
          layout->y_attr = Y_ATTR_BOTTOM_PERCENT;
        } else {
          layout->y_attr = Y_ATTR_BOTTOM;
        }
      } else if (strchr(y, '%') != NULL) {
        layout->y = tk_atoi(y);
        layout->y_attr = Y_ATTR_PERCENT;
      } else {
        layout->y = tk_atoi(y);
        layout->y_attr = Y_ATTR_DEFAULT;
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

static ret_t widget_layout_calc(self_layouter_default_t* layout, rect_t* r, wh_t parent_w,
                                wh_t parent_h) {
  uint8_t x_attr = layout->x_attr;
  uint8_t y_attr = layout->y_attr;
  uint8_t w_attr = layout->w_attr;
  uint8_t h_attr = layout->h_attr;
  xy_t x = x_attr == X_ATTR_UNDEF ? r->x : layout->x;
  xy_t y = y_attr == Y_ATTR_UNDEF ? r->y : layout->y;
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

    switch (x_attr) {
      case X_ATTR_CENTER:
        x = (parent_w - w) / 2 + x;
        break;
      case X_ATTR_CENTER_PERCENT:
        x = (parent_w - w) / 2 + (x * parent_w) / 100;
        break;
      case X_ATTR_RIGHT:
        x = (parent_w - w) - x;
        break;
      case X_ATTR_RIGHT_PERCENT:
        x = (parent_w - w) - (x * parent_w) / 100;
        break;
      case X_ATTR_PERCENT:
        x = parent_w * x / 100;
        break;
      default:
        break;
    }

    switch (y_attr) {
      case Y_ATTR_MIDDLE:
        y = (parent_h - h) / 2 + y;
        break;
      case Y_ATTR_MIDDLE_PERCENT:
        y = (parent_h - h) / 2 + (y * parent_h) / 100;
        break;
      case Y_ATTR_BOTTOM:
        y = (parent_h - h) - y;
        break;
      case Y_ATTR_BOTTOM_PERCENT:
        y = (parent_h - h) - (y * parent_h) / 100;
        break;
      case Y_ATTR_PERCENT:
        y = parent_h * y / 100;
        break;
      default:
        break;
    }
  }

  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;

  return RET_OK;
}

ret_t widget_layout_self_with_rect(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
  self_layouter_default_t* l = (self_layouter_default_t*)layouter;

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (self_layouter_default_is_valid(layouter)) {
    widget_layout_calc(l, &r, area->w, area->h);
    widget_move_resize(widget, r.x + area->x, r.y + area->y, r.w, r.h);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t self_layouter_default_layout(self_layouter_t* layouter, widget_t* widget, rect_t* area) {
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);

  return widget_layout_self_with_rect(layouter, widget, area);
}

static ret_t self_layouter_default_destroy(self_layouter_t* layouter) {
  self_layouter_default_t* l = (self_layouter_default_t*)layouter;
  str_reset(&(layouter->params));
  TKMEM_FREE(l);

  return RET_OK;
}

static self_layouter_t* self_layouter_default_clone(self_layouter_t* layouter) {
  self_layouter_default_t* l = TKMEM_ZALLOC(self_layouter_default_t);

  memcpy(l, layouter, sizeof(*l));
  str_init(&(l->layouter.params), 0);
  str_set(&(l->layouter.params), layouter->params.str);

  return (self_layouter_t*)l;
}

static const self_layouter_vtable_t s_self_layouter_default_vtable = {
    .type = "default",
    .clone = self_layouter_default_clone,
    .to_string = self_layouter_default_to_string,
    .get_param = self_layouter_default_get_param,
    .set_param = self_layouter_default_set_param,
    .layout = self_layouter_default_layout,
    .destroy = self_layouter_default_destroy};

bool_t self_layouter_default_is_valid(self_layouter_t* layouter) {
  return layouter && layouter->vt == &s_self_layouter_default_vtable;
}

self_layouter_t* self_layouter_default_create(void) {
  self_layouter_t* l = NULL;
  self_layouter_default_t* layouter = NULL;

  layouter = TKMEM_ZALLOC(self_layouter_default_t);
  return_value_if_fail(layouter != NULL, NULL);

  l = (self_layouter_t*)layouter;

  layouter->x_attr = X_ATTR_UNDEF;
  layouter->y_attr = Y_ATTR_UNDEF;
  layouter->w_attr = W_ATTR_UNDEF;
  layouter->h_attr = H_ATTR_UNDEF;

  str_init(&(l->params), 0);
  l->vt = &s_self_layouter_default_vtable;

  return (self_layouter_t*)layouter;
}

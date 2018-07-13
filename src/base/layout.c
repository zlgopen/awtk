/**
 * File:   layout.c
 * Author: AWTK Develop Team
 * Brief:  layout
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/layout.h"
#include "base/tokenizer.h"

widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y,
                                     const char* w, const char* h) {
  return_value_if_fail(layout != NULL, NULL);

  memset(layout, 0x00, sizeof(*layout));
  if (x != NULL) {
    if (x[0] == 'c') {
      if (x[6] == ':') {
        layout->x = tk_atoi(x + 7);
      }
      layout->x_attr = X_ATTR_CENTER;
    } else if (x[0] == 'r') {
      if (x[5] == ':') {
        layout->x = tk_atoi(x + 6);
      }
      layout->x_attr = X_ATTR_RIGHT;
    } else if (strstr(x, "%") != NULL) {
      layout->x = tk_atoi(x);
      layout->x_attr = X_ATTR_PERCENT;
    } else {
      layout->x = tk_atoi(x);
      layout->x_attr = X_ATTR_DEFAULT;
    }
  } else {
    layout->x_attr = X_ATTR_DEFAULT;
  }

  if (y != NULL) {
    if (y[0] == 'm') {
      if (y[6] == ':') {
        layout->y = tk_atoi(y + 7);
      }
      layout->y_attr = Y_ATTR_MIDDLE;
    } else if (y[0] == 'b') {
      if (y[6] == ':') {
        layout->y = tk_atoi(y + 7);
      }
      layout->y_attr = Y_ATTR_BOTTOM;
    } else if (strstr(y, "%") != NULL) {
      layout->y = tk_atoi(y);
      layout->y_attr = Y_ATTR_PERCENT;
    } else {
      layout->y = tk_atoi(y);
      layout->y_attr = Y_ATTR_DEFAULT;
    }
  } else {
    layout->y_attr = Y_ATTR_DEFAULT;
  }

  if (w != NULL) {
    layout->w = tk_atoi(w);
    layout->w_attr = W_ATTR_PIXEL;
    if (w != NULL) {
      if (strchr(w, '%') != NULL) {
        layout->w_attr = W_ATTR_PERCENT;
      } else if (strstr(w, "fill") != NULL) {
        layout->w_attr = W_ATTR_FILL;
      }
    }
  } else {
    layout->h_attr = H_ATTR_PIXEL;
  }

  if (h != NULL) {
    layout->h = tk_atoi(h);
    layout->h_attr = H_ATTR_PIXEL;
    if (h != NULL) {
      if (strchr(h, '%') != NULL) {
        layout->h_attr = H_ATTR_PERCENT;
      } else if (strstr(h, "fill") != NULL) {
        layout->h_attr = H_ATTR_FILL;
      }
    }
  } else {
    layout->h_attr = H_ATTR_PIXEL;
  }

  return layout;
}

ret_t widget_layout_calc(const widget_layout_t* layout, rect_t* r, wh_t parent_w, wh_t parent_h) {
  xy_t x = layout->x;
  xy_t y = layout->y;
  wh_t w = layout->w;
  wh_t h = layout->h;
  uint8_t x_attr = layout->x_attr;
  uint8_t y_attr = layout->y_attr;
  uint8_t w_attr = layout->w_attr;
  uint8_t h_attr = layout->h_attr;

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
      case X_ATTR_RIGHT:
        x = (parent_w - w) - x;
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
      case Y_ATTR_BOTTOM:
        y = (parent_h - h) - y;
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

ret_t widget_set_self_layout(widget_t* widget, const widget_layout_t* layout) {
  return_value_if_fail(widget != NULL && layout != NULL, RET_BAD_PARAMS);
  if (widget->layout_params == NULL) {
    widget->layout_params = TKMEM_ZALLOC(layout_params_t);
  }
  return_value_if_fail(widget->layout_params != NULL, RET_OOM);
  memcpy(&(widget->layout_params->self), layout, sizeof(*layout));
  widget->layout_params->self.inited = TRUE;

  return RET_OK;
}

ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w,
                                    const char* h) {
  widget_layout_t layout;
  return_value_if_fail(widget != NULL && x != NULL && y != NULL && w != NULL && h != NULL,
                       RET_BAD_PARAMS);
  widget_layout_parse(&layout, x, y, w, h);

  return widget_set_self_layout(widget, &layout);
}

ret_t widget_set_children_layout_params(widget_t* widget, const char* params) {
  children_layout_t cl;
  return_value_if_fail(widget != NULL && params != NULL, RET_BAD_PARAMS);

  return widget_set_children_layout(widget, children_layout_parser(&cl, params));
}

ret_t widget_set_children_layout(widget_t* widget, const children_layout_t* cl) {
  children_layout_t* layout = NULL;
  return_value_if_fail(widget != NULL && cl != NULL, RET_BAD_PARAMS);

  if (widget->layout_params == NULL) {
    widget->layout_params = TKMEM_ZALLOC(layout_params_t);
  }
  return_value_if_fail(widget->layout_params != NULL, RET_OOM);
  layout = &(widget->layout_params->children);
  *layout = *cl;
  layout->inited = TRUE;

  return RET_OK;
}

ret_t widget_layout(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  widget_layout_self(widget);

  return widget_layout_children(widget);
}

ret_t widget_layout_self_with_wh(widget_t* widget, wh_t w, wh_t h) {
  rect_t r;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL && widget->layout_params != NULL) {
    widget_layout_t* layout = &(widget->layout_params->self);
    if (layout->inited) {
      widget_layout_calc(layout, &r, w, h);
      widget_move_resize(widget, r.x, r.y, r.w, r.h);
    }
  }

  return RET_OK;
}

ret_t widget_layout_self(widget_t* widget) {
  value_t v;
  int32_t layout_w = 0;
  int32_t layout_h = 0;
  widget_t* parent = NULL;
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);

  parent = widget->parent;
  if (widget_get_prop(parent, WIDGET_PROP_LAYOUT_W, &v) == RET_OK) {
    layout_w = value_int(&v);
  } else {
    layout_w = parent->w;
  }

  if (widget_get_prop(parent, WIDGET_PROP_LAYOUT_H, &v) == RET_OK) {
    layout_h = value_int(&v);
  } else {
    layout_h = parent->h;
  }

  return widget_layout_self_with_wh(widget, layout_w, layout_h);
}

ret_t widget_layout_children_default(widget_t* widget) {
  value_t v;
  wh_t w = 0;
  wh_t h = 0;
  xy_t x = 0;
  xy_t y = 0;
  uint32_t i = 0;
  uint32_t n = 0;
  uint32_t rows = 0;
  uint32_t cols = 0;
  int32_t layout_w = 0;
  int32_t layout_h = 0;
  uint8_t x_margin = 0;
  uint8_t y_margin = 0;
  widget_t* iter = NULL;
  uint8_t spacing = 0;
  widget_t** children = NULL;

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children == NULL) {
    return RET_OK;
  }

  if (widget_get_prop(widget, WIDGET_PROP_LAYOUT_W, &v) == RET_OK) {
    layout_w = value_int(&v);
  } else {
    layout_w = widget->w;
  }

  if (widget_get_prop(widget, WIDGET_PROP_LAYOUT_H, &v) == RET_OK) {
    layout_h = value_int(&v);
  } else {
    layout_h = widget->h;
  }

  n = widget->children->size;
  children = (widget_t**)(widget->children->elms);

  if (widget->layout_params && widget->layout_params->children.inited) {
    children_layout_t* layout = &(widget->layout_params->children);

    x = layout->x_margin;
    y = layout->y_margin;
    x_margin = layout->x_margin;
    y_margin = layout->y_margin;
    spacing = layout->spacing;

    if (layout->rows_is_height) {
      rows = (layout_h - 2 * y_margin) / (layout->rows + spacing);
    } else {
      rows = layout->rows;
    }

    if (layout->cols_is_width) {
      cols = (layout_w - 2 * x_margin) / (layout->cols + spacing);
    } else {
      cols = layout->cols;
    }

    if (rows == 1 && cols == 0) { /*hbox*/
      h = layout_h - 2 * y_margin;
      w = layout_w - 2 * x_margin - (n - 1) * spacing;

      return_value_if_fail(h > 0 && w > 0, RET_BAD_PARAMS);

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_layout_self_with_wh(iter, w, h);
      }

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_move_resize(iter, x, y, iter->w, h);
        x += iter->w + spacing;
        return_value_if_fail(x <= layout_w, RET_BAD_PARAMS);
      }

      for (i = 0; i < n; i++) {
        widget_layout_children(children[i]);
      }

      return RET_OK;
    } else if (cols == 1 && rows == 0) { /*vbox*/
      w = layout_w - 2 * x_margin;
      h = layout_h - 2 * y_margin - (n - 1) * spacing;
      return_value_if_fail(w > 0 && h > 0, RET_BAD_PARAMS);

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_layout_self_with_wh(iter, w, h);
      }

      for (i = 0; i < n; i++) {
        iter = children[i];
        return_value_if_fail(y <= layout_h, RET_BAD_PARAMS);
        widget_move_resize(iter, x, y, w, iter->h);
        y += iter->h + spacing;
      }

      for (i = 0; i < n; i++) {
        widget_layout_children(children[i]);
      }

      return RET_OK;
    } else if (cols > 0 && rows > 0) { /*grid|vlist|hlist*/
      uint8_t r = 0;
      uint8_t c = 0;
      wh_t item_w = 0;
      wh_t item_h = 0;
      w = layout_w - 2 * x_margin - (cols - 1) * spacing;
      h = layout_h - 2 * y_margin - (rows - 1) * spacing;
      item_w = w / cols;
      item_h = h / rows;
      return_value_if_fail(item_w > 0 && item_h > 0, RET_BAD_PARAMS);

      w = (cols - 1) * spacing + cols * item_w;
      h = (rows - 1) * spacing + rows * item_h;

      x = x_margin = (layout_w - w) >> 1;
      y = y_margin = (layout_h - h) >> 1;

      for (i = 0; i < n; i++) {
        iter = children[i];

        if (y >= layout_h || x >= layout_w) {
          widget_move_resize(iter, 0, 0, 0, 0);
          continue;
        }

        widget_move_resize(iter, x, y, item_w, item_h);
        c++;
        if (c == cols) {
          r++;
          y += item_h + spacing;
          c = 0;
          x = x_margin;
        } else {
          x += item_w + spacing;
        }

        widget_layout_children(children[i]);
      }

      return RET_OK;
    } else if (rows == 0 && cols == 0) { /*free layout*/
      /*fall through*/
    } else { /*not support*/
      log_debug("not supported(rows=%d, cols=%d)\n", rows, cols);
      return RET_OK;
    }
  }

  for (i = 0; i < n; i++) {
    iter = children[i];
    widget_layout_self(iter);
  }

  for (i = 0; i < n; i++) {
    iter = children[i];
    x = iter->x;
    y = iter->y;
    w = iter->w;
    h = iter->h;
    if (iter->layout_params) {
      widget_layout_t* l = &(iter->layout_params->self);
      if (l->w_attr == W_ATTR_FILL || l->h_attr == H_ATTR_FILL) {
        widget_t* prev = i > 0 ? widget_get_child(widget, i - 1) : NULL;
        widget_t* next = (i + 1) == widget->children->size ? NULL : widget_get_child(widget, i + 1);

        if (l->w_attr == W_ATTR_FILL) {
          if (prev) {
            x = prev->x + prev->w + spacing;
          } else {
            x = x_margin;
          }
          if (next) {
            w = next->x - spacing - x;
          } else {
            w = layout_w - x_margin - x;
          }
        }

        if (l->h_attr == H_ATTR_FILL) {
          if (prev != NULL) {
            y = prev->y + prev->h + spacing;
          } else {
            y = y_margin;
          }

          if (next != NULL) {
            h = next->y - spacing - y;
          } else {
            h = layout_h - y_margin - y;
          }
        }

        return_value_if_fail(w > 0 && h > 0, RET_BAD_PARAMS);
        widget_move_resize(iter, x, y, w, h);
      }
    }
  }

  for (i = 0; i < n; i++) {
    widget_layout_children(children[i]);
  }

  return RET_OK;
}

children_layout_t* children_layout_parser(children_layout_t* layout, const char* params) {
  tokenizer_t tokenizer;
  tokenizer_t* t = &tokenizer;
  return_value_if_fail(layout != NULL && params != NULL, NULL);

  memset(layout, 0x00, sizeof(*layout));
  tokenizer_init(t, params, strlen(params), " ,");

  while (tokenizer_has_more(t)) {
    const char* token = tokenizer_next(t);
    int val = tk_atoi(tk_skip_to_num(token));

    switch (*token) {
      case 'w': {
        layout->cols = val;
        layout->cols_is_width = TRUE;
        break;
      }
      case 'h': {
        layout->rows = val;
        layout->rows_is_height = TRUE;
        break;
      }
      case 'c': {
        layout->cols = val;
        layout->cols_is_width = FALSE;
        break;
      }
      case 'r': {
        layout->rows = val;
        layout->rows_is_height = FALSE;
        break;
      }
      case 'x': {
        layout->x_margin = val;
        break;
      }
      case 'y': {
        layout->y_margin = val;
        break;
      }
      case 'm': {
        layout->x_margin = val;
        layout->y_margin = val;
        break;
      }
      case 's': {
        layout->spacing = val;
        break;
      }
      default:
        break;
    }
  }
  tokenizer_deinit(t);

  return layout;
}

/**
 * File:   layout.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  layout
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/layout.h"

widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y,
                                     const char* w, const char* h) {
  return_value_if_fail(layout != NULL, NULL);

  memset(layout, 0x00, sizeof(*layout));
  if (x != NULL) {
    if (x[0] == 'c') {
      if (x[6] == ':') {
        layout->x = atoi(x + 7);
      }
      layout->x_attr = X_ATTR_CENTER;
    } else if (x[0] == 'r') {
      if (x[5] == ':') {
        layout->x = atoi(x + 6);
      }
      layout->x_attr = X_ATTR_RIGHT;
    } else if (strstr(x, "%") != NULL) {
      layout->x = atoi(x);
      layout->x_attr = X_ATTR_PERCENT;
    } else {
      layout->x = atoi(x);
      layout->x_attr = X_ATTR_DEFAULT;
    }
  } else {
    layout->x_attr = X_ATTR_DEFAULT;
  }

  if (y != NULL) {
    if (y[0] == 'm') {
      if (y[6] == ':') {
        layout->y = atoi(y + 7);
      }
      layout->y_attr = Y_ATTR_MIDDLE;
    } else if (y[0] == 'b') {
      if (y[6] == ':') {
        layout->y = atoi(y + 7);
      }
      layout->y_attr = Y_ATTR_BOTTOM;
    } else if (strstr(y, "%") != NULL) {
      layout->y = atoi(y);
      layout->y_attr = Y_ATTR_PERCENT;
    } else {
      layout->y = atoi(y);
      layout->y_attr = Y_ATTR_DEFAULT;
    }
  } else {
    layout->y_attr = Y_ATTR_DEFAULT;
  }

  if (w != NULL) {
    layout->w = atoi(w);
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
    layout->h = atoi(h);
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
      w = (parent_w * layout->w) / 100;
    }

    if (h_attr == H_ATTR_PERCENT) {
      h = (parent_h * layout->h) / 100;
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

ret_t widget_set_parsed_self_layout_params(widget_t* widget, const widget_layout_t* layout) {
  return_value_if_fail(widget != NULL && layout != NULL, RET_BAD_PARAMS);
  if (widget->layout_params == NULL) {
    widget->layout_params = MEM_ZALLOC(layout_params_t);
  }
  return_value_if_fail(widget->layout_params != NULL, RET_OOM);
  memcpy(&(widget->layout_params->self), layout, sizeof(*layout));

  return RET_OK;
}

ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w,
                                    const char* h) {
  widget_layout_t layout;
  return_value_if_fail(widget != NULL && x != NULL && y != NULL && w != NULL && h != NULL,
                       RET_BAD_PARAMS);
  widget_layout_parse(&layout, x, y, w, h);

  return widget_set_parsed_self_layout_params(widget, &layout);
}

ret_t widget_set_children_layout_params(widget_t* widget, uint8_t rows, uint8_t cols,
                                        uint8_t margin, uint8_t cell_spacing) {
  children_layout_t* layout = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->layout_params == NULL) {
    widget->layout_params = MEM_ZALLOC(layout_params_t);
  }
  return_value_if_fail(widget->layout_params != NULL, RET_OOM);
  layout = &(widget->layout_params->children);
  layout->rows = rows;
  layout->cols = cols;
  layout->margin = margin;
  layout->cell_spacing = cell_spacing;

  return RET_OK;
}

ret_t widget_layout(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  widget_layout_self(widget);

  return widget_layout_children(widget);
}

ret_t widget_layout_self(widget_t* widget) {
  rect_t r;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL && widget->layout_params != NULL) {
    widget_layout_t* layout = &(widget->layout_params->self);
    widget_layout_calc(layout, &r, widget->parent->w, widget->parent->h);
    widget_move_resize(widget, r.x, r.y, r.w, r.h);
  }

  return RET_OK;
}

ret_t widget_layout_children(widget_t* widget) {
  wh_t w = 0;
  wh_t h = 0;
  xy_t x = 0;
  xy_t y = 0;
  uint32_t i = 0;
  uint32_t n = 0;
  uint8_t margin = 0;
  widget_t* iter = NULL;
  uint8_t cell_spacing = 0;
  widget_t** children = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children == NULL) {
    return RET_OK;
  }

  if (widget->vt->on_layout_children != NULL) {
    return widget->vt->on_layout_children(widget);
  }

  n = widget->children->size;
  children = (widget_t**)(widget->children->elms);

  if (widget->layout_params) {
    children_layout_t* layout = &(widget->layout_params->children);
    uint8_t rows = layout->rows;
    uint8_t cols = layout->cols;

    x = layout->margin;
    y = layout->margin;
    margin = layout->margin;
    cell_spacing = layout->cell_spacing;

    if (rows == 1 && cols == 0) { /*hbox*/
      h = widget->h - 2 * margin;
      return_value_if_fail(h > 0, RET_BAD_PARAMS);

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_layout_self(iter);
      }

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_move_resize(iter, x, y, iter->w, h);
        x += iter->w + cell_spacing;
        return_value_if_fail(x <= widget->w, RET_BAD_PARAMS);
      }

      for (i = 0; i < n; i++) {
        widget_layout_children(children[i]);
      }

      return RET_OK;
    } else if (cols == 1 && rows == 0) { /*vbox*/
      w = widget->w - 2 * margin;
      return_value_if_fail(w > 0, RET_BAD_PARAMS);

      for (i = 0; i < n; i++) {
        iter = children[i];
        widget_layout_self(iter);
      }

      for (i = 0; i < n; i++) {
        iter = children[i];
        return_value_if_fail(y <= widget->h, RET_BAD_PARAMS);
        widget_move_resize(iter, x, y, w, iter->h);
        y += iter->h + cell_spacing;
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
      w = widget->w - 2 * margin - (cols - 1) * cell_spacing;
      h = widget->h - 2 * margin - (rows - 1) * cell_spacing;
      item_w = w / cols;
      item_h = h / rows;
      return_value_if_fail(item_w > 0 && item_h > 0, RET_BAD_PARAMS);

      for (i = 0; i < n; i++) {
        iter = children[i];
        return_value_if_fail(y <= widget->h, RET_BAD_PARAMS);

        widget_move_resize(iter, x, y, item_w, item_h);
        c++;
        if (c == cols) {
          r++;
          y += item_h + cell_spacing;
          c = 0;
          x = margin;
        } else {
          x += item_w + cell_spacing;
        }
      }

      for (i = 0; i < n; i++) {
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
            x = prev->x + prev->w + cell_spacing;
          } else {
            x = margin;
          }
          if (next) {
            w = (next->x - cell_spacing) - x;
          } else {
            w = widget->w - margin - x;
          }
        }

        if (l->h_attr == H_ATTR_FILL) {
          if (prev != NULL) {
            y = prev->y + prev->h + cell_spacing;
          } else {
            y = margin;
          }

          if (next != NULL) {
            h = next->y - cell_spacing - y;
          } else {
            h = widget->h - margin - y;
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
  const char* p = params;
  return_value_if_fail(layout != NULL && params != NULL, NULL);

  memset(layout, 0x00, sizeof(*layout));

  layout->rows = atoi(p);
  p = strchr(p, ' ');
  if (p != NULL) {
    while (*p == ' ') p++;
    if (*p) {
      layout->cols = atoi(p);
    }

    p = strchr(p, ' ');
    if (p != NULL) {
      while (*p == ' ') p++;
      if (*p) {
        layout->margin = atoi(p);
      }

      p = strchr(p, ' ');
      if (p != NULL) {
        while (*p == ' ') p++;
        if (*p) {
          layout->cell_spacing = atoi(p);
        }
      }
    }
  }

  return layout;
}

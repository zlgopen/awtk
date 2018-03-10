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

#include "base/layout.h"

widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y,
                                     const char* w, const char* h) {
  return_value_if_fail(layout != NULL, NULL);

  memset(layout, 0x00, sizeof(*layout));
  if (x != NULL) {
    if (strstr(x, "center") != NULL) {
      if (x[6] == ':') {
        layout->x = atoi(x + 7);
      }
      layout->x_attr = X_ATTR_CENTER;
    } else if (strstr(x, "right") != NULL) {
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
    if (strstr(y, "middle") != NULL) {
      if (y[6] == ':') {
        layout->y = atoi(y + 7);
      }
      layout->y_attr = Y_ATTR_MIDDLE;
    } else if (strstr(y, "bottom") != NULL) {
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
    if (w != NULL && strchr(w, '%') != NULL) {
      layout->w_attr = W_ATTR_PERCENT;
    } else {
      layout->w_attr = W_ATTR_PIXEL;
    }
  } else {
    layout->h_attr = H_ATTR_PIXEL;
  }

  if (h != NULL) {
    layout->h = atoi(h);
    if (strchr(h, '%') != NULL) {
      layout->h_attr = H_ATTR_PERCENT;
    } else {
      layout->h_attr = H_ATTR_PIXEL;
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

ret_t widget_set_self_layout_params(widget_t* widget, const widget_layout_t* layout) {
  /*TODO*/
  return RET_OK;
}

ret_t widget_set_children_layout_params(widget_t* widget, uint8_t rows, uint8_t cols,
                                        uint8_t margin, uint8_t cell_h_margin,
                                        uint8_t cell_v_margin) {
  /*TODO*/
  return RET_OK;
}

ret_t widget_layout(widget_t* widget) {
  /*TODO*/
  return RET_OK;
}

ret_t widget_layout_self(widget_t* widget) {
  /*TODO*/
  return RET_OK;
}

ret_t widget_layout_children(widget_t* widget) {
  /*TODO*/
  return RET_OK;
}

/**
 * File:   layout.c
 * Author: AWTK Develop Team
 * Brief:  widget layout
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/layout.h"
#include "base/self_layouter_factory.h"
#include "base/children_layouter_factory.h"

ret_t widget_auto_adjust_size(widget_t* widget) {
  int32_t w = 0;
  int32_t h = 0;
  int32_t right = 0;
  int32_t bottom = 0;
  int32_t margin = 0;
  int32_t margin_right = 0;
  int32_t margin_bottom = 0;
  style_t* style = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  if (style != NULL) {
    margin = style_get_int(style, STYLE_ID_MARGIN, 2);
    margin_right = style_get_int(style, STYLE_ID_MARGIN_RIGHT, margin);
    margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  right = iter->x + iter->w + margin_right;
  bottom = iter->y + iter->h + margin_bottom;
  if (right > w) {
    w = right;
  }
  if (bottom > h) {
    h = bottom;
  }
  WIDGET_FOR_EACH_CHILD_END();

  if (w != 0 && h != 0) {
    widget_resize(widget, w, h);
  }

  return RET_OK;
}

ret_t widget_layout(widget_t* widget) {
  widget_layout_self(widget);
  widget_layout_children(widget);

  if (widget->auto_adjust_size) {
    widget_auto_adjust_size(widget);
  }

  return RET_OK;
}

ret_t widget_layout_self(widget_t* widget) {
  value_t v;
  rect_t r = {0, 0, 0, 0};
  widget_t* parent = NULL;
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);

  parent = widget->parent;
  if (widget_get_prop(parent, WIDGET_PROP_LAYOUT_W, &v) == RET_OK) {
    r.w = value_int(&v);
  } else {
    r.w = parent->w;
  }

  if (widget_get_prop(parent, WIDGET_PROP_LAYOUT_H, &v) == RET_OK) {
    r.h = value_int(&v);
  } else {
    r.h = parent->h;
  }

  return self_layouter_layout(widget->self_layout, widget, &r);
}

ret_t widget_layout_children_default(widget_t* widget) {
  if (widget->children_layout != NULL) {
    return children_layouter_layout(widget->children_layout, widget);
  } else {
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    widget_layout(iter);
    WIDGET_FOR_EACH_CHILD_END()

    return RET_OK;
  }
}

ret_t widget_layout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_layout_children != NULL) {
    return widget->vt->on_layout_children(widget);
  } else {
    return widget_layout_children_default(widget);
  }
}

ret_t widget_set_self_layout(widget_t* widget, const char* params) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->self_layout != NULL) {
    if (tk_str_eq(widget->self_layout->params.str, params)) {
      return RET_OK;
    }
    self_layouter_destroy(widget->self_layout);
    widget->self_layout = NULL;
  }

  if (params != NULL && params[0] != '\0') {
    widget->self_layout = self_layouter_create(params);
  }

  if (widget->self_layout != NULL) {
    str_set(&(widget->self_layout->params), params);
  }

  return widget_set_need_relayout(widget);
}

ret_t widget_set_children_layout(widget_t* widget, const char* params) {
  return_value_if_fail(widget != NULL && params != NULL, RET_BAD_PARAMS);
  if (widget->children_layout != NULL) {
    if (tk_str_eq(widget->children_layout->params.str, params)) {
      return RET_OK;
    }
    children_layouter_destroy(widget->children_layout);
  }

  widget->children_layout = children_layouter_create(params);

  if (widget->children_layout != NULL) {
    str_set(&(widget->children_layout->params), params);
  }

  return widget_set_need_relayout(widget);
}

ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w,
                                    const char* h) {
  char params[128];
  tk_snprintf(params, sizeof(params) - 1, "default(x=%s, y=%s, w=%s, h=%s)", x, y, w, h);

  return widget_set_self_layout(widget, params);
}

ret_t widget_layout_floating_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL) {
    uint32_t i = 0;
    uint32_t n = widget->children->size;
    widget_t** children = (widget_t**)(widget->children->elms);

    for (i = 0; i < n; i++) {
      widget_t* iter = children[i];
      if (iter->floating) {
        widget_layout(iter);
      }
    }
  }

  return RET_OK;
}

ret_t widget_get_children_for_layout(widget_t* widget, darray_t* result, bool_t keep_disable,
                                     bool_t keep_invisible) {
  return_value_if_fail(widget != NULL && result != NULL, RET_BAD_PARAMS);

  result->size = 0;
  if (widget->children != NULL) {
    uint32_t i = 0;
    uint32_t n = widget->children->size;
    widget_t** children = (widget_t**)(widget->children->elms);

    for (i = 0; i < n; i++) {
      widget_t* iter = children[i];

      if (iter->floating) {
        continue;
      }

      if (!(iter->enable)) {
        if (!keep_disable) {
          continue;
        }
      }

      if (!(iter->visible)) {
        if (!keep_invisible) {
          continue;
        }
      }

      darray_push(result, iter);
    }
  }

  return RET_OK;
}

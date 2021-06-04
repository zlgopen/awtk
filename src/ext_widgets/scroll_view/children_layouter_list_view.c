/**
 * File:   children_layouter_list_view_list_view.c
 * Author: AWTK Develop Team
 * Brief:  children layouter list view
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/rect.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/widget.h"
#include "tkc/tokenizer.h"
#include "scroll_view/scroll_bar.h"
#include "scroll_view/scroll_view.h"
#include "scroll_view/list_view.h"
#include "scroll_view/list_view_h.h"
#include "scroll_view/children_layouter_list_view.h"

static const char* children_layouter_list_view_to_string(children_layouter_t* layouter) {
  char temp[32];
  str_t* str = &(layouter->params);
  children_layouter_list_view_t* layout = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(layout != NULL, NULL);
  memset(temp, 0, sizeof(temp));
  str_set(str, "list_view(");
  str_append(str, temp);

  if (layout->item_height) {
    tk_snprintf(temp, sizeof(temp) - 1, "item_height=%d,", (int)(layout->item_height));
  }
  str_append(str, temp);

  if (layout->default_item_height) {
    tk_snprintf(temp, sizeof(temp) - 1, "default_item_height=%d,",
                (int)(layout->default_item_height));
  }
  str_append(str, temp);

  if (layout->x_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "x_margin=%d,", (int)(layout->x_margin));
    str_append(str, temp);
  }

  if (layout->y_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "y_margin=%d,", (int)(layout->y_margin));
    str_append(str, temp);
  }

  if (layout->spacing) {
    tk_snprintf(temp, sizeof(temp) - 1, "spacing=%d,", (int)(layout->spacing));
    str_append(str, temp);
  }

  if (layout->cols > 1) {
    tk_snprintf(temp, sizeof(temp) - 1, "cols=%d,", (int)(layout->cols));
    str_append(str, temp);
  }

  if (!(layout->keep_disable)) {
    str_append(str, "keep_disable=false,");
  }

  if (layout->keep_invisible) {
    str_append(str, "keep_invisible=true,");
  }

  if (layout->hlayouter) {
    str_append(str, "hlayouter=true,");
  }

  str_trim_right(str, ",");
  str_append(str, ")");

  return str->str;
}

static ret_t children_layouter_list_view_set_param(children_layouter_t* layouter, const char* name,
                                                   const value_t* v) {
  int val = value_int(v);
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;

  switch (*name) {
    case 'x': {
      l->x_margin = val;
      break;
    }
    case 'y': {
      l->y_margin = val;
      break;
    }
    case 'm': {
      l->x_margin = val;
      l->y_margin = val;
      break;
    }
    case 's': {
      l->spacing = val;
      break;
    }
    case 'i': {
      l->item_height = val;
      break;
    }
    case 'd': {
      l->default_item_height = val;
      break;
    }
    case 'c': {
      l->cols = val;
      break;
    }
    case 'k': {
      if (strstr(name, "invisible") != NULL || name[1] == 'i') {
        l->keep_invisible = value_bool(v);
      } else if (strstr(name, "disable") != NULL || name[1] == 'd') {
        l->keep_disable = value_bool(v);
      }
      break;
    }
    case 'h': {
      if (strstr(name, "layouter") != NULL || name[1] == 'l') {
        l->hlayouter = value_bool(v);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t children_layouter_list_view_get_param(children_layouter_t* layouter, const char* name,
                                                   value_t* v) {
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;

  switch (*name) {
    case 'x': {
      value_set_int(v, l->x_margin);
      return RET_OK;
    }
    case 'y': {
      value_set_int(v, l->y_margin);
      return RET_OK;
    }
    case 'm': {
      value_set_int(v, l->x_margin);
      return RET_OK;
    }
    case 's': {
      value_set_int(v, l->spacing);
      return RET_OK;
    }
    case 'i': {
      value_set_int(v, l->item_height);
      return RET_OK;
    }
    case 'd': {
      value_set_int(v, l->default_item_height);
      return RET_OK;
    }
    case 'c': {
      value_set_int(v, l->cols);
      return RET_OK;
    }
    case 'k': {
      if (strstr(name, "invisible") != NULL || name[1] == 'i') {
        value_set_bool(v, l->keep_invisible);
        return RET_OK;
      } else if (strstr(name, "disable") != NULL || name[1] == 'd') {
        value_set_bool(v, l->keep_disable);
        return RET_OK;
      }
      break;
    }
    case 'h': {
      if (strstr(name, "layouter") != NULL || name[1] == 'l') {
        value_set_bool(v, l->hlayouter);
      }
      break;
    }
    default: {
      assert(!"not support param");
      break;
    }
  }

  return RET_FAIL;
}

static ret_t children_layouter_list_view_for_list_view_children_layout_h(
    darray_t* children_for_layout, int32_t item_height, int32_t default_item_height) {
  int32_t i = 0;
  int32_t h = 0;
  int32_t n = 0;
  widget_t** children = NULL;
  return_value_if_fail(children_for_layout != NULL, RET_BAD_PARAMS);
  n = children_for_layout->size;
  children = (widget_t**)children_for_layout->elms;
  for (i = 0; i < n; i++) {
    widget_t* iter = children[i];

    if (iter->w == 0) {
      iter->w = iter->parent->w;
    }
    if (iter->h == 0) {
      iter->h = item_height;
    }

    widget_layout(iter);

    h = item_height;
    if (h <= 0) {
      h = iter->h;
    }
    if (h <= 0) {
      h = default_item_height;
    }
    widget_resize(iter, iter->w, h);
  }
  return RET_OK;
}

static ret_t children_layouter_list_view_for_list_view_children_layout_w(
    darray_t* children_for_layout, uint32_t cols, int32_t x_margin, int32_t y_margin,
    int32_t spacing, int32_t scroll_view_w) {
  int32_t i = 0;
  int32_t w = 0;
  int32_t x = x_margin;
  int32_t y = y_margin;
  widget_t** children = NULL;
  return_value_if_fail(children_for_layout != NULL, RET_BAD_PARAMS);
  w = scroll_view_w - 2 * x_margin;
  children = (widget_t**)children_for_layout->elms;
  if (cols <= 1) {
    for (i = 0; i < children_for_layout->size; i++) {
      widget_t* iter = children[i];
      widget_move_resize(iter, x, y, w, iter->h);
      widget_layout_children(iter);
      y += (iter->h + spacing);
    }
  } else {
    int32_t j = 0;
    int32_t n = 0;
    int32_t h = 0;
    int32_t size = children_for_layout->size;
    uint32_t rows = (size % cols) ? (size / cols) + 1 : (size / cols);

    w = (w - (cols - 1) * spacing) / cols;
    for (i = 0; i < rows && n < size; i++) {
      h = 0;
      for (j = 0; j < cols && n < size; j++, n++) {
        widget_t* iter = children[n];
        int32_t tmp_x = x + j * (w + spacing);
        widget_move_resize(iter, tmp_x, y, w, iter->h);
        widget_layout_children(iter);
        h = tk_max(h, iter->h);
      }
      y += (h + spacing);
    }
  }
  return RET_OK;
}

static int32_t children_layouter_list_view_for_list_view_get_virtual_h(
    darray_t* children_for_layout, uint32_t cols, int32_t y_margin, int32_t spacing) {
  int32_t i = 0;
  widget_t** children = NULL;
  int32_t virtual_h = y_margin;
  return_value_if_fail(children_for_layout != NULL, 0);
  children = (widget_t**)children_for_layout->elms;
  if (cols <= 1) {
    for (i = 0; i < children_for_layout->size; i++) {
      virtual_h += (children[i]->h + spacing);
    }
  } else {
    int32_t j = 0;
    int32_t h = 0;
    int32_t num = 0;
    int32_t n = children_for_layout->size;
    uint32_t rows = (n % cols) ? (n / cols) + 1 : (n / cols);
    for (i = 0; i < rows && num < children_for_layout->size; i++) {
      for (j = 0, h = 0; j < cols && num < children_for_layout->size; j++, num++) {
        h = tk_max(h, children[num]->h);
      }
      virtual_h += (h + spacing);
    }
  }
  return virtual_h;
}

static int32_t children_layouter_list_view_for_list_view_get_scroll_view_w(list_view_t* list_view,
                                                                           widget_t* scroll_view,
                                                                           int32_t virtual_h) {
  int32_t scroll_view_w = 0;
  widget_t* scroll_bar = NULL;
  return_value_if_fail(list_view != NULL && scroll_view != NULL, 0);
  scroll_bar = list_view->scroll_bar;
  if (list_view->floating_scroll_bar || scroll_bar == NULL || scroll_bar_is_mobile(scroll_bar) ||
      (!scroll_bar_is_mobile(scroll_bar) && list_view->auto_hide_scroll_bar &&
       scroll_view->h >= virtual_h)) {
    scroll_view_w = list_view->widget.w;
  } else {
    scroll_view_w = list_view->widget.w - scroll_bar->w;
  }

  return scroll_view_w;
}

static ret_t children_layouter_list_view_for_list_view_set_scroll_view_info(widget_t* widget,
                                                                            widget_t* scroll_bar,
                                                                            int32_t virtual_h) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  if (widget->h >= virtual_h) {
    scroll_view_set_offset(widget, 0, 0);
  }
  if (scroll_bar_is_mobile(scroll_bar)) {
    scroll_view_set_yslidable(widget, TRUE);
  }

  scroll_view_set_xslidable(widget, FALSE);
  scroll_view_set_virtual_h(widget, virtual_h);
  scroll_view->xoffset = 0;
  if (scroll_view->yoffset + widget->h > scroll_view->virtual_h) {
    scroll_view->yoffset = scroll_view->virtual_h - widget->h;
    scroll_view->yoffset = scroll_view->yoffset > 0 ? scroll_view->yoffset : 0;
  }
  if (scroll_view->on_scroll) {
    scroll_view->on_scroll(widget, scroll_view->xoffset, scroll_view->yoffset);
  }
  return RET_OK;
}

static ret_t children_layouter_list_view_for_list_view_set_scroll_bar_info(widget_t* widget,
                                                                           list_view_t* list_view,
                                                                           widget_t* scroll_view,
                                                                           int32_t virtual_h,
                                                                           int32_t item_height) {
  return_value_if_fail(widget != NULL && list_view != NULL, RET_BAD_PARAMS);

  scroll_bar_set_params(widget, virtual_h, item_height);
  if (scroll_bar_is_mobile(widget)) {
    if (widget->h > virtual_h) {
      scroll_bar_set_params(widget, widget->h, item_height);
    }

    if (SCROLL_BAR(widget)->auto_hide) {
      widget_set_visible_only(widget, FALSE);
    }
  } else {
    if (scroll_view->h >= virtual_h) {
      scroll_bar_set_value(widget, 0);
      if (list_view->auto_hide_scroll_bar || list_view->floating_scroll_bar) {
        widget_set_visible_only(widget, FALSE);
      } else {
        widget_set_enable(widget, FALSE);
        widget_set_visible_only(widget, TRUE);
      }
    } else {
      if (list_view->auto_hide_scroll_bar && list_view->floating_scroll_bar) {
        widget_set_visible_only(widget, list_view->is_over);
      } else {
        widget_set_enable(widget, TRUE);
        widget_set_visible_only(widget, TRUE);
      }
    }
  }
  return RET_OK;
}

static ret_t children_layouter_list_view_for_list_view_layout(children_layouter_t* layouter,
                                                              widget_t* widget) {
  int32_t virtual_h = 0;
  int32_t item_height = 0;
  list_view_t* list_view = NULL;
  int32_t default_item_height = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(widget != NULL && scroll_view != NULL && l != NULL, RET_BAD_PARAMS);

  list_view = LIST_VIEW(widget->parent);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  item_height = list_view->item_height ? list_view->item_height : l->item_height;
  default_item_height =
      list_view->default_item_height ? list_view->default_item_height : l->default_item_height;

  if (widget->children != NULL) {
    int32_t scroll_view_w = 0;
    darray_t children_for_layout;
    uint32_t cols = l->cols <= 1 ? 1 : l->cols;

    widget_layout_floating_children(widget);
    darray_init(&children_for_layout, widget->children->size, NULL, NULL);

    return_value_if_fail(
        widget_get_children_for_layout(widget, &children_for_layout, l->keep_disable,
                                       l->keep_invisible) == RET_OK,
        RET_BAD_PARAMS);

    children_layouter_list_view_for_list_view_children_layout_h(&children_for_layout, item_height,
                                                                default_item_height);
    virtual_h = children_layouter_list_view_for_list_view_get_virtual_h(&children_for_layout, cols,
                                                                        l->y_margin, l->spacing);
    scroll_view_w =
        children_layouter_list_view_for_list_view_get_scroll_view_w(list_view, widget, virtual_h);

    widget_resize(widget, scroll_view_w, widget->h);
    children_layouter_list_view_for_list_view_children_layout_w(
        &children_for_layout, cols, l->x_margin, l->y_margin, l->spacing, scroll_view_w);

    darray_deinit(&(children_for_layout));
  }

  children_layouter_list_view_for_list_view_set_scroll_view_info(widget, list_view->scroll_bar,
                                                                 virtual_h);
  children_layouter_list_view_for_list_view_set_scroll_bar_info(list_view->scroll_bar, list_view,
                                                                widget, virtual_h, item_height);
  return RET_OK;
}

static ret_t children_layouter_list_view_for_list_view_h_layout(children_layouter_t* layouter,
                                                                widget_t* widget) {
  int32_t virtual_w = 0;
  list_view_h_t* list_view_h = NULL;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(widget != NULL && scroll_view != NULL && l != NULL, RET_BAD_PARAMS);

  list_view_h = LIST_VIEW_H(widget->parent);
  return_value_if_fail(list_view_h != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL && l->cols > 0) {
    int32_t i = 0;
    int32_t nw = 0;
    int32_t x = l->x_margin;
    int32_t y = l->y_margin;
    uint32_t cols = l->cols;
    widget_t** children = NULL;
    darray_t children_for_layout;
    int32_t spacing = l->spacing;
    int32_t h = widget->h - l->y_margin * 2;
    int32_t w = widget->w - l->x_margin * 2;
    int32_t oy = l->y_margin - l->item_height - spacing;
    uint32_t item_w = (w - (cols - 1) * spacing) / cols;

    widget_layout_floating_children(widget);
    darray_init(&children_for_layout, widget->children->size, NULL, NULL);

    return_value_if_fail(
        widget_get_children_for_layout(widget, &children_for_layout, l->keep_disable,
                                       l->keep_invisible) == RET_OK,
        RET_BAD_PARAMS);

    children = (widget_t**)(children_for_layout.elms);
    y = oy;
    virtual_w = w;
    for (i = 0; i < children_for_layout.size; i++) {
      widget_t* iter = children[i];

      if (i % cols == 0) {
        x = l->x_margin + w * nw;
        y += l->item_height + spacing;
        if (y + l->item_height + spacing > h) {
          nw++;
          x += w;
          virtual_w += w;
          y = oy + l->item_height + spacing;
        }
      } else {
        x += item_w + spacing;
      }

      widget_move_resize(iter, x, y, item_w, l->item_height);
      widget_layout(iter);
    }
    scroll_view_set_virtual_w(list_view_h->scroll_view, virtual_w);
    darray_deinit(&children_for_layout);
  }

  return RET_OK;
}

static ret_t children_layouter_list_view_layout(children_layouter_t* layouter, widget_t* widget) {
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);
  if (l->hlayouter) {
    return children_layouter_list_view_for_list_view_h_layout(layouter, widget);
  } else {
    return children_layouter_list_view_for_list_view_layout(layouter, widget);
  }
}

static bool_t children_layouter_list_view_is_valid(children_layouter_t* layouter) {
  return TRUE;
}

static ret_t children_layouter_list_view_destroy(children_layouter_t* layouter) {
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);
  str_reset(&(layouter->params));
  TKMEM_FREE(l);

  return RET_OK;
}

static children_layouter_t* children_layouter_list_view_clone(children_layouter_t* layouter) {
  children_layouter_list_view_t* l = TKMEM_ZALLOC(children_layouter_list_view_t);
  return_value_if_fail(l != NULL, NULL);
  memcpy(l, layouter, sizeof(*l));
  str_init(&(l->layouter.params), 0);
  str_set(&(l->layouter.params), layouter->params.str);

  return (children_layouter_t*)l;
}

static const children_layouter_vtable_t s_children_layouter_list_view_vtable = {
    .type = CHILDREN_LAYOUTER_LIST_VIEW,
    .clone = children_layouter_list_view_clone,
    .to_string = children_layouter_list_view_to_string,
    .get_param = children_layouter_list_view_get_param,
    .set_param = children_layouter_list_view_set_param,
    .layout = children_layouter_list_view_layout,
    .is_valid = children_layouter_list_view_is_valid,
    .destroy = children_layouter_list_view_destroy};

children_layouter_t* children_layouter_list_view_create(void) {
  children_layouter_t* l = NULL;
  children_layouter_list_view_t* layouter = NULL;

  layouter = TKMEM_ZALLOC(children_layouter_list_view_t);
  return_value_if_fail(layouter != NULL, NULL);

  l = (children_layouter_t*)layouter;
  layouter->keep_disable = TRUE;
  str_init(&(l->params), 0);
  l->vt = &s_children_layouter_list_view_vtable;

  return (children_layouter_t*)layouter;
}

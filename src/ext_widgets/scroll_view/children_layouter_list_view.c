/**
 * File:   children_layouter_list_view_list_view.c
 * Author: AWTK Develop Team
 * Brief:  children layouter list view
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
#include "scroll_view/children_layouter_list_view.h"

static const char* children_layouter_list_view_to_string(children_layouter_t* layouter) {
  char temp[32];
  str_t* str = &(layouter->params);
  children_layouter_list_view_t* layout = (children_layouter_list_view_t*)layouter;

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
    default: {
      assert(!"not support param");
      break;
    }
  }

  return RET_FAIL;
}

static ret_t children_layouter_list_view_layout(children_layouter_t* layouter, widget_t* widget) {
  int32_t spacing = 0;
  int32_t x_margin = 0;
  int32_t y_margin = 0;
  int32_t virtual_h = 0;
  int32_t item_height = 0;
  int32_t default_item_height = 0;
  list_view_t* list_view = NULL;
  widget_t* scroll_bar = NULL;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  return_value_if_fail(widget != NULL && scroll_view != NULL && l != NULL, RET_BAD_PARAMS);

  virtual_h = widget->h;
  list_view = LIST_VIEW(widget->parent);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  scroll_bar = list_view->scroll_bar;
  spacing = l->spacing;
  x_margin = l->x_margin;
  y_margin = l->y_margin;
  item_height = list_view->item_height ? list_view->item_height : l->item_height;
  default_item_height =
      list_view->default_item_height ? list_view->default_item_height : l->default_item_height;

  if (widget->children != NULL) {
    int32_t i = 0;
    int32_t n = 0;
    int32_t x = x_margin;
    int32_t y = y_margin;
    int32_t h = item_height;
    widget_t** children = NULL;
    darray_t children_for_layout;
    int32_t w = widget->w - x_margin * 2;
    uint32_t cols = l->cols <= 1 ? 1 : l->cols;

    widget_layout_floating_children(widget);
    darray_init(&children_for_layout, widget->children->size, NULL, NULL);

    return_value_if_fail(
        widget_get_children_for_layout(widget, &children_for_layout, l->keep_disable,
                                       l->keep_invisible) == RET_OK,
        RET_BAD_PARAMS);

    n = children_for_layout.size;
    children = (widget_t**)(children_for_layout.elms);

    if (cols <= 1) {
      for (i = 0; i < n; i++) {
        widget_t* iter = children[i];

        if (item_height <= 0) {
          h = iter->h;
        }

        if (h <= 0) {
          h = default_item_height;
        }

        y = y + (h > 0 ? h : iter->h) + spacing;
        if (y > virtual_h) {
          virtual_h = y;
        }
      }
    } else {
      uint32_t rows = (n % cols) ? (n / cols) + 1 : (n / cols);
      if (item_height == 0) item_height = default_item_height;
      y = (item_height + spacing) * rows;
      if (y > virtual_h) {
        virtual_h = y;
      }
    }

    scroll_view->widget.w = list_view->widget.w;
    if (scroll_bar != NULL) {
      if (!scroll_bar_is_mobile(scroll_bar)) {
        if (list_view->auto_hide_scroll_bar) {
          if (virtual_h <= widget->h) {
            widget_set_visible_only(scroll_bar, FALSE);
            widget_set_enable(scroll_bar, FALSE);
          } else {
            scroll_view->widget.w = list_view->widget.w - scroll_bar->w;
            widget_set_visible_only(scroll_bar, TRUE);
            widget_set_enable(scroll_bar, TRUE);
          }
        } else {
          if (!widget_get_prop_bool(scroll_bar, WIDGET_PROP_VISIBLE, FALSE)) {
            widget_set_visible_only(scroll_bar, TRUE);
            widget_set_enable(scroll_bar, TRUE);
          }
          scroll_view->widget.w = list_view->widget.w - scroll_bar->w;
        }
      }
    }

    y = y_margin;
    w = scroll_view->widget.w - 2 * x_margin;

    if (cols <= 1) {
      for (i = 0; i < n; i++) {
        widget_t* iter = children[i];

        if (item_height <= 0) {
          h = iter->h;
        }

        if (h <= 0) {
          h = default_item_height;
        }

        widget_move_resize(iter, x, y, w, h);
        widget_layout(iter);

        y = iter->y + iter->h + spacing;
      }
    } else {
      uint32_t item_w = (w - (cols - 1) * spacing) / cols;

      h = item_height;
      y = y_margin - item_height - spacing;
      for (i = 0; i < n; i++) {
        widget_t* iter = children[i];

        if (i % cols == 0) {
          x = x_margin;
          y += item_height + spacing;
        } else {
          x += item_w + spacing;
        }

        widget_move_resize(iter, x, y, item_w, h);
        widget_layout(iter);
      }
    }

    if (scroll_bar != NULL && (SCROLL_BAR(scroll_bar)->value) >= y) {
      int32_t offset = tk_max(0, (y - widget->h + item_height));
      scroll_bar_set_value(scroll_bar, offset);
      scroll_view_set_offset(WIDGET(scroll_view), 0, offset);
    }

    darray_deinit(&(children_for_layout));
  } else {
    scroll_bar_set_value(scroll_bar, 0);
    scroll_view_set_offset(WIDGET(scroll_view), 0, 0);
  }

  scroll_view_set_virtual_h(list_view->scroll_view, virtual_h);
  item_height = tk_max(item_height, default_item_height);
  scroll_bar_set_params(list_view->scroll_bar, virtual_h, item_height);
  scroll_view->xoffset = 0;
  if (scroll_view->yoffset + widget->h > scroll_view->virtual_h) {
    scroll_view->yoffset = scroll_view->virtual_h - widget->h;
    scroll_view->yoffset = scroll_view->yoffset > 0 ? scroll_view->yoffset : 0;
  }
  if (scroll_view->on_scroll) {
    scroll_view->on_scroll(widget, scroll_view->xoffset, scroll_view->yoffset);
  }

  scroll_view_set_xslidable(list_view->scroll_view, FALSE);
  if (scroll_bar_is_mobile(list_view->scroll_bar)) {
    scroll_view_set_yslidable(list_view->scroll_view, TRUE);
    widget_set_visible_only(list_view->scroll_bar, FALSE);
  }

  return RET_OK;
}

static bool_t children_layouter_list_view_is_valid(children_layouter_t* layouter) {
  return TRUE;
}

static ret_t children_layouter_list_view_destroy(children_layouter_t* layouter) {
  children_layouter_list_view_t* l = (children_layouter_list_view_t*)layouter;
  str_reset(&(layouter->params));
  TKMEM_FREE(l);

  return RET_OK;
}

static children_layouter_t* children_layouter_list_view_clone(children_layouter_t* layouter) {
  children_layouter_list_view_t* l = TKMEM_ZALLOC(children_layouter_list_view_t);

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

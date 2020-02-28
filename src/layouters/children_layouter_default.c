/**
 * File:   children_layouter_default_default.c
 * Author: AWTK Develop Team
 * Brief:  children layouter default
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

#include "tkc/rect.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/widget.h"
#include "tkc/tokenizer.h"
#include "layouters/self_layouter_default.h"
#include "layouters/children_layouter_default.h"

static const char* children_layouter_default_to_string(children_layouter_t* layouter) {
  char temp[32];
  str_t* str = &(layouter->params);
  children_layouter_default_t* layout = (children_layouter_default_t*)layouter;

  str_set(str, "default(");
  if (layout->cols_is_width) {
    tk_snprintf(temp, sizeof(temp) - 1, "w=%d,", (int)(layout->cols));
  } else {
    tk_snprintf(temp, sizeof(temp) - 1, "c=%d,", (int)(layout->cols));
  }
  str_append(str, temp);

  if (layout->rows_is_height) {
    tk_snprintf(temp, sizeof(temp) - 1, "h=%d,", (int)(layout->rows));
  } else {
    tk_snprintf(temp, sizeof(temp) - 1, "r=%d,", (int)(layout->rows));
  }
  str_append(str, temp);

  if (layout->x_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "x=%d,", (int)(layout->x_margin));
    str_append(str, temp);
  }

  if (layout->y_margin) {
    tk_snprintf(temp, sizeof(temp) - 1, "y=%d,", (int)(layout->y_margin));
    str_append(str, temp);
  }

  if (layout->spacing) {
    tk_snprintf(temp, sizeof(temp) - 1, "s=%d,", (int)(layout->spacing));
    str_append(str, temp);
  }

  if (!(layout->keep_disable)) {
    str_append(str, "keep_disable=false,");
  }

  if (layout->keep_invisible) {
    str_append(str, "keep_invisible=true,");
  }

  if (layout->align_h == ALIGN_H_CENTER) {
    str_append(str, "align_h=center,");
  } else if (layout->align_h == ALIGN_H_RIGHT) {
    str_append(str, "align_h=right,");
  }

  str_trim_right(str, ",");
  str_append(str, ")");

  return str->str;
}

static ret_t children_layouter_default_set_param(children_layouter_t* layouter, const char* name,
                                                 const value_t* v) {
  int val = value_int(v);
  children_layouter_default_t* l = (children_layouter_default_t*)layouter;

  switch (*name) {
    case 'w': {
      l->cols = val;
      l->cols_is_width = TRUE;
      break;
    }
    case 'h': {
      l->rows = val;
      l->rows_is_height = TRUE;
      break;
    }
    case 'c': {
      l->cols = val;
      l->cols_is_width = FALSE;
      break;
    }
    case 'r': {
      l->rows = val;
      l->rows_is_height = FALSE;
      break;
    }
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
    case 'a': {
      const char* align_h = value_str(v);
      return_value_if_fail(align_h != NULL, RET_BAD_PARAMS);

      if (*align_h == 'r') {
        l->align_h = ALIGN_H_RIGHT;
      } else if (*align_h == 'c') {
        l->align_h = ALIGN_H_CENTER;
      } else {
        l->align_h = ALIGN_H_LEFT;
      }
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

static ret_t children_layouter_default_get_param(children_layouter_t* layouter, const char* name,
                                                 value_t* v) {
  children_layouter_default_t* l = (children_layouter_default_t*)layouter;

  switch (*name) {
    case 'w': {
      if (l->cols_is_width) {
        value_set_int(v, l->cols);
        return RET_OK;
      } else {
        return RET_FAIL;
      }
    }
    case 'h': {
      if (l->rows_is_height) {
        value_set_int(v, l->rows);
        return RET_OK;
      } else {
        return RET_FAIL;
      }
    }
    case 'c': {
      if (!l->cols_is_width) {
        value_set_int(v, l->cols);
        return RET_OK;
      } else {
        return RET_FAIL;
      }
    }
    case 'r': {
      if (!l->rows_is_height) {
        value_set_int(v, l->rows);
        return RET_OK;
      } else {
        return RET_FAIL;
      }
    }
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
    case 'a': {
      value_set_int(v, l->align_h);
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

static ret_t children_layouter_default_layout(children_layouter_t* layouter, widget_t* widget) {
  wh_t w = 0;
  wh_t h = 0;
  xy_t x = 0;
  xy_t y = 0;
  uint32_t i = 0;
  uint32_t n = 0;
  uint32_t rows = 0;
  uint32_t cols = 0;
  uint8_t spacing = 0;
  int32_t layout_w = 0;
  int32_t layout_h = 0;
  uint8_t x_margin = 0;
  uint8_t y_margin = 0;
  rect_t area = {0, 0, 0, 0};
  widget_t* iter = NULL;
  widget_t** children = NULL;
  darray_t children_for_layout;
  children_layouter_default_t* layout = (children_layouter_default_t*)layouter;

  if (widget->children == NULL) {
    return RET_OK;
  }

  widget_layout_floating_children(widget);
  darray_init(&children_for_layout, widget->children->size, NULL, NULL);
  return_value_if_fail(
      widget_get_children_for_layout(widget, &children_for_layout, layout->keep_disable,
                                     layout->keep_invisible) == RET_OK,
      RET_BAD_PARAMS);

  n = children_for_layout.size;
  if (children_for_layout.size < 1) {
    darray_deinit(&(children_for_layout));
    return RET_OK;
  }

  children = (widget_t**)(children_for_layout.elms);
  layout_w = widget_get_prop_int(widget, WIDGET_PROP_LAYOUT_W, widget->w);
  layout_h = widget_get_prop_int(widget, WIDGET_PROP_LAYOUT_H, widget->h);

  x = layout->x_margin;
  y = layout->y_margin;
  x_margin = layout->x_margin;
  y_margin = layout->y_margin;
  spacing = layout->spacing;

  if (layout->rows_is_height) {
    rows = tk_roundi((layout_h - 2.0f * y_margin + spacing) / (layout->rows + spacing));
  } else {
    rows = layout->rows;
  }

  if (layout->cols_is_width) {
    cols = tk_roundi((layout_w - 2.0f * x_margin + spacing) / (layout->cols + spacing));
  } else {
    cols = layout->cols;
  }

  if (rows == 1 && cols == 0) { /*hbox*/
    uint32_t xoffset = x;
    uint32_t children_w = 0;
    h = layout_h - 2 * y_margin;
    w = layout_w - 2 * x_margin - (n - 1) * spacing;

    return_value_if_fail(h > 0 && w > 0, RET_BAD_PARAMS);

    area = rect_init(0, 0, w, h);
    for (i = 0; i < n; i++) {
      iter = children[i];
      if (self_layouter_default_is_valid(iter->self_layout)) {
        widget_layout_self_with_rect(iter->self_layout, iter, &area);
      }
    }

    for (i = 0; i < n; i++) {
      iter = children[i];
      children_w += iter->w + spacing;
    }
    children_w -= spacing;

    switch (layout->align_h) {
      case ALIGN_H_RIGHT: {
        xoffset = layout_w - x_margin - children_w;
        break;
      }
      case ALIGN_H_CENTER: {
        xoffset = (layout_w - children_w) / 2;
        break;
      }
      default:
        break;
    }

    x = xoffset;
    for (i = 0; i < n; i++) {
      iter = children[i];
      widget_move_resize(iter, x, y, iter->w, h);
      x += iter->w + spacing;
    }

    for (i = 0; i < n; i++) {
      widget_layout_children(children[i]);
    }
  } else if (cols == 1 && rows == 0) { /*vbox*/
    w = layout_w - 2 * x_margin;
    h = layout_h - 2 * y_margin - (n - 1) * spacing;
    return_value_if_fail(w > 0 && h > 0, RET_BAD_PARAMS);

    area = rect_init(0, 0, w, h);
    for (i = 0; i < n; i++) {
      iter = children[i];
      if (self_layouter_default_is_valid(iter->self_layout)) {
        widget_layout_self_with_rect(iter->self_layout, iter, &area);
      }
    }

    for (i = 0; i < n; i++) {
      iter = children[i];
      widget_move_resize(iter, x, y, w, iter->h);
      y += iter->h + spacing;
    }

    for (i = 0; i < n; i++) {
      widget_layout_children(children[i]);
    }
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

      area = rect_init(x, y, item_w, item_h);
      widget_move_resize(iter, x, y, item_w, item_h);
      if (self_layouter_default_is_valid(iter->self_layout)) {
        if (self_layouter_get_param_int(iter->self_layout, "x_attr", 0) == X_ATTR_UNDEF) {
          self_layouter_set_param_str(iter->self_layout, "x", "0");
        }

        if (self_layouter_get_param_int(iter->self_layout, "y_attr", 0) == Y_ATTR_UNDEF) {
          self_layouter_set_param_str(iter->self_layout, "y", "0");
        }

        widget_layout_self_with_rect(iter->self_layout, iter, &area);
      }

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
  } else { /*not support*/
    log_debug("not supported(rows=%d, cols=%d)\n", rows, cols);
  }

  darray_deinit(&(children_for_layout));

  return RET_OK;
}

static bool_t children_layouter_default_is_valid(children_layouter_t* layouter) {
  children_layouter_default_t* l = (children_layouter_default_t*)layouter;

  return (l->rows == 0 && l->cols == 0) ? FALSE : TRUE;
}

static ret_t children_layouter_default_destroy(children_layouter_t* layouter) {
  children_layouter_default_t* l = (children_layouter_default_t*)layouter;
  str_reset(&(layouter->params));
  TKMEM_FREE(l);

  return RET_OK;
}

static children_layouter_t* children_layouter_default_clone(children_layouter_t* layouter) {
  children_layouter_default_t* l = TKMEM_ZALLOC(children_layouter_default_t);

  memcpy(l, layouter, sizeof(*l));
  str_init(&(l->layouter.params), 0);
  str_set(&(l->layouter.params), layouter->params.str);

  return (children_layouter_t*)l;
}

static const children_layouter_vtable_t s_children_layouter_default_vtable = {
    .type = "default",
    .clone = children_layouter_default_clone,
    .to_string = children_layouter_default_to_string,
    .get_param = children_layouter_default_get_param,
    .set_param = children_layouter_default_set_param,
    .layout = children_layouter_default_layout,
    .is_valid = children_layouter_default_is_valid,
    .destroy = children_layouter_default_destroy};

children_layouter_t* children_layouter_default_create(void) {
  children_layouter_t* l = NULL;
  children_layouter_default_t* layouter = NULL;

  layouter = TKMEM_ZALLOC(children_layouter_default_t);
  return_value_if_fail(layouter != NULL, NULL);

  layouter->keep_disable = TRUE;
  l = (children_layouter_t*)layouter;

  str_init(&(l->params), 0);
  l->vt = &s_children_layouter_default_vtable;
  return (children_layouter_t*)layouter;
}

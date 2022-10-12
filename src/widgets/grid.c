/**
 * File:   grid.c
 * Author: AWTK Develop Team
 * Brief:  网格。
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-06-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "tkc/func_call_parser.h"

#include "grid.h"
#include "base/layout.h"

typedef struct _column_definition_t {
  double w;
  uint8_t left_margin;
  uint8_t right_margin;
  uint8_t top_margin;
  uint8_t bottom_margin;
  bool_t fill_available;

  /*用于临时存储*/
  uint32_t real_w;
} column_definition_t;

typedef struct _column_definition_parser_t {
  func_call_parser_t parser;
  column_definition_t* definition;
} column_definition_parser_t;

static ret_t column_definition_on_param(func_call_parser_t* parser, const char* name,
                                        const char* value) {
  column_definition_parser_t* p = (column_definition_parser_t*)parser;
  switch (*name) {
    case 'w': {
      p->definition->w = tk_atof(value);
      break;
    }
    case 'l': {
      p->definition->left_margin = tk_atoi(value);
      break;
    }
    case 'r': {
      p->definition->right_margin = tk_atoi(value);
      break;
    }
    case 't': {
      p->definition->top_margin = tk_atoi(value);
      break;
    }
    case 'b': {
      p->definition->bottom_margin = tk_atoi(value);
      break;
    }
    case 'f': {
      p->definition->fill_available = tk_atob(value);
      break;
    }
    case 'm': {
      int32_t margin = tk_atoi(value);
      p->definition->top_margin = margin;
      p->definition->bottom_margin = margin;
      p->definition->left_margin = margin;
      p->definition->right_margin = margin;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static column_definition_t* column_definition_create(const char* str) {
  column_definition_parser_t p;
  column_definition_t* definition = TKMEM_ZALLOC(column_definition_t);
  return_value_if_fail(definition != NULL, NULL);

  func_call_parser_init(&(p.parser), str, tk_strlen(str));
  p.parser.on_name = NULL;
  p.parser.on_done = NULL;
  p.definition = definition;
  p.parser.on_param = column_definition_on_param;

  func_call_parser_parse(&(p.parser));
  func_call_parser_deinit(&(p.parser));

  if (!(definition->w != 0 || definition->fill_available)) {
    TKMEM_FREE(definition);
  }

  return definition;
}

static ret_t column_definition_destroy(column_definition_t* definition) {
  TKMEM_FREE(definition);
  return RET_OK;
}

static ret_t grid_parse_columns_definition(widget_t* widget, const char* definitions) {
  tokenizer_t t;
  grid_t* grid = GRID(widget);

  return_value_if_fail(tokenizer_init(&t, definitions, strlen(definitions), ";") != NULL,
                       RET_BAD_PARAMS);

  while (tokenizer_has_more(&t)) {
    const char* iter = tokenizer_next(&t);
    column_definition_t* col = column_definition_create(iter);
    break_if_fail(col != NULL);
    if (darray_push(&(grid->cols_definition), col) != RET_OK) {
      column_definition_destroy(col);
      break;
    }
  }
  tokenizer_deinit(&t);

  return RET_OK;
}

ret_t grid_set_rows(widget_t* widget, uint32_t rows) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL, RET_BAD_PARAMS);

  grid->rows = rows;

  return RET_OK;
}

ret_t grid_set_columns_definition(widget_t* widget, const char* columns_definition) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL, RET_BAD_PARAMS);

  grid->columns_definition = tk_str_copy(grid->columns_definition, columns_definition);
  darray_clear(&(grid->cols_definition));

  if (grid->columns_definition != NULL) {
    grid_parse_columns_definition(widget, grid->columns_definition);
  }

  return RET_OK;
}

ret_t grid_set_show_grid(widget_t* widget, bool_t show_grid) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL, RET_BAD_PARAMS);

  grid->show_grid = show_grid;

  return RET_OK;
}

static ret_t grid_get_prop(widget_t* widget, const char* name, value_t* v) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(WIDGET_PROP_ROWS, name)) {
    value_set_uint32(v, grid->rows);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_COLUMNS_DEFINITION, name)) {
    value_set_str(v, grid->columns_definition);
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_SHOW_GRID, name)) {
    value_set_bool(v, grid->show_grid);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t grid_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(WIDGET_PROP_ROWS, name)) {
    grid_set_rows(widget, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_COLUMNS_DEFINITION, name)) {
    grid_set_columns_definition(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(WIDGET_PROP_SHOW_GRID, name)) {
    grid_set_show_grid(widget, value_bool(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t grid_on_destroy(widget_t* widget) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(widget != NULL && grid != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(grid->columns_definition);
  darray_deinit(&(grid->cols_definition));

  return RET_OK;
}

static ret_t grid_on_paint_self(widget_t* widget, canvas_t* c) {
  grid_t* grid = GRID(widget);

  (void)grid;

  return RET_OK;
}

static ret_t grid_on_event(widget_t* widget, event_t* e) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(widget != NULL && grid != NULL, RET_BAD_PARAMS);

  (void)grid;

  return RET_OK;
}

static ret_t grid_on_layout_children_impl(widget_t* widget) {
  uint32_t i = 0;
  int32_t tw = 0;
  uint32_t cols = 0;
  int32_t ox = 0;
  int32_t row_height = 0;
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL && grid->rows > 0, RET_BAD_PARAMS);
  return_value_if_fail(grid->cols_definition.size > 0, RET_BAD_PARAMS);

  cols = grid->cols_definition.size;
  row_height = widget->h / grid->rows;

  for (i = 0; i < cols; i++) {
    column_definition_t* def = (column_definition_t*)darray_get(&(grid->cols_definition), i);
    bool_t negative = def->w < 0;
    double w = tk_abs(def->w);

    if (w <= 1) {
      def->real_w = w * widget->w;
    } else {
      def->real_w = w;
    }

    if (negative) {
      def->real_w = widget->w - def->real_w;
    }

    tw += def->real_w;
  }

  log_if_fail(tw <= widget->w);

  if (tw < widget->w) {
    /*把剩余宽度加到fill_available=true的列上，只允许一列指定fill_available=true*/
    int32_t remain_w = widget->w - tw;
    for (i = 0; i < cols; i++) {
      column_definition_t* def = (column_definition_t*)darray_get(&(grid->cols_definition), i);
      if (def->fill_available) {
        def->real_w += remain_w;
        break;
      }
    }
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  uint32_t row = i / cols;
  uint32_t col = i % cols;
  column_definition_t* def = (column_definition_t*)darray_get(&(grid->cols_definition), col);

  int32_t x = def->left_margin + ox;
  int32_t y = def->top_margin + row * row_height;
  int32_t w = def->real_w - def->left_margin - def->right_margin;
  int32_t h = row_height - def->top_margin - def->bottom_margin;

  log_if_fail(w > 0 && h > 0);
  log_if_fail(w <= widget->w);
  log_if_fail(h <= row_height);

  widget_move_resize_ex(iter, x, y, w, h, FALSE);
  if (iter->self_layout) {
    rect_t area = rect_init(x, y, w, h);
    self_layouter_layout(iter->self_layout, iter, &area);
  }
  widget_layout_children(iter);

  if ((col + 1) == cols) {
    ox = 0;
  } else {
    ox += def->real_w;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t grid_on_layout_children(widget_t* widget) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children_layout != NULL) {
    widget_layout_children_default(widget);
  } else if (grid->rows > 0 && grid->columns_definition != NULL) {
    grid_on_layout_children_impl(widget);
  }

  return RET_OK;
}

const char* s_grid_properties[] = {WIDGET_PROP_ROWS, WIDGET_PROP_COLUMNS_DEFINITION,
                                   WIDGET_PROP_SHOW_GRID, NULL};

static ret_t grid_draw_grid(widget_t* widget, canvas_t* c) {
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t row = 0;
  uint32_t col = 0;
  uint32_t cols = 0;
  int32_t row_height = 0;
  grid_t* grid = GRID(widget);
  color_t grid_color = color_init(0, 0, 0, 0xff);
  return_value_if_fail(grid != NULL && grid->rows > 0, RET_BAD_PARAMS);

  if (grid->cols_definition.size > 0) {
    grid_color = style_get_color(widget->astyle, STYLE_ID_GRID_COLOR, grid_color);
    cols = grid->cols_definition.size;
    row_height = widget->h / grid->rows;

    canvas_set_stroke_color(c, grid_color);
    for (row = 0; row < grid->rows; row++) {
      if (row) {
        canvas_draw_hline(c, 0, y, widget->w);
      }
      y += row_height;
    }

    for (col = 0; col < cols; col++) {
      column_definition_t* def = (column_definition_t*)darray_get(&(grid->cols_definition), col);
      if (col) {
        canvas_draw_vline(c, x, 0, widget->h);
      }
      x += def->real_w;
    }
  }

  return RET_OK;
}

static ret_t grid_on_paint_border(widget_t* widget, canvas_t* c) {
  grid_t* grid = GRID(widget);

  if (grid->show_grid && widget->astyle != NULL) {
    rect_t r = rect_init(0, 0, widget->w, widget->h);

    grid_draw_grid(widget, c);
    widget_stroke_border_rect(widget, c, &r);
  }

  return RET_OK;
}

static ret_t grid_on_paint_background_impl(widget_t* widget, canvas_t* c) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL && grid->rows > 0, RET_BAD_PARAMS);
  return_value_if_fail(grid->cols_definition.size > 0, RET_BAD_PARAMS);

  if (widget->astyle != NULL) {
    uint32_t i = 0;
    uint32_t row_height = widget->h / grid->rows;

    color_t trans = color_init(0, 0, 0, 0);
    color_t bg = style_get_color(widget->astyle, STYLE_ID_BG_COLOR, trans);
    color_t odd_bg = style_get_color(widget->astyle, STYLE_ID_ODD_BG_COLOR, bg);
    color_t even_bg = style_get_color(widget->astyle, STYLE_ID_EVEN_BG_COLOR, bg);

    if (odd_bg.rgba.a == 0 && even_bg.rgba.a == 0) {
      return RET_OK;
    }

    for (i = 0; i < grid->rows; i++) {
      rect_t r = rect_init(0, i * row_height, widget->w, row_height);
      canvas_set_fill_color(c, ((i % 2) == 0) ? even_bg : odd_bg);
      canvas_fill_rect(c, r.x, r.y, r.w, r.h);
    }
  }

  return RET_OK;
}

static ret_t grid_on_paint_background(widget_t* widget, canvas_t* c) {
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL, RET_BAD_PARAMS);
  if (grid->rows > 0 && grid->cols_definition.size > 0) {
    grid_on_paint_background_impl(widget, c);
  } else {
    widget_draw_background(widget, c);
  }

  return RET_OK;
}

TK_DECL_VTABLE(grid) = {.size = sizeof(grid_t),
                        .type = WIDGET_TYPE_GRID,
                        .clone_properties = s_grid_properties,
                        .persistent_properties = s_grid_properties,
                        .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                        .create = grid_create,
                        .on_paint_self = grid_on_paint_self,
                        .set_prop = grid_set_prop,
                        .get_prop = grid_get_prop,
                        .on_event = grid_on_event,
                        .on_layout_children = grid_on_layout_children,
                        .on_paint_border = grid_on_paint_border,
                        .on_paint_background = grid_on_paint_background,
                        .on_destroy = grid_on_destroy};

widget_t* grid_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(grid), x, y, w, h);
  grid_t* grid = GRID(widget);
  return_value_if_fail(grid != NULL, NULL);

  grid->rows = 3;
  grid->show_grid = FALSE;
  grid->columns_definition = NULL;
  darray_init(&(grid->cols_definition), 5, (tk_destroy_t)column_definition_destroy, NULL);

  return widget;
}

widget_t* grid_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, grid), NULL);

  return widget;
}

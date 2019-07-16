/**
 * File:   tab_button_group.h
 * Author: AWTK Develop Team
 * Brief:  tab_button_group
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/tab_button_group.h"

static ret_t tab_button_group_on_layout_children_non_compact(widget_t* widget) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = widget->h;
  int32_t nr = widget->children->size;
  int32_t item_w = widget->w / nr;
  int32_t first_w = widget->w - (nr - 1) * item_w;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  w = i == 0 ? first_w : item_w;
  widget_move_resize(iter, x, y, w, h);
  x += w;
  widget_layout_children(iter);
  WIDGET_FOR_EACH_CHILD_END();

  hscrollable_set_xoffset(tab_button_group->hscrollable, 0);
  hscrollable_set_virtual_w(tab_button_group->hscrollable, widget->w);
  hscrollable_set_always_scrollable(tab_button_group->hscrollable, FALSE);

  return RET_OK;
}

static ret_t tab_button_group_on_layout_children_compact(widget_t* widget) {
  value_t v;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = widget->h;
  widget_t* active = NULL;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  iter->h = h;
  if (widget_get_prop(iter, WIDGET_PROP_MIN_W, &v) == RET_OK) {
    w = value_int(&v);
  } else {
    w = iter->w;
  }

  widget_move_resize(iter, x, y, w, h);
  widget_layout_children(iter);
  x += w;
  if (widget_get_value(iter)) {
    active = iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  hscrollable_set_xoffset(tab_button_group->hscrollable, 0);
  hscrollable_set_virtual_w(tab_button_group->hscrollable, x - 1);
  hscrollable_set_always_scrollable(tab_button_group->hscrollable, FALSE);

  if (active != NULL) {
    widget_ensure_visible_in_viewport(active);
  }

  return RET_OK;
}

static ret_t tab_button_group_on_layout_children(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  if (widget->children && widget->children->size > 0) {
    if (tab_button_group->compact) {
      return tab_button_group_on_layout_children_compact(widget);
    } else {
      return tab_button_group_on_layout_children_non_compact(widget);
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static widget_t* tab_button_group_get_pages(widget_t* widget) {
  return_value_if_fail(widget && widget->parent, NULL);

  return widget_lookup_by_type(widget->parent, WIDGET_TYPE_PAGES, TRUE);
}

static ret_t tab_button_group_on_paint_active_button(widget_t* widget, widget_t* button,
                                                     canvas_t* c) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t bd = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  widget_t* pages = tab_button_group_get_pages(widget);
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  widget_t* last_child = widget_get_child(widget, widget_count_children(widget) - 1);

  if (pages != NULL) {
    int32_t y = 0;
    int32_t delta = 0;
    int32_t w = button->w - 2;
    int32_t x = button->x + 1 - tab_button_group->hscrollable->xoffset;

    if ((pages->y + pages->h) > widget->y) {
      y = button->y + button->h;
      delta = -1;
    } else {
      delta = 1;
      y = button->y;
    }

    canvas_set_stroke_color(c, fg);
    if (fg.rgba.a) {
#ifdef WITH_NANOVG_GPU
      vgcanvas_t* vg = canvas_get_vgcanvas(c);
      vgcanvas_translate(vg, c->ox, c->oy);
      vgcanvas_set_fill_color(vg, fg);
      vgcanvas_begin_path(vg);
      vgcanvas_rect(vg, x - 0.5, y - 1, w + 1, 2);
      vgcanvas_fill(vg);
      vgcanvas_begin_path(vg);
      vgcanvas_translate(vg, -c->ox, -c->oy);
#else
      canvas_draw_hline(c, x, y, w);
      canvas_draw_hline(c, x, y + delta, w);
#endif
    }

    if (bd.rgba.a) {
      canvas_set_stroke_color(c, bd);

      if (tab_button_group->compact) {
        if (last_child != NULL) {
          int32_t right = last_child->x + last_child->w;
          if (widget->w > right) {
            canvas_draw_hline(c, right, y, widget->w - right);
          }
        }
      } else {
        if (x > 1) {
          canvas_draw_hline(c, 0, y, x);
        }

        if (widget->w > (x + w)) {
          canvas_draw_hline(c, x + w, y, widget->w - x - w);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_on_paint_border(widget_t* widget, canvas_t* c) {
  hscrollable_t* hscrollable = NULL;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  hscrollable = tab_button_group->hscrollable;

  if (tab_button_group->compact && tab_button_group->scrollable) {
    if (hscrollable->virtual_w > widget->w) {
      rect_t r = rect_init(0, 0, widget->w, widget->h);
      return widget_stroke_border_rect(widget, c, &r);
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_on_paint_end(widget_t* widget, canvas_t* c) {
  value_t v;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_get_prop(iter, WIDGET_PROP_VALUE, &v) == RET_OK && value_bool(&v)) {
    return tab_button_group_on_paint_active_button(widget, iter, c);
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t tab_button_group_get_prop(widget_t* widget, const char* name, value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    value_set_bool(v, tab_button_group->compact);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCROLLABLE)) {
    value_set_bool(v, tab_button_group->scrollable);
    return RET_OK;
  }

  return hscrollable_get_prop(tab_button_group->hscrollable, name, v);
}

static ret_t tab_button_group_set_prop(widget_t* widget, const char* name, const value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    tab_button_group_set_compact(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SCROLLABLE)) {
    tab_button_group_set_scrollable(widget, value_bool(v));
    return RET_OK;
  }

  return hscrollable_set_prop(tab_button_group->hscrollable, name, v);
}

static ret_t tab_button_group_invalidate(widget_t* widget, rect_t* r) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  return hscrollable_invalidate(tab_button_group->hscrollable, r);
}

static ret_t tab_button_group_on_paint_children(widget_t* widget, canvas_t* c) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  return hscrollable_on_paint_children(tab_button_group->hscrollable, c);
}

static ret_t tab_button_group_on_event(widget_t* widget, event_t* e) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  if (tab_button_group->scrollable && tab_button_group->compact) {
    return hscrollable_on_event(tab_button_group->hscrollable, e);
  } else {
    return RET_OK;
  }
}

ret_t tab_button_group_set_compact(widget_t* widget, bool_t compact) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->compact = compact;

  return RET_OK;
}

ret_t tab_button_group_set_scrollable(widget_t* widget, bool_t scrollable) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->scrollable = scrollable;

  return RET_OK;
}

static ret_t tab_button_group_on_destroy(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(widget != NULL && tab_button_group != NULL, RET_BAD_PARAMS);

  hscrollable_destroy(tab_button_group->hscrollable);

  return RET_OK;
}

TK_DECL_VTABLE(tab_button_group) = {.size = sizeof(tab_button_group_t),
                                    .type = WIDGET_TYPE_TAB_BUTTON_GROUP,
                                    .scrollable = TRUE,
                                    .parent = TK_PARENT_VTABLE(widget),
                                    .create = tab_button_group_create,
                                    .set_prop = tab_button_group_set_prop,
                                    .get_prop = tab_button_group_get_prop,
                                    .invalidate = tab_button_group_invalidate,
                                    .on_event = tab_button_group_on_event,
                                    .on_destroy = tab_button_group_on_destroy,
                                    .on_paint_children = tab_button_group_on_paint_children,
                                    .on_layout_children = tab_button_group_on_layout_children,
                                    .on_paint_border = tab_button_group_on_paint_border,
                                    .on_paint_end = tab_button_group_on_paint_end,
                                    .on_paint_self = tab_button_group_on_paint_self};

widget_t* tab_button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(tab_button_group), x, y, w, h);
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(widget != NULL, NULL);

  tab_button_group->hscrollable = hscrollable_create(widget);

  return widget;
}

widget_t* tab_button_group_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, tab_button_group), NULL);

  return widget;
}

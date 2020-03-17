/**
 * File:   slide_menu.h
 * Author: AWTK Develop Team
 * Brief:  slide_menu
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
 * 2018-12-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "widgets/button.h"
#include "base/layout.h"
#include "base/widget_vtable.h"
#include "slide_menu/slide_menu.h"
#include "widget_animators/widget_animator_scroll.h"

#define MAX_VISIBLE_NR 10

const char* s_slide_menu_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_ALIGN_V,
                                         SLIDE_MENU_PROP_MIN_SCALE, NULL};

static ret_t slide_menu_set_xoffset(slide_menu_t* slide_menu, int32_t xoffset) {
  if (slide_menu->xoffset != xoffset) {
    slide_menu->xoffset = xoffset;
    widget_layout_children(WIDGET(slide_menu));
    widget_invalidate(WIDGET(slide_menu), NULL);
  }

  return RET_OK;
}

int32_t slide_menu_fix_index(widget_t* widget, int32_t index) {
  uint32_t nr = widget_count_children(widget);

  while (index < 0) {
    index += nr;
  }

  return (index % nr);
}

static widget_t* slide_menu_get_child(widget_t* widget, int32_t index) {
  widget_t** children = NULL;
  return_value_if_fail(widget != NULL && widget->children, NULL);

  children = (widget_t**)(widget->children->elms);
  return_value_if_fail(children != NULL, NULL);

  return children[slide_menu_fix_index(widget, index)];
}

static widget_t* slide_menu_find_target(widget_t* widget, xy_t x, xy_t y) {
  widget_t* current = NULL;
  int32_t r = 0;
  int32_t b = 0;
  int32_t xx = 0;
  int32_t yy = 0;
  point_t p = {x, y};
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(widget != NULL && slide_menu != NULL, NULL);

  if (widget->grab_widget != NULL) {
    return widget->grab_widget;
  }

  widget_to_local(widget, &p);
  current = slide_menu_get_child(widget, slide_menu->value);
  return_value_if_fail(current != NULL, NULL);

  r = current->x + current->w;
  b = current->y + current->h;
  xx = p.x;
  yy = p.y;

  if (current->enable && xx >= current->x && yy >= current->y && xx <= r && yy <= b) {
    return current;
  }

  return NULL;
}

static int32_t slide_menu_get_visible_nr(widget_t* widget) {
  if (widget->w == 0 || widget->h == 0) return 0;
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  int32_t n = (widget->w - widget->h) / (slide_menu->min_scale * widget->h) + 1;

  n = tk_min(n, MAX_VISIBLE_NR);
  n = tk_min(n, widget_count_children(widget));

  return n > 0 ? n : 0;
}

static rect_t slide_menu_get_clip_r(widget_t* widget) {
  int32_t x = 0;
  int32_t w = 0;
  int32_t h = widget->h;
  int32_t nr = slide_menu_get_visible_nr(widget) - 1;
  slide_menu_t* slide_menu = SLIDE_MENU(widget);

  nr = tk_max(1, nr);
  if (nr > 0 && (nr % 2) != 0) {
    nr--;
    /*keep nr is odd*/
  }

  w = h + h * nr * slide_menu->min_scale;
  x = tk_roundi((widget->w - w) / 2.0f);

  return rect_init(x, 0, w, h);
}

static ret_t slide_menu_paint_children(widget_t* widget, canvas_t* c) {
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  int32_t left = c->ox + iter->x;
  int32_t right = left + iter->w;

  if (left >= (c->clip_right - 1) || right <= (c->clip_left + 1)) {
    iter->dirty = FALSE;
    continue;
  }

  widget_paint(iter, c);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t slide_menu_paint_mask(widget_t* widget, canvas_t* c, rect_t* r) {
  int32_t i = 0;
  int32_t x = 0;
  int32_t n = r->w / 2 + 2;
  int32_t cx = widget->w / 2;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  easing_func_t easing = easing_get(EASING_LINEAR);
  color_t mask_color = style_get_color(style, STYLE_ID_MASK_COLOR, trans);

  if (mask_color.rgba.a) {
    for (i = 0; i < n; i++) {
      x = cx - i;

      if (x >= 0) {
        mask_color.rgba.a = 0xff * easing((float_t)i / (float_t)n);
        canvas_set_stroke_color(c, mask_color);
        canvas_draw_vline(c, x, 0, widget->h);
      }
    }

    for (i = 0; i < n; i++) {
      x = cx + i;

      if (x < widget->w) {
        mask_color.rgba.a = 0xff * easing((float_t)i / (float_t)n);
        canvas_set_stroke_color(c, mask_color);
        canvas_draw_vline(c, x, 0, widget->h);
      }
    }
  }

  return RET_OK;
}

static ret_t slide_menu_on_paint_children(widget_t* widget, canvas_t* c) {
  if (slide_menu_get_visible_nr(widget) >= 1) {
    rect_t r;
    rect_t save_r;
    rect_t clip_r = slide_menu_get_clip_r(widget);
    vgcanvas_t* vg = canvas_get_vgcanvas(c);

    clip_r.x += c->ox;
    clip_r.y += c->oy;
    canvas_get_clip_rect(c, &save_r);
    r = rect_intersect(&save_r, &clip_r);

    canvas_save(c);
    if (vg != NULL) {
      vgcanvas_save(vg);
      vgcanvas_clip_rect(vg, (float_t)r.x, (float_t)r.y, (float_t)r.w, (float_t)r.h);
    }

    canvas_set_clip_rect(c, &r);
    slide_menu_paint_children(widget, c);
    canvas_set_clip_rect(c, &save_r);

    if (vg != NULL) {
      vgcanvas_clip_rect(vg, (float_t)save_r.x, (float_t)save_r.y, (float_t)save_r.w,
                         (float_t)save_r.h);
      vgcanvas_restore(vg);
    }
    canvas_restore(c);

    slide_menu_paint_mask(widget, c, &clip_r);
  }

  return RET_OK;
}

static int32_t slide_menu_get_delta_index(widget_t* widget) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(widget != NULL && slide_menu != NULL, 0);

  return tk_roundi((float_t)(slide_menu->xoffset) / (float_t)(widget->h));
}

static uint32_t slide_menu_get_visible_children(widget_t* widget,
                                                widget_t* children[MAX_VISIBLE_NR]) {
  uint32_t i = 0;
  uint32_t curr = 0;
  int32_t max_size = widget->h;
  uint32_t nr = widget_count_children(widget);
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  int32_t delta_index = slide_menu_get_delta_index(widget);
  int32_t index = slide_menu_fix_index(widget, slide_menu->value - delta_index);
  int32_t rounded_xoffset = delta_index * max_size;
  int32_t xoffset = slide_menu->xoffset - rounded_xoffset;

  for (i = 0; i < MAX_VISIBLE_NR; i++) {
    children[i] = NULL;
  }

  for (i = 0; i < nr; i++) {
    widget_get_child(widget, i)->visible = FALSE;
  }

  nr = slide_menu_get_visible_nr(widget);
  if (nr == 0) return 0;

  if (xoffset >= 0) {
    curr = MAX_VISIBLE_NR / 2;
  } else {
    curr = MAX_VISIBLE_NR / 2 - 1;
  }

  children[curr] = slide_menu_get_child(widget, index);
  for (i = 1; i <= (nr - 1) / 2; i++) {
    children[curr - i] = slide_menu_get_child(widget, index - i);
    children[curr + i] = slide_menu_get_child(widget, index + i);
  }

  if (nr % 2 == 0) {
    if (xoffset >= 0) {
      children[curr - i] = slide_menu_get_child(widget, index - i);
    } else {
      children[curr + i] = slide_menu_get_child(widget, index + i);
    }
  }

  for (i = 0; i < MAX_VISIBLE_NR; i++) {
    widget_t* iter = children[i];
    if (iter != NULL) {
      iter->visible = TRUE;
      /*log_debug("nr=%d %d %s\n", nr, i, iter->name);*/
    }
  }

  return curr;
}

static int32_t slide_menu_calc_child_size(slide_menu_t* slide_menu, int32_t i, int32_t curr,
                                          int32_t xo) {
  float_t scale = 0;
  int32_t max_size = WIDGET(slide_menu)->h;
  float_t min_scale = slide_menu->min_scale;
  int32_t xoffset = xo + max_size * (i - curr);

  if (tk_abs(xoffset) < max_size) {
    scale = 1 + tk_abs(xoffset) * (min_scale - 1) / max_size;
  } else {
    scale = min_scale;
  }

  return max_size * scale;
}

static int32_t slide_menu_calc_child_y(align_v_t align_v, int32_t max_size, int32_t size) {
  int32_t y = 0;

  switch (align_v) {
    case ALIGN_V_TOP: {
      y = 0;
      break;
    }
    case ALIGN_V_MIDDLE: {
      y = (max_size - size) / 2;
      break;
    }
    default: {
      y = max_size - size;
      break;
    }
  }

  return y;
}

static ret_t slide_menu_do_layout_children(widget_t* widget) {
  int32_t i = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t size = 0;
  widget_t* iter = NULL;
  int32_t max_size = widget->h;
  int32_t visible_nr = MAX_VISIBLE_NR;
  widget_t* children[MAX_VISIBLE_NR];
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  uint32_t curr = slide_menu_get_visible_children(widget, children);
  int32_t rounded_xoffset = slide_menu_get_delta_index(widget) * max_size;
  int32_t xoffset = slide_menu->xoffset - rounded_xoffset;

  /*curr widget*/
  iter = children[curr];
  size = slide_menu_calc_child_size(slide_menu, curr, curr, xoffset);
  x = (widget->w - max_size) / 2 + xoffset;
  y = slide_menu_calc_child_y(slide_menu->align_v, max_size, size);
  widget_move_resize(iter, x, y, size, size);

  /*left*/
  for (i = curr - 1; i >= 0; i--) {
    iter = children[i];
    if (iter == NULL) break;

    size = slide_menu_calc_child_size(slide_menu, i, curr, xoffset);
    x = children[i + 1]->x - size;
    y = slide_menu_calc_child_y(slide_menu->align_v, max_size, size);
    widget_move_resize(iter, x, y, size, size);
  }

  /*right*/
  for (i = curr + 1; i < visible_nr; i++) {
    iter = children[i];
    if (iter == NULL) break;

    size = slide_menu_calc_child_size(slide_menu, i, curr, xoffset);
    x = children[i - 1]->x + children[i - 1]->w;
    y = slide_menu_calc_child_y(slide_menu->align_v, max_size, size);
    widget_move_resize(iter, x, y, size, size);
  }

  return RET_OK;
}

static ret_t slide_menu_layout_children(widget_t* widget) {
  return_value_if_fail(widget->w >= 2 * widget->h, RET_BAD_PARAMS);

  if (widget_count_children(widget) > 0) {
    slide_menu_do_layout_children(widget);
  }

  return RET_OK;
}

static ret_t slide_menu_get_prop(widget_t* widget, const char* name, value_t* v) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, slide_menu->value);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_MENU_PROP_MIN_SCALE)) {
    value_set_float(v, slide_menu->min_scale);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ALIGN_V)) {
    value_set_int(v, slide_menu->align_v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, slide_menu->xoffset);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_menu_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    slide_menu_set_value(widget, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_MENU_PROP_MIN_SCALE)) {
    slide_menu_set_min_scale(widget, value_float(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    slide_menu_set_xoffset(slide_menu, value_int(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ALIGN_V)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = align_v_type_find(value_str(v));
      if (kv != NULL) {
        slide_menu_set_align_v(widget, (align_v_t)(kv->value));
      }
    } else {
      slide_menu_set_align_v(widget, (align_v_t)(value_int(v)));
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_menu_on_pointer_down(slide_menu_t* slide_menu, pointer_event_t* e) {
  velocity_t* v = &(slide_menu->velocity);

  slide_menu->xdown = e->x;

  velocity_reset(v);
  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t slide_menu_on_pointer_move(slide_menu_t* slide_menu, pointer_event_t* e) {
  velocity_t* v = &(slide_menu->velocity);

  velocity_update(v, e->e.time, e->x, e->y);
  slide_menu_set_xoffset(slide_menu, e->x - slide_menu->xdown);

  return RET_OK;
}

static ret_t slide_menu_set_value_only(slide_menu_t* slide_menu, int32_t index) {
  widget_t* widget = WIDGET(slide_menu);

  if (index != slide_menu->value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);

    slide_menu->value = index;

    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
  }

  return RET_OK;
}

static ret_t slide_menu_on_scroll_done(void* ctx, event_t* e) {
  int32_t index = 0;
  int32_t delta_index = 0;
  widget_t* widget = WIDGET(ctx);
  slide_menu_t* slide_menu = SLIDE_MENU(ctx);
  return_value_if_fail(widget != NULL && slide_menu != NULL && widget->children != NULL,
                       RET_BAD_PARAMS);

  delta_index = slide_menu_get_delta_index(widget);
  index = slide_menu_fix_index(widget, slide_menu->value - delta_index);

  slide_menu->wa = NULL;
  slide_menu->xoffset = 0;
  slide_menu_set_value_only(slide_menu, index);

  return RET_REMOVE;
}

static ret_t slide_menu_scroll_to(widget_t* widget, int32_t xoffset_end) {
  int32_t xoffset = 0;
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(slide_menu != NULL, RET_FAIL);

  xoffset = slide_menu->xoffset;
  if (xoffset == xoffset_end) {
    return RET_OK;
  }

  slide_menu->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
  return_value_if_fail(slide_menu->wa != NULL, RET_OOM);

  widget_animator_scroll_set_params(slide_menu->wa, xoffset, 0, xoffset_end, 0);
  widget_animator_on(slide_menu->wa, EVT_ANIM_END, slide_menu_on_scroll_done, slide_menu);
  widget_animator_start(slide_menu->wa);

  return RET_OK;
}

static ret_t slide_menu_on_pointer_up(slide_menu_t* slide_menu, pointer_event_t* e) {
  int32_t xoffset_end = 0;
  point_t p = {e->x, e->y};
  velocity_t* v = &(slide_menu->velocity);
  widget_t* widget = WIDGET(slide_menu);
  int32_t nr = widget_count_children(widget);

  velocity_update(v, e->e.time, e->x, e->y);
  if (!slide_menu->dragged) {
    widget_to_local(widget, &p);
    if (nr > 1) {
      int32_t right = (widget->w + widget->h) / 2;
      int32_t left = (widget->w - widget->h) / 2;
      float_t min_scale = slide_menu->min_scale;
      int32_t small_size = widget->h * min_scale;

      if (p.x > right) {
        int32_t delta = (p.x - right) / small_size + 1;

        xoffset_end = -widget->h * delta;
        xoffset_end = tk_roundi((float_t)xoffset_end / (float_t)(widget->h)) * widget->h;
      } else if (p.x < left) {
        int32_t delta = (left - p.x) / small_size + 1;

        xoffset_end = widget->h * delta;
        xoffset_end = tk_roundi((float_t)xoffset_end / (float_t)(widget->h)) * widget->h;
      }
    }

    if (xoffset_end == 0) {
      widget_invalidate(WIDGET(slide_menu), NULL);
      widget_layout_children(WIDGET(slide_menu));

      return RET_OK;
    }
  } else {
    xoffset_end = slide_menu->xoffset + v->xv;
    xoffset_end = tk_roundi((float_t)xoffset_end / (float_t)(widget->h)) * widget->h;
  }

  slide_menu_scroll_to(WIDGET(slide_menu), xoffset_end);

  return RET_OK;
}

static ret_t slide_menu_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(widget != NULL && slide_menu != NULL, RET_BAD_PARAMS);

  if (slide_menu->wa != NULL) {
    return RET_STOP;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
      slide_menu->dragged = FALSE;
      slide_menu->pressed = TRUE;
      widget_grab(widget->parent, widget);
      slide_menu_on_pointer_down(slide_menu, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      if (slide_menu->pressed) {
        slide_menu_on_pointer_up(slide_menu, (pointer_event_t*)e);
        ret = slide_menu->dragged ? RET_STOP : RET_OK;
        widget_ungrab(widget->parent, widget);
        slide_menu->pressed = FALSE;
        slide_menu->dragged = FALSE;
      }
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      if (slide_menu->pressed) {
        slide_menu->xoffset = 0;
        slide_menu->pressed = FALSE;
        slide_menu->dragged = FALSE;
        widget_ungrab(widget->parent, widget);
        log_debug("slide menu: EVT_POINTER_DOWN_ABORT\n");
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (slide_menu->dragged) {
        slide_menu_on_pointer_move(slide_menu, evt);
      } else if (evt->pressed && slide_menu->pressed) {
        int32_t delta = evt->x - slide_menu->xdown;

        if (tk_abs(delta) >= TK_DRAG_THRESHOLD) {
          pointer_event_t abort;
          pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, evt->x, evt->y);
          widget_dispatch_event_to_target_recursive(widget, (event_t*)(&abort));
          slide_menu->dragged = TRUE;
        }
      }
      ret = slide_menu->dragged ? RET_STOP : RET_OK;
      break;
    }
    default:
      break;
  }

  return ret;
}

TK_DECL_VTABLE(slide_menu) = {.size = sizeof(slide_menu_t),
                              .inputable = TRUE,
                              .type = WIDGET_TYPE_SLIDE_MENU,
                              .clone_properties = s_slide_menu_properties,
                              .persistent_properties = s_slide_menu_properties,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = slide_menu_create,
                              .set_prop = slide_menu_set_prop,
                              .get_prop = slide_menu_get_prop,
                              .find_target = slide_menu_find_target,
                              .on_paint_children = slide_menu_on_paint_children,
                              .on_layout_children = slide_menu_layout_children,
                              .on_event = slide_menu_on_event};

widget_t* slide_menu_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(slide_menu), x, y, w, h);
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(slide_menu != NULL, NULL);

  slide_menu->value = 1;
  slide_menu->min_scale = 0.8f;
  slide_menu->align_v = ALIGN_V_BOTTOM;

  return widget;
}

ret_t slide_menu_set_value(widget_t* widget, uint32_t index) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(slide_menu != NULL, RET_BAD_PARAMS);

  slide_menu_set_value_only(slide_menu, index);
  widget_layout_children(widget);

  return widget_invalidate(widget, NULL);
}

ret_t slide_menu_set_min_scale(widget_t* widget, float_t min_scale) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(slide_menu != NULL, RET_BAD_PARAMS);

  slide_menu->min_scale = tk_max(0.5f, tk_min(min_scale, 1));

  return widget_invalidate(widget, NULL);
}

ret_t slide_menu_set_align_v(widget_t* widget, align_v_t align_v) {
  slide_menu_t* slide_menu = SLIDE_MENU(widget);
  return_value_if_fail(slide_menu != NULL, RET_BAD_PARAMS);

  slide_menu->align_v = align_v;

  return widget_invalidate(widget, NULL);
}

widget_t* slide_menu_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slide_menu), NULL);

  return widget;
}

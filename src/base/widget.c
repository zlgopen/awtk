/**
 * File:   widget.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  basic class of all widget
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utf8.h"
#include "base/utils.h"
#include "base/enums.h"
#include "base/locale.h"
#include "base/widget.h"
#include "base/prop_names.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

ret_t widget_move(widget_t* widget, xy_t x, xy_t y) {
  event_t e = {EVT_MOVE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->x = x;
  widget->y = y;

  widget_invalidate(widget, NULL);
  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_resize(widget_t* widget, wh_t w, wh_t h) {
  event_t e = {EVT_RESIZE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->w = w;
  widget->h = h;

  widget_invalidate(widget, NULL);
  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h) {
  event_t e = {EVT_MOVE_RESIZE, widget};
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->x = x;
  widget->y = y;
  widget->w = w;
  widget->h = h;
  widget_invalidate(widget, NULL);
  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_set_value(widget_t* widget, uint32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_VALUE, value_set_uint32(&v, value));
}

ret_t widget_use_style(widget_t* widget, const char* value) {
  return_value_if_fail(widget != NULL && value != NULL, RET_BAD_PARAMS);

  widget->style_type = (uint8_t)atoi(value);
  widget_update_style(widget);

  return RET_OK;
}

ret_t widget_set_text(widget_t* widget, const wchar_t* text) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_wstr(&v, text));
}

ret_t widget_set_tr_text(widget_t* widget, const char* text) {
  value_t v;
  const char* tr_text = NULL;
  return_value_if_fail(widget != NULL && text != NULL, RET_OK);

  tr_text = locale_tr(locale(), text);
#ifdef WITH_DYNAMIC_TR
  str_set(&(widget->tr_key), text);
#endif /*WITH_DYNAMIC_TR*/

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
}

ret_t widget_re_translate_text(widget_t* widget) {
#ifdef WITH_DYNAMIC_TR
  if (widget->tr_key.size) {
    value_t v;
    const char* tr_text = locale_tr(locale(), widget->tr_key.str);
    widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
  }

  if (widget->children != NULL) {
    uint32_t i = 0;
    uint32_t n = 0;
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_re_translate_text(iter);
    }
  }
#else
  return RET_FAIL;
#endif /*WITH_DYNAMIC_TR*/
}

uint32_t widget_get_value(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, 0);

  return widget_get_prop(widget, WIDGET_PROP_VALUE, &v) == RET_OK ? value_int(&v) : 0;
}

const wchar_t* widget_get_text(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, 0);

  return widget_get_prop(widget, WIDGET_PROP_TEXT, &v) == RET_OK ? value_wstr(&v) : 0;
}

ret_t widget_set_name(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);
  str_set(&(widget->name), name);

  return RET_OK;
}

ret_t widget_set_enable(widget_t* widget, bool_t enable) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->enable != enable) {
    widget->enable = enable;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_focused(widget_t* widget, bool_t focused) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->focused != focused) {
    widget->focused = focused;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_state(widget_t* widget, widget_state_t state) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->state != state) {
    widget->state = state;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_destroy_children(widget_t* widget) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_destroy(iter);
    }
    widget->children->size = 0;
  }

  return RET_OK;
}

ret_t widget_add_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  child->parent = widget;
  if (widget->children == NULL) {
    widget->children = array_create(4);
  }

  return array_push(widget->children, child) ? RET_OK : RET_FAIL;
}

ret_t widget_remove_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (widget->target == child) {
    widget->target = NULL;
  }

  if (widget->key_target == child) {
    widget->key_target = NULL;
  }

  return array_remove(widget->children, NULL, child) ? RET_OK : RET_NOT_FOUND;
}

static widget_t* widget_lookup_child(widget_t* widget, const char* name) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (str_eq(&(iter->name), name)) {
        return iter;
      }
    }
  }

  return NULL;
}

static widget_t* widget_lookup_all(widget_t* widget, const char* name) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (str_eq(&(iter->name), name)) {
        return iter;
      } else {
        iter = widget_lookup_all(iter, name);
        if (iter != NULL) {
          return iter;
        }
      }
    }
  }

  return NULL;
}

widget_t* widget_lookup(widget_t* widget, const char* name, bool_t recursive) {
  if (recursive) {
    return widget_lookup_all(widget, name);
  } else {
    return widget_lookup_child(widget, name);
  }
}

static ret_t widget_set_visible_self(widget_t* widget, bool_t visible) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->visible != visible) {
    widget->visible = visible;
    widget_update_style(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

static ret_t widget_set_visible_recursive(widget_t* widget, bool_t visible) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->visible = visible;
  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_set_visible_recursive(iter, visible);
    }
  }

  return RET_OK;
}

ret_t widget_set_visible(widget_t* widget, bool_t visible, bool_t recursive) {
  if (recursive) {
    return widget_set_visible_recursive(widget, visible);
  } else {
    return widget_set_visible_self(widget, visible);
  }
}

widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y) {
  widget_t* ret = NULL;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->vt && widget->vt->find_target) {
    ret = widget->vt->find_target(widget, x, y);
  } else {
    ret = widget_find_target_default(widget, x, y);
  }

  return ret;
}

ret_t widget_dispatch(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->vt && widget->vt->on_event) {
    ret = widget->vt->on_event(widget, e);
  } else {
    ret = widget_on_event_default(widget, e);
  }

  if (widget->emitter != NULL) {
    ret = emitter_dispatch(widget->emitter, e);
  }

  return ret;
}

uint32_t widget_on(widget_t* widget, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  if (widget->emitter == NULL) {
    widget->emitter = emitter_create();
  }

  return emitter_on(widget->emitter, type, on_event, ctx);
}

uint32_t widget_child_on(widget_t* widget, const char* name, event_type_t type,
                         event_func_t on_event, void* ctx) {
  return widget_on(widget_lookup(widget, name, TRUE), type, on_event, ctx);
}

ret_t widget_off(widget_t* widget, uint32_t id) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->emitter != NULL, RET_BAD_PARAMS);

  return emitter_off(widget->emitter, id);
}

ret_t widget_off_by_func(widget_t* widget, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->emitter != NULL, RET_BAD_PARAMS);

  return emitter_off_by_func(widget->emitter, type, on_event, ctx);
}

ret_t widget_draw_icon_text(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  rect_t dst;
  bitmap_t img;
  style_t* style = &(widget->style);
  color_t trans = color_init(0, 0, 0, 0);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, 20);

  if (text == NULL) {
    text = &(widget->text);
  }

  if (icon == NULL) {
    icon = style_get_str(style, STYLE_ID_ICON, NULL);
  }

  if (text != NULL && text->size > 0) {
    color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
    const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);

    canvas_set_text_color(c, tc);
    canvas_set_font(c, font_name, font_size);
  }

  if (icon != NULL && image_manager_load(image_manager(), icon, &img) == RET_OK) {
    xy_t cx = 0;
    xy_t cy = 0;

    if (text != NULL && text->size > 0) {
      if (widget->h > (img.h + font_size)) {
        rect_init(dst, 0, 0, widget->w, widget->h - font_size);
        cx = dst.w >> 1;
        cy = dst.h >> 1;
        canvas_draw_icon(c, &img, cx, cy);

        w = canvas_measure_text(c, text->str, text->size);
        x = (widget->w - w) >> 1;
        y = widget->h - font_size;
        canvas_draw_text(c, text->str, text->size, x, y);
      } else {
        rect_init(dst, 0, 0, widget->h, widget->h);
        cx = dst.w >> 1;
        cy = dst.h >> 1;
        canvas_draw_icon(c, &img, cx, cy);

        x = widget->h + 2;
        y = (widget->h - font_size) >> 1;
        canvas_draw_text(c, text->str, text->size, x, y);
      }
    } else {
      rect_init(dst, 0, 0, widget->w, widget->h);
      cx = dst.w >> 1;
      cy = dst.h >> 1;
      canvas_draw_icon(c, &img, cx, cy);
    }
  } else if (text != NULL && text->size > 0) {
    int32_t align_h = style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
    int32_t align_v = style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);
    int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 2);

    w = canvas_measure_text(c, text->str, text->size);
    switch (align_v) {
      case ALIGN_V_TOP:
        y = margin;
        break;
      case ALIGN_V_BOTTOM:
        y = widget->h - margin - font_size;
        break;
      default:
        y = (widget->h - font_size) >> 1;
        break;
    }

    switch (align_h) {
      case ALIGN_H_LEFT:
        x = margin;
        break;
      case ALIGN_H_RIGHT:
        x = widget->w - margin - w;
        break;
      default:
        x = (widget->w - w) >> 1;
        break;
    }

    canvas_draw_text(c, text->str, text->size, x, y);
  }

  return RET_OK;
}

ret_t widget_draw_background(widget_t* widget, canvas_t* c) {
  rect_t dst;
  bitmap_t img;
  style_t* style = &(widget->style);
  color_t trans = color_init(0, 0, 0, 0);
  const char* image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, trans);

  if (bg.rgba.a) {
    canvas_set_fill_color(c, bg);
    canvas_fill_rect(c, 0, 0, widget->w, widget->h);
  }

  if (image_name != NULL) {
    if (image_manager_load(image_manager(), image_name, &img) == RET_OK) {
      rect_init(dst, 0, 0, widget->w, widget->h);
      image_draw_type_t draw_type =
          (image_draw_type_t)style_get_int(style, STYLE_ID_BG_IMAGE_DRAW_TYPE, IMAGE_DRAW_CENTER);
      canvas_draw_image_ex(c, &img, draw_type, &dst);
    }
  }

  return RET_OK;
}

ret_t widget_draw_border(widget_t* widget, canvas_t* c) {
  style_t* style = &(widget->style);
  color_t trans = color_init(0, 0, 0, 0);
  color_t bd = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);

  if (bd.rgba.a) {
    canvas_set_stroke_color(c, bd);
    canvas_stroke_rect(c, 0, 0, widget->w, widget->h);
  }

  return RET_OK;
}

ret_t widget_paint_helper(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text) {
  widget_draw_icon_text(widget, c, icon, text);

  return RET_OK;
}

ret_t widget_paint(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_translate(c, widget->x, widget->y);
#ifdef FAST_MODE
  if (widget->dirty) {
    widget_t* parent = widget->parent;
    if (parent != NULL && !(parent->dirty)) {
      color_t trans = color_init(0, 0, 0, 0);
      style_t* style = &(parent->style);
      color_t bg = style_get_color(style, STYLE_ID_BG_COLOR, trans);
      if (bg.rgba.a != 0) {
        canvas_set_fill_color(c, bg);
        canvas_fill_rect(c, 0, 0, widget->w, widget->h);
      }
    }

    widget_on_paint_background(widget, c);
    widget_on_paint_self(widget, c);
  }
#else
  widget_on_paint_background(widget, c);
  widget_on_paint_self(widget, c);
#endif
  widget_on_paint_children(widget, c);
  widget_on_paint_done(widget, c);

  canvas_untranslate(c, widget->x, widget->y);
  widget->dirty = FALSE;

  return RET_OK;
}

ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  event_t e = {EVT_PROP_CHANGED, widget};
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_X)) {
    widget->x = (wh_t)value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_Y)) {
    widget->y = (wh_t)value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_W)) {
    widget->w = (wh_t)value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_H)) {
    widget->h = (wh_t)value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_VISIBLE)) {
    widget->visible = !!value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_STYLE)) {
    widget->style_type = value_int(v);
    widget_update_style(widget);
  } else if (str_fast_equal(name, WIDGET_PROP_ENABLE)) {
    widget->enable = !!value_int(v);
  } else if (str_fast_equal(name, WIDGET_PROP_NAME)) {
    widget_set_name(widget, value_str(v));
  } else if (str_fast_equal(name, WIDGET_PROP_TEXT)) {
    wstr_from_value(&(widget->text), v);
  } else {
    ret = RET_NOT_FOUND;
  }

  if (widget->vt->set_prop) {
    ret_t ret1 = widget->vt->set_prop(widget, name, v);
    if (ret == RET_NOT_FOUND) {
      ret = ret1;
    }
  }

  if (ret != RET_NOT_FOUND) {
    widget_dispatch(widget, &e);
  }

  return ret;
}

ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_X)) {
    value_set_int32(v, widget->x);
  } else if (str_fast_equal(name, WIDGET_PROP_Y)) {
    value_set_int32(v, widget->y);
  } else if (str_fast_equal(name, WIDGET_PROP_W)) {
    value_set_int32(v, widget->w);
  } else if (str_fast_equal(name, WIDGET_PROP_H)) {
    value_set_int32(v, widget->h);
  } else if (str_fast_equal(name, WIDGET_PROP_VISIBLE)) {
    value_set_bool(v, widget->visible);
  } else if (str_fast_equal(name, WIDGET_PROP_STYLE)) {
    value_set_int(v, widget->style_type);
  } else if (str_fast_equal(name, WIDGET_PROP_ENABLE)) {
    value_set_bool(v, widget->enable);
  } else if (str_fast_equal(name, WIDGET_PROP_NAME)) {
    value_set_str(v, widget->name.str);
  } else if (str_fast_equal(name, WIDGET_PROP_TEXT)) {
    value_set_wstr(v, widget->text.str);
  } else {
    if (widget->vt->get_prop) {
      ret = widget->vt->get_prop(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

ret_t widget_on_paint_background(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_background) {
    ret = widget->vt->on_paint_background(widget, c);
  } else {
    if (widget->style.data) {
      widget_draw_background(widget, c);
    }
  }

  return ret;
}

ret_t widget_on_paint_self(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_self) {
    ret = widget->vt->on_paint_self(widget, c);
  } else {
    paint_event_t paint;
    paint.c = c;
    paint.e.type = EVT_PAINT;
    paint.e.target = widget;

    widget_dispatch(widget, (event_t*)&paint);
  }

  return ret;
}

ret_t widget_on_paint_children(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_children) {
    ret = widget->vt->on_paint_children(widget, c);
  } else {
    ret = widget_on_paint_children_default(widget, c);
  }

  return ret;
}

ret_t widget_on_paint_done(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_done) {
    ret = widget->vt->on_paint_done(widget, c);
  } else {
    if (widget->style.data) {
      ret = widget_draw_border(widget, c);
    }
  }

  return ret;
}

ret_t widget_on_keydown(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_keydown) {
    ret = widget->vt->on_keydown(widget, e);
  } else if (widget->key_target != NULL) {
    widget_on_keydown(widget->key_target, e);
  }

  return ret;
}

ret_t widget_on_keyup(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_keyup) {
    ret = widget->vt->on_keyup(widget, e);
  } else if (widget->key_target != NULL) {
    widget_on_keyup(widget->key_target, e);
  }

  return ret;
}

ret_t widget_on_click(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_click) {
    ret = widget->vt->on_click(widget, e);
  } else if (widget->target != NULL) {
    widget_on_click(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_down) {
    ret = widget->vt->on_pointer_down(widget, e);
  }

  target = widget_find_target(widget, e->x, e->y);
  if (target != NULL && target->enable) {
    event_t focus = {EVT_FOCUS, target};

    if (widget->key_target) {
      event_t blur = {EVT_BLUR, widget->key_target};
      widget->key_target->focused = FALSE;
      widget_dispatch(widget->key_target, &blur);
    }

    widget->target = target;
    widget->key_target = target;
    widget->key_target->focused = TRUE;
    widget_dispatch(widget->key_target, &focus);
  }

  if (widget->target != NULL) {
    ret = widget_on_pointer_down(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_move) {
    ret = widget->vt->on_pointer_move(widget, e);
  }

  target = widget_find_target(widget, e->x, e->y);
  if (target != widget->target) {
    if (widget->target != NULL) {
      event_t leave = {EVT_POINTER_LEAVE, widget->target};
      widget_dispatch(widget->target, &leave);
    }

    if (target != NULL) {
      event_t enter = {EVT_POINTER_ENTER, widget->target};
      widget_dispatch(target, &enter);
    }
    widget->target = target;
  } else if (widget->target != NULL) {
    widget_on_pointer_move(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_up) {
    ret = widget->vt->on_pointer_up(widget, e);
  }

  if (widget->target != NULL) {
    widget_on_pointer_up(widget->target, e);
  }

  return ret;
}

ret_t widget_grab(widget_t* widget, widget_t* child) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && child != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  if (widget->vt->grab) {
    ret = widget->vt->grab(widget, child);
  } else {
    ret = widget_grab_default(widget, child);
  }

  return ret;
}

ret_t widget_ungrab(widget_t* widget, widget_t* child) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  if (widget->vt->ungrab) {
    ret = widget->vt->ungrab(widget, child);
  } else {
    ret = widget_ungrab_default(widget, child);
  }

  return ret;
}

ret_t widget_destroy(widget_t* widget) {
  event_t e = {EVT_DESTROY, widget};
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    widget_dispatch(widget, &e);
    emitter_destroy(widget->emitter);
  }

  if (widget->children != NULL) {
    widget_destroy_children(widget);
    array_destroy(widget->children);
  }

  if (widget->vt->destroy) {
    widget->vt->destroy(widget);
  }

  if (widget->layout_params != NULL) {
    TKMEM_FREE(widget->layout_params);
  }

  str_reset(&(widget->name));
#ifdef WITH_DYNAMIC_TR
  str_reset(&(widget->tr_key));
#endif /*WITH_DYNAMIC_TR*/
  wstr_reset(&(widget->text));
  memset(widget, 0x00, sizeof(widget_t));
  TKMEM_FREE(widget);

  return RET_OK;
}

static ret_t widget_set_dirty(widget_t* widget) {
  uint32_t i = 0;
  uint32_t n = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->dirty = TRUE;
  if (widget->children != NULL) {
    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      widget_set_dirty(iter);
    }
  }

  return RET_OK;
}

ret_t widget_invalidate(widget_t* widget, rect_t* r) {
  rect_t rself;
  if (r == NULL) {
    rect_init(rself, 0, 0, widget->w, widget->h);
    r = &rself;
  }

  return_value_if_fail(widget != NULL && r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(r->x >= 0 && r->y >= 0, RET_BAD_PARAMS);
  return_value_if_fail((r->x + r->w) <= widget->w && (r->y + r->h) <= widget->h, RET_BAD_PARAMS);

  widget_set_dirty(widget);
  if (widget->vt && widget->vt->invalidate) {
    return widget->vt->invalidate(widget, r);
  } else {
    return widget_invalidate_default(widget, r);
  }
}

ret_t widget_update_style(widget_t* widget) {
  uint8_t state = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  state = widget->state;
  if (!widget->enable) {
    state = WIDGET_STATE_DISABLE;
  }

  widget->style.data = theme_find_style(theme(), widget->type, widget->style_type, state);

  return RET_OK;
}

widget_t* widget_init(widget_t* widget, widget_t* parent, uint8_t type) {
  return_value_if_fail(widget != NULL, NULL);

  widget->dirty = TRUE;
  widget->type = type;
  widget->style_type = 0;
  widget->enable = TRUE;
  widget->visible = TRUE;
  widget->emitter = NULL;
  widget->children = NULL;
  widget->state = WIDGET_STATE_NORMAL;

  if (parent) {
    widget_add_child(parent, widget);
  }

  str_init(&(widget->name), 0);
#ifdef WITH_DYNAMIC_TR
  str_init(&(widget->tr_key), 0);
#endif /*WITH_DYNAMIC_TR*/
  wstr_init(&(widget->text), 0);
  if (!widget->vt) {
    widget->vt = widget_vtable_default();
  }

  if (type != WIDGET_WINDOW_MANAGER) {
    widget_update_style(widget);
  }

  return widget;
}

ret_t widget_to_local(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x -= iter->x;
    p->y -= iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

ret_t widget_to_global(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x += iter->x;
    p->y += iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

uint32_t widget_count_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, 0);

  return widget->children != NULL ? widget->children->size : 0;
}

widget_t* widget_get_child(widget_t* widget, uint32_t index) {
  return_value_if_fail(widget != NULL && widget->children != NULL, NULL);
  return_value_if_fail(index < widget->children->size, NULL);

  return WIDGETP(widget->children->elms[index]);
}

ret_t widget_to_xml(widget_t* widget) {
  const wchar_t* text = NULL;
  const key_type_value_t* kv = widget_type_find_by_value(widget->type);

  log_debug("<%s name=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\"", kv->name, widget->name.str,
            widget->x, widget->y, widget->w, widget->h);
  text = widget_get_text(widget);
  if (text) {
    char str[128];
    utf8_from_utf16(text, str, sizeof(str));
    log_debug(" text=\"%s\"", str);
  }

  if (widget->children) {
    uint32_t i = 0;
    log_debug(">\n");
    for (i = 0; i < widget_count_children(widget); i++) {
      widget_t* iter = widget_get_child(widget, i);
      widget_to_xml(iter);
    }

    log_debug("</%s>\n", kv->name);
  } else {
    log_debug("/>\n");
  }

  return RET_OK;
}

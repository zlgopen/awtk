/**
 * File:   widget.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/time.h"
#include "base/mem.h"
#include "base/utf8.h"
#include "base/utils.h"
#include "base/enums.h"
#include "base/locale.h"
#include "base/widget.h"
#include "base/layout.h"
#include "base/system_info.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t widget_destroy_only(widget_t* widget);

ret_t widget_move(widget_t* widget, xy_t x, xy_t y) {
  event_t e = event_init(EVT_WILL_MOVE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->x != x || widget->y != y) {
    widget_dispatch(widget, &e);
    widget->x = x;
    widget->y = y;
    e.type = EVT_MOVE;
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_resize(widget_t* widget, wh_t w, wh_t h) {
  event_t e = event_init(EVT_WILL_RESIZE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->w != w || widget->h != h) {
    widget_dispatch(widget, &e);
    widget->w = w;
    widget->h = h;

    e.type = EVT_RESIZE;
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h) {
  event_t e = event_init(EVT_WILL_MOVE_RESIZE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->x != x || widget->y != y || widget->w != w || widget->h != h) {
    widget_dispatch(widget, &e);
    widget->x = x;
    widget->y = y;
    widget->w = w;
    widget->h = h;
    e.type = EVT_MOVE_RESIZE;
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_value(widget_t* widget, int32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_VALUE, value_set_uint32(&v, value));
}

ret_t widget_use_style(widget_t* widget, const char* value) {
  return_value_if_fail(widget != NULL && value != NULL, RET_BAD_PARAMS);

  widget->style_type = atoi(value);
  widget_update_style(widget);

  return RET_OK;
}

ret_t widget_set_text(widget_t* widget, const wchar_t* text) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_wstr(&v, text));
}

ret_t widget_set_text_utf8(widget_t* widget, const char* text) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, text));
}

ret_t widget_set_tr_text(widget_t* widget, const char* text) {
  value_t v;
  const char* tr_text = NULL;
  return_value_if_fail(widget != NULL && text != NULL, RET_OK);

  tr_text = locale_tr(locale(), text);
#ifdef WITH_DYNAMIC_TR
  TKMEM_FREE(widget->tr_key);
  widget->tr_key = tk_strdup(text);
#endif /*WITH_DYNAMIC_TR*/

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
}

ret_t widget_re_translate_text(widget_t* widget) {
#ifdef WITH_DYNAMIC_TR
  if (widget->tr_key != NULL) {
    value_t v;
    const char* tr_text = locale_tr(locale(), widget->tr_key);
    widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
    widget_invalidate(widget, NULL);
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_re_translate_text(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
#else
  return RET_FAIL;
#endif /*WITH_DYNAMIC_TR*/
}

int32_t widget_get_value(widget_t* widget) {
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

  if (widget->name != NULL) {
    TKMEM_FREE(widget->name);
  }
  widget->name = tk_strdup(name);

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
    widget_invalidate_force(widget);
  }

  return RET_OK;
}

ret_t widget_set_opacity(widget_t* widget, uint8_t opacity) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->opacity = opacity;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t widget_set_rotation(widget_t* widget, float_t rotation) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
#ifdef WITH_VGCANVAS_LCD
  widget->rotation = rotation;
  widget_invalidate(widget, NULL);
  return RET_OK;
#else
  (void)rotation;
  return RET_NOT_IMPL;
#endif /*WITH_VGCANVAS_LCD*/
}

ret_t widget_set_scale(widget_t* widget, float_t scale_x, float_t scale_y) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
#ifdef WITH_VGCANVAS_LCD
  widget->scale_x = scale_x;
  widget->scale_y = scale_y;
  widget_invalidate(widget, NULL);
  return RET_OK;
#else
  (void)scale_x;
  (void)scale_y;
  return RET_NOT_IMPL;
#endif /*WITH_VGCANVAS_LCD*/
}

ret_t widget_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
#ifdef WITH_VGCANVAS_LCD
  widget->anchor_x = anchor_x;
  widget->anchor_y = anchor_y;
  widget_invalidate(widget, NULL);
  return RET_OK;
#else
  (void)anchor_x;
  (void)anchor_y;
  return RET_NOT_IMPL;
#endif /*WITH_VGCANVAS_LCD*/
}

ret_t widget_destroy_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_destroy_only(iter);
  WIDGET_FOR_EACH_CHILD_END();
  widget->children->size = 0;

  return RET_OK;
}

ret_t widget_add_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  child->parent = widget;
  if (widget->children == NULL) {
    widget->children = array_create(4);
  }

  if (widget->vt->on_add_child) {
    if (widget->vt->on_add_child(widget, child) == RET_OK) {
      return RET_OK;
    }
  }

  return array_push(widget->children, child);
}

ret_t widget_remove_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (widget->target == child) {
    widget->target = NULL;
  }

  if (widget->grab_widget == child) {
    widget->grab_widget = NULL;
  }

  if (widget->key_target == child) {
    widget->key_target = NULL;
  }

  if (widget->vt->on_remove_child) {
    if (widget->vt->on_remove_child(widget, child) == RET_OK) {
      return RET_OK;
    }
  }

  child->parent = NULL;
  return array_remove(widget->children, NULL, child, NULL);
}

static widget_t* widget_lookup_child(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->name != NULL && tk_str_eq(iter->name, name)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

static widget_t* widget_lookup_all(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->name != NULL && tk_str_eq(iter->name, name)) {
    return iter;
  } else {
    iter = widget_lookup_all(iter, name);
    if (iter != NULL) {
      return iter;
    }
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* widget_lookup(widget_t* widget, const char* name, bool_t recursive) {
  if (recursive) {
    return widget_lookup_all(widget, name);
  } else {
    return widget_lookup_child(widget, name);
  }
}

static widget_t* widget_lookup_by_type_child(widget_t* widget, const char* type_name) {
  return_value_if_fail(widget != NULL && type_name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(iter->vt->type_name, type_name)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

static widget_t* widget_lookup_by_type_all(widget_t* widget, const char* type_name) {
  return_value_if_fail(widget != NULL && type_name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(iter->vt->type_name, type_name)) {
    return iter;
  } else {
    iter = widget_lookup_by_type_all(iter, type_name);
    if (iter != NULL) {
      return iter;
    }
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* widget_lookup_by_type(widget_t* widget, const char* type_name, bool_t recursive) {
  if (recursive) {
    return widget_lookup_by_type_all(widget, type_name);
  } else {
    return widget_lookup_by_type_child(widget, type_name);
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
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->visible = visible;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_set_visible_recursive(iter, visible);
  WIDGET_FOR_EACH_CHILD_END()

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

int32_t widget_on(widget_t* widget, event_type_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  if (widget->emitter == NULL) {
    widget->emitter = emitter_create();
  }

  return emitter_on(widget->emitter, type, on_event, ctx);
}

int32_t widget_child_on(widget_t* widget, const char* name, event_type_t type,
                        event_func_t on_event, void* ctx) {
  return widget_on(widget_lookup(widget, name, TRUE), type, on_event, ctx);
}

ret_t widget_off(widget_t* widget, int32_t id) {
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
  rect_t r;
  bitmap_t img;
  style_t* style = &(widget->style);
  int32_t margin = style_get_int(style, STYLE_ID_MARGIN, 2);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);

  wh_t w = widget->w - 2 * margin;
  wh_t h = widget->h - 2 * margin;

  if (text == NULL) {
    text = &(widget->text);
  }

  if (icon == NULL) {
    icon = style_get_str(style, STYLE_ID_ICON, NULL);
  }

  widget_prepare_text_style(widget, c);
  font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  if (icon != NULL && image_manager_load(image_manager(), icon, &img) == RET_OK) {
    float_t dpr = system_info()->device_pixel_ratio;

    if (text != NULL && text->size > 0) {
      if (h > (img.h / dpr + font_size)) {
        int text_h = font_size + margin;
        int text_y = widget->h - text_h;

        r = rect_init(0, 0, widget->w, text_y);
        canvas_draw_icon_in_rect(c, &img, &r);

        r = rect_init(margin, text_y, w, text_h);
      } else {
        r = rect_init(0, 0, widget->h, widget->h);
        canvas_draw_icon_in_rect(c, &img, &r);

        canvas_set_text_align(c, ALIGN_H_LEFT, ALIGN_V_MIDDLE);
        r = rect_init(widget->h, margin, widget->w - widget->h - margin, h);
      }
      canvas_draw_text_in_rect(c, text->str, text->size, &r);
    } else {
      r = rect_init(0, 0, widget->w, widget->h);
      canvas_draw_icon_in_rect(c, &img, &r);
    }
  } else if (text != NULL && text->size > 0) {
    r = rect_init(margin, margin, w, h);
    canvas_draw_text_in_rect(c, text->str, text->size, &r);
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
      dst = rect_init(0, 0, widget->w, widget->h);
      image_draw_type_t draw_type =
          (image_draw_type_t)style_get_int(style, STYLE_ID_BG_IMAGE_DRAW_TYPE, IMAGE_DRAW_CENTER);
      int32_t start_time = time_now_ms();
      canvas_draw_image_ex(c, &img, draw_type, &dst);
      dst.w = time_now_ms() - start_time;
    }
  }

  return RET_OK;
}

ret_t widget_draw_border(widget_t* widget, canvas_t* c) {
  style_t* style = &(widget->style);
  color_t trans = color_init(0, 0, 0, 0);
  color_t bd = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  int32_t border = style_get_int(style, STYLE_ID_BORDER, BORDER_ALL);

  if (bd.rgba.a) {
    wh_t w = widget->w;
    wh_t h = widget->h;

    canvas_set_stroke_color(c, bd);
    if (border == BORDER_ALL) {
      canvas_stroke_rect(c, 0, 0, w, h);
    } else {
      if (border & BORDER_TOP) {
        canvas_draw_hline(c, 0, 0, w);
      } else if (border & BORDER_BOTTOM) {
        canvas_draw_hline(c, 0, h - 1, w);
      } else if (border & BORDER_LEFT) {
        canvas_draw_hline(c, 0, 0, h);
      } else if (border & BORDER_RIGHT) {
        canvas_draw_hline(c, w - 1, 0, h);
      }
    }
  }

  return RET_OK;
}

ret_t widget_paint_helper(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text) {
  if (widget->style.data) {
    widget_draw_icon_text(widget, c, icon, text);
  }

  return RET_OK;
}

ret_t widget_paint(widget_t* widget, canvas_t* c) {
  uint8_t save_alpha = 0;
  vgcanvas_t* vg = NULL;
  bool_t need_transform = FALSE;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (!widget->visible || widget->w <= 0 || widget->h <= 0) {
    widget->dirty = FALSE;
    return RET_OK;
  }

  save_alpha = c->lcd->global_alpha;

  if (widget->opacity < TK_OPACITY_ALPHA) {
    uint8_t alpha = (widget->opacity * save_alpha) / 0xff;
    canvas_set_global_alpha(c, alpha);
  }

#ifdef WITH_VGCANVAS_LCD
  {
    need_transform = !tk_fequal(widget->scale_x, 1) || !tk_fequal(widget->scale_y, 1) ||
                     !tk_fequal(widget->rotation, 0);

    if (need_transform) {
      float_t anchor_x = widget->anchor_x * widget->w;
      float_t anchor_y = widget->anchor_y * widget->h;

      vg = lcd_get_vgcanvas(c->lcd);
      vgcanvas_save(vg);
      vgcanvas_translate(vg, widget->x, widget->y);
      vgcanvas_translate(vg, anchor_x, anchor_y);
      if (!tk_fequal(widget->rotation, 0)) {
        vgcanvas_rotate(vg, widget->rotation);
      }

      if (!tk_fequal(widget->scale_x, 1) || !tk_fequal(widget->scale_y, 1)) {
        vgcanvas_scale(vg, widget->scale_x, widget->scale_y);
      }
      vgcanvas_translate(vg, -anchor_x, -anchor_y);
    } else {
      canvas_translate(c, widget->x, widget->y);
    }
  }
#else
  (void)vg;
  (void)need_transform;
  canvas_translate(c, widget->x, widget->y);
#endif /*WITH_VGCANVAS_LCD*/

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

  widget->dirty = FALSE;
  if (widget->opacity < 0xff) {
    canvas_set_global_alpha(c, save_alpha);
  }

#ifdef WITH_VGCANVAS_LCD
  if (need_transform) {
    vgcanvas_restore(vg);
  } else {
    canvas_untranslate(c, widget->x, widget->y);
  }
#else
  canvas_untranslate(c, widget->x, widget->y);
#endif /*WITH_VGCANVAS_LCD*/

  return RET_OK;
}

ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  prop_change_event_t e;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  e.value = v;
  e.name = name;
  e.e = event_init(EVT_PROP_WILL_CHANGE, widget);
  widget_dispatch(widget, (event_t*)&e);

  if (tk_str_eq(name, WIDGET_PROP_X)) {
    widget->x = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_Y)) {
    widget->y = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_W)) {
    widget->w = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_H)) {
    widget->h = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE)) {
    widget->visible = !!value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_STYLE)) {
    widget->style_type = value_int(v);
    widget_update_style(widget);
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE)) {
    widget->enable = !!value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    widget_set_name(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    wstr_from_value(&(widget->text), v);
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TEXT)) {
    widget_set_tr_text(widget, value_str(v));
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
    e.e.type = EVT_PROP_CHANGED;
    widget_dispatch(widget, (event_t*)&e);
    widget_invalidate(widget, NULL);
  }

  return ret;
}

ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_X)) {
    value_set_int32(v, widget->x);
  } else if (tk_str_eq(name, WIDGET_PROP_Y)) {
    value_set_int32(v, widget->y);
  } else if (tk_str_eq(name, WIDGET_PROP_W)) {
    value_set_int32(v, widget->w);
  } else if (tk_str_eq(name, WIDGET_PROP_H)) {
    value_set_int32(v, widget->h);
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE)) {
    value_set_bool(v, widget->visible);
  } else if (tk_str_eq(name, WIDGET_PROP_STYLE)) {
    value_set_int(v, widget->style_type);
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE)) {
    value_set_bool(v, widget->enable);
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    value_set_str(v, widget->name);
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    value_set_wstr(v, widget->text.str);
  } else {
    if (widget->vt->get_prop) {
      ret = widget->vt->get_prop(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  /*default*/
  if (ret == RET_NOT_FOUND) {
    if (tk_str_eq(name, WIDGET_PROP_LAYOUT_W)) {
      value_set_int32(v, widget->w);
      ret = RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_LAYOUT_H)) {
      value_set_int32(v, widget->h);
      ret = RET_OK;
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

ret_t widget_dispatch_to_target(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  e->target = widget;
  widget_dispatch(widget, e);

  if (widget->target) {
    ret = widget_dispatch_to_target(widget->target, e);
  }

  return ret;
}

ret_t widget_dispatch_to_key_target(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  e->target = widget;
  widget_dispatch(widget, e);

  if (widget->key_target) {
    ret = widget_dispatch_to_target(widget->key_target, e);
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

static ret_t widget_dispatch_leave_event(widget_t* widget) {
  widget_t* target = widget;

  while (target != NULL) {
    event_t e = event_init(EVT_POINTER_LEAVE, target);

    widget_dispatch(target, &e);
    target = target->target;
  }

  return RET_OK;
}

static ret_t widget_dispatch_blur_event(widget_t* widget) {
  widget_t* target = widget;

  while (target != NULL) {
    event_t e = event_init(EVT_BLUR, target);
    if (target->focused) {
      target->focused = FALSE;
      widget_dispatch(target, &e);
    }
    target = target->key_target;
  }

  return RET_OK;
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
    if (target->type != WIDGET_KEYBOARD) {
      if (!target->focused) {
        event_t focus = event_init(EVT_FOCUS, target);
        if (widget->key_target) {
          widget_dispatch_blur_event(widget->key_target);
        }
        widget_dispatch(target, &focus);
      }

      widget->key_target = target;
      widget->key_target->focused = TRUE;
    }
  }

  widget->target = target;
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
      widget_dispatch_leave_event(widget->target);
    }

    if (target != NULL) {
      event_t enter = event_init(EVT_POINTER_ENTER, target);
      widget_dispatch(target, &enter);
    }
    widget->target = target;
  }

  if (widget->target != NULL) {
    widget_on_pointer_move(widget->target, e);
  }

  return ret;
}

ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, (event_t*)e);
  if (widget->vt->on_pointer_up) {
    ret = widget->vt->on_pointer_up(widget, e);
  }

  target = widget_find_target(widget, e->x, e->y);
  if (target != NULL) {
    widget_on_pointer_up(target, e);
  }

  return ret;
}

ret_t widget_layout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_layout_children != NULL) {
    return widget->vt->on_layout_children(widget);
  } else {
    return widget_layout_children_default(widget);
  }
}

ret_t widget_grab(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  widget->grab_widget = child;

  if (widget->parent) {
    widget_grab(widget->parent, widget);
  }

  return RET_OK;
}

ret_t widget_ungrab(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->grab_widget == child) {
    if (widget->parent) {
      widget_ungrab(widget->parent, widget);
    }
    widget->grab_widget = NULL;
  }

  return RET_OK;
}

ret_t widget_foreach(widget_t* widget, tk_visit_t visit, void* ctx) {
  return_value_if_fail(widget != NULL && visit != NULL, RET_BAD_PARAMS);

  if (visit(ctx, widget) != RET_OK) {
    return RET_DONE;
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_foreach(iter, visit, ctx);
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

widget_t* widget_get_window(widget_t* widget) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && widget->parent != NULL, NULL);

  while (iter) {
    if (iter->parent && iter->parent->type == WIDGET_WINDOW_MANAGER) {
      return iter;
    }
    iter = iter->parent;
  }

  return iter;
}

static ret_t widget_destroy_only(widget_t* widget) {
  event_t e = event_init(EVT_DESTROY, widget);
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

  TKMEM_FREE(widget->name);
#ifdef WITH_DYNAMIC_TR
  TKMEM_FREE(widget->tr_key);
#endif /*WITH_DYNAMIC_TR*/
  wstr_reset(&(widget->text));
  memset(widget, 0x00, sizeof(widget_t));
  TKMEM_FREE(widget);

  return RET_OK;
}

ret_t widget_destroy(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL) {
    widget_remove_child(widget->parent, widget);
  }

  return widget_destroy_only(widget);
}

static ret_t widget_set_dirty(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->dirty = TRUE;
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_set_dirty(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

ret_t widget_invalidate(widget_t* widget, rect_t* r) {
  rect_t rself;
  if (r == NULL) {
    rself = rect_init(0, 0, widget->w, widget->h);
    r = &rself;
  }

  return_value_if_fail(widget != NULL && r != NULL, RET_BAD_PARAMS);

  if (widget->dirty) {
    return RET_OK;
  }

  widget_set_dirty(widget);
  if (widget->vt && widget->vt->invalidate) {
    return widget->vt->invalidate(widget, r);
  } else {
    return widget_invalidate_default(widget, r);
  }
}

ret_t widget_invalidate_force(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  widget->dirty = FALSE;

  return widget_invalidate(widget, NULL);
}

const void* widget_get_window_theme(widget_t* widget) {
  value_t v;
  const char* name = NULL;
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, NULL);

  name = win->name;
  if (widget_get_prop(win, WIDGET_PROP_THEME, &v) == RET_OK && value_str(&v) != NULL) {
    name = value_str(&v);
  }

  if (name != NULL) {
    const resource_info_t* res =
        resource_manager_ref(resource_manager(), RESOURCE_TYPE_THEME, name);
    if (res != NULL) {
      return res->data;
    }
  }

  return NULL;
}

static const void* widget_get_style_data(widget_t* widget, uint8_t state) {
  theme_t t;
  const void* data = NULL;
  const char* type_name = widget->vt ? widget->vt->type_name : NULL;

  return_value_if_fail(type_name != NULL, NULL);

  t.data = (const uint8_t*)widget_get_window_theme(widget);
  if (t.data != NULL) {
    data = theme_find_style(&t, type_name, widget->style_type, state);
  }

  if (data == NULL) {
    data = theme_find_style(theme(), type_name, widget->style_type, state);
  }

  return data;
}

ret_t widget_update_style(widget_t* widget) {
  uint8_t state = 0;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  state = widget->state;
  if (!widget->enable) {
    state = WIDGET_STATE_DISABLE;
  }

  widget->style.data = widget_get_style_data(widget, state);

  return RET_OK;
}

widget_t* widget_init(widget_t* widget, widget_t* parent, uint8_t type) {
  return_value_if_fail(widget != NULL, NULL);

  widget->dirty = TRUE;
  widget->type = type;
  widget->opacity = 0xff;
  widget->style_type = 0;
  widget->enable = TRUE;
  widget->visible = TRUE;
  widget->emitter = NULL;
  widget->children = NULL;
  widget->state = WIDGET_STATE_NORMAL;

  if (parent) {
    widget_add_child(parent, widget);
  }

  wstr_init(&(widget->text), 0);
  if (!widget->vt) {
    widget->vt = widget_vtable_default();
  }

  if (parent != NULL) {
    widget_update_style(widget);
  }

#ifdef WITH_VGCANVAS_LCD
  widget->scale_x = 1;
  widget->scale_y = 1;
  widget->rotation = 0;
  widget->anchor_x = 0.5;
  widget->anchor_y = 0.5;
#endif /*WITH_VGCANVAS_LCD*/

  return widget;
}

ret_t widget_to_screen(widget_t* widget, point_t* p) {
  value_t v;
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x += iter->x;
    p->y += iter->y;

    if (widget_get_prop(iter, WIDGET_PROP_XOFFSET, &v) == RET_OK) {
      p->x -= value_int(&v);
    }

    if (widget_get_prop(iter, WIDGET_PROP_YOFFSET, &v) == RET_OK) {
      p->y -= value_int(&v);
    }

    iter = iter->parent;
  }

  return RET_OK;
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

int32_t widget_count_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, 0);

  return widget->children != NULL ? widget->children->size : 0;
}

widget_t* widget_get_child(widget_t* widget, int32_t index) {
  return_value_if_fail(widget != NULL && widget->children != NULL, NULL);
  return_value_if_fail(index < widget->children->size, NULL);

  return WIDGET(widget->children->elms[index]);
}

ret_t widget_to_xml(widget_t* widget) {
  const wchar_t* text = NULL;
  const key_type_value_t* kv = widget_type_find_by_value(widget->type);

  log_debug("<%s name=\"%s\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\"", kv->name, widget->name,
            (int)(widget->x), (int)(widget->y), (int)(widget->w), (int)(widget->h));
  text = widget_get_text(widget);
  if (text) {
    char str[128];
    utf8_from_utf16(text, str, sizeof(str));
    log_debug(" text=\"%s\"", str);
  }

  if (widget->children) {
    int32_t i = 0;
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

ret_t widget_prepare_text_style(widget_t* widget, canvas_t* c) {
  style_t* style = &(widget->style);
  color_t trans = color_init(0, 0, 0, 0);
  color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  align_h_t align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  align_v_t align_v = (align_v_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);

  canvas_set_text_color(c, tc);
  canvas_set_font(c, font_name, font_size);
  canvas_set_text_align(c, align_h, align_v);

  return RET_OK;
}

#define ASSIGN_PROP(prop) clone->prop = widget->prop;
widget_t* widget_clone(widget_t* widget, widget_t* parent) {
  widget_t* clone = NULL;
  const char** properties = NULL;
  return_value_if_fail(widget != NULL && widget->vt != NULL && widget->vt->create != NULL, NULL);

  clone = widget->vt->create(parent, widget->x, widget->y, widget->w, widget->h);
  return_value_if_fail(clone != NULL, NULL);

  ASSIGN_PROP(opacity);
  ASSIGN_PROP(enable);
  ASSIGN_PROP(visible);
  ASSIGN_PROP(style_type);

  clone->name = tk_strdup(widget->name);
#ifdef WITH_DYNAMIC_TR
  clone->tr_key = tk_strdup(widget->tr_key);
#endif /*WITH_DYNAMIC_TR*/

  if (widget->text.size) {
    wstr_set(&(clone->text), widget->text.str);
  }

  properties = widget->vt->properties;
  if (properties != NULL) {
    value_t v;
    uint32_t i = 0;
    for (i = 0; properties[i] != NULL; i++) {
      const char* prop = properties[i];
      if (widget_get_prop(widget, prop, &v) == RET_OK) {
        widget_set_prop(clone, prop, &v);
      }
    }
  }

  if (widget->layout_params != NULL) {
    clone->layout_params = TKMEM_ZALLOC(layout_params_t);
    return_value_if_fail(clone->layout_params != NULL, clone);

    memcpy(clone->layout_params, widget->layout_params, sizeof(layout_params_t));
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_clone(iter, clone);
  WIDGET_FOR_EACH_CHILD_END();

  return clone;
}

#define PROP_EQ(prop) (widget->prop == other->prop)
bool_t widget_equal(widget_t* widget, widget_t* other) {
  bool_t ret = FALSE;
  const char** properties = NULL;
  return_value_if_fail(widget != NULL && other != NULL, FALSE);

  ret = PROP_EQ(opacity) && PROP_EQ(enable) && PROP_EQ(visible) && PROP_EQ(style_type) &&
        PROP_EQ(vt) && PROP_EQ(x) && PROP_EQ(y) && PROP_EQ(w) && PROP_EQ(h);
  if (widget->name != NULL || other->name != NULL) {
    ret = ret && (tk_str_eq(widget->name, other->name) || PROP_EQ(name));
  }

  if (!ret) {
    return ret;
  }

  ret = ret && wstr_equal(&(widget->text), &(other->text));

#ifdef WITH_DYNAMIC_TR
  if (widget->tr_key != NULL || other->tr_key != NULL) {
    ret = ret && (tk_str_eq(widget->tr_key, other->tr_key) || PROP_EQ(tr_key));
  }

  if (!ret) {
    return ret;
  }
#endif /*WITH_DYNAMIC_TR*/

  properties = widget->vt->properties;
  if (properties != NULL) {
    value_t v1;
    value_t v2;
    uint32_t i = 0;
    for (i = 0; properties[i] != NULL; i++) {
      const char* prop = properties[i];
      if (widget_get_prop(widget, prop, &v1) != RET_OK) {
        continue;
      }

      if (widget_get_prop(other, prop, &v2) != RET_OK) {
        return FALSE;
      }

      if (!value_equal(&v1, &v2)) {
        return FALSE;
      }
    }
  }

  if (widget->layout_params != NULL || other->layout_params != NULL) {
    if (widget->layout_params && other->layout_params) {
      ret =
          ret && memcmp(widget->layout_params, other->layout_params, sizeof(layout_params_t)) == 0;
    } else {
      return FALSE;
    }
  }

  if (!ret) {
    return ret;
  }

  if (widget->children == other->children) {
    return TRUE;
  }

  if (widget->children == NULL || other->children == NULL) {
    return FALSE;
  }

  if (widget->children->size != other->children->size) {
    return FALSE;
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_t* iter_other = WIDGET(other->children->elms[i]);
  if (!widget_equal(iter, iter_other)) {
    return FALSE;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return TRUE;
}

/**
 * File:   rich_text.h
 * Author: AWTK Develop Team
 * Brief:  rich_text
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
 * 2018-07-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "base/image_manager.h"
#include "rich_text/rich_text.h"
#include "rich_text/rich_text_parser.h"
#include "widget_animators/widget_animator_scroll.h"

static bool_t rich_text_is_scollable(widget_t* widget) {
  rich_text_t* rich_text = RICH_TEXT(widget);

  return (rich_text->content_h + rich_text->margin * 2) > widget->h;
}

static ret_t rich_text_reset(widget_t* widget) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(rich_text != NULL, RET_BAD_PARAMS);

  if (rich_text->node != NULL) {
    rich_text_node_destroy(rich_text->node);
    rich_text->node = NULL;
  }

  if (rich_text->render_node != NULL) {
    rich_text_render_node_destroy(rich_text->render_node);
    rich_text->render_node = NULL;
  }

  return RET_OK;
}

static ret_t rich_text_on_paint_text(widget_t* widget, canvas_t* c) {
  rect_t r;
  rect_t r_save;
  int32_t align_h = ALIGN_H_LEFT;
  rich_text_render_node_t* iter = NULL;
  rich_text_t* rich_text = RICH_TEXT(widget);
  int32_t yoffset = rich_text->yoffset;
  return_value_if_fail(widget != NULL && rich_text != NULL && c != NULL, RET_BAD_PARAMS);

  if (widget->w <= rich_text->margin << 1 || widget->h <= rich_text->margin << 1) {
    return RET_OK;
  }

  r = rect_init(c->ox, c->oy, widget->w, widget->h);
  r.x += rich_text->margin;
  r.y += rich_text->margin;
  r.w -= rich_text->margin << 1;
  r.h -= rich_text->margin << 1;

  canvas_get_clip_rect(c, &r_save);
  r = rect_intersect(&r, &r_save);
  canvas_set_clip_rect(c, &r);

  if (widget->astyle) {
    align_h = style_get_int(widget->astyle, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_LEFT);
  }

  iter = rich_text->render_node;
  while (iter != NULL) {
    r = iter->rect;
    r.y -= yoffset;

    if ((r.y + r.h) <= 0) {
      iter = iter->next;
      continue;
    }

    if (r.y > widget->h) {
      break;
    }

    if (align_h == ALIGN_H_CENTER) {
      r.x += iter->align_h_w >> 1;
    } else if (align_h == ALIGN_H_RIGHT) {
      r.x += iter->align_h_w;
    }

    switch (iter->node->type) {
      case RICH_TEXT_TEXT: {
        rect_t cr;
        int32_t i = 0;
        float_t x = r.x;
        wchar_t* text = iter->text;
        int32_t spacing = iter->spacing;
        rich_text_font_t* font = &(iter->node->u.text.font);

        canvas_set_text_color(c, font->color);
        canvas_set_font(c, font->name, font->size);
        canvas_set_text_align(c, ALIGN_H_LEFT, font->align_v);

        for (i = 0; i < iter->size; i++) {
          float_t cw = canvas_measure_text(c, text + i, 1);
          cr.x = x;
          cr.y = r.y;
          cr.h = r.h;
          cr.w = cw + 1;

          canvas_draw_text_in_rect(c, text + i, 1, &cr);
          x += cw;
          if (spacing > 0) {
            if (rich_text_is_flexable_w_char(text[i])) {
              x += iter->flexible_w_char_delta_w;
              spacing -= iter->flexible_w_char_delta_w;
            }
          }
        }
        break;
      }
      case RICH_TEXT_IMAGE: {
        bitmap_t bitmap;
        const char* name = iter->node->u.image.name;
        image_draw_type_t draw_type = iter->node->u.image.draw_type;

        if (widget_load_image(widget, name, &bitmap) == RET_OK) {
          canvas_draw_image_ex(c, &bitmap, draw_type, &r);
        }
        break;
      }
      default:
        break;
    }

    iter = iter->next;
  }
  canvas_set_clip_rect(c, &r_save);

  return RET_OK;
}

static ret_t rich_text_ensure_render_node(widget_t* widget, canvas_t* c) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(widget != NULL && rich_text != NULL, RET_BAD_PARAMS);

  if (rich_text->need_reset) {
    str_t str;
    str_init(&str, widget->text.size * 4 + 1);
    str_from_wstr(&str, widget->text.str);
    rich_text_reset(widget);
    rich_text->node = rich_text_parse(str.str, str.size);
    str_reset(&str);
    rich_text->need_reset = FALSE;
  }

  if (rich_text->render_node != NULL) {
    return RET_OK;
  }

  if (rich_text->node == NULL) {
    return RET_FAIL;
  }

  if (rich_text->render_node == NULL) {
    int32_t w = widget->w;
    int32_t h = widget->h;
    int32_t line_gap = rich_text->line_gap;
    int32_t margin = rich_text->margin;

    rich_text->render_node =
        rich_text_render_node_layout(widget, rich_text->node, c, w, h, margin, line_gap);
    widget_set_prop_int(WIDGET(rich_text), WIDGET_PROP_YOFFSET, 0);
  }
  return_value_if_fail(rich_text->render_node != NULL, RET_OOM);

  return RET_OK;
}

static ret_t rich_text_on_paint_self(widget_t* widget, canvas_t* c) {
  if (rich_text_ensure_render_node(widget, c) == RET_OK) {
    return rich_text_on_paint_text(widget, c);
  }

  return RET_OK;
}

static ret_t rich_text_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  rich_text_t* rich_text = RICH_TEXT(ctx);
  return_value_if_fail(widget != NULL && rich_text != NULL, RET_BAD_PARAMS);

  rich_text->wa = NULL;

  return RET_REMOVE;
}

ret_t rich_text_scroll_to(widget_t* widget, int32_t yoffset_end) {
  int32_t yoffset = 0;
  rich_text_t* rich_text = RICH_TEXT(widget);
  int32_t virtual_h = rich_text->content_h + rich_text->margin * 2;
  return_value_if_fail(rich_text != NULL, RET_FAIL);

  if (!rich_text_is_scollable(widget)) {
    rich_text->yoffset = 0;
    return RET_OK;
  }

  if (yoffset_end < 0) {
    yoffset_end = 0;
  }

  if ((yoffset_end + widget->h) > virtual_h) {
    yoffset_end = virtual_h - widget->h;
  }

  if (yoffset_end == rich_text->yoffset) {
    return RET_OK;
  }

  yoffset = rich_text->yoffset;
  if (rich_text->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)rich_text->wa;
    if (yoffset_end != rich_text->yoffset) {
      bool_t changed = wa->y_to != yoffset_end;
      bool_t in_range = wa->y_to >= 0 && wa->y_to < (virtual_h - widget->h);
      if (changed && in_range) {
        wa->y_to = yoffset_end;
        wa->y_from = rich_text->yoffset;
        wa->base.now = 0;
        wa->base.start_time = 0;
      }
    }
  } else {
    rich_text->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
    return_value_if_fail(rich_text->wa != NULL, RET_OOM);
    widget_animator_scroll_set_params(rich_text->wa, 0, yoffset, 0, yoffset_end);
    widget_animator_on(rich_text->wa, EVT_ANIM_END, rich_text_on_scroll_done, rich_text);
    widget_animator_start(rich_text->wa);
  }

  return RET_OK;
}

ret_t rich_text_scroll_delta_to(widget_t* widget, int32_t yoffset_delta) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(rich_text != NULL, RET_FAIL);

  rich_text->yoffset_end = rich_text->yoffset + yoffset_delta;

  return rich_text_scroll_to(widget, rich_text->yoffset_end);
}

static uint32_t rich_text_get_row_height(widget_t* widget) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  if (rich_text->render_node != NULL) {
    int32_t row_height = tk_max(rich_text->render_node->rect.h, 30);

    return tk_min(row_height, widget->h / 2);
  } else {
    return 30;
  }
}

static ret_t rich_text_on_pointer_down(rich_text_t* rich_text, pointer_event_t* e) {
  velocity_t* v = &(rich_text->velocity);

  velocity_reset(v);
  rich_text->ydown = e->y;
  rich_text->yoffset_save = rich_text->yoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t rich_text_on_pointer_move(rich_text_t* rich_text, pointer_event_t* e) {
  velocity_t* v = &(rich_text->velocity);
  int32_t dy = e->y - rich_text->ydown;
  velocity_update(v, e->e.time, e->x, e->y);

  if (rich_text->wa == NULL && dy) {
    int32_t yoffset = rich_text->yoffset_save - dy;
    widget_set_prop_int(WIDGET(rich_text), WIDGET_PROP_YOFFSET, yoffset);
  }

  return RET_OK;
}

static ret_t rich_text_on_pointer_up(rich_text_t* rich_text, pointer_event_t* e) {
  int32_t yoffset_end = 0;
  widget_t* widget = WIDGET(rich_text);
  velocity_t* v = &(rich_text->velocity);

  velocity_update(v, e->e.time, e->x, e->y);
  yoffset_end = rich_text->yoffset - v->yv;

  rich_text_scroll_to(widget, yoffset_end);

  return RET_OK;
}

static ret_t rich_text_down(widget_t* widget) {
  uint32_t row_height = rich_text_get_row_height(widget);
  return rich_text_scroll_delta_to(widget, row_height);
}

static ret_t rich_text_up(widget_t* widget) {
  uint32_t row_height = rich_text_get_row_height(widget);
  return rich_text_scroll_delta_to(widget, -row_height);
}

static ret_t rich_text_pagedown(widget_t* widget) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  uint32_t row_height = rich_text_get_row_height(widget);
  int32_t h = widget->h - rich_text->margin * 2 - 30;
  int32_t pageh = tk_max(h, row_height);

  return rich_text_scroll_delta_to(widget, pageh);
}

static ret_t rich_text_pageup(widget_t* widget) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  uint32_t row_height = rich_text_get_row_height(widget);
  int32_t h = widget->h - rich_text->margin * 2 - 30;
  int32_t pageh = tk_max(h, row_height);

  return rich_text_scroll_delta_to(widget, -pageh);
}

static ret_t rich_text_on_key_down(widget_t* widget, key_event_t* evt) {
  ret_t ret = RET_OK;

  if (evt->key == TK_KEY_PAGEDOWN) {
    rich_text_pagedown(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_PAGEUP) {
    rich_text_pageup(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_UP) {
    rich_text_up(widget);
    ret = RET_STOP;
  } else if (evt->key == TK_KEY_DOWN) {
    rich_text_down(widget);
    ret = RET_STOP;
  }

  return ret;
}

static ret_t rich_text_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  rich_text_t* rich_text = RICH_TEXT(widget);
  bool_t scrollable = rich_text_is_scollable(widget);
  return_value_if_fail(rich_text != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN:
      rich_text->pressed = TRUE;
      if (scrollable) {
        widget_grab(widget->parent, widget);
        rich_text_on_pointer_down(rich_text, (pointer_event_t*)e);
      }
      break;
    case EVT_POINTER_MOVE: {
      if (scrollable) {
        pointer_event_t* evt = (pointer_event_t*)e;
        if (evt->pressed && rich_text->pressed) {
          rich_text_on_pointer_move(rich_text, evt);
          widget_invalidate(widget, NULL);
          ret = RET_STOP;
        }
      }
      break;
    }
    case EVT_POINTER_UP: {
      rich_text->pressed = FALSE;
      if (scrollable) {
        rich_text_on_pointer_up(rich_text, (pointer_event_t*)e);
        widget_ungrab(widget->parent, widget);
        ret = RET_STOP;
      }
      break;
    }
    case EVT_WHEEL: {
      if (scrollable) {
        wheel_event_t* evt = (wheel_event_t*)e;
        if (evt->dy > 0) {
          rich_text_down(widget);
        } else if (evt->dy < 0) {
          rich_text_up(widget);
        }

        ret = RET_STOP;
      }
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      rich_text->pressed = FALSE;
      rich_text->yoffset = rich_text->yoffset_save;
      break;
    }
    case EVT_POINTER_LEAVE:
      break;
    case EVT_POINTER_ENTER:
      break;
    case EVT_KEY_DOWN: {
      if (scrollable) {
        ret = rich_text_on_key_down(widget, (key_event_t*)e);
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      rich_text->need_reset = TRUE;
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t rich_text_set_prop(widget_t* widget, const char* name, const value_t* v) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(rich_text != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    return rich_text_set_text(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_LINE_GAP)) {
    rich_text->line_gap = value_int(v);
    rich_text->need_reset = TRUE;
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    rich_text->margin = value_int(v);
    rich_text->need_reset = TRUE;
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    rich_text->yoffset = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t rich_text_get_prop(widget_t* widget, const char* name, value_t* v) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(rich_text != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_LINE_GAP)) {
    value_set_int(v, rich_text->line_gap);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    value_set_int(v, rich_text->margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, rich_text->yoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H)) {
    value_set_int(v, rich_text->content_h + 2 * rich_text->margin);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t rich_text_on_destroy(widget_t* widget) {
  return rich_text_reset(widget);
}

static const char* s_rich_text_clone_properties[] = {WIDGET_PROP_MARGIN, WIDGET_PROP_LINE_GAP,
                                                     NULL};
TK_DECL_VTABLE(rich_text) = {.size = sizeof(rich_text_t),
                             .type = "rich_text",
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = rich_text_create,
                             .clone_properties = s_rich_text_clone_properties,
                             .on_event = rich_text_on_event,
                             .set_prop = rich_text_set_prop,
                             .get_prop = rich_text_get_prop,
                             .on_destroy = rich_text_on_destroy,
                             .on_paint_self = rich_text_on_paint_self};

widget_t* rich_text_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(rich_text), x, y, w, h);
}

ret_t rich_text_set_text(widget_t* widget, const char* text) {
  rich_text_t* rich_text = RICH_TEXT(widget);
  return_value_if_fail(rich_text != NULL, RET_BAD_PARAMS);

  wstr_set_utf8(&(widget->text), text);
  rich_text->need_reset = TRUE;
  return RET_OK;
}

widget_t* rich_text_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, rich_text), NULL);

  return widget;
}

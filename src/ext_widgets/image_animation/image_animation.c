/**
 * File:   image_animation.h
 * Author: AWTK Develop Team
 * Brief:  image_animation
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
 * 2018-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/timer.h"
#include "base/image_manager.h"
#include "image_animation/image_animation.h"

static ret_t image_animation_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  char name[NAME_LEN + 1];
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  if (image_animation->image != NULL && image_animation->sequence != NULL &&
      image_animation->index >= 0) {
    memset(name, 0x00, sizeof(name));
    tk_strncpy(name, image_animation->image, NAME_LEN);
    name[strlen(name)] = image_animation->sequence[image_animation->index];

    if (widget_load_image(widget, name, &bitmap) == RET_OK) {
      rect_t r = rect_init(0, 0, widget->w, widget->h);
      canvas_draw_icon_in_rect(c, &bitmap, &r);
    }
  }

  return RET_OK;
}

static ret_t image_animation_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, IMAGE_ANIMATION_PROP_LOOP)) {
    value_set_bool(v, image_animation->loop);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_IMAGE)) {
    value_set_str(v, image_animation->image);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_SEQUENCE)) {
    value_set_str(v, image_animation->sequence);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_AUTO_PLAY)) {
    value_set_bool(v, image_animation->auto_play);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_INTERVAL)) {
    value_set_int(v, image_animation->interval);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DELAY)) {
    value_set_int(v, image_animation->delay);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_animation_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, IMAGE_ANIMATION_PROP_LOOP)) {
    return image_animation_set_loop(widget, value_bool(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_IMAGE)) {
    return image_animation_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_SEQUENCE)) {
    return image_animation_set_sequence(widget, value_str(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_AUTO_PLAY)) {
    return image_animation_set_auto_play(widget, value_bool(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_INTERVAL)) {
    return image_animation_set_interval(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DELAY)) {
    return image_animation_set_delay(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static ret_t image_animation_destroy(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  if (image_animation->timer_id != TK_INVALID_ID) {
    timer_remove(image_animation->timer_id);
    image_animation->timer_id = TK_INVALID_ID;
  }
  TKMEM_FREE(image_animation->image);
  TKMEM_FREE(image_animation->sequence);

  return RET_OK;
}

static const widget_vtable_t s_image_animation_vtable = {
    .size = sizeof(image_animation_t),
    .type = WIDGET_TYPE_IMAGE_ANIMATION,
    .create = image_animation_create,
    .destroy = image_animation_destroy,
    .get_prop = image_animation_get_prop,
    .set_prop = image_animation_set_prop,
    .on_paint_self = image_animation_on_paint_self};

static ret_t image_animation_delay_play(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  image_animation->timer_id = TK_INVALID_ID;
  image_animation_play(widget);

  return RET_REMOVE;
}

static ret_t image_animation_on_open(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  if (image_animation->auto_play) {
    if (image_animation->delay > 0) {
      image_animation->timer_id =
          timer_add(image_animation_delay_play, widget, image_animation->delay);
    } else {
      image_animation_play(widget);
    }
  }

  return RET_REMOVE;
}

widget_t* image_animation_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* win = widget_get_window(parent);
  image_animation_t* image_animation = TKMEM_ZALLOC(image_animation_t);
  widget_t* widget = WIDGET(image_animation);
  return_value_if_fail(image_animation != NULL, NULL);

  image_animation->index = 0;
  image_animation->interval = 16;
  image_animation->loop = TRUE;
  image_animation->auto_play = FALSE;

  widget_on(win, EVT_WINDOW_WILL_OPEN, image_animation_on_open, image_animation);

  return widget_init(widget, parent, &s_image_animation_vtable, x, y, w, h);
}

ret_t image_animation_set_loop(widget_t* widget, bool_t loop) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image_animation->loop = loop;

  return RET_OK;
}

ret_t image_animation_set_image(widget_t* widget, const char* image) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && image != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image_animation->image);
  image_animation->image = tk_strdup(image);

  return RET_OK;
}

ret_t image_animation_set_interval(widget_t* widget, uint32_t interval) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image_animation->interval = interval;

  return RET_OK;
}

ret_t image_animation_set_delay(widget_t* widget, uint32_t delay) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image_animation->delay = delay;

  return RET_OK;
}

ret_t image_animation_set_auto_play(widget_t* widget, bool_t auto_play) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image_animation->auto_play = auto_play;

  return RET_OK;
}

ret_t image_animation_set_sequence(widget_t* widget, const char* sequence) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && sequence != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image_animation->sequence);
  image_animation->sequence = tk_strdup(sequence);

  return RET_OK;
}

static ret_t image_animation_on_update(const timer_info_t* info) {
  ret_t ret = RET_REMOVE;
  widget_t* widget = WIDGET(info->ctx);
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  if (image_animation->image != NULL && image_animation->sequence != NULL) {
    if (image_animation->index < 0) {
      image_animation->index = 0;
      ret = RET_REPEAT;
    } else {
      if (image_animation->sequence[image_animation->index + 1]) {
        image_animation->index++;
        ret = RET_REPEAT;
      } else {
        if (image_animation->loop) {
          event_t e = event_init(EVT_ANIM_ONCE, widget);

          image_animation->index = 0;
          ret = RET_REPEAT;

          widget_dispatch(widget, &e);
        } else {
          event_t e = event_init(EVT_ANIM_END, widget);
          image_animation->index = -1;

          widget_dispatch(widget, &e);
        }
      }
    }

    widget_invalidate(widget, NULL);
  }

  return ret;
}

ret_t image_animation_play(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (image_animation->timer_id == TK_INVALID_ID) {
    image_animation->timer_id =
        timer_add(image_animation_on_update, widget, image_animation->interval);
  }

  return RET_OK;
}

ret_t image_animation_stop(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image_animation_pause(widget);
  image_animation->index = -1;

  return RET_OK;
}

ret_t image_animation_pause(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (image_animation->timer_id != TK_INVALID_ID) {
    timer_remove(image_animation->timer_id);
    image_animation->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

widget_t* image_animation_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_image_animation_vtable, NULL);

  return widget;
}

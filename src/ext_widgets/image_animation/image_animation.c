/**
 * File:   image_animation.h
 * Author: AWTK Develop Team
 * Brief:  image_animation
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
 * 2018-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/idle.h"
#include "base/timer.h"
#include "base/image_manager.h"
#include "image_animation/image_animation.h"

static ret_t image_animation_start_init_if_not_inited(widget_t* widget);

ret_t image_animation_get_image_name(image_animation_t* image_animation,
                                     char name[TK_NAME_LEN + 1]) {
  memset(name, 0x00, TK_NAME_LEN + 1);
  if (image_animation->sequence != NULL) {
    tk_strncpy(name, image_animation->image, TK_NAME_LEN);
    image_animation->index =
        image_animation->index >= strlen(image_animation->sequence) ? 0 : image_animation->index;
    name[strlen(name)] = image_animation->sequence[image_animation->index];
  } else {
    const char* format = image_animation->format ? image_animation->format : "%s%d";
    tk_snprintf(name, TK_NAME_LEN, format, image_animation->image, image_animation->index);
  }

  return RET_OK;
}

#define MAX_CACHE_NR 60

static ret_t on_idle_unload_image(const idle_info_t* info) {
  bitmap_t bitmap;
  widget_t* widget = NULL;
  image_animation_t* image_animation = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  widget = WIDGET(info->ctx);
  image_animation = IMAGE_ANIMATION(info->ctx);
  return_value_if_fail(widget != NULL && image_animation != NULL, RET_REMOVE);

  if (image_animation->image_buffer != NULL) {
    memset(&bitmap, 0x00, sizeof(bitmap));
    bitmap.buffer = (graphic_buffer_t*)(image_animation->image_buffer);
    widget_unload_image(widget, &bitmap);
  }

  return RET_REMOVE;
}

static ret_t image_animation_load_image(image_animation_t* image_animation, bitmap_t* bitmap) {
  char name[TK_NAME_LEN + 1];
  widget_t* widget = WIDGET(image_animation);
  return_value_if_fail(widget != NULL && image_animation != NULL && bitmap != NULL, RET_BAD_PARAMS);

  image_animation_get_image_name(image_animation, name);

  tk_strncpy(image_animation->image_name, name, TK_NAME_LEN);

  return widget_load_image(widget, name, bitmap);
}

static ret_t image_animation_on_paint_self(widget_t* widget, canvas_t* c) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && image_animation != NULL && image_animation->image != NULL,
                       RET_BAD_PARAMS);

  image_animation_start_init_if_not_inited(widget);

  if (image_animation->index >= 0) {
    bitmap_t bitmap;

    if (image_animation_load_image(image_animation, &bitmap) == RET_OK) {
      rect_t s = rect_init(0, 0, bitmap.w, bitmap.h);
      rect_t d = rect_init(0, 0, widget->w, widget->h);
      canvas_draw_image_scale_down(c, &bitmap, &s, &d);

      if (image_animation->unload_after_paint) {
        image_animation->image_buffer = (bitmap.buffer);
        idle_add(on_idle_unload_image, widget);
      }
    }
  }

  return RET_OK;
}

static ret_t image_animation_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, IMAGE_ANIMATION_PROP_LOOP)) {
    value_set_bool(v, image_animation->loop);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, image_animation->image);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_START_INDEX)) {
    value_set_int(v, image_animation->start_index);
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_END_INDEX)) {
    value_set_int(v, image_animation->end_index);
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
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, image_animation->format);
    return RET_OK;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_UNLOAD_AFTER_PAINT)) {
    value_set_bool(v, image_animation->unload_after_paint);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_animation_set_prop(widget_t* widget, const char* name, const value_t* v) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, IMAGE_ANIMATION_PROP_LOOP)) {
    return image_animation_set_loop(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return image_animation_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_START_INDEX)) {
    image_animation->start_index = value_uint32(v);
    image_animation->index = image_animation->start_index;
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_END_INDEX)) {
    image_animation->end_index = value_uint32(v);
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_SEQUENCE)) {
    return image_animation_set_sequence(widget, value_str(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_AUTO_PLAY)) {
    return image_animation_set_auto_play(widget, value_bool(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_INTERVAL)) {
    return image_animation_set_interval(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DELAY)) {
    return image_animation_set_delay(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    return image_animation_set_format(widget, value_str(v));
  } else if (tk_str_eq(name, IMAGE_ANIMATION_PROP_UNLOAD_AFTER_PAINT)) {
    return image_animation_set_unload_after_paint(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static ret_t image_animation_on_destroy(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && image_animation != NULL, RET_BAD_PARAMS);

  if (image_animation->timer_id != TK_INVALID_ID) {
    timer_remove(image_animation->timer_id);
    image_animation->timer_id = TK_INVALID_ID;
  }
  image_animation->image_buffer = NULL;
  TKMEM_FREE(image_animation->image);
  TKMEM_FREE(image_animation->sequence);
  TKMEM_FREE(image_animation->format);

  return RET_OK;
}

static const char* s_image_animation_clone_properties[] = {IMAGE_ANIMATION_PROP_LOOP,
                                                           IMAGE_ANIMATION_PROP_SEQUENCE,
                                                           IMAGE_ANIMATION_PROP_START_INDEX,
                                                           IMAGE_ANIMATION_PROP_END_INDEX,
                                                           IMAGE_ANIMATION_PROP_INTERVAL,
                                                           IMAGE_ANIMATION_PROP_AUTO_PLAY,
                                                           NULL};

TK_DECL_VTABLE(image_animation) = {.size = sizeof(image_animation_t),
                                   .type = WIDGET_TYPE_IMAGE_ANIMATION,
                                   .clone_properties = s_image_animation_clone_properties,
                                   .parent = TK_PARENT_VTABLE(widget),
                                   .create = image_animation_create,
                                   .on_destroy = image_animation_on_destroy,
                                   .get_prop = image_animation_get_prop,
                                   .set_prop = image_animation_set_prop,
                                   .on_paint_self = image_animation_on_paint_self};

static ret_t image_animation_delay_play(const timer_info_t* info) {
  widget_t* widget = NULL;
  image_animation_t* image_animation = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  widget = WIDGET(info->ctx);
  image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && image_animation != NULL, RET_REMOVE);

  image_animation->timer_id = TK_INVALID_ID;
  image_animation_play(widget);

  return RET_REMOVE;
}

static ret_t image_animation_start_init_if_not_inited(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(widget != NULL && image_animation != NULL, RET_BAD_PARAMS);

  if (!(image_animation->inited)) {
    if (image_animation->auto_play) {
      if (image_animation->delay > 0) {
        image_animation->timer_id =
            timer_add(image_animation_delay_play, widget, image_animation->delay);
      } else {
        image_animation_play(widget);
      }
    }

    image_animation->inited = TRUE;
  }

  return RET_OK;
}

widget_t* image_animation_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(image_animation), x, y, w, h);
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);

  return_value_if_fail(image_animation != NULL, NULL);

  image_animation->index = 0;
  image_animation->end_index = 0;
  image_animation->start_index = 0;
  image_animation->interval = 16;
  image_animation->loop = TRUE;
  image_animation->auto_play = FALSE;

  return widget;
}

ret_t image_animation_set_loop(widget_t* widget, bool_t loop) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->loop = loop;

  return RET_OK;
}

ret_t image_animation_set_image(widget_t* widget, const char* image) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && image != NULL, RET_BAD_PARAMS);

  image_animation->image = tk_str_copy(image_animation->image, image);

  return RET_OK;
}

ret_t image_animation_set_interval(widget_t* widget, uint32_t interval) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->interval = interval;

  return RET_OK;
}

ret_t image_animation_set_delay(widget_t* widget, uint32_t delay) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->delay = delay;

  return RET_OK;
}

ret_t image_animation_set_auto_play(widget_t* widget, bool_t auto_play) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->auto_play = auto_play;

  return RET_OK;
}

ret_t image_animation_set_unload_after_paint(widget_t* widget, bool_t unload_after_paint) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->unload_after_paint = unload_after_paint;

  return RET_OK;
}

ret_t image_animation_set_sequence(widget_t* widget, const char* sequence) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && sequence != NULL, RET_BAD_PARAMS);

  if (sequence != NULL && sequence[0] != '\0') {
    image_animation->sequence = tk_str_copy(image_animation->sequence, sequence);
  } else {
    TKMEM_FREE(image_animation->sequence);
    image_animation->sequence = NULL;
  }

  return widget_invalidate(widget, NULL);
}

ret_t image_animation_set_range_sequence(widget_t* widget, uint32_t start_index,
                                         uint32_t end_index) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation->index = start_index;
  image_animation->end_index = end_index;
  image_animation->start_index = start_index;

  return RET_OK;
}

static ret_t image_animation_restart(image_animation_t* image_animation) {
  if (image_animation->sequence != NULL) {
    image_animation->index = 0;
  } else {
    image_animation->index = image_animation->start_index;
  }

  return RET_REPEAT;
}

ret_t image_animation_next(widget_t* widget) {
  ret_t ret = RET_DONE;
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  if (image_animation->sequence) {
    if (image_animation->index + 1 < strlen(image_animation->sequence)) {
      image_animation->index++;
      ret = RET_OK;
    }
  } else {
    if (image_animation->index < image_animation->end_index) {
      image_animation->index++;
      ret = RET_OK;
    }
  }

  return ret;
}

ret_t image_animation_update(widget_t* widget) {
  ret_t ret = RET_REMOVE;
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && image_animation->image != NULL, RET_REMOVE);

  if (image_animation->index < 0) {
    ret = image_animation_restart(image_animation);
  } else {
    ret = image_animation_next(widget);

    if (ret == RET_DONE) {
      if (image_animation->loop) {
        event_t e = event_init(EVT_ANIM_ONCE, widget);
        widget_dispatch(widget, &e);

        ret = image_animation_restart(image_animation);
      } else {
        event_t e = event_init(EVT_ANIM_END, widget);
        image_animation->index = -1;

        widget_dispatch(widget, &e);
      }
    } else {
      ret = RET_REPEAT;
    }
  }

  widget_invalidate_force(widget, NULL);

  return ret;
}

static ret_t image_animation_on_update(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  return image_animation_update(widget);
}

ret_t image_animation_play(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  if (image_animation->timer_id == TK_INVALID_ID) {
    image_animation->timer_id =
        timer_add(image_animation_on_update, widget, image_animation->interval);
  }

  return RET_OK;
}

ret_t image_animation_stop(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  image_animation_pause(widget);
  image_animation->index = -1;

  return RET_OK;
}

ret_t image_animation_pause(widget_t* widget) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL, RET_BAD_PARAMS);

  if (image_animation->timer_id != TK_INVALID_ID) {
    timer_remove(image_animation->timer_id);
    image_animation->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

ret_t image_animation_set_format(widget_t* widget, const char* format) {
  image_animation_t* image_animation = IMAGE_ANIMATION(widget);
  return_value_if_fail(image_animation != NULL && format != NULL, RET_BAD_PARAMS);

  if (format != NULL && format[0] != '\0') {
    image_animation->format = tk_str_copy(image_animation->format, format);
  } else {
    TKMEM_FREE(image_animation->format);
    image_animation->format = NULL;
  }

  return widget_invalidate(widget, NULL);
}

widget_t* image_animation_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, image_animation), NULL);

  return widget;
}

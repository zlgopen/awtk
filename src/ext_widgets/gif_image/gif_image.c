/**
 * File:   gif_image.h
 * Author: AWTK Develop Team
 * Brief:  gif_image
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
 * 2018-11-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/widget_vtable.h"
#include "gif_image/gif_image.h"

#ifdef AWTK_WEB
static ret_t gif_image_on_timer(const timer_info_t* info) {
  gif_image_t* image = GIF_IMAGE(info->ctx);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  image->index = 0;
  if (WIDGET(image)->visible) {
    widget_invalidate_force(WIDGET(image), NULL);
  }

  return RET_REPEAT;
}
#else
static ret_t gif_image_on_timer(const timer_info_t* info) {
  gif_image_t* image = GIF_IMAGE(info->ctx);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  image->index++;
  if (WIDGET(image)->visible) {
    widget_invalidate_force(WIDGET(image), NULL);
  }
  return RET_REPEAT;
}
#endif /*AWTK_WEB*/

static ret_t gif_image_on_paint_self(widget_t* widget, canvas_t* c) {
  wh_t y = 0;
  wh_t h = 0;
  rect_t src;
  rect_t dst;
  uint32_t frames_nr;
  bitmap_t bitmap;
  vgcanvas_t* vg = NULL;
  gif_image_t* image = GIF_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  return_value_if_fail(image_base != NULL && image != NULL && widget != NULL && c != NULL,
                       RET_BAD_PARAMS);

  if (image_base->image == NULL || image_base->image[0] == '\0') {
    widget_paint_helper(widget, c, NULL, NULL);
    return RET_OK;
  }

  vg = canvas_get_vgcanvas(c);
  return_value_if_fail(widget_load_image(widget, image_base->image, &bitmap) == RET_OK,
                       RET_BAD_PARAMS);
#ifdef AWTK_WEB
  bitmap.gif_frame_h = bitmap.h;
  frames_nr = 1;
#else
  if (!bitmap.is_gif) {
    if (image->timer_id != TK_INVALID_ID) {
      image->index = 0;
      timer_remove(image->timer_id);
      image->timer_id = TK_INVALID_ID;
    }
    return RET_OK;
  }
  frames_nr = bitmap.gif_frames_nr;
#endif /*AWTK_WEB*/

  if (frames_nr > 0) {
    image->index %= frames_nr;
  } else {
    image->index = 0;
  }

#ifdef AWTK_WEB
  if (image->timer_id == TK_INVALID_ID) {
    image->timer_id = timer_add(gif_image_on_timer, image, 16);
  }
#else
  if (frames_nr > 1) {
    uint32_t delay = bitmap.gif_delays[image->index];
    if (image->timer_id == TK_INVALID_ID) {
      image->index = 0;
      image->delay = delay;
      image->timer_id = timer_add(gif_image_on_timer, image, image->delay);
    } else if (image->delay != delay) {
      image->delay = delay;
      timer_info_t* timer = (timer_info_t*)timer_find(image->timer_id);
      if (timer) timer->duration = image->delay;
    }
  } else if (image->timer_id != TK_INVALID_ID) {
    image->index = 0;
    timer_remove(image->timer_id);
    image->timer_id = TK_INVALID_ID;
  }
#endif /*AWTK_WEB*/

  h = bitmap.gif_frame_h;
  y = bitmap.gif_frame_h * image->index;

  if (vg != NULL) {
    if (image_need_transform(widget)) {
      vgcanvas_save(vg);
      image_transform(widget, c);
      vgcanvas_draw_icon(vg, &bitmap, 0, y, bitmap.w, h, 0, 0, widget->w, widget->h);
      vgcanvas_restore(vg);

      return RET_OK;
    }
  }

  src = rect_init(0, y, bitmap.w, h);
  dst = rect_init(0, 0, widget->w, widget->h);
  canvas_draw_image_scale_down(c, &bitmap, &src, &dst);

  return RET_OK;
}

static const char* s_gif_image_properties[] = {
    WIDGET_PROP_IMAGE,     WIDGET_PROP_SCALE_X,    WIDGET_PROP_SCALE_Y,
    WIDGET_PROP_ANCHOR_X,  WIDGET_PROP_ANCHOR_Y,   WIDGET_PROP_ROTATION,
    WIDGET_PROP_CLICKABLE, WIDGET_PROP_SELECTABLE, NULL};

static ret_t gif_image_on_destroy(widget_t* widget) {
  gif_image_t* image = GIF_IMAGE(widget);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  if (image->timer_id != TK_INVALID_ID) {
    timer_remove(image->timer_id);
    image->timer_id = TK_INVALID_ID;
  }

  return image_base_on_destroy(widget);
}

TK_DECL_VTABLE(gif_image) = {.size = sizeof(gif_image_t),
                             .type = WIDGET_TYPE_GIF_IMAGE,
                             .clone_properties = s_gif_image_properties,
                             .persistent_properties = s_gif_image_properties,
                             .parent = TK_PARENT_VTABLE(image_base),
                             .create = gif_image_create,
                             .on_destroy = gif_image_on_destroy,
                             .on_event = image_base_on_event,
                             .on_paint_self = gif_image_on_paint_self,
                             .on_paint_background = widget_on_paint_null,
                             .on_copy = image_base_on_copy,
                             .set_prop = image_base_set_prop,
                             .get_prop = image_base_get_prop};

widget_t* gif_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(gif_image), x, y, w, h);
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, NULL);

  image_base_init(widget);

  return widget;
}

widget_t* gif_image_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, gif_image), NULL);

  return widget;
}

/**
 * File:   mutable_image.h
 * Author: AWTK Develop Team
 * Brief:  mutable_image
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
 * 2019-02-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "blend/image_g2d.h"
#include "base/widget_vtable.h"
#include "mutable_image/mutable_image.h"

static bitmap_format_t mutable_image_get_disire_format(widget_t* widget, canvas_t* c) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  bitmap_format_t format = BITMAP_FMT_NONE;

  if (mutable_image->fb != NULL) {
    format = (bitmap_format_t)(mutable_image->fb->format);
  } else {
    format = lcd_get_desired_bitmap_format(c->lcd);
  }

  return format;
}

static bitmap_t* mutable_image_prepare_image(widget_t* widget, canvas_t* c) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  bitmap_format_t format = mutable_image_get_disire_format(widget, c);
  return_value_if_fail(mutable_image != NULL && mutable_image->prepare_image != NULL, NULL);

  if (mutable_image->create_image != NULL) {
    void* ctx = mutable_image->create_image_ctx;
    mutable_image->image = mutable_image->create_image(ctx, format, mutable_image->image);
  } else if (mutable_image->image == NULL) {
    mutable_image->image = bitmap_create_ex(widget->w, widget->h, 0, format);
  }

  if (mutable_image->image == NULL) {
    return NULL;
  }

  if (mutable_image->prepare_image != NULL) {
    bitmap_t* image = mutable_image->image;
    void* ctx = mutable_image->prepare_image_ctx;

    return_value_if_fail(mutable_image->prepare_image(ctx, image) == RET_OK, NULL);
    image->flags |= BITMAP_FLAG_CHANGED;
  }

  return mutable_image->image;
}

ret_t mutable_image_on_paint_self(widget_t* widget, canvas_t* canvas) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  bitmap_t* bitmap = mutable_image_prepare_image(widget, canvas);

  if (bitmap == NULL) {
    return RET_FAIL;
  }

  if (mutable_image->fb != NULL) {
    rect_t r = rect_init(0, 0, bitmap->w, bitmap->h);
    image_copy(mutable_image->fb, bitmap, &r, canvas->ox, canvas->oy);
  } else {
    if (image_need_transform(widget)) {
      vgcanvas_t* vg = canvas_get_vgcanvas(canvas);
      if (vg != NULL) {
        vgcanvas_save(vg);
        image_transform(widget, canvas);
        vgcanvas_draw_icon(vg, bitmap, 0, 0, bitmap->w, bitmap->h, 0, 0, widget->w, widget->h);
        vgcanvas_restore(vg);

        return RET_OK;
      }
    }

    if (bitmap->buffer != NULL) {
      rect_t dst = rect_init(0, 0, widget->w, widget->h);
      canvas_draw_image_center(canvas, bitmap, &dst);
    }
  }

  return RET_OK;
}

static const char* s_mutable_image_clone_properties[] = {WIDGET_PROP_SCALE_X,  WIDGET_PROP_SCALE_Y,
                                                         WIDGET_PROP_ANCHOR_X, WIDGET_PROP_ANCHOR_Y,
                                                         WIDGET_PROP_ROTATION, NULL};

ret_t mutable_image_on_destroy(widget_t* widget) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  return_value_if_fail(widget != NULL && mutable_image != NULL, RET_BAD_PARAMS);

  if (mutable_image->fb != NULL) {
    bitmap_destroy(mutable_image->fb);
    mutable_image->fb = NULL;
  }

  if (mutable_image->image != NULL) {
    bitmap_destroy(mutable_image->image);
    mutable_image->image = NULL;
  }

  return image_base_on_destroy(widget);
}

TK_DECL_VTABLE(mutable_image) = {.size = sizeof(mutable_image_t),
                                 .type = WIDGET_TYPE_MUTABLE_IMAGE,
                                 .clone_properties = s_mutable_image_clone_properties,
                                 .parent = TK_PARENT_VTABLE(image_base),
                                 .create = mutable_image_create,
                                 .on_destroy = mutable_image_on_destroy,
                                 .on_event = image_base_on_event,
                                 .on_paint_self = mutable_image_on_paint_self,
                                 .on_paint_background = widget_on_paint_null,
                                 .set_prop = image_base_set_prop,
                                 .get_prop = image_base_get_prop};

static ret_t mutable_image_invalidate(const timer_info_t* info) {
  widget_invalidate_force(WIDGET(info->ctx), NULL);

  return RET_REPEAT;
}

widget_t* mutable_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(mutable_image), x, y, w, h);
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  return_value_if_fail(mutable_image != NULL, NULL);

  mutable_image_init(widget);

  return widget;
}

widget_t* mutable_image_init(widget_t* widget) {
  image_base_init(widget);
  widget_add_timer(widget, mutable_image_invalidate, 16);

  return widget;
}

ret_t mutable_image_set_prepare_image(widget_t* widget, mutable_image_prepare_image_t prepare_image,
                                      void* prepare_image_ctx) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  return_value_if_fail(mutable_image != NULL && prepare_image != NULL, RET_BAD_PARAMS);

  mutable_image->prepare_image = prepare_image;
  mutable_image->prepare_image_ctx = prepare_image_ctx;

  return RET_OK;
}

ret_t mutable_image_set_create_image(widget_t* widget, mutable_image_create_image_t create_image,
                                     void* create_image_ctx) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  return_value_if_fail(mutable_image != NULL && create_image != NULL, RET_BAD_PARAMS);

  mutable_image->create_image = create_image;
  mutable_image->create_image_ctx = create_image_ctx;

  return RET_OK;
}

ret_t mutable_image_set_framebuffer(widget_t* widget, uint32_t w, uint32_t h,
                                    bitmap_format_t format, uint8_t* buff) {
  mutable_image_t* mutable_image = MUTABLE_IMAGE(widget);
  return_value_if_fail(mutable_image != NULL && buff != NULL, RET_BAD_PARAMS);

  mutable_image->fb = bitmap_create();
  return_value_if_fail(mutable_image->fb != NULL, RET_OOM);

  bitmap_init(mutable_image->fb, w, h, format, buff);
  mutable_image->fb->should_free_handle = TRUE;

  return RET_OK;
}

widget_t* mutable_image_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, mutable_image), NULL);

  return widget;
}

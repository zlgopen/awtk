/**
 * File:   gif_image.h
 * Author: AWTK Develop Team
 * Brief:  gif_image
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef WITH_STB_IMAGE
#include "image_loader/image_loader_stb.h"

#ifdef AWTK_WEB
#include <emscripten.h>

#define GIF_RAW_NONE 0
#define GIF_RAW_LOADING 1
#define GIF_RAW_READY 2
#define GIF_RAW_FAIL 3

static ret_t gif_image_ensure_raw(widget_t* widget, const uint8_t** data, uint32_t* size) {
  gif_image_t* image = GIF_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  const char* name = image_base->image;
  const char* theme = NULL;
  int32_t status = GIF_RAW_NONE;
  return_value_if_fail(image != NULL && name != NULL && data != NULL && size != NULL,
                       RET_BAD_PARAMS);

  theme = image->gif_am != NULL && image->gif_am->theme != NULL ? image->gif_am->theme : "default";

  status = EM_ASM_INT({ return ImageLoader.getRawStatus(pointerToString($0), pointerToString($1)); },
                      name, theme);
  if (status == GIF_RAW_NONE || status == GIF_RAW_LOADING) {
    int32_t req = EM_ASM_INT(
        { return ImageLoader.requestRaw(pointerToString($0), pointerToString($1)); }, name, theme);
    return (req == GIF_RAW_FAIL) ? RET_FAIL : RET_BUSY;
  }
  if (status != GIF_RAW_READY) {
    return RET_FAIL;
  }

  *data = (const uint8_t*)EM_ASM_INT(
      { return ImageLoader.getRawPtr(pointerToString($0), pointerToString($1)); }, name, theme);
  *size = (uint32_t)EM_ASM_INT(
      { return ImageLoader.getRawSize(pointerToString($0), pointerToString($1)); }, name, theme);
  return (*data != NULL && *size > 0) ? RET_OK : RET_FAIL;
}
#endif /*AWTK_WEB*/

static ret_t gif_image_load_gif_stb(widget_t* widget, const char* name, bitmap_t* bitmap,
                                    bool_t is_fragmented_loading) {
  gif_image_t* image = GIF_IMAGE(widget);
#ifndef AWTK_WEB
  assets_manager_t* am = image->gif_am;
  locale_info_t* locale = locale_info();
  const char* region = NULL;
  const asset_info_t* res = NULL;
  char real_name[MAX_PATH + 1];
#endif /*AWTK_WEB*/

  return_value_if_fail(image != NULL && name != NULL && bitmap != NULL, RET_BAD_PARAMS);

#ifdef AWTK_WEB
  {
    const uint8_t* raw = NULL;
    uint32_t raw_size = 0;
    ret_t ret = RET_FAIL;
    ret_t raw_ret = RET_FAIL;

    (void)is_fragmented_loading;
    raw_ret = gif_image_ensure_raw(widget, &raw, &raw_size);
    if (raw_ret != RET_OK) {
      return raw_ret;
    }
    ret = stb_load_gif_next_frame(bitmap, &(image->gif_context), &(image->gif_msg),
                                  &(image->gif_cache), &(image->gif_delays), raw, raw_size,
                                  &(image->gif_layers), &(image->gif_on_end));
    if (ret == RET_OK) {
      bitmap_set_dirty(bitmap, TRUE);
    }
    return ret;
  }
#else
  if (am != NULL && am->locale_info != NULL) {
    locale = am->locale_info;
  }

  if (!is_fragmented_loading) {
    return widget_load_image(widget, name, bitmap);
  }

  if (image->gif_res == NULL) {
    region = strrchr(name, '#');
    if (region != NULL) {
      tk_strncpy(real_name, name, region - name);
      name = real_name;
    }
    if (strstr(name, TK_LOCALE_MAGIC) != NULL) {
      char locale_str[TK_NAME_LEN + 1];
      char real_name[TK_NAME_LEN + 1];
      const char* language = locale->language;
      const char* country = locale->country;

      if (strlen(language) > 0 && strlen(country) > 0) {
        tk_snprintf(locale_str, sizeof(locale_str) - 1, "%s_%s", language, country);
        tk_replace_locale(name, real_name, locale_str);
        res = assets_manager_ref(am, ASSET_TYPE_IMAGE, real_name);
        if (res != NULL) {
          image->gif_res = res;
          return stb_load_gif_next_frame(bitmap, &(image->gif_context), &(image->gif_msg),
                                         &(image->gif_cache), &(image->gif_delays), res->data,
                                         res->size, &(image->gif_layers), &(image->gif_on_end));
        }
      }

      tk_replace_locale(name, real_name, language);
      res = assets_manager_ref(am, ASSET_TYPE_IMAGE, real_name);
      if (res != NULL) {
        image->gif_res = res;
        return stb_load_gif_next_frame(bitmap, &(image->gif_context), &(image->gif_msg),
                                       &(image->gif_cache), &(image->gif_delays), res->data,
                                       res->size, &(image->gif_layers), &(image->gif_on_end));
      }

      tk_replace_locale(name, real_name, "");
      res = assets_manager_ref(am, ASSET_TYPE_IMAGE, real_name);
      if (res != NULL) {
        image->gif_res = res;
        return stb_load_gif_next_frame(bitmap, &(image->gif_context), &(image->gif_msg),
                                       &(image->gif_cache), &(image->gif_delays), res->data,
                                       res->size, &(image->gif_layers), &(image->gif_on_end));
      }

      return RET_FAIL;
    } else {
      res = assets_manager_ref(am, ASSET_TYPE_IMAGE, name);
      if (res == NULL) {
        image->gif_res = NULL;
        return RET_FAIL;
      }
      image->gif_res = res;
      return stb_load_gif_next_frame(bitmap, &(image->gif_context), &(image->gif_msg),
                                     &(image->gif_cache), &(image->gif_delays), res->data,
                                     res->size, &(image->gif_layers), &(image->gif_on_end));
    }
  } else {
    return stb_load_gif_next_frame(
        bitmap, &(image->gif_context), &(image->gif_msg), &(image->gif_cache), &(image->gif_delays),
        image->gif_res->data, image->gif_res->size, &(image->gif_layers), &(image->gif_on_end));
  }
#endif /*AWTK_WEB*/
}
#endif /*WITH_STB_IMAGE*/

static ret_t gif_image_load_gif(widget_t* widget, const char* name, bitmap_t* bitmap,
                                bool_t is_fragmented_loading) {
  gif_image_t* image = GIF_IMAGE(widget);
  return_value_if_fail(image != NULL && name != NULL && bitmap != NULL, RET_BAD_PARAMS);

#ifdef WITH_STB_IMAGE
  return gif_image_load_gif_stb(widget, name, bitmap, is_fragmented_loading);
#endif /*WITH_STB_IMAGE*/
  return widget_load_image(widget, name, bitmap);
}

static ret_t gif_image_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  gif_image_t* image = GIF_IMAGE(info->ctx);
  image_base_t* image_base = IMAGE_BASE(widget);
  bitmap_t* bitmap = &(image->bitmap);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  if (image->running) {
    image->index++;
    image->gif_update_flag = TRUE;
    if (image->part_buffer_load_mode) {
      gif_image_load_gif(widget, image_base->image, bitmap, image->part_buffer_load_mode);
    }
  }
  if (WIDGET(image)->visible) {
    widget_invalidate_force(WIDGET(image), NULL);
  }
  return RET_REPEAT;
}

static ret_t gif_image_paint_self_normal(widget_t* widget, canvas_t* c) {
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
  if (!bitmap.is_gif) {
    if (image->timer_id != TK_INVALID_ID) {
      image->index = 0;
      timer_remove(image->timer_id);
      image->timer_id = TK_INVALID_ID;
    }
    return RET_OK;
  }
  frames_nr = bitmap.gif_frames_nr;

  if (image->gif_update_flag) {
    if (frames_nr > 0) {
      image->index %= frames_nr;
      if (image->loop > 0 && image->index == frames_nr - 1 && !image->loop_done) {
        image->loop--;
      }
    } else {
      image->index = 0;
    }
    image->gif_update_flag = FALSE;
  }

  if (image->loop == 0) {
    if (frames_nr > 0) {
      image->index = frames_nr - 1;
    } else {
      image->index = 0;
    }
    image->loop_done = TRUE;
  }
  if (image->loop > 0 && frames_nr > 1) {
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
    timer_remove(image->timer_id);
    image->timer_id = TK_INVALID_ID;
  }

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

  widget_paint_helper(widget, c, NULL, NULL);

  return RET_OK;
}

static ret_t gif_image_paint_self_part_buffer_load(widget_t* widget, canvas_t* c) {
  wh_t y = 0;
  wh_t h = 0;
  rect_t src;
  rect_t dst;
  vgcanvas_t* vg = NULL;
  gif_image_t* image = GIF_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  bitmap_t* bitmap = &(image->bitmap);
  return_value_if_fail(image_base != NULL && image != NULL && widget != NULL && c != NULL,
                       RET_BAD_PARAMS);

  if (image_base->image == NULL || image_base->image[0] == '\0') {
    widget_paint_helper(widget, c, NULL, NULL);
    return RET_OK;
  }
  vg = canvas_get_vgcanvas(c);

  if (bitmap->buffer == NULL) {
    ret_t load_ret = gif_image_load_gif(widget, image_base->image, bitmap,
                                        image->part_buffer_load_mode);
    if (load_ret != RET_OK) {
      widget_paint_helper(widget, c, NULL, NULL);
      return RET_OK;
    }
  }

  if (image->gif_update_flag) {
    if (image->loop > 0 && image->gif_on_end && !image->loop_done) {
      image->loop--;
    }
    image->gif_update_flag = FALSE;
  }

  if (!bitmap->is_gif) {
    if (image->timer_id != TK_INVALID_ID) {
      timer_remove(image->timer_id);
      image->timer_id = TK_INVALID_ID;
    }
    return RET_OK;
  }

  if (image->loop == 0) {
    image->loop_done = TRUE;
  }

  if (image->loop > 0) {
    uint32_t delay;
    delay = image->gif_delays;
    if (image->timer_id == TK_INVALID_ID) {
      image->delay = delay;
      image->timer_id = timer_add(gif_image_on_timer, image, image->delay);
    } else if (image->delay != delay) {
      image->delay = delay;
      timer_info_t* timer = (timer_info_t*)timer_find(image->timer_id);
      if (timer) timer->duration = image->delay;
      if (image->gif_on_end) {
        ret_t load_ret = gif_image_load_gif(widget, image_base->image, bitmap,
                                            image->part_buffer_load_mode);
        if (load_ret != RET_OK && load_ret != RET_BUSY) {
          return RET_BAD_PARAMS;
        }
      }
    }
  } else if (image->timer_id != TK_INVALID_ID) {
    timer_remove(image->timer_id);
    image->timer_id = TK_INVALID_ID;
  }

  y = 0;
  h = bitmap->gif_frame_h;

#ifdef AWTK_WEB
  bitmap_set_dirty(bitmap, TRUE);
#endif /*AWTK_WEB*/

  if (vg != NULL) {
    if (image_need_transform(widget)) {
      vgcanvas_save(vg);
      image_transform(widget, c);
      vgcanvas_draw_icon(vg, bitmap, 0, y, bitmap->w, h, 0, 0, widget->w, widget->h);
      vgcanvas_restore(vg);

      return RET_OK;
    }
  }

  src = rect_init(0, y, bitmap->w, h);
  dst = rect_init(0, 0, widget->w, widget->h);
  canvas_draw_image_scale_down(c, bitmap, &src, &dst);
  widget_paint_helper(widget, c, NULL, NULL);

  return RET_OK;
}

static ret_t gif_image_on_paint_self(widget_t* widget, canvas_t* c) {
  gif_image_t* image = GIF_IMAGE(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

#ifdef WITH_STB_IMAGE
  if (image->part_buffer_load_mode) {
    return gif_image_paint_self_part_buffer_load(widget, c);
  } else {
    return gif_image_paint_self_normal(widget, c);
  }
#endif /*WITH_STB_IMAGE*/
  return gif_image_paint_self_normal(widget, c);
}

static const char* s_gif_image_properties[] = {WIDGET_PROP_IMAGE,
                                               WIDGET_PROP_SCALE_X,
                                               WIDGET_PROP_SCALE_Y,
                                               WIDGET_PROP_ANCHOR_X,
                                               WIDGET_PROP_ANCHOR_Y,
                                               WIDGET_PROP_ROTATION,
                                               WIDGET_PROP_CLICKABLE,
                                               WIDGET_PROP_SELECTABLE,
                                               WIDGET_PROP_LOOP,
                                               WIDGET_PROP_PART_BUFFER_LOAD_MODE,
                                               NULL};

static ret_t gif_image_on_destroy(widget_t* widget) {
  gif_image_t* image = GIF_IMAGE(widget);
  assets_manager_t* am = image->gif_am;
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  if (image->timer_id != TK_INVALID_ID) {
    timer_remove(image->timer_id);
    image->timer_id = TK_INVALID_ID;
  }
#ifdef WITH_STB_IMAGE
  if (image->part_buffer_load_mode) {
    stb_gif_frame_free(&(image->gif_context), &(image->gif_msg), &(image->gif_cache));
    bitmap_deinit(&(image->bitmap));
    if (image->gif_res != NULL) {
      assets_manager_unref(am, image->gif_res);
      image->gif_res = NULL;
    }
  }
#endif /*WITH_STB_IMAGE*/

  return image_base_on_destroy(widget);
}

ret_t gif_image_play(widget_t* widget) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  gif_image->running = TRUE;

  return RET_OK;
}

ret_t gif_image_pause(widget_t* widget) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  gif_image->running = FALSE;

  return RET_OK;
}

ret_t gif_image_stop(widget_t* widget) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  assets_manager_t* am = gif_image->gif_am;
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  gif_image->index = 0;
  gif_image->running = FALSE;
#ifdef WITH_STB_IMAGE
  if (gif_image->part_buffer_load_mode) {
    return_value_if_fail(am != NULL, RET_BAD_PARAMS);
    stb_gif_frame_reset(&(gif_image->gif_context), &(gif_image->gif_msg), &(gif_image->gif_cache),
                        FALSE);
    gif_image->gif_layers = 0;
    gif_image_load_gif(widget, image_base->image, &(gif_image->bitmap),
                       gif_image->part_buffer_load_mode);
  }
#endif /*WITH_STB_IMAGE*/

  return RET_OK;
}

ret_t gif_image_set_loop(widget_t* widget, uint32_t loop) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  if (gif_image->loop_done) {
    gif_image->index = 0;
    gif_image->loop_done = FALSE;
  }

  gif_image->loop = loop;

  return RET_OK;
}

ret_t gif_image_set_part_buffer_load_mode(widget_t* widget, bool_t part_buffer_load_mode) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

#ifdef AWTK_WEB
  if (!part_buffer_load_mode) {
    log_warn("gif_image: part_buffer_load_mode must stay TRUE on Web\n");
  }
  gif_image->part_buffer_load_mode = TRUE;
#else
#ifdef WITH_STB_IMAGE
  {
    assets_manager_t* am = gif_image->gif_am;
    image_base_t* image_base = IMAGE_BASE(widget);
    if (gif_image->part_buffer_load_mode && !part_buffer_load_mode) {
      stb_gif_frame_reset(&(gif_image->gif_context), &(gif_image->gif_msg), &(gif_image->gif_cache),
                          FALSE);
      assets_manager_unref(am, gif_image->gif_res);
      gif_image->gif_res = NULL;
    }
    if (part_buffer_load_mode) {
      gif_image_load_gif(widget, image_base->image, &(gif_image->bitmap), part_buffer_load_mode);
    }
  }
#endif /*WITH_STB_IMAGE*/
  gif_image->part_buffer_load_mode = part_buffer_load_mode;
#endif /*AWTK_WEB*/

  return RET_OK;
}

ret_t gif_image_set_name(widget_t* widget, const char* name) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  gif_image->index = 0;
  {
    ret_t set_ret = image_base_set_image(widget, name);
    return_value_if_fail(set_ret == RET_OK, set_ret);
  }

#ifdef WITH_STB_IMAGE
  if (gif_image->part_buffer_load_mode) {
    ret_t ret = RET_OK;
    stb_gif_frame_reset(&(gif_image->gif_context), &(gif_image->gif_msg), &(gif_image->gif_cache),
                        TRUE);
    if (gif_image->gif_res != NULL) {
      assets_manager_unref(gif_image->gif_am, gif_image->gif_res);
      gif_image->gif_res = NULL;
    }
    gif_image->gif_layers = 0;
    bitmap_deinit(&(gif_image->bitmap));
    memset(&(gif_image->bitmap), 0x00, sizeof(bitmap_t));
    ret = gif_image_load_gif(widget, name, &(gif_image->bitmap), gif_image->part_buffer_load_mode);
    if (ret == RET_BUSY) {
      return RET_OK;
    }
    (void)ret;
  }
#endif /*WITH_STB_IMAGE*/

  return RET_OK;
}

static ret_t gif_image_get_prop(widget_t* widget, const char* name, value_t* v) {
  gif_image_t* image = GIF_IMAGE(widget);
  return_value_if_fail(image != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_RUNNING)) {
    value_set_bool(v, image->running);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOOP)) {
    value_set_uint32(v, image->loop);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_PART_BUFFER_LOAD_MODE)) {
    value_set_bool(v, image->part_buffer_load_mode);
    return RET_OK;
  } else {
    return image_base_get_prop(widget, name, v);
  }
}

static ret_t gif_image_set_prop(widget_t* widget, const char* name, const value_t* v) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_RUNNING)) {
    gif_image->running = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOOP)) {
    return gif_image_set_loop(widget, value_uint32(v));
  } else if (tk_str_eq(name, WIDGET_PROP_PART_BUFFER_LOAD_MODE)) {
    return gif_image_set_part_buffer_load_mode(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return gif_image_set_name(widget, value_str(v));
  } else {
    return image_base_set_prop(widget, name, v);
  }
}

static ret_t gif_image_init(widget_t* widget) {
  gif_image_t* gif_image = GIF_IMAGE(widget);
  return_value_if_fail(gif_image != NULL, RET_BAD_PARAMS);

  image_base_init(widget);
  gif_image_play(widget);
  gif_image->loop = 0xffffffff;
  gif_image->loop_done = FALSE;
  gif_image->gif_update_flag = TRUE;
  memset(&(gif_image->bitmap), 0, sizeof(bitmap_t));
  gif_image->gif_am = widget_get_assets_manager(widget);
#ifdef AWTK_WEB
  gif_image->part_buffer_load_mode = TRUE;
#endif /*AWTK_WEB*/
  stb_gif_frame_create(&(gif_image->gif_context), &(gif_image->gif_msg));

  return RET_OK;
}

TK_DECL_VTABLE(gif_image) = {.size = sizeof(gif_image_t),
                             .type = WIDGET_TYPE_GIF_IMAGE,
                             .space_key_to_activate = TRUE,
                             .return_key_to_activate = TRUE,
                             .clone_properties = s_gif_image_properties,
                             .persistent_properties = s_gif_image_properties,
                             .get_parent_vt = TK_GET_PARENT_VTABLE(image_base),
                             .create = gif_image_create,
                             .init = gif_image_init,
                             .on_destroy = gif_image_on_destroy,
                             .on_event = image_base_on_event,
                             .on_paint_self = gif_image_on_paint_self,
                             .on_copy = image_base_on_copy,
                             .set_prop = gif_image_set_prop,
                             .get_prop = gif_image_get_prop};

widget_t* gif_image_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(gif_image), x, y, w, h);
  return_value_if_fail(gif_image_init(widget) == RET_OK, NULL);
  return widget;
}

widget_t* gif_image_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, gif_image), NULL);

  return widget;
}

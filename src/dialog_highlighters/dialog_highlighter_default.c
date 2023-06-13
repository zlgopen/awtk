/**
 * File:   dialog_highlighter.c
 * Author: AWTK Develop Team
 * Brief:  dialog_highlighter
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget_vtable.h"
#include "base/dialog_highlighter.h"
#include "dialog_highlighters/dialog_highlighter_default.h"
#include "window_manager/window_manager_default.h"

static ret_t dialog_highlighter_default_draw_mask(canvas_t* c, uint8_t alpha) {
  if (alpha > 1) {
    wh_t w = canvas_get_width(c);
    wh_t h = canvas_get_height(c);
    color_t mask = color_init(0, 0, 0, alpha);

    canvas_set_fill_color(c, mask);
    canvas_fill_rect(c, 0, 0, w, h);
  }

  return RET_OK;
}

static ret_t dialog_highlighter_default_get_window_rect_diff_on_visit(void* ctx, const void* data) {
  uint32_t i = 0;
  rect_t diff_rects[4];
  void** arges = (void**)ctx;
  rect_t* rect = (rect_t*)data;
  rect_t* r = (rect_t*)arges[1];
  slist_t* diff_rect_list = (slist_t*)arges[0];
  /* 显示裁剪区和 system_bar 差集，如果存在差集的话，就意味着相交了，把相交部分排除，留下差集部分 */
  if (rect_diff(rect, r, &diff_rects[0], &diff_rects[1], &diff_rects[2], &diff_rects[3])) {
    for (i = 0; i < ARRAY_SIZE(diff_rects); i++) {
      if (diff_rects[i].w != 0 && diff_rects[i].h != 0) {
        rect_t* data = TKMEM_ZALLOC(rect_t);
        break_if_fail(data != NULL);
        memcpy(data, &diff_rects[i], sizeof(rect_t));
        slist_append(diff_rect_list, data);
      }
    }
    return RET_REMOVE;
  }
  return RET_OK;
}

static ret_t dialog_highlighter_default_get_window_rect_diff(slist_t* rect_list, rect_t* r) {
  void* arges[2];
  void* iter = NULL;
  slist_t diff_rect_list;
  slist_init(&diff_rect_list, NULL, NULL);
  arges[0] = &diff_rect_list;
  arges[1] = r;
  slist_foreach(rect_list, dialog_highlighter_default_get_window_rect_diff_on_visit, arges);
  if (!slist_is_empty(&diff_rect_list)) {
    while ((iter = slist_head_pop(&diff_rect_list)) != NULL) {
      slist_append(rect_list, iter);
    }
  }
  slist_deinit(&diff_rect_list);
  return RET_OK;
}

static ret_t dialog_highlighter_default_draw_window_rect_diff_on_visit(void* ctx, const void* data) {
  void** arges = (void**)ctx;
  canvas_t* c = arges[0];
  rect_t* window_rect = (rect_t*)data;
  uint8_t alpha = (uint8_t)tk_pointer_to_int(arges[1]);
  color_t mask = color_init(0, 0, 0, alpha);
  canvas_set_fill_color(c, mask);
  canvas_fill_rect(c, window_rect->x, window_rect->y, window_rect->w, window_rect->h);
  return RET_OK;
}

static ret_t dialog_highlighter_default_draw_mask_window(dialog_highlighter_t* h, canvas_t* c, uint8_t alpha) {
  if (alpha > 1) {
    uint32_t i = 0;
    void* arges[2];
    widget_t* widget = window_manager();
    dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

    if (slist_is_empty(&dh->win_mask_rect_list)) {
      /* 排除掉显示裁剪区中的包括的 system_bar 的区域 */
      slist_append(&dh->win_mask_rect_list, rect_create(h->clip_rect.x, h->clip_rect.y, h->clip_rect.w, h->clip_rect.h));
      for (i = 0; i < dh->system_bar_top_clip_rects.size; i++) {
        rect_t* r = (rect_t*)darray_get(&dh->system_bar_top_clip_rects, i);
        dialog_highlighter_default_get_window_rect_diff(&dh->win_mask_rect_list, r);
      }
      for (i = 0; i < dh->system_bar_bottom_clip_rects.size; i++) {
        rect_t* r = (rect_t*)darray_get(&dh->system_bar_bottom_clip_rects, i);
        dialog_highlighter_default_get_window_rect_diff(&dh->win_mask_rect_list, r);
      }
    }

    arges[0] = c;
    arges[1] = tk_pointer_from_int(alpha);
    slist_foreach(&dh->win_mask_rect_list, dialog_highlighter_default_draw_window_rect_diff_on_visit, arges);
  }

  return RET_OK;
}

static ret_t dialog_highlighter_default_draw_mask_system_bar(dialog_highlighter_t* h, canvas_t* c, uint8_t alpha, bool_t is_clip_rect) {
  if (alpha > 1) {
    uint32_t i = 0;
    widget_t* widget = window_manager();
    color_t mask = color_init(0, 0, 0, alpha);
    dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
    for (i = 0; i < dh->system_bar_top_clip_rects.size; i++) {
      rect_t* r = (rect_t*)darray_get(&dh->system_bar_top_clip_rects, i);
      if (is_clip_rect) {
        /* 因为显示裁剪区是会包括一些 system_bar 的区域的，只要重叠了就意味着这个区域被画到显示裁剪区中了，所以就跳过绘图 */
        rect_t in = rect_intersect(r, &h->clip_rect);
        if (in.w != 0 && in.h != 0) {
          continue;
        }
      }
      canvas_set_fill_color(c, mask);
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
    for (i = 0; i < dh->system_bar_bottom_clip_rects.size; i++) {
      rect_t* r = (rect_t*)darray_get(&dh->system_bar_bottom_clip_rects, i);
      if (is_clip_rect) {
        /* 因为显示裁剪区是会包括一些 system_bar 的区域的，只要重叠了就意味着这个区域被画到显示裁剪区中了，所以就跳过绘图 */
        rect_t in = rect_intersect(r, &h->clip_rect);
        if (in.w != 0 && in.h != 0) {
          continue;
        }
      }
      canvas_set_fill_color(c, mask);
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  return RET_OK;
}

static ret_t dialog_highlighter_default_draw_system_bar(dialog_highlighter_t* h, canvas_t* c) {
  uint32_t i = 0;
  widget_t* widget = window_manager();
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, index)
    if (iter->vt->is_window) {
      if (tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR)) {
        for (i = 0; i < dh->system_bar_top_clip_rects.size; i++) {
          rect_t* r = (rect_t*)darray_get(&dh->system_bar_top_clip_rects, i);
          widget_paint_with_clip(iter, r, c, widget_paint);
        }
      } else if(tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM)) {
        for (i = 0; i < dh->system_bar_bottom_clip_rects.size; i++) {
          rect_t* r = (rect_t*)darray_get(&dh->system_bar_bottom_clip_rects, i);
          widget_paint_with_clip(iter, r, c, widget_paint);
        }
      }
    }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t dialog_highlighter_default_draw_mask_from_percent(dialog_highlighter_t* h, canvas_t* c,
                                                               float_t percent) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

  return_value_if_fail(dh != NULL && c != NULL, RET_BAD_PARAMS);

  if (dh->start_alpha != dh->end_alpha) {
    uint8_t a = ((dh->end_alpha - dh->start_alpha) * percent) + dh->start_alpha;
    return dialog_highlighter_default_draw_mask(c, a);
  } else {
    return dialog_highlighter_default_draw_mask(c, dh->end_alpha);
  }
}

ret_t dialog_highlighter_default_set_system_bar_alpha(dialog_highlighter_t* h, uint8_t alpha) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(dh != NULL, RET_BAD_PARAMS);
  dh->system_bar_alpha = 0xff - alpha;
  return RET_OK;
}

uint8_t dialog_highlighter_default_get_alpha(dialog_highlighter_t* h, float_t percent) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(dh != NULL, 0x0);

  if (dh->start_alpha == dh->end_alpha) {
    return dh->start_alpha;
  } else {
    return (0xff * (dh->end_alpha - dh->start_alpha) * percent) / 0xff;
  }
}

static ret_t dialog_highlighter_default_prepare(dialog_highlighter_t* h, canvas_t* c) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(h != NULL && h->vt != NULL && c != NULL, RET_BAD_PARAMS);

  /*
   * optimization:
   * if dh->start_alpha == dh->end_alpha, we draw mask layer only at the first time.
   */
  if (dh->start_alpha == dh->end_alpha) {
    dialog_highlighter_default_draw_mask(c, dh->start_alpha);
  }
  dialog_highlighter_default_draw_system_bar(h, c);

  return RET_OK;
}

static ret_t dialog_highlighter_default_draw(dialog_highlighter_t* h, float_t percent) {
  rect_t r;
  rect_t src;
  rect_t dst;
  rect_t save_r;
  canvas_t* c;
  bitmap_t* img;
  bool_t is_window_animator = FALSE;
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

  if (percent == 1 && dh->update_background) {
    /* if window is open, update prev win image */
    bitmap_t prev_img = {0};
    widget_t* wm = window_manager();
    widget_t* prev_win = h->win != NULL ? h->win : window_manager_get_prev_window(wm);
    if (widget_set_prop_pointer(wm, WIDGET_PROP_CURR_WIN, h->dialog) == RET_OK) {
      window_manager_snap_prev_window(wm, prev_win, &prev_img);
    }
  }

  c = h->canvas;
  img = &(h->img);
  src = rect_init(0, 0, img->w, img->h);
  dst = rect_init(0, 0, canvas_get_width(c), canvas_get_height(c));

  if (percent == 1) {
    /* if window is open, enable clip and draw system bar */
    is_window_animator = FALSE;
    canvas_draw_image(c, img, &src, &dst);
    dialog_highlighter_default_draw_system_bar(h, c);
  } else {
    is_window_animator = TRUE;
    canvas_get_clip_rect(c, &save_r);
    r = rect_intersect(&save_r, &h->clip_rect);
    canvas_save(c);
    canvas_set_clip_rect(c, &r);
    canvas_draw_image(c, img, &src, &dst);
    canvas_set_clip_rect(c, &save_r);
    canvas_restore(c);
  }

  /*
  * only if dh->start_alpha != dh->end_alpha, we draw mask layer very time.
  */
  if (dh->start_alpha != dh->end_alpha) {
    uint8_t alpha = ((dh->end_alpha - dh->start_alpha) * percent) + dh->start_alpha;
    uint8_t a = 0xff - ((dh->system_bar_alpha * (0xff - alpha)) >> 8);
    dialog_highlighter_default_draw_mask_window(h, c, alpha);
    dialog_highlighter_default_draw_mask_system_bar(h, c, a, h->used_by_others && is_window_animator);
  } else {
    /* 解决黑色色块绘制到贴图导致 system_bar 的颜色不同步的问题 */
    uint8_t a = 0xff - ((dh->system_bar_alpha * (0xff - dh->end_alpha)) >> 8);
    dialog_highlighter_default_draw_mask_system_bar(h, c, a, h->used_by_others && is_window_animator);
  }

  return RET_OK;
}

static bool_t dialog_highlighter_default_is_dynamic(dialog_highlighter_t* h) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

  return (dh->start_alpha != dh->end_alpha);
}

static ret_t dialog_highlighter_default_system_bar_top_append_clip_rect(dialog_highlighter_t* h, rect_t* rect) {
  ret_t ret = RET_OK;
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  if (rect->w != 0 && rect->h != 0) {
    rect_t* r = TKMEM_ZALLOC(rect_t);
    memcpy(r, rect, sizeof(rect_t));
    ret = darray_push(&dh->system_bar_top_clip_rects, r);
  }
  return ret;
}

static ret_t dialog_highlighter_default_system_bar_bottom_append_clip_rect(dialog_highlighter_t* h, rect_t* rect) {
  ret_t ret = RET_OK;
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  if (rect->w != 0 && rect->h != 0) {
    rect_t* r = TKMEM_ZALLOC(rect_t);
    memcpy(r, rect, sizeof(rect_t));
    ret = darray_push(&dh->system_bar_bottom_clip_rects, r);
  }
  return ret;
}

static ret_t dialog_highlighter_default_on_destroy(dialog_highlighter_t* h) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  slist_deinit(&dh->win_mask_rect_list);
  darray_deinit(&dh->system_bar_top_clip_rects);
  darray_deinit(&dh->system_bar_bottom_clip_rects);
  return RET_OK;
}

static ret_t dialog_highlighter_default_set_bg_clip_rect(dialog_highlighter_t* h, rect_t* rect) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  darray_clear(&dh->system_bar_top_clip_rects);
  darray_clear(&dh->system_bar_bottom_clip_rects);
  slist_remove_all(&dh->win_mask_rect_list);
  h->clip_rect = rect_init(rect->x, rect->y, rect->w, rect->h);
  return RET_OK;
}

static const dialog_highlighter_vtable_t s_dialog_highlighter_default_vt = {
    .type = "dialog_highlighter_default_t",
    .desc = "dialog_highlighter_default_t",
    .size = sizeof(dialog_highlighter_default_t),
    .prepare = dialog_highlighter_default_prepare,
    .draw_mask = dialog_highlighter_default_draw_mask_from_percent,
    .set_system_bar_alpha = dialog_highlighter_default_set_system_bar_alpha,
    .set_bg_clip_rect = dialog_highlighter_default_set_bg_clip_rect,
    .on_destroy = dialog_highlighter_default_on_destroy,
    .system_bar_bottom_append_clip_rect = dialog_highlighter_default_system_bar_bottom_append_clip_rect,
    .system_bar_top_append_clip_rect = dialog_highlighter_default_system_bar_top_append_clip_rect,
    .get_alpha = dialog_highlighter_default_get_alpha,
    .is_dynamic = dialog_highlighter_default_is_dynamic,
    .draw = dialog_highlighter_default_draw};

dialog_highlighter_t* dialog_highlighter_default_create(tk_object_t* args) {
  value_t v;
  dialog_highlighter_t* h = dialog_highlighter_create(&s_dialog_highlighter_default_vt);
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(h != NULL, NULL);

  dh->end_alpha = 0;
  dh->start_alpha = 0;
  dh->system_bar_alpha = 0xff;
  dh->update_background = FALSE;

  slist_init(&dh->win_mask_rect_list, default_destroy, NULL);
  darray_init(&dh->system_bar_top_clip_rects, 4, default_destroy, NULL);
  darray_init(&dh->system_bar_bottom_clip_rects, 4, default_destroy, NULL);

  if (tk_object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_ALPHA, &v) == RET_OK) {
    dh->start_alpha = value_int(&v);
    dh->end_alpha = value_int(&v);
  }

  if (tk_object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_START_ALPHA, &v) == RET_OK) {
    dh->start_alpha = value_int(&v);
  }

  if (tk_object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_END_ALPHA, &v) == RET_OK) {
    dh->end_alpha = value_int(&v);
  }

  if (tk_object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_UPDATE_BACKGROUND, &v) == RET_OK) {
    dh->update_background = value_bool(&v);
  }

  return h;
}

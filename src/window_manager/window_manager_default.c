/**
 * File:   window_manager_default.c
 * Author: AWTK Develop Team
 * Brief:  default window manager
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/keys.h"
#include "tkc/mem.h"
#include "base/idle.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/layout.h"
#include "tkc/time_now.h"
#include "base/dialog.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/input_method.h"
#include "base/image_manager.h"
#include "base/canvas_offline.h"
#include "base/dirty_rects.inc"
#include "base/dialog_highlighter_factory.h"
#include "window_manager/window_manager_default.h"

static ret_t window_manager_animate_done(widget_t* widget);
static ret_t window_manager_default_update_fps(widget_t* widget);
static ret_t window_manager_invalidate_system_bar(widget_t* widget);
static ret_t window_manager_default_reset_window_animator(widget_t* widget);
static ret_t window_manager_default_reset_dialog_highlighter(widget_t* widget);
static ret_t window_manager_default_invalidate(widget_t* widget, const rect_t* r);
static ret_t window_manager_default_get_client_r(widget_t* widget, rect_t* r);
static ret_t window_manager_default_do_open_window(widget_t* wm, widget_t* window);
static ret_t window_manager_default_layout_child(widget_t* widget, widget_t* window);
static ret_t window_manager_default_paint_always_on_top(widget_t* widget, canvas_t* c);
static ret_t window_manager_default_layout_system_bar(widget_t* widget, widget_t* window);
static ret_t window_manager_default_create_dialog_highlighter(widget_t* widget, widget_t* curr_win);
static ret_t window_manager_default_layout_not_system_bar(widget_t* widget, widget_t* window,
                                                          rect_t client_r);

static bool_t window_is_opened(widget_t* widget) {
  int32_t stage = widget_get_prop_int(widget, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);

  return stage == WINDOW_STAGE_OPENED || stage == WINDOW_STAGE_SUSPEND;
}

static ret_t wm_on_screen_saver_timer(const timer_info_t* info) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(info->ctx);
  event_t e = event_init(EVT_SCREEN_SAVER, wm);
  wm->screen_saver_timer_id = TK_INVALID_ID;

  emitter_dispatch(WINDOW_MANAGER(wm)->global_emitter, &e);
  widget_dispatch(WIDGET(wm), &e);
  log_debug("emit: EVT_SCREEN_SAVER\n");

  return RET_REMOVE;
}

static ret_t window_manager_start_or_reset_screen_saver_timer(window_manager_default_t* wm) {
  if (wm->screen_saver_time > 0) {
    if (wm->screen_saver_timer_id == TK_INVALID_ID) {
      wm->screen_saver_timer_id = timer_add(wm_on_screen_saver_timer, wm, wm->screen_saver_time);
    } else {
      timer_modify(wm->screen_saver_timer_id, wm->screen_saver_time);
    }
  } else {
    if (wm->screen_saver_timer_id != TK_INVALID_ID) {
      timer_remove(wm->screen_saver_timer_id);
      wm->screen_saver_timer_id = TK_INVALID_ID;
    }
  }

  return RET_OK;
}

static widget_t* window_manager_find_prev_window(widget_t* widget) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
    for (i = nr - 2; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (widget_is_normal_window(iter) || widget_is_dialog(iter) || widget_is_popup(iter) ||
          (widget_is_overlay(iter) && !widget_is_always_on_top(iter))) {
        return iter;
      }
    }
  }

  return NULL;
}

static widget_t* window_manager_find_prev_normal_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size >= 2) {
    WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
    if (i <= widget->children->size - 2 && widget_is_normal_window(iter) && iter->visible) {
      return iter;
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return NULL;
}

static ret_t window_manager_default_set_paint_system_bar_by_window_animator(widget_t* widget,
                                                                            rect_t* rect) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR) && !wm->is_animator_paint_system_bar_top) {
    rect_t r = rect_init(iter->x, iter->y, iter->w, iter->h);
    rect_t dr = rect_intersect(rect, &r);
    if (dr.w == 0 || dr.h == 0) {
      wm->is_animator_paint_system_bar_top = TRUE;
    } else if (dr.w == r.w && dr.h == r.h) {
      wm->is_animator_paint_system_bar_top = FALSE;
    } else {
      wm->is_animator_paint_system_bar_top = TRUE;
    }
  } else if (tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM) &&
             !wm->is_animator_paint_system_bar_bottom) {
    rect_t r = rect_init(iter->x, iter->y, iter->w, iter->h);
    rect_t dr = rect_intersect(rect, &r);
    if (dr.w == 0 || dr.h == 0) {
      wm->is_animator_paint_system_bar_bottom = TRUE;
    } else if (dr.w == r.w && dr.h == r.h) {
      wm->is_animator_paint_system_bar_bottom = FALSE;
    } else {
      wm->is_animator_paint_system_bar_bottom = TRUE;
    }
  }
  WIDGET_FOR_EACH_CHILD_END()
  return RET_OK;
}

ret_t window_manager_default_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img) {
#ifndef WITHOUT_WINDOW_ANIMATORS
  canvas_t* c = NULL;
  rect_t r = {0};
  canvas_t* canvas = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(img != NULL && wm != NULL && curr_win != NULL, RET_BAD_PARAMS);

  c = native_window_get_canvas(wm->native_window);
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);
  window_manager_check_and_layout(widget);

  r = rect_init(curr_win->x, curr_win->y, curr_win->w, curr_win->h);

  canvas_save(c);
  canvas = canvas_offline_create(lcd_get_physical_width(c->lcd), lcd_get_physical_height(c->lcd),
                                 lcd_get_desired_bitmap_format(c->lcd));

  canvas_offline_begin_draw(canvas);
  canvas_set_clip_rect(canvas, &r);
  ENSURE(widget_on_paint_background(widget, canvas) == RET_OK);
  ENSURE(widget_paint(curr_win, canvas) == RET_OK);
  canvas_offline_end_draw(canvas);
  ENSURE(canvas_offline_bitmap_move_to_new_bitmap(canvas, img) == RET_OK);
  ENSURE(canvas_offline_destroy(canvas) == RET_OK);
  window_manager_default_set_paint_system_bar_by_window_animator(widget, &r);
  img->flags |= BITMAP_FLAG_OPAQUE;
  canvas_restore(c);
  /* 清除在线画布的缓存，确保绘制完窗口动画后，lcd对象的数据能和在线画布同步 */
  canvas_reset_cache(c);
#endif
  return RET_OK;
}

#ifndef WITHOUT_WINDOW_ANIMATORS
static ret_t window_manager_default_snap_prev_window_draw_dialog_highlighter_and_get_alpha(
    widget_t* widget, canvas_t* c, uint8_t* alpha) {
  value_t v;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  if (widget_get_prop(widget, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
    const char* args = value_str(&v);
    if (args != NULL && *args != '\0') {
      dialog_highlighter_factory_t* f = dialog_highlighter_factory();
      dialog_highlighter_t* dialog_highlighter =
          dialog_highlighter_factory_create_highlighter(f, args, widget);

      if (dialog_highlighter != NULL) {
        dialog_highlighter_draw_mask(dialog_highlighter, c, 1.0f);
        *alpha = dialog_highlighter_get_alpha(dialog_highlighter, 1.0f);
        widget_off_by_func(widget, EVT_DESTROY, dialog_highlighter_on_dialog_destroy,
                           dialog_highlighter);
        dialog_highlighter_destroy(dialog_highlighter);
        return RET_OK;
      }
    }
  }
  return RET_FAIL;
}

static ret_t window_manager_default_snap_prev_window_get_system_bar_rect(
    widget_t* widget, slist_t* system_bar_top_list, slist_t* system_bar_bottom_list) {
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->vt->is_window) {
    if (tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR)) {
      rect_t* r = rect_create(iter->x, iter->y, iter->w, iter->h);
      slist_append(system_bar_top_list, r);
    } else if (tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM)) {
      rect_t* r = rect_create(iter->x, iter->y, iter->w, iter->h);
      slist_append(system_bar_bottom_list, r);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()
  return RET_OK;
}

static bool_t window_manager_default_snap_prev_window_check_paint_system_bar(
    slist_t* diff_rect_list) {
  bool_t su = FALSE;
  rect_t* rect = NULL;
  slist_node_t* iter = NULL;
  iter = diff_rect_list->first;
  while (iter != NULL) {
    rect = (rect_t*)iter->data;
    if (rect->w != 0 && rect->h != 0) {
      su = TRUE;
      break;
    }
    iter = iter->next;
  }
  return su;
}

static ret_t window_manager_default_snap_prev_window_get_system_bar_rect_diff_on_visit(
    void* ctx, const void* data) {
  uint32_t i = 0;
  rect_t diff_rects[4];
  void** arges = (void**)ctx;
  rect_t* rect = (rect_t*)data;
  rect_t* r = (rect_t*)arges[1];
  slist_t* diff_rect_list = (slist_t*)arges[0];
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

static ret_t window_manager_default_snap_prev_window_get_system_bar_rect_diff(slist_t* rect_list,
                                                                              rect_t* r) {
  void* arges[2];
  void* iter = NULL;
  slist_t diff_rect_list;
  slist_init(&diff_rect_list, NULL, NULL);
  arges[0] = &diff_rect_list;
  arges[1] = r;
  slist_foreach(rect_list,
                window_manager_default_snap_prev_window_get_system_bar_rect_diff_on_visit, arges);
  if (!slist_is_empty(&diff_rect_list)) {
    while ((iter = slist_head_pop(&diff_rect_list)) != NULL) {
      slist_append(rect_list, iter);
    }
  }
  slist_deinit(&diff_rect_list);
  return RET_OK;
}

static ret_t window_manager_default_snap_prev_window_system_bar_top_push_clip_rect(
    void* ctx, const void* data) {
  rect_t* r = (rect_t*)data;
  dialog_highlighter_t* dialog_highlighter = (dialog_highlighter_t*)ctx;
  dialog_highlighter_system_bar_top_append_clip_rect(dialog_highlighter, r);
  return RET_OK;
}

static ret_t window_manager_default_snap_prev_window_system_bar_bottom_push_clip_rect(
    void* ctx, const void* data) {
  rect_t* r = (rect_t*)data;
  dialog_highlighter_t* dialog_highlighter = (dialog_highlighter_t*)ctx;
  dialog_highlighter_system_bar_bottom_append_clip_rect(dialog_highlighter, r);
  return RET_OK;
}

#endif

ret_t window_manager_default_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img) {
#ifndef WITHOUT_WINDOW_ANIMATORS
  rect_t r = {0};
  uint8_t alpha = 0xFF;
  canvas_t* c = NULL;
  canvas_t* canvas = NULL;
  int32_t end = -1, start = -1;
  bool_t is_fullscreen_window = FALSE;
  dialog_highlighter_t* dialog_highlighter = NULL;
  slist_t system_bar_top_rect_list, system_bar_bottom_rect_list;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  const char* curr_highlight = widget_get_prop_str(wm->curr_win, WIDGET_PROP_HIGHLIGHT, NULL);

  return_value_if_fail(img != NULL && wm != NULL && prev_win != NULL, RET_BAD_PARAMS);

  c = native_window_get_canvas(wm->native_window);
  return_value_if_fail(c != NULL && c->lcd != NULL, RET_BAD_PARAMS);
  dialog_highlighter = wm->dialog_highlighter;
  window_manager_check_and_layout(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter == prev_win) {
    start = i;
    end = widget->children->size - 2;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  is_fullscreen_window = widget_is_fullscreen_window(prev_win);
  r = rect_init(prev_win->x, prev_win->y, prev_win->w, prev_win->h);

  canvas_save(c);
  canvas = canvas_offline_create(lcd_get_physical_width(c->lcd), lcd_get_physical_height(c->lcd),
                                 lcd_get_desired_bitmap_format(c->lcd));

  canvas_offline_begin_draw(canvas);
  canvas_set_clip_rect(canvas, NULL);
  ENSURE(widget_on_paint_background(widget, canvas) == RET_OK);
  if (!is_fullscreen_window) {
    window_manager_paint_system_bar(widget, canvas);
  }
  {
    widget_t** children = (widget_t**)(widget->children->elms);
    slist_init(&system_bar_top_rect_list, default_destroy, NULL);
    slist_init(&system_bar_bottom_rect_list, default_destroy, NULL);
    if (!is_fullscreen_window) {
      window_manager_default_snap_prev_window_get_system_bar_rect(widget, &system_bar_top_rect_list,
                                                                  &system_bar_bottom_rect_list);
    }
    for (; start <= end; ++start) {
      widget_t* iter = children[start];
      if (widget_is_system_bar(iter) || !iter->visible) continue;
      /* 过滤 curr_win 的对象 */
      if (iter != wm->curr_win) {
        rect_t iter_rect = rect_init(iter->x, iter->y, iter->w, iter->h);
        /* 给前面的高亮对话框叠加黑色色块 */
        if (widget_is_support_highlighter(iter)) {
          uint8_t a = 0x0;
          if (window_manager_default_snap_prev_window_draw_dialog_highlighter_and_get_alpha(
                  iter, canvas, &a) == RET_OK) {
            /* 计算最终叠加后的透明度值 */
            alpha = alpha * (1 - a / 255.0f);
          }
        }
        /* 如果不是全屏的话，就削减 system_bar 的显示裁剪区 */
        if (!is_fullscreen_window && !widget_is_normal_window(iter)) {
          window_manager_default_snap_prev_window_get_system_bar_rect_diff(
              &system_bar_top_rect_list, &iter_rect);
          window_manager_default_snap_prev_window_get_system_bar_rect_diff(
              &system_bar_bottom_rect_list, &iter_rect);
        }
        rect_merge(&r, &iter_rect);
        ENSURE(widget_paint(iter, canvas) == RET_OK);
      }
    }
    /* 检查是否还有 system_bar 的显示区域，如果有则让其绘图 */
    wm->is_animator_paint_system_bar_top =
        window_manager_default_snap_prev_window_check_paint_system_bar(&system_bar_top_rect_list);
    wm->is_animator_paint_system_bar_bottom =
        window_manager_default_snap_prev_window_check_paint_system_bar(
            &system_bar_bottom_rect_list);
  }

  if (dialog_highlighter != NULL) {
    dialog_highlighter_set_bg_clip_rect(dialog_highlighter, &r);
    if (!is_fullscreen_window) {
      /* 把 system_bar 的显示裁剪区域追加到高亮对象中 */
      slist_foreach(&system_bar_top_rect_list,
                    window_manager_default_snap_prev_window_system_bar_top_push_clip_rect,
                    dialog_highlighter);
      slist_foreach(&system_bar_bottom_rect_list,
                    window_manager_default_snap_prev_window_system_bar_bottom_push_clip_rect,
                    dialog_highlighter);
    }
    if (curr_highlight != NULL) {
      dialog_highlighter_set_system_bar_alpha(dialog_highlighter, 0xFF - alpha);
      dialog_highlighter_set_win(dialog_highlighter, prev_win);
      /* 把没有遮罩的 system_bar 绘制到离线画布上 */
      dialog_highlighter_prepare_ex(dialog_highlighter, c, canvas);
    }
  }
  canvas_offline_end_draw(canvas);
  ENSURE(canvas_offline_bitmap_move_to_new_bitmap(canvas, img) == RET_OK);
  ENSURE(canvas_offline_destroy(canvas) == RET_OK);
  img->flags |= BITMAP_FLAG_OPAQUE;
  canvas_restore(c);

  if (dialog_highlighter != NULL) {
    dialog_highlighter_set_bg(dialog_highlighter, img);
  }
  slist_deinit(&system_bar_top_rect_list);
  slist_deinit(&system_bar_bottom_rect_list);
  wm->last_curr_win = wm->curr_win;
  wm->curr_win = NULL;
#endif
  return RET_OK;
}

static ret_t window_manager_on_highlighter_destroy(void* ctx, event_t* e) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(ctx);

  if (wm->dialog_highlighter == e->target) {
    wm->dialog_highlighter = NULL;
  }

  return RET_OK;
}

static dialog_highlighter_t* window_manager_default_get_dialog_highlighter(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return wm->dialog_highlighter;
}

static ret_t window_manager_default_dialog_highlighter_destroy(widget_t* widget) {
  dialog_highlighter_t* dialog_highlighter = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  dialog_highlighter = wm->dialog_highlighter;

  if (dialog_highlighter != NULL) {
    widget_t* dialog = dialog_highlighter->dialog;
    if (dialog != NULL) {
      widget_off_by_func(dialog, EVT_DESTROY, dialog_highlighter_on_dialog_destroy,
                         dialog_highlighter);
    }
    dialog_highlighter_destroy(dialog_highlighter);
    wm->dialog_highlighter = NULL;
  }
  return RET_OK;
}

static ret_t window_manager_default_create_dialog_highlighter(widget_t* widget,
                                                              widget_t* curr_win) {
  ret_t ret = RET_FAIL;
  dialog_highlighter_t* dialog_highlighter = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  const char* curr_highlight = widget_get_prop_str(curr_win, WIDGET_PROP_HIGHLIGHT, NULL);

  dialog_highlighter = wm->dialog_highlighter;
  if (dialog_highlighter != NULL && curr_highlight != NULL &&
      (dialog_highlighter->dialog != curr_win ||
       dialog_highlighter->used_by_others)) { /* dialog_highlighter被其他窗口使用过，需重新生成 */
    window_manager_default_dialog_highlighter_destroy(widget);
    dialog_highlighter = NULL;
  }

  if (dialog_highlighter == NULL && widget_is_support_highlighter(curr_win) &&
      curr_highlight != NULL && *curr_highlight != '\0') {
    dialog_highlighter_factory_t* f = dialog_highlighter_factory();
    dialog_highlighter = dialog_highlighter_factory_create_highlighter(f, curr_highlight, curr_win);

    if (dialog_highlighter != NULL) {
      wm->dialog_highlighter = dialog_highlighter;

      emitter_on(EMITTER(dialog_highlighter), EVT_DESTROY, window_manager_on_highlighter_destroy,
                 widget);
      ret = RET_OK;
    }
  }

  /* 把 dialog_highlighter 给键盘窗口使用 */
  if (dialog_highlighter != NULL && widget_is_keyboard(curr_win)) {
    dialog_highlighter->used_by_others = TRUE;
  } else if (dialog_highlighter != NULL && !widget_has_highlighter(curr_win)) {
    /* 把 dialog_highlighter 给自身没高亮且支持高亮的窗口使用 */
    if (widget_is_support_highlighter(curr_win)) {
      dialog_highlighter->used_by_others = TRUE;
    } else { /* 因为当支持高亮的窗口销毁的时候会释放 dialog_highlighter 局部, 防止不支持高亮的窗口使用 dialog_highlighter 高亮贴图。 */
      wm->dialog_highlighter = NULL;
    }
  }

  return ret;
}

static ret_t window_manager_prepare_dialog_highlighter(widget_t* widget, widget_t* prev_win,
                                                       widget_t* curr_win) {
  if (window_manager_default_create_dialog_highlighter(widget, curr_win) == RET_OK) {
    bitmap_t img = {0};
    ret_t ret = window_manager_default_snap_prev_window(widget, prev_win, &img);
    if (ret != RET_OK) {
      window_manager_default_dialog_highlighter_destroy(widget);
    }
    return ret;
  }

  return RET_FAIL;
}

static ret_t window_manager_create_highlighter(widget_t* widget, widget_t* curr_win) {
  widget_t* prev_win = window_manager_find_prev_normal_window(widget);
  window_manager_prepare_dialog_highlighter(widget, prev_win, curr_win);
  return RET_OK;
}

static bool_t window_manager_curr_win_is_top_animator_window(widget_t* wm, widget_t* curr_win) {
  bool_t is_find = FALSE;
  WIDGET_FOR_EACH_CHILD_BEGIN_R(wm, iter, i)
  if (widget_is_normal_window(iter) && !is_find && curr_win != iter) {
    return FALSE;
  }
  if (iter == curr_win) {
    is_find = TRUE;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()
  return TRUE;
}

static ret_t window_manager_create_animator(window_manager_default_t* wm, widget_t* curr_win,
                                            bool_t open) {
  value_t v;
  const char* anim_hint = NULL;
  widget_t* prev_win = window_manager_find_prev_normal_window(WIDGET(wm));
  const char* key = open ? WIDGET_PROP_OPEN_ANIM_HINT : WIDGET_PROP_CLOSE_ANIM_HINT;

  if (prev_win == curr_win || prev_win == NULL) {
    return RET_FAIL;
  }

  return_value_if_fail(wm != NULL && prev_win != NULL && curr_win != NULL, RET_BAD_PARAMS);

  if (wm->animator != NULL ||
      !window_manager_curr_win_is_top_animator_window(WIDGET(wm), curr_win)) {
    return RET_FAIL;
  }

  if (widget_get_prop(curr_win, key, &v) == RET_OK) {
    anim_hint = value_str(&(v));
  } else {
    key = WIDGET_PROP_ANIM_HINT;
    if (widget_get_prop(curr_win, key, &v) == RET_OK) {
      anim_hint = value_str(&(v));
    }
  }

  wm->is_animator_paint_system_bar_top = FALSE;
  wm->is_animator_paint_system_bar_bottom = FALSE;
  if (anim_hint && *anim_hint) {
    canvas_t* c = native_window_get_canvas(wm->native_window);
    window_manager_default_create_dialog_highlighter(WIDGET(wm), curr_win);
    wm->curr_win = curr_win;
    if (open) {
      wm->animator = window_animator_create_for_open(anim_hint, c, prev_win, curr_win);
    } else {
      wm->animator = window_animator_create_for_close(anim_hint, c, prev_win, curr_win);
    }

    wm->animating = wm->animator != NULL;

    if (wm->animating) {
      wm->animator->is_paint_system_bar_top = wm->is_animator_paint_system_bar_top;
      wm->animator->is_paint_system_bar_bottom = wm->is_animator_paint_system_bar_bottom;

      wm->ignore_user_input = TRUE;
      log_debug("ignore_user_input\n");
    }
  } else {
    /* 动画播放完毕后刷新wm，避免出现高亮残留 */
    widget_invalidate_force(WIDGET(wm), NULL);
    if (widget_get_prop(curr_win, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
      wm->curr_win = curr_win;
      window_manager_invalidate_system_bar(WIDGET(wm));
      window_manager_prepare_dialog_highlighter(WIDGET(wm), prev_win, curr_win);
    }
  }

  return wm->animating ? RET_OK : RET_FAIL;
}

static ret_t on_idle_invalidate(const timer_info_t* info) {
  widget_t* curr_win = WIDGET(info->ctx);
  widget_invalidate_force(curr_win, NULL);

  return RET_REMOVE;
}

static ret_t window_manager_check_if_need_open_animation(const idle_info_t* info) {
  widget_t* curr_win = WIDGET(info->ctx);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(curr_win->parent);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  if (wm->dialog_highlighter != NULL && widget_is_normal_window(curr_win)) {
    window_manager_default_dialog_highlighter_destroy(WIDGET(wm));
  }

  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);
  wm->ready_animator = FALSE;
  if (window_manager_create_animator(wm, curr_win, TRUE) != RET_OK) {
    widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
    if (prev_win != NULL) {
      if (!widget_is_keyboard(curr_win) && !widget_is_overlay(curr_win)) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
      }
    }
    window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
    widget_add_timer(curr_win, on_idle_invalidate, 100);
  }

  return RET_REMOVE;
}

static ret_t on_window_switch_done(void* ctx, event_t* e) {
  widget_t* to_close = WIDGET(ctx);
  log_debug("window %s close\n", to_close->name);
  window_manager_close_window_force(to_close->parent, to_close);

  return RET_REMOVE;
}

static ret_t window_manager_default_switch_to(widget_t* widget, widget_t* curr_win,
                                              widget_t* target_win, bool_t close) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(curr_win != NULL && target_win != NULL && wm != NULL, RET_BAD_PARAMS);

  wm->ready_animator = FALSE;
  widget_restack(target_win, 0xffffff);
  if (close) {
    widget_on(target_win, EVT_WINDOW_TO_FOREGROUND, on_window_switch_done, curr_win);
  }

  if (window_manager_create_animator(wm, target_win, TRUE) != RET_OK) {
    window_manager_dispatch_window_event(curr_win, EVT_WINDOW_TO_BACKGROUND);
    window_manager_dispatch_window_event(target_win, EVT_WINDOW_TO_FOREGROUND);
    widget_invalidate_force(target_win, NULL);
  }

  return RET_OK;
}

static ret_t window_manager_dispatch_window_open(widget_t* curr_win) {
  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);

  return window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
}

static ret_t window_manager_idle_dispatch_window_open(const idle_info_t* info) {
  widget_t* curr_win = WIDGET(info->ctx);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(curr_win->parent);
  if (wm != NULL) {
    wm->ready_animator = FALSE;
    window_manager_dispatch_window_open(curr_win);
  }

  return RET_REMOVE;
}

static ret_t window_manager_check_if_need_close_animation(window_manager_default_t* wm,
                                                          widget_t* curr_win) {
  return window_manager_create_animator(wm, curr_win, FALSE);
}

static ret_t window_manager_default_do_open_window(widget_t* widget, widget_t* window) {
  if (widget->children != NULL && widget->children->size > 0) {
    widget_add_idle(window, (idle_func_t)window_manager_check_if_need_open_animation);
  } else {
    widget_add_idle(window, (idle_func_t)window_manager_idle_dispatch_window_open);
  }

  return RET_OK;
}

static ret_t wm_on_destroy_child(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  (void)e;
  if (!widget->destroying) {
    window_manager_dispatch_top_window_changed(widget);
  }

  return RET_REMOVE;
}

static ret_t window_manager_default_open_window(widget_t* widget, widget_t* window) {
  ret_t ret = RET_OK;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  wm->prev_win = window_manager_get_top_window(widget);

  if (wm->animator != NULL) {
    wm->pending_open_window = window;
  } else {
    wm->ready_animator = TRUE;
    window_manager_default_do_open_window(widget, window);
  }

  ret = widget_add_child(widget, window);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  window_manager_default_layout_child(widget, window);

  window->dirty = FALSE;
  widget->target = window;

  widget_invalidate(window, NULL);

  widget_set_prop_pointer(window, WIDGET_PROP_NATIVE_WINDOW, wm->native_window);
  widget_on(window, EVT_DESTROY, wm_on_destroy_child, widget);
  widget_update_style(widget);

  return ret;
}

static ret_t window_manager_idle_destroy_window(const idle_info_t* info) {
  widget_t* win = WIDGET(info->ctx);
  widget_destroy(win);

  return RET_OK;
}

static ret_t window_manager_prepare_close_window(widget_t* widget, widget_t* window) {
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  if (widget->target == window) {
    widget->target = NULL;
  }

  if (widget->key_target == window) {
    widget->key_target = NULL;
  }

  if (widget->grab_widget != NULL) {
    if (widget->grab_widget == window) {
      widget->grab_widget = NULL;
    }
  }

  return RET_OK;
}

static ret_t window_manager_default_on_idle_check_and_close_window(const idle_info_t* idle) {
  widget_t* win = WIDGET(idle->ctx);
  window_manager_close_window_force(window_manager(), win);
  return RET_REMOVE;
}

static ret_t window_manager_default_close_window(widget_t* widget, widget_t* window) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);

  if (!window_is_opened(window)) {
    uint32_t idle_id = widget_get_prop_int(window, "check_and_close_window_idle_id", 0);
    if (idle_id == 0) {
      idle_id = widget_add_idle(window, window_manager_default_on_idle_check_and_close_window);
      widget_set_prop_int(window, "check_and_close_window_idle_id", idle_id);
    }
    return RET_OK;
  }
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);

  if (wm->animator) {
    if (widget_is_keyboard(window)) {
      input_method_t* im = input_method();
      if (im != NULL) {
        widget_t* win = widget_get_window(im->widget);
        if (win == wm->animator->prev_win && widget_is_normal_window(wm->animator->curr_win)) {
          /* 如果已经打开下一个窗口后，就直接释放上一个窗口的软键盘，不必播放软键盘的动画 */
          window_manager_close_window_force(window->parent, window);
          return RET_OK;
        }
      }
    }
    wm->pending_close_window = window;
    return RET_OK;
  }

  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  if (window_manager_check_if_need_close_animation(wm, window) != RET_OK) {
    widget_t* prev_win = NULL;

    widget_remove_child(widget, window);
    idle_add(window_manager_idle_destroy_window, window);
    if (wm->dialog_highlighter != NULL && wm->dialog_highlighter->dialog == window) {
      window_manager_default_dialog_highlighter_destroy(widget);
    }
    /* 这里是解决没有结束动画，但是 prev_win 是高亮的对话框的情况 */
    prev_win = window_manager_get_top_window(widget);
    if (widget_is_keyboard(prev_win)) {
      input_method_t* im = input_method();
      if (im != NULL) {
        if (im->keyboard != NULL && im->keyboard == prev_win) {
          prev_win = widget_get_window(im->widget);
        }
      }
    }
    if (prev_win != NULL) {
      if (!widget_is_keyboard(window)) {
        bool_t find = FALSE;
        bool_t is_create = TRUE;
        const char* curr_highlight = NULL;
        widget_t* widget_highlighter = prev_win;

        wm->curr_win = prev_win;
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_FOREGROUND);

        WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
        if (!find && iter == prev_win) {
          find = TRUE;
        }
        if (find) {
          if (!widget_is_support_highlighter(iter)) {
            is_create = FALSE;
            break;
          }
          curr_highlight = widget_get_prop_str(iter, WIDGET_PROP_HIGHLIGHT, NULL);
          if (curr_highlight != NULL && *curr_highlight != '\0') {
            widget_highlighter = iter;
            break;
          }
        }
        WIDGET_FOR_EACH_CHILD_END();
        if (is_create) {
          wm->curr_win = widget_highlighter;
          window_manager_create_highlighter(widget, widget_highlighter);
          wm->curr_win = prev_win;
        }
      }
    }
  }

  if (widget->children == NULL || widget->children->size == 0) {
    widget_invalidate_force(widget, NULL);
  }

  return RET_OK;
}

static ret_t window_manager_default_close_window_force(widget_t* widget, widget_t* window) {
  ret_t ret = RET_OK;
  bool_t close_window = TRUE;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

  if (wm->animator != NULL) {
    bool_t is_open = wm->animator->open;
    bool_t curr_is_target = wm->animator->curr_win == window;
    if (curr_is_target || curr_is_target) {
      ret = window_manager_animate_done(widget);
      close_window = FALSE;
    }
    if (curr_is_target && is_open) {
      close_window = TRUE;
    }
  }

  if (close_window) {
    window_manager_prepare_close_window(widget, window);
    window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
    widget_remove_child(widget, window);
    widget_destroy(window);
  }

  return ret;
}

static widget_t* window_manager_default_find_target(widget_t* widget, xy_t x, xy_t y) {
  return window_manager_find_target(widget, NULL, x, y);
}

static ret_t window_manager_paint_cursor(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (wm->r_cursor.w > 0 && wm->r_cursor.h > 0) {
    return_value_if_fail(image_manager_get_bitmap(image_manager(), wm->cursor, &bitmap) == RET_OK,
                         RET_BAD_PARAMS);
    canvas_draw_icon(c, &bitmap, wm->r_cursor.x, wm->r_cursor.y);
  }

  return RET_OK;
}

static ret_t window_manager_update_cursor(widget_t* widget, int32_t x, int32_t y) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  uint32_t w = wm->r_cursor.w;
  uint32_t h = wm->r_cursor.h;

  if (w > 0 && h > 0) {
    uint32_t hw = w >> 1;
    uint32_t hh = h >> 1;
    int32_t oldx = wm->r_cursor.x;
    int32_t oldy = wm->r_cursor.y;
    rect_t r = rect_init(oldx - hw, oldy - hh, w, h);

    window_manager_default_invalidate(widget, &r);

    wm->r_cursor.x = x;
    wm->r_cursor.y = y;

    r = rect_init(x - hw, y - hh, w, h);

    window_manager_default_invalidate(widget, &r);
  }

  return RET_OK;
}

static ret_t window_manager_paint_normal(widget_t* widget, canvas_t* c) {
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
  uint32_t i = 0;
  uint32_t y = 0;
  uint32_t h = 0;
  uint32_t tmp_h = 0;
  uint32_t number = 0;
#endif
  uint64_t start_time = time_now_ms();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (WINDOW_MANAGER(wm)->max_fps) {
    uint32_t duration = 1000 / WINDOW_MANAGER(wm)->max_fps;
    uint32_t elapsed_time = start_time - wm->last_paint_time + wm->last_paint_cost;
    /* 
     * 上一帧的绘图耗时加上各种事件触发耗时小于绘图间隔时间，则跳过绘图并且控制睡眠时间。
     * 控制睡眠时间是为了防止睡眠时间过长导致减低绘图次数。
     */
    if (elapsed_time < duration) {
      window_manager_set_curr_expected_sleep_time(widget, duration - elapsed_time);
      return RET_OK;
    }
  }

  fps_inc(&(wm->fps));
  if (WINDOW_MANAGER(wm)->show_fps) {
    rect_t fps_rect = rect_init(0, 0, 60, 30);
    window_manager_default_invalidate(widget, &fps_rect);
  }
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
  if (wm->native_window->dirty_rects.max.w > 0 && wm->native_window->dirty_rects.max.h > 0) {
    rect_t r = native_window_calc_dirty_rect(wm->native_window);
    if (r.w > 0 && r.h > 0) {
      assert(r.w <= FRAGMENT_FRAME_BUFFER_SIZE);
      y = r.y;
      h = r.h;
      tmp_h = FRAGMENT_FRAME_BUFFER_SIZE / r.w;
      number = r.h / tmp_h;

      for (i = 0; i <= number; i++) {
        dirty_rects_t tmp_dirty_rects;
        r.y = y + i * tmp_h;
        if (i == number) {
          tmp_h = h % tmp_h;
        }
        r.h = tmp_h;
        if (r.h == 0) {
          break;
        }
        dirty_rects_init(&(tmp_dirty_rects));
        dirty_rects_add(&(tmp_dirty_rects), (const rect_t*)&r);
        canvas_t* c = native_window_get_canvas(wm->native_window);
        canvas_begin_frame(c, (const dirty_rects_t*)&tmp_dirty_rects, LCD_DRAW_NORMAL);
        widget_paint(WIDGET(wm), c);
        window_manager_paint_cursor(widget, c);
        canvas_end_frame(c);
        dirty_rects_deinit(&(tmp_dirty_rects));
      }

      native_window_update_last_dirty_rect(wm->native_window);
    }
    native_window_clear_dirty_rect(wm->native_window);
  }
#else
  if (native_window_begin_frame(wm->native_window, LCD_DRAW_NORMAL) == RET_OK) {
    if (widget->children == NULL || widget->children->size == 0) {
      color_t bg = color_init(0xff, 0xff, 0xff, 0xff);
      canvas_set_fill_color(c, bg);
      canvas_fill_rect(c, 0, 0, widget->w, widget->h);
    } else {
      /* 
       * 获取当前帧的脏矩形列表和 fb 的脏矩形列表合并后的新的脏矩形列表，
       * 如果没有 fb 的脏矩形列表的话，会退化为获取当前帧的脏矩形。
       */
      dirty_rects_t* dirty_rects = (dirty_rects_t*)lcd_get_dirty_rects(c->lcd);
      if (dirty_rects == NULL && !lcd_is_support_dirty_rect(c->lcd)) {
        dirty_rects = &(wm->native_window->dirty_rects);
      }
      dirty_rects_paint(dirty_rects, WIDGET(wm), c, widget_paint);
    }
    window_manager_paint_cursor(widget, c);
    native_window_end_frame(wm->native_window);
  }
#endif
  wm->last_paint_time = time_now_ms();
  wm->last_paint_cost = wm->last_paint_time - start_time;

  return RET_OK;
}

static ret_t window_manager_invalidate_system_bar(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    widget_invalidate_force(iter, NULL);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

#ifndef WITHOUT_WINDOW_ANIMATORS
static ret_t window_manager_animate_done_set_window_foreground(widget_t* widget, widget_t* prev_win,
                                                               widget_t* curr_win) {
  bool_t is_set = FALSE;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (prev_win == iter) {
    is_set = TRUE;
  } else if (curr_win == iter) {
    is_set = FALSE;
  }

  if (is_set && (widget_is_normal_window(iter) || i + 1 == widget->children->size)) {
    window_manager_dispatch_window_event(iter, EVT_WINDOW_TO_FOREGROUND);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static bool_t window_manager_default_is_dialog_highlighter(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, FALSE);

  if (widget_is_support_highlighter(widget) &&
      widget_get_prop(widget, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
    return TRUE;
  }

  return FALSE;
}

static widget_t* window_manager_default_find_top_dialog_highlighter(widget_t* widget,
                                                                    widget_t* prev_win,
                                                                    widget_t* curr_win) {
  int32_t i = 0;
  widget_t* dialog = NULL;
  widget_t** children = (widget_t**)(widget->children->elms);
  i = widget->children->size - 1;

  for (; i >= 0; i--) {
    widget_t* iter = children[i];
    if (iter == prev_win) {
      break;
    }
    if (iter == curr_win) {
      continue;
    }
    if (window_manager_default_is_dialog_highlighter(iter)) {
      dialog = iter;
      break;
    }
  }
  return dialog;
}

static ret_t window_manager_animate_done(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (wm->animator != NULL) {
    bool_t is_open = wm->animator->open;
    widget_t* top_dialog_highligth = NULL;
    widget_t* prev_win = wm->animator->prev_win;
    widget_t* curr_win = wm->animator->curr_win;

    window_animator_destroy(wm->animator);
    bool_t curr_win_is_keyboard = widget_is_keyboard(curr_win);
    bool_t curr_win_is_normal_window = widget_is_normal_window(curr_win);

    wm->animator = NULL;
    wm->animating = FALSE;
    wm->ignore_user_input = FALSE;

    if (is_open) {
      /*此时前一个窗口并非是真正的前一个窗口，而是前一个normal窗口，所以这里重新找真正的前一个窗口*/
      prev_win = window_manager_find_prev_window(WIDGET(wm));
      /* 结束打开窗口动画后 */
      if (!curr_win_is_keyboard && prev_win != curr_win) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
      }
      if (!curr_win_is_normal_window) {
        top_dialog_highligth = window_manager_default_find_top_dialog_highlighter(
            widget, prev_win, curr_win_is_keyboard ? curr_win : NULL);
      }
      if (widget_is_window_opened(curr_win)) {
        //for swtich to
        window_manager_dispatch_window_event(curr_win, EVT_WINDOW_TO_FOREGROUND);
      } else {
        window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
      }
    } else {
      /* 结束关闭窗口动画后 */
      if (!curr_win_is_keyboard) {
        window_manager_animate_done_set_window_foreground(widget, prev_win, curr_win);
      }
      top_dialog_highligth =
          window_manager_default_find_top_dialog_highlighter(widget, prev_win, curr_win);
    }
    /* 制作一张没有最后一个对话框的高亮背景贴图 */
    if (top_dialog_highligth != NULL) {
      widget_t* tmp_curr_win = wm->curr_win;
      wm->curr_win = top_dialog_highligth;
      window_manager_create_highlighter(widget, top_dialog_highligth);
      wm->curr_win = tmp_curr_win;
    }

    if (wm->pending_close_window != NULL) {
      widget_t* window = wm->pending_close_window;
      wm->pending_close_window = NULL;
      if (wm->pending_open_window != NULL) {
        window_manager_close_window_force(widget, window);
      } else {
        window_manager_close_window(widget, window);
      }
    }

    if (wm->pending_open_window != NULL) {
      widget_t* window = wm->pending_open_window;
      wm->pending_open_window = NULL;
      window_manager_default_do_open_window(widget, window);
    }

    /* 动画播放完毕后刷新wm，避免出现高亮残留 */
    widget_invalidate_force(widget, NULL);
  }

  return RET_OK;
}

static ret_t window_manager_paint_animation(widget_t* widget, canvas_t* c) {
  paint_event_t e;
  uint64_t start_time = time_now_ms();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  ENSURE(window_animator_begin_frame(wm->animator) == RET_OK);

  widget_dispatch(widget, paint_event_init(&e, EVT_BEFORE_PAINT, widget, c));

  ret_t ret = window_animator_update(wm->animator, start_time);
  window_manager_default_paint_always_on_top(widget, c);

  widget_dispatch(widget, paint_event_init(&e, EVT_AFTER_PAINT, widget, c));

  ENSURE(window_animator_end_frame(wm->animator) == RET_OK);

  wm->last_paint_cost = time_now_ms() - start_time;
  fps_inc(&(wm->fps));

  if (ret == RET_DONE) {
    window_manager_animate_done(widget);
  }

  return RET_OK;
}
#else
static ret_t window_manager_animate_done(widget_t* widget) {
  return RET_OK;
}
#endif /*WITHOUT_WINDOW_ANIMATORS*/

static ret_t window_manager_default_update_fps(widget_t* widget) {
  canvas_t* c = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  c = native_window_get_canvas(wm->native_window);
  canvas_set_fps(c, WINDOW_MANAGER(wm)->show_fps, fps_get(&(wm->fps)));

  return RET_OK;
}

static ret_t window_manager_default_paint(widget_t* widget) {
  ret_t ret = RET_OK;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  canvas_t* c = native_window_get_canvas(wm->native_window);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_set_global_alpha(c, 0xff);
  window_manager_default_update_fps(widget);
  window_manager_set_curr_expected_sleep_time(widget, 0xFFFFFFFF);

#ifndef WITHOUT_WINDOW_ANIMATORS
  if (wm->animator != NULL) {
    ret = window_manager_paint_animation(widget, c);
  } else if (!wm->ready_animator) {
    ret = window_manager_paint_normal(widget, c);
  }
#else
  ret = window_manager_paint_normal(widget, c);
#endif /*WITHOUT_WINDOW_ANIMATORS*/
  return ret;
}

static ret_t window_manager_default_invalidate(widget_t* widget, const rect_t* r) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (wm->native_window != NULL) {
    native_window_invalidate(wm->native_window, r);
  }

  return RET_OK;
}

static widget_t* window_manager_default_get_prev_window(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return wm->prev_win;
}

static ret_t window_manager_default_paint_always_on_top(widget_t* widget, canvas_t* c) {
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->visible) {
    if (widget_get_prop_bool(iter, WIDGET_PROP_ALWAYS_ON_TOP, FALSE)) {
      widget_paint(iter, c);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_default_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t start = 0;
  bool_t cover_highlighter = FALSE;
  bool_t has_fullscreen_win = FALSE;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (wm->dialog_highlighter != NULL && wm->dialog_highlighter->dialog == iter) {
    start = i;
    break;
  } else if (iter->visible && widget_is_normal_window(iter)) {
    start = i;
    cover_highlighter = TRUE;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  if (!cover_highlighter && wm->dialog_highlighter != NULL) {
    dialog_highlighter_draw(wm->dialog_highlighter, 1);
  } else {
    /*paint normal windows*/
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    if (i >= start && iter->visible) {
      if (widget_is_normal_window(iter)) {
        widget_paint(iter, c);

        if (!has_fullscreen_win) {
          has_fullscreen_win = widget_is_fullscreen_window(iter);
        }
        start = i + 1;
        break;
      }
    }
    WIDGET_FOR_EACH_CHILD_END()

    /*paint system_bar*/
    if (!has_fullscreen_win) {
      window_manager_paint_system_bar(widget, c);
    }
  }
  /*paint dialog and other*/
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    if ((!widget_is_system_bar(iter) && !widget_is_normal_window(iter)) ||
        (wm->dialog_highlighter != NULL && wm->dialog_highlighter->dialog != NULL &&
         widget_is_normal_window(iter))) {
      widget_paint(iter, c);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  window_manager_default_paint_always_on_top(widget, c);

  return RET_OK;
}

static ret_t window_manager_default_on_remove_child(widget_t* widget, widget_t* window) {
  widget_t* top = window_manager_get_top_main_window(widget);

  if (top != NULL) {
    rect_t r;
    r = rect_init(window->x, window->y, window->w, window->h);
    widget_invalidate(top, &r);
  }

  if (widget->destroying) {
    widget_off_by_ctx(window, widget);
  }

  return RET_FAIL;
}

static ret_t window_manager_default_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    value_set_str(v, wm->cursor);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CANVAS)) {
    canvas_t* c = native_window_get_canvas(wm->native_window);
    value_set_pointer(v, c);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CURR_WIN)) {
    value_set_pointer(v, wm->curr_win);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCREEN_SAVER_TIME)) {
    value_set_uint32(v, wm->screen_saver_time);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_FPS)) {
    value_set_bool(v, WINDOW_MANAGER(wm)->show_fps);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX_FPS)) {
    value_set_uint32(v, WINDOW_MANAGER(wm)->max_fps);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_default_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    return window_manager_set_cursor(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_CURR_WIN)) {
    wm->curr_win = value_pointer(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCREEN_SAVER_TIME)) {
    return window_manager_set_screen_saver_time(widget, value_uint32(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_FPS)) {
    return window_manager_set_show_fps(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX_FPS)) {
    return window_manager_set_max_fps(widget, value_uint32(v));
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_default_on_destroy(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

#ifndef WITHOUT_WINDOW_ANIMATORS
  if (wm->animator != NULL) {
    wm->animator->prev_win = NULL;
    wm->animator->curr_win = NULL;
    window_animator_destroy(wm->animator);
    wm->animator = NULL;
  }
#endif /*WITHOUT_WINDOW_ANIMATORS*/

  TK_OBJECT_UNREF(wm->native_window);

  return RET_OK;
}

static ret_t window_manager_default_on_layout_children(widget_t* widget) {
  rect_t client_r = rect_init(0, 0, widget->w, widget->h);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    window_manager_default_layout_system_bar(widget, iter);
  }
  WIDGET_FOR_EACH_CHILD_END();

  window_manager_default_get_client_r(widget, &client_r);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (!widget_is_system_bar(iter)) {
    window_manager_default_layout_not_system_bar(widget, iter, client_r);
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t window_manager_default_resize(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_default_post_init(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_default_set_cursor(widget_t* widget, const char* cursor);
static ret_t window_manager_default_set_show_fps(widget_t* widget, bool_t show_fps);
static ret_t window_manager_default_dispatch_input_event(widget_t* widget, event_t* e);
static ret_t window_manager_default_set_screen_saver_time(widget_t* widget,
                                                          uint32_t screen_saver_time);

static ret_t window_manager_default_get_pointer(widget_t* widget, xy_t* x, xy_t* y, bool_t* pressed,
                                                bool_t* in_pointer_up) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  input_device_status_t* ids = window_manager_get_input_device_status(widget);
  return_value_if_fail(wm != NULL && ids != NULL, RET_BAD_PARAMS);

  if (x != NULL) {
    *x = ids->last_x;
  }
  if (y != NULL) {
    *y = ids->last_y;
  }
  if (pressed != NULL) {
    *pressed = ids->pressed;
  }
  if (in_pointer_up != NULL) {
    *in_pointer_up = ids->in_pointer_up;
  }

  return RET_OK;
}

static ret_t window_manager_default_is_animating(widget_t* widget, bool_t* playing) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  *playing = wm->animating;
  return RET_OK;
}

static ret_t window_manager_default_orientation(widget_t* widget, wh_t w, wh_t h,
                                                lcd_orientation_t old_orientation,
                                                lcd_orientation_t new_orientation) {
  ret_t ret = RET_OK;
  rect_t r = rect_init(0, 0, w, h);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  ret = native_window_set_orientation(wm->native_window, old_orientation, new_orientation);
  return_value_if_fail(ret == RET_OK, ret);

  widget_move_resize(widget, 0, 0, w, h);
  native_window_invalidate(wm->native_window, &r);
  native_window_update_last_dirty_rect(wm->native_window);

  return widget_layout_children(widget);
}

ret_t window_manager_default_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  input_device_status_t* ids = window_manager_get_input_device_status(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_ORIENTATION_WILL_CHANGED) {
    wh_t w, h;
    lcd_orientation_t new_orientation;
    lcd_orientation_t old_orientation;
    orientation_event_t* evt = orientation_event_cast(e);
    lcd_t* lcd = native_window_get_canvas(wm->native_window)->lcd;
    return_value_if_fail(lcd != NULL && evt != NULL, RET_FAIL);
    w = lcd->w;
    h = lcd->h;
    new_orientation = evt->orientation;
    old_orientation = evt->old_orientation;
    native_window_clear_dirty_rect(wm->native_window);
    if (tk_is_swap_size_by_orientation(old_orientation, new_orientation)) {
      w = lcd->h;
      h = lcd->w;
    }

    ret = lcd_set_orientation(lcd, old_orientation, new_orientation);
    return_value_if_fail(ret == RET_OK, ret);
#if !defined(WITH_GPU) && defined(WITH_FAST_LCD_PORTRAIT)
    if (system_info()->flags & SYSTEM_INFO_FLAG_FAST_LCD_PORTRAIT) {
      ENSURE(image_manager_unload_all(widget_get_image_manager(widget)) == RET_OK);
    }
#endif
    window_manager_default_orientation(widget, w, h, old_orientation, new_orientation);
    e->type = EVT_ORIENTATION_CHANGED;

    widget_dispatch(widget, e);

    window_manager_default_reset_window_animator(widget);
    window_manager_default_reset_dialog_highlighter(widget);
  } else if (e->type == EVT_THEME_CHANGED) {
    window_manager_on_theme_changed(widget);
    window_manager_default_reset_window_animator(widget);
    window_manager_default_reset_dialog_highlighter(widget);
  } else if (e->type == EVT_TOP_WINDOW_CHANGED) {
    input_device_status_abort_all_pressed_keys(ids);
  }

  return RET_OK;
}

static bool_t widget_is_system_bar_top(widget_t* window) {
  return tk_str_eq(window->vt->type, WIDGET_TYPE_SYSTEM_BAR);
}

static bool_t widget_is_system_bar_bottom(widget_t* window) {
  return tk_str_eq(window->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM);
}

static ret_t window_manager_default_get_client_r(widget_t* widget, rect_t* r) {
  *r = rect_init(0, 0, widget->w, widget->h);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_is_system_bar_top(iter)) {
    r->y = iter->h;
    r->h -= iter->h;
  }

  if (widget_is_system_bar_bottom(iter)) {
    r->h -= iter->h;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t window_manager_default_layout_system_bar(widget_t* widget, widget_t* window) {
  window->x = 0;
  window->w = widget->w;

  if (widget_is_system_bar_bottom(window)) {
    window->y = widget->h - window->h;
  }

  widget_layout(window);

  return RET_OK;
}

static ret_t window_manager_default_layout_not_system_bar(widget_t* widget, widget_t* window,
                                                          rect_t client_r) {
  xy_t x = window->x;
  xy_t y = window->y;
  wh_t w = window->w;
  wh_t h = window->h;

  if (widget_is_normal_window(window)) {
    if (widget_is_fullscreen_window(window)) {
      x = 0;
      y = 0;
      w = widget->w;
      h = widget->h;
    } else {
      x = client_r.x;
      y = client_r.y;
      w = client_r.w;
      h = client_r.h;
    }
  } else if (widget_is_dialog(window)) {
    rect_t widget_r = {widget->x, widget->y, widget->w, widget->h};
    rect_t window_r = {window->x, window->y, window->w, window->h};
    rect_merge(&widget_r, &window_r);
    /* Force centering if the dialog box is out of window_manager! */
    if (widget_r.x < widget->x || widget_r.y < widget->y || widget_r.w > widget->w ||
        widget_r.h > widget->h) {
      w = tk_min(widget->w, window->w);
      h = tk_min(widget->h, window->h);
      x = (widget->w - w) >> 1;
      y = (widget->h - h) >> 1;
    }
  } else {
    x = window->x;
    y = window->y;
    w = window->w;
    h = window->h;
  }

  widget_move_resize_ex(window, x, y, w, h, FALSE);
  widget_layout(window);

  return RET_OK;
}

static ret_t window_manager_default_layout_child(widget_t* widget, widget_t* window) {
  if (widget_is_system_bar(window)) {
    return window_manager_default_layout_system_bar(widget, window);
  } else {
    rect_t client_r = rect_init(0, 0, widget->w, widget->h);
    window_manager_default_get_client_r(widget, &client_r);
    return window_manager_default_layout_not_system_bar(widget, window, client_r);
  }
}

static ret_t window_manager_default_set_fullscreen(widget_t* widget, bool_t fullscreen) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  return native_window_set_fullscreen(wm->native_window, fullscreen);
}

static ret_t window_manager_default_resize(widget_t* widget, wh_t w, wh_t h) {
  ret_t ret = RET_OK;
  rect_t r = rect_init(0, 0, w, h);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  lcd_orientation_t lcd_orientation = system_info()->lcd_orientation;
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  ret = native_window_resize(wm->native_window, w, h, TRUE);
  return_value_if_fail(ret == RET_OK, ret);

  if (lcd_orientation == LCD_ORIENTATION_90 || lcd_orientation == LCD_ORIENTATION_270) {
    wh_t tmp_w = w;
    w = h;
    h = tmp_w;
    r = rect_init(0, 0, w, h);
  }

  if (widget->w == w && widget->h == h) {
    return RET_OK;
  }

  widget_move_resize(widget, 0, 0, w, h);
  native_window_invalidate(wm->native_window, &r);
  native_window_update_last_dirty_rect(wm->native_window);

  return widget_layout_children(widget);
}

static ret_t window_manager_default_post_init(widget_t* widget, wh_t w, wh_t h) {
  native_window_info_t info;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->lcd_w = w;
  wm->lcd_h = h;
  wm->native_window = native_window_create(widget);

  if (native_window_get_info(wm->native_window, &info) == RET_OK) {
    w = info.w;
    h = info.h;
  }

  window_manager_default_resize(widget, w, h);

  return RET_OK;
}

static ret_t window_manager_default_dispatch_input_event(widget_t* widget, event_t* e) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  input_device_status_t* ids = window_manager_get_input_device_status(widget);
  return_value_if_fail(wm != NULL && e != NULL, RET_BAD_PARAMS);

  window_manager_start_or_reset_screen_saver_timer(wm);

  native_window_preprocess_event(wm->native_window, e);
  if (wm->ignore_user_input) {
    log_debug("animating ignore input\n");
    input_device_status_on_ignore_input_event(ids, widget, e);
    return RET_OK;
  }

  input_device_status_on_input_event(ids, widget, e);
  window_manager_update_cursor(widget, ids->last_x, ids->last_y);

  return RET_OK;
}

static ret_t window_manager_default_set_show_fps(widget_t* widget, bool_t show_fps) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->show_fps = show_fps;

  return RET_OK;
}

static ret_t window_manager_default_set_screen_saver_time(widget_t* widget,
                                                          uint32_t screen_saver_time) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->screen_saver_time = screen_saver_time;
  window_manager_start_or_reset_screen_saver_timer(wm);

  return RET_OK;
}

static ret_t window_manager_default_set_cursor(widget_t* widget, const char* cursor) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

#if defined(ENABLE_CURSOR)
  if (tk_str_eq(cursor, wm->cursor)) {
    return RET_OK;
  }

  tk_strncpy(wm->cursor, cursor, TK_NAME_LEN);
  if (cursor != NULL && *cursor) {
    bitmap_t bitmap;
    bitmap_t* img = NULL;

    if (image_manager_get_bitmap(image_manager(), cursor, &bitmap) == RET_OK) {
      img = &bitmap;
    }

    if (native_window_set_cursor(wm->native_window, cursor, img) != RET_OK) {
      wm->r_cursor.w = bitmap.w;
      wm->r_cursor.h = bitmap.h;
    } else {
      wm->r_cursor.w = 0;
      wm->r_cursor.h = 0;
    }
  }
#endif /*ENABLE_CURSOR*/

  return RET_OK;
}

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    widget_paint(iter, c);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

ret_t window_manager_paint_system_bar_top(widget_t* widget, canvas_t* c) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->vt->is_window && tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR)) {
    widget_paint(iter, c);
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

ret_t window_manager_paint_system_bar_bottom(widget_t* widget, canvas_t* c) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->vt->is_window && tk_str_eq(iter->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM)) {
    widget_paint(iter, c);
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_default_reset_window_animator(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  if (wm->animator != NULL) {
    window_animator_t* wa = wm->animator;
    if (wa->dialog_highlighter == NULL) {
      bitmap_destroy(&(wa->prev_img));
    }
    bitmap_destroy(&(wa->curr_img));
    window_manager_snap_prev_window(widget, wa->prev_win, &(wa->prev_img));
    window_manager_snap_curr_window(widget, wa->curr_win, &(wa->curr_img));
  }
#endif
  return RET_OK;
}

static ret_t window_manager_default_reset_dialog_highlighter(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  if (wm->dialog_highlighter != NULL) {
    bitmap_t img;
    widget_t* prev = window_manager_find_prev_normal_window(widget);

    memset(&img, 0x00, sizeof(img));
    if (prev != NULL) {
      dialog_highlighter_clear_image(wm->dialog_highlighter);
      wm->curr_win = wm->last_curr_win;
      window_manager_default_snap_prev_window(widget, prev, &img);
      wm->curr_win = NULL;
    }
  }
  return RET_OK;
}

static ret_t window_manager_default_native_window_resized(widget_t* widget, void* handle) {
  native_window_info_t ainfo;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return_value_if_fail(native_window_get_info(wm->native_window, &ainfo) == RET_OK, RET_FAIL);

  native_window_clear_dirty_rect(wm->native_window);
  window_manager_default_resize(widget, ainfo.w, ainfo.h);

  window_manager_default_reset_window_animator(widget);
  window_manager_default_reset_dialog_highlighter(widget);

  return RET_OK;
}

static ret_t window_manager_default_on_locale_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  return_value_if_fail(widget != NULL, RET_FAIL);

  window_manager_default_reset_window_animator(widget);
  window_manager_default_reset_dialog_highlighter(widget);

  return RET_OK;
}

static ret_t window_manager_default_dispatch_native_window_event(widget_t* widget, event_t* e,
                                                                 void* handle) {
  input_device_status_t* ids = window_manager_get_input_device_status(widget);
  return_value_if_fail(ids != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_NATIVE_WINDOW_RESIZED) {
    window_manager_default_native_window_resized(widget, handle);
  } else if (e->type == EVT_NATIVE_WINDOW_ENTER) {
    int32_t x = ((pointer_event_t*)e)->x;
    int32_t y = ((pointer_event_t*)e)->y;
    input_device_status_on_pointer_enter(ids, widget, x, y);
  } else if (e->type == EVT_NATIVE_WINDOW_LEAVE) {
    input_device_status_on_pointer_leave(ids, widget);
  }

  return RET_OK;
}

static window_manager_vtable_t s_window_manager_self_vtable = {
    .switch_to = window_manager_default_switch_to,
    .paint = window_manager_default_paint,
    .post_init = window_manager_default_post_init,
    .set_cursor = window_manager_default_set_cursor,
    .open_window = window_manager_default_open_window,
    .close_window = window_manager_default_close_window,
    .set_show_fps = window_manager_default_set_show_fps,
    .get_prev_window = window_manager_default_get_prev_window,
    .close_window_force = window_manager_default_close_window_force,
    .dispatch_input_event = window_manager_default_dispatch_input_event,
    .dispatch_native_window_event = window_manager_default_dispatch_native_window_event,
    .set_screen_saver_time = window_manager_default_set_screen_saver_time,
    .get_pointer = window_manager_default_get_pointer,
    .is_animating = window_manager_default_is_animating,
    .snap_curr_window = window_manager_default_snap_curr_window,
    .snap_prev_window = window_manager_default_snap_prev_window,
    .get_dialog_highlighter = window_manager_default_get_dialog_highlighter,
    .resize = window_manager_default_resize,
    .set_fullscreen = window_manager_default_set_fullscreen};

static const widget_vtable_t s_window_manager_vtable = {
    .size = sizeof(window_manager_t),
    .type = WIDGET_TYPE_WINDOW_MANAGER,
    .is_window_manager = TRUE,
    .set_prop = window_manager_default_set_prop,
    .get_prop = window_manager_default_get_prop,
    .on_event = window_manager_default_on_event,
    .invalidate = window_manager_default_invalidate,
    .on_layout_children = window_manager_default_on_layout_children,
    .on_paint_children = window_manager_default_on_paint_children,
    .on_remove_child = window_manager_default_on_remove_child,
    .find_target = window_manager_default_find_target,
    .on_destroy = window_manager_default_on_destroy};

widget_t* window_manager_create(void) {
  window_manager_default_t* wm = TKMEM_ZALLOC(window_manager_default_t);
  return_value_if_fail(wm != NULL, NULL);

  wm->ready_animator = FALSE;
  WINDOW_MANAGER(wm)->max_fps = TK_MAX_FPS;

  locale_info_on(locale_info(), EVT_LOCALE_CHANGED, window_manager_default_on_locale_changed, wm);
  return window_manager_init(WINDOW_MANAGER(wm), &s_window_manager_vtable,
                             &s_window_manager_self_vtable);
}

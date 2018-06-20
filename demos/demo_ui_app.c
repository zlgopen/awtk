/**
 * File:   demo1_app.c
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/label.h"
#include "base/timer.h"
#include "base/button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/utils.h"
#include "base/window.h"
#include "base/check_button.h"
#include "base/progress_bar.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "ui_loader/ui_builder_default.h"

static void install_click_hander(widget_t* widget);

static void open_window(const char* name, widget_t* to_close) {
  widget_t* win = to_close ? window_open_and_close(name, to_close) : window_open(name);

  install_click_hander(win);

  if (win->type == WIDGET_DIALOG) {
    dialog_modal(win);
  }
}

static ret_t on_open_window(void* ctx, event_t* e) {
  open_window((const char*)ctx, NULL);

  (void)e;

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  widget_t* win = (widget_t*)ctx;
  (void)e;
  return window_close(win);
}

static ret_t on_quit(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;

  dialog_quit(dialog, 0);
  (void)e;
  return RET_OK;
}

static ret_t on_show_fps(void* ctx, event_t* e) {
  widget_t* button = WIDGET(ctx);
  widget_t* widget = window_manager();
  window_manager_t* wm = WINDOW_MANAGER(widget);

  widget_invalidate(widget, NULL);
  window_manager_set_show_fps(widget, !wm->show_fps);
  widget_set_text(button, wm->show_fps ? L"Hide FPS" : L"Show FPS");

  return RET_OK;
}

static ret_t on_mem_test(void* ctx, event_t* e) {
  char text[32];
  uint32_t size = 100 * 1024;
  uint32_t memset_speed = 0;
  uint32_t memcpy_speed = 0;
  widget_t* win = WIDGET(ctx);
  widget_t* label_memset = widget_lookup(win, "memset", TRUE);
  widget_t* label_cost = widget_lookup(win, "cost", TRUE);
  widget_t* label_memcpy = widget_lookup(win, "memcpy", TRUE);
  void* buff = TKMEM_ALLOC(size);
  uint32_t cost = tk_mem_speed_test(buff, size, &memcpy_speed, &memset_speed);
  TKMEM_FREE(buff);

  tk_snprintf(text, sizeof(text), "%ums", cost);
  widget_set_text_utf8(label_cost, text);

  tk_snprintf(text, sizeof(text), "memset: %uK/s", memset_speed);
  widget_set_text_utf8(label_memset, text);

  tk_snprintf(text, sizeof(text), "memcpy: %uK/s", memcpy_speed);
  widget_set_text_utf8(label_memcpy, text);

  return RET_OK;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 10) % 100;
  progress_bar_set_value(progress_bar, value);

  (void)e;
  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* progress_bar = widget_lookup(win, "bar1", TRUE);
  uint8_t value = (PROGRESS_BAR(progress_bar)->value + 90) % 100;
  progress_bar_set_value(progress_bar, value);

  (void)e;
  return RET_OK;
}

#include "widget_animators/widget_animator_opacity.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_move.h"
#include "widget_animators/widget_animator_scale.h"
#include "widget_animators/widget_animator_rotation.h"

static void on_move_animator(widget_t* widget) {
  widget_animator_t* animator = widget_animator_move_create(widget, 1000, 0, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, widget->x, widget->y, widget->x + 200, widget->y);
  widget_animator_set_repeat(animator, 1000);
  widget_animator_start(animator);
}

static void on_fade_animator(widget_t* widget) {
  widget_animator_t* animator = widget_animator_opacity_create(widget, 1000, 0, EASING_SIN_OUT);
  widget_animator_opacity_set_params(animator, 50, 255);
  widget_animator_set_yoyo(animator, 1000);
  widget_animator_start(animator);
}

static void on_value_animator(widget_t* widget) {
  widget_animator_t* animator = widget_animator_value_create(widget, 1000, 0, EASING_SIN_INOUT);
  widget_animator_value_set_params(animator, 50, 100);
  widget_animator_set_yoyo(animator, 1000);
  widget_animator_start(animator);
}

static ret_t install_one(void* ctx, void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget->name.size) {
    const char* name = widget->name.str;
    if (strstr(name, "open:") != NULL) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
    } else if (strstr(name, "memtest") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_mem_test, win);
    } else if (strstr(name, "inc") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_inc, win);
    } else if (strstr(name, "show_fps") != NULL) {
      widget_on(widget, EVT_CLICK, on_show_fps, widget);
    } else if (strstr(name, "dec") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_dec, win);
    } else if (strstr(name, "close") != NULL) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_close, win);
      }
    } else if (strstr(name, "quit") != NULL) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_quit, win);
      }
    } else if (strcmp(name, "move") == 0) {
      on_move_animator(widget);
    } else if (strcmp(name, "fade") == 0) {
      on_fade_animator(widget);
    } else if (strcmp(name, "value") == 0) {
      on_value_animator(widget);
    }
  }
  (void)ctx;

  return RET_OK;
}

static void install_click_hander(widget_t* widget) {
  widget_foreach(widget, install_one, widget);
}

#include "base/idle.h"
#include "base/resource_manager.h"

static uint32_t s_preload_nr = 0;
static const preload_res_t s_preload_res[] = {{RESOURCE_TYPE_IMAGE, "bg800x480"},
                                              {RESOURCE_TYPE_IMAGE, "earth"},
                                              {RESOURCE_TYPE_IMAGE, "dialog_title"},
                                              {RESOURCE_TYPE_IMAGE, "rgb"},
                                              {RESOURCE_TYPE_IMAGE, "rgba"}};

static ret_t timer_preload(const timer_info_t* timer) {
  char text[64];
  widget_t* win = WIDGET(timer->ctx);
  uint32_t total = ARRAY_SIZE(s_preload_res);
  widget_t* bar = widget_lookup(win, "bar", TRUE);
  widget_t* status = widget_lookup(win, "status", TRUE);

  if (s_preload_nr == total) {
    open_window("main", win);

    return RET_REMOVE;
  } else {
    uint32_t value = 0;
    const preload_res_t* iter = s_preload_res + s_preload_nr++;
    switch (iter->type) {
      case RESOURCE_TYPE_IMAGE: {
        bitmap_t img;
        image_manager_load(image_manager(), iter->name, &img);
        break;
      }
      default: {
        resource_manager_ref(resource_manager(), iter->type, iter->name);
        break;
      }
    }

    value = (s_preload_nr * 100) / total;
    tk_snprintf(text, sizeof(text), "Load: %s(%u/%u)", iter->name, s_preload_nr, total);
    widget_set_value(bar, value);
    widget_set_text_utf8(status, text);

    return RET_REPEAT;
  }
}

static ret_t show_preload_res_window() {
  uint32_t interval = 2000 / ARRAY_SIZE(s_preload_res);
  widget_t* win = window_open("preload");
  timer_add(timer_preload, win, interval);

  return RET_OK;
}

ret_t application_init() {
  return show_preload_res_window();
}

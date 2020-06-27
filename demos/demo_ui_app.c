/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  demoui
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

/*
 * XXX: 本示例只是为了展示功能，不适合作为编写代码参考，编写代码请参考：https://github.com/zlgopen/awtk-c-demos
 */

#include "awtk.h"
#include "ext_widgets.h"
#include "base/font_manager.h"
#include "base/event_recorder_player.h"

static ret_t on_clone_tab(void* ctx, event_t* e);
static ret_t widget_clone_tab(widget_t* widget);
static void install_click_hander(widget_t* widget);
static void open_window(const char* name, widget_t* to_close);

uint32_t tk_mem_speed_test(void* buffer, uint32_t length, uint32_t* pmemcpy_speed,
                           uint32_t* pmemset_speed) {
  uint32_t i = 0;
  uint32_t cost = 0;
  uint32_t total_cost = 0;
  uint32_t memcpy_speed;
  uint32_t memset_speed;
  uint32_t max_size = 100 * 1024 * 1024;
  uint64_t start = time_now_ms();
  uint32_t nr = max_size / length;

  for (i = 0; i < nr; i++) {
    memset(buffer, 0x00, length);
  }
  cost = time_now_ms() - start;
  total_cost = cost;
  if (cost) {
    memset_speed = ((max_size / cost) * 1000) / 1024;
  }

  start = time_now_ms();
  for (i = 0; i < nr; i++) {
    uint32_t half = length >> 1;
    memcpy(buffer, (char*)buffer + half, half);
    memcpy((char*)buffer + half, buffer, half);
  }
  cost = time_now_ms() - start;
  total_cost += cost;

  if (cost) {
    memcpy_speed = ((max_size / cost) * 1000) / 1024;
  }

  if (pmemset_speed != NULL) {
    *pmemset_speed = memset_speed;
  }

  if (pmemcpy_speed != NULL) {
    *pmemcpy_speed = memcpy_speed;
  }

  return total_cost;
}

static ret_t window_to_background(void* ctx, event_t* e) {
  widget_t* win = WIDGET(e->target);
  log_debug("%s to_background\n", win->name);
  (void)win;
  return RET_OK;
}

static ret_t window_to_foreground(void* ctx, event_t* e) {
  widget_t* win = WIDGET(e->target);
  log_debug("%s to_foreground\n", win->name);
  (void)win;
  return RET_OK;
}

static ret_t on_context_menu(void* ctx, event_t* e) {
  open_window("menu_point", NULL);

  return RET_OK;
}

static ret_t update_title_on_timer(const timer_info_t* info) {
  char text[128];
  tk_snprintf(text, sizeof(text), "change title:%d", random() % 100);

  widget_set_text_utf8(WIDGET(info->ctx), text);

  return RET_REPEAT;
}

static void open_window(const char* name, widget_t* to_close) {
  widget_t* win = to_close ? window_open_and_close(name, to_close) : window_open(name);

  widget_on(win, EVT_WINDOW_TO_BACKGROUND, window_to_background, win);
  widget_on(win, EVT_WINDOW_TO_FOREGROUND, window_to_foreground, win);

  install_click_hander(win);

  if (tk_str_eq(name, "tab_scrollable")) {
    widget_clone_tab(win);
    widget_clone_tab(win);
    widget_clone_tab(win);
    widget_clone_tab(win);
  }

  if (tk_str_eq(name, "list_view")) {
    widget_add_timer(win, update_title_on_timer, 1000);
  }

  if (tk_str_eq(widget_get_type(win), WIDGET_TYPE_DIALOG)) {
    int32_t ret = dialog_modal(win);

    if (tk_str_eq(win->name, "back_to_home") && ret == 0) {
      window_manager_back_to_home(window_manager());
    }
  }
}

static ret_t on_paint_linear_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0xff, 0, 0, 0xff);
  color_t ecolor = color_init(0xff, 0, 0, 0x0);
  uint32_t spacing = 10;
  uint32_t w = (widget->w - 3 * spacing) >> 1;
  uint32_t h = (widget->h - 3 * spacing) >> 1;
  rect_t r = rect_init(spacing, spacing, w, h);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  vgcanvas_set_fill_linear_gradient(vg, r.x, r.y, r.x + r.w, r.y + r.h, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, r.x + r.w, 0);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0, 0xff, 0xff);
  vgcanvas_set_fill_linear_gradient(vg, r.x, r.y, r.x + r.w, r.y, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, 0, r.y + r.h);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0xff, 0xff, 0xff);
  vgcanvas_set_fill_linear_gradient(vg, r.x, r.y, r.x + r.w, r.y, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, -(r.x + r.w), 0);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0, 0xff, 0xff);
  vgcanvas_set_fill_linear_gradient(vg, r.x, r.y, r.x, r.y + r.h, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_paint_radial_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0xff, 0, 0, 0xff);
  color_t ecolor = color_init(0xff, 0, 0, 0);
  uint32_t spacing = 10;
  uint32_t w = (widget->w - 3 * spacing) >> 1;
  uint32_t h = (widget->h - 3 * spacing) >> 1;
  rect_t r = rect_init(spacing, spacing, w, h);
  uint32_t radial = tk_min(w, h) / 2;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  vgcanvas_set_fill_radial_gradient(vg, r.x + w / 2, r.y + h / 2, 0, radial, scolor, ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, r.x + r.w, 0);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0, 0xff, 0xff);
  vgcanvas_set_fill_radial_gradient(vg, r.x + w / 2, r.y + h / 2, radial / 4, radial, scolor,
                                    ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, 0, r.y + r.h);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0xff, 0xff, 0xff);
  vgcanvas_set_fill_radial_gradient(vg, r.x + w / 2, r.y + h / 2, radial / 3, radial, scolor,
                                    ecolor);
  vgcanvas_fill(vg);

  vgcanvas_translate(vg, -(r.x + r.w), 0);
  vgcanvas_rect(vg, r.x, r.y, r.w, r.h);
  ecolor = color_init(0, 0, 0xff, 0xff);
  vgcanvas_set_fill_radial_gradient(vg, r.x + w / 2, r.y + h / 2, radial / 2, radial, scolor,
                                    ecolor);
  vgcanvas_fill(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_paint_stroke_gradient(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  widget_t* widget = WIDGET(e->target);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  color_t scolor = color_init(0xff, 0, 0, 0xff);
  color_t ecolor = color_init(0, 0xff, 0, 0xff);
  uint32_t r = tk_min(widget->w, widget->h) / 3;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_stroke_linear_gradient(vg, 0, 0, widget->w, widget->h, scolor, ecolor);

  vgcanvas_move_to(vg, 0, 0);
  vgcanvas_set_line_width(vg, 5);

  vgcanvas_line_to(vg, widget->w / 2, widget->h);
  vgcanvas_line_to(vg, widget->w / 2, 0);
  vgcanvas_line_to(vg, widget->w, widget->h);
  vgcanvas_stroke(vg);

  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, widget->w / 2, widget->h / 2, r, 0, M_PI * 2, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_restore(vg);

  return RET_OK;
}

#include "vg_common.inc"

static ret_t on_paint_vgcanvas(void* ctx, event_t* e) {
  paint_event_t* evt = paint_event_cast(e);
  canvas_t* c = evt->c;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_line_width(vg, 1);
  vgcanvas_set_stroke_color(vg, color_init(0, 0xff, 0, 0xff));
  vgcanvas_set_fill_color(vg, color_init(0xff, 0, 0, 0xff));

  draw_basic_shapes(vg, FALSE);
  vgcanvas_translate(vg, 0, 50);
  draw_basic_shapes(vg, TRUE);
  vgcanvas_translate(vg, 0, 50);
  stroke_lines(vg);
  vgcanvas_translate(vg, 0, 50);
  draw_image(vg);

  vgcanvas_translate(vg, 50, 100);
  draw_matrix(vg);
  vgcanvas_translate(vg, 0, 100);

  draw_text(vg);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t on_open_window(void* ctx, event_t* e) {
  const char* name = (const char*)ctx;

  if (tk_str_eq(name, "toast")) {
    dialog_toast("Hello AWTK!\nThis is a toast!", 3000);
  } else if (tk_str_eq(name, "info")) {
    dialog_info("info", "hello awtk");
  } else if (tk_str_eq(name, "warn")) {
    dialog_warn(NULL, "Hello AWTK!\nDanger!!!");
  } else if (tk_str_eq(name, "confirm")) {
    dialog_confirm(NULL, "Hello AWTK!\nAre you sure to close?");
  } else {
    open_window(name, NULL);
  }

  (void)e;

#if 0
  /*for test only*/
  widget_on(WIDGET(e->target), EVT_CLICK, on_open_window, (void*)name);
  return RET_REMOVE;
#else
  return RET_OK;
#endif
}

static ret_t on_fullscreen(void* ctx, event_t* e) {
  widget_t* btn = WIDGET(ctx);
  window_t* win = WINDOW(widget_get_window(btn));

  if (win->fullscreen) {
    window_set_fullscreen(WIDGET(win), FALSE);
    widget_set_text_utf8(btn, "Fullscreen");
  } else {
    window_set_fullscreen(WIDGET(win), TRUE);
    widget_set_text_utf8(btn, "Unfullscreen");
  }

  return RET_OK;
}

static ret_t on_close(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  (void)e;
  return window_close(win);
}

static ret_t on_start(void* ctx, event_t* e) {
  widget_start_animator(NULL, NULL);

  return RET_OK;
}

static ret_t on_pause(void* ctx, event_t* e) {
  widget_pause_animator(NULL, NULL);

  return RET_OK;
}

static ret_t on_stop(void* ctx, event_t* e) {
  widget_stop_animator(NULL, NULL);

  return RET_OK;
}

static ret_t on_send_key(void* ctx, event_t* e) {
  widget_t* button = WIDGET(e->target);
  char text[2];
  text[0] = (char)button->text.str[0];
  text[1] = '\0';

  input_method_commit_text(input_method(), text);

  return RET_OK;
}

static ret_t on_backspace(void* ctx, event_t* e) {
  input_method_dispatch_key(input_method(), TK_KEY_BACKSPACE);

  return RET_OK;
}

static ret_t on_quit(void* ctx, event_t* e) {
  widget_t* dialog = WIDGET(ctx);

  dialog_quit(dialog, 0);
  (void)e;
  return RET_OK;
}

static ret_t on_back_to_home(void* ctx, event_t* e) {
  widget_t* dialog = WIDGET(ctx);

  dialog_quit(dialog, 0);

  (void)e;
  return RET_OK;
}

static ret_t on_quit_app(void* ctx, event_t* e) {
  tk_quit();

  return RET_OK;
}

static ret_t on_combo_box_will_change(void* ctx, event_t* e) {
  widget_t* combo_box = WIDGET(ctx);
  widget_t* win = widget_get_window(combo_box);
  widget_t* value = widget_lookup(win, "old_value", TRUE);

  if (value != NULL) {
    widget_set_tr_text(value, combo_box_get_text(combo_box));
  }

  return RET_OK;
}

static ret_t on_pages_add_child(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* win = widget_get_window(widget);

  widget_t* close_btn = widget_lookup(win, "close", TRUE);
  widget_t* text_label = widget_lookup(win, "text", TRUE);
  widget_t* tab_button_parent = widget_lookup_by_type(win, "tab_button", TRUE)->parent;

  if (close_btn != NULL) {
    widget_on(close_btn, EVT_CLICK, on_close, win);
  }

  if (text_label != NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(tab_button_parent, iter, i)

    if (tk_str_eq(iter->vt->type, "tab_button")) {
      if (TAB_BUTTON(iter)->value) {
        widget_set_text_utf8(text_label, iter->name);
      }
    }

    WIDGET_FOR_EACH_CHILD_END();
  }

  return RET_OK;
}

static ret_t on_combo_box_changed(void* ctx, event_t* e) {
  widget_t* combo_box = WIDGET(ctx);
  widget_t* win = widget_get_window(combo_box);
  widget_t* value = widget_lookup(win, "value", TRUE);

  if (value != NULL) {
    widget_set_tr_text(value, combo_box_get_text(combo_box));
  }

  return RET_OK;
}

static ret_t on_remove_self(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_remove_child(widget->parent, widget);
  widget_destroy(widget);

  return RET_OK;
}

static ret_t on_clone_self(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* clone = widget_clone(widget, widget->parent);
  widget_on(clone, EVT_CLICK, on_clone_self, clone);

  return RET_OK;
}

static ret_t widget_clone_tab(widget_t* widget) {
  char text[32];
  widget_t* button = widget_lookup(widget, "clone_button", TRUE);
  widget_t* view = widget_lookup(widget, "clone_view", TRUE);
  widget_t* new_button = widget_clone(button, button->parent);

  widget_t* new_view = widget_clone(view, view->parent);
  tk_snprintf(text, sizeof(text), "Clone(%d)", widget_index_of(new_button));
  widget_set_text_utf8(new_button, text);
  widget_set_value(new_button, TRUE);

  widget_child_on(new_view, "clone_tab", EVT_CLICK, on_clone_tab, widget_get_window(widget));
  widget_set_text_utf8(widget_lookup_by_type(new_view, WIDGET_TYPE_LABEL, TRUE), text);

  return RET_OK;
}

static ret_t on_clone_tab(void* ctx, event_t* e) {
  return widget_clone_tab(WIDGET(ctx));
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

extern ret_t assets_set_global_theme(const char* name);
static ret_t on_reload_theme_test(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  assets_manager_t* am = widget_get_assets_manager(widget);
  const char* theme = "default";

  if (tk_str_eq(am->theme, theme)) {
    theme = "dark";
  }

  assets_set_global_theme(theme);

  return RET_OK;
}

static ret_t on_snapshot(void* ctx, event_t* e) {
#ifndef AWTK_WEB
  bitmap_t* bitmap = widget_take_snapshot(window_manager());
  bitmap_save_png(bitmap, "test.png");
  bitmap_destroy(bitmap);
#endif /*AWTK_WEB*/

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

  font_manager_shrink_cache(font_manager(), 1);

  return RET_OK;
}

static ret_t progress_bar_animate_delta(widget_t* win, const char* name, int32_t delta) {
  widget_t* progress_bar = widget_lookup(win, name, TRUE);
  int32_t value = (PROGRESS_BAR(progress_bar)->value + delta);
  widget_animate_value_to(progress_bar, tk_max(0, tk_min(100, value)), 500);

  return RET_OK;
}

static ret_t on_inc(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  progress_bar_animate_delta(win, "bar1", 10);
  progress_bar_animate_delta(win, "bar2", 10);
  (void)e;
  return RET_OK;
}

static ret_t on_dec(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  progress_bar_animate_delta(win, "bar1", -10);
  progress_bar_animate_delta(win, "bar2", -10);

  (void)e;
  return RET_OK;
}

static ret_t on_change_font_size(void* ctx, event_t* e) {
  float_t font_scale = 1;
  widget_t* win = WIDGET(ctx);

  if (widget_get_value(widget_lookup(win, "font_small", TRUE))) {
    font_scale = 0.9;
  } else if (widget_get_value(widget_lookup(win, "font_big", TRUE))) {
    font_scale = 1.1;
  }
  system_info_set_font_scale(system_info(), font_scale);
  widget_invalidate_force(win, NULL);

  return RET_OK;
}

static ret_t on_change_locale(void* ctx, event_t* e) {
  char country[3];
  char language[3];
  const char* str = (const char*)ctx;

  tk_strncpy(language, str, 2);
  tk_strncpy(country, str + 3, 2);
  locale_info_change(locale_info(), language, country);

  return RET_OK;
}

static ret_t install_one(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);
  widget_t* win = widget_get_window(widget);

  if (widget->name != NULL) {
    const char* name = widget->name;
    if (strstr(name, "open:") != NULL) {
      widget_on(widget, EVT_CLICK, on_open_window, (void*)(name + 5));
      widget_on(widget, EVT_LONG_PRESS, on_open_window, (void*)(name + 5));
      if (tk_str_eq(name, "open:menu_point")) {
        widget_on(widget, EVT_CONTEXT_MENU, on_context_menu, win);
      }

    } else if (tk_str_eq(name, "paint_linear_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_linear_gradient, NULL);
    } else if (tk_str_eq(name, "paint_radial_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_radial_gradient, NULL);
    } else if (tk_str_eq(name, "paint_stroke_gradient")) {
      widget_on(widget, EVT_PAINT, on_paint_stroke_gradient, NULL);
    } else if (tk_str_eq(name, "paint_vgcanvas")) {
      widget_on(widget, EVT_PAINT, on_paint_vgcanvas, NULL);
    } else if (tk_str_eq(name, "snapshot")) {
      widget_on(widget, EVT_CLICK, on_snapshot, NULL);
    } else if (tk_str_eq(name, "memtest")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_mem_test, win);
    } else if (tk_str_eq(name, "reload_theme")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_reload_theme_test, win);
    } else if (tk_str_eq(name, "show_fps")) {
      widget_on(widget, EVT_CLICK, on_show_fps, widget);
    } else if (tk_str_eq(name, "clone_self")) {
      widget_on(widget, EVT_CLICK, on_clone_self, widget);
    } else if (tk_str_eq(name, "clone_tab")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_clone_tab, win);
    } else if (tk_str_eq(name, "remove_self")) {
      widget_on(widget, EVT_CLICK, on_remove_self, widget);
    } else if (tk_str_eq(name, "chinese")) {
      widget_on(widget, EVT_CLICK, on_change_locale, (void*)"zh_CN");
    } else if (tk_str_eq(name, "english")) {
      widget_on(widget, EVT_CLICK, on_change_locale, (void*)"en_US");
    } else if (tk_str_eq(name, "font_small") || tk_str_eq(name, "font_normal") ||
               tk_str_eq(name, "font_big")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_VALUE_CHANGED, on_change_font_size, win);
    } else if (tk_str_eq(name, "inc_value")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_inc, win);
    } else if (strstr(name, "dec_value") != NULL) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_dec, win);
    } else if (tk_str_eq(name, "close")) {
      widget_on(widget, EVT_CLICK, on_close, win);
    } else if (tk_str_eq(name, "fullscreen")) {
      widget_on(widget, EVT_CLICK, on_fullscreen, widget);
    } else if (tk_str_eq(name, "start")) {
      widget_on(widget, EVT_CLICK, on_start, win);
    } else if (tk_str_eq(name, "pause")) {
      widget_on(widget, EVT_CLICK, on_pause, win);
    } else if (tk_str_eq(name, "stop")) {
      widget_on(widget, EVT_CLICK, on_stop, win);
    } else if (tk_str_eq(name, "key")) {
      widget_on(widget, EVT_CLICK, on_send_key, NULL);
    } else if (tk_str_eq(name, "backspace")) {
      widget_on(widget, EVT_CLICK, on_backspace, NULL);
    } else if (tk_str_eq(name, "quit")) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_quit, win);
      }
    } else if (tk_str_eq(name, "back_to_home")) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_back_to_home, win);
      }
    } else if (tk_str_eq(name, "exit")) {
      widget_t* win = widget_get_window(widget);
      if (win) {
        widget_on(widget, EVT_CLICK, on_quit_app, win);
      }
    } else if (tk_str_eq(name, "pages")) {
      widget_on(widget, EVT_WIDGET_ADD_CHILD, on_pages_add_child, widget);
    }
  } else if (tk_str_eq(widget->vt->type, "combo_box")) {
    widget_on(widget, EVT_VALUE_CHANGED, on_combo_box_changed, widget);
    widget_on(widget, EVT_VALUE_WILL_CHANGE, on_combo_box_will_change, widget);
  }
  (void)ctx;

  return RET_OK;
}

static void install_click_hander(widget_t* widget) {
  widget_foreach(widget, install_one, widget);
}

#include "base/idle.h"
#include "base/assets_manager.h"

static uint32_t s_preload_nr = 0;
static const preload_res_t s_preload_res[] = {{ASSET_TYPE_IMAGE, "earth"},
                                              {ASSET_TYPE_IMAGE, "dialog_title"},
                                              {ASSET_TYPE_IMAGE, "rgb"},
                                              {ASSET_TYPE_IMAGE, "rgba"}};

static ret_t timer_preload(const timer_info_t* timer) {
  char text[64];
  widget_t* win = WIDGET(timer->ctx);
  uint32_t total = ARRAY_SIZE(s_preload_res);
  widget_t* bar = widget_lookup(win, "bar", TRUE);
  widget_t* status = widget_lookup(win, "status", TRUE);

  if (s_preload_nr == total) {
#if !defined(MOBILE_APP)
    window_open("system_bar");
/*    window_open("system_bar_bottom");*/
#endif /*MOBILE_APP*/

    open_window("main", win);

    return RET_REMOVE;
  } else {
    uint32_t value = 0;
    const preload_res_t* iter = s_preload_res + s_preload_nr++;
    switch (iter->type) {
      case ASSET_TYPE_IMAGE: {
        bitmap_t img;
        image_manager_get_bitmap(image_manager(), iter->name, &img);
        break;
      }
      default: {
        assets_manager_ref(assets_manager(), iter->type, iter->name);
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
  uint32_t interval = 500 / ARRAY_SIZE(s_preload_res);
  widget_t* win = window_open("preload");
  timer_add(timer_preload, win, interval);

  return RET_OK;
}

static ret_t close_window_on_event(void* ctx, event_t* e) {
  window_close(WIDGET(ctx));

  return RET_REMOVE;
}

static ret_t on_screen_saver(void* ctx, event_t* e) {
  widget_t* win = NULL;
  const char* screen_saver_win = "image_animation";

  if (widget_child(window_manager(), screen_saver_win) != NULL) {
    log_debug("screen saver exist.\n");
    return RET_OK;
  }

  win = window_open(screen_saver_win);
  widget_on(win, EVT_POINTER_MOVE, close_window_on_event, win);
  widget_on(win, EVT_POINTER_UP, close_window_on_event, win);
  widget_on(win, EVT_KEY_UP, close_window_on_event, win);

  return RET_OK;
}

static ret_t on_key_record_play_events(void* ctx, event_t* e) {
#ifdef WITH_EVENT_RECORDER_PLAYER
  key_event_t* evt = (key_event_t*)e;

  if (evt->key == TK_KEY_F5) {
    event_recorder_player_start_record("event_log.bin");
    return RET_STOP;
  } else if (evt->key == TK_KEY_F6) {
    event_recorder_player_stop_record();
    return RET_STOP;
  } else if (evt->key == TK_KEY_F7) {
    event_recorder_player_start_play("event_log.bin", 0xffff);
    return RET_STOP;
  } else if (evt->key == TK_KEY_F8) {
    event_recorder_player_stop_play();
    return RET_STOP;
  } else if (evt->key == TK_KEY_F9) {
    tk_mem_dump();
    return RET_STOP;
  } else if (evt->key == TK_KEY_F10) {
    font_manager_unload_all(font_manager());
    image_manager_unload_all(image_manager());
    assets_manager_clear_cache(assets_manager(), ASSET_TYPE_UI);
    tk_mem_dump();
    return RET_STOP;
  }
#endif /*WITH_EVENT_RECORDER_PLAYER*/

  return RET_OK;
}

static ret_t on_key_back_or_back_to_home(void* ctx, event_t* e) {
  key_event_t* evt = (key_event_t*)e;
  if (evt->key == TK_KEY_F2) {
    window_manager_back(WIDGET(ctx));

    return RET_STOP;
  } else if (evt->key == TK_KEY_F3) {
    window_manager_back_to_home(WIDGET(ctx));

    return RET_STOP;
  } else if (evt->key == TK_KEY_F4) {
    window_manager_back_to(WIDGET(ctx), "main");

    return RET_STOP;
  }

  return RET_OK;
}

static ret_t wm_on_before_paint(void* ctx, event_t* e) {
  return RET_OK;
}

static ret_t wm_on_after_paint(void* ctx, event_t* e) {
  return RET_OK;
}

static ret_t wm_on_low_memory(void* ctx, event_t* evt) {
  log_debug("low memory\n");
  return RET_OK;
}

static ret_t wm_on_out_of_memory(void* ctx, event_t* evt) {
  log_debug("out of memory\n");
  return RET_OK;
}

static ret_t wm_on_request_quit(void* ctx, event_t* evt) {
  /*
   * do some cleanup work here
   * return RET_STOP to ignore the request
   */
  /*return RET_STOP;*/

  return RET_OK;
}

ret_t application_init() {
  char path[MAX_PATH + 1];
  widget_t* wm = window_manager();

  /*enable screen saver*/
  window_manager_set_screen_saver_time(wm, 180 * 1000);
  widget_on(wm, EVT_SCREEN_SAVER, on_screen_saver, NULL);

  widget_on(wm, EVT_KEY_DOWN, on_key_back_or_back_to_home, wm);
  widget_on(wm, EVT_KEY_UP, on_key_record_play_events, wm);
  widget_on(wm, EVT_BEFORE_PAINT, wm_on_before_paint, wm);
  widget_on(wm, EVT_AFTER_PAINT, wm_on_after_paint, wm);
  widget_on(wm, EVT_LOW_MEMORY, wm_on_low_memory, wm);
  widget_on(wm, EVT_OUT_OF_MEMORY, wm_on_out_of_memory, wm);
  widget_on(wm, EVT_REQUEST_QUIT_APP, wm_on_request_quit, wm);

  fs_get_user_storage_path(os_fs(), path);
  log_debug("user storage path:%s\n", path);

  return show_preload_res_window();
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#ifdef WITH_FS_RES
#define APP_DEFAULT_FONT "default_full"
#endif /*WITH_FS_RES*/

#include "awtk_main.inc"

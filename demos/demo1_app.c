/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic demo
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

#include "base/timer.h"
#include "base/enums.h"
#include "widgets/button.h"
#include "base/dialog.h"
#include "widgets/image.h"
#include "widgets/label.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/utf8.h"
#include "base/window.h"
#include "widgets/slider.h"
#include "widgets/group_box.h"
#include "widgets/check_button.h"
#include "base/image_manager.h"
#include "widgets/progress_bar.h"

#include "common.inc"

static ret_t on_show_dialog(void* ctx, event_t* e) {
  uint32_t code = 0;
  widget_t* ok = NULL;
  widget_t* label = NULL;
  widget_t* cancel = NULL;
  widget_t* win = dialog_create_simple(NULL, 0, 0, 240, 160);
  dialog_t* dialog = DIALOG(win);

  widget_set_text(dialog->title, L"Dialog");

  ok = button_create(dialog->client, 20, 80, 80, 30);
  widget_set_text(ok, L"Go");

  cancel = button_create(dialog->client, 140, 80, 80, 30);
  widget_set_text(cancel, L"Cancel");

  label = label_create(dialog->client, 10, 30, 200, 30);
  widget_set_text(label, L"Are you ready!");

  widget_on(ok, EVT_CLICK, on_ok, dialog);
  widget_on(cancel, EVT_CLICK, on_cancel, dialog);

  code = dialog_modal(win);
  log_debug("code=%d\n", (int)code);

  (void)code;
  tk_mem_dump();
  (void)e;

  return RET_OK;
}

ret_t application_init() {
  widget_t* image = NULL;
  widget_t* ok = NULL;
  widget_t* label = NULL;
  widget_t* cancel = NULL;
  widget_t* slider = NULL;
  widget_t* progress_bar = NULL;
  widget_t* check_button = NULL;
  widget_t* radio_button = NULL;
  widget_t* show_dialog = NULL;
  widget_t* group_box = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);

  ok = button_create(win, 10, 5, 80, 30);
  widget_set_text(ok, L"Inc");

  cancel = button_create(win, 100, 5, 80, 30);
  widget_set_text(cancel, L"Dec");

  show_dialog = button_create(win, 190, 5, 80, 30);
  widget_set_text(show_dialog, L"Dialog");

  image = image_create(win, 10, 230, 100, 100);
  image_set_image(image, "earth");
  image_set_draw_type(image, IMAGE_DRAW_ICON);

  image = image_create(win, 100, 230, 100, 100);
  image_set_image(image, "earth");
  image_set_draw_type(image, IMAGE_DRAW_SCALE);

  image = image_create(win, 10, 360, 100, 100);
  image_set_image(image, "bricks");
  image_set_draw_type(image, IMAGE_DRAW_ICON);

  image = image_create(win, 100, 360, 100, 100);
  image_set_image(image, "bricks");
  image_set_draw_type(image, IMAGE_DRAW_SCALE);

  label = label_create(win, 10, 40, 80, 30);
  widget_set_text(label, L"Left");
  widget_use_style(label, "left");

  label = label_create(win, 100, 40, 80, 30);
  widget_set_text(label, L"C enter");
#ifdef WITH_STB_FONT
  widget_use_style(label, "center");
#else
  widget_use_style(label, "center-ap");
#endif

  label = label_create(win, 190, 40, 80, 30);
  widget_set_text(label, L"Right");
  widget_use_style(label, "right");

  progress_bar = progress_bar_create(win, 10, 80, 168, 20);
  widget_set_value(progress_bar, 40);
  timer_add(on_timer, progress_bar, 500);

#ifdef WITH_STB_FONT
  progress_bar_set_show_text(progress_bar, TRUE);
#endif

  slider = slider_create(win, 10, 105, 168, 20);
  widget_set_value(slider, 40);

  progress_bar = progress_bar_create(win, 260, 80, 20, 118);
  widget_set_value(progress_bar, 40);
  progress_bar_set_vertical(progress_bar, TRUE);
  timer_add(on_timer, progress_bar, 500);

  slider = slider_create(win, 230, 80, 20, 118);
  widget_set_value(slider, 40);
  slider_set_vertical(slider, TRUE);

  widget_on(ok, EVT_CLICK, on_inc, progress_bar);
  widget_on(cancel, EVT_CLICK, on_dec, progress_bar);
  widget_on(show_dialog, EVT_CLICK, on_show_dialog, NULL);

  check_button = check_button_create(win, 10, 150, 80, 30);
  widget_set_text(check_button, L"Book");

  check_button = check_button_create(win, 100, 150, 80, 30);
  widget_set_text(check_button, L"Food");

  radio_button = check_button_create_radio(win, 10, 200, 80, 30);
  widget_set_text(radio_button, L"Book");

  radio_button = check_button_create_radio(win, 100, 200, 80, 30);
  widget_set_text(radio_button, L"Food");

  radio_button = check_button_create_radio(win, 190, 200, 80, 30);
  widget_set_text(radio_button, L"Pencil");
  widget_set_value(radio_button, TRUE);

  group_box = group_box_create(win, 10, 300, 32 * 3, 32);
  widget_use_style(group_box, "box");
  radio_button = check_button_create_radio(group_box, 0, 0, 32, 32);
  widget_use_style(radio_button, "left");
  radio_button = check_button_create_radio(group_box, 32, 0, 32, 32);
  widget_use_style(radio_button, "middle");
  radio_button = check_button_create_radio(group_box, 64, 0, 32, 32);
  widget_use_style(radio_button, "right");

  check_button = check_button_create(win, 108, 300, 32, 32);
  widget_use_style(check_button, "mute");

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

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

#include "base/timer.h"
#include "base/enums.h"
#include "base/button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/label.h"
#include "base/mem.h"
#include "base/utils.h"
#include "base/utf8.h"
#include "base/window.h"
#include "base/slider.h"
#include "base/group_box.h"
#include "base/check_button.h"
#include "base/image_manager.h"
#include "base/progress_bar.h"
#include "widget_animators/widget_animator_opacity.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_move.h"

static uint32_t animators_nr = 0;
static widget_animator_t* animators[10];

static ret_t on_start(void* ctx, event_t* e) {
  uint32_t i = 0;
  for (i = 0; i < animators_nr; i++) {
    widget_animator_start(animators[i]);
  }

  return RET_OK;
}

static ret_t on_stop(void* ctx, event_t* e) {
  uint32_t i = 0;
  for (i = 0; i < animators_nr; i++) {
    widget_animator_stop(animators[i]);
  }

  return RET_OK;
}

ret_t application_init() {
  int yoyo = 100;
  widget_t* image = NULL;
  widget_t* image1 = NULL;
  widget_t* stop = NULL;
  widget_t* start = NULL;
  widget_t* opacity = NULL;
  widget_t* image_opacity = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_animator_t* animator = NULL;
  uint32_t delay = 0;

  start = button_create(win, 10, 5, 80, 30);
  widget_set_text(start, L"Start");

  stop = button_create(win, 100, 5, 80, 30);
  widget_set_text(stop, L"Stop");

  image = image_create(win, 10, 230, 100, 100);
  image_set_image_name(image, "earth");

  image1 = image_create(win, 10, 230, 100, 100);
  image_set_image_name(image1, "earth");

  image_opacity = image_create(win, 10, 150, 32, 32);
  image_set_image_name(image_opacity, "earth");

  opacity = button_create(win, 100, 150, 80, 30);
  widget_set_text(opacity, L"Opacity");

  progress_bar = progress_bar_create(win, 10, 80, 168, 20);

  animator = widget_animator_move_create(image, 1000, delay, easing_get(EASING_SIN_INOUT));
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_repeat(animator, yoyo);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_move_create(image1, 1000, delay, easing_get(EASING_SIN_INOUT));
  widget_animator_move_set_params(animator, image1->x, image1->y, image1->x + 100, image->y);
  widget_animator_set_yoyo(animator, yoyo);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_value_create(progress_bar, 1000, delay, easing_get(EASING_SIN_INOUT));
  widget_animator_value_set_params(animator, 50, 100);
  widget_animator_set_yoyo(animator, yoyo);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_opacity_create(opacity, 1000, delay, easing_get(EASING_SIN_OUT));
  widget_animator_opacity_set_params(animator, 50, 255);
  widget_animator_set_yoyo(animator, yoyo);
  animators[animators_nr++] = animator;

  animator = widget_animator_opacity_create(image_opacity, 1000, delay, easing_get(EASING_SIN_INOUT));
  widget_animator_opacity_set_params(animator, 50, 255);
  widget_animator_set_yoyo(animator, yoyo);
  animators[animators_nr++] = animator;

  widget_on(start, EVT_CLICK, on_start, NULL);
  widget_on(stop, EVT_CLICK, on_stop, NULL);

  return RET_OK;
}

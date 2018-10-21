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
#include "widget_animators/widget_animator_scale.h"
#include "widget_animators/widget_animator_rotation.h"

static uint32_t animators_nr = 0;
static widget_animator_t* animators[50];

static ret_t on_start(void* ctx, event_t* e) {
  uint32_t i = 0;
  for (i = 0; i < animators_nr; i++) {
    widget_animator_start(animators[i]);
  }

  widget_set_enable(WIDGET(ctx), FALSE);

  return RET_OK;
}

ret_t application_init() {
  int times = 100;
  widget_t* image = NULL;
  widget_t* image1 = NULL;
  widget_t* image2 = NULL;
  widget_t* image3 = NULL;
  widget_t* image4 = NULL;
  widget_t* image5 = NULL;
  widget_t* start = NULL;
  widget_t* opacity = NULL;
  widget_t* image6 = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_animator_t* animator = NULL;
  uint32_t delay = 0;

  start = button_create(win, 10, 10, 80, 30);
  widget_set_text(start, L"Start");

  opacity = button_create(win, 100, 10, 80, 30);
  widget_set_text(opacity, L"Opacity");

  image = image_create(win, 10, 220, 32, 32);
  image_set_image(image, "earth");
  image_set_draw_type(image, IMAGE_DRAW_ICON);

  image1 = image_create(win, 10, 220, 32, 32);
  image_set_image(image1, "earth");
  image_set_draw_type(image1, IMAGE_DRAW_ICON);

  image2 = image_create(win, 10, 118, 100, 100);
  image_set_image(image2, "bricks");
  image_set_draw_type(image2, IMAGE_DRAW_ICON);

  image3 = image_create(win, 50, 118, 32, 32);
  image_set_image(image3, "earth");
  image_set_draw_type(image3, IMAGE_DRAW_ICON);

  image4 = image_create(win, 100, 118, 32, 32);
  image_set_image(image4, "earth");
  image_set_draw_type(image4, IMAGE_DRAW_ICON);

  image5 = image_create(win, 140, 118, 32, 32);
  image_set_image(image5, "earth");
  image_set_draw_type(image5, IMAGE_DRAW_ICON);

  image6 = image_create(win, 180, 118, 32, 32);
  image_set_image(image6, "earth");
  image_set_draw_type(image6, IMAGE_DRAW_ICON);

  progress_bar = progress_bar_create(win, 10, 80, 168, 20);

  animator = widget_animator_move_create(image, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_repeat(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_move_create(image1, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image1->x, image1->y, image1->x + 100, image->y);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_value_create(progress_bar, 1000, delay, EASING_SIN_INOUT);
  widget_animator_value_set_params(animator, 50, 100);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_opacity_create(opacity, 1000, delay, EASING_SIN_OUT);
  widget_animator_opacity_set_params(animator, 50, 255);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  animator = widget_animator_opacity_create(image6, 2000, delay, EASING_SIN_INOUT);
  widget_animator_opacity_set_params(animator, 50, 255);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_rotation_create(image2, 1000, delay, EASING_SIN_INOUT);
  widget_animator_rotation_set_params(animator, 0, 3.14 * 2);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_scale_create(image3, 1000, delay, EASING_SIN_INOUT);
  widget_animator_scale_set_params(animator, 0.5f, 0.5f, 2.0f, 2.0f);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_scale_create(image4, 2000, delay, EASING_SIN_IN);
  widget_animator_scale_set_params(animator, 1.0f, 1.0f, -1.0f, 1.0f);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_scale_create(image5, 1000, delay, EASING_SIN_IN);
  widget_animator_scale_set_params(animator, 1.0f, 1.0f, 0, 0);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_scale_create(image5, 1000, delay, EASING_SIN_IN);
  widget_animator_scale_set_params(animator, 0, 0, 1.0f, 1.0f);
  animators[animators_nr++] = animator;

  animator = widget_animator_move_create(image5, 1000, delay, EASING_SIN_IN);
  widget_animator_move_set_params(animator, image5->x, image5->y, image5->x, image5->y + 100);
  animators[animators_nr++] = animator;

  delay += 1000;
  animator = widget_animator_opacity_create(image5, 2000, delay, EASING_SIN_OUT);
  widget_animator_opacity_set_params(animator, 255, 0);
  widget_animator_set_yoyo(animator, times);
  animators[animators_nr++] = animator;

  widget_on(start, EVT_CLICK, on_start, start);

  return RET_OK;
}

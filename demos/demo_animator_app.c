/**
 * File:   demo1_app.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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
#include "base/widget_animator_manager.h"
#include "widget_animators/widget_animator_opacity.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_move.h"
#include "widget_animators/widget_animator_scale.h"
#include "widget_animators/widget_animator_rotation.h"

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

ret_t application_init() {
  int times = 100;
  widget_t* image = NULL;
  widget_t* image1 = NULL;
  widget_t* image2 = NULL;
  widget_t* image3 = NULL;
  widget_t* image4 = NULL;
  widget_t* image5 = NULL;
  widget_t* start = NULL;
  widget_t* pause = NULL;
  widget_t* stop = NULL;
  widget_t* opacity = NULL;
  widget_t* image6 = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_animator_t* animator = NULL;
  uint32_t delay = 0;

  start = button_create(win, 10, 10, 80, 30);
  widget_set_text(start, L"Start");

  pause = button_create(win, 100, 10, 80, 30);
  widget_set_text(pause, L"Pause");

  stop = button_create(win, 190, 10, 80, 30);
  widget_set_text(stop, L"Stop");

  opacity = button_create(win, 10, 50, 80, 30);
  widget_set_text(opacity, L"Opacity");

  progress_bar = progress_bar_create(win, 10, 90, 168, 20);

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

  animator = widget_animator_move_create(image, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_destroy_when_done(animator, FALSE);
  widget_animator_set_repeat(animator, times);
  widget_animator_start(animator);

  delay += 1000;
  animator = widget_animator_move_create(image1, 1000, delay, EASING_SIN_INOUT);
  widget_animator_move_set_params(animator, image1->x, image1->y, image1->x + 100, image->y);
  widget_animator_set_destroy_when_done(animator, FALSE);
  widget_animator_set_yoyo(animator, times);
  widget_animator_start(animator);

  widget_create_animator(progress_bar,
                         "value(from=50, to=100, duration=500, yoyo_times=1000, delay=1000)");

  widget_create_animator(opacity, "opacity(to=50, duration=500, yoyo_times=1000)");

  widget_create_animator(image6, "opacity(to=50, duration=500, yoyo_times=1000, delay=1000)");

  widget_create_animator(image2, "rotation(to=6.28, yoyo_times=0, duration=1000, easing=sin_out)");

  widget_create_animator(image3, "scale(x_to=-1, yoyo_times=1000, duration=1000, easing=sin_out)");

  widget_create_animator(image4,
                         "scale(x_to=2, y_to=2, yoyo_times=1000, duration=1000, easing=sin_out)");

  widget_create_animator(image5, "y(to=400, duration=1000, easing=sin_out)");
  widget_create_animator(image5, "opacity(to=0, duration=500, yoyo_times=1000, delay=1000)");

  widget_on(start, EVT_CLICK, on_start, NULL);
  widget_on(pause, EVT_CLICK, on_pause, NULL);
  widget_on(stop, EVT_CLICK, on_stop, NULL);

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"

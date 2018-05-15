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
#include "widget_animators/widget_animator_move.h"

static ret_t on_start(void* ctx, event_t* e) {
  widget_animator_start((widget_animator_t*)ctx);

  return RET_OK;
}

static ret_t on_stop(void* ctx, event_t* e) {
  widget_animator_stop((widget_animator_t*)ctx);

  return RET_OK;
}

ret_t application_init() {
  widget_t* image = NULL;
  widget_t* stop = NULL;
  widget_t* start = NULL;
  widget_t* progress_bar = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_animator_t* animator = NULL;
  
  start = button_create(win, 10, 5, 80, 30);
  widget_set_text(start, L"Start");
  
  stop = button_create(win, 128, 5, 80, 30);
  widget_set_text(stop, L"Stop");

  image = image_create(win, 10, 230, 100, 100);
  image_set_image_name(image, "earth");
  
  animator = widget_animator_move_create(image, 1000, NULL);
  widget_animator_set_yoyo(animator, TRUE);

  progress_bar = progress_bar_create(win, 10, 80, 168, 20);
  
  animator = widget_animator_move_create(image, 1000, easing_get(EASING_SIN_INOUT));
  widget_animator_move_set_params(animator, image->x, image->y, image->x + 100, image->y + 100);
  widget_animator_set_yoyo(animator, TRUE);

  widget_on(start, EVT_CLICK, on_start, animator);
  widget_on(stop, EVT_CLICK, on_stop, animator);

  return RET_OK;
}

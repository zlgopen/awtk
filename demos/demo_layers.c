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

#include "awtk.h"
#include "lcd/lcd_mem_rgba8888.h"
#include "layer_window/layer_manager.h"

static canvas_t* s_canvas_fast = NULL;
#define create_fast_canvas(w, h) canvas_offline_create(w, h, BITMAP_FMT_RGBA8888)

static ret_t layers_init(void) {
  layer_t* layer = NULL;
  layer_manager_t* layer_manager = layer_manager_create();
  return_value_if_fail(layer_manager != NULL, RET_BAD_PARAMS);
  layer_manager_set(layer_manager);

  s_canvas_fast = create_fast_canvas(300, 460);
  layer = layer_create("fast", s_canvas_fast, 10, 10, 60);
  assert(layer != NULL);
  layer_manager_add(layer_manager, layer);
  layer_start(layer);
  layer_set_show_fps(layer, TRUE);

  return RET_OK;
}

static ret_t layers_deinit(void) {
  if (s_canvas_fast != NULL) {
    canvas_reset(s_canvas_fast);
  }
  layer_manager_destroy(layer_manager());
  layer_manager_set(NULL);

  return RET_OK;
}

ret_t application_init() {
  widget_t* win = NULL;

  layers_init();
  window_open("system_bar");
  window_open("main");
  win = window_open("layer_window");

  return RET_OK;
}

ret_t application_exit() {
  log_debug("application_exit\n");
  layers_deinit();
  return RET_OK;
}

#include "awtk_main.inc"

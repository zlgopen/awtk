/**
 * File:   main_loop_simple.h
 * Author: AWTK Develop Team
 * Brief:  a simple main loop
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MAIN_LOOP_SIMPLE_H
#define TK_MAIN_LOOP_SIMPLE_H

#include "base/idle.h"
#include "base/timer.h"
#include "tkc/mutex.h"
#include "base/main_loop.h"
#include "base/event_queue.h"
#include "base/font_manager.h"
#include "base/window_manager.h"

BEGIN_C_DECLS

struct _main_loop_simple_t;
typedef struct _main_loop_simple_t main_loop_simple_t;

typedef ret_t (*main_loop_dispatch_input_t)(main_loop_simple_t* loop);

struct _main_loop_simple_t {
  main_loop_t base;
  event_queue_t* queue;

  wh_t w;
  wh_t h;
  bool_t pressed;
  bool_t key_pressed;
  xy_t last_x;
  xy_t last_y;
  uint8_t last_key;
  tk_mutex_t* mutex;
  void* user1;
  void* user2;
  void* user3;
  void* user4;
  event_source_manager_t* event_source_manager;
  main_loop_dispatch_input_t dispatch_input;
};

main_loop_simple_t* main_loop_simple_init(int w, int h, main_loop_queue_event_t queue_event,
                                          main_loop_recv_event_t recv_event);

ret_t main_loop_simple_reset(main_loop_simple_t* loop);
ret_t main_loop_post_key_event(main_loop_t* l, bool_t pressed, uint8_t key);
ret_t main_loop_post_pointer_event(main_loop_t* l, bool_t pressed, xy_t x, xy_t y);

END_C_DECLS

#endif /*TK_MAIN_LOOP_SIMPLE_H*/

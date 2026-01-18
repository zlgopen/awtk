/**
 * File:   main_loop_console.h
 * Author: AWTK Develop Team
 * Brief:  a console main loop
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-06-30 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_MAIN_LOOP_CONSOLE_H
#define TK_MAIN_LOOP_CONSOLE_H

#include "base/idle.h"
#include "base/timer.h"
#include "tkc/mutex.h"
#include "base/main_loop.h"
#include "base/event_queue.h"

BEGIN_C_DECLS

struct _main_loop_console_t;
typedef struct _main_loop_console_t main_loop_console_t;

typedef ret_t (*main_loop_dispatch_input_t)(main_loop_console_t* loop);

struct _main_loop_console_t {
  main_loop_t base;
  event_queue_t* queue;

  tk_mutex_t* mutex;

  event_source_manager_t* event_source_manager;
  main_loop_dispatch_input_t dispatch_input;
};

main_loop_console_t* main_loop_console_init(void);

#ifndef MAIN_LOOP_QUEUE_SIZE
#define MAIN_LOOP_QUEUE_SIZE 20
#endif /*MAIN_LOOP_QUEUE_SIZE*/

END_C_DECLS

#endif /*TK_MAIN_LOOP_CONSOLE_H*/

/**
 * File:   main_loop.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  main_loop interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_MAIN_LOOP_H
#define LFTK_MAIN_LOOP_H

#include "base/widget.h"

BEGIN_C_DECLS

struct _main_loop_t;
typedef struct _main_loop_t main_loop_t;

typedef ret_t (*main_loop_run_t)(main_loop_t* l);
typedef ret_t (*main_loop_quit_t)(main_loop_t* l);
typedef ret_t (*main_loop_destroy_t)(main_loop_t* l);
typedef ret_t (*main_loop_remove_timer_t)(main_loop_t* l, id_t id);
typedef id_t  (*main_loop_add_timer_t)(main_loop_t* l, uint32_t time_ms, event_handler on_event, void* ctx);

struct _main_loop_t {
  main_loop_run_t run;
  main_loop_quit_t quit;
  main_loop_destroy_t destroy;
  main_loop_add_timer_t add_timer;
  main_loop_remove_timer_t remove_timer;

  bool_t running;
};

main_loop_t* default_main_loop(void);

ret_t main_loop_run(main_loop_t* l);
ret_t main_loop_quit(main_loop_t* l);
ret_t main_loop_destroy(main_loop_t* l);
ret_t main_loop_remove_timer(main_loop_t* l, id_t id);
id_t  main_loop_add_timer(main_loop_t* l, uint32_t time_ms, event_handler on_event, void* ctx);

END_C_DECLS

#endif/*LFTK_MAIN_LOOP_H*/


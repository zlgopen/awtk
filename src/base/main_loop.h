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

struct _main_loop_t {
  main_loop_run_t run;
  main_loop_quit_t quit;
  main_loop_destroy_t destroy;

  bool_t running;
};

main_loop_t* main_loop_init(int w, int h);

main_loop_t* main_loop_get_default(void);
ret_t main_loop_set_default(main_loop_t* loop);

ret_t main_loop_run(main_loop_t* l);
ret_t main_loop_quit(main_loop_t* l);
ret_t main_loop_destroy(main_loop_t* l);

END_C_DECLS

#endif/*LFTK_MAIN_LOOP_H*/


/**
 * File:   lftk.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  lftk
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
 * 2018-03-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/lftk.h"
#include "base/timer.h"
#include "base/platform.h"
#include "base/main_loop.h"

ret_t lftk_init(wh_t w, wh_t h, uint32_t* heap, uint32_t size) {
  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);
  return_value_if_fail(mem_init(heap, size) == RET_OK, RET_FAIL);
  return_value_if_fail(main_loop_init(w, h) != NULL, RET_FAIL);

  return RET_OK;
}

ret_t lftk_run() {
  main_loop_run(main_loop_get_default());
  main_loop_destroy(main_loop_get_default());

  return RET_OK;
}

static ret_t lftk_quit_idle(const timer_info_t* timer) {
  return main_loop_quit(main_loop_get_default());
}

ret_t lftk_quit() { 
  timer_add(lftk_quit_idle, NULL, 0); 
	return RET_OK;
}

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

#include "lftk.h"
#include "base/mem.h"
#include "base/timer.h"
#include "base/platform.h"
#include "base/main_loop.h"
#include "font/font_bitmap.h"
#include "base/font_manager.h"
#include "base/image_manager.h"
#include "base/resource_manager.h"

#ifdef WITH_STB_FONT
#include "font/font_stb.h"
#endif /*WITH_STB_FONT*/

#ifdef WITH_STB_IMAGE
#include "image_loader/image_loader_stb.h"
#endif /*WITH_STB_IMAGE*/

static ret_t lftk_add_font(const resource_info_t* res) {
  if (res->subtype == RESOURCE_TYPE_FONT_BMP) {
    font_manager_add(default_fm(), font_bitmap_create(res->name, res->data, res->size));
#ifdef WITH_STB_FONT
  } else if (res->subtype == RESOURCE_TYPE_FONT_TTF) {
    font_manager_add(default_fm(), font_stb_create(res->name, res->data, res->size));
#endif /*WITH_STB_FONT*/
  } else {
    log_debug("not support font type\n");
  }

  return RET_OK;
}

ret_t lftk_init_resources() {
  uint32_t i = 0;
  uint32_t nr = 0;
  const resource_info_t** all = resource_manager_get_all(&nr);

  for (i = 0; i < nr; i++) {
    const resource_info_t* iter = all[i];
    switch (iter->type) {
      case RESOURCE_TYPE_FONT:
        lftk_add_font(iter);
        break;
      case RESOURCE_TYPE_THEME:
        theme_init(iter->data);
        break;
    }
  }

  return RET_OK;
}

ret_t lftk_init(wh_t w, wh_t h, uint32_t* heap, uint32_t size) {
  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);
  return_value_if_fail(mem_init(heap, size) == RET_OK, RET_FAIL);
  return_value_if_fail(main_loop_init(w, h) != NULL, RET_FAIL);

#ifdef WITH_STB_IMAGE
  image_manager_init(default_im(), image_loader_stb());
#endif /*WITH_STB_IMAGE*/

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

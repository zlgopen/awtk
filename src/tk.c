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

#include "tk.h"
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

static ret_t tk_add_font(const resource_info_t* res) {
  if (res->subtype == RESOURCE_TYPE_FONT_BMP) {
#ifdef WITH_BITMAP_FONT
    font_manager_add(font_manager(), font_bitmap_create(res->name, res->data, res->size));
#endif
  } else if (res->subtype == RESOURCE_TYPE_FONT_TTF) {
#ifdef WITH_STB_FONT
    font_manager_add(font_manager(), font_stb_create(res->name, res->data, res->size));
#endif /*WITH_STB_FONT*/
  } else {
    log_debug("not support font type:%d\n", res->subtype);
  }
  return RET_OK;
}

ret_t tk_init_resources() {
  uint32_t i = 0;
  uint32_t nr = resource_manager()->resources.size;
  const resource_info_t** all = (const resource_info_t**)(resource_manager()->resources.elms);

  for (i = 0; i < nr; i++) {
    const resource_info_t* iter = all[i];
    switch (iter->type) {
      case RESOURCE_TYPE_FONT:
        tk_add_font(iter);
        break;
      case RESOURCE_TYPE_THEME:
        theme_init(iter->data);
        break;
    }
  }

  return RET_OK;
}

ret_t tk_init(wh_t w, wh_t h, uint32_t* heap, uint32_t size) {
  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);
  return_value_if_fail(mem_init(heap, size) == RET_OK, RET_FAIL);
  return_value_if_fail(main_loop_init(w, h) != NULL, RET_FAIL);
 
  image_manager_set(image_manager_create());
  resource_manager_set(resource_manager_create(30));

#ifdef WITH_STB_IMAGE
  image_manager_init(image_manager(), image_loader_stb());
#endif /*WITH_STB_IMAGE*/

  return RET_OK;
}

ret_t tk_run() {
  main_loop_run(main_loop());
  main_loop_destroy(main_loop());
  resource_manager_destroy(resource_manager());

  return RET_OK;
}

static ret_t tk_quit_idle(const timer_info_t* timer) {
  return main_loop_quit(main_loop());
}

ret_t tk_quit() {
  timer_add(tk_quit_idle, NULL, 0);
  return RET_OK;
}

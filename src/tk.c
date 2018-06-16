/**
 * File:   awtk.c
 * Author: AWTK Develop Team
 * Brief:  awtk
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
 * 2018-03-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tk.h"
#include "base/mem.h"
#include "base/idle.h"
#include "base/time.h"
#include "base/timer.h"
#include "base/locale.h"
#include "base/platform.h"
#include "base/main_loop.h"
#include "font/font_bitmap.h"
#include "base/font_manager.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "base/widget_factory.h"
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
        if (theme()->data == NULL && strcmp(iter->name, "default") == 0) {
          theme_init(iter->data);
        }
        break;
    }
  }

  return RET_OK;
}

ret_t tk_init_internal(void) {
  image_loader_t* loader = NULL;
#ifdef WITH_STB_IMAGE
  loader = image_loader_stb();
#endif /*WITH_STB_IMAGE*/

  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);

  return_value_if_fail(timer_init(time_now_ms) == RET_OK, RET_FAIL);
  return_value_if_fail(idle_manager_set(idle_manager_create()) == RET_OK, RET_FAIL);
  return_value_if_fail(widget_factory_set(widget_factory_create()) == RET_OK, RET_FAIL);
  return_value_if_fail(resource_manager_set(resource_manager_create(30)) == RET_OK, RET_FAIL);
  return_value_if_fail(locale_set(locale_create(NULL, NULL)) == RET_OK, RET_FAIL);
  return_value_if_fail(font_manager_set(font_manager_create()) == RET_OK, RET_FAIL);
  return_value_if_fail(image_manager_set(image_manager_create(loader)) == RET_OK, RET_FAIL);
  return_value_if_fail(window_manager_set(window_manager_create()) == RET_OK, RET_FAIL);

  return RET_OK;
}

ret_t tk_init(wh_t w, wh_t h) {
  return_value_if_fail(tk_init_internal() == RET_OK, RET_FAIL);

  return main_loop_init(w, h) != NULL ? RET_OK : RET_FAIL;
}

ret_t tk_deinit_internal(void) {
  image_manager_destroy(image_manager());
  image_manager_set(NULL);

  font_manager_destroy(font_manager());
  font_manager_set(NULL);

  locale_destroy(locale());
  locale_set(NULL);

  resource_manager_destroy(resource_manager());
  resource_manager_set(NULL);

  idle_manager_destroy(idle_manager());
  idle_manager_set(NULL);

  timer_manager_destroy(timer_manager());
  timer_manager_set(NULL);

  widget_destroy(window_manager());
  window_manager_set(NULL);

  return RET_OK;
}

static ret_t tk_exit(void) {
  main_loop_destroy(main_loop());

  return tk_deinit_internal();
}

ret_t tk_run() {
  main_loop_run(main_loop());

  return tk_exit();
}

static ret_t tk_quit_idle(const timer_info_t* timer) {
  return main_loop_quit(main_loop());
}

ret_t tk_quit() {
  timer_add(tk_quit_idle, NULL, 0);
  return RET_OK;
}

ret_t tk_set_lcd_orientation(lcd_orientation_t orientation) {
  main_loop_t* loop = main_loop();
  system_info_t* info = system_info();
  return_value_if_fail(loop != NULL && info != NULL, RET_OK);
  assert(orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_0);
  return_value_if_fail(orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_0,
                       RET_NOT_IMPL);

  if (info->lcd_orientation != orientation) {
    wh_t w = info->lcd_w;
    wh_t h = info->lcd_h;
    lcd_t* lcd = loop->canvas.lcd;

    info->lcd_orientation = orientation;
    if (orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_270) {
      w = info->lcd_h;
      h = info->lcd_w;
    }

    lcd->w = w;
    lcd->h = h;
    window_manager_resize(window_manager(), w, h);
  }

  return RET_OK;
}

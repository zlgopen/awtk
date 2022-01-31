/**
 * File:   awtk.c
 * Author: AWTK Develop Team
 * Brief:  awtk
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "awtk.h"
#include "tkc/mem.h"
#include "tkc/fscript.h"
#include "base/idle.h"
#include "base/timer.h"
#include "tkc/thread.h"
#include "tkc/time_now.h"
#include "base/locale_info.h"
#include "tkc/platform.h"
#include "base/main_loop.h"
#include "base/font_manager.h"
#include "base/input_method.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "base/widget_factory.h"
#include "base/assets_manager.h"
#include "fscript_ext/fscript_ext.h"
#ifdef FSCRIPT_WITH_WIDGET
#include "fscript_ext/fscript_widget.h"
#endif /*FSCRIPT_WITH_WIDGET*/

#ifdef WITH_VGCANVAS
#include "base/vgcanvas_asset_manager.h"
#endif

#ifdef WITH_FSCRIPT_EXT
#ifndef WITH_DATA_READER_WRITER
#define WITH_DATA_READER_WRITER
#endif /*WITH_DATA_READER_WRITER*/
#endif /*WITH_FSCRIPT_EXT*/

#ifdef WITH_DATA_READER_WRITER
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"
#include "tkc/data_writer_file.h"
#include "tkc/data_writer_wbuffer.h"
#include "tkc/data_reader_file.h"
#include "tkc/data_reader_mem.h"
#include "base/data_reader_asset.h"
#endif /*WITH_DATA_READER_WRITER*/

#include "base/widget_animator_manager.h"
#include "font_loader/font_loader_bitmap.h"
#include "base/window_animator_factory.h"

#include "widgets/widgets.h"
#include "base/self_layouter_factory.h"
#include "base/children_layouter_factory.h"
#include "base/dialog_highlighter_factory.h"

#ifndef WITHOUT_LAYOUT
#include "layouters/self_layouter_builtins.h"
#include "layouters/children_layouter_builtins.h"
#endif /*WITHOUT_LAYOUT*/

#ifndef WITHOUT_WINDOW_ANIMATORS
#include "window_animators/window_animator_builtins.h"
#endif /*WITHOUT_WINDOW_ANIMATORS*/

#ifndef WITHOUT_DIALOG_HIGHLIGHTER
#include "dialog_highlighters/dialog_highlighter_builtins.h"
#endif /*WITHOUT_DIALOG_HIGHLIGHTER*/

#ifndef WITHOUT_CLIPBOARD
#ifdef WITH_SDL
#include "clip_board/clip_board_sdl.h"
#define clip_board_create clip_board_sdl_create
#else
#include "clip_board/clip_board_default.h"
#define clip_board_create clip_board_default_create
#endif /*WITH_SDL*/
#endif /*WITHOUT_CLIPBOARD*/

#ifdef WITH_TRUETYPE_FONT
#include "font_loader/font_loader_truetype.h"
#endif /*WITH_TRUETYPE_FONT*/

#ifdef WITH_STB_IMAGE
#include "image_loader/image_loader_stb.h"
#endif /*WITH_STB_IMAGE*/

#ifdef AWTK_WEB
#include "image_loader_web.h"
#endif /*AWTK_WEB*/

static uint64_t s_ui_thread_id = 0;

static ret_t tk_add_font(const asset_info_t* res) {
  if (res->subtype == ASSET_TYPE_FONT_BMP) {
#ifdef WITH_BITMAP_FONT
    font_manager_add_font(font_manager(), font_bitmap_create(res->name, res->data, res->size));
#endif
  } else if (res->subtype == ASSET_TYPE_FONT_TTF) {
#ifdef WITH_TRUETYPE_FONT
    font_manager_add_font(font_manager(), font_truetype_create(res->name, res->data, res->size));
#endif /*WITH_TRUETYPE_FONT*/
  } else {
    log_debug("not support font type:%d\n", res->subtype);
  }
  return RET_OK;
}

ret_t tk_init_assets(void) {
  uint32_t i = 0;
  uint32_t nr = assets_manager()->assets.size;
  const asset_info_t** all = (const asset_info_t**)(assets_manager()->assets.elms);

  for (i = 0; i < nr; i++) {
    const asset_info_t* iter = all[i];
    switch (iter->type) {
      case ASSET_TYPE_FONT:
        tk_add_font(iter);
        break;
      case ASSET_TYPE_STYLE:
        if (theme()->data == NULL && strcmp(iter->name, "default") == 0) {
          theme_set_theme_data(theme(), iter->data);
        }
        break;
    }
  }

  return RET_OK;
}

static ret_t tk_clear_cache_when_oom(uint32_t tried_times) {
  if (tried_times == 1) {
    image_manager_unload_unused(image_manager(), 10);
    font_manager_shrink_cache(font_manager(), 10);
  } else if (tried_times == 2) {
    image_manager_unload_unused(image_manager(), 0);
    font_manager_shrink_cache(font_manager(), 0);
  } else if (tried_times == 3) {
    event_t e = event_init(EVT_LOW_MEMORY, NULL);
    font_manager_unload_all(font_manager());
    widget_dispatch(window_manager(), &e);
  } else {
    event_t e = event_init(EVT_OUT_OF_MEMORY, NULL);
    widget_dispatch(window_manager(), &e);
  }

  return RET_OK;
}

static tk_semaphore_t* s_clear_cache_semaphore = NULL;

static ret_t exec_clear_cache(exec_info_t* info) {
  uint32_t tried_times = (uint32_t)tk_pointer_to_int(info->ctx);

  tk_clear_cache_when_oom(tried_times);
  tk_semaphore_post(s_clear_cache_semaphore);
  log_debug("clear cache: %u\n", tried_times);

  return RET_REMOVE;
}

static ret_t awtk_mem_on_out_of_memory(void* ctx, uint32_t tried_times, uint32_t need_size) {
  if (tk_is_ui_thread()) {
    tk_clear_cache_when_oom(tried_times);
  } else {
    event_queue_req_t req;
    memset(&req, 0x00, sizeof(req));
    req.exec_in_ui.e.type = REQ_EXEC_IN_UI;
    req.exec_in_ui.info.func = exec_clear_cache;
    req.exec_in_ui.info.ctx = tk_pointer_from_int(tried_times);

    log_debug("req clear cache begin: %u\n", tried_times);
    main_loop_queue_event(main_loop(), &req);
    tk_semaphore_wait(s_clear_cache_semaphore, 10000);
    log_debug("req clear cache done: %u\n", tried_times);
  }

  return RET_OK;
}

ret_t tk_init_internal(void) {
  font_loader_t* font_loader = NULL;

  s_ui_thread_id = tk_thread_self();
#ifndef WITHOUT_FSCRIPT
  fscript_global_init();
#endif
#ifdef WITH_FSCRIPT_EXT
  fscript_ext_init();
#ifdef FSCRIPT_WITH_WIDGET
  fscript_widget_register();
#endif /*FSCRIPT_WITH_WIDGET*/

#endif /*WITH_FSCRIPT_EXT*/

#ifdef WITH_DATA_READER_WRITER
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "asset", data_reader_asset_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);
#endif /*WITH_DATA_READER_WRITER*/

#ifdef WITH_STB_IMAGE
  image_loader_register(image_loader_stb());
#endif /*WITH_STB_IMAGE*/

#ifdef AWTK_WEB
  image_loader_register(image_loader_web());
#endif /*AWTK_WEB*/

#ifdef WITH_TRUETYPE_FONT
  font_loader = font_loader_truetype();
#elif defined(WITH_BITMAP_FONT)
  font_loader = font_loader_bitmap();
#endif /*WITH_TRUETYPE_FONT*/

  return_value_if_fail(timer_prepare(time_now_ms) == RET_OK, RET_FAIL);
  return_value_if_fail(idle_manager_set(idle_manager_create()) == RET_OK, RET_FAIL);
  return_value_if_fail(widget_factory_set(widget_factory_create()) == RET_OK, RET_FAIL);
  return_value_if_fail(theme_set(theme_default_create(NULL)) == RET_OK, RET_FAIL);
  return_value_if_fail(assets_manager_set(assets_manager_create(30)) == RET_OK, RET_FAIL);
#ifndef WITHOUT_INPUT_METHOD
  return_value_if_fail(input_method_set(input_method_create()) == RET_OK, RET_FAIL);
#endif /*WITHOUT_INPUT_METHOD*/
#ifdef WITH_VGCANVAS
  return_value_if_fail(vgcanvas_asset_manager_set(vgcanvas_asset_manager_create()) == RET_OK,
                       RET_FAIL);
#endif
  return_value_if_fail(locale_info_set(locale_info_create(NULL, NULL)) == RET_OK, RET_FAIL);
  return_value_if_fail(font_manager_set(font_manager_create(font_loader)) == RET_OK, RET_FAIL);
  return_value_if_fail(font_manager_set_assets_manager(font_manager(), assets_manager()) == RET_OK,
                       RET_FAIL);
  return_value_if_fail(image_manager_set(image_manager_create()) == RET_OK, RET_FAIL);
#ifndef WITHOUT_WINDOW_ANIMATORS
  return_value_if_fail(window_animator_factory_set(window_animator_factory_create()) == RET_OK,
                       RET_FAIL);
  window_animator_register_builtins();
#endif /*WITHOUT_WIDGET_ANIMATORS*/

#ifndef WITHOUT_DIALOG_HIGHLIGHTER
  return_value_if_fail(
      dialog_highlighter_factory_set(dialog_highlighter_factory_create()) == RET_OK, RET_FAIL);
#endif /*WITHOUT_DIALOG_HIGHLIGHTER*/

#ifndef WITHOUT_LAYOUT
  return_value_if_fail(children_layouter_factory_set(children_layouter_factory_create()) == RET_OK,
                       RET_FAIL);
  return_value_if_fail(self_layouter_factory_set(self_layouter_factory_create()) == RET_OK,
                       RET_FAIL);

  self_layouter_register_builtins();
  children_layouter_register_builtins();
#endif /*WITHOUT_LAYOUT*/

  return_value_if_fail(window_manager_set(window_manager_create()) == RET_OK, RET_FAIL);

#ifndef WITHOUT_CLIPBOARD
  return_value_if_fail(clip_board_set(clip_board_create()) == RET_OK, RET_FAIL);
#endif /*WITHOUT_CLIPBOARD*/

#ifndef WITHOUT_WIDGET_ANIMATORS
  return_value_if_fail(widget_animator_manager_set(widget_animator_manager_create()) == RET_OK,
                       RET_FAIL);
#endif /*WITHOUT_WIDGET_ANIMATORS*/

#ifndef WITHOUT_DIALOG_HIGHLIGHTER
  dialog_highlighter_register_builtins();
#endif /*WITHOUT_DIALOG_HIGHLIGHTER*/

  tk_widgets_init();
  tk_mem_set_on_out_of_memory(awtk_mem_on_out_of_memory, NULL);
  s_clear_cache_semaphore = tk_semaphore_create(0, "clear_cache");

  return RET_OK;
}

ret_t tk_init(wh_t w, wh_t h, app_type_t app_type, const char* app_name, const char* app_root) {
  main_loop_t* loop = NULL;
  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);
  return_value_if_fail(tk_mem_init_stage2() == RET_OK, RET_FAIL);
  ENSURE(system_info_init(app_type, app_name, app_root) == RET_OK);
  return_value_if_fail(tk_init_internal() == RET_OK, RET_FAIL);

  loop = main_loop_init(w, h);
  return_value_if_fail(loop != NULL, RET_FAIL);

  return RET_OK;
}

ret_t tk_deinit_internal(void) {
  idle_manager_dispatch(idle_manager());
#ifndef WITHOUT_CLIPBOARD
  clip_board_destroy(clip_board());
  clip_board_set(NULL);
#endif /*WITHOUT_CLIPBOARD*/

#ifndef WITHOUT_LAYOUT
  children_layouter_factory_destroy(children_layouter_factory());
  children_layouter_factory_set(NULL);

  self_layouter_factory_destroy(self_layouter_factory());
  self_layouter_factory_set(NULL);
#endif /*WITHOUT_LAYOUT*/

  image_manager_destroy(image_manager());
  image_manager_set(NULL);

  window_manager_close_all(window_manager());

  widget_factory_destroy(widget_factory());
  widget_factory_set(NULL);

  idle_manager_dispatch(idle_manager());

  window_manager_destroy(window_manager());
  window_manager_set(NULL);
#ifndef WITHOUT_INPUT_METHOD
  input_method_destroy(input_method());
  input_method_set(NULL);
#endif /*WITHOUT_INPUT_METHOD*/

#ifndef WITHOUT_DIALOG_HIGHLIGHTER
  dialog_highlighter_factory_destroy(dialog_highlighter_factory());
  dialog_highlighter_factory_set(NULL);
#endif /*WITHOUT_DIALOG_HIGHLIGHTER*/

#ifndef WITHOUT_WINDOW_ANIMATORS
  window_animator_factory_destroy(window_animator_factory());
  window_animator_factory_set(NULL);
#endif /*WITHOUT_WINDOW_ANIMATORS*/

#ifndef WITHOUT_WIDGET_ANIMATORS
  widget_animator_manager_destroy(widget_animator_manager());
  widget_animator_manager_set(NULL);
#endif /*WITHOUT_WIDGET_ANIMATORS*/

  timer_manager_destroy(timer_manager());
  timer_manager_set(NULL);

  idle_manager_destroy(idle_manager());
  idle_manager_set(NULL);

  theme_destroy(theme());
  theme_set(NULL);

  font_manager_destroy(font_manager());
  font_manager_set(NULL);

  locale_info_destroy(locale_info());
  locale_info_set(NULL);

  assets_manager_destroy(assets_manager());
  assets_manager_set(NULL);

#ifdef WITH_VGCANVAS
  vgcanvas_asset_manager_destroy(vgcanvas_asset_manager());
  vgcanvas_asset_manager_set(NULL);
#endif

#ifdef WITH_DATA_READER_WRITER
  data_writer_factory_destroy(data_writer_factory());
  data_reader_factory_destroy(data_reader_factory());
  data_writer_factory_set(NULL);
  data_reader_factory_set(NULL);
#endif /*WITH_DATA_READER_WRITER*/

  system_info_deinit();
#ifndef WITHOUT_FSCRIPT
  fscript_global_deinit();
#endif
  tk_semaphore_destroy(s_clear_cache_semaphore);

  return RET_OK;
}

ret_t tk_exit(void) {
  main_loop_destroy(main_loop());
  main_loop_set(NULL);

  return tk_deinit_internal();
}

ret_t tk_run() {
  main_loop_run(main_loop());

  return tk_exit();
}

static ret_t tk_quit_idle(const timer_info_t* timer) {
  main_loop_t* loop = main_loop();

  loop->app_quited = TRUE;

  return main_loop_quit(loop);
}

ret_t tk_quit() {
  timer_add(tk_quit_idle, NULL, 0);
  return RET_OK;
}

ret_t tk_set_lcd_orientation(lcd_orientation_t orientation) {
  main_loop_t* loop = main_loop();
  lcd_orientation_t old_orientation;
  system_info_t* info = system_info();
  return_value_if_fail(loop != NULL && info != NULL, RET_OK);

  if (info->lcd_orientation != orientation) {
    orientation_event_t e;
    old_orientation = info->lcd_orientation;
    system_info_set_lcd_orientation(info, orientation);

    orientation_event_init(&e, EVT_ORIENTATION_WILL_CHANGED, NULL, old_orientation, orientation);
    widget_dispatch(window_manager(), (event_t*)&e);
  }

  return RET_OK;
}

int32_t tk_get_pointer_x(void) {
  return window_manager_get_pointer_x(window_manager());
}

int32_t tk_get_pointer_y(void) {
  return window_manager_get_pointer_y(window_manager());
}

bool_t tk_is_pointer_pressed(void) {
  return window_manager_get_pointer_pressed(window_manager());
}

bool_t tk_is_ui_thread(void) {
  return s_ui_thread_id == tk_thread_self();
}

typedef struct _idle_callback_info_t {
  void* ctx;
  bool_t done;
  bool_t sync;
  ret_t result;
  tk_callback_t func;
} idle_callback_info_t;

static idle_callback_info_t* idle_callback_info_create(tk_callback_t func, void* ctx) {
  idle_callback_info_t* info = TKMEM_ZALLOC(idle_callback_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->func = func;
  info->ctx = ctx;

  return info;
}

static ret_t idle_callback_info_destroy(idle_callback_info_t* info) {
  return_value_if_fail(info != NULL && info->func != NULL, RET_BAD_PARAMS);

  memset(info, 0x00, sizeof(*info));
  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t idle_func_of_callback(const idle_info_t* info) {
  idle_callback_info_t* cinfo = (idle_callback_info_t*)(info->ctx);
  ret_t ret = cinfo->func(cinfo->ctx);

  if (cinfo->sync) {
    cinfo->done = TRUE;
    cinfo->result = ret;
    /*call by sync not allow repeat*/
    assert(ret != RET_REPEAT);
    return RET_REMOVE;
  } else {
    if (ret != RET_REPEAT) {
      idle_callback_info_destroy(cinfo);
      ret = RET_REMOVE;
    }

    return ret;
  }
}

ret_t tk_run_in_ui_thread(tk_callback_t func, void* ctx, bool_t wait_until_done) {
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  if (tk_is_ui_thread()) {
    return func(ctx);
  } else {
    idle_callback_info_t* info = idle_callback_info_create(func, ctx);
    return_value_if_fail(info != NULL, RET_OOM);

    info->sync = wait_until_done;
    if (idle_queue(idle_func_of_callback, info) == RET_OK) {
      ret_t ret = RET_OK;

      if (wait_until_done) {
        while (!(info->done)) {
          sleep_ms(20);
        }
        ret = info->result;
        idle_callback_info_destroy(info);
      }

      return ret;
    } else {
      idle_callback_info_destroy(info);
      return RET_FAIL;
    }
  }
}

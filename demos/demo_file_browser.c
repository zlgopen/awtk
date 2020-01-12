/**
 * File:   demo_file_browser.c
 * Author: AWTK Develop Team
 * Brief:  basic demo
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "ext_widgets/file_browser/file_chooser.h"

static ret_t on_quit(void* ctx, event_t* e) {
  tk_quit();
  return RET_OK;
}

static ret_t tk_on_choose_file_result(file_chooser_t* chooser) {
  widget_t* win = WIDGET(chooser->on_result_ctx);
  widget_t* cwd = widget_lookup(win, "cwd", TRUE);
  widget_t* filename = widget_lookup(win, "filename", TRUE);
  if (chooser->aborted) {
    widget_set_text_utf8(cwd, "aborted");
    widget_set_text_utf8(filename, "aborted");
  } else {
    widget_set_text_utf8(cwd, chooser->cwd.str);
    if(chooser->filename.str) {
      widget_set_text_utf8(filename, chooser->filename.str);
    } else {
      widget_set_text_utf8(filename, "");
    }
  }

  return RET_OK;
}

static ret_t on_file_save(void* ctx, event_t* e) {
  file_chooser_t* chooser = file_chooser_create("./", NULL);
  file_chooser_set_on_result(chooser, tk_on_choose_file_result, ctx);

  return file_chooser_choose_file_for_save(chooser);
}

static ret_t on_file_open(void* ctx, event_t* e) {
  file_chooser_t* chooser = file_chooser_create("./", NULL);
  file_chooser_set_on_result(chooser, tk_on_choose_file_result, ctx);

  return file_chooser_choose_file_for_open(chooser);
}

static ret_t on_choose_folder(void* ctx, event_t* e) {
  file_chooser_t* chooser = file_chooser_create("./", NULL);
  return_value_if_fail(chooser != NULL, RET_OOM);
  file_chooser_set_on_result(chooser, tk_on_choose_file_result, ctx);

  return file_chooser_choose_folder(chooser);
}

static ret_t on_manager(void* ctx, event_t* e) {
  file_chooser_t* chooser = file_chooser_create("./", NULL);
  return_value_if_fail(chooser != NULL, RET_OOM);
  file_chooser_set_on_result(chooser, tk_on_choose_file_result, ctx);

  return file_chooser_choose_folder(chooser);
}

ret_t application_init() {
  widget_t* win = NULL;
  tk_ext_widgets_init();

  win = window_open("file_browser");

  widget_child_on(win, "file_save", EVT_CLICK, on_file_save, win);
  widget_child_on(win, "file_open", EVT_CLICK, on_file_open, win);
  widget_child_on(win, "folder", EVT_CLICK, on_choose_folder, win);
  widget_child_on(win, "manager", EVT_CLICK, on_manager, win);
  widget_child_on(win, "quit", EVT_CLICK, on_quit, win);

  return RET_OK;
}

/**
 * File:   file_chooser.c
 * Author: AWTK Develop Team
 * Brief:  file/folder choosers
 *
 * Copyright (c) 2020 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget.h"
#include "base/dialog.h"
#include "base/window.h"
#include "file_browser/file_chooser.h"
#include "file_browser/file_browser_view.h"

file_chooser_t* file_chooser_create(void) {
  file_chooser_t* chooser = TKMEM_ZALLOC(file_chooser_t);
  return_value_if_fail(chooser != NULL, NULL);

  emitter_init(EMITTER(chooser));
  str_init(&(chooser->cwd), 0);
  str_init(&(chooser->filename), 0);
  return chooser;
}

ret_t file_chooser_set_init_dir(file_chooser_t* chooser, const char* init_dir) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);

  chooser->init_dir = tk_str_copy(chooser->init_dir, init_dir);

  return RET_OK;
}

ret_t file_chooser_set_filter(file_chooser_t* chooser, const char* filter) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);

  chooser->filter = tk_str_copy(chooser->filter, filter);

  return RET_OK;
}

file_chooser_t* file_chooser_cast(file_chooser_t* chooser) {
  return (file_chooser_t*)chooser;
}

static ret_t file_choose_on_click_to_close(void* ctx, event_t* e) {
  done_event_t done;
  widget_t* win = widget_get_window(WIDGET(e->target));
  file_chooser_t* chooser = (file_chooser_t*)ctx;

  chooser->aborted = TRUE;
  emitter_dispatch(EMITTER(chooser), done_event_init(&done, RET_OK));

  if (widget_is_dialog(win)) {
    dialog_quit(win, DIALOG_QUIT_CANCEL);
  } else {
    widget_close_window(win);
  }
  file_chooser_destroy(chooser);

  return RET_OK;
}

static ret_t file_choose_on_ok(void* ctx, event_t* e) {
  done_event_t done;
  widget_t* win = widget_get_window(WIDGET(e->target));
  file_chooser_t* chooser = (file_chooser_t*)ctx;
  widget_t* widget = widget_lookup_by_type(win, WIDGET_TYPE_FILE_BROWSER_VIEW, TRUE);
  widget_t* selected_file = widget_lookup(widget, FILE_BROWSER_VIEW_SELECTED_FILE, TRUE);

  str_set(&(chooser->cwd), file_browser_view_get_cwd(widget));
  if (selected_file != NULL) {
    str_from_wstr(&(chooser->filename), selected_file->text.str);
  }

  chooser->aborted = FALSE;
  if (emitter_dispatch(EMITTER(chooser), done_event_init(&done, RET_OK)) == RET_OK) {
    if (widget_is_dialog(win)) {
      dialog_quit(win, DIALOG_QUIT_OK);
    } else {
      widget_close_window(win);
    }
    file_chooser_destroy(chooser);
  }

  return RET_OK;
}

ret_t file_chooser_choose(file_chooser_t* chooser) {
  widget_t* win = window_open(chooser->ui);
  widget_t* file_browser_view = widget_lookup_by_type(win, WIDGET_TYPE_FILE_BROWSER_VIEW, TRUE);

  widget_child_on(win, FILE_CHOOSER_OK, EVT_CLICK, file_choose_on_ok, chooser);
  widget_child_on(win, FILE_CHOOSER_CANCEL, EVT_CLICK, file_choose_on_click_to_close, chooser);

  if (chooser->init_dir != NULL || chooser->filter != NULL) {
    if (chooser->filter != NULL) {
      file_browser_view_set_filter(file_browser_view, chooser->filter);
    }

    if (chooser->init_dir != NULL) {
      file_browser_view_set_init_dir(file_browser_view, chooser->init_dir);
    }
  }
  file_browser_view_reload(file_browser_view);

  if (widget_is_dialog(win)) {
    dialog_modal(win);
  }

  return RET_OK;
}

ret_t file_chooser_choose_file_for_save(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);

  chooser->ui = FILE_CHOOSER_UI_CHOOSE_FILE_FOR_SAVE;

  return file_chooser_choose(chooser);
}

ret_t file_chooser_choose_file_for_open(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);
  chooser->ui = FILE_CHOOSER_UI_CHOOSE_FILE_FOR_OPEN;

  return file_chooser_choose(chooser);
}

ret_t file_chooser_choose_folder(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);

  chooser->ui = FILE_CHOOSER_UI_CHOOSE_FOLDER;
  file_chooser_set_filter(chooser, STR_FILTER_DIR_ONLY);

  return file_chooser_choose(chooser);
}

const char* file_chooser_get_dir(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, NULL);

  return chooser->cwd.str;
}

const char* file_chooser_get_filename(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, NULL);

  return chooser->filename.str;
}

bool_t file_chooser_is_aborted(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, FALSE);

  return chooser->aborted;
}

ret_t file_chooser_destroy(file_chooser_t* chooser) {
  return_value_if_fail(chooser != NULL, RET_BAD_PARAMS);

  str_reset(&(chooser->cwd));
  str_reset(&(chooser->filename));

  TKMEM_FREE(chooser->filter);
  TKMEM_FREE(chooser->init_dir);
  emitter_deinit(EMITTER(chooser));

  TKMEM_FREE(chooser);

  return RET_OK;
}

/**
 * File:   file_dialog.c
 * Author: AWTK Develop Team
 * Brief: 文件对话框。
 *
 * Copyright (c) Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-05-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "ext_widgets/file_browser/file_chooser.h"

#if defined(WITH_SDL) && (!defined(LINUX_FB) && !defined(ANDROID) && !defined(IOS))
#define WITH_NATIVE_FILE_DIALOG 1
#endif /*WITH_SDL*/

#ifdef WITH_NATIVE_FILE_DIALOG
#include "../3rd/nativefiledialog/src/include/nfd.h"

static const char* filters_to_nfd(const char* filters, str_t* str) {
  str_set(str, filters);
  str_replace(str, ".", ",");
  str_trim(str, ",");

  return str->str;
}

static darray_t* tk_choose_files_native(const char* filters, const char* init_dir) {
  str_t str;
  nfdpathset_t pathSet;
  nfdresult_t result = NFD_OKAY;

  str_init(&str, 64);
  memset(&pathSet, 0x00, sizeof(pathSet));

  result = NFD_OpenDialogMultiple(filters_to_nfd(filters, &str), init_dir, &pathSet);
  str_reset(&str);

  if (result == NFD_OKAY) {
    size_t i = 0;
    size_t n = NFD_PathSet_GetCount(&pathSet);
    darray_t* ret = darray_create(n, default_destroy, NULL);
    for (i = 0; i < n; ++i) {
      nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
      char* iter = tk_strdup(path);
      if (iter != NULL) {
        darray_push(ret, iter);
      }
      log_debug("Path %i: %s\n", (int)i, path);
    }
    NFD_PathSet_Free(&pathSet);
    return ret;
  }

  return NULL;
}

static char* tk_choose_file_native(const char* filters, const char* init_dir) {
  str_t str;
  nfdchar_t* outPath = NULL;

  str_init(&str, 64);
  nfdresult_t result = NFD_OpenDialog(filters_to_nfd(filters, &str), init_dir, &outPath);
  str_reset(&str);

  if (result == NFD_OKAY) {
    char* ret = tk_strdup(outPath);
    log_debug("%s\n", outPath);
    free(outPath);
    return ret;
  } else if (result == NFD_CANCEL) {
    log_debug("%s\n", "User pressed cancel.");
  } else {
    log_warn("Error: %s\n", NFD_GetError());
  }

  return NULL;
}

static char* tk_choose_folder_native(const char* init_dir) {
  nfdchar_t* outPath = NULL;
  nfdresult_t result = NFD_PickFolder(init_dir, &outPath);

  if (result == NFD_OKAY) {
    char* ret = tk_strdup(outPath);
    log_debug("%s\n", outPath);
    free(outPath);
    return ret;
  } else if (result == NFD_CANCEL) {
    log_debug("%s\n", "User pressed cancel.");
  } else {
    log_warn("Error: %s\n", NFD_GetError());
  }

  return NULL;
}

static char* tk_choose_file_for_save_native(const char* filters, const char* init_dir) {
  str_t str;
  nfdchar_t* outPath = NULL;
  nfdresult_t result = NFD_OKAY;

  str_init(&str, 64);
  result = NFD_SaveDialog(filters_to_nfd(filters, &str), init_dir, &outPath);
  str_reset(&str);

  if (result == NFD_OKAY) {
    char* ret = tk_strdup(outPath);
    log_debug("%s\n", outPath);
    free(outPath);
    return ret;
  } else if (result == NFD_CANCEL) {
    log_debug("%s\n", "User pressed cancel.");
  } else {
    log_warn("Error: %s\n", NFD_GetError());
  }

  return NULL;
}
#endif /*WITH_NATIVE_FILE_DIALOG*/

static ret_t on_choose_file_result(void* ctx, event_t* e) {
  str_t* str = (str_t*)ctx;
  file_chooser_t* chooser = (file_chooser_t*)(e->target);

  if (!file_chooser_is_aborted(chooser)) {
    char full_path[MAX_PATH + 1];
    const char* dir = file_chooser_get_dir(chooser);
    const char* name = file_chooser_get_filename(chooser);
    path_build(full_path, sizeof(full_path) - 1, dir, name, NULL);

    str_set(str, full_path);
  }

  return RET_OK;
}

static char* tk_choose_file_awtk(const char* filters, const char* init_dir) {
  str_t str;
  file_chooser_t* chooser = file_chooser_create();

  str_init(&str, MAX_PATH + 1);
  emitter_on(EMITTER(chooser), EVT_DONE, on_choose_file_result, &str);
  file_chooser_set_init_dir(chooser, init_dir);
  file_chooser_set_filter(chooser, filters);
  if (file_chooser_choose_file_for_open(chooser) == RET_OK) {
    return str.str;
  } else {
    str_reset(&str);
    return NULL;
  }
}

static darray_t* tk_choose_files_awtk(const char* filters, const char* init_dir) {
  char* filename = tk_choose_file_awtk(filters, init_dir);
  if (filename != NULL) {
    darray_t* ret = darray_create(1, default_destroy, NULL);
    if (ret != NULL) {
      darray_push(ret, filename);
    } else {
      TKMEM_FREE(filename);
    }

    return ret;
  }

  return NULL;
}

static char* tk_choose_folder_awtk(const char* init_dir) {
  str_t str;
  file_chooser_t* chooser = file_chooser_create();

  str_init(&str, MAX_PATH + 1);
  emitter_on(EMITTER(chooser), EVT_DONE, on_choose_file_result, &str);
  file_chooser_set_init_dir(chooser, init_dir);

  if (file_chooser_choose_folder(chooser) == RET_OK) {
    return str.str;
  } else {
    str_reset(&str);
    return NULL;
  }
}

static char* tk_choose_file_for_save_awtk(const char* filters, const char* init_dir) {
  str_t str;
  file_chooser_t* chooser = file_chooser_create();

  str_init(&str, MAX_PATH + 1);
  emitter_on(EMITTER(chooser), EVT_DONE, on_choose_file_result, &str);
  file_chooser_set_init_dir(chooser, init_dir);
  file_chooser_set_filter(chooser, filters);

  if (file_chooser_choose_file_for_save(chooser) == RET_OK) {
    return str.str;
  } else {
    str_reset(&str);
    return NULL;
  }
}

static const char* fix_init_dir(char path[MAX_PATH + 1], const char* init_dir) {
  if (init_dir == NULL) {
    path_cwd(path);
  } else {
    path_normalize(init_dir, path, MAX_PATH);
  }

  return path;
}

darray_t* tk_choose_files(const char* filters, const char* init_dir) {
  char path[MAX_PATH + 1] = {0};
  init_dir = fix_init_dir(path, init_dir);

#ifdef WITH_NATIVE_FILE_DIALOG
  if (system_info()->app_type == APP_DESKTOP) {
    return tk_choose_files_native(filters, init_dir);
  }
#endif /*WITH_NATIVE_FILE_DIALOG*/

  return tk_choose_files_awtk(filters, init_dir);
}

char* tk_choose_file(const char* filters, const char* init_dir) {
  char path[MAX_PATH + 1] = {0};
  init_dir = fix_init_dir(path, init_dir);

#ifdef WITH_NATIVE_FILE_DIALOG
  if (system_info()->app_type == APP_DESKTOP) {
    return tk_choose_file_native(filters, init_dir);
  }
#endif /*WITH_NATIVE_FILE_DIALOG*/

  return tk_choose_file_awtk(filters, init_dir);
}

char* tk_choose_folder(const char* init_dir) {
  char path[MAX_PATH + 1] = {0};
  init_dir = fix_init_dir(path, init_dir);

#ifdef WITH_NATIVE_FILE_DIALOG
  if (system_info()->app_type == APP_DESKTOP) {
    return tk_choose_folder_native(init_dir);
  }
#endif /*WITH_NATIVE_FILE_DIALOG*/

  return tk_choose_folder_awtk(init_dir);
}

char* tk_choose_file_for_save(const char* filters, const char* init_dir) {
  char path[MAX_PATH + 1] = {0};
  init_dir = fix_init_dir(path, init_dir);

#ifdef WITH_NATIVE_FILE_DIALOG
  if (system_info()->app_type == APP_DESKTOP) {
    return tk_choose_file_for_save_native(filters, init_dir);
  }
#endif /*WITH_NATIVE_FILE_DIALOG*/

  return tk_choose_file_for_save_awtk(filters, init_dir);
}

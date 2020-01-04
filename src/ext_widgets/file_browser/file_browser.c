/**
 * File:   file_browser.c
 * Author: AWTK Develop Team
 * Brief:  file_browser
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
 * 2020-01-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/path.h"
#include "file_browser.h"

file_browser_t* file_browser_create(fs_t* fs) {
  file_browser_t* fb = NULL;
  return_value_if_fail(fs != NULL, NULL);
  fb = TKMEM_ZALLOC(file_browser_t);

  return_value_if_fail(fb != NULL, NULL);
  fb->fs = fs;

  return fb;
}

ret_t file_browser_set_cwd(file_browser_t* fb, const char* cwd) {
  char path[MAX_PATH + 1];
  return_value_if_fail(fb != NULL && cwd != NULL, RET_BAD_PARAMS);

  memset(path, 0x00, sizeof(path));
  path_normalize(cwd, path, MAX_PATH);
  str_set(&(fb->cwd), path);

  return RET_OK;
}

ret_t file_browser_create_dir(file_browser_t* fb, const char* name) {
  return RET_OK;
}

ret_t file_browser_remove_dir(file_browser_t* fb, const char* name) {
  return RET_OK;
}

ret_t file_browser_enter(file_browser_t* fb, const char* dir) {
  return_value_if_fail(fb != NULL && dir != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t file_browser_up(file_browser_t* fb) {
  return RET_OK;
}

ret_t file_browser_set_filter(file_browser_t* fb, tk_filter_t filter) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->filter = filter;

  return RET_OK;
}

ret_t file_browser_set_compare(file_browser_t* fb, tk_compare_t compare) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = compare;

  return RET_OK;
}

uint32_t file_browser_get_items_nr(file_browser_t* fb) {
  return 0;
}

filebrowser_item_t* file_browser_get_item(file_browser_t* fb, uint32_t index) {
  return NULL;
}

ret_t file_browser_destroy(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);
  darray_deinit(&(fb->items));
  str_reset(&(fb->cwd));
  TKMEM_FREE(fb);

  return RET_OK;
}

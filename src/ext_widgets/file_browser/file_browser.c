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
#include "tkc/utils.h"
#include "ext_widgets/file_browser/file_browser.h"

static ret_t file_browser_extend_items(file_browser_t* fb, uint32_t capacity) {
  fb_item_t* items = NULL;
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  if (fb->items_capacity >= capacity) {
    return RET_OK;
  }

  items = TKMEM_REALLOC(fb->items, capacity * sizeof(fb_item_t));
  if (items != NULL) {
    fb->items = items;
    fb->items_capacity = capacity;

    return RET_OK;
  } else {
    return RET_OOM;
  }
}

static fb_item_t* file_browser_add_item(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, NULL);

  if (fb->items_size >= fb->items_capacity) {
    uint32_t capacity = fb->items_capacity + fb->items_capacity / 2 + 10;
    if (file_browser_extend_items(fb, capacity) != RET_OK) {
      return NULL;
    }
  }

  if (fb->items_size < fb->items_capacity) {
    fb_item_t* item = fb->items + fb->items_size;
    memset(item, 0x00, sizeof(*item));
    fb->items_size++;
    return item;
  }

  return NULL;
}

static fb_item_t* file_browser_find_item(file_browser_t* fb, const char* name) {
  uint32_t i = 0;

  for (i = 0; i < fb->items_size; i++) {
    fb_item_t* iter = fb->items + i;
    if (tk_str_eq(iter->info.name, name)) {
      return iter;
    }
  }

  return NULL;
}

file_browser_t* file_browser_create(fs_t* fs) {
  file_browser_t* fb = NULL;
  return_value_if_fail(fs != NULL, NULL);
  fb = TKMEM_ZALLOC(file_browser_t);
  return_value_if_fail(fb != NULL, NULL);

  fb->fs = fs;
  tk_strcpy(fb->cwd, "/");
  wbuffer_init_extendable(&(fb->copy_items));

  emitter_init(EMITTER(fb));

  return fb;
}

ret_t file_browser_set_cwd(file_browser_t* fb, const char* cwd) {
  char path[MAX_PATH + 1];
  return_value_if_fail(fb != NULL && cwd != NULL, RET_BAD_PARAMS);
  assert(strlen(cwd) <= MAX_PATH);

  if (path_is_abs(cwd)) {
    tk_strncpy(fb->cwd, cwd, MAX_PATH);
  } else {
    memset(path, 0x00, sizeof(path));
    return_value_if_fail(path_abs(cwd, path, MAX_PATH) == RET_OK, RET_BAD_PARAMS);
    tk_strncpy(fb->cwd, path, MAX_PATH);
  }

  file_browser_refresh(fb);

  return RET_OK;
}

ret_t file_browser_create_dir(file_browser_t* fb, const char* name) {
  char fullpath[MAX_PATH + 1];
  fb_item_t* item = file_browser_find_item(fb, name);
  return_value_if_fail(item == NULL, RET_FOUND);

  memset(fullpath, 0x00, sizeof(fullpath));
  return_value_if_fail(fb != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path_build(fullpath, MAX_PATH, fb->cwd, name, NULL) == RET_OK, RET_FAIL);

  return fs_create_dir(fb->fs, fullpath);
}

ret_t file_browser_create_file(file_browser_t* fb, const char* name, const void* buff,
                               uint32_t size) {
  ret_t ret = RET_OK;
  fs_file_t* fp = NULL;
  char fullpath[MAX_PATH + 1];
  fb_item_t* item = file_browser_find_item(fb, name);
  return_value_if_fail(item == NULL || item->info.is_reg_file, RET_FOUND);

  memset(fullpath, 0x00, sizeof(fullpath));
  return_value_if_fail(fb != NULL && name != NULL && buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path_build(fullpath, MAX_PATH, fb->cwd, name, NULL) == RET_OK, RET_FAIL);

  fp = fs_open_file(fb->fs, fullpath, "wb+");
  return_value_if_fail(fp != NULL, RET_FAIL);

  if (fs_file_write(fp, buff, size) != size) {
    ret = RET_FAIL;
  }

  fs_file_close(fp);

  return ret;
}

static ret_t file_browser_remove_item_recursive(file_browser_t* fb, const char* fullpath) {
  fs_file_stat_t st;

  return fs_remove_dir(fb->fs, fullpath);
}

ret_t file_browser_remove(file_browser_t* fb, const char* name) {
  char fullpath[MAX_PATH + 1];

  memset(fullpath, 0x00, sizeof(fullpath));
  return_value_if_fail(fb != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path_build(fullpath, MAX_PATH, fb->cwd, name, NULL) == RET_OK, RET_FAIL);

  return file_browser_remove_item_recursive(fb, fullpath);
}

ret_t file_browser_copy(file_browser_t* fb, const char** names) {
  uint32_t i = 0;
  wbuffer_t* wb = NULL;
  return_value_if_fail(fb != NULL && names != NULL, RET_BAD_PARAMS);

  wb = &(fb->copy_items);
  wb->cursor = 0;
  return_value_if_fail(wbuffer_write_string(wb, fb->cwd) == RET_OK, RET_OOM);

  for (i = 0; names[i] != NULL; i++) {
    if (wbuffer_write_string(wb, names[i]) != RET_OK) {
      wb->cursor = 0;
      return RET_FAIL;
    }
  }

  return RET_OK;
}

ret_t file_browser_cut(file_browser_t* fb, const char** names) {
  return_value_if_fail(fb != NULL && names != NULL, RET_BAD_PARAMS);

  fb->cut = TRUE;
  return file_browser_copy(fb, names);
}

bool_t file_browser_can_paste(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, FALSE);

  return fb->copy_items.cursor > 0;
}

static ret_t file_browser_copy_item(file_browser_t* fb, const char* src, const char* dst,
                                    const char* name) {
  /*TODO*/
  return RET_NOT_IMPL;
}

ret_t file_browser_paste(file_browser_t* fb) {
  rbuffer_t rb;
  wbuffer_t* wb = NULL;
  const char* src = NULL;
  const char* name = NULL;
  return_value_if_fail(file_browser_can_paste(fb), RET_BAD_PARAMS);

  wb = &(fb->copy_items);
  rbuffer_init(&rb, wb->data, wb->cursor);
  return_value_if_fail(rbuffer_read_string(&rb, &src) == RET_OK, RET_BAD_PARAMS);

  while (rbuffer_read_string(&rb, &name) == RET_OK) {
    if (file_browser_copy_item(fb, src, fb->cwd, name) != RET_OK) {
      return RET_FAIL;
    } else if (fb->cut) {
      /*TODO remove files*/
    }
  }

  return RET_OK;
}

ret_t file_browser_refresh(file_browser_t* fb) {
  fs_item_t info;
  fs_dir_t* dir = NULL;
  return_value_if_fail(fb != NULL && fb->cwd[0] != '\0', RET_BAD_PARAMS);

  fb->items_size = 0;
  dir = fs_open_dir(fb->fs, fb->cwd);
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  while (fs_dir_read(dir, &info) == RET_OK) {
    fb_item_t* iter = NULL;
    if (tk_str_eq(info.name, ".") || tk_str_eq(info.name, "..")) {
      continue;
    }

    if(fb->filter != NULL && fb->filter(fb->filter_ctx, &info) == FALSE) {
      continue;
    }

    iter = file_browser_add_item(fb);
    return_value_if_fail(iter != NULL, RET_OOM);

    iter->info = info;
  }

  fs_dir_close(dir);

  return RET_OK;
}

ret_t file_browser_enter(file_browser_t* fb, const char* dir) {
  char path[MAX_PATH + 1];
  return_value_if_fail(fb != NULL && dir != NULL, RET_BAD_PARAMS);

  memset(path, 0x00, sizeof(path));
  return_value_if_fail(path_build(path, MAX_PATH, fb->cwd, dir, NULL) == RET_OK, RET_FAIL);

  return file_browser_set_cwd(fb, path);
}

ret_t file_browser_up(file_browser_t* fb) {
  char* p = NULL;
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  p = strrchr(fb->cwd, TK_PATH_SEP);
  if (p != NULL) {
    *p = '\0';
  }

  if (fb->cwd[0] == '\0') {
    fb->cwd[0] = '/';
  }

  return file_browser_refresh(fb);
}

ret_t file_browser_set_filter(file_browser_t* fb, tk_filter_t filter, void* ctx) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->filter = filter;
  fb->filter_ctx = ctx;

  return RET_OK;
}

ret_t file_browser_set_compare(file_browser_t* fb, tk_compare_t compare) {
  return_value_if_fail(fb != NULL && compare != NULL, RET_BAD_PARAMS);

  fb->compare = compare;

  return RET_OK;
}

static int compare_by_name(const void* a, const void* b) {
  return 0;
}

static int compare_by_size(const void* a, const void* b) {
  return 0;
}

static int compare_by_mtime(const void* a, const void* b) {
  return 0;
}

static int compare_by_name_dec(const void* a, const void* b) {
  return 0;
}

static int compare_by_size_dec(const void* a, const void* b) {
  return 0;
}

static int compare_by_mtime_dec(const void* a, const void* b) {
  return 0;
}

ret_t file_browser_sort(file_browser_t* fb) {
  /*TODO*/
  return RET_OK;
}

ret_t file_browser_sort_by_name(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? compare_by_name : compare_by_name_dec;

  return file_browser_sort(fb);
}

ret_t file_browser_sort_by_size(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? compare_by_size : compare_by_size_dec;

  return file_browser_sort(fb);
}

ret_t file_browser_sort_by_mtime(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? compare_by_mtime : compare_by_mtime_dec;

  return file_browser_sort(fb);
}

uint32_t file_browser_get_items_nr(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, 0);

  return fb->items_size;
}

fb_item_t* file_browser_get_item(file_browser_t* fb, uint32_t index) {
  return_value_if_fail(fb != NULL && index < fb->items_size, NULL);

  return fb->items + index;
}

ret_t file_browser_destroy(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  if (fb->items != NULL) {
    TKMEM_FREE(fb->items);
  }

  emitter_deinit(EMITTER(fb));
  wbuffer_deinit(&(fb->copy_items));
  TKMEM_FREE(fb);

  return RET_OK;
}

bool_t fb_filter_files_only(void* ctx, const void* data) {
  fs_item_t* item = (fs_item_t*)data;
  
  return item->is_reg_file;
}

bool_t fb_filter_directories_only(void* ctx, const void* data) {
  fs_item_t* item = (fs_item_t*)data;
  
  return item->is_dir;
}

bool_t fb_filter_by_ext_names(void* ctx, const void* data) {
  fs_item_t* item = (fs_item_t*)data;
  const char* ext_names = (const char*)ctx;
  const char* p = strrchr(item->name, '.');

  if(ext_names == NULL) {
    return TRUE;
  }

  if(p == NULL) {
    return FALSE;
  }

  return strstr(ext_names, p) != NULL;
}


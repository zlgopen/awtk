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

static ret_t file_browser_sort(file_browser_t* fb);

static ret_t file_browser_extend_items(file_browser_t* fb, uint32_t capacity) {
  fb_item_t* items = NULL;
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  if (fb->items_capacity >= capacity) {
    return RET_OK;
  }

  items = TKMEM_REALLOC(fb->items, capacity * sizeof(fb_item_t));
  if (items != NULL) {
    uint32_t delta = capacity - fb->items_capacity;

    fb->items = items;
    fb->items_capacity = capacity;

    memset(items + fb->items_size, 0x00, delta * sizeof(fb_item_t));

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
    fb->items_size++;
    return item;
  }

  return NULL;
}

static fb_item_t* file_browser_find_item(file_browser_t* fb, const char* name) {
  uint32_t i = 0;

  for (i = 0; i < fb->items_size; i++) {
    fb_item_t* iter = fb->items + i;
    if (tk_str_eq(iter->name, name)) {
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
  fb->ignore_hidden_files = TRUE;
  fb->compare = fb_compare_by_name;

  return fb;
}

ret_t file_browser_set_cwd(file_browser_t* fb, const char* cwd) {
  char path[MAX_PATH + 1];
  return_value_if_fail(fb != NULL && cwd != NULL, RET_BAD_PARAMS);
  assert(strlen(cwd) <= MAX_PATH);

  if (path_is_abs(cwd)) {
    path_normalize(cwd, fb->cwd, MAX_PATH);
  } else {
    memset(path, 0x00, sizeof(path));
    return_value_if_fail(path_abs(cwd, path, MAX_PATH) == RET_OK, RET_BAD_PARAMS);
    path_normalize(path, fb->cwd, MAX_PATH);
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
  return_value_if_fail(item == NULL || item->is_reg_file, RET_FOUND);

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
  fs_stat_info_t st;
  return_value_if_fail(fs_stat(fb->fs, fullpath, &st) == RET_OK, RET_BAD_PARAMS);

  if (st.is_reg_file) {
    return fs_remove_file(fb->fs, fullpath);
  } else if (st.is_dir) {
    fs_dir_t* dir = fs_open_dir(fb->fs, fullpath);
    if (dir != NULL) {
      fs_item_t info;
      char sub_path[MAX_PATH + 1];
      while (fs_dir_read(dir, &info) == RET_OK) {
        if (tk_str_eq(info.name, ".") || tk_str_eq(info.name, "..")) {
          continue;
        }

        ENSURE(path_build(sub_path, MAX_PATH, fullpath, info.name, NULL) == RET_OK);
        if (file_browser_remove_item_recursive(fb, sub_path) != RET_OK) {
          log_warn("file_browser_remove_item_recursive failed\n");
        }
      }
      fs_dir_close(dir);
    }

    return fs_remove_dir(fb->fs, fullpath);
  } else {
    assert(!"not supported");
    return fs_remove_file(fb->fs, fullpath);
  }
}

ret_t file_browser_remove(file_browser_t* fb, const char* name) {
  char fullpath[MAX_PATH + 1];

  memset(fullpath, 0x00, sizeof(fullpath));
  return_value_if_fail(fb != NULL && name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path_build(fullpath, MAX_PATH, fb->cwd, name, NULL) == RET_OK, RET_FAIL);

  return file_browser_remove_item_recursive(fb, fullpath);
}

ret_t file_browser_refresh(file_browser_t* fb) {
  fs_item_t info;
  fs_stat_info_t st;
  fs_dir_t* dir = NULL;
  char fullpath[MAX_PATH + 1];
  return_value_if_fail(fb != NULL && fb->cwd[0] != '\0', RET_BAD_PARAMS);

  fb->items_size = 0;
  dir = fs_open_dir(fb->fs, fb->cwd);
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  memset(fullpath, 0x00, sizeof(fullpath));
  while (fs_dir_read(dir, &info) == RET_OK) {
    fb_item_t* iter = NULL;
    if (tk_str_eq(info.name, ".") || tk_str_eq(info.name, "..")) {
      continue;
    }

    if (fb->ignore_hidden_files && info.name[0] == '.') {
      continue;
    }

    if (fb->filter != NULL && fb->filter(fb->filter_ctx, &info) == FALSE) {
      continue;
    }

    path_build(fullpath, MAX_PATH, fb->cwd, info.name, NULL);
    if (fs_stat(os_fs(), fullpath, &st) != RET_OK) {
      log_debug("stat file failed:%s\n", fullpath);
      continue;
    }

    iter = file_browser_add_item(fb);
    return_value_if_fail(iter != NULL, RET_OOM);

    iter->size = st.size;
    iter->mtime = st.mtime;
    iter->ctime = st.ctime;
    iter->mtime = st.mtime;
    iter->is_dir = st.is_dir;
    iter->is_reg_file = st.is_reg_file;
    iter->name = tk_str_copy(iter->name, info.name);
  }
  fs_dir_close(dir);

  file_browser_sort(fb);

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
  while (p != NULL && (p[1] == '\0' || (p[1] == '.' && p[2] == '\0'))) {
    *p = '\0';
    p = strrchr(fb->cwd, TK_PATH_SEP);
  }

  if (p != NULL) {
    *p = '\0';
  }

  if (fb->cwd[0] == '\0') {
    fb->cwd[0] = '/';
    fb->cwd[1] = '\0';
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
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = compare;

  return RET_OK;
}

static int fb_dir_first(fb_item_t* aa, fb_item_t* bb) {
  if (aa->is_dir) {
    return -1;
  } else if (bb->is_dir) {
    return 1;
  } else {
    return 0;
  }
}

int fb_compare_by_name(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return strcmp(aa->name, bb->name);
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_type(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if (aa->is_reg_file && bb->is_reg_file) {
    const char* atype = strrchr(aa->name, '.');
    const char* btype = strrchr(bb->name, '.');

    if (atype == NULL) {
      return -1;
    }

    if (btype == NULL) {
      return 1;
    }

    return strcasecmp(atype + 1, btype + 1);
  }

  if (aa->is_dir && bb->is_dir) {
    return strcmp(aa->name, bb->name);
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_size(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return aa->size - bb->size;
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_mtime(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return aa->mtime - bb->mtime;
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_name_dec(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return -strcmp(aa->name, bb->name);
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_type_dec(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if (aa->is_reg_file && bb->is_reg_file) {
    const char* atype = strrchr(aa->name, '.');
    const char* btype = strrchr(bb->name, '.');

    if (atype == NULL) {
      return 1;
    }

    if (btype == NULL) {
      return -1;
    }

    return -strcasecmp(atype + 1, btype + 1);
  }

  if (aa->is_dir && bb->is_dir) {
    return -strcmp(aa->name, bb->name);
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_size_dec(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return bb->size - aa->size;
  }

  return fb_dir_first(aa, bb);
}

int fb_compare_by_mtime_dec(const void* a, const void* b) {
  fb_item_t* aa = (fb_item_t*)a;
  fb_item_t* bb = (fb_item_t*)b;

  if ((aa->is_reg_file && bb->is_reg_file) || (aa->is_dir && bb->is_dir)) {
    return bb->mtime - aa->mtime;
  }

  return fb_dir_first(aa, bb);
}

static ret_t file_browser_sort(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  if (fb->compare != NULL && fb->items_size > 1) {
    qsort(fb->items, fb->items_size, sizeof(fb_item_t), fb->compare);
  }

  return RET_OK;
}

ret_t file_browser_sort_by_name(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? fb_compare_by_name : fb_compare_by_name_dec;

  return file_browser_sort(fb);
}

ret_t file_browser_sort_by_type(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? fb_compare_by_type : fb_compare_by_type_dec;

  return file_browser_sort(fb);
}

ret_t file_browser_sort_by_size(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? fb_compare_by_size : fb_compare_by_size_dec;

  return file_browser_sort(fb);
}

ret_t file_browser_sort_by_mtime(file_browser_t* fb, bool_t ascending) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->compare = ascending ? fb_compare_by_mtime : fb_compare_by_mtime_dec;

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

ret_t file_browser_set_ignore_hidden_files(file_browser_t* fb, bool_t ignore_hidden_files) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  fb->ignore_hidden_files = ignore_hidden_files;

  return RET_OK;
}

ret_t file_browser_destroy(file_browser_t* fb) {
  return_value_if_fail(fb != NULL, RET_BAD_PARAMS);

  if (fb->items != NULL) {
    uint32_t i = 0;
    log_debug("items_capacity=%u\n", fb->items_capacity);
    for (i = 0; i < fb->items_capacity; i++) {
      fb_item_t* iter = fb->items + i;
      TKMEM_FREE(iter->name);
    }
    TKMEM_FREE(fb->items);
  }

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

  if (item->is_dir) {
    return TRUE;
  }

  if (ext_names == NULL) {
    return TRUE;
  }

  if (p == NULL) {
    return FALSE;
  }

  return strstr(ext_names, p) != NULL;
}

/**
 * File:   romfs.c
 * Author: AWTK Develop Team
 * Brief:  romfs
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-07-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "conf_io/conf_node.h"
#include "conf_io/conf_ubjson.h"

typedef struct _romfs_fs_t {
  fs_t fs;
  uint32_t header_size;
  uint32_t body_size;
  const uint8_t* header_data;
  const uint8_t* body_data;
  conf_doc_t* doc;
} romfs_fs_t;

typedef struct _romfs_file_t {
  int32_t size;
  int32_t offset;
  const uint8_t* data;
} romfs_file_t;

static int32_t romfs_file_read(fs_file_t* file, void* buffer, uint32_t size) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);
  int32_t rsize = (fp->offset + size) <= fp->size ? size : (fp->size - fp->offset);

  memcpy(buffer, fp->data + fp->offset, rsize);
  fp->offset += rsize;

  return rsize;
}

static int32_t romfs_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
  return RET_NOT_IMPL;
}

static int32_t romfs_file_printf(fs_file_t* file, const char* const format_str, va_list vl) {
  return RET_NOT_IMPL;
}

static ret_t romfs_file_seek(fs_file_t* file, int32_t offset) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);

  fp->offset = offset;

  return RET_OK;
}

static int64_t romfs_file_tell(fs_file_t* file) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);

  return fp->offset;
}

static int64_t romfs_file_size(fs_file_t* file) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);

  return fp->size;
}

static ret_t romfs_file_stat(fs_file_t* file, fs_stat_info_t* fst) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);

  memset(fst, 0x00, sizeof(*fst));
  fst->size = fp->size;
  fst->is_reg_file = TRUE;

  return RET_OK;
}

static ret_t romfs_file_sync(fs_file_t* file) {
  return RET_NOT_IMPL;
}

static ret_t romfs_file_truncate(fs_file_t* file, int32_t size) {
  return RET_NOT_IMPL;
}

static bool_t romfs_file_eof(fs_file_t* file) {
  romfs_file_t* fp = (romfs_file_t*)(file->data);

  return fp->offset >= fp->size;
}

static ret_t romfs_file_close(fs_file_t* file) {
  TKMEM_FREE(file);
  return RET_OK;
}

typedef struct _romfs_dir_t {
  conf_node_t* first;
  conf_node_t* iter;
} romfs_dir_t;

static ret_t romfs_dir_rewind(fs_dir_t* dir) {
  romfs_dir_t* d = (romfs_dir_t*)(dir->data);

  d->iter = d->first;

  return RET_OK;
}

static ret_t romfs_dir_read(fs_dir_t* dir, fs_item_t* item) {
  romfs_dir_t* d = (romfs_dir_t*)(dir->data);

  memset(item, 0x00, sizeof(fs_item_t));
  if (d->iter != NULL) {
    value_t v;
    if (conf_node_get_child_value(d->iter, "name", &v) == RET_OK) {
      tk_strncpy(item->name, value_str(&v), MAX_PATH);
    }

    if (conf_node_get_child_value(d->iter, "type", &v) == RET_OK) {
      item->is_dir = tk_str_eq(value_str(&v), "dir");
      item->is_reg_file = tk_str_eq(value_str(&v), "file");
    }

    d->iter = d->iter->next;
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t romfs_dir_close(fs_dir_t* dir) {
  romfs_dir_t* d = (romfs_dir_t*)dir->data;

  TKMEM_FREE(d);
  TKMEM_FREE(dir);

  return RET_OK;
}

static const fs_file_vtable_t s_file_vtable = {.read = romfs_file_read,
                                               .write = romfs_file_write,
                                               .printf = romfs_file_printf,
                                               .seek = romfs_file_seek,
                                               .tell = romfs_file_tell,
                                               .size = romfs_file_size,
                                               .stat = romfs_file_stat,
                                               .sync = romfs_file_sync,
                                               .truncate = romfs_file_truncate,
                                               .eof = romfs_file_eof,
                                               .close = romfs_file_close};

static ret_t romfs_file_destory(romfs_file_t* fp) {
  TKMEM_FREE(fp);

  return RET_OK;
}

static fs_file_t* fs_file_create(romfs_file_t* fp) {
  fs_file_t* f = NULL;
  return_value_if_fail(fp != NULL, NULL);

  f = TKMEM_ZALLOC(fs_file_t);
  if (f != NULL) {
    f->vt = &s_file_vtable;
    f->data = fp;
  } else {
    romfs_file_destory(fp);
  }

  return f;
}

static conf_node_t* romfs_find_node(conf_node_t* node, const char* name) {
  value_t v;
  char curr_name[MAX_PATH + 1] = {0};
  const char* p = strchr(name, '/');
  conf_node_t* iter = conf_node_get_first_child(node);

  if (p == NULL) {
    p = strchr(name, '\\');
  }

  if (p != NULL) {
    tk_strncpy(curr_name, name, p - name);
    name = p + 1;
  } else {
    tk_strncpy(curr_name, name, sizeof(curr_name) - 1);
    name = NULL;
  }

  while (iter != NULL) {
    if (conf_node_get_child_value(iter, "name", &v) == RET_OK) {
      if (tk_str_eq(value_str(&v), curr_name)) {
        if (name == NULL) {
          return iter;
        } else {
          conf_node_t* children = conf_node_find_child(iter, "children");
          if (children != NULL) {
            return romfs_find_node(children, name);
          } else {
            return NULL;
          }
        }
      }
    }

    iter = iter->next;
  }

  return NULL;
}

static romfs_file_t* romfs_file_create(fs_t* fs, const char* name) {
  conf_node_t* node = NULL;
  romfs_fs_t* rfs = (romfs_fs_t*)fs;
  romfs_file_t* file = NULL;
  node = romfs_find_node(rfs->doc->root, name);

  if (node != NULL) {
    value_t v;
    file = TKMEM_ZALLOC(romfs_file_t);
    return_value_if_fail(file != NULL, NULL);

    file->offset = 0;
    if (conf_node_get_child_value(node, "size", &v) == RET_OK) {
      file->size = value_int(&v);
    }

    if (conf_node_get_child_value(node, "data", &v) == RET_OK) {
      file->data = rfs->body_data + value_uint32(&v);
      assert(value_uint32(&v) < rfs->body_size);
    }
  }

  return file;
}

static fs_file_t* romfs_open_file(fs_t* fs, const char* name, const char* mode) {
  return_value_if_fail(name != NULL && mode != NULL, NULL);

  return fs_file_create(romfs_file_create(fs, name));
}

static ret_t romfs_remove_file(fs_t* fs, const char* name) {
  return RET_NOT_IMPL;
}

static bool_t romfs_file_exist(fs_t* fs, const char* name) {
  fs_stat_info_t st;
  return_value_if_fail(name != NULL, FALSE);

  if (fs_stat(fs, name, &st) == RET_OK) {
    return st.is_reg_file;
  } else {
    return FALSE;
  }
}

static ret_t romfs_file_rename(fs_t* fs, const char* name, const char* new_name) {
  return RET_NOT_IMPL;
}

static const fs_dir_vtable_t s_dir_vtable = {
    .read = romfs_dir_read, .rewind = romfs_dir_rewind, .close = romfs_dir_close};

static romfs_dir_t* romfs_dir_create(fs_t* fs, const char* name) {
  romfs_dir_t* dir = NULL;
  romfs_fs_t* rfs = (romfs_fs_t*)fs;
  conf_node_t* node = romfs_find_node(rfs->doc->root, name);
  return_value_if_fail(node != NULL, NULL);
  node = conf_node_find_child(node, "children");
  return_value_if_fail(node != NULL, NULL);
  node = conf_node_get_first_child(node);

  dir = TKMEM_ZALLOC(romfs_dir_t);
  return_value_if_fail(dir != NULL, NULL);
  dir->first = node;
  dir->iter = node;

  return dir;
}

static fs_dir_t* romfs_open_dir(fs_t* fs, const char* name) {
  fs_dir_t* dir = NULL;
  romfs_dir_t* rdir = romfs_dir_create(fs, name);

  return_value_if_fail(rdir != NULL, NULL);
  dir = TKMEM_ZALLOC(fs_dir_t);

  if (dir != NULL) {
    dir->vt = &s_dir_vtable;
    dir->data = rdir;
  } else {
    TKMEM_FREE(rdir);
  }

  return dir;
}

static ret_t romfs_remove_dir(fs_t* fs, const char* name) {
  return RET_NOT_IMPL;
}

static ret_t romfs_change_dir(fs_t* fs, const char* name) {
  return RET_NOT_IMPL;
}

static ret_t romfs_create_dir(fs_t* fs, const char* name) {
  return RET_NOT_IMPL;
}

static bool_t romfs_dir_exist(fs_t* fs, const char* name) {
  fs_stat_info_t st;
  return_value_if_fail(name != NULL, FALSE);

  if (fs_stat(fs, name, &st) == RET_OK) {
    return st.is_dir;
  } else {
    return FALSE;
  }
}

static ret_t romfs_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  return romfs_file_rename(fs, name, new_name);
}

static int32_t romfs_get_file_size(fs_t* fs, const char* name) {
  fs_stat_info_t st;
  return_value_if_fail(name != NULL, FALSE);

  if (fs_stat(fs, name, &st) == RET_OK && st.is_reg_file) {
    return st.size;
  } else {
    return -1;
  }
}

static ret_t romfs_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb,
                                 int32_t* total_kb) {
  return RET_NOT_IMPL;
}

static ret_t romfs_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  strcpy(path, "/bin/app");
  return RET_OK;
}

static ret_t romfs_get_temp_path(fs_t* fs, char path[MAX_PATH + 1]) {
  return RET_NOT_IMPL;
}

static ret_t romfs_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]) {
  return RET_NOT_IMPL;
}

static ret_t romfs_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  return RET_NOT_IMPL;
}

static ret_t romfs_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  conf_node_t* node = NULL;
  romfs_fs_t* rfs = (romfs_fs_t*)fs;
  return_value_if_fail(rfs->doc != NULL && rfs->doc->root != NULL, RET_BAD_PARAMS);

  node = romfs_find_node(rfs->doc->root, name);
  if (node != NULL) {
    value_t v;
    memset(fst, 0x00, sizeof(fs_stat_info_t));

    if (conf_node_get_child_value(node, "size", &v) == RET_OK) {
      fst->size = value_int(&v);
    }

    if (conf_node_get_child_value(node, "type", &v) == RET_OK) {
      fst->is_dir = tk_str_eq(value_str(&v), "dir");
      fst->is_reg_file = tk_str_eq(value_str(&v), "file");
    }
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static romfs_fs_t s_romfs_fs = {.fs =
                                    {
                                        .open_file = romfs_open_file,
                                        .remove_file = romfs_remove_file,
                                        .file_exist = romfs_file_exist,
                                        .file_rename = romfs_file_rename,

                                        .open_dir = romfs_open_dir,
                                        .remove_dir = romfs_remove_dir,
                                        .create_dir = romfs_create_dir,
                                        .change_dir = romfs_change_dir,
                                        .dir_exist = romfs_dir_exist,
                                        .dir_rename = romfs_dir_rename,

                                        .get_file_size = romfs_get_file_size,
                                        .get_disk_info = romfs_get_disk_info,
                                        .get_cwd = romfs_get_cwd,
                                        .get_exe = romfs_get_exe,
                                        .get_user_storage_path = romfs_get_user_storage_path,
                                        .get_temp_path = romfs_get_temp_path,
                                        .stat = romfs_stat,
                                    },
                                .header_size = 0,
                                .body_size = 0,
                                .header_data = NULL,
                                .body_data = NULL,
                                .doc = NULL};

ret_t romfs_init(const uint8_t* header_data, uint32_t header_size, const uint8_t* body_data,
                 uint32_t body_size) {
  return_value_if_fail(header_data != NULL && body_data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_romfs_fs.doc == NULL, RET_BAD_PARAMS);

  s_romfs_fs.body_data = body_data;
  s_romfs_fs.header_data = header_data;
  s_romfs_fs.body_size = body_size;
  s_romfs_fs.header_size = header_size;

  s_romfs_fs.doc = conf_doc_load_ubjson(header_data, header_size);

  return RET_OK;
}

ret_t romfs_deinit(void) {
  return_value_if_fail(s_romfs_fs.doc != NULL, RET_BAD_PARAMS);

  conf_doc_destroy(s_romfs_fs.doc);
  s_romfs_fs.doc = NULL;

  return RET_OK;
}

fs_t* romfs_get(void) {
  return (fs_t*)&s_romfs_fs;
}

#if defined(WITH_ROMFS_AS_OS_FS) || defined(AWTK_WEB)
fs_t* os_fs(void) {
  return_value_if_fail(s_romfs_fs.doc != NULL, NULL);

  return (fs_t*)&s_romfs_fs;
}
#endif /*WITH_ROMFS_AS_OS_FS*/

/**
 * File:   romfs_maker.c
 * Author: AWTK Develop Team
 * Brief:  romfs maker
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
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/buffer.h"
#include "ubjson/ubjson_writer.h"

static ret_t make_romfs_one_level(const char* path, ubjson_writer_t* writer, uint32_t* offset,
                                  fs_file_t* fp);

static ret_t make_romfs_dir(const char* filename, const char* name, ubjson_writer_t* writer,
                            uint32_t* offset, fs_file_t* fp) {
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, "name", name);
  ubjson_writer_write_kv_str(writer, "type", "dir");
  ubjson_writer_write_kv_array_begin(writer, "children");
  make_romfs_one_level(filename, writer, offset, fp);
  ubjson_writer_write_array_end(writer);
  ubjson_writer_write_object_end(writer);

  return RET_OK;
}

static ret_t make_romfs_one_level(const char* path, ubjson_writer_t* writer, uint32_t* offset,
                                  fs_file_t* fp) {
  fs_item_t item;
  fs_t* fs = os_fs();
  fs_dir_t* dir = NULL;
  char filename[MAX_PATH + 1];
  return_value_if_fail(path != NULL && writer != NULL && offset != NULL, RET_BAD_PARAMS);

  dir = fs_open_dir(fs, path);
  return_value_if_fail(dir != NULL, RET_BAD_PARAMS);

  while (fs_dir_read(dir, &item) == RET_OK) {
    if (tk_str_eq(item.name, ".") || tk_str_eq(item.name, "..")) {
      continue;
    }

    path_build(filename, MAX_PATH, path, item.name, NULL);
    if (item.is_reg_file) {
      uint32_t size = 0;
      void* data = file_read(filename, &size);
      assert(data != NULL);
      assert(fs_file_write(fp, data, size) == size);
      TKMEM_FREE(data);

      ubjson_writer_write_object_begin(writer);
      ubjson_writer_write_kv_str(writer, "name", item.name);
      ubjson_writer_write_kv_int(writer, "size", size);
      ubjson_writer_write_kv_int(writer, "data", *offset);
      ubjson_writer_write_kv_str(writer, "type", "file");
      ubjson_writer_write_object_end(writer);
      *offset += size;
    } else if (item.is_dir) {
      make_romfs_dir(filename, item.name, writer, offset, fp);
    }
  }
  fs_dir_close(dir);

  return RET_OK;
}

static ret_t make_romfs(const char* path, const char* header_filename, const char* body_filename,
                        bool_t with_folder_name) {
  wbuffer_t wb;
  ubjson_writer_t ub;
  ret_t ret = RET_OK;
  fs_file_t* fp = NULL;
  ubjson_writer_t* writer = &ub;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);
  return_value_if_fail(header_filename != NULL && body_filename != NULL, RET_BAD_PARAMS);

  wbuffer_init_extendable(&wb);
  ubjson_writer_init(writer, (ubjson_write_callback_t)wbuffer_write_binary, &wb);

  log_debug("%s => %s\n", path, header_filename);

  fp = fs_open_file(os_fs(), body_filename, "wb+");
  if (fp != NULL) {
    uint32_t offset = 0;
    if (with_folder_name) {
      char name[MAX_PATH + 1] = {0};
      path_basename(path, name, sizeof(name) - 1);

      ubjson_writer_write_array_begin(writer);
      make_romfs_dir(path, name, writer, &offset, fp);
      ubjson_writer_write_array_end(writer);
    } else {
      ubjson_writer_write_array_begin(writer);
      ret = make_romfs_one_level(path, writer, &offset, fp);
      ubjson_writer_write_array_end(writer);
    }

    fs_file_close(fp);

    if (ret == RET_OK) {
      file_write(header_filename, wb.data, wb.cursor);
    }
  }

  wbuffer_deinit(&wb);

  return RET_OK;
}

int main(int argc, char* argv[]) {
  const char* path = NULL;
  const char* name = NULL;
  char header_filename[MAX_PATH + 1] = {0};
  char body_filename[MAX_PATH + 1] = {0};
  bool_t with_folder_name = FALSE;

  if (argc < 3) {
    log_info("Usage: %s dir name [with_folder_name]\n", argv[0]);
    return -1;
  }

  path = argv[1];
  name = argv[2];
  if (argc > 3) {
    with_folder_name = tk_atob(argv[3]);
  }

  tk_snprintf(header_filename, sizeof(header_filename), "%s.header", name);
  tk_snprintf(body_filename, sizeof(body_filename), "%s.body", name);

  make_romfs(path, header_filename, body_filename, with_folder_name);

  return 0;
}

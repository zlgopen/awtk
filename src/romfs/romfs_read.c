/**
 * File:   romfs_read.c
 * Author: AWTK Develop Team
 * Brief:  romfs read
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

#include "romfs.c"

static ret_t romfs_read(const char* filename, const char* output_filename) {
  fs_t* fs = romfs_get();
  fs_stat_info_t info;

  if (fs_stat(fs, filename, &info) == RET_OK) {
    if (info.is_reg_file) {
      uint32_t i = 0;
      uint32_t size = info.size;
      fs_file_t* fp = fs_open_file(fs, filename, "rb");
      uint8_t* buff1 = (uint8_t*)TKMEM_ALLOC(size);
      uint8_t* buff2 = (uint8_t*)TKMEM_ALLOC(size);

      /*test*/
      for (i = 0; i < size; i++) {
        ENSURE(fs_file_tell(fp) == i);
        ENSURE(fs_file_read(fp, buff1 + i, 1) == 1);
      }

      ENSURE(fs_file_eof(fp));
      ENSURE(fs_file_read(fp, buff2, size) == 0);
      ENSURE(fs_file_seek(fp, 0) == RET_OK);
      ENSURE(fs_file_tell(fp) == 0);
      ENSURE(fs_file_read(fp, buff2, size) == size);
      ENSURE(fs_file_eof(fp));

      ENSURE(memcmp(buff1, buff2, size) == 0);

      file_write(output_filename, buff2, size);
      TKMEM_FREE(buff1);
      TKMEM_FREE(buff2);
      fs_file_close(fp);
    } else {
      fs_dir_t* dir = fs_open_dir(fs, filename);
      if (dir != NULL) {
        str_t str;
        fs_item_t item;

        str_init(&str, 100);
        while (fs_dir_read(dir, &item) == RET_OK) {
          str_append_format(&str, MAX_PATH, "[%s] %s\n", item.is_reg_file ? "file" : "dir ",
                            item.name);
        }
        file_write(output_filename, str.str, str.size);
        str_reset(&str);
        fs_dir_close(dir);
      }
    }

    log_debug("%s => %s\n", filename, output_filename);
  } else {
    log_debug("not found %s\n", filename);
  }

  return RET_OK;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    log_debug("Usage: %s romfs filename output_filename\n", argv[0]);
    return 0;
  } else {
    uint32_t header_size = 0;
    uint8_t* header_data = NULL;
    uint32_t body_size = 0;
    uint8_t* body_data = NULL;
    char header_filename[MAX_PATH + 1];
    char body_filename[MAX_PATH + 1];
    const char* romfs = argv[1];
    const char* filename = argv[2];
    const char* output_filename = argv[3];

    tk_snprintf(header_filename, sizeof(header_filename) - 1, "%s.header", romfs);
    header_data = file_read(header_filename, &header_size);

    tk_snprintf(body_filename, sizeof(body_filename) - 1, "%s.body", romfs);
    body_data = file_read(body_filename, &body_size);

    if (header_data != NULL && body_data != NULL) {
      romfs_init(header_data, header_size, body_data, body_size);
    }

    romfs_read(filename, output_filename);

    TKMEM_FREE(header_data);
    TKMEM_FREE(body_data);
    romfs_deinit();
  }

  return 0;
}

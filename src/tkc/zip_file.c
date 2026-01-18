/**
 * File:   zip_file.c
 * Author: AWTK Develop Team
 * Brief:  zip file
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-19 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "miniz/miniz.h"
#include "miniz/miniz_zip.h"
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/buffer.h"

ret_t zip_file_extract(const char* zipfile, const char* dst_dir, const char* password) {
  mz_zip_archive zip;
  mz_uint i = 0;
  mz_uint nr = 0;
  ret_t ret = RET_OK;
  char filename[MAX_PATH + 1];
  char full_path[MAX_PATH + 1];

  memset(&zip, 0x00, sizeof(zip));
  return_value_if_fail(zipfile != NULL && dst_dir != NULL, RET_BAD_PARAMS);
  return_value_if_fail(mz_zip_reader_init_file(&zip, zipfile, 0), RET_BAD_PARAMS);
  nr = mz_zip_reader_get_num_files(&zip);

  for (i = 0; i < nr; i++) {
    int ret = mz_zip_reader_get_filename(&zip, i, filename, MAX_PATH);
    if (ret > 0) {
      filename[ret] = '\0';
    } else {
      ret = RET_FAIL;
      break;
    }

    path_build(full_path, MAX_PATH, dst_dir, filename, NULL);
    log_debug("%s => %s\n", filename, full_path);
    if (mz_zip_reader_is_file_a_directory(&zip, i)) {
      fs_create_dir_r(os_fs(), full_path);
    } else {
      if (!mz_zip_reader_extract_file_to_file(&zip, filename, full_path, 0)) {
        ret = RET_FAIL;
        break;
      }
    }
  }
  mz_zip_reader_end(&zip);

  return ret;
}

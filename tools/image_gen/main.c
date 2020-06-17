/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "image_gen.h"
#include "common/utils.h"
#include "base/image_manager.h"
#include "image_loader/image_loader_stb.h"

typedef struct _image_format_t {
  bool_t mono;
  bool_t require_bgra;
  bool_t enable_bgr565;
  bool_t enable_rgb565;
} image_format_t;

ret_t image_format_set(image_format_t* image_format, const wchar_t* format) {
  if (format != NULL) {
    if (wcsstr(format, L"mono")) {
      image_format->mono = TRUE;
    }

    if (wcsstr(format, L"bgra")) {
      image_format->require_bgra = TRUE;
    }

    if (wcsstr(format, L"bgr565")) {
      image_format->enable_bgr565 = TRUE;
    };

    if (wcsstr(format, L"rgb565")) {
      image_format->enable_rgb565 = TRUE;
    };
  }
  return RET_OK;
}

ret_t gen_one(const char* input_file, const char* output_file, const char* theme,
              image_format_t* image_format) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(input_file, output_file)) {
    bitmap_t image;
    uint32_t size = 0;
    uint8_t* buff = NULL;
    buff = (uint8_t*)read_file(input_file, &size);
    if (buff != NULL) {
      ret = stb_load_image(0, buff, size, &image, image_format->require_bgra,
                           image_format->enable_bgr565, image_format->enable_rgb565);
      if (ret == RET_OK) {
        ret = image_gen(&image, output_file, theme, image_format->mono);
      }
      TKMEM_FREE(buff);
    } else {
      ret = RET_FAIL;
    }
    if (ret != RET_OK) {
      GEN_ERROR(input_file);
    }
  }
  return ret;
}

static ret_t gen_floder(const char* in_flodername, const char* out_flodername, const char* theme,
                        image_format_t* image_format) {
  ret_t ret = RET_OK;
  fs_dir_t* dir = fs_open_dir(os_fs(), in_flodername);
  fs_item_t item;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file) {
      str_t str_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      str_replace(&str_name, ext_array, "");
      filter_name(str_name.str);
      str_append(&str_name, ".data");
      path_build(in_name, MAX_PATH, in_flodername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_flodername, str_name.str, NULL);
      ret = gen_one(in_name, out_name, theme, image_format);
      str_reset(&str_name);
      if (ret != RET_OK) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return RET_OK;
}

int wmain(int argc, wchar_t* argv[]) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  const wchar_t* format = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename (bgra|bgr565|rgb565|mono)\n", argv[0]);

    return 0;
  }

  if (argc > 3) {
    format = argv[3];
  }
  image_format_t image_format = {FALSE, FALSE, FALSE, FALSE};
  image_format_set(&image_format, format);

  str_t theme_name;
  str_init(&theme_name, 0);
  if (argc > 4) {
    str_from_wstr(&theme_name, argv[4]);
  }

  str_t in_file;
  str_t out_file;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);
  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_floder(in_filename, out_filename, theme_name.str, &image_format);
  } else if (in_stat_info.is_reg_file == TRUE) {
    gen_one(in_filename, out_filename, theme_name.str, &image_format);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&theme_name);

  return 0;
}

#include "common/main.inc"

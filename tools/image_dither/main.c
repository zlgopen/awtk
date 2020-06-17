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
 * 2020-05-26 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "image_dither.h"
#include "common/utils.h"
#include "base/image_manager.h"
#include "../image_gen/image_gen.h"
#include "image_loader/image_loader_stb.h"

typedef enum _output_format_t {
  OUTPUT_FORMAT_NONE,
  OUTPUT_FORMAT_PNG,
  OUTPUT_FORMAT_RES,
  OUTPUT_FORMAT_DATA,
} output_format_t;

bitmap_format_t get_image_format(const wchar_t* format) {
  if (format != NULL) {
    if (wcsstr(format, L"bgra")) {
      return BITMAP_FMT_BGRA8888;
    } else if (wcsstr(format, L"rgba")) {
      return BITMAP_FMT_RGBA8888;
    } else if (wcsstr(format, L"bgr565")) {
      return BITMAP_FMT_BGR565;
    } else if (wcsstr(format, L"rgb565")) {
      return BITMAP_FMT_RGB565;
    };
  }
  return BITMAP_FMT_NONE;
}

output_format_t get_output_format(const wchar_t* format) {
  if (format != NULL) {
    if (wcsstr(format, L"png")) {
      return OUTPUT_FORMAT_PNG;
    } else if (wcsstr(format, L"res")) {
      return OUTPUT_FORMAT_RES;
    } else if (wcsstr(format, L"data")) {
      return OUTPUT_FORMAT_DATA;
    };
  }
  return OUTPUT_FORMAT_NONE;
}

ret_t res_image_gen(str_t* out_file, bitmap_t* bitmap, const char* theme) {
  str_t temp_file;
  uint32_t size = 0;
  uint8_t* buff = NULL;
  ret_t ret = RET_FAIL;

  str_init(&temp_file, out_file->size + 5);

  str_append(&temp_file, out_file->str);
  str_append(&temp_file, ".png");

  image_dither_image_wirte_png_file(temp_file.str, bitmap);

  buff = (uint8_t*)read_file(temp_file.str, &size);
  if (buff != NULL) {
    ret = output_res_c_source(out_file->str, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_PNG, buff,
                              size);
  }

  file_remove(temp_file.str);

  TKMEM_FREE(buff);

  return ret;
}

ret_t gen_one(str_t* in_file, str_t* out_file, const char* theme, output_format_t output_format,
              bitmap_format_t image_format) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(in_file->str, out_file->str)) {
    bitmap_t bitmap;
    uint32_t size = 0;
    uint8_t* buff = NULL;
    buff = (uint8_t*)read_file(in_file->str, &size);
    if (buff != NULL) {
      if (output_format == OUTPUT_FORMAT_DATA) {
        ret = image_dither_load_image(buff, size, &bitmap, image_format);
        if (ret == RET_OK) {
          ret = image_gen(&bitmap, out_file->str, theme, FALSE);
        }
      } else {
        bitmap_format_t temp_image_format = BITMAP_FMT_RGBA8888;
        if (image_format == BITMAP_FMT_BGRA8888) {
          temp_image_format = BITMAP_FMT_BGRA8888;
        }

        ret = image_dither_load_image(buff, size, &bitmap, temp_image_format);
        if (ret == RET_OK) {
          if (output_format == OUTPUT_FORMAT_RES) {
            res_image_gen(out_file, &bitmap, theme);
          } else {
            image_dither_image_wirte_png_file(out_file->str, &bitmap);
          }
        }
      }

      bitmap_destroy(&bitmap);
      TKMEM_FREE(buff);
    } else {
      ret = RET_FAIL;
    }
    if (ret != RET_OK) {
      GEN_ERROR(in_file->str);
    }
  }
  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  str_t in_file;
  str_t out_file;

  str_t theme_name;

  const wchar_t* format = NULL;
  const wchar_t* str_output = NULL;
  bitmap_format_t image_format = BITMAP_FMT_NONE;
  output_format_t output_format = OUTPUT_FORMAT_NONE;

  platform_prepare();

  if (argc < 4) {
    printf("Usage: %S in_filename out_filename (png|res|data) (bgra|rgba|bgr565|rgb565) \n",
           argv[0]);

    return 0;
  }

  if (argc > 3) {
    str_output = argv[3];
  }

  output_format = get_output_format(str_output);
  if (output_format == OUTPUT_FORMAT_NONE) {
    printf("set (png|res|data) \n");
    return 0;
  }

  if (argc > 4) {
    format = argv[4];
  }

  image_format = get_image_format(format);
  if (image_format == BITMAP_FMT_NONE && output_format == OUTPUT_FORMAT_DATA) {
    printf("set (bgra|rgba|bgr565|rgb565) \n");
    return 0;
  }

  str_init(&theme_name, 0);
  if (argc > 5) {
    str_from_wstr(&theme_name, argv[5]);
  }

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  gen_one(&in_file, &out_file, theme_name.str, output_format, image_format);

  str_reset(&in_file);
  str_reset(&out_file);

  return 0;
}

#include "common/main.inc"

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

#include "tkc/mem.h"
#include "tkc/platform.h"
#include "common/utils.h"
#include "font_gen.h"
#include "font_loader/font_loader_bitmap.h"
#ifdef WITH_STB_FONT
#include "font_loader/font_loader_stb.h"
#else
#include "font_loader/font_loader_ft.h"
#endif /*WITH_STB_FONT*/

int wmain(int argc, wchar_t* argv[]) {
  uint32_t size = 0;
  bool_t mono = FALSE;
  font_t* font = NULL;
  char* str_buff = NULL;
  uint8_t* ttf_buff = NULL;
  uint32_t font_size = 20;
  const char* ttf_filename = NULL;
  const char* str_filename = NULL;
  const char* out_filename = NULL;
  const char* theme_name = NULL;

  platform_prepare();

  if (argc < 5) {
    printf("Usage: %S ttf_filename str_filename out_filename font_size [mono]\n", argv[0]);

    return 0;
  }

  font_size = tk_watoi(argv[4]);

  if (argc > 5 && tk_wstr_eq(argv[5], L"mono")) {
    mono = TRUE;
  }

  str_t str_theme = {0};
  if (argc > 6) {
    str_from_wstr(&str_theme, argv[6]);
    theme_name = str_theme.str;
  }

  str_t ttf_file;
  str_t str_file;
  str_t out_file;

  str_init(&ttf_file, 0);
  str_init(&str_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&ttf_file, argv[1]);
  str_from_wstr(&str_file, argv[2]);
  str_from_wstr(&out_file, argv[3]);

  ttf_filename = ttf_file.str;
  str_filename = str_file.str;
  out_filename = out_file.str;

  exit_if_need_not_update_for_infiles(out_filename, 2, ttf_filename, str_filename);

  ttf_buff = (uint8_t*)read_file(ttf_filename, &size);
  return_value_if_fail(ttf_buff != NULL, 0);
#ifdef WITH_STB_FONT
  if (mono) {
    assert(!"not support mono font");
  } else {
    font = font_stb_create("default", ttf_buff, size);
  }
#else
  if (mono) {
    font = font_ft_mono_create("default", ttf_buff, size);
  } else {
    font = font_ft_create("default", ttf_buff, size);
  }
#endif /*WITH_STB_FONT*/

  str_buff = read_file(str_filename, &size);
  return_value_if_fail(str_buff != NULL, 0);

  if (font != NULL) {
    font_gen(font, (uint16_t)font_size, str_buff, out_filename, theme_name);
  }

  TKMEM_FREE(ttf_buff);
  TKMEM_FREE(str_buff);

  str_reset(&ttf_file);
  str_reset(&str_file);
  str_reset(&out_file);
  str_reset(&str_theme);

  printf("done\n");

  return 0;
}

#include "common/main.inc"

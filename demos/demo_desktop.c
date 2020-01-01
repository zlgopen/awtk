/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "assets.h"
#include "ext_widgets.h"

ret_t application_init(void) {
  tk_ext_widgets_init();
  window_open("edit");

  return RET_OK;
}

#if defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else
int main(void) {
#endif
  char res_root[MAX_PATH + 1];
  char app_root[MAX_PATH + 1];
  path_app_root(app_root);
  memset(res_root, 0x00, sizeof(res_root));

  path_build(res_root, MAX_PATH, app_root, "demos", NULL);
  tk_init(800, 480, APP_DESKTOP, "AWTK Demo", res_root);
  system_info_set_default_font(system_info(), "default_full");

  assets_init();
  application_init();

  tk_run();

  return 0;
}

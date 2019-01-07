/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/mem.h"
#include "tkc/path.h"
#include "widgets/window.h"
#include "widgets/label.h"
#include "widgets/edit.h"
#include "base/system_info.h"

ret_t application_init(void) {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(win, 0, 0, 0, 0);
  widget_t* edit = edit_create(win, 0, 0, 0, 0);

  widget_set_text(label, L"hello awtk!");
  widget_set_self_layout_params(label, "0", "middle", "30%", "30");

  widget_set_self_layout_params(edit, "30%", "middle", "60%", "30");

  widget_layout(win);

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

  assets_init();
  application_init();

  tk_run();

  return 0;
}

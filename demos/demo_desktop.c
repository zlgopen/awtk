/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/mem.h"
#include "resource.h"
#include "base/system_info.h"
#include "base/window.h"
#include "base/label.h"

ret_t application_init(void) {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* label = label_create(win, 0, 0, 0, 0);

  widget_use_style(label, "big_green");
  widget_set_text(label, L"hello awtk!");
  widget_set_self_layout_params(label, "center", "middle", "50%", "30");

  widget_layout(win);

  return RET_OK;
}

#if defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else
int main(void) {
#endif

  system_info_init(APP_DESKTOP, "AWTK Demo");
  tk_init(800, 480);

  resource_init();
  application_init();

  tk_run();

  return 0;
}

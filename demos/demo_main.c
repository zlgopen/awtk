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
#include "tkc/fs.h"
#include "tkc/path.h"
#include "base/system_info.h"
#include "base/window_manager.h"

ret_t application_init(void);

#ifdef USE_GUI_MAIN
int gui_app_start(int lcd_w, int lcd_h) {
  tk_init(lcd_w, lcd_h, APP_MOBILE, NULL, NULL);
#else

#if defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  int32_t lcd_w = 320;
  int32_t lcd_h = 480;
#else
int main(int argc, char* argv[]) {
  int32_t lcd_w = 320;
  int32_t lcd_h = 480;

  if (argc >= 2) {
    lcd_w = tk_atoi(argv[1]);
  }
  if (argc >= 3) {
    lcd_h = tk_atoi(argv[2]);
  }
#endif

#if defined(WIN32)
#if !defined(NDEBUG)
  {
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w+t", stdout);
  }
#endif /*NDEBUG*/
#endif /*WIN32*/

  tk_init(lcd_w, lcd_h, APP_SIMULATOR, NULL, NULL);
#endif

//#define WITH_LCD_PORTRAIT 1
#if defined(USE_GUI_MAIN) && defined(WITH_LCD_PORTRAIT)
  if (lcd_w > lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#ifdef WITH_LCD_LANDSCAPE
  if (lcd_w < lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#ifdef WITH_FS_RES
  system_info_set_default_font(system_info(), "default_full");
#endif /*WITH_FS_RES*/

  log_debug("Build at: %s %s\n", __DATE__, __TIME__);
  assets_init();
  application_init();

#ifdef ENABLE_CURSOR
  window_manager_set_cursor(window_manager(), "cursor");
#endif /*ENABLE_CURSOR*/

  tk_run();

  return 0;
}

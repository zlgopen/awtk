#include "lftk.h"
#include "base/resource_manager.h"
#include "res/inc/fonts/ap.data"
#include "res/inc/fonts/default.data"
#include "res/inc/fonts/default_ttf.data"
#include "res/inc/images/bricks.data"
#include "res/inc/images/checked.data"
#include "res/inc/images/dialog_title.data"
#include "res/inc/images/earth.data"
#include "res/inc/images/green_btn_n.data"
#include "res/inc/images/green_btn_o.data"
#include "res/inc/images/green_btn_p.data"
#include "res/inc/images/info.data"
#include "res/inc/images/left_off.data"
#include "res/inc/images/left_on.data"
#include "res/inc/images/middle_off.data"
#include "res/inc/images/middle_on.data"
#include "res/inc/images/muted.data"
#include "res/inc/images/radio_checked.data"
#include "res/inc/images/radio_unchecked.data"
#include "res/inc/images/red_btn_n.data"
#include "res/inc/images/red_btn_o.data"
#include "res/inc/images/red_btn_p.data"
#include "res/inc/images/right_off.data"
#include "res/inc/images/right_on.data"
#include "res/inc/images/slider_bg.data"
#include "res/inc/images/slider_drag.data"
#include "res/inc/images/slider_drag_o.data"
#include "res/inc/images/slider_drag_p.data"
#include "res/inc/images/slider_fg.data"
#include "res/inc/images/slider_v_bg.data"
#include "res/inc/images/slider_v_fg.data"
#include "res/inc/images/unchecked.data"
#include "res/inc/images/unmuted.data"
#include "res/inc/theme/default.data"
#include "res/inc/ui/bottom.data"
#include "res/inc/ui/dialog1.data"
#include "res/inc/ui/dialog2.data"
#include "res/inc/ui/fade.data"
#include "res/inc/ui/top.data"
#include "res/inc/ui/window.data"
#include "res/inc/ui/window1.data"
#include "res/inc/ui/window2.data"

ret_t resource_init(void) {
  resource_manager_init(30);

  resource_manager_add(font_ap);
  resource_manager_add(font_default);
  resource_manager_add(font_default_ttf);
  resource_manager_add(image_bricks);
  resource_manager_add(image_checked);
  resource_manager_add(image_dialog_title);
  resource_manager_add(image_earth);
  resource_manager_add(image_green_btn_n);
  resource_manager_add(image_green_btn_o);
  resource_manager_add(image_green_btn_p);
  resource_manager_add(image_info);
  resource_manager_add(image_left_off);
  resource_manager_add(image_left_on);
  resource_manager_add(image_middle_off);
  resource_manager_add(image_middle_on);
  resource_manager_add(image_muted);
  resource_manager_add(image_radio_checked);
  resource_manager_add(image_radio_unchecked);
  resource_manager_add(image_red_btn_n);
  resource_manager_add(image_red_btn_o);
  resource_manager_add(image_red_btn_p);
  resource_manager_add(image_right_off);
  resource_manager_add(image_right_on);
  resource_manager_add(image_slider_bg);
  resource_manager_add(image_slider_drag);
  resource_manager_add(image_slider_drag_o);
  resource_manager_add(image_slider_drag_p);
  resource_manager_add(image_slider_fg);
  resource_manager_add(image_slider_v_bg);
  resource_manager_add(image_slider_v_fg);
  resource_manager_add(image_unchecked);
  resource_manager_add(image_unmuted);
  resource_manager_add(theme_default);
  resource_manager_add(ui_bottom);
  resource_manager_add(ui_dialog1);
  resource_manager_add(ui_dialog2);
  resource_manager_add(ui_fade);
  resource_manager_add(ui_top);
  resource_manager_add(ui_window);
  resource_manager_add(ui_window1);
  resource_manager_add(ui_window2);

  lftk_init_resources();
  return RET_OK;
}

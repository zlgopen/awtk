#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "res/inc/fonts/default.data"
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
#include "res/inc/images/question.data"
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
#include "res/inc/images/warn.data"
#include "res/inc/styles/default.data"
#endif

ret_t assets_init(void) {
  assets_manager_t* rm = assets_manager();

#ifdef WITH_FS_RES
  assets_manager_load(rm, ASSET_TYPE_THEME, "default");
  assets_manager_load(rm, ASSET_TYPE_FONT, "default_ttf");
#else
  assets_manager_add(rm, font_default);
  assets_manager_add(rm, image_bricks);
  assets_manager_add(rm, image_checked);
  assets_manager_add(rm, image_dialog_title);
  assets_manager_add(rm, image_earth);
  assets_manager_add(rm, image_green_btn_n);
  assets_manager_add(rm, image_green_btn_o);
  assets_manager_add(rm, image_green_btn_p);
  assets_manager_add(rm, image_info);
  assets_manager_add(rm, image_left_off);
  assets_manager_add(rm, image_left_on);
  assets_manager_add(rm, image_middle_off);
  assets_manager_add(rm, image_middle_on);
  assets_manager_add(rm, image_muted);
  assets_manager_add(rm, image_question);
  assets_manager_add(rm, image_radio_checked);
  assets_manager_add(rm, image_radio_unchecked);
  assets_manager_add(rm, image_red_btn_n);
  assets_manager_add(rm, image_red_btn_o);
  assets_manager_add(rm, image_red_btn_p);
  assets_manager_add(rm, image_right_off);
  assets_manager_add(rm, image_right_on);
  assets_manager_add(rm, image_slider_bg);
  assets_manager_add(rm, image_slider_drag);
  assets_manager_add(rm, image_slider_drag_o);
  assets_manager_add(rm, image_slider_drag_p);
  assets_manager_add(rm, image_slider_fg);
  assets_manager_add(rm, image_slider_v_bg);
  assets_manager_add(rm, image_slider_v_fg);
  assets_manager_add(rm, image_unchecked);
  assets_manager_add(rm, image_unmuted);
  assets_manager_add(rm, image_warn);
  assets_manager_add(rm, style_default);
#endif

  tk_init_resources();
  return RET_OK;
}

#include "tk.h"
#include "base/resource_manager.h"
#ifndef WITH_FS_RES
#include "res/inc/fonts/ap.data"
#include "res/inc/fonts/default.data"
#include "res/inc/fonts/default_ttf.data"
#include "res/inc/images/bg800x480.data"
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
#include "res/inc/images/message.data"
#include "res/inc/images/middle_off.data"
#include "res/inc/images/middle_on.data"
#include "res/inc/images/muted.data"
#include "res/inc/images/question.data"
#include "res/inc/images/radio_checked.data"
#include "res/inc/images/radio_unchecked.data"
#include "res/inc/images/red_btn_n.data"
#include "res/inc/images/red_btn_o.data"
#include "res/inc/images/red_btn_p.data"
#include "res/inc/images/rgb.data"
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
#include "res/inc/theme/default.data"
#include "res/inc/ui/animation.data"
#include "res/inc/ui/animator.data"
#include "res/inc/ui/basic.data"
#include "res/inc/ui/bottom.data"
#include "res/inc/ui/dialog1.data"
#include "res/inc/ui/dialog2.data"
#include "res/inc/ui/fade.data"
#include "res/inc/ui/htranslate.data"
#include "res/inc/ui/main.data"
#include "res/inc/ui/memtest.data"
#include "res/inc/ui/slide_view.data"
#include "res/inc/ui/slide_view_auto_play.data"
#include "res/inc/ui/slide_view_h.data"
#include "res/inc/ui/slide_view_v.data"
#include "res/inc/ui/top.data"
#include "res/inc/ui/vtranslate.data"
#endif

ret_t resource_init(void) {
  resource_manager_t* rm = resource_manager();

#ifdef WITH_FS_RES
  resource_manager_load(rm, RESOURCE_TYPE_THEME, "default");
  resource_manager_load(rm, RESOURCE_TYPE_FONT, "default_ttf");
#else
  resource_manager_add(rm, font_ap);
  resource_manager_add(rm, font_default);
  resource_manager_add(rm, font_default_ttf);
  resource_manager_add(rm, image_bg800x480);
  resource_manager_add(rm, image_bricks);
  resource_manager_add(rm, image_checked);
  resource_manager_add(rm, image_dialog_title);
  resource_manager_add(rm, image_earth);
  resource_manager_add(rm, image_green_btn_n);
  resource_manager_add(rm, image_green_btn_o);
  resource_manager_add(rm, image_green_btn_p);
  resource_manager_add(rm, image_info);
  resource_manager_add(rm, image_left_off);
  resource_manager_add(rm, image_left_on);
  resource_manager_add(rm, image_message);
  resource_manager_add(rm, image_middle_off);
  resource_manager_add(rm, image_middle_on);
  resource_manager_add(rm, image_muted);
  resource_manager_add(rm, image_question);
  resource_manager_add(rm, image_radio_checked);
  resource_manager_add(rm, image_radio_unchecked);
  resource_manager_add(rm, image_red_btn_n);
  resource_manager_add(rm, image_red_btn_o);
  resource_manager_add(rm, image_red_btn_p);
  resource_manager_add(rm, image_rgb);
  resource_manager_add(rm, image_right_off);
  resource_manager_add(rm, image_right_on);
  resource_manager_add(rm, image_slider_bg);
  resource_manager_add(rm, image_slider_drag);
  resource_manager_add(rm, image_slider_drag_o);
  resource_manager_add(rm, image_slider_drag_p);
  resource_manager_add(rm, image_slider_fg);
  resource_manager_add(rm, image_slider_v_bg);
  resource_manager_add(rm, image_slider_v_fg);
  resource_manager_add(rm, image_unchecked);
  resource_manager_add(rm, image_unmuted);
  resource_manager_add(rm, image_warn);
  resource_manager_add(rm, theme_default);
  resource_manager_add(rm, ui_animation);
  resource_manager_add(rm, ui_animator);
  resource_manager_add(rm, ui_basic);
  resource_manager_add(rm, ui_bottom);
  resource_manager_add(rm, ui_dialog1);
  resource_manager_add(rm, ui_dialog2);
  resource_manager_add(rm, ui_fade);
  resource_manager_add(rm, ui_htranslate);
  resource_manager_add(rm, ui_main);
  resource_manager_add(rm, ui_memtest);
  resource_manager_add(rm, ui_slide_view);
  resource_manager_add(rm, ui_slide_view_auto_play);
  resource_manager_add(rm, ui_slide_view_h);
  resource_manager_add(rm, ui_slide_view_v);
  resource_manager_add(rm, ui_top);
  resource_manager_add(rm, ui_vtranslate);
#endif

  tk_init_resources();
  return RET_OK;
}

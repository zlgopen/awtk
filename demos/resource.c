#include "lftk.h"
#include "base/resource_manager.h"
#include "res/src/fonts/font20.data"
#include "res/src/images/bricks.data"
#include "res/src/images/checked.data"
#include "res/src/images/dialog_title.data"
#include "res/src/images/earth.data"
#include "res/src/images/green_btn_n.data"
#include "res/src/images/green_btn_o.data"
#include "res/src/images/green_btn_p.data"
#include "res/src/images/info.data"
#include "res/src/images/radio_checked.data"
#include "res/src/images/radio_unchecked.data"
#include "res/src/images/red_btn_n.data"
#include "res/src/images/red_btn_o.data"
#include "res/src/images/red_btn_p.data"
#include "res/src/images/unchecked.data"
#include "res/src/theme/default.data"
#include "res/src/ui/dialog1.data"
#include "res/src/ui/dialog2.data"
#include "res/src/ui/window1.data"

ret_t resource_init() {
  resource_manager_init(15);

  resource_manager_add(font_font20);
  resource_manager_add(image_bricks);
  resource_manager_add(image_checked);
  resource_manager_add(image_dialog_title);
  resource_manager_add(image_earth);
  resource_manager_add(image_green_btn_n);
  resource_manager_add(image_green_btn_o);
  resource_manager_add(image_green_btn_p);
  resource_manager_add(image_info);
  resource_manager_add(image_radio_checked);
  resource_manager_add(image_radio_unchecked);
  resource_manager_add(image_red_btn_n);
  resource_manager_add(image_red_btn_o);
  resource_manager_add(image_red_btn_p);
  resource_manager_add(image_unchecked);
  resource_manager_add(theme_default);
  resource_manager_add(ui_dialog1);
  resource_manager_add(ui_dialog2);
  resource_manager_add(ui_window1);

  lftk_init_resources();
  return RET_OK;
}

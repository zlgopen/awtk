/**
 * File:   widgets.c
 * Author: AWTK Develop Team
 * Brief:  register widgets
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "base/widget_factory.h"

#include "base/window.h"
#include "base/dialog.h"
#ifndef AWTK_NOGUI
#include "widgets/image.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "widgets/slider.h"
#include "widgets/pages.h"
#include "widgets/popup.h"
#include "widgets/button_group.h"
#include "widgets/group_box.h"
#include "widgets/dialog_title.h"
#include "widgets/dialog_client.h"
#include "widgets/check_button.h"
#include "widgets/progress_bar.h"
#include "widgets/color_tile.h"
#include "widgets/clip_view.h"

#ifndef AWTK_LITE
#include "widgets/system_bar.h"
#include "widgets/calibration_win.h"
#include "widgets/dragger.h"
#include "widgets/tab_button.h"
#include "widgets/tab_control.h"
#include "widgets/row.h"
#include "widgets/grid.h"
#include "widgets/view.h"
#include "widgets/overlay.h"
#include "widgets/edit.h"
#include "widgets/column.h"
#include "widgets/app_bar.h"
#include "widgets/grid_item.h"
#include "widgets/combo_box.h"
#include "widgets/combo_box_item.h"
#include "widgets/tab_button_group.h"
#include "widgets/spin_box.h"
#include "widgets/digit_clock.h"
#endif /*AWTK_LITE*/
#endif /*AWTK_NOGUI*/

ret_t tk_widgets_init(void) {
  widget_factory_t* f = widget_factory();
  widget_factory_register(f, WIDGET_TYPE_DIALOG, dialog_create);
  widget_factory_register(f, WIDGET_TYPE_NORMAL_WINDOW, window_create);
#ifndef AWTK_NOGUI
  widget_factory_register(f, WIDGET_TYPE_DIALOG_TITLE, dialog_title_create);
  widget_factory_register(f, WIDGET_TYPE_DIALOG_CLIENT, dialog_client_create);
  widget_factory_register(f, WIDGET_TYPE_IMAGE, image_create);
  widget_factory_register(f, WIDGET_TYPE_BUTTON, button_create);
  widget_factory_register(f, WIDGET_TYPE_LABEL, label_create);
  widget_factory_register(f, WIDGET_TYPE_PROGRESS_BAR, progress_bar_create);
  widget_factory_register(f, WIDGET_TYPE_SLIDER, slider_create);
  widget_factory_register(f, WIDGET_TYPE_CHECK_BUTTON, check_button_create);
  widget_factory_register(f, WIDGET_TYPE_RADIO_BUTTON, check_button_create_radio);
  widget_factory_register(f, WIDGET_TYPE_PAGES, pages_create);
  widget_factory_register(f, WIDGET_TYPE_BUTTON_GROUP, button_group_create);
  widget_factory_register(f, WIDGET_TYPE_POPUP, popup_create);
  widget_factory_register(f, WIDGET_TYPE_COLOR_TILE, color_tile_create);
  widget_factory_register(f, WIDGET_TYPE_CLIP_VIEW, clip_view_create);
  widget_factory_register(f, WIDGET_TYPE_GROUP_BOX, group_box_create);
#ifndef AWTK_LITE
  widget_factory_register(f, WIDGET_TYPE_SYSTEM_BAR, system_bar_create);
  widget_factory_register(f, WIDGET_TYPE_SYSTEM_BAR_BOTTOM, system_bar_bottom_create);
  widget_factory_register(f, WIDGET_TYPE_CALIBRATION_WIN, calibration_win_create);
  widget_factory_register(f, WIDGET_TYPE_VIEW, view_create);
  widget_factory_register(f, WIDGET_TYPE_OVERLAY, overlay_create);
  widget_factory_register(f, WIDGET_TYPE_EDIT, edit_create);
  widget_factory_register(f, WIDGET_TYPE_TAB_CONTROL, tab_control_create);
  widget_factory_register(f, WIDGET_TYPE_TAB_BUTTON, tab_button_create);
  widget_factory_register(f, WIDGET_TYPE_TAB_BUTTON_GROUP, tab_button_group_create);
  widget_factory_register(f, WIDGET_TYPE_SPIN_BOX, spin_box_create);
  widget_factory_register(f, WIDGET_TYPE_DRAGGER, dragger_create);
  widget_factory_register(f, WIDGET_TYPE_COMBO_BOX, combo_box_create);
  widget_factory_register(f, WIDGET_TYPE_COMBO_BOX_ITEM, combo_box_item_create);
  widget_factory_register(f, WIDGET_TYPE_GRID, grid_create);
  widget_factory_register(f, WIDGET_TYPE_GRID_ITEM, grid_item_create);
  widget_factory_register(f, WIDGET_TYPE_ROW, row_create);
  widget_factory_register(f, WIDGET_TYPE_COLUMN, column_create);
  widget_factory_register(f, WIDGET_TYPE_APP_BAR, app_bar_create);
  widget_factory_register(f, WIDGET_TYPE_DIGIT_CLOCK, digit_clock_create);
#endif /*AWTK_LITE*/
#endif /**AWTK_NOGUI*/
  return RET_OK;
}

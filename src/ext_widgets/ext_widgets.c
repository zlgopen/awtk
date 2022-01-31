/**
 * File:   ext_widgets.c
 * Author: AWTK Develop Team
 * Brief:  ext widgets
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"

#if defined(WITH_SDL) || defined(LINUX)
#ifndef WITH_WIDGET_VPAGE
#define WITH_WIDGET_VPAGE
#endif /*WITH_WIDGET_VPAGE*/
#endif /*WITH_SDL*/

#include "ext_widgets.h"
#include "vpage/vpage.h"
#include "switch/switch.h"
#include "gauge/gauge.h"
#include "mledit/mledit.h"
#include "features/draggable.h"
#include "mledit/line_number.h"
#include "gauge/gauge_pointer.h"
#include "gif_image/gif_image.h"
#include "svg_image/svg_image.h"
#include "keyboard/keyboard.h"
#include "keyboard/candidates.h"
#include "keyboard/lang_indicator.h"
#include "base/widget_factory.h"
#include "rich_text/rich_text.h"
#include "rich_text/rich_text_view.h"
#include "slide_menu/slide_menu.h"
#include "image_value/image_value.h"
#include "time_clock/time_clock.h"
#include "scroll_view/list_item.h"
#include "scroll_view/list_view.h"
#include "slide_view/slide_view.h"
#include "scroll_view/scroll_bar.h"
#include "scroll_view/scroll_view.h"
#include "scroll_view/list_view_h.h"
#include "combo_box_ex/combo_box_ex.h"
#include "color_picker/color_picker.h"
#include "scroll_label/hscroll_label.h"
#include "slide_view/slide_indicator.h"
#include "canvas_widget/canvas_widget.h"
#include "text_selector/text_selector.h"
#include "color_picker/color_component.h"
#include "progress_circle/progress_circle.h"
#include "image_animation/image_animation.h"
#include "mutable_image/mutable_image.h"
#include "base/children_layouter_factory.h"
#include "scroll_view/children_layouter_list_view.h"

#if defined(WITH_FS_RES) || defined(WITH_FS)
#include "file_browser/file_browser_view.h"
#endif /*WITH_FS*/

static ret_t children_layouter_register_ext_builtins(void) {
  children_layouter_factory_t* f = children_layouter_factory();

  children_layouter_factory_register(f, CHILDREN_LAYOUTER_LIST_VIEW,
                                     children_layouter_list_view_create);

  return RET_OK;
}

ret_t tk_ext_widgets_init(void) {
  widget_factory_t* f = widget_factory();

  FACTORY_TABLE_BEGIN(s_ext_widgets)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_RICH_TEXT, rich_text_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_RICH_TEXT_VIEW, rich_text_view_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_COLOR_PICKER, color_picker_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_COLOR_COMPONENT, color_component_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SCROLL_VIEW, scroll_view_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_LIST_VIEW, list_view_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_LIST_VIEW_H, list_view_h_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_LIST_ITEM, list_item_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SCROLL_BAR, scroll_bar_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SCROLL_BAR_DESKTOP, scroll_bar_create_desktop)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SCROLL_BAR_MOBILE, scroll_bar_create_mobile)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SLIDE_VIEW, slide_view_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SLIDE_INDICATOR, slide_indicator_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SLIDE_INDICATOR_ARC, slide_indicator_create_arc)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_KEYBOARD, keyboard_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_LANG_INDICATOR, lang_indicator_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_CANDIDATES, candidates_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_TIME_CLOCK, time_clock_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_GAUGE, gauge_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_GAUGE_POINTER, gauge_pointer_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_TEXT_SELECTOR, text_selector_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SWITCH, switch_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_IMAGE_ANIMATION, image_animation_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_PROGRESS_CIRCLE, progress_circle_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SVG_IMAGE, svg_image_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_GIF_IMAGE, gif_image_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_CANVAS_WIDGET, canvas_widget_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_IMAGE_VALUE, image_value_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_SLIDE_MENU, slide_menu_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_MUTABLE_IMAGE, mutable_image_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_MLEDIT, mledit_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_LINE_NUMBER, line_number_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_HSCROLL_LABEL, hscroll_label_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_COMBO_BOX_EX, combo_box_ex_create)
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_DRAGGABLE, draggable_create)

  FACTORY_TABLE_ENTRY("guage", gauge_create)
  FACTORY_TABLE_ENTRY("guage_pointer", gauge_pointer_create)

#ifdef WITH_WIDGET_VPAGE
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_VPAGE, vpage_create)
#endif /*WITH_WIDGET_VPAGE*/

#ifdef TK_FILE_BROWSER_VIEW_H
  FACTORY_TABLE_ENTRY(WIDGET_TYPE_FILE_BROWSER_VIEW, file_browser_view_create)
#endif /*TK_FILE_BROWSER_VIEW_H*/
  FACTORY_TABLE_END()

  children_layouter_register_ext_builtins();

  return widget_factory_register_multi(f, s_ext_widgets);
}
